
//=---------------------------------------------------------------------=
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
// 
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
// 
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=



#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif

#ifndef __ImplAAFMobSlot_h__
#include "ImplAAFMobSlot.h"
#endif

#ifndef __ImplAAFFile_h__
#include "ImplAAFFile.h"
#endif

#ifndef __ImplEnumAAFMobSlots_h__
#include "ImplEnumAAFMobSlots.h"
#endif

#ifndef __ImplAAFOperationGroup_h__
#include "ImplAAFOperationGroup.h"
#endif

#ifndef __ImplAAFTaggedValue_h__
#include "ImplAAFTaggedValue.h"
#endif

#ifndef __ImplEnumAAFTaggedValues_h__
#include "ImplEnumAAFTaggedValues.h"
#endif

#ifndef __ImplAAFKLVData_h__
#include "ImplAAFKLVData.h"
#endif

#ifndef __ImplEnumAAFKLVData_h__
#include "ImplEnumAAFKLVData.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"



#ifndef __ImplAAFMob_h__
#include "ImplAAFMob.h"
#endif

#include "ImplAAFHeader.h"
#include "ImplAAFContentStorage.h"
#include "ImplAAFObjectCreation.h"
#include "ImplAAFTimelineMobSlot.h"
#include "ImplAAFSourceClip.h"
#include "ImplAAFSequence.h"
#include "ImplAAFPulldown.h"
#include "ImplAAFFindSourceInfo.h"
#include "ImplAAFDictionary.h"
#include "AAFDataDefs.h"
#include "ImplEnumAAFMobSlots.h"
#include "ImplEnumAAFComponents.h"

#include <assert.h>
#include <wchar.h>
#include "AAFResult.h"
#include "aafCvt.h"
#include "AAFUtils.h"

#include "ImplAAFBuiltinDefs.h"
#include "ImplAAFSmartPointer.h"
typedef ImplAAFSmartPointer<ImplAAFDataDef> ImplAAFDataDefSP;


extern "C" const aafClassID_t CLSID_EnumAAFMobSlots;
extern "C" const aafClassID_t CLSID_EnumAAFTaggedValues;
extern "C" const aafClassID_t CLSID_AAFFindSourceInfo;
extern "C" const aafClassID_t CLSID_AAFTypeDefString;
extern "C" const aafClassID_t CLSID_EnumAAFKLVData;

ImplAAFMob::ImplAAFMob ()
: _mobID(			PID_Mob_MobID,			L"MobID"),
  _name(			PID_Mob_Name,			L"Name"),
  _creationTime(    PID_Mob_CreationTime,	L"CreationTime"),
  _lastModified(    PID_Mob_LastModified,	L"LastModified"),
  _slots(			PID_Mob_Slots,			L"Slots"),
  _userComments(	PID_Mob_UserComments,	L"UserComments"),
  _KLVData(			PID_Mob_KLVData,		L"KLVData")
{
	_persistentProperties.put(_mobID.address());
	_persistentProperties.put(_name.address());
	_persistentProperties.put(_creationTime.address());
	_persistentProperties.put(_lastModified.address());
	_persistentProperties.put(_slots.address());
	_persistentProperties.put(_userComments.address());
	_persistentProperties.put(_KLVData.address());
	(void)aafMobIDNew(&_mobID);		// Move this out of constructor when we get 2-stage create
	AAFGetDateTime(&_creationTime);
	AAFGetDateTime(&_lastModified);
}


ImplAAFMob::~ImplAAFMob ()
{
	// Release all of the mob slot pointers.
	size_t count = _slots.count();
	for (size_t i = 0; i < count; i++)
	{
		ImplAAFMobSlot *pSlot = _slots.clearValueAt(i);
		if (pSlot)
		{
		  pSlot->ReleaseReference();
		  pSlot = 0;
		}
	}

	if(_userComments.isPresent())
	{
		count = _userComments.count();
		for (size_t j = 0; j < count; j++)
		{
			ImplAAFTaggedValue* pTaggedValue = _userComments.clearValueAt(j);
			if (pTaggedValue)
			  pTaggedValue->ReleaseReference();
			pTaggedValue = 0;
		}
	}
	if(_KLVData.isPresent())
	{
		count = _KLVData.count();
		for (size_t j = 0; j < count; j++)
		{
			ImplAAFKLVData* pKLVData = _KLVData.clearValueAt(j);
			if (pKLVData)
			  pKLVData->ReleaseReference();
			pKLVData = 0;
		}
	}
}

//****************
// AppendSlot()
//
AAFRESULT STDMETHODCALLTYPE
	ImplAAFMob::AppendSlot
        (ImplAAFMobSlot *  pSlot)  //@parm [in,out] Mob Name length
{
	if (NULL == pSlot)
		return AAFRESULT_NULL_PARAM;
  if (pSlot->attached ())
    return AAFRESULT_OBJECT_ALREADY_ATTACHED;

	_slots.appendValue(pSlot);
	pSlot->AcquireReference();
	return AAFRESULT_SUCCESS;
}

//****************
// PrependSlot()
//
AAFRESULT STDMETHODCALLTYPE
	ImplAAFMob::PrependSlot
        (ImplAAFMobSlot *  pSlot)  //@parm [in,out] Mob Name length
{
	if (NULL == pSlot)
		return AAFRESULT_NULL_PARAM;
  if (pSlot->attached ())
    return AAFRESULT_OBJECT_ALREADY_ATTACHED;

	_slots.prependValue(pSlot);
	pSlot->AcquireReference();
	return AAFRESULT_SUCCESS;
}

//****************
// InsertSlotAt()
//
AAFRESULT STDMETHODCALLTYPE
	ImplAAFMob::InsertSlotAt
        (aafUInt32 index,          //@parm [in] index to insert
		 ImplAAFMobSlot *  pSlot)  //@parm [in] Mob Name length
{
	if (NULL == pSlot)
		return AAFRESULT_NULL_PARAM;
  if (pSlot->attached ())
    return AAFRESULT_OBJECT_ALREADY_ATTACHED;

	if (index > _slots.count()) // we can "insert" one after the end
	  return AAFRESULT_BADINDEX;

	_slots.insertAt(pSlot, index);
	pSlot->AcquireReference();
	return AAFRESULT_SUCCESS;
}

//****************
// RemoveSlotAt()
//
AAFRESULT STDMETHODCALLTYPE
	ImplAAFMob::RemoveSlotAt
        (aafUInt32 index)  //@parm [in] index of slot to remove
{
	if (index >= _slots.count())
	  return AAFRESULT_BADINDEX;

	ImplAAFMobSlot *pSlot = NULL;
  pSlot = _slots.removeAt(index);
  if (pSlot)
  {
    // We have removed an element from a "stong reference container" so we must
    // decrement the objects reference count. This will not delete the object
    // since the caller must have alread acquired a reference. (transdel 2000-MAR-10)
    pSlot->ReleaseReference ();
  }

  return AAFRESULT_SUCCESS;
}

//****************
// GetSlotAt()
//
AAFRESULT STDMETHODCALLTYPE
	ImplAAFMob::GetSlotAt
        (aafUInt32 index,          //@parm [in] index to insert
		 ImplAAFMobSlot ** ppSlot)  //@parm [out] returned slot
{
	AAFRESULT aafError = AAFRESULT_SUCCESS;

	if (NULL == ppSlot)
		return AAFRESULT_NULL_PARAM;


	if (_slots.find (index, *ppSlot))
  {
    assert (*ppSlot); // It is an internal DM programming error if this assertion fails!
    (*ppSlot)->AcquireReference ();
  }
  else
  {
	  aafError = AAFRESULT_BADINDEX;
  }

	return aafError;
}

//****************
// LookupSlot()
//
AAFRESULT STDMETHODCALLTYPE
	ImplAAFMob::LookupSlot
        (aafSlotID_t slotId,          //@parm [in] ID of slot to get
		 ImplAAFMobSlot ** ppSlot)  //@parm [out] returned slot
{
	return FindSlotBySlotID (slotId, ppSlot);
}

//****************
// RemoveSlot()
//
AAFRESULT STDMETHODCALLTYPE
	ImplAAFMob::RemoveSlot
        (ImplAAFMobSlot *  pSlot)  //@parm [in,out] Mob Name length
{
	if (NULL == pSlot)
		return AAFRESULT_NULL_PARAM;
  if (!pSlot->attached ()) // slot could not possibly be in _slots container.
    return AAFRESULT_OBJECT_NOT_ATTACHED;

  size_t index;
  if (_slots.findIndex (pSlot, index))
	  return RemoveSlotAt (index);
  else
    return AAFRESULT_OBJECT_NOT_FOUND;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::GetMobID (aafMobID_t *pMobID)
{
	XPROTECT()
	  {
		/* This will support both 1.x and 2.x */
		if (pMobID)
		  {
			*pMobID = _mobID;
		  }
		else
		  {
			RAISE(AAFRESULT_NULL_PARAM);
		  }
	  }

	XEXCEPT
	  {
	  }
	XEND;

	return(AAFRESULT_SUCCESS);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::GetName (aafCharacter *pName,
	aafUInt32 bufSize)
{
	bool stat;

	if(pName == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(!_name.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	else
		stat = _name.copyToBuffer(pName, bufSize);
	
	if (! stat)
	{
	  return AAFRESULT_SMALLBUF;	// Shouldn't the API have a length parm?
	}

	return(AAFRESULT_SUCCESS); 
}

//****************
// GetNameBufLen()
//
AAFRESULT STDMETHODCALLTYPE
ImplAAFMob::GetNameBufLen
        (aafUInt32 *  pSize)  //@parm [in,out] Mob Name length
{
	if(pSize == NULL)
		return(AAFRESULT_NULL_PARAM);
	
	if(!_name.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	else
	{
		*pSize = _name.size();
		return(AAFRESULT_SUCCESS); 
	}
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::GetModTime (aafTimeStamp_t *lastModified)
{
  // Validate input pointer...
  if (NULL == lastModified)
    return (AAFRESULT_NULL_PARAM);

	*lastModified = _lastModified;
	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::GetCreateTime (aafTimeStamp_t *creationTime)
{
  // Validate input pointer...
  if (NULL == creationTime)
    return (AAFRESULT_NULL_PARAM);

	*creationTime = _creationTime;

	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::GetMobInfo (aafTimeStamp_t *lastModified,
                           aafTimeStamp_t *creationTime,
							aafCharacter *name,
							aafUInt32 nameLen)
{
  // Validate input pointers...
  if (NULL == lastModified || NULL == creationTime || NULL == name)
    return (AAFRESULT_NULL_PARAM);

    
  XPROTECT()
	{
		// Assign from local copies so that we do not change the
    // contents of the input parameters if there is a failure.
    // This is the documented behavior for output parameters.
    aafTimeStamp_t modified, created;

		CHECK(GetCreateTime (&created));
		CHECK(GetModTime (&modified));
		CHECK(GetName(name, nameLen));

		*lastModified = modified;
		*creationTime = created;
	}
	XEXCEPT
	XEND;

	return(AAFRESULT_SUCCESS); 
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::CountSlots (aafNumSlots_t *pNumSlots)
{
  // Validate input pointer...
  if (NULL == pNumSlots)
    return (AAFRESULT_NULL_PARAM);
	
	*pNumSlots = _slots.count();

	return(AAFRESULT_SUCCESS);
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::SetModTime (aafTimeStamp_constref modTime)
{
	_lastModified = modTime;

	return(AAFRESULT_SUCCESS);
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::SetCreateTime (aafTimeStamp_constref createTime)
{
	_creationTime = createTime;

	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::SetMobID (aafMobID_constref newMobID)
{
	AAFRESULT				hr = AAFRESULT_SUCCESS;
	ImplAAFMob				*mobPtr = NULL;
	ImplAAFHeader			*head = NULL;
	ImplAAFContentStorage	*cstore = NULL;

	XPROTECT()
	{
	  if (inFile())
		{
		hr = MyHeadObject(&head);
		if(hr == AAFRESULT_SUCCESS)
		{			
			hr = head->GetContentStorage(&cstore);
			if (hr == AAFRESULT_SUCCESS)
			{
				// Does a mob with the new ID already exist?  If so, return error
				hr= cstore->LookupMob(newMobID, &mobPtr) ;
				if(hr== AAFRESULT_SUCCESS)
				{
					mobPtr->ReleaseReference();
					mobPtr = NULL;
					RAISE(AAFRESULT_DUPLICATE_MOBID);
				}	
				else if(hr== AAFRESULT_MOB_NOT_FOUND)
				{
					_mobID = newMobID;
					CHECK(cstore->ChangeIndexedMobID (this, newMobID));
				}
				else
					RAISE(hr);
				
				cstore->ReleaseReference();
				cstore = NULL;
			}

			head->ReleaseReference();
			head = NULL;
		}
		else
			RAISE(hr);
	  }
    else
		 _mobID = newMobID;

	} /* XPROTECT */
	XEXCEPT
	{
		if (cstore)
		{
			cstore->ReleaseReference();
			cstore = NULL;
		}
		if (head)
		{
			head->ReleaseReference();
			head = NULL;
		}

	}
	XEND;

	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::SetName (const aafCharacter *pName)
{	
	if(pName == NULL)
		return(AAFRESULT_NULL_PARAM);

	_name = pName;

	return(AAFRESULT_SUCCESS); 
}

// skip virtual AAFRESULT Verify(char *buf, validateData_t *result);
// What doe's this do?

//****************
// GetMobKind()
//
AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::GetMobKind (aafMobKind_t *pMobKind)
{
	if(pMobKind == NULL)
		return(AAFRESULT_NULL_PARAM);
	*pMobKind = kAAFAllMob;				// Abstract superclass, only match "all"
	return AAFRESULT_SUCCESS;
}

// This method creates an "abstract" class. It should FAIL but it does not.
// We should either remove this method from the API or change it so that
// it taks the class definition for one of the "derived" classes TimelineMobSlot,
// StaticMobSlot, EventMobSlot, etc...(transdel 2000-MAR-10)
//
 AAFRESULT STDMETHODCALLTYPE
   ImplAAFMob::AppendNewSlot (ImplAAFSegment *segment,
                           aafSlotID_t  slotID,
                           const aafCharacter *slotName,
                           ImplAAFMobSlot **newSlot)
{
	ImplAAFMobSlot * tmpSlot = NULL;
	aafLength_t length = CvtInt32toLength(0, length);
	aafLength_t	mobLength = CvtInt32toLength(0, mobLength);
  ImplAAFDictionary *pDictionary = NULL;

  // Validate input pointers...
  if (NULL == segment || NULL == slotName || NULL == newSlot)
    return (AAFRESULT_NULL_PARAM);

	*newSlot = NULL;

	XPROTECT()
	{
		CHECK(GetDictionary(&pDictionary));
		CHECK(pDictionary->GetBuiltinDefs()->cdMobSlot()->
			  CreateInstance ((ImplAAFObject**) &tmpSlot));
		pDictionary->ReleaseReference();
		pDictionary = NULL;

//!!!	CHECK(tmpSlot->WriteRational(OMMSLTEditRate, editRate));
		CHECK(tmpSlot->SetSegment(segment));
		CHECK(tmpSlot->SetSlotID(slotID));
		CHECK(tmpSlot->SetName(slotName));

		/* Append new slot to mob */
		_slots.appendValue(tmpSlot);
	} /* XPROTECT */
	XEXCEPT
	{
		if (tmpSlot)
		  tmpSlot->ReleaseReference();
		tmpSlot = 0;
		if(pDictionary != NULL)
		  pDictionary->ReleaseReference();
		pDictionary = 0;
	}
	XEND;

	*newSlot = tmpSlot;
	if (tmpSlot)
		tmpSlot->AcquireReference();

	return(AAFRESULT_SUCCESS);
}

//****************
// AppendNewTimelineSlot()
//
AAFRESULT STDMETHODCALLTYPE
	ImplAAFMob::AppendNewTimelineSlot
        (const aafRational_t &editRate,   //@parm [in] Edit rate property value
		 ImplAAFSegment * segment,   //@parm [in] Segment to append as slot component
		 aafSlotID_t  slotID,   //@parm [in] The Slot ID
         const aafCharacter *  slotName,   //@parm [in] Slot Name (optional)
		 aafPosition_t  origin,
		 ImplAAFTimelineMobSlot ** newSlot)  //@parm [out] Newly created slot
{
	ImplAAFTimelineMobSlot	*aSlot = NULL;
	ImplAAFMobSlot			*tmpSlot = NULL;
  ImplAAFDictionary *pDictionary = NULL;
///fLength_t length = CvtInt32toLength(0, length);
///	aafLength_t	mobLength = CvtInt32toLength(0, mobLength);


  // Validate input pointers...
  if (NULL == segment || NULL == slotName || NULL == newSlot)
    return (AAFRESULT_NULL_PARAM);

	*newSlot = NULL;

	XPROTECT()
	  {
		CHECK(GetDictionary(&pDictionary));
		CHECK(pDictionary->GetBuiltinDefs()->cdTimelineMobSlot()->
			  CreateInstance ((ImplAAFObject**) &aSlot));
		pDictionary->ReleaseReference();
		pDictionary = NULL;
    CHECK(aSlot->Initialize());
		CHECK(aSlot->SetSegment(segment));
		CHECK(aSlot->SetSlotID(slotID));
		CHECK(aSlot->SetName(slotName));
		CHECK(aSlot->SetEditRate(editRate));
		CHECK(aSlot->SetOrigin(origin));

		/* Append new slot to mob */
		tmpSlot = aSlot;
		_slots.appendValue(tmpSlot);

	  } /* XPROTECT */

	XEXCEPT
	  {
		if (aSlot)
		  aSlot->ReleaseReference();
		aSlot = 0;
		if(pDictionary != NULL)
		  pDictionary->ReleaseReference();
		pDictionary = 0;
	  }
	XEND;

	*newSlot = aSlot;
	if (aSlot)
		aSlot->AcquireReference();

	return(AAFRESULT_SUCCESS);
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::GetSlots (ImplEnumAAFMobSlots **ppEnum)
{
  // Validate input pointer...
  if (NULL == ppEnum)
    return (AAFRESULT_NULL_PARAM);

  
  ImplEnumAAFMobSlots *theEnum =
	(ImplEnumAAFMobSlots *) CreateImpl (CLSID_EnumAAFMobSlots);
  if(theEnum == NULL)
	return(E_FAIL);

	// !!!Does not obey search criteria yet
	XPROTECT()
	{
		OMStrongReferenceVectorIterator<ImplAAFMobSlot>* iter = 
			new OMStrongReferenceVectorIterator<ImplAAFMobSlot>(_slots);
		if(iter == 0)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(theEnum->Initialize(&CLSID_EnumAAFMobSlots, this, iter));
		*ppEnum = theEnum;
	}
	XEXCEPT
	{
		if (theEnum)
		  theEnum->ReleaseReference();
		theEnum = 0;
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::AppendComment ( const aafCharacter*  pTagName,
								const aafCharacter*  pComment)
{
	ImplAAFTaggedValue*			pTaggedValue = NULL;
	ImplEnumAAFTaggedValues*	pEnum = NULL;
	
	aafCharacter					oldTagName[64];
	aafBool						commentFound = kAAFFalse;
	aafUInt32					numComments = 0;
	ImplAAFDictionary *pDictionary = NULL;
	
	if (pTagName == NULL || pComment == NULL)
		return AAFRESULT_NULL_PARAM;


	XPROTECT()
	{
		CHECK(CountComments(&numComments));
		if (numComments > 0)
		{
			CHECK(GetComments(&pEnum));
			CHECK(pEnum->NextOne(&pTaggedValue));
			while(pTaggedValue)
			{
				CHECK(pTaggedValue->GetName(oldTagName, sizeof(oldTagName)));
				if (wcscmp(oldTagName, pTagName) == 0)
				{
					commentFound = kAAFTrue;
					break;
				}
				pTaggedValue->ReleaseReference();
				pTaggedValue = NULL;
				pEnum->NextOne(&pTaggedValue);
			}
			pEnum->ReleaseReference();
			pEnum = 0;
		}

		CHECK(GetDictionary(&pDictionary));
    // Do not reference count the following type and class definitions...
    ImplAAFTypeDef *pTaggedValueType = pDictionary->GetBuiltinDefs()->tdString();
    assert (pTaggedValueType); // this is supposed to be a builtin type.
 		ImplAAFClassDef *pTaggedValueClass = pDictionary->GetBuiltinDefs()->cdTaggedValue();
    assert (pTaggedValueClass); // this is supposed to be a builtin type.

		if (commentFound)
		{
			// Update existing comment
			CHECK(pTaggedValue->SetValue((wcslen(pComment)+1)*sizeof(aafCharacter), (aafDataValue_t)pComment));
			pTaggedValue->ReleaseReference();
			pTaggedValue = 0;
		}
		else
		{
			// Create a new comment and add it to the list!
			CHECK(pTaggedValueClass->CreateInstance ((ImplAAFObject**) &pTaggedValue));
			CHECK(pTaggedValue->Initialize(pTagName,
										   pTaggedValueType,
                       ((wcslen(pComment)+1)*sizeof(aafCharacter)), 
                       (aafDataValue_t)pComment));
			_userComments.appendValue(pTaggedValue);
		}
		pDictionary->ReleaseReference();
		pDictionary = NULL;

	}
	XEXCEPT
	{
		if(pDictionary != NULL)
		  pDictionary->ReleaseReference();
		pDictionary = 0;
	}
	XEND;

//	if (pTaggedValue)
//		pTaggedValue->AcquireReference();

	return(AAFRESULT_SUCCESS);
}

//****************
// RemoveComment()
//
AAFRESULT STDMETHODCALLTYPE
	ImplAAFMob::RemoveComment
        (ImplAAFTaggedValue * comment)
{
	if (! comment)
		return AAFRESULT_NULL_PARAM;
  if (!comment->attached ()) // object could not possibly be in container.
    return AAFRESULT_OBJECT_NOT_ATTACHED;
	if(!_userComments.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
  size_t index;
  if (_userComments.findIndex (comment, index))
  {
	  _userComments.removeAt(index);
    // We have removed an element from a "stong reference container" so we must
    // decrement the objects reference count. This will not delete the object
    // since the caller must have alread acquired a reference. (transdel 2000-MAR-10)
    comment->ReleaseReference ();
  }
  else
  {
    return AAFRESULT_OBJECT_NOT_FOUND;
  }

	return(AAFRESULT_SUCCESS);
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::CountComments (aafUInt32*  pNumComments)
{
	if (pNumComments == NULL)
		return AAFRESULT_NULL_PARAM;

	if(!_userComments.isPresent())
	{	// If the userComments property is not present then
		// number of user comments is zero!
		*pNumComments = 0; //return AAFRESULT_PROP_NOT_PRESENT;
	}
	else
	{
		*pNumComments = _userComments.count();
	}
		
	return(AAFRESULT_SUCCESS);
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::GetComments (ImplEnumAAFTaggedValues** ppEnum)
{
  // Validate input pointer...
  if (NULL == ppEnum)
    return (AAFRESULT_NULL_PARAM);

	if(!_userComments.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
		
	ImplEnumAAFTaggedValues* theEnum =
	  (ImplEnumAAFTaggedValues *)CreateImpl(CLSID_EnumAAFTaggedValues);

	XPROTECT()
	{
		OMStrongReferenceVectorIterator<ImplAAFTaggedValue>* iter = 
			new OMStrongReferenceVectorIterator<ImplAAFTaggedValue>(_userComments);
		if(iter == 0)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(theEnum->Initialize(&CLSID_EnumAAFTaggedValues, this, iter));
		*ppEnum = theEnum;
	}
	XEXCEPT
	{
		if (theEnum)
		  theEnum->ReleaseReference();
		theEnum = 0;
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::AppendKLVData (ImplAAFKLVData * pData)
{
	if (NULL == pData)
		return AAFRESULT_NULL_PARAM;
  if (pData->attached ())
    return AAFRESULT_OBJECT_ALREADY_ATTACHED;

	_KLVData.appendValue(pData);
	pData->AcquireReference();
	return AAFRESULT_SUCCESS;
}

//****************
// RemoveKLVData()
//
AAFRESULT STDMETHODCALLTYPE
	ImplAAFMob::RemoveKLVData
        (ImplAAFKLVData * pData)
{
	if (! pData)
		return AAFRESULT_NULL_PARAM;
  if (!pData->attached ()) // object could not possibly be in container.
    return AAFRESULT_OBJECT_NOT_ATTACHED;
	if(!_KLVData.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
  size_t index;
  if (_KLVData.findIndex (pData, index))
  {
	  _KLVData.removeAt(index);
    // We have removed an element from a "stong reference container" so we must
    // decrement the objects reference count. This will not delete the object
    // since the caller must have alread acquired a reference. (transdel 2000-MAR-10)
    pData->ReleaseReference ();
  }
  else
  {
    return AAFRESULT_OBJECT_NOT_FOUND;
  }

	return(AAFRESULT_SUCCESS);
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::CountKLVData (aafUInt32*  pNumComments)
{
	if (pNumComments == NULL)
		return AAFRESULT_NULL_PARAM;

	if(!_KLVData.isPresent())
	{	// If the userComments property is not present then
		// number of user comments is zero!
		*pNumComments = 0; //return AAFRESULT_PROP_NOT_PRESENT;
	}
	else
	{
		*pNumComments = _KLVData.count();
	}
		
	return(AAFRESULT_SUCCESS);
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::GetKLVData (ImplEnumAAFKLVData** ppEnum)
{
  if (NULL == ppEnum)
	return AAFRESULT_NULL_PARAM;
  *ppEnum = 0;
	
  ImplEnumAAFKLVData *theEnum = (ImplEnumAAFKLVData *)CreateImpl (CLSID_EnumAAFKLVData);
	
  XPROTECT()
	{
		OMStrongReferenceVectorIterator<ImplAAFKLVData>* iter = 
			new OMStrongReferenceVectorIterator<ImplAAFKLVData>(_KLVData);
		if(iter == 0)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(theEnum->Initialize(&CLSID_EnumAAFKLVData, this, iter));
	  *ppEnum = theEnum;
	}
  XEXCEPT
	{
	  if (theEnum)
		{
		  theEnum->ReleaseReference();
		  theEnum = 0;
		}
	}
  XEND;
	
  return(AAFRESULT_SUCCESS);
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::OffsetToMobTimecode (ImplAAFSegment *tcSlotID,
                           aafPosition_t *offset,
                           aafTimecode_t *result)
{
//	ImplAAFMobSlot		*slot = NULL;
//	ImplAAFPulldown		*pdwn = NULL;
//	ImplAAFSegment		*pdwnInput = NULL;
//	ImplEnumAAFMobSlots *iter = NULL;
//	ImplAAFSegment		*seg = NULL;
//	aafTimecode_t		timecode;
//	aafBool				reverse = kAAFFalse;
//	aafUInt32			frameOffset;
//	aafUID_t			dataDefID;
//	aafPosition_t		newStart;
//	aafInt32			start32;
	

  // Validate input pointers...
  if (NULL == tcSlotID || NULL == offset || NULL == result)
    return (AAFRESULT_NULL_PARAM);
  
  return AAFRESULT_NOT_IN_CURRENT_VERSION;
#if 0
	memset(result, 0, sizeof(aafTimecode_t));
	memset(&timecode, 0, sizeof(aafTimecode_t));
	result->startFrame = 0;
	
	XPROTECT()
	{
		
		/* Find timecode slot in mob */
		//!!!			iterHdl = new AAFIterate(_file);
		CHECK(GetSlots (&iter));
		while(iter->NextOne(&slot) == AAFRESULT_SUCCESS)
		{
			CHECK(slot->GetSegment(&seg));
			
			/* Verify that it's a timecode slot by looking at the
			* datakind of the slot segment. 
			*/
			ImplAAFDataDefSP pDataDef;
			CHECK(seg->GetDataDef(&pDataDef));
			CHECK(pDataDef->GetAUID(&dataDefID));
			
			if (!EqualAUID(&dataDefID, &DDEF_Timecode))
			{
				seg->ReleaseReference();
				seg = NULL;
			}
		}
		if (seg == NULL)
		{
			RAISE(AAFRESULT_TIMECODE_NOT_FOUND);
		}
		
		CHECK(seg->SegmentOffsetToTC(offset, &timecode))
			
			
			/* Assume found at this point, so finish generating result */
			/* If this is a Film Composer file that has a mask or pulldown object
			* in the timecode slot, pass the position through the mask/pulldown
			* before adding it to the start timecode.
			*/
			if (pdwn)
			{
				reverse = kAAFFalse;
				CHECK(pdwn->MapOffset(*offset, reverse, &newStart, NULL));
				CHECK(TruncInt64toInt32(newStart, &start32));
				timecode.startFrame += start32;
			}
			else
			{
				CHECK(TruncInt64toUInt32(*offset, &frameOffset));
				timecode.startFrame += frameOffset;
			}
			
			*result = timecode;
			if(slot != NULL)
			{
				slot->ReleaseReference();
				slot = NULL;
			}
			if(pdwn != NULL)
			{
				pdwn->ReleaseReference();
				pdwn = NULL;
			}
			if(pdwnInput != NULL)
			{
				pdwnInput->ReleaseReference();
				pdwnInput = NULL;
			}
			if(iter != NULL)
			{
				iter->ReleaseReference();
				iter = NULL;
			}
	} /* XPROTECT */
	
	XEXCEPT
	{
		if(iter != NULL)
		  iter->ReleaseReference();
		iter = 0;
		if(slot != NULL)
		  slot->ReleaseReference();
		slot = 0;
		if(pdwn != NULL)
		  pdwn->ReleaseReference();
		pdwn = 0;
		if(pdwnInput != NULL)
		  pdwnInput->ReleaseReference();
		pdwnInput = 0;
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
#endif
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::FindSlotBySlotID (aafSlotID_t	slotID,
                           ImplAAFMobSlot **destSlot)
{
	aafUInt32		loop;
	aafNumSlots_t	numSlots;
	ImplAAFMobSlot	*tmpSlot = NULL;
	aafSlotID_t	tmpSlotID;
	aafBool			foundSlot = kAAFFalse;
	

  // Validate input pointers...
  if (NULL == destSlot)
    return (AAFRESULT_NULL_PARAM);
  
	XPROTECT()
	{
		*destSlot = NULL;
		
		// For size entries the valid positions are 0 .. size - 1
		CHECK(CountSlots(&numSlots));
		for (loop = 0; loop < numSlots; loop++)
		{
			_slots.getValueAt(tmpSlot, loop);
			CHECK(tmpSlot->GetSlotID(&tmpSlotID));
			if (tmpSlotID == slotID)
			{
				foundSlot = kAAFTrue;
				break;
			}
		}
		if (!foundSlot)
		{
			RAISE(AAFRESULT_SLOT_NOT_FOUND);
		}
		*destSlot = tmpSlot;
		if (tmpSlot)
			tmpSlot->AcquireReference();
	}
  XEXCEPT
	{
	}
  XEND;

  return(AAFRESULT_SUCCESS);
}


// trr: Does this method only work for AAFSourceMobs? If so we should probably move
// it the AAFSourceMob.dod.
AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::OffsetToTimecode (aafSlotID_t*slotID,
                           aafPosition_t *  offset,
                           aafTimecode_t *  result)
{
	ImplAAFTimelineMobSlot	*slot = NULL;
	ImplAAFSegment			*seg = NULL;
	aafTimecode_t 			timecode;
	aafMediaCriteria_t		mediaCrit;
	ImplAAFFindSourceInfo	*sourceInfo = NULL;
	aafRational_t			editRate;
	aafPosition_t			frameOffset64;
	aafLength_t				zeroLen = CvtInt32toLength(0, zeroLen);
	ImplEnumAAFMobSlots		*slotIter = NULL;
	ImplAAFDictionary		*dict = NULL;
	ImplAAFDataDef			*dataDef = NULL;
	ImplAAFMob				*tapeMob = NULL;

  // Validate input pointers...
  if (NULL == slotID || NULL == offset || NULL == result)
    return (AAFRESULT_NULL_PARAM);

  memset(result, 0, sizeof(aafTimecode_t));
	memset(&timecode, 0, sizeof(aafTimecode_t));
	result->startFrame = 0;
	
	XPROTECT()
	{

		CHECK(GetDictionary(&dict));
		ImplAAFDataDefSP pDDPicture;
		CHECK(dict->LookupDataDef(DDEF_Picture, &pDDPicture));
		mediaCrit.type = kAAFAnyRepresentation;
		CHECK(InternalSearchSource(*slotID, *offset, kAAFTapeMob,
			&mediaCrit, NULL, &sourceInfo));
		
		CHECK(sourceInfo->GetMob(&tapeMob));
		CHECK(tapeMob->GetSlots (&slotIter));
		while(slotIter->NextOne((ImplAAFMobSlot**)&slot) == AAFRESULT_SUCCESS)
		{
			CHECK(slot->GetEditRate(&editRate));
			CHECK(slot->GetSegment(&seg));
			slot->ReleaseReference();
			slot = NULL;
			/* Verify that it's a timecode slot by looking at the
			* datadef of the slot segment.
			*/
			CHECK(seg->GetDataDef(&dataDef));
			aafBool		isTimecode;
			CHECK(dataDef->IsDataDefOf(pDDPicture, &isTimecode));
			if (isTimecode)
			{
				/* Assume found at this point, so finish generating result */
//!!!				CHECK(AAFConvertEditRate(sourceInfo.editrate, sourceInfo.position,
//					editRate, kRoundCeiling, &frameOffset64));
				
				CHECK(tapeMob->OffsetToMobTimecode(seg, &frameOffset64, &timecode));
				dataDef->ReleaseReference();
				dataDef = NULL;
				break;
			}
			dataDef->ReleaseReference();
			dataDef = NULL;
		} /* for */
		tapeMob->ReleaseReference();
		tapeMob = NULL;
		slotIter->ReleaseReference();
		slotIter = NULL;
    sourceInfo->ReleaseReference();
		dict->ReleaseReference();
		dict = NULL;
		
		
		*result = timecode;
	} /* XPROTECT */
	XEXCEPT
	{
		if (tapeMob)
		  dataDef->ReleaseReference();
		dataDef = 0;
		if (dataDef)
		  dataDef->ReleaseReference();
		dataDef = 0;
		if (slotIter)
		  slotIter->ReleaseReference();
		slotIter = 0;
		if (slot)
		  slot->ReleaseReference();
		slot = 0;
    if (sourceInfo)
      sourceInfo->ReleaseReference();
    sourceInfo = NULL;
		if (dict)
		  dict->ReleaseReference();
		dict = 0;
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}


// trr: Does this method only work for AAFSourceMobs? If so we should probably move
// it the AAFSourceMob.dod.
AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::TimecodeToOffset (aafTimecode_t  timecode,
                           aafSlotID_t  slotID,
                           aafFrameOffset_t *  result)
{
	ImplEnumAAFMobSlots *iterHdl = NULL;
	ImplEnumAAFComponents *sequIter = NULL;
	ImplAAFMobSlot *slot = NULL;
	ImplAAFTimelineMobSlot *timelineSlot = NULL;
	ImplAAFPulldown *pdwn = NULL;
	ImplAAFSegment *seg = NULL;
	ImplAAFSegment *pdwnInput = NULL;
	aafPosition_t zero;
	aafBool found = kAAFFalse;
	aafRational_t	editRate;
	ImplAAFFindSourceInfo	*sourceInfo = NULL;
	ImplAAFMob	*tapeMob = NULL;

  // Validate input pointer...
  if (NULL == result)
    return (AAFRESULT_NULL_PARAM);

	CvtInt32toPosition(0, zero);
	
	XPROTECT()
	{
		CHECK(InternalSearchSource(slotID, zero, kAAFTapeMob,
			NULL /* mediaCrit */, NULL, &sourceInfo));
		
		CHECK(sourceInfo->GetMob(&tapeMob));
		CHECK(tapeMob->GetSlots(&iterHdl));
		while(iterHdl->NextOne(&slot) == AAFRESULT_SUCCESS)
		{
			timelineSlot = dynamic_cast<ImplAAFTimelineMobSlot*>(slot);
			if(timelineSlot != NULL)
			{
				CHECK(timelineSlot->GetEditRate(&editRate));
				CHECK(timelineSlot->GetSegment(&seg));
			
				if(seg->SegmentTCToOffset(&timecode, &editRate, result) == AAFRESULT_SUCCESS)
					found = kAAFTrue;
				
				timelineSlot->ReleaseReference();
				timelineSlot = NULL;
				seg->ReleaseReference();
				seg = NULL;
			}
			
			slot->ReleaseReference();
			slot = NULL;
		}
		
		if(!found)
		{
			RAISE(AAFRESULT_TIMECODE_NOT_FOUND);
		}
		
		/* Release reference, so the useCount is decremented */
		tapeMob->ReleaseReference();	
		tapeMob = NULL;
		sourceInfo->ReleaseReference();	
		sourceInfo = NULL;
		iterHdl->ReleaseReference();
		iterHdl = NULL;
		if(sequIter != NULL)
		{
			sequIter->ReleaseReference();
			sequIter = NULL;
		}
		if(pdwn != NULL)
		{
			pdwn->ReleaseReference();
			pdwn = NULL;
		}
		if(seg != NULL)
		{
			seg->ReleaseReference();
			seg = NULL;
		}
		if(pdwnInput != NULL)
		{
			pdwnInput->ReleaseReference();
			pdwnInput = NULL;
		}
	}
	
	XEXCEPT
	{
		if (tapeMob)
		  tapeMob->ReleaseReference();	
		tapeMob = 0;
		if (sourceInfo)
		  sourceInfo->ReleaseReference();	
		sourceInfo = 0;
		if(iterHdl != NULL)
		  iterHdl->ReleaseReference();
		iterHdl = 0;
		if(sequIter != NULL)
		  sequIter->ReleaseReference();
		sequIter = 0;
		if(slot != NULL)
		  slot->ReleaseReference();
		slot = 0;
		if(pdwn != NULL)
		  pdwn->ReleaseReference();
		pdwn = 0;
		if(seg != NULL)
		  seg->ReleaseReference();
		seg = 0;
		if(pdwnInput != NULL)
		  pdwnInput->ReleaseReference();
		pdwnInput = 0;
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}




AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::Copy (const aafCharacter *  /*destMobName*/,
                           ImplAAFMob ** /*destMob*/)
{
#if FULL_TOOLKIT
	AAFMob * newMob;
	aafBool isPrimary, isMaster;
	AAFHeader * head;
	AAFRESULT aafError = AAFRESULT_SUCCESS;
	
	*destMob = NULL;
	aafAssert((_file->GetOpenType() != kOmOpenRead), _file, 
					AAFRESULT_WRONG_OPENMODE);

	XPROTECT(_file)
	  {
		CHECK(CopyTree((AAFObject **)&newMob));

		/* MobSetNewProps will create a new mob ID, set name, and add
		 * to the Primary mobs index if needed
		 */

		isPrimary = IsAPrimaryMob(&aafError);
		isMaster = IsTypeOf("MMOB", &aafError);
		if (aafError == AAFRESULT_SUCCESS)
		  {
			CHECK(newMob->SetPrimary(isPrimary));
			CHECK(newMob->SetName(destMobName));
		  }
		else
		  {
			RAISE(aafError);
		  }

		/* Store the new mob in the HEAD Mobs */
		CHECK(_file->GetHeadObject(&head));
		CHECK(head->AddToMobList(newMob));
	  } /* XPROTECT */

	XEXCEPT
	  {
		/* NOTE: This function needs more cleanup (delete mob, out of index) */
	  }
	XEND;

	*destMob = newMob;
	return(AAFRESULT_SUCCESS);
#else
  return AAFRESULT_NOT_IN_CURRENT_VERSION;
#endif
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::ChangeRef (aafMobID_constref oldMobID,
                           aafMobID_constref newMobID)
{
	ImplEnumAAFMobSlots		*iter = NULL;
	ImplAAFMobSlot			*slot = NULL;

	XPROTECT()
	{
		CHECK(GetSlots (&iter));
		while(iter->NextOne(&slot) == AAFRESULT_SUCCESS)
		{
			CHECK(slot->ChangeContainedReferences(oldMobID, newMobID));
			slot->ReleaseReference();
			slot = NULL;
		}
    iter->ReleaseReference(); // 2000-OCT-19 transdel: plug memory leak
	}
	XEXCEPT
	{
		if(slot)
		  slot->ReleaseReference();
		slot = 0;
		if(iter)
		  iter->ReleaseReference();
		iter = 0;
	}
	XEND;

	return(AAFRESULT_SUCCESS);
}



 /*************************************************************************
 * Function: CloneExternal()
 *
 *      This function clones the given mob in the source file into
 *      a destination mob in the destination file with the same Mob ID.
 *      If resolveDependencies is kAAFTrue, it will also clone all mobs 
 *      referenced by the given source mob. 
 *
 *      If includeMedia is kAAFTrue, it will also copy the media data 
 *      associated with the source mob.  The destination mob is
 *      returned. All private data is also cloned.
 *
 *      If the media data is not in the file, it will not attempt to
 *      try and find it in another file and clone it.
 *
 *      This function supports both 1.x and 2.x files.
 *
 * Argument Notes:
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 *************************************************************************/
AAFRESULT STDMETHODCALLTYPE
   ImplAAFMob::CloneExternal (aafDepend_t  /*resolveDependencies*/,
                           aafIncMedia_t  /*includeMedia*/,
                           ImplAAFFile * /*destFile*/,
                           ImplAAFMob ** /*destMob*/)
{
#if FULL_TOOLKIT
    aafMobID_t saveMobID, newMobID;
	aafString destMobName = NULL;
    AAFMob * tmpDestMob = NULL;
	AAFObject *tmpDestMedia = NULL;
	AAFHeader *destHead = NULL;
	aafTimeStamp_t saveCreationTime, saveModTime;
	aafFileRev_t srcRev, destRev;
	AAFRESULT aafError = AAFRESULT_SUCCESS;

	*destMob = NULL;

	XPROTECT(_file)
	  {
		CHECK(_file->GetRevision(&srcRev));
		CHECK(destFile->GetRevision(&destRev));
		if (srcRev != destRev)
		  {
			RAISE(AAFRESULT_FILEREV_DIFF);
		  }

		/* Copy the Mob, then change the MobID back to its original */
		/* Remember the creation and modification times, to restore later */
		CHECK(GetMobID(&saveMobID));
		CHECK(GetModTime(&saveModTime));
		CHECK(GetCreateTime(&saveCreationTime));

		destMobName = (aafString)_file->omOptMalloc(OMMOBNAME_SIZE);
		strncpy(destMobName, "\0", 1);
		  {
			aafError = ReadString(OMMOBJName, 
									  (char *)destMobName, OMMOBNAME_SIZE);
		  }

		CHECK(CopyExternal(resolveDependencies,
								  includeMedia, destMobName, destFile,
								  &tmpDestMedia, &tmpDestMob));
		CHECK(tmpDestMob->GetMobID(&newMobID));

		 /* omfiMobSetIdentity will take out the existing hash table
		 * entry and add an entry for the original cloned mobID.
		 */
		CHECK(tmpDestMob->SetIdentity(saveMobID));

		/* Restore creation and modification times */
		  {
			CHECK(tmpDestMob->WriteTimeStamp(OMMOBJCreationTime,
									 saveCreationTime));
		  }
		CHECK(tmpDestMob->WriteTimeStamp(OMMOBJLastModified,
								 saveModTime)); 

		/* If it is a primary mob, put it in the primary mob index */
		if (IsAPrimaryMob(&aafError))
		  {
			if (aafError == AAFRESULT_SUCCESS)
			  {
				CHECK(tmpDestMob->SetPrimary(kAAFTrue));
			  }
			else
			  {
				RAISE(aafError);
			  }
		  }
		if (destMobName)
		  _file->omOptFree(destMobName);
		destMobName = NULL;

		if (includeMedia)
		  {
			/* The media data object has already been copied.  Now, we have
			 * to change the MobID back to the original, since we're cloning 
			 */
			if (tmpDestMedia)
			  {
				/* Change the mob ID to be the original mob ID */
				  {
					CHECK(tmpDestMedia->WriteUID(OMMDATMobID,saveMobID));
				  }

//!!!Need to put this back				CHECK(TableRemoveUID(destHead->_dataObjs, newMobID));
				/* Add to the MediaData hash table */
				CHECK(destHead->AppendDataObject(saveMobID, tmpDestMedia));
			  }

			/* If the media data is not in this file, don't report an error */
		  }

	  }
	XEXCEPT
	  {
		if (destMobName)
		  _file->omOptFree(destMobName);
		/* NOTE: This function needs more cleanup (delete mob, out of index) */
		if (XCODE() == AAFRESULT_TABLE_DUP_KEY)
		  return(AAFRESULT_DUPLICATE_MOBID);
	  }
	XEND;

	*destMob = tmpDestMob;
	return(AAFRESULT_SUCCESS);
#else
  return AAFRESULT_NOT_IN_CURRENT_VERSION;
#endif
}



  // @commDeletes the entire Mob structure \(the MOBJ object and all its contained objects\)
  // and deletes the entry from the Header.

// Internal to the toolkit functions
AAFRESULT
    ImplAAFMob::GetNthMobSlot (aafInt32 index /*0-based*/, ImplAAFMobSlot **ppMobSlot)
{
	AAFRESULT rc = AAFRESULT_SUCCESS;
	ImplAAFMobSlot	*obj;

  if (NULL == ppMobSlot)
		return AAFRESULT_NULL_PARAM;
  
  _slots.getValueAt(obj, index);
	if (obj)
		obj->AcquireReference();
	else
		rc = AAFRESULT_NO_MORE_OBJECTS; // AAFRESULT_BADINDEX ???
	*ppMobSlot = obj;

	return rc;
}

AAFRESULT STDMETHODCALLTYPE
ImplAAFMob::AddPhysSourceRef (aafAppendOption_t  addType,
							  aafRational_t  editrate,
							  aafSlotID_t  aMobSlot,
							  const aafUID_t & essenceKind,
							  aafSourceRef_t  ref,
							  aafLength_t  srcRefLength)
{
	ImplAAFComponent		*cpnt = NULL;
	ImplAAFSegment			*slotSeg = NULL;
	AAFRESULT				status = AAFRESULT_SUCCESS;
	aafSlotID_t				tmpSlotID;
	ImplAAFSourceClip		*sclp = NULL;
	ImplAAFSequence			*sequence = NULL;
	ImplAAFMobSlot			*slot = NULL;
	ImplAAFTimelineMobSlot	*newSlot = NULL;
	aafPosition_t			zeroPos;
	ImplAAFDictionary *pDictionary = NULL;

	XPROTECT()
	{
		CvtInt32toInt64(0, &zeroPos);
		CHECK(GetDictionary(&pDictionary));
		CHECK(pDictionary->GetBuiltinDefs()->cdSourceClip()->
			  CreateInstance ((ImplAAFObject **) &sclp));
		if (NULL == sclp)
			RAISE(AAFRESULT_NOMEMORY);
		ImplAAFDataDefSP pDataDef;
		CHECK(pDictionary->LookupDataDef(essenceKind, &pDataDef))
		pDictionary->ReleaseReference();
		pDictionary = NULL;
		CHECK(sclp->Initialize(pDataDef, srcRefLength, ref));
				
		status = FindSlotBySlotID(aMobSlot, &slot);
		if (status == AAFRESULT_SUCCESS)
		{
			CHECK(slot->GetSlotID(&tmpSlotID));
			if(addType == kAAFForceOverwrite)
			{
				CHECK(slot->SetSegment(sclp));
			}
			else if(addType == kAAFAppend)
			{
				CHECK(slot->GetSegment(&slotSeg));
				if(slotSeg != NULL)
				{
					CHECK(slotSeg->GenerateSequence(&sequence));
					CHECK(sequence->AppendComponent (sclp));

					sequence->ReleaseReference();
					sequence = NULL;
					slotSeg->ReleaseReference();
					slotSeg = NULL;
				}
			}
			//!!! else return an error
			slot->ReleaseReference();
			slot = NULL;
		}
		else
		{
				CHECK(AppendNewTimelineSlot(editrate, sclp,
								aMobSlot, L"ASlot", zeroPos, &newSlot) );
			newSlot->ReleaseReference();
			newSlot = NULL;
		}

		sclp->ReleaseReference();
		sclp = NULL;
	} /* XPROTECT */
	XEXCEPT
	{
		if(sequence != NULL)
		  sequence->ReleaseReference();
		sequence = 0;
		if(sclp != NULL)
		  sclp->ReleaseReference();
		sclp = 0;
		if(slot != NULL)
		  slot->ReleaseReference();
		slot = 0;
		if(slotSeg != NULL)
		  slotSeg->ReleaseReference();
		slotSeg = 0;
		if(slot != NULL)
		  slot->ReleaseReference();
		slot = 0;
		if(cpnt != NULL)
		  cpnt->ReleaseReference();
		cpnt = 0;
		if(pDictionary != NULL)
		  pDictionary->ReleaseReference();
		pDictionary = 0;
	}
	XEND;

	return (AAFRESULT_SUCCESS);
}

/* NOTE: the assumption is that this function should be used primarily 
* for master mob and down 
*/
AAFRESULT ImplAAFMob::InternalSearchSource(	
										   aafSlotID_t trackID,
										   aafPosition_t offset,
										   aafMobKind_t mobKind,
										   aafMediaCriteria_t *pMediaCrit,
										   aafOperationChoice_t *pOperationChoice,
										   ImplAAFFindSourceInfo **ppSourceInfo)
{
	ImplAAFMobSlot 			*track = NULL;
	ImplAAFSegment			*rootObj = NULL;
	aafRational_t			srcRate;
	aafPosition_t			diffPos, nextPos;
	aafBool					sourceFound = kAAFFalse, foundTransition = kAAFFalse;
	ImplAAFMob				*nextMob = NULL;
	aafInt32				nestDepth, pulldownPhase;
	aafPosition_t			zeroPos = CvtInt32toPosition(0, zeroPos);
	aafLength_t				cpntLen, nextLen, minLength, newLen;
	ImplAAFPulldown			*pulldownObj = NULL;
	aafSlotID_t				nextTrackID;
	ImplAAFFindSourceInfo	*sourceInfo = NULL ;
	ImplAAFComponent		*leafObj = NULL;
	ImplAAFOperationGroup	*effeObject = NULL;
	
	if(ppSourceInfo == NULL)
		return(AAFRESULT_NULL_PARAM);
	
	/* Initialize outputs */
	sourceInfo = (ImplAAFFindSourceInfo *)CreateImpl(CLSID_AAFFindSourceInfo);
	if (NULL == sourceInfo)
		return (AAFRESULT_NOMEMORY);
	sourceInfo->AcquireReference();		// This will be passed out
	*ppSourceInfo = sourceInfo;
	sourceInfo->Clear();
	
	XPROTECT()
	{
		/* Find segment at offset */
		CHECK(FindSlotBySlotID (trackID,  &track));
		CHECK(track->FindSegment(offset, &rootObj, &srcRate, &diffPos));
		CHECK(rootObj->GetLength(&cpntLen));
		CHECK(sourceInfo->SetComponent(rootObj));
		
		/*** Find leaf object in this track that points to the next mob ***/
		CHECK(MobFindLeaf(track, 
			pMediaCrit, pOperationChoice,
			rootObj, offset, cpntLen,
			NULL, NULL, 
			NULL, /* Shouldn't be scopes here */
			zeroPos,
			&leafObj, &minLength, &foundTransition, &effeObject,
			&nestDepth, NULL));
		if (Int64Less(minLength, cpntLen))
		{
			/* Figure out diffPos */
			newLen = minLength;
			/* NOTE: What should diffPos be in this case? */
		}
		else
		{
			newLen = cpntLen;
		}
		
		/*** Find the next mob, factoring in mask object edit rate conversions,
		*** and 1.0 track mappings.
		***/
		CHECK(FindNextMob(track, (ImplAAFSegment *)leafObj, 
			cpntLen, diffPos,
			&nextMob, &nextTrackID, &nextPos, &pulldownObj, &pulldownPhase, &nextLen));
		if(pulldownObj != NULL)
		{
			CHECK(sourceInfo->AddPulldown(pulldownObj));
		} 
		
		/*** Find component at referenced position in new mob ***/
		CHECK(nextMob->MobFindSource(nextTrackID, nextPos, nextLen,
			mobKind, pMediaCrit, pOperationChoice,
			sourceInfo, &sourceFound));
		if (!sourceFound)
			RAISE(AAFRESULT_TRAVERSAL_NOT_POSS);

		nextMob->ReleaseReference();
		nextMob = 0;
		if (leafObj)
		  leafObj->ReleaseReference();
		leafObj = 0;
		if (effeObject)
		  effeObject->ReleaseReference();
		effeObject = 0;
		rootObj->ReleaseReference();
		rootObj = 0;
		track->ReleaseReference();
		track = 0;
		sourceInfo->ReleaseReference();
		sourceInfo = 0;
	} /* XPROTECT */
	XEXCEPT
	{
		if(XCODE() == AAFRESULT_PARSE_EFFECT_AMBIGUOUS)
			sourceInfo->SetOperationGroup(effeObject);

		if (nextMob)
		  nextMob->ReleaseReference();
		nextMob = 0;
		if (leafObj)
		  leafObj->ReleaseReference();
		leafObj = 0;
		if (effeObject)
		  effeObject->ReleaseReference();
		effeObject = 0;
		if (rootObj)
		  rootObj->ReleaseReference();
		rootObj = 0;
		if (track)
		  track->ReleaseReference();
		track = 0;
		if (sourceInfo)
		  sourceInfo->ReleaseReference();
		sourceInfo = 0;
	}
	XEND;
	
	return AAFRESULT_SUCCESS;
}
										   										   
AAFRESULT ImplAAFMob::MobFindLeaf(ImplAAFMobSlot *track,
								  aafMediaCriteria_t *mediaCrit,
								  aafOperationChoice_t *operationChoice,
								  ImplAAFComponent *rootObj,
								  aafPosition_t rootPos,
								  aafLength_t rootLen,
								  ImplAAFComponent	*prevObject,
								  ImplAAFComponent *nextObject,
								  ImplAAFScopeStack *scopeStack,
								  aafPosition_t	currentObjPos,
								  ImplAAFComponent **foundObj,
								  aafLength_t *minLength,
								  aafBool *foundTransition,
								  ImplAAFOperationGroup **effeObject,
								  aafInt32	*nestDepth,
								  aafPosition_t *diffPos)
{
	aafSlotID_t trackID;

	/* Parameter initialization and checking */
	if(nestDepth == NULL || foundObj == NULL || effeObject == NULL || minLength == NULL)
		return(AAFRESULT_NULL_PARAM);
	if(rootObj == NULL)
		return(AAFRESULT_NULLOBJECT);

	*nestDepth = 0;
	*foundObj = NULL;
	*effeObject = NULL;
	CvtInt32toLength(0, (*minLength));
	
	XPROTECT()
	{
		CHECK(track->GetSlotID(&trackID));
		
		CHECK(rootObj->GetMinimumBounds(rootPos, rootLen,this, track, mediaCrit, currentObjPos,
			operationChoice, prevObject, nextObject, scopeStack,
			diffPos, minLength, effeObject, nestDepth,
			foundObj, foundTransition));
	} /* XPROTECT */
	XEXCEPT
	{
		/* At least try and return length if we can */
		//return(XCODE());
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}

/* This function will resolve mask objects to find the correct offset,
* and will map track IDs for 1.0 files.
*/
AAFRESULT ImplAAFMob::FindNextMob(ImplAAFMobSlot *track, 
								  ImplAAFSegment *segment,
								  aafLength_t length,
								  aafPosition_t diffPos,
								  ImplAAFMob **retMob,
								  aafSlotID_t *retTrackID,
								  aafPosition_t *retPos,
								  ImplAAFPulldown **pulldownObj,
								  aafInt32 *pulldownPhase,
								  aafLength_t *retLen)
{
	ImplAAFSourceClip		*sclp = NULL;
	aafLength_t				sclpLen;
	aafBool					isMask = kAAFFalse, reverse = kAAFFalse;
	aafSourceRef_t			sourceRef;
	ImplAAFMob				*nextMob = NULL;
  aafMobID_t				nullMobID = { 0 };		// Need "isNIL" utility
	ImplAAFMobSlot 			*nextTrack = NULL;
	aafSlotID_t				tmpTrackID, nextTrackID;
	aafPosition_t			tmpPos, convertPos;

	if(segment == NULL || retMob == NULL || retTrackID == NULL || retPos == NULL || retLen == NULL)
		return(AAFRESULT_NULL_PARAM);

	XPROTECT()
	{
		/* Initialize return parameters */
		*retMob = NULL;
		*retTrackID = 0;
		if(pulldownObj)
			*pulldownObj = NULL;
		
		/* Get source editrate from track, for later conversion */
		
		CHECK(segment->TraverseToClip(length, (ImplAAFSegment **)&sclp, pulldownObj, pulldownPhase,& sclpLen,
			&isMask));
		
		
		CHECK(sclp->GetSourceReference(&sourceRef));
		if (memcmp(&nullMobID, &sourceRef.sourceID, sizeof(sourceRef.sourceID)) == 0)
		{
			RAISE(AAFRESULT_TRAVERSAL_NOT_POSS);
		}
		/* Get next mob */
		CHECK(sclp->ResolveRef(&nextMob));
		
		tmpTrackID = sourceRef.sourceSlotID;
		
		/* Get destination track's edit rate */
		CHECK(nextMob->FindSlotBySlotID(tmpTrackID, (ImplAAFMobSlot **)&nextTrack));
		CHECK(nextTrack->GetSlotID(&nextTrackID));
		
		/* If its a MASK, apply the mask bits to the offset difference
		* (into the MASK object).  Then add this to the startTime in the
		* Source Clip.  Then do an editrate conversion in case it is
		* needed (24/1 -> 96/1 for Film Composer).  Then follow this 
		* offset to the next mob in the mob chain.
		*/
		if (isMask)
		{
			reverse = kAAFFalse;
			/* !!!Check out if we need phase returned from here */
			CHECK((*pulldownObj)->MapOffset(diffPos, reverse, &tmpPos, NULL));
		}
		else
			tmpPos = diffPos;
		
		CHECK(AddInt64toInt64(sourceRef.startTime, &tmpPos));
		if (!isMask)
		{
			CHECK(nextTrack->ConvertToMyRate(tmpPos,track, &convertPos));
		}
		else
			convertPos = tmpPos;
		
		*retMob = nextMob;
		*retTrackID = nextTrackID;
		*retPos = convertPos;
		*retLen = sclpLen;

		nextTrack->ReleaseReference();
		nextTrack = 0;

		sclp->ReleaseReference(); // causes 800400c8 to be returned from MasterMob::OpenEssence
	}
	XEXCEPT
	{
		if (nextTrack)
		  nextTrack->ReleaseReference();
		nextTrack = 0;
		if (nextMob)
		  nextMob->ReleaseReference();
		nextMob = 0;
		if (sclp)
			sclp->ReleaseReference();
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}

AAFRESULT ImplAAFMob::MobFindSource(
									aafSlotID_t trackID,
									aafPosition_t offset, /* offset in referenced units */
									aafLength_t length,   /* expected length of clip */
									aafMobKind_t mobKind,
									aafMediaCriteria_t *mediaCrit,
									aafOperationChoice_t *operationChoice,
									ImplAAFFindSourceInfo *sourceInfo,
									aafBool *foundSource)
{
	ImplAAFMobSlot			*track = NULL;
	ImplAAFPulldown			*pulldownObj = NULL;
	ImplAAFSegment			*rootObj = NULL;
	ImplAAFComponent		*leafObj = NULL;
	ImplAAFOperationGroup	*effeObject = NULL;
	ImplAAFMob				*nextMob = NULL;
	aafSlotID_t				foundTrackID;
	aafBool					nextFoundSource = kAAFFalse, foundTransition = kAAFFalse;
	aafPosition_t			foundPos, diffPos, zeroPos;
	aafRational_t			srcRate;
	aafLength_t				tmpLength, foundLen, minLength, newLen;
	aafInt32				nestDepth, pulldownPhase;
	aafMobKind_t			tstKind;
	
	CvtInt32toInt64(0, &zeroPos);
	if(sourceInfo == NULL || foundSource == NULL)
		return(AAFRESULT_NULL_PARAM);

	XPROTECT()
	{
		/* Initialize return values */
		*foundSource = kAAFFalse;
		CHECK(sourceInfo->Clear());
		
		/* Verify that track and position are valid */
		CHECK(FindSlotBySlotID (trackID,  &track));
		CHECK(track->FindSegment(offset, &rootObj, &srcRate, &diffPos));
		CHECK(rootObj->GetLength(&tmpLength));
		if (Int64Less(length, tmpLength))
		{
			tmpLength = length;
		}
		
		
		/* 1) Is this the mob that we're looking for? */
		CHECK(GetMobKind (&tstKind));
		if ((mobKind == kAAFCompMob) ||
			(mobKind == kAAFMasterMob) || 
			(mobKind == kAAFFileMob) ||
			(mobKind == kAAFTapeMob) ||
			(mobKind == kAAFFilmMob))
		{
			*foundSource = (mobKind == tstKind ? kAAFTrue : kAAFFalse);
		}
		else if (mobKind == kAAFAllMob)
			*foundSource = kAAFTrue;
		else
		{
			RAISE(AAFRESULT_INVALID_MOBTYPE);
		}
		
		if (*foundSource)
		{
			CHECK(sourceInfo->Init(this, trackID, offset,
				srcRate, tmpLength,
				/*!!!*/NULL));		// What to put in for CPNT
				
			rootObj->ReleaseReference();
			rootObj = 0;
			track->ReleaseReference();
			track = 0;
			return(AAFRESULT_SUCCESS);
		}
		
		/* 2) If not right mob type, find component at referenced position 
		* in new mob.
		*/
		
		/* Found the clip on the top level track - now, traverse down
		* to find the leaf source clip if it exists.  We are assuming
		* that there will not be nested sequences.
		*
		* NOTE: Media Composer uses nested sequences to embed 0-length 
		* fills in compositions.
		* We are also assuming that this function will probably be called
		* on the master mob and down - so, we shouldn't run into transitions.
		* So, passing NULL for prevObject and nextObject is probably alright.
		*/
		CHECK(MobFindLeaf(track, mediaCrit, operationChoice, 
			rootObj, offset, tmpLength,
			NULL, NULL, 
			NULL, /* Shouldn't be scopes here */
			zeroPos,
			&leafObj, &minLength, &foundTransition,
			&effeObject, &nestDepth, NULL));
		
		if (Int64Less(minLength, length))
		{
			/* Figure out diffPos!!! (changed newDIffPos -> tmpLength */
			newLen = minLength;
		}
		else
		{
			newLen = length;
		}
		
		/*** Find the next mob, factoring in mask object edit rate conversions,
		*** and 1.0 track mappings.
		***/
		CHECK(FindNextMob(track, (ImplAAFSegment *)leafObj, 
			tmpLength, diffPos,
			&nextMob, &foundTrackID, &foundPos, &pulldownObj, &pulldownPhase, &foundLen));
		
//		if(pulldownObj != NULL)
//		{
//!!!			aafPulldownDir_t	direction;

//!!!			{
//				CHECK(pulldownObj->ReadPulldownDirectionType(OMPDWNDirection, &direction));
//			}
//			if(direction == kAAFFilmToTapeSpeed)	/* kAAFFilmToTapeSpeed */
//			{
//				(*sourceInfo).filmTapePdwn = pulldownObj;
//				(*sourceInfo).filmTapePhase = pulldownPhase;
//			}
//			else				/* kAAFTapeToFilmSpeed */
//			{
//				(*sourceInfo).tapeFilmPdwn = pulldownObj;
//				(*sourceInfo).tapeFilmPhase = pulldownPhase;
//			}
//		} 
		/* Find component at referenced position in new mob */
		CHECK(nextMob->MobFindSource(foundTrackID,
			foundPos, foundLen,
			mobKind, mediaCrit, operationChoice,
			sourceInfo, &nextFoundSource));
		if (nextFoundSource)
		{
			*foundSource = nextFoundSource;
		}
		else /* Failure - null  out return values */
		{
			RAISE(AAFRESULT_TRAVERSAL_NOT_POSS);
		}
		
		nextMob->ReleaseReference();
		nextMob = 0;
		if (leafObj)
		  leafObj->ReleaseReference();
		leafObj = 0;
		if (effeObject)
		  effeObject->ReleaseReference();
		effeObject = 0;
		rootObj->ReleaseReference();
		rootObj = 0;
		track->ReleaseReference();
		track = 0;
	}
	XEXCEPT
	{
		if(XCODE() == AAFRESULT_PARSE_EFFECT_AMBIGUOUS)
			sourceInfo->SetOperationGroup(effeObject);

		if (nextMob)
		  nextMob->ReleaseReference();
		nextMob = 0;
		if (leafObj)
		  leafObj->ReleaseReference();
		leafObj = 0;
		if (effeObject)
		  effeObject->ReleaseReference();
		effeObject = 0;
		if (rootObj)
		  rootObj->ReleaseReference();
		rootObj = 0;
		if (track)
		  track->ReleaseReference();
		track = 0;
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}

AAFRESULT ImplAAFMob::ReconcileMobLength(void)
{
	return(AAFRESULT_ABSTRACT_CLASS);	// MUST call one of the subclasses
}


const OMMaterialIdentification&
  ImplAAFMob::identification(void) const
{
  return *reinterpret_cast<const OMMaterialIdentification*>(&_mobID.reference());
}



////////////////////////////////////////////////////////////////////////////////

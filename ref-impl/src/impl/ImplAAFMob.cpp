
/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/



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

#if defined(_MAC) || defined(macintosh)
#include <wstring.h>
#endif

#include <assert.h>
#include "AAFResult.h"
#include "aafCvt.h"
#include "AAFUtils.h"
#include "AAFDefUIDs.h"

extern "C" const aafClassID_t CLSID_EnumAAFMobSlots;
extern "C" const aafClassID_t CLSID_EnumAAFTaggedValues;
extern "C" const aafClassID_t CLSID_AAFFindSourceInfo;
extern "C" const aafClassID_t CLSID_AAFTypeDefString;

ImplAAFMob::ImplAAFMob ()
: _mobID(			PID_Mob_MobID,			"MobID"),
  _name(			PID_Mob_Name,			"Name"),
  _creationTime(    PID_Mob_CreationTime,	"CreationTime"),
  _lastModified(    PID_Mob_LastModified,	"LastModified"),
  _slots(			PID_Mob_Slots,			"Slots"),
  _userComments(	PID_Mob_UserComments,	"UserComments")
{
	_persistentProperties.put(_mobID.address());
	_persistentProperties.put(_name.address());
	_persistentProperties.put(_creationTime.address());
	_persistentProperties.put(_lastModified.address());
	_persistentProperties.put(_slots.address());
	_persistentProperties.put(_userComments.address());
	(void)aafMobIDNew(&_mobID);		// Move this out of constructor when we get 2-stage create
	AAFGetDateTime(&_creationTime);
	AAFGetDateTime(&_lastModified);
}


ImplAAFMob::~ImplAAFMob ()
{
	// Release all of the mob slot pointers.
	size_t size = _slots.getSize();
	for (size_t i = 0; i < size; i++)
	{
		ImplAAFMobSlot *pSlot = _slots.setValueAt(0, i);
		if (pSlot)
		{
			pSlot->ReleaseReference();
		}
	}

	if(_userComments.isPresent())
	{
		size = _userComments.getSize();
		for (size_t j = 0; j < size; j++)
		{
			ImplAAFTaggedValue* pTaggedValue = _userComments.setValueAt(0, j);
			if (pTaggedValue)
				pTaggedValue->ReleaseReference();
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
	AAFRESULT aafError = AAFRESULT_SUCCESS;

	if (NULL == pSlot)
		return AAFRESULT_NULL_PARAM;

	_slots.appendValue(pSlot);
	pSlot->AcquireReference();
	return aafError;
}

//****************
// RemoveSlot()
//
AAFRESULT STDMETHODCALLTYPE
	ImplAAFMob::RemoveSlot
        (ImplAAFMobSlot *  pSlot)  //@parm [in,out] Mob Name length
{
  return AAFRESULT_NOT_IN_CURRENT_VERSION;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::GetMobID (aafUID_t *pMobID)
{
	AAFRESULT aafError = AAFRESULT_SUCCESS;

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
    ImplAAFMob::GetName (aafWChar *pName,
	aafInt32 bufSize)
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
        (aafInt32 *  pSize)  //@parm [in,out] Mob Name length
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
	aafAssert(lastModified != NULL, NULL, AAFRESULT_NULL_PARAM);
	*lastModified = _lastModified;
	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::GetCreateTime (aafTimeStamp_t *creationTime)
{
	aafAssert(creationTime != NULL, NULL, AAFRESULT_NULL_PARAM);

	*creationTime = _creationTime;

	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::GetMobInfo (aafTimeStamp_t *lastModified,
                           aafTimeStamp_t *creationTime,
							aafWChar *name,
							aafInt32 nameLen)
{
	aafAssert(creationTime != NULL, NULL, AAFRESULT_NULL_PARAM);
	aafAssert(creationTime != NULL, NULL, AAFRESULT_NULL_PARAM);
	aafAssert(creationTime != NULL, NULL, AAFRESULT_NULL_PARAM);
    XPROTECT()
	{
		CHECK(GetCreateTime (creationTime));
		CHECK(GetModTime (creationTime));
		CHECK(GetName(name, nameLen));
	}
	XEXCEPT
	XEND;

	return(AAFRESULT_SUCCESS); 
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::GetNumSlots (aafNumSlots_t *pNumSlots)
{
   size_t numSlots;

	_slots.getSize(numSlots);
	
	*pNumSlots = numSlots;

	return(AAFRESULT_SUCCESS);
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::SetModTime (aafTimeStamp_t *modTime)
{
	_lastModified = *modTime;

	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::SetMobID (aafUID_t *newMobID)
{
	AAFRESULT				hr = AAFRESULT_SUCCESS;
	ImplAAFMob				*mobPtr = NULL;
	ImplAAFHeader			*head = NULL;
	ImplAAFContentStorage	*cstore = NULL;

	if(newMobID == NULL)
		return(AAFRESULT_NULL_PARAM);

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
					_mobID = *newMobID;
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
		 _mobID = *newMobID;

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
    ImplAAFMob::SetName (aafWChar *pName)
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
	*pMobKind = kAllMob;				// Abstract superclass, only match "all"
	return AAFRESULT_SUCCESS;
}

 AAFRESULT STDMETHODCALLTYPE
   ImplAAFMob::AppendNewSlot (ImplAAFSegment *segment,
                           aafSlotID_t  slotID,
                           aafWChar *slotName,
                           ImplAAFMobSlot **newSlot)
{
	ImplAAFMobSlot * tmpSlot = NULL;
	aafLength_t length = CvtInt32toLength(0, length);
	aafLength_t	mobLength = CvtInt32toLength(0, mobLength);
  ImplAAFDictionary *pDictionary = NULL;
	AAFRESULT aafError = AAFRESULT_SUCCESS;

	*newSlot = NULL;
	aafAssert((segment != NULL), _file, AAFRESULT_NULL_PARAM);

	XPROTECT()
	{
		CHECK(GetDictionary(&pDictionary));
		tmpSlot = (ImplAAFMobSlot *)pDictionary->CreateImplObject(AUID_AAFMobSlot);
		if(tmpSlot == NULL)
			RAISE(AAFRESULT_NOMEMORY);
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
		if(pDictionary != NULL)
			pDictionary->ReleaseReference();
		return(XCODE());
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
        (aafRational_t  editRate,   //@parm [in] Edit rate property value
		 ImplAAFSegment * segment,   //@parm [in] Segment to append as slot component
		 aafSlotID_t  slotID,   //@parm [in] The Slot ID
         aafWChar *  slotName,   //@parm [in] Slot Name (optional)
		 aafPosition_t  origin,
		 ImplAAFTimelineMobSlot ** newSlot)  //@parm [out] Newly created slot
{
	ImplAAFTimelineMobSlot	*aSlot = NULL;
	ImplAAFMobSlot			*tmpSlot = NULL;
  ImplAAFDictionary *pDictionary = NULL;
///fLength_t length = CvtInt32toLength(0, length);
///	aafLength_t	mobLength = CvtInt32toLength(0, mobLength);
	AAFRESULT aafError = AAFRESULT_SUCCESS;

	*newSlot = NULL;
	aafAssert((segment != NULL), _file, AAFRESULT_NULL_PARAM);

	XPROTECT()
	  {
		CHECK(GetDictionary(&pDictionary));
		aSlot = (ImplAAFTimelineMobSlot *)pDictionary->CreateImplObject(AUID_AAFTimelineMobSlot);
		if (NULL == aSlot)
			return (AAFRESULT_NOMEMORY);
		pDictionary->ReleaseReference();
		pDictionary = NULL;

		CHECK(aSlot->SetSegment(segment));
		CHECK(aSlot->SetSlotID(slotID));
		CHECK(aSlot->SetName(slotName));
		CHECK(aSlot->SetEditRate(&editRate));
		CHECK(aSlot->SetOrigin(origin));

		/* Append new slot to mob */
		tmpSlot = aSlot;
		_slots.appendValue(tmpSlot);

	  } /* XPROTECT */

	XEXCEPT
	  {
		if (aSlot)
			aSlot->ReleaseReference();
		if(pDictionary != NULL)
			pDictionary->ReleaseReference();
		return(XCODE());
	  }
	XEND;

	*newSlot = aSlot;
	if (aSlot)
		aSlot->AcquireReference();

	return(AAFRESULT_SUCCESS);
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::EnumAAFAllMobSlots (ImplEnumAAFMobSlots **ppEnum)
{
	ImplEnumAAFMobSlots		*theEnum = (ImplEnumAAFMobSlots *)CreateImpl (CLSID_EnumAAFMobSlots);
	if(theEnum == NULL)
		return(E_FAIL);

	// !!!Does not obey search criteria yet
	XPROTECT()
	{
		CHECK(theEnum->SetEnumMob(this));
		CHECK(theEnum->Reset());
		*ppEnum = theEnum;
	}
	XEXCEPT
	{
		if (theEnum)
			theEnum->ReleaseReference();
		return(XCODE());
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::AppendComment ( aafWChar*  pTagName,
								aafWChar*  pComment)
{
	ImplAAFTaggedValue*			pTaggedValue = NULL;
	ImplEnumAAFTaggedValues*	pEnum = NULL;
	
	aafWChar					oldTagName[64];
	aafBool						commentFound = AAFFalse;
	aafUID_t					stringTypeUID = CLSID_AAFTypeDefString;
	aafUInt32					numComments = 0;
	ImplAAFDictionary *pDictionary = NULL;
	
	if (pTagName == NULL || pComment == NULL)
		return AAFRESULT_NULL_PARAM;


	XPROTECT()
	{
		CHECK(GetNumComments(&numComments));
		if (numComments > 0)
		{
			CHECK(EnumAAFAllMobComments(&pEnum));
			CHECK(pEnum->NextOne(&pTaggedValue));
			while(pTaggedValue)
			{
				CHECK(pTaggedValue->GetName(oldTagName, sizeof(oldTagName)));
				if (wcscmp(oldTagName, pTagName) == 0)
				{
					commentFound = AAFTrue;
					break;
				}
				pTaggedValue->ReleaseReference();
				pTaggedValue = NULL;
				pEnum->NextOne(&pTaggedValue);
			}
			pEnum->ReleaseReference();
		}
		if (commentFound)
		{
			// Update existing comment
			CHECK(pTaggedValue->SetValue((wcslen(pComment)*sizeof(aafWChar)+2), (aafDataValue_t)pComment));
			pTaggedValue->ReleaseReference();
		}
		else
		{
			// Create a new comment and add it to the list!
			CHECK(GetDictionary(&pDictionary));
			pTaggedValue = (ImplAAFTaggedValue *)pDictionary->CreateImplObject(AUID_AAFTaggedValue);
			pDictionary->ReleaseReference();
			pDictionary = NULL;
			CHECK(pTaggedValue->Initialize(pTagName, &stringTypeUID));
			CHECK(pTaggedValue->SetValue((wcslen(pComment)*sizeof(aafWChar)+2), (aafDataValue_t)pComment));
			_userComments.appendValue(pTaggedValue);
		}
	}
	XEXCEPT
	{
		if(pDictionary != NULL)
			pDictionary->ReleaseReference();
		return(XCODE());
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
        (aafMobComment_t *  comment)
{
  return AAFRESULT_NOT_IN_CURRENT_VERSION;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::GetNumComments (aafUInt32*  pNumComments)
{
	size_t	numComments;

	if (pNumComments == NULL)
		return AAFRESULT_NULL_PARAM;

	if(!_userComments.isPresent())
	{	// If the userComments property is not present then
		// number of user comments is zero!
		*pNumComments = 0; //return AAFRESULT_PROP_NOT_PRESENT;
	}
	else
	{
		_userComments.getSize(numComments);

		*pNumComments = numComments;
	}
		
	return(AAFRESULT_SUCCESS);
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::EnumAAFAllMobComments (ImplEnumAAFTaggedValues** ppEnum)
{
	if(!_userComments.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
		
	ImplEnumAAFTaggedValues*	theEnum = (ImplEnumAAFTaggedValues *)CreateImpl(CLSID_EnumAAFTaggedValues);

	XPROTECT()
	{
		CHECK(theEnum->SetEnumStrongProperty(this, &_userComments));
		CHECK(theEnum->Reset());
		*ppEnum = theEnum;
	}
	XEXCEPT
	{
		if (theEnum)
			theEnum->ReleaseReference();
		return(XCODE());
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::OffsetToMobTimecode (ImplAAFSegment *tcSlotID,
                           aafPosition_t *offset,
                           aafTimecode_t *result)
{
	ImplAAFMobSlot		*slot = NULL;
	ImplAAFPulldown		*pdwn = NULL;
	ImplAAFSegment		*pdwnInput = NULL;
	ImplEnumAAFMobSlots *iter = NULL;
	ImplAAFSegment		*seg = NULL;
	aafTimecode_t		timecode;
	aafBool				reverse = AAFFalse;
	aafUInt32			frameOffset;
	aafUID_t			dataDefID;
	aafPosition_t		newStart;
	aafInt32			start32;
	AAFRESULT			aafError = AAFRESULT_SUCCESS;
	
	memset(result, 0, sizeof(aafTimecode_t));
	memset(&timecode, 0, sizeof(aafTimecode_t));
	result->startFrame = 0;
	
	XPROTECT()
	{
		
		/* Find timecode slot in mob */
		//!!!			iterHdl = new AAFIterate(_file);
		CHECK(EnumAAFAllMobSlots (&iter));
		while(iter->NextOne(&slot) == AAFRESULT_SUCCESS)
		{
			CHECK(slot->GetSegment(&seg));
			
			/* Verify that it's a timecode slot by looking at the
			* datakind of the slot segment. 
			*/
			CHECK(seg->GetDataDef(&dataDefID));
			
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
				reverse = AAFFalse;
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
		if(slot != NULL)
			slot->ReleaseReference();
		if(pdwn != NULL)
			pdwn->ReleaseReference();
		if(pdwnInput != NULL)
			pdwnInput->ReleaseReference();
		return(XCODE());
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::FindSlotBySlotID (aafSlotID_t	slotID,
                           ImplAAFMobSlot **destSlot)
{
	aafInt32		loop, numSlots;
	ImplAAFMobSlot	*tmpSlot = NULL;
	aafSlotID_t	tmpSlotID;
	aafBool			foundSlot = AAFFalse;

	XPROTECT()
	{
		*destSlot = NULL;
		
		// For size entries the valid positions are 0 .. size - 1
		CHECK(GetNumSlots(&numSlots));
		for (loop = 0; loop < numSlots; loop++)
		{
			_slots.getValueAt(tmpSlot, loop);
			CHECK(tmpSlot->GetSlotID(&tmpSlotID));
			if (tmpSlotID == slotID)
			{
				foundSlot = AAFTrue;
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
	ImplAAFComponent		*subSegment = NULL;
	ImplAAFSegment			*seg = NULL;
	aafTimecode_t 			timecode;
	aafMediaCriteria_t		mediaCrit;
	ImplAAFFindSourceInfo	*sourceInfo;
	aafRational_t			editRate;
	aafPosition_t			frameOffset64;
	aafUID_t				datakind;
	aafLength_t				zeroLen = CvtInt32toLength(0, zeroLen);
	ImplEnumAAFMobSlots		*slotIter = NULL;
	ImplAAFDictionary		*dict = NULL;
	ImplAAFDataDef			*dataDef = NULL;
	ImplAAFMob				*tapeMob = NULL;

	memset(result, 0, sizeof(aafTimecode_t));
	memset(&timecode, 0, sizeof(aafTimecode_t));
	result->startFrame = 0;
	
	XPROTECT()
	{
		CHECK(GetDictionary(&dict));
		mediaCrit.type = kAAFAnyRepresentation;
		CHECK(InternalSearchSource(*slotID, *offset, kTapeMob,
			&mediaCrit, NULL, &sourceInfo));
		
		CHECK(sourceInfo->GetMob(&tapeMob));
		CHECK(tapeMob->EnumAAFAllMobSlots (&slotIter));
		while(slotIter->NextOne((ImplAAFMobSlot**)&slot) == AAFRESULT_SUCCESS)
		{
			CHECK(slot->GetEditRate(&editRate));
			CHECK(slot->GetSegment(&seg));
			slot->ReleaseReference();
			slot = NULL;
			/* Verify that it's a timecode slot by looking at the
			* datakind of the slot segment.
			*/
			CHECK(seg->GetDataDef(&datakind));
			CHECK(dict->LookupDataDefintion(&datakind, &dataDef));
			aafUID_t	uid = DDEF_Timecode;
			aafBool		isTimecode;
			CHECK(dataDef->IsDataDefOf(&uid, &isTimecode));
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
		dict->ReleaseReference();
		dict = NULL;
		
		
		*result = timecode;
	} /* XPROTECT */
	XEXCEPT
	{
		if (tapeMob)
			dataDef->ReleaseReference();
		if (dataDef)
			dataDef->ReleaseReference();
		if (slotIter)
			slotIter->ReleaseReference();
		if (slot)
			slot->ReleaseReference();
		if (dict)
			dict->ReleaseReference();
		return(XCODE());
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
	AAFRESULT	aafError = AAFRESULT_SUCCESS;
	aafBool found = AAFFalse;
	aafRational_t	editRate;
	aafLength_t	zeroLen;
	ImplAAFFindSourceInfo	*sourceInfo = NULL;
	ImplAAFMob	*tapeMob = NULL;
	
	CvtInt32toPosition(0, zero);
	CvtInt32toLength(0, zeroLen);
	
	XPROTECT()
	{
		CHECK(InternalSearchSource(slotID, zero, kTapeMob,
			NULL /* mediaCrit */, NULL, &sourceInfo));
		
		CHECK(sourceInfo->GetMob(&tapeMob));
		CHECK(tapeMob->EnumAAFAllMobSlots(&iterHdl));
		while(iterHdl->NextOne(&slot) == AAFRESULT_SUCCESS)
		{
			timelineSlot = dynamic_cast<ImplAAFTimelineMobSlot*>(slot);
			if(timelineSlot != NULL)
			{
				CHECK(timelineSlot->GetEditRate(&editRate));
				CHECK(timelineSlot->GetSegment(&seg));
			
				if(seg->SegmentTCToOffset(&timecode, &editRate, result) == AAFRESULT_SUCCESS)
					found = AAFTrue;
				
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
		if (sourceInfo)
			sourceInfo->ReleaseReference();	
		if(iterHdl != NULL)
			iterHdl->ReleaseReference();
		if(sequIter != NULL)
			sequIter->ReleaseReference();
		if(slot != NULL)
			slot->ReleaseReference();
		if(pdwn != NULL)
			pdwn->ReleaseReference();
		if(seg != NULL)
			seg->ReleaseReference();
		if(pdwnInput != NULL)
			pdwnInput->ReleaseReference();
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}




AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::Copy (aafWChar *  /*destMobName*/,
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
		return(XCODE());
	  }
	XEND;

	*destMob = newMob;
	return(AAFRESULT_SUCCESS);
#else
  return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFMob::ChangeRef (aafUID_t *oldMobID,
                           aafUID_t *newMobID)
{
	ImplEnumAAFMobSlots		*iter = NULL;
	ImplAAFMobSlot			*slot = NULL;

	XPROTECT()
	{
		CHECK(EnumAAFAllMobSlots (&iter));
		while(iter->NextOne(&slot) == AAFRESULT_SUCCESS)
		{
			CHECK(slot->ChangeContainedReferences(oldMobID, newMobID));
			slot->ReleaseReference();
			slot = NULL;
		}
	}
	XEXCEPT
	{
		if(slot)
			slot->ReleaseReference();
		if(iter)
			iter->ReleaseReference();
	}
	XEND;

	return(AAFRESULT_SUCCESS);
}



 /*************************************************************************
 * Function: CloneExternal()
 *
 *      This function clones the given mob in the source file into
 *      a destination mob in the destination file with the same Mob ID.
 *      If resolveDependencies is AAFTrue, it will also clone all mobs 
 *      referenced by the given source mob. 
 *
 *      If includeMedia is AAFTrue, it will also copy the media data 
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
    aafUID_t saveMobID, newMobID;
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
				CHECK(tmpDestMob->SetPrimary(AAFTrue));
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
		return(XCODE());
	  }
	XEND;

	*destMob = tmpDestMob;
	return(AAFRESULT_SUCCESS);
#else
  return AAFRESULT_NOT_IMPLEMENTED;
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
							  aafUID_t *pEssenceKind,
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
	ImplEnumAAFComponents	*compEnum = NULL;
	aafPosition_t			zeroPos;
  ImplAAFDictionary *pDictionary = NULL;

	XPROTECT()
	{
		CvtInt32toInt64(0, &zeroPos);
		CHECK(GetDictionary(&pDictionary));
		sclp = (ImplAAFSourceClip *)pDictionary->CreateImplObject(AUID_AAFSourceClip);
		if (NULL == sclp)
			RAISE(AAFRESULT_NOMEMORY);
		pDictionary->ReleaseReference();
		pDictionary = NULL;
		CHECK(sclp->Initialize(pEssenceKind, &srcRefLength, ref));
				
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
		if(sclp != NULL)
			sclp->ReleaseReference();
		if(slot != NULL)
			slot->ReleaseReference();
		if(slotSeg != NULL)
			slotSeg->ReleaseReference();
		if(slot != NULL)
			slot->ReleaseReference();
		if(cpnt != NULL)
			cpnt->ReleaseReference();
		if(pDictionary != NULL)
			pDictionary->ReleaseReference();
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
	aafBool					sourceFound = AAFFalse, foundTransition = AAFFalse;
	ImplAAFMob				*nextMob = NULL;
	aafInt32				nestDepth, pulldownPhase;
	aafPosition_t			zeroPos = CvtInt32toPosition(0, zeroPos);
	aafLength_t				cpntLen, nextLen, minLength, newLen;
	ImplAAFPulldown			*pulldownObj = NULL;
	aafSlotID_t				nextTrackID;
	ImplAAFFindSourceInfo	*sourceInfo = NULL ;
	ImplAAFComponent		*leafObj = NULL;
	ImplAAFOperationGroup	*effeObject;
	
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
		if (leafObj)
			leafObj->ReleaseReference();
		if (effeObject)
			effeObject->ReleaseReference();
		rootObj->ReleaseReference();
		track->ReleaseReference();
		sourceInfo->ReleaseReference();
	} /* XPROTECT */
	XEXCEPT
	{
		if(XCODE() == AAFRESULT_PARSE_EFFECT_AMBIGUOUS)
			sourceInfo->SetOperationGroup(effeObject);

		if (nextMob)
			nextMob->ReleaseReference();
		if (leafObj)
			leafObj->ReleaseReference();
		if (effeObject)
			effeObject->ReleaseReference();
		if (rootObj)
			rootObj->ReleaseReference();
		if (track)
			track->ReleaseReference();
		if (sourceInfo)
			sourceInfo->ReleaseReference();
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
		return(XCODE());
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
	aafBool					isMask = AAFFalse, reverse = AAFFalse;
	aafSourceRef_t			sourceRef;
	ImplAAFMob				*nextMob = NULL;
	aafUID_t				nullUID = NilMOBID;		// Need "isNIL" utility
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
		if (EqualAUID(&nullUID, &sourceRef.sourceID))
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
			reverse = AAFFalse;
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

//		sclp->ReleaseReference(); // causes 800400c8 to be returned from MasterMob::OpenEssence
	}
	XEXCEPT
	{
		if (nextTrack)
			nextTrack->ReleaseReference();
		if (nextMob)
			nextMob->ReleaseReference();
//		if (sclp)
//			sclp->ReleaseReference();
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
	aafBool					nextFoundSource = AAFFalse, foundTransition = AAFFalse;
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
		*foundSource = AAFFalse;
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
		if ((mobKind == kCompMob) ||
			(mobKind == kMasterMob) || 
			(mobKind == kFileMob) ||
			(mobKind == kTapeMob) ||
			(mobKind == kFilmMob))
		{
			*foundSource = (mobKind == tstKind ? AAFTrue : AAFFalse);
		}
		else if (mobKind == kAllMob)
			*foundSource = AAFTrue;
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
			track->ReleaseReference();
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
		if (leafObj)
			leafObj->ReleaseReference();
		if (effeObject)
			effeObject->ReleaseReference();
		rootObj->ReleaseReference();
		track->ReleaseReference();
	}
	XEXCEPT
	{
		if(XCODE() == AAFRESULT_PARSE_EFFECT_AMBIGUOUS)
			sourceInfo->SetOperationGroup(effeObject);

		if (nextMob)
			nextMob->ReleaseReference();
		if (leafObj)
			leafObj->ReleaseReference();
		if (effeObject)
			effeObject->ReleaseReference();
		if (rootObj)
			rootObj->ReleaseReference();
		if (track)
			track->ReleaseReference();
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}

AAFRESULT ImplAAFMob::ReconcileMobLength(void)
{
	return(AAFRESULT_ABSTRACT_CLASS);	// MUST call one of the subclasses
}


OMDEFINE_STORABLE(ImplAAFMob, AUID_AAFMob);



////////////////////////////////////////////////////////////////////////////////

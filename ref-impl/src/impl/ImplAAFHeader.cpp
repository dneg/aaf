
//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2007, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#include "AAFTypes.h"
#include "AAFResult.h"
#include "aafTable.h"
#include "aafErr.h"
#include "AAFUtils.h"
#include "ImplAAFModule.h"

#ifndef __ImplAAFMob_h__
#include "ImplAAFMob.h"
#endif

#ifndef __ImplEnumAAFMobs_h__
#include "ImplEnumAAFMobs.h"
#endif

#ifndef __ImplEnumAAFEssenceData_h__
#include "ImplEnumAAFEssenceData.h"
#endif

#ifndef __ImplAAFDictionary_h__
#include "ImplAAFDictionary.h"
#endif

#ifndef __ImplAAFIdentification_h__
#include "ImplAAFIdentification.h"
#endif

#ifndef __ImplEnumAAFIdentifications_h__
#include "ImplEnumAAFIdentifications.h"
#endif

#ifndef __ImplAAFHeader_h__
#include "ImplAAFHeader.h"
#endif

#include "ImplAAFFile.h"
#include "ImplAAFBuiltinDefs.h"
#include "ImplAAFFileDescriptor.h"
#include "ImplAAFSourceMob.h"

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include "ImplAAFSmartPointer.h"
typedef ImplAAFSmartPointer<ImplAAFIdentification>
    ImplAAFIdentificationSP;
typedef ImplAAFSmartPointer<ImplEnumAAFIdentifications>
    ImplEnumAAFIdentificationsSP;

#include "OMAssertions.h"
#include <wchar.h>

#include "ImplAAFObjectCreation.h"
#include "OMSetPropertyIterator.h"

#define DEFAULT_NUM_MOBS				1000
#define DEFAULT_NUM_DATAOBJ			200
#define DEFAULT_NUM_DATAKIND_DEFS	100
#define DEFAULT_NUM_EFFECT_DEFS		100

extern "C" const aafClassID_t CLSID_EnumAAFIdentifications;

const aafUID_t NIL_UID = { 0,0,0,{0,0,0,0,0,0,0,0} };


ImplAAFHeader::ImplAAFHeader ()
: _byteOrder(         PID_Header_ByteOrder,          L"ByteOrder"),
  _lastModified(      PID_Header_LastModified,       L"LastModified"),
  _identificationList(PID_Header_IdentificationList, L"IdentificationList"),
  _contentStorage(		PID_Header_Content,	L"Content"),
  _dictionary(PID_Header_Dictionary,	L"Dictionary"),
  _fileRev(PID_Header_Version,		L"Version"),
  _objectModelVersion(PID_Header_ObjectModelVersion, L"ObjectModelVersion"),
  _operationalPattern(PID_Header_OperationalPattern, L"OperationalPattern"),
  _essenceContainers(PID_Header_EssenceContainers, L"EssenceContainers"),
  _descriptiveSchemes(PID_Header_DescriptiveSchemes, L"DescriptiveSchemes")
#if 0 // tjb - not yet
, _primaryMob(PID_Header_PrimaryMob, L"PrimaryMob", L"/Header/Content/Mobs", PID_Mob_MobID)
#endif
{
  _persistentProperties.put(_byteOrder.address());
  _persistentProperties.put(_lastModified.address());
  _persistentProperties.put(_identificationList.address());
  _persistentProperties.put(_contentStorage.address());
  _persistentProperties.put(_dictionary.address());
  _persistentProperties.put(_fileRev.address());
  _persistentProperties.put(_objectModelVersion.address());
  _persistentProperties.put(_operationalPattern.address());
  _persistentProperties.put(_essenceContainers.address());
  _persistentProperties.put(_descriptiveSchemes.address());
#if 0 // tjb - not yet
  _persistentProperties.put(_primaryMob.address());
#endif

	_toolkitRev = AAFReferenceImplementationVersion;
	_file = NULL;
}


ImplAAFHeader::~ImplAAFHeader ()
{
	// Release all of the id pointers in the id list.
	//
	aafUInt32 count = _identificationList.count();
	for (aafUInt32 i = 0; i < count; i++) {
		ImplAAFIdentification *pIdent = _identificationList.clearValueAt(i);

		if (pIdent) {
		  pIdent->ReleaseReference();
		  pIdent = 0;
		}
	}

	// Release the content storage pointer.
	ImplAAFContentStorage *contentStorage = _contentStorage.clearValue();
	if (contentStorage) {
	  contentStorage->ReleaseReference();
	  contentStorage = 0;
	}

	// Release the dictionary pointer.
	ImplAAFDictionary *dictionary = _dictionary.clearValue();
	if (dictionary) {
	  dictionary->ReleaseReference();
	  dictionary = 0;
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::LookupMob (aafMobID_constref mobID,
                           ImplAAFMob **ppMob)
{
    ImplAAFContentStorage *cstore = NULL;

    if (! ppMob)
	  {
		return AAFRESULT_NULL_PARAM;
	  }
	XPROTECT()
	{
		cstore = GetContentStorage();		// Does not AddRef
		CHECK(cstore->LookupMob(mobID, ppMob));
	}
	XEXCEPT
	XEND

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::CountMobs (aafMobKind_t mobKind,
                           aafNumSlots_t *pNumMobs)
{
    ImplAAFContentStorage *cstore = NULL;

    if (! pNumMobs)
	  {
		return AAFRESULT_NULL_PARAM;
	  }
	XPROTECT()
	{
		cstore = GetContentStorage();		// Does not AddRef
		CHECK(cstore->CountMobs(mobKind, pNumMobs));
	}
	XEXCEPT
	XEND

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::GetMobs (aafSearchCrit_t *pSearchCriteria,
                           ImplEnumAAFMobs **ppEnum)
{
    ImplAAFContentStorage *cstore = NULL;
    if (! ppEnum)
	  {
		return AAFRESULT_NULL_PARAM;
	  }
	XPROTECT()
	{
		cstore = GetContentStorage();		// Does not AddRef
		CHECK(cstore->GetMobs(pSearchCriteria, ppEnum));
	}
	XEXCEPT
	{
	}
	XEND

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::AddMob (ImplAAFMob *pMob)
{
    ImplAAFContentStorage *cstore = NULL;

    if (! pMob)
	  {
		return AAFRESULT_NULL_PARAM;
	  }
	XPROTECT()
	{
		cstore = GetContentStorage();		// Does not AddRef
		CHECK(cstore->AddMob(pMob));
	}
	XEXCEPT
	XEND

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::RemoveMob (ImplAAFMob *pMob)
{
    ImplAAFContentStorage *cstore = NULL;

    if (! pMob)
		return AAFRESULT_NULL_PARAM;
	  
 	if (! pMob->attached())
		return AAFRESULT_MOB_NOT_FOUND;	

	XPROTECT()
	{
		cstore = GetContentStorage();		// Does not AddRef
		CHECK(cstore->RemoveMob(pMob));
	}
	XEXCEPT
	XEND

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::CountEssenceData(aafUInt32 *  pNumEssenceData)
{
    ImplAAFContentStorage *cstore = NULL;

	if(pNumEssenceData == NULL)
		 return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		cstore = GetContentStorage();		// Does not AddRef
		CHECK(cstore->CountEssenceData(pNumEssenceData));
	}
	XEXCEPT
	XEND

	return AAFRESULT_SUCCESS;
}


// Implementer note:
// based on omfmIsMediaDataPresent
//
AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::IsEssenceDataPresent (aafMobID_constref fileMobID,
                           aafFileFormat_t fmt,
                           aafBool *pResult)
{
    ImplAAFContentStorage *cstore = NULL;
    if ((! pResult))
	  {
		return AAFRESULT_NULL_PARAM;
	  }
	XPROTECT()
	{
		cstore = GetContentStorage();		// Does not AddRef
		CHECK(cstore->IsEssenceDataPresent(fileMobID, fmt, pResult));
	}
	XEXCEPT
	XEND

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::EnumEssenceData (ImplEnumAAFEssenceData **ppEnum)
{
    ImplAAFContentStorage *cstore = NULL;
	
	if (! ppEnum)
	  {
		return AAFRESULT_NULL_PARAM;
	  }
	XPROTECT()
	{
		cstore = GetContentStorage();		// Does not AddRef
		CHECK(cstore->EnumEssenceData(ppEnum));
	}
	XEXCEPT
	XEND

	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::AddEssenceData (ImplAAFEssenceData * pEssenceData)
{
    ImplAAFContentStorage *cstore = NULL;
	if (! pEssenceData)
	{
		 return AAFRESULT_NULL_PARAM;
	}
	XPROTECT()
	{
		cstore = GetContentStorage();		// Does not AddRef
		CHECK(cstore->AddEssenceData(pEssenceData));
	}
	XEXCEPT
	XEND

	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::RemoveEssenceData (ImplAAFEssenceData * pEssenceData)
{
    ImplAAFContentStorage *cstore = NULL;
	if (! pEssenceData)
		return AAFRESULT_NULL_PARAM;
	
	if (! pEssenceData->attached())
		return AAFRESULT_ESSENCE_NOT_FOUND;

	XPROTECT()
	{
		cstore = GetContentStorage();		// Does not AddRef
		CHECK(cstore->RemoveEssenceData(pEssenceData));
	}
	XEXCEPT
	XEND

	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::LookupEssenceData (aafMobID_constref mobID,
                           ImplAAFEssenceData **ppEssenceData)
{
    ImplAAFContentStorage *cstore = NULL;

    if (! ppEssenceData)
	  {
		return AAFRESULT_NULL_PARAM;
	  }
	XPROTECT()
	{
		cstore = GetContentStorage();		// Does not AddRef
		CHECK(cstore->LookupEssenceData(mobID, ppEssenceData));
	}
	XEXCEPT
	XEND

	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::GetContentStorage (ImplAAFContentStorage ** ppContentStorage)
{
  if (! ppContentStorage)
	{
	  return AAFRESULT_NULL_PARAM;
	}

	*ppContentStorage = GetContentStorage(); // does not increment reference!
	if (*ppContentStorage) {
		(*ppContentStorage)->AcquireReference();
		return AAFRESULT_SUCCESS;
	} else {
		return AAFRESULT_NULLOBJECT; // trr - is there a better error code?
	}
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::GetDictionary (ImplAAFDictionary ** ppDictionary) const
{
  if (! ppDictionary)
	{
	  return AAFRESULT_NULL_PARAM;
	}

  *ppDictionary = GetDictionary();
  if (*ppDictionary)
  {
    (*ppDictionary)->AcquireReference();
    return AAFRESULT_SUCCESS;
  }
  else
    return AAFRESULT_NULLOBJECT;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::GetLastIdentification (ImplAAFIdentification ** ppIdentification)
{
  if (! ppIdentification)
	{
	  return AAFRESULT_NULL_PARAM;
	}

  AAFRESULT result;
  aafUInt32 count = _identificationList.count();

  if (count > 0) {
    // For count entries the valid positions are 0 .. count - 1
    // get the last one in the vector.
    _identificationList.getValueAt(*ppIdentification, count - 1);

		// We are returning a non-null reference.
		if (*ppIdentification) {
			(*ppIdentification)->AcquireReference();
			result = AAFRESULT_SUCCESS;
		} else {
			result = AAFRESULT_NO_MORE_OBJECTS; // trr - is there a better error code?
		}
  } else {
    *ppIdentification = 0;
    result = AAFRESULT_INCONSISTANCY;
  }
  return result;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::LookupIdentification (const aafUID_t & generation,
                           ImplAAFIdentification ** ppIdentification)
{
  if ((! ppIdentification))
	{
	  return AAFRESULT_NULL_PARAM;
	}

  AAFRESULT hr;

  ImplEnumAAFIdentificationsSP pEnumIds;
  hr = GetIdentifications (&pEnumIds);
  if (AAFRESULT_FAILED (hr)) return hr;

  ImplAAFIdentificationSP pTestId;
  while (AAFRESULT_SUCCEEDED (pEnumIds->NextOne (&pTestId)))
	{
	  aafUID_t testGen;
	  hr = pTestId->GetGenerationID (&testGen);
	  if (AAFRESULT_FAILED (hr)) return hr;
	  if (EqualAUID (&testGen, &generation))
		{
		  ASSERTU (ppIdentification);
		  *ppIdentification = pTestId;
		  ASSERTU (*ppIdentification);
		  (*ppIdentification)->AcquireReference ();
		  return AAFRESULT_SUCCESS;
		}
	}
  return AAFRESULT_OBJECT_NOT_FOUND;
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFHeader::CountIdentifications
(aafUInt32 *  pNumIdents)
{
	if (! pNumIdents)
	{
		return AAFRESULT_NULL_PARAM;
	}

 	aafUInt32 count = _identificationList.count();
	*pNumIdents = count;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::GetIdentifications (ImplEnumAAFIdentifications ** ppEnum)
{
	if (NULL == ppEnum)
		return AAFRESULT_NULL_PARAM;
	*ppEnum = 0;
	
	ImplEnumAAFIdentifications *theEnum = (ImplEnumAAFIdentifications *)CreateImpl (CLSID_EnumAAFIdentifications);
	
	XPROTECT()
	{
		OMStrongReferenceVectorIterator<ImplAAFIdentification>* iter = 
			new OMStrongReferenceVectorIterator<ImplAAFIdentification>(_identificationList);
		if(iter == 0)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(theEnum->Initialize(&CLSID_EnumAAFIdentifications, this, iter));
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
ImplAAFHeader::GetIdentificationAt
(aafUInt32 index,
 ImplAAFIdentification ** ppIdentification)
{
  if (! ppIdentification)
	return AAFRESULT_NULL_PARAM;

  aafUInt32 max;
  CountIdentifications(&max);
  if (index >= max)
	return AAFRESULT_BADINDEX;

  ImplAAFIdentification * pId = 0;
  _identificationList.getValueAt(pId, index);
  ASSERTU (pId);
  pId->AcquireReference ();

  ASSERTU (ppIdentification);
  *ppIdentification = pId;
  // Let the ref count pass from pId to *ppIdentification.
  return AAFRESULT_SUCCESS;
}



AAFRESULT 
    ImplAAFHeader::AddIdentificationObject
(aafProductIdentification_constptr pIdent)
{
	ImplAAFIdentification *		identObj;
	aafProductIdentification_t ident;

	XPROTECT()
	{		
		if(pIdent == (aafProductIdentification_t *)NULL)
		{
			ident.companyName = L"Unknown";
			ident.productName = L"Unknown";
			ident.productVersionString = (aafWChar*)NULL;
			ident.productID = NIL_UID;
			ident.platform = (aafWChar*)NULL;
			ident.productVersion = 0;
		}
		else
		{
		    ident = *pIdent;
		}
		
	XASSERT(pIdent != NULL, AAFRESULT_NEED_PRODUCT_IDENT);

    if (ident.productVersionString == 0) {
      ident.productVersionString = L"Unknown version";
    }

    // Get the dictionary so that we can use the factory
    // method to create the identification.
    ImplAAFDictionary *pDictionary = GetDictionary();
    if (NULL == pDictionary)
      CHECK(AAFRESULT_NOMEMORY);
    CHECK(pDictionary->GetBuiltinDefs()->cdIdentification()->
		  CreateInstance((ImplAAFObject **)&identObj));
    if (NULL == identObj)
      CHECK(AAFRESULT_NOMEMORY);
	CHECK(identObj->Initialize(ident.companyName,
							   ident.productName,
							   ident.productVersionString,
							   ident.productID));

	if (ident.productVersion)
	  {
		CHECK (identObj->SetProductVersion (*ident.productVersion));
	  }

    _identificationList.appendValue(identObj);
 
	}
	XEXCEPT
	{
	}
	XEND
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT 
    ImplAAFHeader::AppendIdentification (ImplAAFIdentification * pIdent)
{
	if (! pIdent)
	{
		return AAFRESULT_NULL_PARAM;
	}

	if (pIdent->attached())
	    return AAFRESULT_OBJECT_ALREADY_ATTACHED;

	_identificationList.appendValue(pIdent);
	pIdent->AcquireReference();

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::GetRefImplVersion (aafProductVersion_t *pToolkitVersion)
{
  if (! pToolkitVersion)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  *pToolkitVersion = _toolkitRev;
  return (AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::GetFileRevision (aafVersionType_t *pRevision)
{
  if (! pRevision)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  *pRevision = _fileRev;
  return (AAFRESULT_SUCCESS);
}

void ImplAAFHeader::SetFileRevision (aafVersionType_t revision)
{
  _fileRev = revision;
}
	
AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::GetLastModified (aafTimeStamp_t * pTimeStamp)
{
  if (! pTimeStamp)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  *pTimeStamp = _lastModified;
  return AAFRESULT_SUCCESS;
}

AAFRESULT ImplAAFHeader::SetModified(void)		// To NOW
{
	aafTimeStamp_t	now = { {0,0,0}, {0,0,0,0} };

	AAFGetDateTime(&now);
	_lastModified = now;
	return (AAFRESULT_SUCCESS);
}

void ImplAAFHeader::SetByteOrder(const aafInt16 byteOrder)
{
	_byteOrder = byteOrder;
}

void ImplAAFHeader::SetDictionary(ImplAAFDictionary *pDictionary)
{
  if( !pDictionary )
	return; // AAFRESULT_NULL_PARAM

  if( pDictionary->attached() )
	return; // AAFRESULT_OBJECT_ALREADY_ATTACHED

  _dictionary = pDictionary;
  if (pDictionary)
    pDictionary->AcquireReference();
}

AAFRESULT ImplAAFHeader::SetToolkitRevisionCurrent()
{
	_toolkitRev = AAFReferenceImplementationVersion;
	return (AAFRESULT_SUCCESS);
}


// trr - NOTE: Eventhough this method returns a reference counted object it
// does NOT bump the reference count. Currently only other file that calls
// this method is ImplAAFMob.cpp. There is another version the conforms to our other API guidlines:
// AAFRESULT GetContentStorage(ImplAAFContentStorage **ppContentStorage);
// 
ImplAAFContentStorage *ImplAAFHeader::GetContentStorage()
{
  ImplAAFContentStorage	*result = _contentStorage;

  // Create the content storage object if it does not exist.
  if (NULL == result)
  { // Get the dictionary so that we can use the factory
    // method to create the identification.
    ImplAAFDictionary *pDictionary = GetDictionary();
    if (NULL != pDictionary)
	  {
		pDictionary->GetBuiltinDefs()->cdContentStorage()->
		  CreateInstance ((ImplAAFObject **)&result);
		_contentStorage = result;
	  }
  }

  return(result);
}

// Fill in when dictionary property is supported.
ImplAAFDictionary *ImplAAFHeader::GetDictionary() const
{
  ImplAAFDictionary	*result = _dictionary;

  // Note - in the case where this is the first GetDictionary() on
  // Header (in order to initialize the OM properties on the
  // newly-created Header object), the _dictionary property won't be
  // set up yet.  If that's the case, get the dictionary the
  // old-fashioned way (through AAFObject).
  if (! result)
	{
	  AAFRESULT hr = ImplAAFObject::GetDictionary(&result);
	  ASSERTU (AAFRESULT_SUCCEEDED (hr));
	  ASSERTU (result);
	  // clients of GetDictionary(void) expect the dictionary to *not*
	  // be reference-counted.
	  aafUInt32 refcnt = result->ReleaseReference ();
	  // make sure at least one reference remains.
	  ASSERTU (refcnt > 0);
#if 1 // HACK4MEIP2
	  const_cast<ImplAAFHeader*>(this)->_dictionary = result;
	  const_cast<ImplAAFHeader*>(this)->_dictionary->AcquireReference();
#endif
	}
  return(result);
}


bool ImplAAFHeader::IsObjectModelVersionPresent () const
{
  return _objectModelVersion.isPresent ();
}


aafUInt32 ImplAAFHeader::GetObjectModelVersion () const
{
  ASSERTU (IsObjectModelVersionPresent());
  return _objectModelVersion;
}


void ImplAAFHeader::SetObjectModelVersion (aafUInt32 version)
{
  _objectModelVersion = version;
  ASSERTU (IsObjectModelVersionPresent());
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::GetStoredByteOrder (eAAFByteOrder_t *  pOrder)
{
  if (!pOrder)
    return AAFRESULT_NULL_PARAM;

  if (_byteOrder == 0x4d4d) // 'MM'
  {
    *pOrder = kAAFByteOrderBig;
  }
  else  // 'II'
  {
    *pOrder = kAAFByteOrderLittle;
  }

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::GetNativeByteOrder (eAAFByteOrder_t *  pOrder)
{
  if (!pOrder)
    return AAFRESULT_NULL_PARAM;

	OMByteOrder byteOrder = hostByteOrder();
	if (byteOrder == littleEndian)
  {
		*pOrder = kAAFByteOrderLittle;
	} 
  else 
  {
		*pOrder = kAAFByteOrderBig;
	}

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::GetPrimaryMob (ImplAAFMob** /*pPrimaryMob*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::SetPrimaryMob (ImplAAFMob* /*pPrimaryMob*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

  
  
AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::GetOperationalPattern (aafUID_t* pOperationalPattern)
{
  if (!pOperationalPattern)
    return AAFRESULT_NULL_PARAM;

  if (!_operationalPattern.isPresent())
    return AAFRESULT_PROP_NOT_PRESENT;


  *pOperationalPattern = _operationalPattern;


  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::SetOperationalPattern (const aafUID_t& operationalPatternID)
{
  _operationalPattern = operationalPatternID;


  return AAFRESULT_SUCCESS;
}



// Update the contents of _essenceContainers set.
AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::UpdateEssenceContainers ()
{
  if (_essenceContainers.isPresent())
  {
    _essenceContainers.clear();
    _essenceContainers.removeProperty();
  }


  //
  // For each ContainerFormat property in the file (which
  // is an optional property on a FileDescriptor), get its
  // ID and append to the _essenceContainers set
  //

  aafSearchCrit_t  searchCrit;
  searchCrit.searchTag = kAAFByMobKind;
  searchCrit.tags.mobKind = kAAFFileMob;

  ImplEnumAAFMobs*  pSourceMobs = 0;
  AAFRESULT  hr = GetMobs( &searchCrit, &pSourceMobs );

  if (AAFRESULT_SUCCEEDED(hr))
  {
    ImplAAFMob*  pMob = 0;
    while (AAFRESULT_SUCCEEDED (pSourceMobs->NextOne (&pMob)))
    {
      ImplAAFSourceMob*  pSourceMob = dynamic_cast<ImplAAFSourceMob*>(pMob);
      ASSERTU(pSourceMob);

      ImplAAFEssenceDescriptor*  pDescriptor = 0;
      pSourceMob->GetEssenceDescriptor( &pDescriptor );
      ImplAAFFileDescriptor*  pFileDescriptor =
        dynamic_cast<ImplAAFFileDescriptor*>(pDescriptor);

      if (pFileDescriptor)
      {
        ImplAAFContainerDef*  pContainerDef = 0;
        hr = pFileDescriptor->GetContainerFormat( &pContainerDef );
        if (AAFRESULT_SUCCEEDED (hr))
        {
          aafUID_t  containerDefID;
          pContainerDef->GetAUID( &containerDefID );

          if (! _essenceContainers.contains(containerDefID))
          {
            _essenceContainers.insert(containerDefID);
          }

          pContainerDef->ReleaseReference();
          pContainerDef = 0;
        }
      }

      pDescriptor->ReleaseReference();
      pDescriptor = 0;

      pMob->ReleaseReference();
      pMob = 0;
    }
  }

  if (pSourceMobs)
  {
    pSourceMobs->ReleaseReference();
    pSourceMobs = 0;
  }


  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::CountEssenceContainers (aafUInt32 * pCount)
{
  if (!pCount)
    return AAFRESULT_NULL_PARAM;

  if (!_essenceContainers.isPresent())
    return AAFRESULT_PROP_NOT_PRESENT;


  *pCount = _essenceContainers.count();


  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::GetEssenceContainers (aafUInt32 maxEssenceContainersCount,
                                         aafUID_t* pEssenceContainers)
{
  if (!pEssenceContainers)
    return AAFRESULT_NULL_PARAM;

  if (!_essenceContainers.isPresent())
    return AAFRESULT_PROP_NOT_PRESENT;

  if (_essenceContainers.count() > maxEssenceContainersCount)
    return AAFRESULT_SMALLBUF;


  aafUID_t*  pNextEssenceContainer = pEssenceContainers;
  OMSetPropertyIterator<aafUID_t> iter( _essenceContainers, OMBefore );
  while( ++iter )
  {
    *pNextEssenceContainer = iter.value();
    pNextEssenceContainer++;
  }


  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::IsEssenceContainerPresent (const aafUID_t& essenceContainer,
                                              aafBoolean_t* pIsPresent)
{
  if (!pIsPresent)
    return AAFRESULT_NULL_PARAM;

  if (!_essenceContainers.isPresent())
    return AAFRESULT_PROP_NOT_PRESENT;


  if (_essenceContainers.contains( essenceContainer))
  {
    *pIsPresent = kAAFTrue;
  }
  else
  {
    *pIsPresent = kAAFFalse;
  }


  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
ImplAAFHeader::CountDescriptiveSchemes
        (aafUInt32 * pCount)
{
  if (!pCount)
    return AAFRESULT_NULL_PARAM;

  if (!_descriptiveSchemes.isPresent())
    return AAFRESULT_PROP_NOT_PRESENT;


  *pCount = _descriptiveSchemes.count();


  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFHeader::GetDescriptiveSchemes
        (aafUInt32 maxDescriptiveSchemesCount,
         aafUID_t* pDescriptiveSchemes)
{
  if (!pDescriptiveSchemes)
    return AAFRESULT_NULL_PARAM;

  if (!_descriptiveSchemes.isPresent())
    return AAFRESULT_PROP_NOT_PRESENT;

  if (_descriptiveSchemes.count() > maxDescriptiveSchemesCount)
    return AAFRESULT_SMALLBUF;


  aafUID_t*  pNextDescriptiveScheme = pDescriptiveSchemes;
  OMSetPropertyIterator<aafUID_t> iter( _descriptiveSchemes, OMBefore );
  while( ++iter )
  {
    *pNextDescriptiveScheme = iter.value();
    pNextDescriptiveScheme++;
  }


  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFHeader::IsDescriptiveSchemePresent
        (const aafUID_t& descriptiveSchemeID,
         aafBoolean_t* pIsPresent)
{
  if (!pIsPresent)
    return AAFRESULT_NULL_PARAM;

  if (!_descriptiveSchemes.isPresent())
    return AAFRESULT_PROP_NOT_PRESENT;


  if( _descriptiveSchemes.contains( descriptiveSchemeID ) )
  {
    *pIsPresent = kAAFTrue;
  }
  else
  {
    *pIsPresent = kAAFFalse;
  }


  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFHeader::AddDescriptiveScheme
        (const aafUID_t& descriptiveSchemeID)
{
  if (_descriptiveSchemes.isPresent())
  {
    if (_descriptiveSchemes.contains(descriptiveSchemeID))
      return AAFRESULT_INVALID_PARAM;
  }


  _descriptiveSchemes.insert(descriptiveSchemeID);


  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFHeader::RemoveDescriptiveScheme
        (const aafUID_t& descriptiveSchemeID)
{
  if (!_descriptiveSchemes.isPresent())
    return AAFRESULT_PROP_NOT_PRESENT;

  if (! _descriptiveSchemes.contains(descriptiveSchemeID))
    return AAFRESULT_INVALID_PARAM;


  _descriptiveSchemes.remove(descriptiveSchemeID);

  if (_descriptiveSchemes.count() == 0)
  {
    _descriptiveSchemes.removeProperty();
  }


  return AAFRESULT_SUCCESS;
}



void ImplAAFHeader::SetContentStorage( ImplAAFContentStorage* pNewStorage )
{
  ImplAAFContentStorage* pStorage = _contentStorage.clearValue();
  if (pStorage) {
    pStorage->ReleaseReference();
    pStorage = 0;
  }

  pNewStorage->AcquireReference();
  _contentStorage = pNewStorage;
}

void ImplAAFHeader::ClearIdentificationList()
{
  _identificationList.removeAllObjects();
}

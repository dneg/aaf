
/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


#include "AAFTypes.h"
#include "AAFResult.h"
#include "aafTable.h"
#include "aafErr.h"

#ifndef __ImplAAFMob_h__
#include "ImplAAFMob.h"
#endif

#ifndef __ImplEnumAAFMobs_h__
#include "ImplEnumAAFMobs.h"
#endif

#ifndef __ImplEnumAAFMedia_h__
#include "ImplEnumAAFMedia.h"
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

#include <assert.h>

#if defined(__MWERKS__)
// Since the ansi standard does not define wcslen and the other wide
// string functions are not normally placed in string.h along with the
// single-byte string functions, as is done with VC++, CodeWarrior
// places all of the "wide-string" functions in wstring.h.
#include <wstring.h>
#endif

// BobT 11-Sept-1998: Changed '#if 0' to '#if FULL_TOOLKIT'.
#if FULL_TOOLKIT
#include <stdio.h>
#include <string.h>
#include "aafCvt.h"
#include "aafTable.h"
#include "AAFPrivate.h"
#include "AAFUtils.h"
#endif

#include "ImplAAFObjectCreation.h"

#define DEFAULT_NUM_MOBS				1000
#define DEFAULT_NUM_DATAOBJ			200
#define DEFAULT_NUM_DATAKIND_DEFS	100
#define DEFAULT_NUM_EFFECT_DEFS		100


// This is temporary and will be replaced when sequence
// factory method is available.
extern "C" const aafClassID_t CLSID_AAFIdentification;
extern "C" const aafClassID_t	CLSID_AAFContentStorage;

ImplAAFHeader::ImplAAFHeader ()
: _byteOrder(         PID_HEADER_BYTEORDER,          "byteOrder"),
  _lastModified(      PID_HEADER_LASTMODIFIED,       "lastModified"),
  _identificationList(PID_HEADER_IDENTIFICATIONLIST, "identificationList"),
  _contentStorage(		PID_HEADER_CONTENTSTORAGE,	"contentStorage")
{
  _persistentProperties.put(_byteOrder.address());
  _persistentProperties.put(_lastModified.address());
  _persistentProperties.put(_identificationList.address());
  _persistentProperties.put(_contentStorage.address());

  //!!!	_head = this;
//	file->InternalSetHead(this);
#if FULL_TOOLKIT
	_mobs = (omTable_t *)NULL;
	_dataObjs = (omTable_t *)NULL;
#endif
	_fileRev.major = 0;
	_fileRev.minor = 0;
	_toolkitRev.major = 0;
	_toolkitRev.minor = 0;
	_toolkitRev.tertiary = 0;
	_toolkitRev.type = kVersionUnknown;
	_toolkitRev.patchLevel = 0;
//!!!	_byteOrder;
//!!!	_lastModified;
// trr - Moved conditional creation of content storage to GetContentStorage method
// so that we do not leak an object when the file is restored. We may have to do
// something similar for the dictionary.
//	_contentStorage = (ImplAAFContentStorage *)CreateImpl(CLSID_AAFContentStorage);
}


ImplAAFHeader::~ImplAAFHeader ()
{
	// Release all of the id pointers in the id list.
	//
	ImplAAFIdentification *pIdent = NULL;
  size_t size;
  _identificationList.getSize(size);
	for (size_t i = 0; i < size; i++) {
		_identificationList.getValueAt(pIdent, i);

		if (pIdent) {
			pIdent->ReleaseReference();
			// Set value to 0 so the OM can perform any necessary cleanup.
			pIdent = 0;
			_identificationList.setValueAt(pIdent, i);
		}
	}

	// Release the content storage pointer. Set the 
	if (_contentStorage) {
		_contentStorage->ReleaseReference();
		_contentStorage = 0;
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::LookupMob (aafUID_t *pMobID,
                           ImplAAFMob **ppMob)
{
    if ((! pMobID) || (! ppMob))
	  {
		return AAFRESULT_NULL_PARAM;
	  }
	ImplAAFContentStorage *cstore = GetContentStorage();
	return(cstore->LookupMob(pMobID, ppMob));
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::GetNumMobs (aafMobKind_t mobKind,
                           aafNumSlots_t *pNumMobs)
{
    if (! pNumMobs)
	  {
		return AAFRESULT_NULL_PARAM;
	  }
	ImplAAFContentStorage *cstore = GetContentStorage();
	return(cstore->GetNumMobs(mobKind, pNumMobs));
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::EnumAAFPrimaryMobs (ImplEnumAAFMobs **ppEnum)
{
    if (! ppEnum)
	  {
		return AAFRESULT_NULL_PARAM;
	  }
	ImplAAFContentStorage *cstore = GetContentStorage();
	return(cstore->GetPrimaryMobs(ppEnum));
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::EnumAAFAllMobs (aafSearchCrit_t *pSearchCriteria,
                           ImplEnumAAFMobs **ppEnum)
{
    if (! ppEnum)
	  {
		return AAFRESULT_NULL_PARAM;
	  }
	ImplAAFContentStorage *cstore = GetContentStorage();
	return(cstore->GetMobs(pSearchCriteria, ppEnum));
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::AppendMob (ImplAAFMob *pMob)
{
    if (! pMob)
	  {
		return AAFRESULT_NULL_PARAM;
	  }
	ImplAAFContentStorage *cstore = GetContentStorage();
	return(cstore->AppendMob(pMob));
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::RemoveMob (ImplAAFMob *pMob)
{
    if (! pMob)
	  {
		return AAFRESULT_NULL_PARAM;
	  }
	ImplAAFContentStorage *cstore = GetContentStorage();
	return(cstore->RemoveMob(pMob));
}



// Implementer note:
// based on omfmIsMediaDataPresent
//
AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::IsMediaDataPresent (aafUID_t *pFileMobID,
                           aafFileFormat_t fmt,
                           aafBool *pResult)
{
    if ((! pFileMobID) || (! pResult))
	  {
		return AAFRESULT_NULL_PARAM;
	  }
	ImplAAFContentStorage *cstore = GetContentStorage();
	return(cstore->IsMediaDataPresent(pFileMobID, fmt, pResult));
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::EnumAAFMediaObjects (aafMediaCriteria_t *pMediaCriteria,
                           ImplEnumAAFMedia **ppEnum)
{
    if ((! pMediaCriteria) || (! ppEnum))
	  {
		return AAFRESULT_NULL_PARAM;
	  }
	ImplAAFContentStorage *cstore = GetContentStorage();
	return(cstore->GetMedia(pMediaCriteria, ppEnum));
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::AppendMedia (ImplAAFMedia * pMedia)
{
  if (! pMedia)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::RemoveMedia (ImplAAFMedia * pMedia)
{
  if (! pMedia)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  return AAFRESULT_NOT_IMPLEMENTED;
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
    ImplAAFHeader::GetDictionary (ImplAAFDictionary ** ppDictionary)
{
  if (! ppDictionary)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::GetLastIdentification (ImplAAFIdentification ** ppIdentification)
{
  if (! ppIdentification)
	{
	  return AAFRESULT_NULL_PARAM;
	}

  AAFRESULT result;
  size_t size;
  _identificationList.getSize(size);

  if (size > 0) {
    // For size entries the valid positions are 0 .. size - 1
    // get the last one in the vector.
    _identificationList.getValueAt(*ppIdentification, size - 1);

		// We are returning a non-null reference.
		if (*ppIdentification) {
			(*ppIdentification)->AcquireReference();
			result = AAFRESULT_SUCCESS;
		} else {
			result = AAFRESULT_NO_MORE_OBJECTS; // trr - is there a better error code?
		}
  } else {
    *ppIdentification = 0;
    result = AAFRESULT_NOT_IMPLEMENTED; // tjb - Should be AAFRESULT_FAILURE
  }
  return result;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::GetIdentificationByGen (aafUID_t * pGeneration,
                           ImplAAFIdentification ** ppIdentification)
{
  if ((! pGeneration) || (! ppIdentification))
	{
	  return AAFRESULT_NULL_PARAM;
	}
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
ImplAAFHeader::GetNumIdents
(aafUInt32 *  pNumIdents)
{
    if (! pNumIdents)
	  {
		return AAFRESULT_NULL_PARAM;
	  }
	return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::EnumAAFIdents (ImplEnumAAFIdentifications ** ppEnum)
{
  if (! ppEnum)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  return AAFRESULT_NOT_IMPLEMENTED;
}

AAFRESULT 
    ImplAAFHeader::GetNumIdentifications (aafInt32 *pCount)
{
  if (! pCount)
	{
	  return AAFRESULT_NULL_PARAM;
	}
	size_t size;
  _identificationList.getSize(size);
	*pCount = size;
  return AAFRESULT_SUCCESS;
}

AAFRESULT 
    ImplAAFHeader::AddIdentificationObject (aafProductIdentification_t *pIdent)
{
	ImplAAFIdentification *		identObj;
	aafProductIdentification_t	fiction;
	aafBool						dummyIDNT = AAFFalse;
	aafProductVersion_t			dummyVersion;
	
//!!!	aafAssertValidFHdl(_file);
	XPROTECT()
	{		
		if(pIdent == (aafProductIdentification_t *)NULL)
		{
			fiction.companyName = L"Unknown";
			fiction.productName = L"Unknown";
			fiction.productVersionString = (aafWChar*)NULL;
			fiction.productID = -1;
			fiction.platform = (aafWChar*)NULL;
			fiction.productVersion.major = 0;
			fiction.productVersion.minor = 0;
			fiction.productVersion.tertiary = 0;
			fiction.productVersion.patchLevel = 0;
			fiction.productVersion.type = kVersionUnknown;
			pIdent = &fiction;
			dummyIDNT = AAFTrue;
		}
		
	XASSERT(pIdent != NULL, OM_ERR_NEED_PRODUCT_IDENT);
    if (pIdent->productVersionString == 0) {
      pIdent->productVersionString = L"Unknown version";
    }
    if (pIdent->platform == 0) {
      pIdent->platform = L"Windows NT";
    }

    identObj = static_cast<ImplAAFIdentification *>(CreateImpl(CLSID_AAFIdentification));
    if (NULL == identObj)
      CHECK(AAFRESULT_NOMEMORY);
    CHECK(identObj->SetCompanyName(pIdent->companyName));
    CHECK(identObj->SetProductName(pIdent->productName));
    CHECK(identObj->SetProductVersionString(pIdent->productVersionString));

    _identificationList.appendValue(identObj);
 
    dummyVersion.major = 0;
	}
	XEXCEPT
	{
	}
	XEND
	
	return(OM_ERR_NONE);
}


AAFRESULT 
    ImplAAFHeader::AppendIdentification (ImplAAFIdentification * pIdent)
{
  if (! pIdent)
	{
	  return AAFRESULT_NULL_PARAM;
	}

	_identificationList.appendValue(pIdent);
	pIdent->AcquireReference();


  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT 
    ImplAAFHeader::RemoveIdentification (ImplAAFIdentification * pIdent)
{
  if (! pIdent)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  return AAFRESULT_NOT_IMPLEMENTED;
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
#if FULL_TOOLKIT
	XPROTECT(_file)
	{
		if (IsPropertyPresent(OMVersion, OMVersionType))
		  {
			if ((_fileRev.major == 1) && (_fileRev.minor == 0))
			  *pRevision = kAAFRev1;
			else
				RAISE(OM_ERR_FILEREV_NOT_SUPP);
		  }
		else
			RAISE(OM_ERR_FILEREV_NOT_SUPP);
	}
	XEXCEPT
	{
	}
	XEND;

	return (OM_ERR_NONE);
#else
  return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

	
AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::GetLastModified (aafTimeStamp_t * pTimeStamp)
{
  if (! pTimeStamp)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  return AAFRESULT_NOT_IMPLEMENTED;
}

AAFRESULT ImplAAFHeader::SetToolkitRevisionCurrent()
{
	_toolkitRev = AAFReferenceImplementationVersion;
	return (OM_ERR_NONE);
}

AAFRESULT ImplAAFHeader::LoadMobTables(void)
{
	ImplAAFContentStorage *cstore = GetContentStorage();
	return(cstore->LoadMobTables());
}

/************************
 * Function: BuildMediaCache (INTERNAL)
 *
 * 		This function is a callback from the openFile and createFile
 *		group of functions.  This callback exists in order to allow the
 *		media layer to be independant, and yet have information of its
 *		own in the opaque file handle.
 *
 * Argument Notes:
 *		<none>.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
AAFRESULT ImplAAFHeader::BuildMediaCache(void)
{
#if FULL_TOOLKIT
	aafInt32						siz, n, dataObjTableSize;
	AAFObject *				obj;
	aafUID_t					uid;
	
	XPROTECT(_file)
	{
		{
		   siz = GetObjRefArrayLength(OMHEADMediaData);
			dataObjTableSize = (siz < DEFAULT_NUM_DATAOBJ ? 
									  DEFAULT_NUM_DATAOBJ : siz);
			CHECK(NewUIDTable(_file, dataObjTableSize, &(_dataObjs)));
			for(n = 1; n <= siz; n++)
			  {
				 CHECK(ReadNthObjRefArray(OMHEADMediaData, 
													  &obj, n));
				 CHECK(obj->ReadUID(OMMDATMobID, &uid));
				 CHECK(TableAddUID(_dataObjs, uid,obj,kOmTableDupAddDup));
			  }
		 }
	}
	XEXCEPT
	XEND
#endif
	
	return(OM_ERR_NONE);
}

AAFRESULT ImplAAFHeader::AppendDataObject(aafUID_t mobID,      /* IN - Mob ID */
						  ImplAAFObject *dataObj)    /* IN - Input Mob */ 
{
#if FULL_TOOLKIT
	XPROTECT(_file)
	  {
		CHECK(AppendObjRefArray(OMHEADMediaData, dataObj));
		CHECK(TableAddUID(_dataObjs, mobID, dataObj, kOmTableDupError));
	  } /* XPROTECT */
	XEXCEPT
	  {
		return(XCODE());
	  }
	XEND;
#endif
	
	return(OM_ERR_NONE);
}

AAFRESULT ImplAAFHeader::IsValidHeadObject(void)
{
#if FULL_TOOLKIT
	aafClassID_t  		omfiID;

	if (GetClassID(omfiID) != OM_ERR_NONE)
		  return(OM_ERR_NOTAAFFILE);
	if (!streq(omfiID, "HEAD"))
	  return(OM_ERR_NOTAAFFILE);
	return(OM_ERR_NONE);
#else
  return AAFRESULT_NOT_IMPLEMENTED;
#endif
}


// trr - NOTE: Eventhough this method returns a reference counted object it
// does NOT bump the reference count. Currently only other file that calls
// this method is ImplAAFMob.cpp. We should probably make this method protected
// or private and create an new version the conforms to our other API guidlines:
// AAFRESULT GetContentStorage(ImplAAFContentStorage **ppContentStorage);
// 
ImplAAFContentStorage *ImplAAFHeader::GetContentStorage()
{
	ImplAAFContentStorage	*result = _contentStorage;

	// Create the content storage object if it does not exist.
	if (NULL == result) {
		result = (ImplAAFContentStorage *)CreateImpl(CLSID_AAFContentStorage);
		_contentStorage = result;
	}

	return(result);
}

// Fill in when dictionary property is supported.
ImplAAFDictionary *ImplAAFHeader::GetDictionary()
{
#if 0
	ImplAAFDictionary	*result = _dictionary;

	// Create the dictionary object if it does not exist.
	if (NULL == result) {
		result = (ImplAAFDictionary *)CreateImpl(CLSID_AAFDictionary);
		_dictionary = result;
	}

	return(result);
#else
	return NULL;
#endif
}

extern "C" const aafClassID_t CLSID_AAFHeader;

OMDEFINE_STORABLE(ImplAAFHeader, CLSID_AAFHeader);

// Cheat!  We're using this object's CLSID instead of object class...
AAFRESULT STDMETHODCALLTYPE
ImplAAFHeader::GetObjectClass(aafUID_t * pClass)
{
  if (! pClass)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  memcpy (pClass, &CLSID_AAFHeader, sizeof (aafClassID_t));
  return AAFRESULT_SUCCESS;
}


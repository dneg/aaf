
/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 *  prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/

#include "AAFTypes.h"
#include "AAFResult.h"
#include "aafTable.h"
#include "aafErr.h"
#include "AAFUtils.h"

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

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include <assert.h>

#if defined(__MWERKS__)
// Since the ansi standard does not define wcslen and the other wide
// string functions are not normally placed in string.h along with the
// single-byte string functions, as is done with VC++, CodeWarrior
// places all of the "wide-string" functions in wstring.h.
#include <wstring.h>
#endif

#include "ImplAAFObjectCreation.h"

#define DEFAULT_NUM_MOBS				1000
#define DEFAULT_NUM_DATAOBJ			200
#define DEFAULT_NUM_DATAKIND_DEFS	100
#define DEFAULT_NUM_EFFECT_DEFS		100

extern "C" const aafClassID_t CLSID_EnumAAFIdentifications;

const aafUID_t NIL_UID = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };


ImplAAFHeader::ImplAAFHeader ()
: _byteOrder(         PID_Header_ByteOrder,          "ByteOrder"),
  _lastModified(      PID_Header_LastModified,       "LastModified"),
  _identificationList(PID_Header_IdentificationList, "IdentificationList"),
  _contentStorage(		PID_Header_Content,	"Content"),
  _dictionary(PID_Header_Dictionary,	"Dictionary"),
  _fileRev(PID_Header_Version,		"Version")
{
  _persistentProperties.put(_byteOrder.address());
  _persistentProperties.put(_lastModified.address());
  _persistentProperties.put(_identificationList.address());
  _persistentProperties.put(_contentStorage.address());
  _persistentProperties.put(_dictionary.address());
  _persistentProperties.put(_fileRev.address());

  //!!!	_head = this;
//	file->InternalSetHead(this);
	_toolkitRev.major = 0;
	_toolkitRev.minor = 0;
	_toolkitRev.tertiary = 0;
	_toolkitRev.type = kVersionUnknown;
	_toolkitRev.patchLevel = 0;
//!!!	_byteOrder;
//!!!	_lastModified;
}


ImplAAFHeader::~ImplAAFHeader ()
{
	// Release all of the id pointers in the id list.
	//
	size_t size = _identificationList.getSize();
	for (size_t i = 0; i < size; i++) {
		ImplAAFIdentification *pIdent = _identificationList.setValueAt(0, i);

		if (pIdent) {
		  pIdent->ReleaseReference();
		  pIdent = 0;
		}
	}

	// Release the content storage pointer.
	ImplAAFContentStorage *contentStorage = _contentStorage.setValue(0);
	if (contentStorage) {
	  contentStorage->ReleaseReference();
	  contentStorage = 0;
	}

	// Release the dictionary pointer.
	ImplAAFDictionary *dictionary = _dictionary.setValue(0);
	if (dictionary) {
	  dictionary->ReleaseReference();
	  dictionary = 0;
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::LookupMob (aafUID_t *pMobID,
                           ImplAAFMob **ppMob)
{
    ImplAAFContentStorage *cstore = NULL;

    if ((! pMobID) || (! ppMob))
	  {
		return AAFRESULT_NULL_PARAM;
	  }
	XPROTECT()
	{
		cstore = GetContentStorage();		// Does not AddRef
		CHECK(cstore->LookupMob(pMobID, ppMob));
	}
	XEXCEPT
	XEND

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::GetNumMobs (aafMobKind_t mobKind,
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
		CHECK(cstore->GetNumMobs(mobKind, pNumMobs));
	}
	XEXCEPT
	XEND

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::EnumAAFAllMobs (aafSearchCrit_t *pSearchCriteria,
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
    ImplAAFHeader::AppendMob (ImplAAFMob *pMob)
{
    ImplAAFContentStorage *cstore = NULL;

    if (! pMob)
	  {
		return AAFRESULT_NULL_PARAM;
	  }
	XPROTECT()
	{
		cstore = GetContentStorage();		// Does not AddRef
		CHECK(cstore->AppendMob(pMob));
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
	  {
		return AAFRESULT_NULL_PARAM;
	  }
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
    ImplAAFHeader::GetNumEssenceData(aafUInt32 *  pNumEssenceData)
{
    ImplAAFContentStorage *cstore = NULL;

	if(pNumEssenceData == NULL)
		 return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		cstore = GetContentStorage();		// Does not AddRef
		CHECK(cstore->GetNumEssenceData(pNumEssenceData));
	}
	XEXCEPT
	XEND

	return AAFRESULT_SUCCESS;
}


// Implementer note:
// based on omfmIsMediaDataPresent
//
AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::IsEssenceDataPresent (aafUID_t *pFileMobID,
                           aafFileFormat_t fmt,
                           aafBool *pResult)
{
    ImplAAFContentStorage *cstore = NULL;
    if ((! pFileMobID) || (! pResult))
	  {
		return AAFRESULT_NULL_PARAM;
	  }
	XPROTECT()
	{
		cstore = GetContentStorage();		// Does not AddRef
		CHECK(cstore->IsEssenceDataPresent(pFileMobID, fmt, pResult));
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
    ImplAAFHeader::AppendEssenceData (ImplAAFEssenceData * pEssenceData)
{
    ImplAAFContentStorage *cstore = NULL;
	if (! pEssenceData)
	{
		 return AAFRESULT_NULL_PARAM;
	}
	XPROTECT()
	{
		cstore = GetContentStorage();		// Does not AddRef
		CHECK(cstore->AppendEssenceData(pEssenceData));
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
	{
		return AAFRESULT_NULL_PARAM;
	}
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
    result = AAFRESULT_INCONSISTANCY;
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
 	size_t	siz;
	if (! pNumIdents)
	{
		return AAFRESULT_NULL_PARAM;
	}
		
	_identificationList.getSize(siz);
	*pNumIdents = siz;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::EnumAAFIdents (ImplEnumAAFIdentifications ** ppEnum)
{
	if (NULL == ppEnum)
		return AAFRESULT_NULL_PARAM;
	*ppEnum = 0;
	
	ImplEnumAAFIdentifications *theEnum = (ImplEnumAAFIdentifications *)CreateImpl (CLSID_EnumAAFIdentifications);
	
	XPROTECT()
	{
		CHECK(theEnum->SetEnumStrongProperty(this, &_identificationList));
		*ppEnum = theEnum;
	}
	XEXCEPT
	{
		if (theEnum)
		  theEnum->ReleaseReference();
		theEnum = 0;
		return(XCODE());
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
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
	
	XPROTECT()
	{		
		if(pIdent == (aafProductIdentification_t *)NULL)
		{
			fiction.companyName = L"Unknown";
			fiction.productName = L"Unknown";
			fiction.productVersionString = (aafWChar*)NULL;
			fiction.productID = NIL_UID;
			fiction.platform = (aafWChar*)NULL;
			fiction.productVersion.major = 0;
			fiction.productVersion.minor = 0;
			fiction.productVersion.tertiary = 0;
			fiction.productVersion.patchLevel = 0;
			fiction.productVersion.type = kVersionUnknown;
			pIdent = &fiction;
			dummyIDNT = AAFTrue;
		}
		
	XASSERT(pIdent != NULL, AAFRESULT_NEED_PRODUCT_IDENT);
    if (pIdent->productVersionString == 0) {
      pIdent->productVersionString = L"Unknown version";
    }
    if (pIdent->platform == 0) {
      pIdent->platform = L"Windows NT";
    }
    
    // Get the dictionary so that we can use the factory
    // method to create the identification.
    ImplAAFDictionary *pDictionary = GetDictionary();
    if (NULL == pDictionary)
      CHECK(AAFRESULT_NOMEMORY);
    CHECK(pDictionary->CreateInstance(&AUID_AAFIdentification, (ImplAAFObject **)&identObj));
    if (NULL == identObj)
      CHECK(AAFRESULT_NOMEMORY);
    CHECK(identObj->SetCompanyName(pIdent->companyName));
    CHECK(identObj->SetProductName(pIdent->productName));
    CHECK(identObj->SetProductVersionString(pIdent->productVersionString));
	CHECK(identObj->SetProductID(&pIdent->productID));

    _identificationList.appendValue(identObj);
 
    dummyVersion.major = 0;
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

	_identificationList.appendValue(pIdent);
	pIdent->AcquireReference();

	return AAFRESULT_SUCCESS;
}


AAFRESULT 
    ImplAAFHeader::RemoveIdentification (ImplAAFIdentification * pIdent)
{
  if (! pIdent)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  return AAFRESULT_NOT_IN_CURRENT_VERSION;
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
	aafTimeStamp_t	now = { 0 };

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
  _dictionary = pDictionary;
  if (pDictionary)
    pDictionary->AcquireReference();

  // If this hasn't been done before, make sure the dictionary has had
  // its properties initialized.
  _dictionary->pvtInitCriticalBuiltins ();
}

AAFRESULT ImplAAFHeader::SetToolkitRevisionCurrent()
{
	_toolkitRev = AAFReferenceImplementationVersion;
	return (AAFRESULT_SUCCESS);
}

AAFRESULT ImplAAFHeader::LoadMobTables(void)
{
	ImplAAFContentStorage *cstore = GetContentStorage();
	return(cstore->LoadMobTables());
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
  if (NULL == result)
  { // Get the dictionary so that we can use the factory
    // method to create the identification.
    ImplAAFDictionary *pDictionary = GetDictionary();
    if (NULL != pDictionary)
	  {
		pDictionary->CreateInstance (&AUID_AAFContentStorage,
									 (ImplAAFObject **)&result);
		_contentStorage = result;
	  }
  }

  return(result);
}

// Fill in when dictionary property is supported.
ImplAAFDictionary *ImplAAFHeader::GetDictionary()
{
  ImplAAFDictionary	*result = _dictionary;
  assert(result);

  // If this hasn't been done before, make sure the dictionary has had
  // its properties initialized.
  _dictionary->pvtInitCriticalBuiltins ();

  // Make sure that _dictionary member points to the same instance
  // as the value returned by ImplAAFObject::GetDictionary()!
  ImplAAFDictionary	*pDictionary = NULL;
  assert(AAFRESULT_SUCCESS == ImplAAFObject::GetDictionary(&pDictionary));
  assert(pDictionary);
  assert(pDictionary == result);
  aafUInt32 refCount = pDictionary->ReleaseReference();
  pDictionary = 0;
  assert(0 != refCount);

  return(result);
}

OMDEFINE_STORABLE(ImplAAFHeader, AUID_AAFHeader);

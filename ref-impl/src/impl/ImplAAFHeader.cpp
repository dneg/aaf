
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

extern "C" const aafClassID_t	CLSID_AAFContentStorage;

ImplAAFHeader::ImplAAFHeader ()
: _byteOrder(         PID_HEADER_BYTEORDER,          "byteOrder"),
  _lastModified(      PID_HEADER_LASTMODIFIED,       "lastModified"),
  _identificationList(PID_HEADER_IDENTIFICATIONLIST, "identificationList"),
  _contentStorage(		PID_HEADER_CONTENTSTORAGE,	"contentStorage")
{
  ImplAAFContentStorage		*cstore;
  
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
	cstore = (ImplAAFContentStorage *)CreateImpl(CLSID_AAFContentStorage);
	_contentStorage = cstore;
}


ImplAAFHeader::~ImplAAFHeader ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::LookupMob (aafUID_t *pMobID,
                           ImplAAFMob **ppMob)
{
    if ((! pMobID) || (! ppMob))
	  {
		return AAFRESULT_NULL_PARAM;
	  }
	ImplAAFContentStorage *cstore = _contentStorage;
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
	ImplAAFContentStorage *cstore = _contentStorage;
	return(cstore->GetNumMobs(mobKind, pNumMobs));
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::EnumAAFPrimaryMobs (ImplEnumAAFMobs **ppEnum)
{
    if (! ppEnum)
	  {
		return AAFRESULT_NULL_PARAM;
	  }
	ImplAAFContentStorage *cstore = _contentStorage;
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
	ImplAAFContentStorage *cstore = _contentStorage;
	return(cstore->GetMobs(pSearchCriteria, ppEnum));
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::AppendMob (ImplAAFMob *pMob)
{
    if (! pMob)
	  {
		return AAFRESULT_NULL_PARAM;
	  }
	ImplAAFContentStorage *cstore = _contentStorage;
	return(cstore->AppendMob(pMob));
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::RemoveMob (ImplAAFMob *pMob)
{
    if (! pMob)
	  {
		return AAFRESULT_NULL_PARAM;
	  }
	ImplAAFContentStorage *cstore = _contentStorage;
	return(cstore->RemoveMob(pMob));
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::IsMediaDataPresent (aafUID_t *pFileMobID,
                           aafFileFormat_t fmt,
                           aafBool *pResult)
{
    if ((! pFileMobID) || (! pResult))
	  {
		return AAFRESULT_NULL_PARAM;
	  }
	ImplAAFContentStorage *cstore = _contentStorage;
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
	ImplAAFContentStorage *cstore = _contentStorage;
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
    result = AAFRESULT_SUCCESS;
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
    ImplAAFHeader::EnumAAFIdents (ImplEnumAAFIdentifications ** ppEnum)
{
  if (! ppEnum)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  return AAFRESULT_NOT_IMPLEMENTED;
}

AAFRESULT 
    ImplAAFHeader::GetNumIdentifications (aafInt32 * /*pCount*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
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
    identObj = new ImplAAFIdentification(
      pIdent->companyName,
      pIdent->productName,
      &pIdent->productVersion,
      pIdent->productVersionString,
      // productID,
      _lastModified,
      &AAFToolkitVersion,
      pIdent->platform
      // generation
      );

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
	_toolkitRev = AAFToolkitVersion;
	return (OM_ERR_NONE);
}

AAFRESULT ImplAAFHeader::LoadMobTables(void)
{
#if FULL_TOOLKIT
	aafInt32			mobTableSize, siz, n;
	AAFObject			*obj;
	AAFMob				*mob;
	aafUID_t			uid;
	
	XPROTECT(_file)
	{
		/*
		 * Make a table of all of the source & composition mobs
		 */
			mobTableSize = GetObjRefArrayLength(OMHEADMobs);

		mobTableSize *= 2; /* Allow for some growth */
		if(mobTableSize < DEFAULT_NUM_MOBS)
			mobTableSize = DEFAULT_NUM_MOBS;
		CHECK(NewUIDTable(_file, mobTableSize, &(_mobs)));
//!!!		CHECK(omfsSetTableDispose(_mobs, &MobDisposeMap1X));

		{
			siz = GetObjRefArrayLength(OMHEADMobs);
			for(n = 1; n <= siz; n++)
			{
				CHECK(ReadNthObjRefArray(OMHEADMobs, &obj, n));
				CHECK(obj->ReadUID(OMMOBJMobID, &uid));
				mob = (AAFMob *)obj;	// !!CASTING
				CHECK(AddMobTableEntry(mob, uid, kOmTableDupAddDup));
			}
		}
	}
	XEXCEPT
	{
	}
	XEND;
#endif

	return (OM_ERR_NONE);
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

ImplAAFContentStorage *ImplAAFHeader::GetContentStorage()
{
	ImplAAFContentStorage	*result = _contentStorage;
	return(result);
}

extern "C" const aafClassID_t CLSID_AAFHeader;

OMDEFINE_STORABLE(ImplAAFHeader, CLSID_AAFHeader);


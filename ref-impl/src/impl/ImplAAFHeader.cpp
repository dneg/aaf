
/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


#include "AAFTypes.h"
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

#include "AAFMob.h"




#ifndef __ImplAAFHeader_h__
#include "ImplAAFHeader.h"
#endif

#ifndef __AAFHeader_h__
#include "AAFHeader.h"
#endif

#include "ImplAAFFile.h"

#include <assert.h>
#if 0
#include <stdio.h>
#include <string.h>
#include "AAFHeader.h"
#include "AAFFile.h"
#include "AAFMob.h"
#include "AAFDataKind.h"
#include "aafCvt.h"
#include "aafTable.h"
#include "AAFPrivate.h"
#include "AAFUtils.h"
#endif

#define DEFAULT_NUM_MOBS				1000
#define DEFAULT_NUM_DATAOBJ			200
#define DEFAULT_NUM_DATAKIND_DEFS	100
#define DEFAULT_NUM_EFFECT_DEFS		100


ImplAAFHeader::ImplAAFHeader ()
{
//!!!	_head = this;
//	file->InternalSetHead(this);
	_mobs = (omTable_t *)NULL;
	_dataObjs = (omTable_t *)NULL;
	_fileRev.major = 0;
	_fileRev.minor = 0;
	_toolkitRev.major = 0;
	_toolkitRev.minor = 0;
	_toolkitRev.tertiary = 0;
	_toolkitRev.type = kVersionUnknown;
	_toolkitRev.patchLevel = 0;
//!!!	_byteOrder;
//!!!	_lastModified;
}


ImplAAFHeader::~ImplAAFHeader ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::LookupMob (aafUID_t *  /*mobID*/,
                           ImplAAFMob ** /*ppMob*/)
{
#if FULL_TOOLKIT
	mobTableEntry_t	*entry;
	AAFMob			*tmpMob;
	
	XPROTECT(_file)
	  {
		/* Get the mob out of the mob hash table */
		entry = (mobTableEntry_t *)TableUIDLookupPtr(_mobs, mobID);
		if (entry != NULL)
		  tmpMob = entry->mob; 

		if (tmpMob)
		  *mob = tmpMob;
		else
		  {
			RAISE(OM_ERR_MOB_NOT_FOUND);
		  }
	  } /* XPROTECT */

	XEXCEPT
	  {
		return(XCODE());
	  }
	XEND;
#endif
	
	return(OM_ERR_NONE);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::GetNumMobs (aafMobKind_t  /*mobKind*/,
                           aafNumSlots_t *  /*pNumMobs*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::GetPrimaryMobs (ImplEnumAAFMobs ** /*ppEnum*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::GetMobs (aafSearchCrit_t *  /*pSearchCriteria*/,
                           ImplEnumAAFMobs ** /*ppEnum*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::AppendMob (ImplAAFMob * /*pMob*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::RemoveMob (ImplAAFMob * /*pMob*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::IsMediaDataPresent (aafUID_t *  /*pFileMobID*/,
                           aafFileFormat_t  /*fmt*/,
                           aafBool *  /*result*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::GetMedia (aafMediaCriteria_t *  /*pMediaCriteria*/,
                           ImplEnumAAFMedia ** /*ppEnum*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::GetDictionary (ImplAAFDictionary ** /*ppDictionary*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::GetLastIdentification (ImplAAFIdentification ** /*ppIdentification*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::GetIdentification (aafUID_t *  /*pGeneration*/,
                           ImplAAFIdentification ** /*ppIdentification*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::GetIdentificationList (ImplEnumAAFIdentifications ** /*ppEnum*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::GetNumIdentifications (aafInt32 * /*pCount*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::AddIdentificationObject (aafProductIdentification_t *pIdent)
{
	AAFObject *					identObj;
	aafProductIdentification_t	fiction;
	aafBool						dummyIDNT = AAFFalse;
	aafProductVersion_t			dummyVersion;
	
//!!!	aafAssertValidFHdl(_file);
	XPROTECT(_file)
	{		
		if(pIdent == (aafProductIdentification_t *)NULL)
		{
			fiction.companyName = "Unknown";
			fiction.productName = "Unknown";
			fiction.productVersionString = (char *)NULL;
			fiction.productID = -1;
			fiction.platform = (char *)NULL;
			fiction.productVersion.major = 0;
			fiction.productVersion.minor = 0;
			fiction.productVersion.tertiary = 0;
			fiction.productVersion.patchLevel = 0;
			fiction.productVersion.type = kVersionUnknown;
			pIdent = &fiction;
			dummyIDNT = AAFTrue;
		}
		
#if FULL_TOOLKIT
		XASSERT(pIdent != NULL, OM_ERR_NEED_PRODUCT_IDENT);
		identObj = new AAFObject(_file, OMClassIDNT);
		if(pIdent->companyName != NULL)
		{
			CHECK(identObj->WriteString(OMIDNTCompanyName, identPtr->companyName));
		}
		if(pIdent->productName != NULL)
		{
			CHECK(identObj->WriteString(OMIDNTProductName, identPtr->productName));
		}
		CHECK(identObj->WriteProductVersionType(OMIDNTProductVersion,
										pIdent->productVersion));
		if(identPtr->productVersionString != NULL)
		{
			CHECK(identObj->WriteString(OMIDNTProductVersionString,
										identPtr->productVersionString));
		}
		CHECK(identObj->WriteInt32(OMIDNTProductID, identPtr->productID));
		if(dummyIDNT)
		{
			CHECK(identObj->WriteTimeStamp(OMIDNTDate, _lastModified));
			dummyVersion.major = 0;
			dummyVersion.minor = 0;
			dummyVersion.tertiary = 0;
			dummyVersion.patchLevel = 0;
			dummyVersion.type = kVersionUnknown;
			CHECK(identObj->WriteProductVersionType(OMIDNTToolkitVersion,
											dummyVersion));
		}
		else
		{
			CHECK(identObj->WriteTimeStamp(OMIDNTDate, _lastModified));
			CHECK(identObj->WriteProductVersionType(OMIDNTToolkitVersion, AAFToolkitVersion));
		}
		
		CHECK(identObj->WriteInt16(OMIDNTByteOrder, _byteOrder));
		CHECK(identObj->WriteString(OMIDNTPlatform, 
				(identPtr->platform != NULL ? identPtr->platform : "Unspecified")));
				
		CHECK(AppendObjRefArray(OMHEADIdentList, identObj));
#endif
	}
	XEXCEPT
	{
	}
	XEND
	
	return(OM_ERR_NONE);
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::GetToolkitVersion (aafProductVersion_t *pToolkitVersion)
{
	*pToolkitVersion = _toolkitRev;
	return (OM_ERR_NONE);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::GetFileRevision (aafVersionType_t *pRevision)
{
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
    ImplAAFHeader::GetByteOrder (aafInt16 *pByteOrder)
{
	*pByteOrder = _byteOrder;
	return(OM_ERR_NONE);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFHeader::GetLastModified (aafTimeStamp_t *  /*pLastModified*/)
{
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

aafBool ImplAAFHeader::IsMediaDataPresent(
									aafUID_t				fileMobUid,	/* IN -- */
									aafFileFormat_t	fmt)
{
#if FULL_TOOLKIT
	ImplAAFObject *	obj;
	aafBool		result;
	
	aafAssertValidFHdl(_file);
	result = AAFFalse;
	
	if (fmt == kAAFiMedia)
	  {
		 obj = (ImplAAFObject *)TableUIDLookupPtr(_dataObjs, fileMobUid);
		 if(obj != NULL)
			result = AAFTrue;
		 }
	  }
	else
	  result = AAFTrue;
	
	return (result);
#else
  return AAFFalse;
#endif
}


AAFRESULT ImplAAFHeader::AppendDataObject(aafUID_t mobID,      /* IN - Mob ID */
						  AAFObject *dataObj)    /* IN - Input Mob */ 
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
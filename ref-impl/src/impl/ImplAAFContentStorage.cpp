
/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


#include "AAFTypes.h"
#include "aafErr.h"
#include "AAFResult.h"

#ifndef __ImplAAFMob_h__
#include "ImplAAFMob.h"
#endif

#ifndef __ImplEnumAAFMobs_h__
#include "ImplEnumAAFMobs.h"
#endif

#ifndef __ImplEnumAAFMedia_h__
#include "ImplEnumAAFMedia.h"
#endif

#ifndef __ImplAAFContentStorage_h__
#include "ImplAAFContentStorage.h"
#endif

#include "ImplAAFObjectCreation.h"

#define DEFAULT_NUM_MOBS				1000
#define DEFAULT_NUM_DATAOBJ			200

extern "C" const aafClassID_t	CLSID_AAFContentStorage;
OMDEFINE_STORABLE(ImplAAFContentStorage, CLSID_AAFContentStorage);

extern "C" const aafClassID_t CLSID_EnumAAFMobs;

ImplAAFContentStorage::ImplAAFContentStorage ()
: _mobs(         PID_CONTENT_STORAGE_MOBS,          "mobs") /*!!!,
  _mediaData(      PID_CONTENT_STORAGE_MEDIA,       "mediaData") */
{
  _persistentProperties.put(_mobs.address());
//!!!  _persistentProperties.put(_mediaData.address());

	NewUIDTable(NULL, 100, &_mobIndex);		//!!! Handle error codes
}


ImplAAFContentStorage::~ImplAAFContentStorage ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFContentStorage::LookupMob (aafUID_t *mobID,
                           ImplAAFMob **ppMob)
{
	ImplAAFMob			*tmpMob;
	
	XPROTECT()
	  {
		/* Get the mob out of the mob hash table */
		tmpMob = (ImplAAFMob *)TableUIDLookupPtr(_mobIndex, *mobID);

		if (tmpMob)
		  *ppMob = tmpMob;
		else
		  {
			RAISE(AAFRESULT_MOB_NOT_FOUND);
		  }
	  } /* XPROTECT */

	XEXCEPT
	  {
		return(XCODE());
	  }
	XEND;
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFContentStorage::GetNumMobs (aafMobKind_t mobKind,
                           aafNumSlots_t *pNumMobs)
{
	size_t				siz;
	ImplEnumAAFMobs		*mobEnum;
	aafSearchCrit_t		criteria;
	AAFRESULT			hr;
	ImplAAFMob			*aMob;

	if(pNumMobs == NULL)
		return AAFRESULT_NULL_PARAM;
	
	 if(mobKind == kAllMob)
	{
		_mobs.getSize(siz);
	}
	else
	{
		criteria.searchTag = kByMobKind;
		criteria.tags.mobKind = mobKind;
		GetMobs (&criteria,&mobEnum);
		siz = 0;
		do {
			hr = mobEnum->NextOne (&aMob);
			if(hr == AAFRESULT_SUCCESS)
				siz++;
 		} while(hr == AAFRESULT_SUCCESS);
	}
	
	*pNumMobs = siz;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFContentStorage::GetPrimaryMobs (ImplEnumAAFMobs ** /*ppEnum*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFContentStorage::GetMobs (aafSearchCrit_t *pSearchCriteria,
                           ImplEnumAAFMobs **ppEnum)
{
	ImplEnumAAFMobs		*theEnum = (ImplEnumAAFMobs *)CreateImpl (CLSID_EnumAAFMobs);
		
	XPROTECT()
	{
		CHECK(theEnum->SetContentStorage(this));
		CHECK(theEnum->SetCriteria(pSearchCriteria));
		CHECK(theEnum->Reset());
		*ppEnum = theEnum;
	}
	XEXCEPT
	{
		return(XCODE());
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}

// Internal to the toolkit functions
AAFRESULT
    ImplAAFContentStorage::GetNthMob (aafInt32 index, ImplAAFMob **ppMob)
{
	ImplAAFMob	*obj;
	_mobs.getValueAt(obj, index);
	*ppMob = obj;

	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFContentStorage::AppendMob (ImplAAFMob *pMob)
{
	aafUID_t	mobID;
	
	XPROTECT()
	{
		_mobs.appendValue(pMob);
		CHECK(pMob->GetMobID(&mobID));
		CHECK(TableAddUID(_mobIndex, mobID, pMob, kAafTableDupError));
	} /* XPROTECT */
	XEXCEPT
	{
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFContentStorage::RemoveMob (ImplAAFMob *pMob)
{
	aafUID_t	mobID;

	XPROTECT()
	{
		CHECK(pMob->GetMobID(&mobID));
		CHECK(TableRemoveUID(_mobIndex, mobID));
//!!!	_mobs.removeValue(pMob);	// This call doesn't exist yet
	} /* XPROTECT */
	XEXCEPT
	{
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFContentStorage::IsMediaDataPresent (aafUID_t *  /*pFileMobID*/,
                           aafFileFormat_t  /*fmt*/,
                           aafBool *  /*result*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFContentStorage::GetMedia (aafMediaCriteria_t *  /*pMediaCriteria*/,
                           ImplEnumAAFMedia ** /*ppEnum*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

AAFRESULT ImplAAFContentStorage::LoadMobTables(void)
{
	size_t				mobTableSize;
	aafInt32			siz, n;
	ImplAAFMob			*mob;
	aafUID_t			uid;
	
	XPROTECT()
	{
		/*
		 * Make a table of all of the source & composition mobs
		 */
		_mobs.getSize(mobTableSize);
		siz = mobTableSize;

		mobTableSize *= 2; /* Allow for some growth */
		if(mobTableSize < DEFAULT_NUM_MOBS)
			mobTableSize = DEFAULT_NUM_MOBS;
		CHECK(NewUIDTable(NULL, mobTableSize, &(_mobIndex)));

		for(n = 0; n < siz; n++)
		{
			_mobs.getValueAt(mob, n);
			CHECK(mob->GetMobID(&uid));
			CHECK(TableAddUID(_mobIndex, uid, mob, kAafTableDupAddDup));
		}
	}
	XEXCEPT
	{
	}
	XEND;

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
AAFRESULT ImplAAFContentStorage::BuildMediaCache(void)
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

aafBool ImplAAFContentStorage::IsMediaDataPresent(
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


AAFRESULT ImplAAFContentStorage::AppendDataObject(aafUID_t mobID,      /* IN - Mob ID */
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

AAFRESULT ImplAAFContentStorage::UnlinkMobID(aafUID_t mobID)
{
	XPROTECT()
	{
		CHECK(TableRemoveUID(_mobIndex, mobID));
	} /* XPROTECT */
	XEXCEPT
	{
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}

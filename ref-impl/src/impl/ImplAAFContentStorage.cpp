
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

#ifndef __ImplEnumAAFEssenceData_h__
#include "ImplEnumAAFEssenceData.h"
#endif

#ifndef __ImplAAFEssenceData_h__
#include "ImplAAFEssenceData.h"
#endif

#ifndef __ImplAAFContentStorage_h__
#include "ImplAAFContentStorage.h"
#endif

#include "AAFPropertyIDs.h"

#include "ImplAAFObjectCreation.h"

#define DEFAULT_NUM_MOBS				1000
#define DEFAULT_NUM_DATAOBJ			200

extern "C" const aafClassID_t	CLSID_AAFContentStorage;
OMDEFINE_STORABLE(ImplAAFContentStorage, CLSID_AAFContentStorage);

extern "C" const aafClassID_t CLSID_EnumAAFMobs;
extern "C" const aafClassID_t CLSID_EnumAAFEssenceData;

ImplAAFContentStorage::ImplAAFContentStorage ()
: _mobIndex(0),
  _mobs(PID_CONTENT_STORAGE_MOBS, "mobs"),
  _essenceData(PID_CONTENT_STORAGE_ESSENCEDATA, "essenceData")
{
  _persistentProperties.put(_mobs.address());
  _persistentProperties.put(_essenceData.address());

  NewUIDTable(NULL, 100, &_mobIndex);		//!!! Handle error codes
}


ImplAAFContentStorage::~ImplAAFContentStorage ()
{
	// Cleanup the non-persistent data...
	TableDispose(_mobIndex);
	_mobIndex = 0;

	// Cleanup the persistent data...
	ImplAAFEssenceData *pEssenceData = NULL;
	ImplAAFMob *pMob = NULL;
	size_t size, i;

	// Release the essence data
	_essenceData.getSize(size);
	for (i = 0; i < size; i++)
	{
		_essenceData.getValueAt(pEssenceData, i);
		if (pEssenceData)
		{
		  pEssenceData->ReleaseReference();
		  // Set value to 0 so OM can perform necessary cleanup.
		  _essenceData.setValueAt(0, i);
		}
	}
	
	// Release the mobs
	_mobs.getSize(size);
	for (i = 0; i < size; i++)
	{
		_mobs.getValueAt(pMob, i);
		if (pMob)
		{
		  pMob->ReleaseReference();
		  // Set value to 0 so OM can perform necessary cleanup.
		  _mobs.setValueAt(0, i);
		}
	}
}


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
			{
		  *ppMob = tmpMob;
			// trr - We are returning a copy of mob pointer so we need to 
			// bump the reference count. Note: The copy that is in the 
			// table has NOT be reference counted.
			tmpMob->AcquireReference();
			}
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
	ImplEnumAAFMobs		*mobEnum = NULL;
	aafSearchCrit_t		criteria;
	AAFRESULT			hr = AAFRESULT_SUCCESS;
	ImplAAFMob			*aMob = NULL;

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
		hr = GetMobs (&criteria,&mobEnum);
		siz = 0;
		do {
			hr = mobEnum->NextOne (&aMob);
			if(hr == AAFRESULT_SUCCESS)
			{
			  siz++;
			  aMob->ReleaseReference();
			  aMob = NULL;
			}
 		} while(hr == AAFRESULT_SUCCESS);
		if(hr == AAFRESULT_NO_MORE_MOBS)
			hr = AAFRESULT_SUCCESS;
		if (mobEnum)
		  mobEnum->ReleaseReference();
	}
	
	*pNumMobs = siz;
	return hr;
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
	if (NULL == ppEnum)
		return AAFRESULT_NULL_PARAM;
	*ppEnum = 0;
	
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
		if (theEnum)
			theEnum->ReleaseReference();
		return(XCODE());
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}

// Internal to the toolkit functions
AAFRESULT
    ImplAAFContentStorage::GetNthMob (aafInt32 index, ImplAAFMob **ppMob)
{
	ImplAAFMob	*obj = NULL;
	_mobs.getValueAt(obj, index);
	*ppMob = obj;
	
	// trr - We are returning a copy of pointer stored in _mobs so we need
	// to bump its reference count.
	if (obj)
		obj->AcquireReference();
	else
		return AAFRESULT_NO_MORE_MOBS;

	return AAFRESULT_SUCCESS;
}

AAFRESULT
    ImplAAFContentStorage::GetNthEssenceData (aafInt32 index, ImplAAFEssenceData **ppEssenceData)
{
  if (NULL == ppEssenceData)
    return AAFRESULT_NULL_PARAM;

  ImplAAFEssenceData *obj = NULL;
  _essenceData.getValueAt(obj, index);
  *ppEssenceData = obj;
	
  // trr - We are returning a copy of pointer stored in _mobs so we need
  // to bump its reference count.
  if (obj)
    obj->AcquireReference();
  else
    return AAFRESULT_NO_MORE_OBJECTS;

  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFContentStorage::AppendMob (ImplAAFMob *pMob)
{
	aafUID_t	mobID;
	ImplAAFMob	*test;

	if (NULL == pMob)
		return AAFRESULT_NULL_PARAM;
	
	XPROTECT()
	{
		CHECK(pMob->GetMobID(&mobID));

		// JeffB: Test is a throwaway, so don't bump the refcount
		if(LookupMob (&mobID, &test) == AAFRESULT_MOB_NOT_FOUND)
		{
			_mobs.appendValue(pMob);
			// trr - We are saving a copy of pointer in _mobs so we need
			// to bump its reference count.
			pMob->AcquireReference();
			CHECK(TableAddUID(_mobIndex, mobID, pMob, kAafTableDupError));
		}
		else
			RAISE(AAFRESULT_DUPLICATE_MOBID);
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
	if (NULL == pMob)
		return AAFRESULT_NULL_PARAM;

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

AAFRESULT
    ImplAAFContentStorage::ChangeIndexedMobID (ImplAAFMob *pMob, aafUID_t *newID)
{
	aafUID_t	mobID;
	if (NULL == pMob)
		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		CHECK(pMob->GetMobID(&mobID));
		CHECK(TableAddUID(_mobIndex, *newID, pMob, kAafTableDupError));
		CHECK(TableRemoveUID(_mobIndex, mobID));
	} /* XPROTECT */
	XEXCEPT
	{
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFContentStorage::GetNumEssenceData(aafUInt32 *  pNumEssenceData)
{
  size_t siz;

  if(pNumEssenceData == NULL)
    return AAFRESULT_NULL_PARAM;
  
  _essenceData.getSize(siz);
  
  *pNumEssenceData = siz;
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFContentStorage::IsEssenceDataPresent (aafUID_t *  /*pFileMobID*/,
                           aafFileFormat_t  /*fmt*/,
                           aafBool *  /*pResult*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFContentStorage::EnumEssenceData (ImplEnumAAFEssenceData ** ppEnum)
{
	if (NULL == ppEnum)
		return AAFRESULT_NULL_PARAM;
	*ppEnum = 0;
	
	ImplEnumAAFEssenceData *theEnum = (ImplEnumAAFEssenceData *)CreateImpl (CLSID_EnumAAFEssenceData);
	
	XPROTECT()
	{
		CHECK(theEnum->SetContentStorage(this));
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
    ImplAAFContentStorage::AppendEssenceData (ImplAAFEssenceData * pEssenceData)
{
	aafUID_t	mobID;
	ImplAAFMob	*pMatchingMob = NULL;

	if (NULL == pEssenceData)
		return AAFRESULT_NULL_PARAM;
	
	XPROTECT()
	{
		// Get the file mob id so that we can validate
		// that there is a file mob already associated
		// with the same AAF file.
		CHECK(pEssenceData->GetFileMobID(&mobID));

		// Make sure the mob id matches an existing mob.
		CHECK(LookupMob (&mobID, &pMatchingMob));


		_essenceData.appendValue(pEssenceData);
		// trr - We are saving a copy of pointer in _essenceData so we need
		// to bump its reference count.
		pEssenceData->AcquireReference();

	} /* XPROTECT */
	XEXCEPT
	{
	}
	XEND;

	if (pMatchingMob)
		pMatchingMob->ReleaseReference();	
	
	return(AAFRESULT_SUCCESS);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFContentStorage::RemoveEssenceData (ImplAAFEssenceData * pEssenceData)
{
//	aafUID_t	mobID;
	if (NULL == pEssenceData)
		return AAFRESULT_NULL_PARAM;

#if 0
	XPROTECT()
	{
		//!!!_essenceData.removeValue(pEssenceData);	// This call doesn't exist yet
	} /* XPROTECT */
	XEXCEPT
	{
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
#else
	return(AAFRESULT_NOT_IMPLEMENTED);
#endif
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
		// dispose of the old table
		if (_mobIndex)
		{
			TableDispose(_mobIndex);
			_mobIndex = 0;
		}
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

aafBool ImplAAFContentStorage::IsEssenceDataPresent(aafUID_t fileMobUid,	/* IN -- */ aafFileFormat_t fmt)
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


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
#include "aafErr.h"
#include "AAFResult.h"
#include "aafUtils.h"

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

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include "ImplAAFObjectCreation.h"

#include <assert.h>

#define DEFAULT_NUM_MOBS				1000
#define DEFAULT_NUM_DATAOBJ			200


extern "C" const aafClassID_t CLSID_EnumAAFMobs;
extern "C" const aafClassID_t CLSID_EnumAAFEssenceData;
static void ReleaseMobRefs(void *valuePtr)
{
	ReleaseImplReference((ImplAAFRoot *)valuePtr);
}

ImplAAFContentStorage::ImplAAFContentStorage ()
: _mobIndex(0),
  _mobs(PID_ContentStorage_Mobs, "Mobs"),
  _essenceData(PID_ContentStorage_EssenceData, "EssenceData")
{
	_persistentProperties.put(_mobs.address());
	_persistentProperties.put(_essenceData.address());

	NewMobIDTable(100, &_mobIndex);		//!!! Handle error codes
//	(void)SetTableDispose(_mobIndex, ReleaseMobRefs);
}



ImplAAFContentStorage::~ImplAAFContentStorage ()
{
	// Cleanup the non-persistent data...
  if (_mobIndex)
  {
	  TableDispose(_mobIndex);
  	_mobIndex = 0;
  }

	// Cleanup the persistent data...

	size_t i;
	// Release the essence data
	size_t size = _essenceData.getSize();
	for (i = 0; i < size; i++)
	{
		ImplAAFEssenceData *pEssenceData = _essenceData.setValueAt(0, i);
		if (pEssenceData)
		{
		  pEssenceData->ReleaseReference();
		  pEssenceData = 0;
		}
	}
	
	// Release the mobs
	size = _mobs.getSize();
	for (i = 0; i < size; i++)
	{
		ImplAAFMob *pMob = _mobs.setValueAt(0, i);
		if (pMob)
		{
		  pMob->ReleaseReference();
		  pMob = 0;
		}
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFContentStorage::LookupMob (aafMobID_constref mobID,
                           ImplAAFMob **ppMob)
{
	ImplAAFMob			*tmpMob;
	
	XPROTECT()
	  {
		/* Get the mob out of the mob hash table */
		tmpMob = (ImplAAFMob *)TableMobIDLookupPtr(_mobIndex, mobID);

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
    ImplAAFContentStorage::CountMobs (aafMobKind_t mobKind,
                           aafNumSlots_t *pNumMobs)
{
	size_t				siz;
	ImplEnumAAFMobs		*mobEnum = NULL;
	aafSearchCrit_t		criteria;
	AAFRESULT			hr = AAFRESULT_SUCCESS;
	ImplAAFMob			*aMob = NULL;

	if(pNumMobs == NULL)
		return AAFRESULT_NULL_PARAM;
	
	if(mobKind == kAAFAllMob)
	{
		_mobs.getSize(siz);
	}
	else
	{
		criteria.searchTag = kAAFByMobKind;
		criteria.tags.mobKind = mobKind;
		hr = GetMobs (&criteria,&mobEnum);
		siz = 0;
		do {
			hr = mobEnum->NextOne (&aMob);
			if(hr == AAFRESULT_SUCCESS)
			{
			  siz++;
			  assert (aMob);
			  aMob->ReleaseReference();
			  aMob = NULL;
			}
 		} while(hr == AAFRESULT_SUCCESS);
		if(hr == AAFRESULT_NO_MORE_MOBS)
			hr = AAFRESULT_SUCCESS;
		if (mobEnum)
		  {
			mobEnum->ReleaseReference();
			mobEnum = 0;
		  }
	}
	
	*pNumMobs = siz;
	return hr;
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
		  {
			theEnum->ReleaseReference();
			theEnum = 0;
		  }
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
    ImplAAFContentStorage::AddMob (ImplAAFMob *pMob)
{
	aafMobID_t	mobID;
	ImplAAFMob	*test;

	if (NULL == pMob)
		return AAFRESULT_NULL_PARAM;
	
	XPROTECT()
	{
		CHECK(pMob->GetMobID(&mobID));

		// JeffB: Test is a throwaway, so don't bump the refcount
		if(LookupMob (mobID, &test) == AAFRESULT_MOB_NOT_FOUND)
		{
			_mobs.appendValue(pMob);
			// trr - We are saving a copy of pointer in _mobs so we need
			// to bump its reference count.
			pMob->AcquireReference();
			CHECK(TableAddMobID(_mobIndex, mobID, pMob, kAafTableDupError));
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
	aafMobID_t	mobID;
	if (NULL == pMob)
		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		CHECK(pMob->GetMobID(&mobID));
		CHECK(TableRemoveMobID(_mobIndex, mobID));
		_mobs.removeValue(pMob);
	} /* XPROTECT */
	XEXCEPT
	{
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}

AAFRESULT
    ImplAAFContentStorage::ChangeIndexedMobID (ImplAAFMob *pMob, aafMobID_constref newID)
{
	aafMobID_t	mobID;
	if (NULL == pMob)
		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		CHECK(pMob->GetMobID(&mobID));
		CHECK(TableAddMobID(_mobIndex, newID, pMob, kAafTableDupError));
		CHECK(TableRemoveMobID(_mobIndex, mobID));
	} /* XPROTECT */
	XEXCEPT
	{
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFContentStorage::CountEssenceData(aafUInt32 *  pNumEssenceData)
{
  size_t siz;

  if(pNumEssenceData == NULL)
    return AAFRESULT_NULL_PARAM;
  
  _essenceData.getSize(siz);
  
  *pNumEssenceData = siz;
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFContentStorage::IsEssenceDataPresent (aafMobID_constref fileMobID,
                           aafFileFormat_t fmt,
                           aafBool *pResult)
{
	aafUInt32			numEssence, n;
	aafMobID_t			testMobID;
	ImplAAFEssenceData	*testData = NULL;

	XPROTECT()
	{
		//!!!fmt is unused?
		*pResult = kAAFFalse;
		CHECK(CountEssenceData(&numEssence));
		for(n = 0; n < numEssence && !(*pResult); n ++)
		{
			CHECK(GetNthEssenceData(n, &testData));
			CHECK(testData->GetFileMobID (&testMobID));
			if(memcmp(&fileMobID, &testMobID, sizeof(testMobID)) == 0)
			{
				*pResult = kAAFTrue;
			}
			testData->ReleaseReference();
			testData = NULL;
		}
	}
	XEXCEPT
	{
		if(testData != NULL)
		  {
			testData->ReleaseReference();
			testData = 0;
		  }
	}
	XEND

	return AAFRESULT_SUCCESS;
}

//Internal function only.  Not exposed through the COM API
AAFRESULT
    ImplAAFContentStorage::LookupEssence (aafMobID_constref fileMobID,
                           ImplAAFEssenceData **ppEssence)
{
	aafUInt32		numEssence, n;
	aafBool			found;
	aafMobID_t		testMobID;
	ImplAAFEssenceData	*testData = NULL;

	XPROTECT()
	{
		found = kAAFFalse;
		CHECK(CountEssenceData(&numEssence));
		for(n = 0; n < numEssence && !found; n ++)
		{
			CHECK(GetNthEssenceData(n, &testData));
			CHECK(testData->GetFileMobID (&testMobID));
			if(memcmp(&fileMobID, &testMobID, sizeof(testMobID)) == 0)
			{
				found = kAAFTrue;
				*ppEssence = testData;
				(*ppEssence)->AcquireReference();
			}
			testData->ReleaseReference();
			testData = NULL;
		}
		// trr - We are returning a copy of pointer stored in _mobs so we need
		// to bump its reference count.
		if (!found)
			RAISE(AAFRESULT_NO_MORE_OBJECTS);
	}
	XEXCEPT
	{
		if(testData != NULL)
		  {
			testData->ReleaseReference();
			testData = 0;
		  }
	}
	XEND

	return AAFRESULT_SUCCESS;
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
		  {
			theEnum->ReleaseReference();
			theEnum = 0;
		  }
		return(XCODE());
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
 }



AAFRESULT STDMETHODCALLTYPE
    ImplAAFContentStorage::AddEssenceData (ImplAAFEssenceData * pEssenceData)
{
	aafMobID_t	mobID;
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
		CHECK(LookupMob (mobID, &pMatchingMob));


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
	  {
		pMatchingMob->ReleaseReference();	
		pMatchingMob = 0;
	  }
	
	return(AAFRESULT_SUCCESS);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFContentStorage::RemoveEssenceData (ImplAAFEssenceData * pEssenceData)
{
	if (NULL == pEssenceData)
		return AAFRESULT_NULL_PARAM;

		//!!!_essenceData.removeValue(pEssenceData);	// This call doesn't exist yet
	
	return(AAFRESULT_SUCCESS);
}

AAFRESULT ImplAAFContentStorage::LoadMobTables(void)
{
	size_t				mobTableSize;
	aafInt32			siz, n;
	ImplAAFMob			*mob;
	aafMobID_t			mobID;
	
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
		CHECK(NewMobIDTable(mobTableSize, &(_mobIndex)));

		for(n = 0; n < siz; n++)
		{
			_mobs.getValueAt(mob, n);
			CHECK(mob->GetMobID(&mobID));
			CHECK(TableAddMobID(_mobIndex, mobID, mob, kAafTableDupAddDup));
		}
	}
	XEXCEPT
	{
	}
	XEND;

	return (AAFRESULT_SUCCESS);
}

AAFRESULT ImplAAFContentStorage::UnlinkMobID(aafMobID_constref mobID)
{
	XPROTECT()
	{
		CHECK(TableRemoveMobID(_mobIndex, mobID));
	} /* XPROTECT */
	XEXCEPT
	{
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}

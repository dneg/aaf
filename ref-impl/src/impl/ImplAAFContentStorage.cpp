
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


#include "AAFTypes.h"
#include "aafErr.h"
#include "AAFResult.h"
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
: _mobs(PID_ContentStorage_Mobs, L"Mobs", PID_Mob_MobID),
  _essenceData(PID_ContentStorage_EssenceData, L"EssenceData", PID_EssenceData_MobID)
{
	_persistentProperties.put(_mobs.address());
	_persistentProperties.put(_essenceData.address());
}



ImplAAFContentStorage::~ImplAAFContentStorage ()
{
	// Cleanup the persistent data...
	// Release the essence data
	OMStrongReferenceSetIterator<OMMaterialIdentification, ImplAAFEssenceData>essenceData(_essenceData);
	while(++essenceData)
	{
		ImplAAFEssenceData *pData = essenceData.clearValue();
		if (pData)
		{
		  pData->ReleaseReference();
		  pData = 0;
		}
	}
	
	// Release the mobs
	OMStrongReferenceSetIterator<OMMaterialIdentification, ImplAAFMob>mobs(_mobs);
	while(++mobs)
	{
		ImplAAFMob *pMob = mobs.clearValue();
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
  if (! ppMob)
    return AAFRESULT_NULL_PARAM;

	
	AAFRESULT result = AAFRESULT_SUCCESS;
  // NOTE: The following type cast is temporary. It should be removed as soon
	// as the OM has a declarative sytax to include the type
	// of the key used in the set. (trr:2000-FEB-26)
	if (_mobs.find((*reinterpret_cast<const OMMaterialIdentification *>(&mobID)),
                             *ppMob))
	{
		assert(NULL != *ppMob);
		(*ppMob)->AcquireReference();
	}
	else
	{
		// no recognized class guid in dictionary
		result = AAFRESULT_MOB_NOT_FOUND;
	}

	return (result);

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
		siz = _mobs.count();
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
		if(hr == AAFRESULT_NO_MORE_OBJECTS)
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
	
  ImplEnumAAFMobs *theEnum = (ImplEnumAAFMobs *)CreateImpl (CLSID_EnumAAFMobs);
	
  XPROTECT()
	{
		OMStrongReferenceSetIterator<OMMaterialIdentification, ImplAAFMob>* iter = 
			new OMStrongReferenceSetIterator<OMMaterialIdentification, ImplAAFMob>(_mobs);
		if(iter == 0)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(theEnum->Initialize(&CLSID_EnumAAFMobs, this, iter));
		CHECK(theEnum->SetCriteria(pSearchCriteria));
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
    ImplAAFContentStorage::AddMob (ImplAAFMob *pMob)
{
	aafMobID_t	mobID;

	if (NULL == pMob)
		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		CHECK(pMob->GetMobID(&mobID));

		// JeffB: Test is a throwaway, so don't bump the refcount
		if(!_mobs.contains((*reinterpret_cast<const OMMaterialIdentification *>(&mobID))))
		{
			if (pMob->attached ())
				return AAFRESULT_OBJECT_ALREADY_ATTACHED;
				
			_mobs.appendValue(pMob);
			// trr - We are saving a copy of pointer in _mobs so we need
			// to bump its reference count.
			pMob->AcquireReference();
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
		
	if (!pMob->attached())
		return AAFRESULT_MOB_NOT_FOUND;	
		
	XPROTECT()
	{
		CHECK(pMob->GetMobID(&mobID));

		if(_mobs.contains((*reinterpret_cast<const OMMaterialIdentification *>(&mobID))))
		{
			_mobs.removeValue(pMob);
			pMob->ReleaseReference(); // the set no longer owns this reference.
		}
		else
			RAISE(AAFRESULT_MOB_NOT_FOUND);
	}
	XEXCEPT
	{
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}

AAFRESULT
    ImplAAFContentStorage::ChangeIndexedMobID (ImplAAFMob *pMob, aafMobID_constref /*newID*/)
{
	aafMobID_t	mobID;
	if (NULL == pMob)
		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		CHECK(pMob->GetMobID(&mobID));
	} /* XPROTECT */
	XEXCEPT
	{
	}
	XEND;
	
	return(AAFRESULT_NOT_IN_CURRENT_VERSION);
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFContentStorage::CountEssenceData(aafUInt32 *  pNumEssenceData)
{
  if(pNumEssenceData == NULL)
    return AAFRESULT_NULL_PARAM;
    
  *pNumEssenceData = _essenceData.count();
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFContentStorage::IsEssenceDataPresent (aafMobID_constref fileMobID,
                           aafFileFormat_t /*fmt*/,
                           aafBool *pResult)
{
	*pResult = _essenceData.contains((*reinterpret_cast<const OMMaterialIdentification *>(&fileMobID)));
	return AAFRESULT_SUCCESS;
}

AAFRESULT
    ImplAAFContentStorage::LookupEssenceData (aafMobID_constref fileMobID,
                           ImplAAFEssenceData **ppEssence)
{
  if (! ppEssence)
    return AAFRESULT_NULL_PARAM;

	
	AAFRESULT result = AAFRESULT_SUCCESS;
  // NOTE: The following type cast is temporary. It should be removed as soon
	// as the OM has a declarative sytax to include the type
	// of the key used in the set. (trr:2000-FEB-26)
	if (_essenceData.find((*reinterpret_cast<const OMMaterialIdentification *>(&fileMobID)),
                             *ppEssence))
	{
		assert(NULL != *ppEssence);
		(*ppEssence)->AcquireReference();
	}
	else
	{
		// no recognized class guid in dictionary
		result = AAFRESULT_MOB_NOT_FOUND;
	}

	return (result);
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
		OMStrongReferenceSetIterator<OMMaterialIdentification, ImplAAFEssenceData>* iter = 
			new OMStrongReferenceSetIterator<OMMaterialIdentification, ImplAAFEssenceData>(_essenceData);
		if(iter == 0)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(theEnum->Initialize(&CLSID_EnumAAFEssenceData, this, iter));
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
    ImplAAFContentStorage::AddEssenceData (ImplAAFEssenceData * pEssenceData)
{
	aafMobID_t	mobID;

	if (NULL == pEssenceData)
		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		CHECK(pEssenceData->GetFileMobID(&mobID));

		// JeffB: Test is a throwaway, so don't bump the refcount
		if(!_essenceData.contains((*reinterpret_cast<const OMMaterialIdentification *>(&mobID))))
		{
			if (pEssenceData->attached())
				return AAFRESULT_OBJECT_ALREADY_ATTACHED;
				
			_essenceData.appendValue(pEssenceData);
			// trr - We are saving a copy of pointer in _mobs so we need
			// to bump its reference count.
			pEssenceData->AcquireReference();
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
    ImplAAFContentStorage::RemoveEssenceData (ImplAAFEssenceData * pEssenceData)
{
	aafMobID_t	mobID;

	if (NULL == pEssenceData)
		return AAFRESULT_NULL_PARAM;

	if (!pEssenceData->attached())
		return AAFRESULT_ESSENCE_NOT_FOUND;

	XPROTECT()
	{
		CHECK(pEssenceData->GetFileMobID(&mobID));

		if(_essenceData.contains((*reinterpret_cast<const OMMaterialIdentification *>(&mobID))))
		{
			_essenceData.removeValue(pEssenceData);
			pEssenceData->ReleaseReference();
		}
		else
			RAISE(AAFRESULT_ESSENCE_NOT_FOUND);
	} 
	XEXCEPT
	{
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}

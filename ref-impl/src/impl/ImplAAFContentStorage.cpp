
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
    ImplAAFContentStorage::ChangeIndexedMobID (ImplAAFMob *pMob, aafMobID_constref newID)
{
	aafMobID_t	mobID;
	if (NULL == pMob)
		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		CHECK(pMob->GetMobID(&mobID));
		RAISE(AAFRESULT_NOT_IMPLEMENTED);
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
  if(pNumEssenceData == NULL)
    return AAFRESULT_NULL_PARAM;
    
  *pNumEssenceData = _essenceData.count();
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFContentStorage::IsEssenceDataPresent (aafMobID_constref fileMobID,
                           aafFileFormat_t fmt,
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

AAFRESULT ImplAAFContentStorage::UnlinkMobID(aafMobID_constref mobID)
{
	return(AAFRESULT_NOT_IMPLEMENTED);
}

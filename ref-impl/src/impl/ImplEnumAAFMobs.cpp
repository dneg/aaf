
/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


#ifndef __ImplAAFMob_h__
#include "ImplAAFMob.h"
#endif






#ifndef __ImplEnumAAFMobs_h__
#include "ImplEnumAAFMobs.h"
#endif

#include <assert.h>
#include "AAFResult.h"
#include "ImplAAFObjectCreation.h"

extern "C" const aafClassID_t CLSID_EnumAAFMobs;

OMDEFINE_STORABLE(ImplEnumAAFMobs, CLSID_EnumAAFMobs);

ImplEnumAAFMobs::ImplEnumAAFMobs ()
{
	_current = 0;
	_cStorage = NULL;
	_criteria.searchTag = kNoSearch;
}


ImplEnumAAFMobs::~ImplEnumAAFMobs ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFMobs::NextOne (ImplAAFMob **ppMob)
{
	aafNumSlots_t	cur = _current, siz;
	aafBool			found = AAFFalse;

    XPROTECT()
	{
		CHECK(_cStorage->GetNumMobs (kAllMob, &siz));
		if(cur < siz)
		{
			found = AAFFalse;
			do {
				CHECK(_cStorage->GetNthMob (cur, ppMob));
				_current = ++cur;
				switch(_criteria.searchTag)
				{
				case kNoSearch:
					found = AAFTrue;
					break;

				case kByMobID:
					aafUID_t	testMOBID;

					CHECK((*ppMob)->GetMobID (&testMOBID));
					if(memcmp(&testMOBID, &(_criteria.tags.mobID), sizeof(testMOBID) == 0))
						found = AAFTrue;
					break;

				case kByMobKind:
					aafMobKind_t	kind;
				
					CHECK((*ppMob)->GetMobKind (&kind));
					if((kind == _criteria.tags.mobKind) || (kAllMob == _criteria.tags.mobKind))
						found = AAFTrue;
					break;
				case kByName:
//				aafString_t *name;
				case kByClass:
//				aafClassID_t objClass;	// shouldn't this be a pointer?
				case kByDatakind:
//				aafUID_t datadef;	// shouldn't this be a pointer?
				case kByMediaCrit:
//				aafCriteriaType_t mediaCrit;
					RAISE(AAFRESULT_NOT_IMPLEMENTED);
				}
			} while(!found && cur < siz);
		}
	
		if(!found)
			RAISE(AAFRESULT_NO_MORE_MOBS);
	}
	XEXCEPT
	XEND

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFMobs::Next (aafUInt32  /*count*/,
                           ImplAAFMob ** /*ppMobs*/,
                           aafUInt32 *  /*pFetched*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFMobs::Skip (aafUInt32  /*count*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFMobs::Reset ()
{
	_current = 0;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFMobs::Clone (ImplEnumAAFMobs **ppEnum)
{
	ImplEnumAAFMobs		*result;

	result = (ImplEnumAAFMobs *)CreateImpl(CLSID_EnumAAFMobs);
	if(result != NULL)
	{
		result->_current = _current;
		result->_cStorage = _cStorage;
		result->_criteria = _criteria;
		*ppEnum = result;
		return AAFRESULT_SUCCESS;
	}
	else
		return AAFRESULT_NOMEMORY;
}

//Internal
AAFRESULT
    ImplEnumAAFMobs::SetContentStorage(ImplAAFContentStorage *pCStore)
{
	_cStorage = pCStore;
	return AAFRESULT_SUCCESS;
}

AAFRESULT
    ImplEnumAAFMobs::SetCriteria(aafSearchCrit_t *pCriteria)
{
	if(NULL == pCriteria)
		_criteria.searchTag = kNoSearch;
	else
		_criteria = *pCriteria;

	return AAFRESULT_SUCCESS;
}



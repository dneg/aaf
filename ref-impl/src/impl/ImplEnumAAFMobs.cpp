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


ImplEnumAAFMobs::ImplEnumAAFMobs ()
{
	_current = 0;
	_cStorage = NULL;
	_criteria.searchTag = kNoSearch;
}


ImplEnumAAFMobs::~ImplEnumAAFMobs ()
{
  if (_cStorage)
  {
    _cStorage->ReleaseReference();
    _cStorage = NULL;
  }
}


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
				case kByDataDef:
//				aafUID_t datadef;	// shouldn't this be a pointer?
				case kByMediaCrit:
//				aafCriteriaType_t mediaCrit;
					RAISE(AAFRESULT_NOT_IMPLEMENTED);
				}
				if(!found)
					(*ppMob)->ReleaseReference();

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
    ImplEnumAAFMobs::Next (aafUInt32 count,
                           ImplAAFMob **ppMobs,
                           aafUInt32 *pFetched)
{
	ImplAAFMob**	ppMob;
	aafUInt32		numMobs;
	HRESULT			hr;

	if ((pFetched == NULL && count != 1) || (pFetched != NULL && count == 1))
		return E_INVALIDARG;

	// Point at the first component in the array.
	ppMob = ppMobs;
	for (numMobs = 0; numMobs < count; numMobs++)
	{
		hr = NextOne(ppMob);
		if (FAILED(hr))
			break;

		// Point at the next component in the array.  This
		// will increment off the end of the array when
		// numComps == count-1, but the for loop should
		// prevent access to this location.
		ppMob++;
	}
	
	if (pFetched)
		*pFetched = numMobs;

	return hr;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFMobs::Skip (aafUInt32 count)
{
	AAFRESULT	hr;
	aafInt32	newCurrent, siz;

	newCurrent = _current + count;

 	switch(_criteria.searchTag)
	{
		case kNoSearch:
			_cStorage->GetNumMobs(kAllMob, &siz);
			break;

		case kByMobKind:
			_cStorage->GetNumMobs(_criteria.tags.mobKind, &siz);
			break;

		case kByMobID:
		case kByName:
//			aafString_t *name;
		case kByClass:
//			aafClassID_t objClass;	// shouldn't this be a pointer?
		case kByDataDef:
//			aafUID_t datadef;	// shouldn't this be a pointer?
		case kByMediaCrit:
//		aafCriteriaType_t mediaCrit;
			return(AAFRESULT_NOT_IMPLEMENTED);
		}

	if(newCurrent < siz)
	{
		_current = newCurrent;
		hr = AAFRESULT_SUCCESS;
	}
	else
	{
		hr = E_FAIL;
	}

	return hr;
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
	ImplEnumAAFMobs*	theEnum;
	HRESULT				hr;
	
	theEnum = (ImplEnumAAFMobs *)CreateImpl(CLSID_EnumAAFMobs);
	if (theEnum == NULL)
		return E_FAIL;
		
	hr = theEnum->SetContentStorage(_cStorage);
	if (SUCCEEDED(hr))
	{
		theEnum->Reset();
		theEnum->Skip(_current);
		*ppEnum = theEnum;
	}
	else
	{
		theEnum->ReleaseReference();
		*ppEnum = NULL;
	}

	return hr;
}

//Internal
AAFRESULT
    ImplEnumAAFMobs::SetContentStorage(ImplAAFContentStorage *pCStore)
{
  if (_cStorage)
		_cStorage->ReleaseReference();

	_cStorage = pCStore;

  if (pCStore)
		pCStore->AcquireReference();

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



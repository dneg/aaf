
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

ImplEnumAAFMobs::ImplEnumAAFMobs ()
{
	_current = 0;
	_cStorage = NULL;
}


ImplEnumAAFMobs::~ImplEnumAAFMobs ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFMobs::NextOne (ImplAAFMob **ppMob)
{
	AAFRESULT		result;
	aafNumSlots_t	cur = _current, siz;

    _cStorage->GetNumMobs (kAllMob /*!!! */, &siz);
	if(cur < siz)
	{
		result = _cStorage->GetNthMob (cur, ppMob);
		_current = ++cur;
	}
	else
		result = AAFRESULT_NO_MORE_MOBS;

	return result;
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
    ImplEnumAAFMobs::Clone (ImplEnumAAFMobs ** /*ppEnum*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

//Internal
AAFRESULT
    ImplEnumAAFMobs::SetContentStorage(ImplAAFContentStorage *pCStore)
{
	_cStorage = pCStore;
	return AAFRESULT_SUCCESS;
}



extern "C" const aafClassID_t CLSID_EnumAAFMobs;

OMDEFINE_STORABLE(ImplEnumAAFMobs, CLSID_EnumAAFMobs);


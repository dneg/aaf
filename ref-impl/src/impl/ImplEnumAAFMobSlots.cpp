/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#ifndef __ImplAAFMobSlot_h__
#include "ImplAAFMobSlot.h"
#endif






#ifndef __ImplEnumAAFMobSlots_h__
#include "ImplEnumAAFMobSlots.h"
#endif

#include <assert.h>

#include "ImplAAFMob.h"
#include "AAFResult.h"

ImplEnumAAFMobSlots::ImplEnumAAFMobSlots ()
{
	_current = 0;
	_mob = NULL;
}


ImplEnumAAFMobSlots::~ImplEnumAAFMobSlots ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFMobSlots::NextOne (ImplAAFMobSlot **ppMobSlot)
{
	AAFRESULT		result;
	aafNumSlots_t	cur = _current, siz;

    _mob->GetNumSlots (&siz);
	if(cur < siz)
	{
		result = _mob->GetNthMobSlot (cur, ppMobSlot);
		_current = ++cur;
	}
	else
		result = AAFRESULT_NO_MORE_OBJECTS;

	return result;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFMobSlots::Next (aafUInt32  /*count*/,
                           ImplAAFMobSlot ** /*ppMobSlots*/,
                           aafUInt32 *  /*pFetched*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFMobSlots::Skip (aafUInt32  /*count*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFMobSlots::Reset ()
{
	_current = 0;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFMobSlots::Clone (ImplEnumAAFMobSlots ** /*ppEnum*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


//Internal
AAFRESULT
    ImplEnumAAFMobSlots::SetEnumMob(ImplAAFMob *aMob)
{
	_mob = aMob;
	return AAFRESULT_SUCCESS;
}

extern "C" const aafClassID_t CLSID_EnumAAFMobSlots;

OMDEFINE_STORABLE(ImplEnumAAFMobSlots, CLSID_EnumAAFMobSlots);


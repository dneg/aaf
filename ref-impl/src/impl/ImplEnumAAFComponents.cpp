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


#include "ImplAAFComponent.h"
#include "ImplEnumAAFComponents.h"
#include "ImplAAFSequence.h"
#include "AAFResult.h"

#include <assert.h>


ImplEnumAAFComponents::ImplEnumAAFComponents ()
{
	_current = 0;
	_pSequence = NULL;
}


ImplEnumAAFComponents::~ImplEnumAAFComponents ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFComponents::NextOne (ImplAAFComponent ** ppComponent)
{
	AAFRESULT	result;
	aafInt32	cur = _current, siz;

    _pSequence->GetNumCpnts(&siz);
	if(cur < siz)
	{
		result = _pSequence->GetNthComponent(cur, ppComponent);
		_current = ++cur;
	}
	else
		result = AAFRESULT_NO_MORE_OBJECTS;

	return result;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFComponents::Next (aafUInt32  /*count*/,
                           ImplAAFComponent ** /*ppComponents*/,
                           aafUInt32 *  /*pFetched*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFComponents::Skip (aafUInt32  /*count*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFComponents::Reset ()
{
	_current = 0;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFComponents::Clone (ImplEnumAAFComponents ** /*ppEnum*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


//Internal
AAFRESULT
    ImplEnumAAFComponents::SetEnumSequence(ImplAAFSequence * pSequence)
{
	_pSequence = pSequence;
	return AAFRESULT_SUCCESS;
}

extern "C" const aafClassID_t CLSID_EnumAAFComponents;

OMDEFINE_STORABLE(ImplEnumAAFComponents, CLSID_EnumAAFComponents);


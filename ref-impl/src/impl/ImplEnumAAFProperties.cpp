
/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#ifndef __ImplAAFProperty_h__
#include "ImplAAFProperty.h"
#endif

#ifndef __ImplEnumAAFProperties_h__
#include "ImplEnumAAFProperties.h"
#endif

#include "ImplEnumerator.h"
#include "ImplAAFObjectCreation.h"

#include <assert.h>
#include <string.h>

extern "C" const aafClassID_t CLSID_EnumAAFProperties;


ImplEnumAAFProperties::ImplEnumAAFProperties ()
  : _rep (0)
{}


ImplEnumAAFProperties::~ImplEnumAAFProperties ()
{
  if (_rep)
	delete _rep;
}

AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFProperties::NextOne (
      ImplAAFProperty ** ppProperty)
{
  if (! _rep) return AAFRESULT_NOT_INITIALIZED;
  return _rep->NextOne (ppProperty);
}



AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFProperties::Next (
      aafUInt32  count,
      ImplAAFProperty ** ppProperties,
      aafUInt32 *  pNumFetched)
{
  if (! _rep) return AAFRESULT_NOT_INITIALIZED;
  return _rep->Next (count, ppProperties, pNumFetched);
}



AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFProperties::Skip (
      aafUInt32  count)
{
  if (! _rep) return AAFRESULT_NOT_INITIALIZED;
  return _rep->Skip (count);
}



AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFProperties::Reset ()
{
  if (! _rep) return AAFRESULT_NOT_INITIALIZED;
  return _rep->Reset ();
}



AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFProperties::Clone (
      ImplEnumAAFProperties ** ppEnum)
{
  ImplEnumAAFProperties * theEnum;

  if (! ppEnum) return AAFRESULT_NULL_PARAM;
  if (! _rep) return AAFRESULT_NOT_INITIALIZED;

  theEnum = (ImplEnumAAFProperties *)CreateImpl(CLSID_EnumAAFProperties);
  if (theEnum == NULL)
	return E_FAIL;
		
  // copy this enumerator
  assert (_rep);
  theEnum->_rep = new ImplEnumerator<ImplAAFProperty*>(*_rep);
  if (! theEnum->_rep)
	{
	  theEnum->ReleaseReference();
	  return AAFRESULT_NOMEMORY;
	}

  assert (ppEnum);
  *ppEnum = theEnum;
  return AAFRESULT_SUCCESS;
}



AAFRESULT
    ImplEnumAAFProperties::Initialize (
      ImplCollection<ImplAAFProperty*> * pProperties)
{
  if (! pProperties)
	return AAFRESULT_NULL_PARAM;

  // make sure it hasn't been init'd before
  assert (! _rep);
  _rep = new ImplEnumerator<ImplAAFProperty*>(pProperties);
  if (! _rep)
	return AAFRESULT_NOMEMORY;
  assert (_rep);

  return AAFRESULT_SUCCESS;
}

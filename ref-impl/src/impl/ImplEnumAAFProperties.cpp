
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

#include <assert.h>
#include <string.h>


ImplEnumAAFProperties::ImplEnumAAFProperties ()
{}


ImplEnumAAFProperties::~ImplEnumAAFProperties ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFProperties::NextOne (
      ImplAAFProperty ** /*ppProperty*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFProperties::Next (
      aafUInt32  /*count*/,
      ImplAAFProperty ** /*ppProperties*/,
      aafUInt32 *  /*pNumFetched*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFProperties::Skip (
      aafUInt32  /*count*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFProperties::Reset ()
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFProperties::Clone (
      ImplEnumAAFProperties ** /*ppEnum*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}




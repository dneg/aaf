/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


#ifndef __ImplAAFPropertyDef_h__
#include "ImplAAFPropertyDef.h"
#endif

#ifndef __ImplAAFPropertyValue_h__
#include "ImplAAFPropertyValue.h"
#endif




#ifndef __ImplAAFProperty_h__
#include "ImplAAFProperty.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFProperty::ImplAAFProperty ()
{}


ImplAAFProperty::~ImplAAFProperty ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFProperty::GetDefinition (
      ImplAAFPropertyDef ** /*ppPropDef*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFProperty::GetValue (
      ImplAAFPropertyValue ** /*ppValue*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}




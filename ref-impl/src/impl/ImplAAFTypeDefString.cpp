/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/





#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFTypeDefString_h__
#include "ImplAAFTypeDefString.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFTypeDefString::ImplAAFTypeDefString ()
{}


ImplAAFTypeDefString::~ImplAAFTypeDefString ()
{}

 AAFRESULT STDMETHODCALLTYPE
   ImplAAFTypeDefString::Initialize (
      aafUID_t *  /*pID*/,
      ImplAAFTypeDef * /*pTypeDef*/,
      ImplAAFPropertyValue * /*pTermValue*/,
      wchar_t *  /*pTypeName*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefString::GetTermValue (
      ImplAAFPropertyValue ** /*ppTermValue*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


OMDEFINE_STORABLE(ImplAAFTypeDefString, AUID_AAFTypeDefString);



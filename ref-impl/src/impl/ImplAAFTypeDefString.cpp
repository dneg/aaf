/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


#ifndef __ImplAAFPropertyValue_h__
#include "ImplAAFPropertyValue.h"
#endif




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
      wchar_t *  /*pTypeName*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefString::GetType (
      ImplAAFTypeDef ** /*ppTypeDef*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefString::GetCount (
      ImplAAFPropertyValue * /*pPropVal*/,
      aafInt64 *  /*pCount*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefString::AppendElements (
      ImplAAFPropertyValue * /*pInPropVal*/,
      ImplAAFPropertyValue ** /*pMemberPropVals*/,
      aafUInt32  /*numElements*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefString::GetElementValues (
      ImplAAFPropertyValue * /*pInPropVal*/,
      aafInt64  /*startElement*/,
      ImplAAFPropertyValue ** /*pOutPropVals*/,
      aafUInt32  /*numElements*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



 AAFRESULT STDMETHODCALLTYPE
   ImplAAFTypeDefString::GetElements (
      ImplAAFPropertyValue * /*pPropVal*/,
      aafInt64  /*startElement*/,
      aafMemPtr_t  /*pData*/,
      aafUInt32  /*numElements*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



  // Override from AAFTypeDef
  AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefString::GetTypeCategory (/*[out]*/ eAAFTypeCategory_t *  /*pTid*/)
  {
    return AAFRESULT_NOT_IMPLEMENTED;
  }


OMDEFINE_STORABLE(ImplAAFTypeDefString, AUID_AAFTypeDefString);



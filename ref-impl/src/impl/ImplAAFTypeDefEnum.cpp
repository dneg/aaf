/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/





#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFTypeDefEnum_h__
#include "ImplAAFTypeDefEnum.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFTypeDefEnum::ImplAAFTypeDefEnum ()
{}


ImplAAFTypeDefEnum::~ImplAAFTypeDefEnum ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefEnum::Initialize (
      aafUID_t *  /*pID*/,
      ImplAAFTypeDef * /*pType*/,
      aafInt32*  /*pElementValues*/,
      aafString_t *  /*pElementNames*/,
      aafUInt32  /*numElems*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefEnum::GetElementType (
      ImplAAFTypeDef ** /*ppTypeDef*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefEnum::CountElements (
      aafUInt32 *  /*pCount*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefEnum::GetElementValues (
      aafUInt32 *  /*pValues*/,
      aafUInt32  /*numElems*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefEnum::GetNameFromValue (
      ImplAAFPropertyValue * /*pValue*/,
      wchar_t *  /*pName*/,
      aafUInt32  /*bufSize*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefEnum::GetNameBufLenFromValue (
      ImplAAFPropertyValue * /*pValue*/,
      aafUInt32 *  /*pLen*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefEnum::GetNameFromInteger (
      aafInt32  /*value*/,
      wchar_t *  /*pName*/,
      aafUInt32  /*bufSize*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefEnum::GetNameBufLenFromInteger (
      aafInt32  /*value*/,
      aafUInt32 *  /*pLen*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefEnum::GetIntegerValue (
      ImplAAFPropertyValue * /*pPropValIn*/,
      aafInt32 *  /*pValueOut*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefEnum::SetIntegerValue (
      ImplAAFPropertyValue * /*pPropValToSet*/,
      aafInt32  /*valueIn*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



  // Override from AAFTypeDef
  AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefEnum::GetTypeCategory (/*[out]*/ eAAFTypeCategory_t *  /*pTid*/)
  {
    return AAFRESULT_NOT_IMPLEMENTED;
  }


OMDEFINE_STORABLE(ImplAAFTypeDefEnum, AUID_AAFTypeDefEnum);



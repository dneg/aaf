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

#ifndef __ImplAAFTypeDefExtEnum_h__
#include "ImplAAFTypeDefExtEnum.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFTypeDefExtEnum::ImplAAFTypeDefExtEnum ()
{}


ImplAAFTypeDefExtEnum::~ImplAAFTypeDefExtEnum ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefExtEnum::Initialize (
      aafUID_t *  /*pID*/,
      wchar_t *  /*pTypeName*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefExtEnum::CountElements (
      aafUInt32 *  /*pCount*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefExtEnum::GetElementValues (
      aafUID_t *  /*pValues*/,
      aafUInt32  /*numElems*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefExtEnum::GetNameFromValue (
      ImplAAFPropertyValue * /*pValue*/,
      wchar_t *  /*pName*/,
      aafUInt32  /*bufSize*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefExtEnum::GetNameBufLenFromValue (
      ImplAAFPropertyValue * /*pValue*/,
      aafUInt32 *  /*pLen*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefExtEnum::GetNameFromAUID (
      aafUID_t *  /*pValue*/,
      wchar_t *  /*pName*/,
      aafUInt32  /*bufSize*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefExtEnum::GetNameBufLenFromAUID (
      aafUID_t *  /*pValue*/,
      aafUInt32 *  /*pLen*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefExtEnum::GetAUIDValue (
      ImplAAFPropertyValue * /*pPropValIn*/,
      aafUID_t *  /*pValueOut*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefExtEnum::SetAUIDValue (
      ImplAAFPropertyValue * /*pPropValToSet*/,
      aafUID_t *  /*pValueIn*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefExtEnum::AppendElement (
      ImplaafUID_t * * /*pValue*/,
      Implwchar_t * * /*pName*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



  // Override from AAFTypeDef
  AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefExtEnum::GetTypeCategory (/*[out]*/ eAAFTypeCategory_t *  /*pTid*/)
  {
    return AAFRESULT_NOT_IMPLEMENTED;
  }


OMDEFINE_STORABLE(ImplAAFTypeDefExtEnum, AUID_AAFTypeDefExtEnum);



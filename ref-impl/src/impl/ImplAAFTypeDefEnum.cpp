/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/




typedef struct _aafTypeDefEnumElementPair_t
{
  [string, size_is(nameSize)] wchar_t * pName;
  IAAFPropertyValue * pValue;
  aafUInt32 nameSize;
} aafTypeDefEnumElementPair_t;












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
      ImplaafTypeDefEnumElementPair_t ** * /*pElements*/,
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
    ImplAAFTypeDefEnum::GetElementName (
      ImplAAFPropertyValue * /*value*/,
      wchar_t *  /*pName*/,
      aafInt32  /*bufSize*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

  //
  // AAFRESULT_SMALLBUF
  //   - bufSize indicates the buffer is too small to hold the string.
  //
  // AAFRESULT_BAD_PARAM
  //   - the given value is not associated with an element of
  //     this type.)


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefEnum::GetElementNameBufLen (
      ImplAAFPropertyValue * /*pValue*/,
      aafInt32 *  /*pLen*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

  //
  // AAFRESULT_BAD_PARAM
  //   - the given value is not associated with an element of this
  //     type.)


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefEnum::GetValueAsValue (
      ImplAAFPropertyValue * /*pPropValIn*/,
      ImplAAFPropertyValue ** /*ppPropValOut*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefEnum::SetValueFromValue (
      ImplAAFPropertyValue * /*pPropValToSet*/,
      ImplAAFPropertyValue * /*pPropValIn*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

  // AAFRESULT_BAD_TYPE
  //   - pPropValIn's type doesn't match GetElementType\(\))

OMDEFINE_STORABLE(ImplAAFTypeDefEnum, AUID_AAFTypeDefEnum);



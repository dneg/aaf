/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/





#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFTypeDefVariableArray_h__
#include "ImplAAFTypeDefVariableArray.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFTypeDefVariableArray::ImplAAFTypeDefVariableArray ()
{}


ImplAAFTypeDefVariableArray::~ImplAAFTypeDefVariableArray ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefVariableArray::Initialize (
      aafUID_t *  /*pID*/,
      ImplAAFTypeDef * /*pTypeDef*/,
      wchar_t *  /*pTypeName*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefVariableArray::GetType (
      ImplAAFTypeDef ** /*ppTypeDef*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefVariableArray::GetCount (
      ImplAAFPropertyValue * /*pPropVal*/,
      aafUInt32 *  /*pCount*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefVariableArray::AppendElement (
      ImplAAFPropertyValue * /*pInPropVal*/,
      ImplAAFPropertyValue * /*pMemberPropVal*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefVariableArray::CreateValueFromValues (
      ImplAAFPropertyValue ** /*ppElementValues*/,
      aafUInt32  /*numElements*/,
      ImplAAFPropertyValue ** /*ppPropVal*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefVariableArray::CreateValueFromCArray (
      aafMemPtr_t  /*pInitData*/,
      aafUInt32  /*initDataSize*/,
      ImplAAFPropertyValue ** /*ppPropVal*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefVariableArray::GetElementValue (
      ImplAAFPropertyValue * /*pInPropVal*/,
      aafUInt32  /*index*/,
      ImplAAFPropertyValue ** /*ppOutPropVal*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefVariableArray::GetCArray (
      ImplAAFPropertyValue * /*pPropVal*/,
      aafMemPtr_t  /*pData*/,
      aafUInt32  /*dataSize*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefVariableArray::SetElementValue (
      ImplAAFPropertyValue * /*pPropVal*/,
      aafUInt32  /*index*/,
      ImplAAFPropertyValue * /*pMemberPropVal*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefVariableArray::SetCArray (
      ImplAAFPropertyValue * /*pPropVal*/,
      aafMemPtr_t  /*pData*/,
      aafUInt32  /*dataSize*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


OMDEFINE_STORABLE(ImplAAFTypeDefVariableArray, AUID_AAFTypeDefVariableArray);



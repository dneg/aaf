/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/





#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFTypeDefStream_h__
#include "ImplAAFTypeDefStream.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFTypeDefStream::ImplAAFTypeDefStream ()
{}


ImplAAFTypeDefStream::~ImplAAFTypeDefStream ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::Initialize (
      aafUID_t *  /*pID*/,
      ImplAAFTypeDef * /*pTypeDef*/,
      wchar_t *  /*pTypeName*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::GetType (
      ImplAAFTypeDef ** /*ppTypeDef*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::GetCount (
      ImplAAFPropertyValue * /*pPropVal*/,
      aafInt64 *  /*pCount*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::AppendElements (
      ImplAAFPropertyValue * /*pInPropVal*/,
      ImplAAFPropertyValue ** /*ppMemberPropVals*/,
      aafUInt32  /*numElements*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::CreateEmpty (
      ImplAAFPropertyValue ** /*ppPropVal*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefStream::GetElementValues (
      ImplAAFPropertyValue * /*pInPropVal*/,
      aafInt64  /*startElement*/,
      ImplAAFPropertyValue ** /*pOutPropVals*/,
      aafUInt32  /*numElements*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



 AAFRESULT STDMETHODCALLTYPE
   ImplAAFTypeDefStream::GetElements (
      ImplAAFPropertyValue * /*pPropVal*/,
      aafInt64  /*startElement*/,
      aafMemPtr_t  /*pData*/,
      aafUInt32  /*numElements*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


OMDEFINE_STORABLE(ImplAAFTypeDefStream, AUID_AAFTypeDefStream);



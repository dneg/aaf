/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/





#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFTypeDefInt_h__
#include "ImplAAFTypeDefInt.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFTypeDefInt::ImplAAFTypeDefInt ()
{}


ImplAAFTypeDefInt::~ImplAAFTypeDefInt ()
{}

 AAFRESULT STDMETHODCALLTYPE
   ImplAAFTypeDefInt::Initialize (
      aafUID_t *  /*pID*/,
      aafUInt32  /*intSize*/,
      aafBool  /*isSigned*/,
      wchar_t *  /*pTypeName*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefInt::CreateValue (
      aafMemPtr_t  /*pVal*/,
      aafUInt32  /*valSize*/,
      ImplAAFPropertyValue ** /*ppPropVal*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefInt::GetInteger (
      ImplAAFPropertyValue * /*pPropVal*/,
      aafMemPtr_t  /*pBuf*/,
      aafUInt32  /*bufSize*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefInt::SetInteger (
      ImplAAFPropertyValue * /*pPropVal*/,
      aafUInt8 *  /*pBuf*/,
      aafUInt32  /*bufSize*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefInt::GetSize (
      aafUInt32 *  /*pSize*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefInt::IsSigned (
      aafBool *  /*pSigned*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


OMDEFINE_STORABLE(ImplAAFTypeDefInt, AUID_AAFTypeDefInt);



/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/





#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFTypeDefRename_h__
#include "ImplAAFTypeDefRename.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFTypeDefRename::ImplAAFTypeDefRename ()
{}


ImplAAFTypeDefRename::~ImplAAFTypeDefRename ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefRename::Initialize (
      aafUID_t *  /*pID*/,
      ImplAAFTypeDef * /*pBaseType*/,
      wchar_t *  /*pTypeName*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefRename::GetBaseType (
      ImplAAFTypeDef ** /*ppBaseType*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefRename::GetName (
      wchar_t *  /*pName*/,
      aafInt32  /*bufSize*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefRename::GetNameBufLen (
      aafInt32 *  /*pLen*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefRename::GetValue (
      ImplAAFPropertyValue * /*pInPropVal*/,
      ImplAAFPropertyValue ** /*ppOutPropVal*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


OMDEFINE_STORABLE(ImplAAFTypeDefRename, AUID_AAFTypeDefRename);



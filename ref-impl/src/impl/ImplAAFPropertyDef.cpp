/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


#ifndef __ImplEnumAAFPropertyValues_h__
#include "ImplEnumAAFPropertyValues.h"
#endif

#ifndef __ImplAAFPropertyValue_h__
#include "ImplAAFPropertyValue.h"
#endif

#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif

 


#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFPropertyDef_h__
#include "ImplAAFPropertyDef.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFPropertyDef::ImplAAFPropertyDef ()
{}


ImplAAFPropertyDef::~ImplAAFPropertyDef ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyDef::GetTypeDef (
      ImplAAFTypeDef ** /*ppTypeDef*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyDef::GetName (
      wchar_t *  /*pName*/,
      aafInt32  /*bufSize*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyDef::GetNameBufLen (
      aafInt32 *  /*pLen*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyDef::GetMinVersion (
      aafVersionType_t *  /*pMinVersion*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyDef::SetMinVersion (
      aafVersionType_t *  /*pMinVersion*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyDef::GetMaxVersion (
      aafVersionType_t *  /*pMaxVersion*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyDef::SetMaxVersion (
      aafVersionType_t *  /*pMaxVersion*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyDef::GetIsSearchable (
      aafBool *  /*pIsSearchable*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyDef::SetIsSearchable (
      aafBool  /*IsSearchable*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyDef::GetDescription (
      wchar_t *  /*pStrDescription*/,
      aafInt32  /*bufSize*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyDef::GetDescriptionBufLen (
      aafInt32 *  /*pLen*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyDef::SetDescription (
      wchar_t *  /*pStrDescription*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyDef::GetDefaultValue (
      ImplAAFPropertyValue ** /*ppDataValue*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyDef::SetDefaultValue (
      ImplAAFPropertyValue * /*pDataValue*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyDef::GetRefValues (
      ImplEnumAAFPropertyValues ** /*ppEnum*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


OMDEFINE_STORABLE(ImplAAFPropertyDef, AUID_AAFPropertyDef);



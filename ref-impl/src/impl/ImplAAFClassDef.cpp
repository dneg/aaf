/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


#ifndef __ImplEnumAAFPropertyDefs_h__
#include "ImplEnumAAFPropertyDefs.h"
#endif

#ifndef __ImplAAFPropertyDef_h__
#include "ImplAAFPropertyDef.h"
#endif

#ifndef __ImplAAFDefObject_h__
#include "ImplAAFDefObject.h"
#endif

#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif




#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFClassDef_h__
#include "ImplAAFClassDef.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFClassDef::ImplAAFClassDef ()
{}


ImplAAFClassDef::~ImplAAFClassDef ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFClassDef::Initialize (
      ImplAAFClassDef * pParentClass)
{
  if (!pParentClass) return AAFRESULT_NULL_PARAM;

  // BobT hack: need real implementation

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFClassDef::GetPropertyDefs (
      ImplEnumAAFPropertyDefs ** /*ppEnum*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFClassDef::CountPropertyDefs (
      aafUInt32 *  /*pCount*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFClassDef::AppendNewPropertyDef (
      aafUID_t *  /*pID*/,
      wchar_t *  /*pName*/,
      ImplAAFTypeDef * /*pTypeDef*/,
      aafBool  /*isOptional*/,
      ImplAAFPropertyDef ** /*ppPropDef*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFClassDef::LookupPropertyDef (
      aafUID_t *  /*pPropID*/,
      ImplAAFPropertyDef ** /*ppPropDef*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFClassDef::GetName (
      wchar_t *  /*pName*/,
      aafUInt32  /*bufSize*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFClassDef::GetNameBufLen (
      aafUInt32 *  /*pBufSize*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFClassDef::GetParent (
      ImplAAFClassDef ** /*pClassDef*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


OMDEFINE_STORABLE(ImplAAFClassDef, AUID_AAFClassDef);



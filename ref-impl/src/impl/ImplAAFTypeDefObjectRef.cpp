/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/





#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFTypeDefObjectRef_h__
#include "ImplAAFTypeDefObjectRef.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFTypeDefObjectRef::ImplAAFTypeDefObjectRef ()
{}


ImplAAFTypeDefObjectRef::~ImplAAFTypeDefObjectRef ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefObjectRef::Initialize (
      aafUID_t *  /*pID*/,
      ImplAAFClassDef * /*pObjType*/,
      wchar_t *  /*pTypeName*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefObjectRef::CreateValue (
      ImplAAFObject * /*pObj*/,
      ImplAAFPropertyValue ** /*ppPropVal*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefObjectRef::GetObjectType (
      ImplAAFClassDef ** /*ppObjType*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefObjectRef::GetObject (
      ImplAAFPropertyValue * /*pPropVal*/,
      ImplAAFObject ** /*ppObject*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefObjectRef::SetObject (
      ImplAAFPropertyValue * /*pPropVal*/,
      ImplAAFObject * /*ppObject*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


OMDEFINE_STORABLE(ImplAAFTypeDefObjectRef, AUID_AAFTypeDefObjectRef);



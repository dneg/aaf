/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/



#ifndef __ImplAAFClassDef_h__
#include "ImplAAFClassDef.h"
#endif

#ifndef __ImplEnumAAFCodecFlavours_h__
#include "ImplEnumAAFCodecFlavours.h"
#endif

#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFCodecDef_h__
#include "ImplAAFCodecDef.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFCodecDef::ImplAAFCodecDef ()
{}


ImplAAFCodecDef::~ImplAAFCodecDef ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFCodecDef::IsEssenceKindSupported (
      aafUID_t *  /*pEssenceKind*/,
      aafBool*  /*pIsSupported*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

 
 AAFRESULT STDMETHODCALLTYPE
    ImplAAFCodecDef::AppendEssenceKind (
      aafUID_t *  /*pEssenceKind*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}




AAFRESULT STDMETHODCALLTYPE
    ImplAAFCodecDef::AreThereFlavours (
      aafBool *  /*pResult*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFCodecDef::GetFileDescriptorClass (
      ImplAAFClassDef ** /*ppClass*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFCodecDef::SetFileDescriptorClass (
      ImplAAFClassDef * /*pClass*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFCodecDef::EnumCodecFlavours (
      ImplEnumAAFCodecFlavours ** /*ppEnum*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

  

OMDEFINE_STORABLE(ImplAAFCodecDef, AUID_AAFCodecDef);



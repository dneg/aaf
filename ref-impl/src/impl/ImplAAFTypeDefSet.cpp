/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/


#ifndef __ImplAAFPropertyValue_h__
#include "ImplAAFPropertyValue.h"
#endif

#ifndef __ImplEnumAAFPropertyValues_h__
#include "ImplEnumAAFPropertyValues.h"
#endif




#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFTypeDefSet_h__
#include "ImplAAFTypeDefSet.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFTypeDefSet::ImplAAFTypeDefSet ()
{}


ImplAAFTypeDefSet::~ImplAAFTypeDefSet ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefSet::Initialize (
      const aafUID_t *  /*pID*/,
      ImplAAFTypeDef * /*pTypeDef*/,
      aafCharacter *  /*pTypeName*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefSet::GetType (
      ImplAAFTypeDef ** /*ppTypeDef*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefSet::GetCount (
      ImplAAFPropertyValue * /*pPropVal*/,
      aafUInt32 *  /*pCount*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefSet::AddElement (
      ImplAAFPropertyValue * /*pInPropVal*/,
      ImplAAFPropertyValue * /*pMemberPropVal*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefSet::CreateValueFromValues (
      ImplAAFPropertyValue ** /*pElementValues*/,
      aafUInt32  /*numElements*/,
      ImplAAFPropertyValue ** /*ppPropVal*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefSet::CreateValueFromCArray (
      aafMemPtr_t  /*pInitData*/,
      aafUInt32  /*initDataSize*/,
      ImplAAFPropertyValue ** /*ppPropVal*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefSet::GetElements (
      ImplAAFPropertyValue * /*pInPropVal*/,
      ImplEnumAAFPropertyValues ** /*ppEnum*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


aafBool ImplAAFTypeDefSet::IsFixedSize (void) const
{
  assert (0);
  return AAFFalse; // Not reached!
}


size_t ImplAAFTypeDefSet::PropValSize (void) const
{
  assert (0);
  return 0; // Not reached!
}


aafBool ImplAAFTypeDefSet::IsRegistered (void) const
{
  assert (0);
  return AAFFalse; // Not reached!
}


size_t ImplAAFTypeDefSet::NativeSize (void) const
{
  assert (0);
  return 0; // Not reached!
}



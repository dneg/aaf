/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


#ifndef __ImplAAFPropertyValue_h__
#include "ImplAAFPropertyValue.h"
#endif

#ifndef __ImplAAFClassDef_h__
#include "ImplAAFClassDef.h"
#endif




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
      aafUID_t *  pID,
      ImplAAFClassDef * pObjType,
      wchar_t *  pTypeName)
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


void ImplAAFTypeDefObjectRef::reorder(OMByte* /*externalBytes*/,
									  size_t /*externalBytesSize*/) const
{
  // nothing to do for obj refs
}


size_t ImplAAFTypeDefObjectRef::externalSize(OMByte* /*internalBytes*/,
											 size_t /*internalBytesSize*/) const
{
  return PropValSize ();
}


void ImplAAFTypeDefObjectRef::externalize(OMByte* internalBytes,
										  size_t internalBytesSize,
										  OMByte* externalBytes,
										  size_t externalBytesSize,
										  OMByteOrder /*byteOrder*/) const
{
  assert (externalBytesSize >= internalBytesSize);
  copy (internalBytes, externalBytes, internalBytesSize);
}


size_t ImplAAFTypeDefObjectRef::internalSize(OMByte* /*externalBytes*/,
											 size_t /*externalBytesSize*/) const
{
  return NativeSize ();
}


void ImplAAFTypeDefObjectRef::internalize(OMByte* externalBytes,
										  size_t externalBytesSize,
										  OMByte* internalBytes,
										  size_t internalBytesSize,
										  OMByteOrder /*byteOrder*/) const
{
  assert (internalBytesSize >= externalBytesSize);
  copy (externalBytes, internalBytes, externalBytesSize);
}

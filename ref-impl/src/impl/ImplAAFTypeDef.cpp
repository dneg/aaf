/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFTypeDef::ImplAAFTypeDef ()
{}


ImplAAFTypeDef::~ImplAAFTypeDef ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDef::GetTypeCategory (
      eAAFTypeCategory_t *  /*pTid*/)
{
  // Should be implemented in derived class.
  return AAFRESULT_INTERNAL_ERROR;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDef::RawAccessType (
      ImplAAFTypeDef ** /*ppRawTypeDef*/)
{
  // Should be implemented in derived class.
  return AAFRESULT_INTERNAL_ERROR;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDef::GetMinVersion (
      aafVersionType_t *  /*pMinVersion*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDef::SetMinVersion (
      aafVersionType_t *  /*pMinVersion*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDef::GetMaxVersion (
      aafVersionType_t *  /*pMaxVersion*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDef::SetMaxVersion (
      aafVersionType_t  /*MaxVersion*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDef::GetSwapNeeded (
      aafSwapNeeded_t *  /*pSwapNeeded*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDef::SetSwapNeeded (
      aafSwapNeeded_t  /*SwapNeeded*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



void ImplAAFTypeDef::reorder(OMByte* /*bytes*/,
							 size_t /*bytesSize*/) const
{
  // Should be implemented in derived class.
  assert (0);
}


size_t ImplAAFTypeDef::externalSize(OMByte* /*internalBytes*/,
									size_t /*internalBytesSize*/) const
{
  // Should be implemented in derived class.
  assert (0);
  return 0; // Not reached!
}


void ImplAAFTypeDef::externalize(OMByte* /*internalBytes*/,
								 size_t /*internalBytesSize*/,
								 OMByte* /*externalBytes*/,
								 size_t /*externalBytesSize*/,
								 OMByteOrder /*byteOrder*/) const
{
  // Should be implemented in derived class.
  assert (0);
}


size_t ImplAAFTypeDef::internalSize(OMByte* /*externalBytes*/,
									size_t /*externalSize*/) const
{
  // Should be implemented in derived class.
  assert (0);
  return 0; // Not reached!
}


void ImplAAFTypeDef::internalize(OMByte* /*externalBytes*/,
								 size_t /*externalBytesSize*/,
								 OMByte* /*internalBytes*/,
								 size_t /*internalBytesSize*/,
								 OMByteOrder /*byteOrder*/) const
{
  // Should be implemented in derived class.
  assert (0);
}


aafBool ImplAAFTypeDef::IsFixedSize (void) const
{
  // Should be implemented in derived class.
  assert (0);
  return AAFFalse; // not reached!
}


size_t ImplAAFTypeDef::PropValSize (void) const
{
  // Should be implemented in derived class.
  assert (0);
  return 0; // not reached!
}


aafBool ImplAAFTypeDef::IsRegistered (void) const
{
  // Should be implemented in derived class.
  assert (0);
  return AAFFalse; // not reached!
}


size_t ImplAAFTypeDef::NativeSize (void) const
{
  // Should be implemented in derived class.
  assert (0);
  return 0; // not reached!
}


OMProperty * ImplAAFTypeDef::pvtCreateOMProperty
  (OMPropertyId pid,
   const aafCharacter * name) const
{
  assert (name);
  size_t wNameLen = wcslen (name);

  // Convert the prop name
  char * nameBuf = new char [wNameLen];
  assert (nameBuf);
  wcstombs (nameBuf, name, wNameLen);

  OMProperty * result = pvtCreateOMPropertyMBS (pid, nameBuf);
  delete[] nameBuf;
  return result;
}


OMProperty * ImplAAFTypeDef::pvtCreateOMPropertyMBS
  (OMPropertyId /*pid*/,
   const char * /*name*/) const
{
  // Should be implemented in derived class.
  assert (0);
  return 0; // not reached!
}


OMDEFINE_STORABLE(ImplAAFTypeDef, AUID_AAFTypeDef);

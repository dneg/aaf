/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


#ifndef __ImplEnumAAFReferenceValues_h__
#include "ImplEnumAAFReferenceValues.h"
#endif




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
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDef::RawAccessType (
      ImplAAFTypeDef ** /*ppRawTypeDef*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
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



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDef::GetRefValues (
      ImplEnumAAFReferenceValues ** /*ppEnum*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



void ImplAAFTypeDef::reorder(OMByte* /*bytes*/,
							 size_t /*bytesSize*/) const
{
  assert (0);
}


size_t ImplAAFTypeDef::externalSize(OMByte* /*internalBytes*/,
									size_t /*internalBytesSize*/) const
{
  assert (0);
  return 0; // Not reached!
}


void ImplAAFTypeDef::externalize(OMByte* /*internalBytes*/,
								 size_t /*internalBytesSize*/,
								 OMByte* /*externalBytes*/,
								 size_t /*externalBytesSize*/,
								 OMByteOrder /*byteOrder*/) const
{
  assert (0);
}


size_t ImplAAFTypeDef::internalSize(OMByte* /*externalBytes*/,
									size_t /*externalSize*/) const
{
  assert (0);
  return 0; // Not reached!
}


void ImplAAFTypeDef::internalize(OMByte* /*externalBytes*/,
								 size_t /*externalBytesSize*/,
								 OMByte* /*internalBytes*/,
								 size_t /*internalBytesSize*/,
								 OMByteOrder /*byteOrder*/) const
{
  assert (0);
}


aafBool ImplAAFTypeDef::IsFixedSize (void) const
{
  assert (0);
  return AAFFalse; // not reached!
}


size_t ImplAAFTypeDef::PropValSize (void) const
{
  assert (0);
  return 0; // not reached!
}


aafBool ImplAAFTypeDef::IsRegistered (void) const
{
  assert (0);
  return AAFFalse; // not reached!
}


size_t ImplAAFTypeDef::NativeSize (void) const
{
  assert (0);
  return 0; // not reached!
}


OMDEFINE_STORABLE(ImplAAFTypeDef, AUID_AAFTypeDef);

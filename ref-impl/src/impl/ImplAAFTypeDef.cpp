/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 *  prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/


#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif

#include <assert.h>
#include <string.h>

#if defined(macintosh) || defined(_MAC)
#include <wstring.h>
#endif

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



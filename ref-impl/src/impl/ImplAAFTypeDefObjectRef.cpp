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
      const aafUID_t *  pID,
      ImplAAFClassDef * pObjType,
      wchar_t *  pTypeName)
{
  AAFRESULT hr;
  aafUID_t id;

  if (! pObjType)  return AAFRESULT_NULL_PARAM;
  hr = pObjType->GetAUID(&id);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;

  return pvtInitialize (pID, &id, pTypeName);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefObjectRef::pvtInitialize (
      const aafUID_t *  pID,
      const aafUID_t * pRefdObjID,
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

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

#include "OMAssertions.h"

#include <string.h>


ImplAAFTypeDefObjectRef::ImplAAFTypeDefObjectRef ()
{}


ImplAAFTypeDefObjectRef::~ImplAAFTypeDefObjectRef ()
{}




AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefObjectRef::CreateValue (
      ImplAAFRoot * /*pObj*/,
      ImplAAFPropertyValue ** /*ppPropVal*/)
{
  // This is a virtual function should be implemented in a derived class.
  return AAFRESULT_INTERNAL_ERROR;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefObjectRef::GetObjectType (
      ImplAAFClassDef ** /*ppObjType*/)
{
  // This is a virtual function should be implemented in a derived class.
  return AAFRESULT_INTERNAL_ERROR;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefObjectRef::GetObject (
      ImplAAFPropertyValue * /*pPropVal*/,
      ImplAAFRoot ** /*ppObject*/)
{
  // This is a virtual function should be implemented in a derived class.
  return AAFRESULT_INTERNAL_ERROR;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefObjectRef::SetObject (
      ImplAAFPropertyValue * /*pPropVal*/,
      ImplAAFRoot * /*ppObject*/)
{
  // This is a virtual function should be implemented in a derived class.
  return AAFRESULT_INTERNAL_ERROR;
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
										  size_t ANAME(externalBytesSize),
										  OMByteOrder NNAME(byteOrder)) const
{
  TRACE("ImplAAFTypeDefObjectRef::externalize");
  PRECONDITION("Valid internal bytes", internalBytes != 0);
  PRECONDITION("Valid internal byte size", internalBytesSize > 0);
  PRECONDITION("Valid external bytes", externalBytes != 0);
  PRECONDITION("Valid external byte size", externalBytesSize > 0);
  PRECONDITION("Internal and external sizes are equal", externalBytesSize == internalBytesSize);

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
										  size_t ANAME(internalBytesSize),
										  OMByteOrder NNAME(byteOrder)) const
{
  TRACE("ImplAAFTypeDefObjectRef::internalize");
  PRECONDITION("Valid external bytes", externalBytes != 0);
  PRECONDITION("Valid external byte size", externalBytesSize > 0);
  PRECONDITION("Valid internal bytes", internalBytes != 0);
  PRECONDITION("Valid internal byte size", internalBytesSize > 0);
  PRECONDITION("Internal and external sizes are equal", internalBytesSize == externalBytesSize);
		           
  copy (externalBytes, internalBytes, externalBytesSize);
}


bool ImplAAFTypeDefObjectRef::IsAggregatable () const
{ return false; }

bool ImplAAFTypeDefObjectRef::IsStreamable () const
{ return false; }

bool ImplAAFTypeDefObjectRef::IsFixedArrayable () const
{ return false; }

bool ImplAAFTypeDefObjectRef::IsVariableArrayable () const
{ return true; }

bool ImplAAFTypeDefObjectRef::IsStringable () const
{ return false; }




// Override callbacks from OMStorable
void ImplAAFTypeDefObjectRef::onSave(void* clientContext) const
{
  ImplAAFTypeDef::onSave(clientContext);
}

void ImplAAFTypeDefObjectRef::onRestore(void* clientContext) const
{
  ImplAAFTypeDef::onRestore(clientContext);
}

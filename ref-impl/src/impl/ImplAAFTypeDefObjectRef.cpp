//=---------------------------------------------------------------------=
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
// 
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
// 
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


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


size_t ImplAAFTypeDefObjectRef::externalSize(const OMByte* /*internalBytes*/,
											 size_t /*internalBytesSize*/) const
{
  return PropValSize ();
}


void ImplAAFTypeDefObjectRef::externalize(const OMByte* internalBytes,
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


size_t ImplAAFTypeDefObjectRef::internalSize(const OMByte* /*externalBytes*/,
											 size_t /*externalBytesSize*/) const
{
  return NativeSize ();
}


void ImplAAFTypeDefObjectRef::internalize(const OMByte* externalBytes,
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

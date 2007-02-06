//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2006, Licensor of the
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
#include "OMAssertions.h"


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

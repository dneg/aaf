//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=





#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif

#ifndef __ImplAAFPropertyDef_h__
#include "ImplAAFPropertyDef.h"
#endif

#ifndef __ImplEnumAAFPropertyDefs_h__
#include "ImplEnumAAFPropertyDefs.h"
#endif





#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFKLVDataDefinition_h__
#include "ImplAAFKLVDataDefinition.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFKLVDataDefinition::ImplAAFKLVDataDefinition ()
{}


ImplAAFKLVDataDefinition::~ImplAAFKLVDataDefinition ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFKLVDataDefinition::AddParentProperty (
      ImplAAFPropertyDef * /*pParentProperty*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFKLVDataDefinition::GetParentProperties (
      ImplEnumAAFPropertyDefs ** /*ppEnum*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFKLVDataDefinition::CountParentProperties (
      aafUInt32*  /*pNumProperties*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFKLVDataDefinition::RemoveParentProperty (
      ImplAAFPropertyDef * /*pParentProperty*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFKLVDataDefinition::GetKLVDataType (
      ImplAAFTypeDef ** /*ppTypeDef*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFKLVDataDefinition::SetKLVDataType (
      ImplAAFTypeDef * /*pTypeDef*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}





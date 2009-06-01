//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef __ImplAAFTaggedValueDefinition_h__
#include "ImplAAFTaggedValueDefinition.h"
#endif

#ifndef __ImplAAFPropertyDef_h__
#include "ImplAAFPropertyDef.h"
#endif

#ifndef __ImplEnumAAFPropertyDefs_h__
#include "ImplEnumAAFPropertyDefs.h"
#endif

#ifndef __AAFPRopertyIDs_h__
#include "AAFPropertyIDs.h"
#endif

#include "AAFStoredObjectIDs.h"

#include <string.h>

extern "C" const aafClassID_t CLSID_EnumAAFPropertyDefs;

ImplAAFTaggedValueDefinition::ImplAAFTaggedValueDefinition ()
{}

ImplAAFTaggedValueDefinition::~ImplAAFTaggedValueDefinition ()
{}
  
AAFRESULT STDMETHODCALLTYPE
    ImplAAFTaggedValueDefinition::Initialize (
      const aafUID_t & id,
      const aafWChar * pName,
      const aafWChar * pDesc)
{
	if (pName == NULL || pDesc == NULL)
	{
	  return AAFRESULT_NULL_PARAM;
	}
	else
	{
	  return pvtInitialize(id, pName, pDesc);
	}
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTaggedValueDefinition::AddParentProperty (
     ImplAAFPropertyDef* /*pParentProperty*/ )
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTaggedValueDefinition::GetParentProperties (
       ImplEnumAAFPropertyDefs** /*ppEnum*/ )
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTaggedValueDefinition::CountParentProperties (
							 aafUInt32* /*pNumProperties*/ )
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTaggedValueDefinition::RemoveParentProperty (
      ImplAAFPropertyDef * /*pParentProperty*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


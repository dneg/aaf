//=---------------------------------------------------------------------=
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

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"
#include "ImplAAFObjectCreation.h"

#ifndef __ImplAAFDataDef_h__
#include "ImplAAFDataDef.h"
#endif


 

#ifndef __ImplAAFParameter_h__
#include "ImplAAFParameter.h"
#endif

#include <string.h>
#include "AAFResult.h"
#include "aafErr.h"
#include "ImplAAFDictionary.h"
#include "ImplAAFTypeDef.h"
#include "ImplAAFParameterDef.h"
#include "ImplAAFTypeDef.h"

ImplAAFParameter::ImplAAFParameter ()
: _parmDef(			PID_Parameter_Definition,	L"Definition")
{
	_persistentProperties.put(_parmDef.address());
}


ImplAAFParameter::~ImplAAFParameter ()
{
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFParameter::SetParameterDefinition (
      ImplAAFParameterDef *pParmDef)
{
	aafUID_t			newUID;
	ImplAAFDictionary	*dict = NULL;

	if(pParmDef == NULL)
		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		CHECK(pParmDef->GetAUID(&newUID));
		CHECK(GetDictionary(&dict));
// This is a weak reference, not yet counted
//		if(dict->LookupParameterDef(&newUID, &def) == AAFRESULT_SUCCESS)
//			def->ReleaseReference();

		_parmDef = newUID;
//		pParmDef->AcquireReference();
		dict->ReleaseReference();
		dict = NULL;
	}
	XEXCEPT
	{
		if(dict)
		  dict->ReleaseReference();
		dict = 0;
	}
	XEND

	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFParameter::GetParameterDefinition (
      ImplAAFParameterDef **ppParmDef)
{
	ImplAAFDictionary	*dict = NULL;

	if(ppParmDef == NULL)
		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
	  CHECK(GetDictionary(&dict));
		CHECK(dict->LookupParameterDef(_parmDef, ppParmDef));
//		(*ppParmDef)->AcquireReference();
		dict->ReleaseReference();
		dict = NULL;
	}
	XEXCEPT
	{
		if(dict)
		  dict->ReleaseReference();
		dict = 0;
	}
	XEND;

	return AAFRESULT_SUCCESS;
}

	



AAFRESULT STDMETHODCALLTYPE
    ImplAAFParameter::GetTypeDefinition (
      ImplAAFTypeDef **ppTypeDef)
{
	ImplAAFParameterDef	*pParameterDef = NULL;

	if(ppTypeDef == NULL)
		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
	  CHECK(GetParameterDefinition(&pParameterDef));
		CHECK(pParameterDef->GetTypeDefinition (ppTypeDef));
		pParameterDef->ReleaseReference();
		pParameterDef = NULL;
	}
	XEXCEPT
	{
		if(pParameterDef)
		  pParameterDef->ReleaseReference();
		pParameterDef = 0;
	}
	XEND;

	return AAFRESULT_SUCCESS;
}

const OMUniqueObjectIdentification&
  ImplAAFParameter::identification(void) const
{
  return *reinterpret_cast<const OMUniqueObjectIdentification*>(&_parmDef.reference());
}







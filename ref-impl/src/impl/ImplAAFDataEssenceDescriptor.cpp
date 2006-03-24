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
// The Original Code of this file is Copyright 1998-2005, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=









#include "AAFPropertyIDs.h"
#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFDataEssenceDescriptor_h__
#include "ImplAAFDataEssenceDescriptor.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFDataEssenceDescriptor::ImplAAFDataEssenceDescriptor ()
:	_dataEssenceCoding(PID_DataEssenceDescriptor_DataEssenceCoding,	L"DataEssenceCoding")
{
	_persistentProperties.put(_dataEssenceCoding.address());
}


ImplAAFDataEssenceDescriptor::~ImplAAFDataEssenceDescriptor ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDataEssenceDescriptor::SetDataEssenceCoding (
      const aafUID_t & dataEssenceCoding)
{
  	_dataEssenceCoding = dataEssenceCoding;

	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDataEssenceDescriptor::GetDataEssenceCoding (
      aafUID_t *  pDataEssenceCoding)
{
  	if (pDataEssenceCoding == NULL)
	  return AAFRESULT_NULL_PARAM;

	if (!_dataEssenceCoding.isPresent())
	  return AAFRESULT_PROP_NOT_PRESENT;

	*pDataEssenceCoding = _dataEssenceCoding;

	return AAFRESULT_SUCCESS;
}




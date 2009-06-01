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






#ifndef __ImplAAFWAVEDescriptor_h__
#include "ImplAAFWAVEDescriptor.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include <string.h>


ImplAAFWAVEDescriptor::ImplAAFWAVEDescriptor ()
:   _summary(PID_WAVEDescriptor_Summary,	L"Summary")
{
	_persistentProperties.put(_summary.address());
}


ImplAAFWAVEDescriptor::~ImplAAFWAVEDescriptor ()
{
	// TODO: free summary
}

AAFRESULT STDMETHODCALLTYPE
ImplAAFWAVEDescriptor::Initialize ()
{
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFWAVEDescriptor::GetSummary (aafUInt32 size, aafDataValue_t pSummary)
{
	if(pSummary == NULL)
		return(AAFRESULT_NULL_PARAM);

	if (_summary.size() > size)
	  return AAFRESULT_SMALLBUF;

	_summary.copyToBuffer(pSummary, size);

	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFWAVEDescriptor::GetSummaryBufferSize (aafUInt32* pSize)
{
	if(pSize == NULL)
		return(AAFRESULT_NULL_PARAM);

	*pSize = _summary.size();

	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFWAVEDescriptor::SetSummary (aafUInt32 size, aafDataValue_t  pSummary)
{
	if(pSummary == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(size > OMPROPERTYSIZE_MAX)
		return(AAFRESULT_BAD_SIZE);

	_summary.setValue(pSummary, static_cast<OMPropertySize>(size));

	return(AAFRESULT_SUCCESS); 
}

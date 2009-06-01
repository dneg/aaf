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

#ifndef __ImplAAFEdgecode_h__
#include "ImplAAFEdgecode.h"
#endif

#include "ImplAAFBuiltinDefs.h"
#include "ImplAAFDictionary.h"

#include <string.h>

#include "AAFTypes.h"
#include "AAFResult.h"
#include "aafErr.h"
#include "AAFUtils.h"
#include "AAFDataDefs.h"

ImplAAFEdgecode::ImplAAFEdgecode ():
_start( PID_EdgeCode_Start, L"Start"),
_filmType( PID_EdgeCode_FilmKind, L"FilmKind"),
_edgeType( PID_EdgeCode_CodeFormat, L"CodeFormat"),
_header( PID_EdgeCode_Header, L"Header")
{
	_persistentProperties.put(_start.address());
	_persistentProperties.put(_filmType.address());
	_persistentProperties.put(_edgeType.address());
	_persistentProperties.put(_header.address());
}


ImplAAFEdgecode::~ImplAAFEdgecode ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFEdgecode::Initialize(aafLength_t		length,
								aafEdgecode_t	edgecode)
{
	HRESULT		rc = AAFRESULT_SUCCESS;
	
	// Even though the AAF specification does not limit the size of the
	// edge code header this reference implementation uses fixed size
	// header which contains only 8 bytes. The following check,
	// unneccesary in the current implementation provided for consistancy
	// with the specification.
	if (sizeof(edgecode.header) > OMPROPERTYSIZE_MAX)
		return(AAFRESULT_BAD_SIZE);

	XPROTECT()
	{
		ImplAAFDictionarySP pDict;
		CHECK(GetDictionary(&pDict));
		CHECK(SetNewProps(length,
						  pDict->GetBuiltinDefs()->ddEdgecode()));
		_start = edgecode.startFrame;
		if (edgecode.filmKind < kAAFFtNull || edgecode.filmKind > kAAFFt65MM)
			return AAFRESULT_INVALID_FILMTYPE;
		else
			_filmType = edgecode.filmKind;
		
		if (edgecode.codeFormat == kAAFEtNull || 
			edgecode.codeFormat == kAAFEtKeycode ||
			edgecode.codeFormat == kAAFEtEdgenum4 ||
			edgecode.codeFormat == kAAFEtEdgenum5 ||
			edgecode.codeFormat == kAAFEtHeaderSize)

			_edgeType = edgecode.codeFormat;
		else
			return AAFRESULT_INVALID_EDGETYPE;

		_header.setValue(edgecode.header, sizeof(edgecode.header));

	}
	XEXCEPT
	XEND;

	return rc;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFEdgecode::GetEdgecode (aafEdgecode_t * pEdgecode)
{
	HRESULT		rc = AAFRESULT_SUCCESS;

	if (pEdgecode == NULL)
		rc = AAFRESULT_NULL_PARAM;
	else
	{
		pEdgecode->startFrame = _start;
		pEdgecode->filmKind = _filmType;
		pEdgecode->codeFormat = _edgeType;
		_header.copyToBuffer(pEdgecode->header, sizeof(pEdgecode->header));
	}
	return rc;
}






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

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#ifndef __ImplAAFEdgecode_h__
#include "ImplAAFEdgecode.h"
#endif

#include "ImplAAFBuiltinDefs.h"
#include "ImplAAFDictionary.h"

#include <assert.h>
#include <string.h>

#include "AAFTypes.h"
#include "AAFResult.h"
#include "aafErr.h"
#include "aafCvt.h"
#include "AAFUtils.h"
#include "AAFDataDefs.h"

ImplAAFEdgecode::ImplAAFEdgecode ():
_start( PID_EdgeCode_Start, "Start"),
_filmType( PID_EdgeCode_FilmKind, "FilmKind"),
_edgeType( PID_EdgeCode_CodeFormat, "CodeFormat"),
_header( PID_EdgeCode_Header, "Header")
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






/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

 
/***********************************************\
*	Stub only.   Implementation not yet added	*
\***********************************************/




#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#ifndef __ImplAAFEdgecode_h__
#include "ImplAAFEdgecode.h"
#endif

#include <assert.h>
#include <string.h>

#include "AAFTypes.h"
#include "AAFResult.h"
#include "aafErr.h"
#include "aafCvt.h"
#include "AAFUtils.h"
#include "AAFDefUIDs.h"


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
    ImplAAFEdgecode::Create(aafLength_t		length,
                            aafEdgecode_t	edgecode)
{
	HRESULT		rc = AAFRESULT_SUCCESS;
	aafUID_t	ecddef = DDEF_Edgecode;
	
	XPROTECT()
	{
		CHECK(SetNewProps(length, &ecddef));
		_start = edgecode.startFrame;
		if (edgecode.filmKind < kFtNull || edgecode.filmKind > kFt65MM)
			return AAFRESULT_INVALID_FILMTYPE;
		else
			_filmType = edgecode.filmKind;
		
		if (edgecode.codeFormat == kEtNull || 
			edgecode.codeFormat == kEtKeycode ||
			edgecode.codeFormat == kEtEdgenum4 ||
			edgecode.codeFormat == kEtEdgenum5 ||
			edgecode.codeFormat == kEtHeaderSize)

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



OMDEFINE_STORABLE(ImplAAFEdgecode, AUID_AAFEdgecode);



/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/


#ifndef __ImplAAFEssencePlugin_h__
#include "ImplAAFEssencePlugin.h"
#endif

#include <string.h>

const aafClassID_t CLSID_AAFDefaultCodec = { 0xDC089C31, 0x9527, 0x11d2, { 0x80, 0x89, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };
const aafClassID_t CLSID_AAFDefaultStream = { 0x83402901, 0x9146, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };

ImplAAFEssencePlugin::ImplAAFEssencePlugin ()
{
}


ImplAAFEssencePlugin::~ImplAAFEssencePlugin ()
{}

AAFRESULT STDMETHODCALLTYPE
	ImplAAFEssencePlugin::NumCodecsMatching(
		aafUID_t		dataDefinition,	//Looking for audio or video codecs
		aafFileRev_t	revision,		//for which variety of file,
		aafInt32		*result)		// Returns the number of matches,
{
	*result = 1;
	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
	ImplAAFEssencePlugin::GetIndexedCodecInfo(
		aafUID_t		dataDefinition,	//Looking for audio or video codecs,
		aafFileRev_t	revision,		//for which variety of file,
		aafInt32		index,			// Which codec. 1 to number of codecs,
		wchar_t			*pName,			// Codec Name,
		aafInt32		bufSize,		//length of the buffer to hold Codec Name,
		aafUID_t		*codecID)		//ID used to access the codec,
{
	memcpy(pName, L"Test Codec", 28);
	return AAFRESULT_SUCCESS;
}

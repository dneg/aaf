//=---------------------------------------------------------------------=
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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#ifndef __ImplAAFEssencePlugin_h__
#include "ImplAAFEssencePlugin.h"
#endif

#include <string.h>

//const aafClassID_t CLSID_AAFDefaultCodec = { 0xDC089C31, 0x9527, 0x11d2, { 0x80, 0x89, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };
//const aafClassID_t CLSID_AAFDefaultStream = { 0x83402901, 0x9146, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };

ImplAAFEssencePlugin::ImplAAFEssencePlugin ()
{
}


ImplAAFEssencePlugin::~ImplAAFEssencePlugin ()
{}

AAFRESULT STDMETHODCALLTYPE
	ImplAAFEssencePlugin::NumCodecsMatching(
		aafUID_t		/*dataDefinition*/,	//Looking for audio or video codecs
		aafFileRev_t	/*revision*/,		//for which variety of file,
		aafInt32		*result)		// Returns the number of matches,
{
	*result = 1;
	return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
	ImplAAFEssencePlugin::GetIndexedCodecInfo(
		aafUID_t		/*dataDefinition*/,	//Looking for audio or video codecs,
		aafFileRev_t	/*revision*/,		//for which variety of file,
		aafInt32		/*index*/,			// Which codec. 1 to number of codecs,
		wchar_t			*pName,			// Codec Name,
		aafInt32		/*bufSize*/,		//length of the buffer to hold Codec Name,
		aafUID_t		*/*codecID*/)		//ID used to access the codec,
{
	memcpy(pName, L"Test Codec", 28);
	return AAFRESULT_SUCCESS;
}

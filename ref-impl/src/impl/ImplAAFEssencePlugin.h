//@doc
//@class    AAFEssenceAccess | Implementation class for AAFEssenceAccess
#ifndef __ImplAAFEssenceAccess_h__
#define __ImplAAFEssenceAccess_h__

#include "AAFTypes.h"
#include "ImplAAFRoot.h"

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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


class ImplAAFEssenceCodec;




class ImplAAFEssencePlugin : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFEssencePlugin ();
  ~ImplAAFEssencePlugin ();


  virtual AAFRESULT STDMETHODCALLTYPE
	NumCodecsMatching(
		aafUID_t		dataDefinition,	//Looking for audio or video codecs
		aafFileRev_t	revision,		//for which variety of file,
		aafInt32		*result);		// Returns the number of matches,

  virtual AAFRESULT STDMETHODCALLTYPE
	GetIndexedCodecInfo(
		aafUID_t		dataDefinition,	//Looking for audio or video codecs,
		aafFileRev_t	revision,		//for which variety of file,
		aafInt32		index,			// Which codec. 1 to number of codecs,
		wchar_t			*pName,			// Codec Name,
		aafInt32		bufSize,		//length of the buffer to hold Codec Name,
		aafUID_t		*codecID);		//ID used to access the codec,

};

#endif // ! __ImplAAFEssenceAccess_h__


//@doc
//@class    AAFEssenceAccess | Implementation class for AAFEssenceAccess
#ifndef __ImplAAFEssenceAccess_h__
#define __ImplAAFEssenceAccess_h__

#include "AAFTypes.h"
#include "ImplAAFRoot.h"

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


public:
  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFEssenceAccessTest.cpp.
  static AAFRESULT test();
};

#endif // ! __ImplAAFEssenceAccess_h__


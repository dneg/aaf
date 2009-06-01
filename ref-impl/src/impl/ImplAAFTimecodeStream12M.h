//@doc
//@class    AAFTimecodeStream12M | Implementation class for AAFTimecodeStream12M
#ifndef __ImplAAFTimecodeStream12M_h__
#define __ImplAAFTimecodeStream12M_h__


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

/***********************************************\
*	Stub only.   Implementation not yet added	*
\***********************************************/


#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif






#ifndef __ImplAAFTimecodeStream_h__
#include "ImplAAFTimecodeStream.h"
#endif


class ImplAAFTimecodeStream12M : public ImplAAFTimecodeStream
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTimecodeStream12M ();

protected:
  virtual ~ImplAAFTimecodeStream12M ();

public:
  /****/
  // Override from AAFTimecodeStream
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSampleSize (/*[out]*/ aafUInt32 *  pSampleSize);


public:
	// SDK-internal calls
  virtual AAFRESULT STDMETHODCALLTYPE
	UnpackTimecode(aafUInt8 *buffer, aafUInt32 buflen, aafUInt32 fps, aafTimecode_t *tc);
  virtual AAFRESULT STDMETHODCALLTYPE
	PackTimecode(aafTimecode_t *tc, aafUInt8 *buffer, aafUInt32 buflen);
  virtual AAFRESULT STDMETHODCALLTYPE
	UnpackUserBits(aafUInt8 *packedBuffer, aafUInt32 packedBuflen,
					aafUInt8 *unpackedBuffer, aafUInt32 unpackedBuflen);
  virtual AAFRESULT STDMETHODCALLTYPE
	PackUserBits(aafUInt8 *unpackedBuffer, aafUInt32 unpackedBuflen,
					aafUInt8 *packedBuffer, aafUInt32 packedBuflen);
};

#endif // ! __ImplAAFTimecodeStream12M_h__



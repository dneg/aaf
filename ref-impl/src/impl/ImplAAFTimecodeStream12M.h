//@doc
//@class    AAFTimecodeStream12M | Implementation class for AAFTimecodeStream12M
#ifndef __ImplAAFTimecodeStream12M_h__
#define __ImplAAFTimecodeStream12M_h__


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



public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFTimecodeStream12M)
};

#endif // ! __ImplAAFTimecodeStream12M_h__



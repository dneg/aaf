//@doc
//@class    AAFTimecode | Implementation class for AAFTimecode
#ifndef __ImplAAFTimecode_h__
#define __ImplAAFTimecode_h__


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


#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif


class ImplAAFTimecode : public ImplAAFSegment
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTimecode ();
  virtual ~ImplAAFTimecode ();



  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
	    (// @parm [in] Length Property Value
         aafLength_t  length,

		 // @parm [in] Timecode Value (startFrame, drop, fps)
		 aafTimecode_t  *timecode);

/****/
  //****************
  // GetTimecode()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTimecode
		// @parm [out] Timecode (startFrame, drop, fps)
        (aafTimecode_t*  timecode);

/****/
  //****************
  // SetTimecode()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetTimecode
		// @parm [in] Timecode (startFrame, drop, fps)
        (aafTimecode_t  *timecode);

/****/
  // Override from AAFSegment
  virtual AAFRESULT STDMETHODCALLTYPE
    SegmentOffsetToTC (/*[in]*/ aafPosition_t *  pOffset,
	  /*[out]*/ aafTimecode_t *  pTimecode);

  // Override from AAFSegment
  virtual AAFRESULT STDMETHODCALLTYPE
    SegmentTCToOffset (/*[in]*/ aafTimecode_t *  pTimecode,
	  /*[in]*/ aafRational_t *  pEditRate,
	  /*[out]*/ aafFrameOffset_t *  pOffset);

public:
	// Methods which are internal to the SDK
virtual AAFRESULT OffsetToTimecodeClip(aafPosition_t offset, ImplAAFTimecode **result,
												aafPosition_t *tcStartPos);
private:
	OMFixedSizeProperty<aafPosition_t>	_start;
	OMFixedSizeProperty<aafUInt16>	_FPS;
	OMFixedSizeProperty<aafBool>	_drop;
};

#endif // ! __ImplAAFTimecode_h__


//@doc
//@class    AAFTimecode | Implementation class for AAFTimecode
#ifndef __ImplAAFTimecode_h__
#define __ImplAAFTimecode_h__

#include "OMStorable.h"

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif

const int PID_TIMECODE_TC           = 5;

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
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFTimecode)

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFTimecodeTest.cpp.
  static AAFRESULT test();
public:
	// Methods which are internal to the SDK
virtual AAFRESULT OffsetToTimecodeClip(aafPosition_t offset, ImplAAFTimecode **result,
												aafPosition_t *tcStartPos);
private:
	OMFixedSizeProperty<aafTimecode_t>	_timecode;
};

#endif // ! __ImplAAFTimecode_h__


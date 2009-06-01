//@doc
//@class    AAFTimelineMobSlot | Implementation class for AAFTimelineMobSlot
#ifndef __ImplAAFTimelineMobSlot_h__
#define __ImplAAFTimelineMobSlot_h__

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


////////////////////////////////////////////////////////////////////////////////
// Types required by this module:
//
// aafBool,
// aafRational_t,
// aafPosition_t,
// aafSlotID_t,



#ifndef __ImplAAFMobSlot_h__
#include "ImplAAFMobSlot.h"
#endif



class ImplAAFTimelineMobSlot : public ImplAAFMobSlot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTimelineMobSlot ();
  ~ImplAAFTimelineMobSlot ();

  virtual AAFRESULT STDMETHODCALLTYPE
	Initialize ();


  //****************
  // GetEditRate()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetEditRate
        (aafRational_t *  result);  //@parm [out,retval] Edit rate property value


  //****************
  // SetEditRate()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetEditRate
        (const aafRational_t &  value);  //@parm [in] Edit rate property value


  //****************
  // GetOrigin()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetOrigin
        (aafPosition_t *  result);  //@parm [out,retval] Origin property value

  //****************
  // SetOrigin()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetOrigin
        (aafPosition_t  value);  //@parm [in] Origin property value

  //***********************************************************
  // METHOD NAME: SetMarkIn()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
  SetMarkIn (
    // @parm [in] aafPosition_t | value | MarkIn property value
    aafPosition_t  value
  );

  //***********************************************************
  // METHOD NAME: GetMarkIn()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
  GetMarkIn (
    // @parm [out,retval] aafPosition_t * | result | MarkIn property value
    aafPosition_t *  result
  );

  //***********************************************************
  // METHOD NAME: SetMarkOut()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
  SetMarkOut (
    // @parm [in] aafPosition_t | value | MarkOut property value
    aafPosition_t  value
  );

  //***********************************************************
  // METHOD NAME: GetMarkOut()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
  GetMarkOut (
    // @parm [out,retval] aafPosition_t * | result | MarkOut property value
    aafPosition_t *  result
  );

  //***********************************************************
  // METHOD NAME: SetUserPos()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
  SetUserPos (
    // @parm [in] aafPosition_t | value | UserPos property value
    aafPosition_t  value
  );

  //***********************************************************
  // METHOD NAME: GetUserPos()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
  GetUserPos (
    // @parm [out,retval] aafPosition_t * | result | UserPos property value
    aafPosition_t *  result
  );

public:
// Internal to the SDK, but available to other SDK internal code.
virtual AAFRESULT FindSegment(aafPosition_t offset,
										  ImplAAFSegment **segment,
										  aafRational_t *srcRate,
										  aafPosition_t *diffPos);
  virtual AAFRESULT ConvertToEditRate(aafPosition_t tmpPos,
										aafRational_t destRate,
										aafPosition_t *convertPos);
  virtual AAFRESULT ConvertToMyRate(aafPosition_t tmpPos,
										  ImplAAFMobSlot *srcSlot,
										aafPosition_t *convertPos);

protected:
	OMFixedSizeProperty<aafRational_t>	_editRate;
	OMFixedSizeProperty<aafPosition_t>	_origin; 
	OMFixedSizeProperty<aafPosition_t>	_markIn; 
	OMFixedSizeProperty<aafPosition_t>	_markOut; 
	OMFixedSizeProperty<aafPosition_t>	_userPos; 

};

#endif // ! __ImplAAFTimelineMobSlot_h__


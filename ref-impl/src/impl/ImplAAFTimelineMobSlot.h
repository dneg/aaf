
//@doc
//@class    AAFTimelineMobSlot | Implementation class for AAFTimelineMobSlot
#ifndef __ImplAAFTimelineMobSlot_h__
#define __ImplAAFTimelineMobSlot_h__

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


////////////////////////////////////////////////////////////////////////////////
// Types required by this module:
//
// aafBool,
// aafRational_t,
// aafPosition_t,
// aafSlotID_t,


const int PID_TIMELINEMOBSLOT_EDITRATE	= 4;
const int PID_TIMELINEMOBSLOT_ORIGIN	= 5;




#ifndef __ImplAAFMobSlot_h__
#include "ImplAAFMobSlot.h"
#endif

#include "OMProperty.h"


class ImplAAFTimelineMobSlot : public ImplAAFMobSlot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTimelineMobSlot ();
  ~ImplAAFTimelineMobSlot ();


  //****************
  // GetEditRate()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetEditRate
        (aafRational_t *  result);  //@parm [out,retval] Edit rate property value


  //****************
  // GetOrigin()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetOrigin
        (aafPosition_t *  result);  //@parm [out,retval] Origin property value


  //****************
  // GetSlotID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSlotID
        (aafSlotID_t *  result);  //@parm [out,retval] Slot id of the Mob Slot


  // Override from AAFMobSlot
  virtual AAFRESULT STDMETHODCALLTYPE
    IsATrack (/*[out,retval]*/ aafBool *  retval);


public:
  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFTimelineMobSlotTest.cpp.
	static AAFRESULT test();

	AAFRESULT GetTrackDesc(aafInt32 nameSize,		// IN - Size of name buffer
							aafWChar *name,			// IN/OUT preallocated buffer to return name
							aafPosition_t *origin,	// OUT - Origin property value
							aafSlotID_t *trackID);	// OUT - ttrack id property value

protected:
	OMFixedSizeProperty<aafRational_t>	_editRate;
	OMFixedSizeProperty<aafPosition_t>	_origin; 

};

#endif // ! __ImplAAFTimelineMobSlot_h__


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

  OMDECLARE_STORABLE(ImplAAFMobSlot)


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
        (aafRational_t *  value);  //@parm [in] Edit rate property value


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

public:
  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFTimelineMobSlotTest.cpp.
	static AAFRESULT test();

  // Return this objects stored object class.
  virtual AAFRESULT STDMETHODCALLTYPE
	GetObjectClass(aafUID_t * pClass);

protected:
	OMFixedSizeProperty<aafRational_t>	_editRate;
	OMFixedSizeProperty<aafPosition_t>	_origin; 

};

#endif // ! __ImplAAFTimelineMobSlot_h__


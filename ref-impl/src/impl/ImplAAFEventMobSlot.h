//@doc
//@class    AAFEventMobSlot | Implementation class for AAFEventMobSlot
#ifndef __ImplAAFEventMobSlot_h__
#define __ImplAAFEventMobSlot_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/


#ifndef __ImplAAFMobSlot_h__
#include "ImplAAFMobSlot.h"
#endif


class ImplAAFEventMobSlot : public ImplAAFMobSlot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFEventMobSlot ();

protected:
  virtual ~ImplAAFEventMobSlot ();

public:


  //****************
  // GetEditRate()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetEditRate
        // @parm [out,retval] Edit rate property value
        (aafRational_t *  pEditRate);

  //****************
  // SetEditRate()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetEditRate
        // @parm [in] Edit rate property value
        (aafRational_t *  pEditRate);





  // Override from AAFMobSlot
  virtual AAFRESULT STDMETHODCALLTYPE
    SetSegment (/*[in]*/ ImplAAFSegment * pSegment);


public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFEventMobSlot)

protected:
  OMFixedSizeProperty<aafRational_t>	_editRate;
};

#endif // ! __ImplAAFEventMobSlot_h__



//@doc
//@class    AAFStaticMobSlot | Implementation class for AAFStaticMobSlot
#ifndef __ImplAAFStaticMobSlot_h__
#define __ImplAAFStaticMobSlot_h__


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


class ImplAAFStaticMobSlot : public ImplAAFMobSlot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFStaticMobSlot ();

protected:
  virtual ~ImplAAFStaticMobSlot ();
};

#endif // ! __ImplAAFStaticMobSlot_h__



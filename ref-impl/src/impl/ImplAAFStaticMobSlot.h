//@doc
//@class    AAFStaticMobSlot | Implementation class for AAFStaticMobSlot
#ifndef __ImplAAFStaticMobSlot_h__
#define __ImplAAFStaticMobSlot_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
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

public:




public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFStaticMobSlot)
};

#endif // ! __ImplAAFStaticMobSlot_h__



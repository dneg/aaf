//@doc
//@class    AAFTextClip | Implementation class for AAFTextClip
#ifndef __ImplAAFTextClip_h__
#define __ImplAAFTextClip_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/








#ifndef __ImplAAFSourceReference_h__
#include "ImplAAFSourceReference.h"
#endif


class ImplAAFTextClip : public ImplAAFSourceReference
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTextClip ();

protected:
  virtual ~ImplAAFTextClip ();

public:




public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFTextClip)
};

#endif // ! __ImplAAFTextClip_h__



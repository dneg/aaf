//@doc
//@class    AAFHTMLDescriptor | Implementation class for AAFHTMLDescriptor
#ifndef __ImplAAFHTMLDescriptor_h__
#define __ImplAAFHTMLDescriptor_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/








#ifndef __ImplAAFFileDescriptor_h__
#include "ImplAAFFileDescriptor.h"
#endif


class ImplAAFHTMLDescriptor : public ImplAAFFileDescriptor
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFHTMLDescriptor ();

protected:
  virtual ~ImplAAFHTMLDescriptor ();

public:




public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFHTMLDescriptor)
};

#endif // ! __ImplAAFHTMLDescriptor_h__



//@doc
//@class    AAFInterpolationDef | Implementation class for AAFInterpolationDef
#ifndef __ImplAAFInterpolationDef_h__
#define __ImplAAFInterpolationDef_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/



class ImplAAFClassDef;


/***********************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
*												*
\***********************************************/





#ifndef __ImplAAFDefObject_h__
#include "ImplAAFDefObject.h"
#endif


class ImplAAFInterpolationDef : public ImplAAFDefObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFInterpolationDef ();

protected:
  virtual ~ImplAAFInterpolationDef();
};

#endif // ! __ImplAAFInterpolationDef_h__



//@doc
//@class    AAFInterpolationDef | Implementation class for AAFInterpolationDef
#ifndef __ImplAAFInterpolationDef_h__
#define __ImplAAFInterpolationDef_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/



class ImplAAFClassDef;


/***********************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
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

public:

  


public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFInterpolationDef)

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFInterpolationDefTest.cpp.
  static AAFRESULT test();
};

#endif // ! __ImplAAFInterpolationDef_h__



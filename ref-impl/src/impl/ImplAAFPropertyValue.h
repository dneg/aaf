//@doc
//@class    AAFPropertyValue | Implementation class for AAFPropertyValue
#ifndef __ImplAAFPropertyValue_h__
#define __ImplAAFPropertyValue_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


class ImplAAFTypeDef;





#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif


class ImplAAFPropertyValue : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFPropertyValue ();

protected:
  virtual ~ImplAAFPropertyValue ();

public:

  //****************
  // GetType()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetType
        // @parm [out] The type definition associated with this property value
        (ImplAAFTypeDef ** ppTypeDef);


  //****************
  // IsDefinedType()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsDefinedType
        // @parm [out] result
        (aafBool *  pIsDefined);


public:
  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFPropertyValueTest.cpp.
  static AAFRESULT test();
};

#endif // ! __ImplAAFPropertyValue_h__



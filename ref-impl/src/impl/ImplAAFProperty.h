//@doc
//@class    AAFProperty | Implementation class for AAFProperty
#ifndef __ImplAAFProperty_h__
#define __ImplAAFProperty_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


class ImplAAFPropertyDef;

class ImplAAFPropertyValue;





#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif


class ImplAAFProperty : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFProperty ();

protected:
  virtual ~ImplAAFProperty ();

public:

  //****************
  // GetDefinition()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDefinition
        // @parm [out] This property's definition
        (ImplAAFPropertyDef ** ppPropDef);


  //****************
  // GetValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetValue
        // @parm [out] The Property Value object associated with this property.
        (ImplAAFPropertyValue ** ppValue);


public:
  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFPropertyTest.cpp.
  static AAFRESULT test();

  //
  // non-published initialization method
  //
  AAFRESULT Initialize (ImplAAFPropertyDef * pPropDef);


private:
  ImplAAFPropertyDef *   _pPropDef;
  ImplAAFPropertyValue * _pPropVal;
};

#endif // ! __ImplAAFProperty_h__



//@doc
//@class    AAFProperty | Implementation class for AAFProperty
#ifndef __ImplAAFProperty_h__
#define __ImplAAFProperty_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/


class ImplAAFPropertyDef;

class ImplAAFPropertyValue;

class OMProperty;





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
  //
  // non-published initialization method
  //
  AAFRESULT Initialize (ImplAAFPropertyDef * pPropDef,
						OMProperty * pOMProp);


  AAFRESULT pvtSetValue (ImplAAFPropertyValue * pValue);


private:
  ImplAAFPropertyDef *   _pPropDef;
  ImplAAFPropertyValue * _pPropVal;
};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
// caution! includes assert.h
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFProperty> ImplAAFPropertySP;

#endif // ! __ImplAAFProperty_h__

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


protected:
  virtual ~ImplAAFPropertyValue ();

  //****************
  //
  // non-published method to set up one of these.
  //
  AAFRESULT SetType (ImplAAFTypeDef * pType);

  const ImplAAFTypeDef * pvtGetType (void) const;

private:
  // type of this property
  ImplAAFTypeDef * _pType;
};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
// caution! includes assert.h
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFPropertyValue> ImplAAFPropPropertyValueSP;

#endif // ! __ImplAAFPropertyValue_h__

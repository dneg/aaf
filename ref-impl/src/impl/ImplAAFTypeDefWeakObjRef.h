//@doc
//@class    AAFTypeDefWeakObjRef | Implementation class for AAFTypeDefWeakObjRef
#ifndef __ImplAAFTypeDefWeakObjRef_h__
#define __ImplAAFTypeDefWeakObjRef_h__

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


class ImplAAFPropertyValue;

class ImplAAFClassDef;

#ifndef __ImplAAFTypeDefObjectRef_h__
#include "ImplAAFTypeDefObjectRef.h"
#endif


class ImplAAFTypeDefWeakObjRef : public ImplAAFTypeDefObjectRef
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTypeDefWeakObjRef ();

protected:
  virtual ~ImplAAFTypeDefWeakObjRef ();

public:

  // Override from AAFTypeDefObjectRef
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
        (aafUID_t *  pID,
         ImplAAFClassDef * pObjType,
         wchar_t *  pTypeName);


  // Override from AAFTypeDefObjectRef
  virtual AAFRESULT STDMETHODCALLTYPE
    SetObject (/*[in]*/ ImplAAFPropertyValue * pPropVal,
      /*[in]*/ ImplAAFObject * ppObject);

  // Override from AAFTypeDefObjectRef
  virtual AAFRESULT STDMETHODCALLTYPE
    GetObject (/*[in]*/ ImplAAFPropertyValue * pPropVal,
      /*[out]*/ ImplAAFObject ** ppObject);

  // Override from AAFTypeDefObjectRef
  virtual AAFRESULT STDMETHODCALLTYPE
    GetObjectType (/*[out]*/ ImplAAFClassDef ** ppObjType);

  // Override from AAFTypeDefObjectRef
  virtual AAFRESULT STDMETHODCALLTYPE
    CreateValue (/*[in]*/ ImplAAFObject * pObj,
      /*[out]*/ ImplAAFPropertyValue ** ppPropVal);

  // Override from AAFTypeDef
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTypeCategory (/*[out]*/ eAAFTypeCategory_t *  pTid);


public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFTypeDefWeakObjRef)

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFTypeDefWeakObjRefTest.cpp.
  static AAFRESULT test();


  // non-published overrides from AAFTypeDef
  aafBool IsFixedSize (void);
  size_t PropValSize (void);

  OMWeakReferenceProperty<ImplAAFClassDef> _referencedType;
};


#endif // ! __ImplAAFTypeDefWeakObjRef_h__

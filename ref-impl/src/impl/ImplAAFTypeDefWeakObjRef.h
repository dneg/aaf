//@doc
//@class    AAFTypeDefWeakObjRef | Implementation class for AAFTypeDefWeakObjRef
#ifndef __ImplAAFTypeDefWeakObjRef_h__
#define __ImplAAFTypeDefWeakObjRef_h__

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
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
    SetObject (/*[in]*/ ImplAAFPropertyValue * pPropVal,
      /*[in]*/ ImplAAFObject * ppObject);

  // Override from AAFTypeDefObjectRef
  virtual AAFRESULT STDMETHODCALLTYPE
    GetObject (/*[in]*/ ImplAAFPropertyValue * pPropVal,
      /*[out]*/ ImplAAFObject ** ppObject);

  // Override from AAFTypeDefObjectRef
  virtual AAFRESULT STDMETHODCALLTYPE
    GetObjectType (/*[out]*/ ImplAAFClassDef ** ppObjType) const;

  // Override from AAFTypeDefObjectRef
  virtual AAFRESULT STDMETHODCALLTYPE
    CreateValue (/*[in]*/ ImplAAFObject * pObj,
      /*[out]*/ ImplAAFPropertyValue ** ppPropVal);

  // Override from AAFTypeDef
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTypeCategory (/*[out]*/ eAAFTypeCategory_t *  pTid);



public:

  // Override from AAFTypeDefObjectRef
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
        (const aafUID_t *  pID,
         const aafUID_t * pRefdObjID,
         wchar_t *  pTypeName);

  // overrides from ImplAAFTypeDef
  //
  aafBool IsFixedSize (void) const;
  size_t PropValSize (void) const;
  aafBool IsRegistered (void) const;
  size_t NativeSize (void) const;

  virtual OMProperty * 
    pvtCreateOMPropertyMBS (OMPropertyId pid,
							const char * name) const;


private:
  // OMWeakReferenceProperty<ImplAAFClassDef> _referencedType;
  OMFixedSizeProperty<aafUID_t>           _referencedType;

  ImplAAFTypeDefSP                     _cachedAuidType;
  ImplAAFSmartPointer<ImplAAFClassDef> _cachedObjType;

  ImplAAFTypeDefSP BaseType (void) const;
};


#endif // ! __ImplAAFTypeDefWeakObjRef_h__

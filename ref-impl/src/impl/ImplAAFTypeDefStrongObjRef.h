//@doc
//@class    AAFTypeDefStrongObjRef | Implementation class for AAFTypeDefStrongObjRef
#ifndef __ImplAAFTypeDefStrongObjRef_h__
#define __ImplAAFTypeDefStrongObjRef_h__

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


class ImplAAFTypeDefStrongObjRef : public ImplAAFTypeDefObjectRef
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTypeDefStrongObjRef ();

protected:
  virtual ~ImplAAFTypeDefStrongObjRef ();

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
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFTypeDefStrongObjRef)


  // Override from AAFTypeDefObjectRef
  virtual AAFRESULT STDMETHODCALLTYPE
    pvtInitialize
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
  OMFixedSizeProperty<aafUID_t> _referencedType;

  // avoid shortcut typedef in an effort to not include other headers
  ImplAAFSmartPointer<ImplAAFClassDef> _cachedObjType;
};


#endif // ! __ImplAAFTypeDefStrongObjRef_h__

//@doc
//@class    AAFTypeDefStrongObjRef | Implementation class for AAFTypeDefStrongObjRef
#ifndef __ImplAAFTypeDefStrongObjRef_h__
#define __ImplAAFTypeDefStrongObjRef_h__

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


  //*************************************************************
  //
  // Overrides from OMType, via inheritace through ImplAAFTypeDef
  //
  //*************************************************************

  /*
  virtual void reorder(OMByte* bytes,
                       size_t bytesSize) const;

  virtual size_t externalSize(void) const;

  virtual void externalize(OMByte* internalBytes,
                           size_t internalBytesSize,
                           OMByte* externalBytes,
                           size_t externalBytesSize,
                           OMByteOrder byteOrder) const;

  virtual size_t internalSize(void) const;

  virtual void internalize(OMByte* externalBytes,
                           size_t externalBytesSize,
                           OMByte* internalBytes,
                           size_t internalBytesSize,
                           OMByteOrder byteOrder) const;
						   */


public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFTypeDefStrongObjRef)

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFTypeDefStrongObjRefTest.cpp.
  static AAFRESULT test();

  // non-published overrides from AAFTypeDef
  aafBool IsFixedSize (void) const;
  size_t PropValSize (void) const;

  // OMWeakReferenceProperty<ImplAAFClassDef> _referencedType;
  OMFixedSizeProperty<aafUID_t>           _referencedType;
};


#endif // ! __ImplAAFTypeDefStrongObjRef_h__

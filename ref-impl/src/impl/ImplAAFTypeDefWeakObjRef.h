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
  OMDECLARE_STORABLE(ImplAAFTypeDefWeakObjRef)

  // non-published overrides from AAFTypeDef
  aafBool IsFixedSize (void);
  size_t PropValSize (void);

  // OMWeakReferenceProperty<ImplAAFClassDef> _referencedType;
  OMFixedSizeProperty<aafUID_t>           _referencedType;
};


#endif // ! __ImplAAFTypeDefWeakObjRef_h__

//@doc
//@class    AAFTypeDef | Implementation class for AAFTypeDef
#ifndef __ImplAAFTypeDef_h__
#define __ImplAAFTypeDef_h__

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


#ifndef __ImplAAFDefObject_h__
#include "ImplAAFDefObject.h"
#endif

#ifndef OMTYPE_H
#include "OMType.h"
#endif

class ImplAAFTypeDef : public ImplAAFDefObject, public OMType
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTypeDef ();

protected:
  virtual ~ImplAAFTypeDef ();

public:

  //****************
  // GetTypeCategory()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTypeCategory
        // @parm [out] Returned type category
        (eAAFTypeCategory_t *  pTid);


  //****************
  // RawAccessType()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RawAccessType
        // @parm [out] the raw access type definition
        (ImplAAFTypeDef ** ppRawTypeDef);


  //****************
  // GetMinVersion()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetMinVersion
        // @parm [out, retval] Pointer to the minimum version 
        (aafVersionType_t *  pMinVersion);


  //****************
  // SetMinVersion()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetMinVersion
        // @parm [in] Minimum AAF Version
        (aafVersionType_t *  pMinVersion);


  //****************
  // GetMaxVersion()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetMaxVersion
        // @parm [out, retval] Pointer to the maximum version 
        (aafVersionType_t *  pMaxVersion);


  //****************
  // SetMaxVersion()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetMaxVersion
        // @parm [in] Maximum AAF Version
        (aafVersionType_t  MaxVersion);


  //****************
  // GetSwapNeeded()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSwapNeeded
        // @parm [out, retval] Specifies whether the bytes in this type need to be swapped
        (aafSwapNeeded_t *  pSwapNeeded);


  //****************
  // SetSwapNeeded()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetSwapNeeded
        // @parm [in] Specifies wheter the bytes in this type need to be swapped
        (aafSwapNeeded_t  SwapNeeded);

public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFTypeDef)

  //
  // non-published methods
  //

  // Returns true if property values of this type are of a fixed size.
  virtual aafBool IsFixedSize (void) const;

  // If this->IsFixedSize(), then will return the size of property
  // values of this type.  If not fixed size, will assert().
  virtual size_t PropValSize (void) const;

  // Returns true if offsets have been registered for this type def.
  virtual aafBool IsRegistered (void) const;

  // If this->IsRegistered(), then will return the native in-memory
  // size of this type.  If not registered, will assert().
  virtual size_t NativeSize (void) const;

  //*************************************************************
  //
  // Overrides from OMType, via inheritace through ImplAAFTypeDef
  //
  //*************************************************************

  virtual void reorder(OMByte* bytes,
                       size_t bytesSize) const;

  virtual size_t externalSize(OMByte* internalBytes,
                              size_t internalBytesSize) const;

  virtual void externalize(OMByte* internalBytes,
                           size_t internalBytesSize,
                           OMByte* externalBytes,
                           size_t externalBytesSize,
                           OMByteOrder byteOrder) const;

  virtual size_t internalSize(OMByte* externalBytes,
                              size_t externalSize) const;

  virtual void internalize(OMByte* externalBytes,
                           size_t externalBytesSize,
                           OMByte* internalBytes,
                           size_t internalBytesSize,
                           OMByteOrder byteOrder) const;
};

#endif // ! __ImplAAFTypeDef_h__

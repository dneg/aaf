//@doc
//@class    AAFTypeDefInt | Implementation class for AAFTypeDefInt
#ifndef __ImplAAFTypeDefInt_h__
#define __ImplAAFTypeDefInt_h__

//=---------------------------------------------------------------------=
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
// 
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
// 
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif

class ImplAAFPropertyValue;

class ImplAAFTypeDefInt : public ImplAAFTypeDef
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTypeDefInt ();

protected:
  virtual ~ImplAAFTypeDefInt ();

public:

  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
        (// @parm [in] auid to be used to identify this type
         const aafUID_t & id,

         // @parm [in] the size of this integer type in bytes
         aafUInt8  intSize,

         // @parm [in] true if this integer type is signed; false for unsigned
         aafBool  isSigned,

         // @parm [in, string] friendly name of this type definition
         const aafCharacter * pTypeName);


  //****************
  // GetTypeCategory()
  //
  // (override from ImplAAFTypeDef)
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTypeCategory
        // @parm [out] Returned type category
        (eAAFTypeCategory_t *  pTid);


  //****************
  // CreateValue()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CreateValue
        (// @parm [in, size_is(valSize)] pointer to integer initialization value
         aafMemPtr_t  pVal,

         // @parm [in] size of integer, in bytes, in pVal
         aafUInt32  valSize,

         // @parm [out] newly created property value
         ImplAAFPropertyValue ** ppPropVal);


  //****************
  // GetInteger()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetInteger
        (// @parm [in] property value from which value is to be read
         ImplAAFPropertyValue * pPropVal,

         // @parm [out, size_is(bufSize)] buffer into which value is written
         aafMemPtr_t  pBuf,

         // @parm [in] size of pBuf buffer in bytes
         aafUInt32  bufSize);


  //****************
  // SetInteger()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetInteger
        (// @parm [in] property value from which value is to be read
         ImplAAFPropertyValue * pPropVal,

         // @parm [in, size_is(bufSize)] buffer from which value is read
         aafUInt8 *  pBuf,

         // @parm [in] size of pBuf buffer in bytes
         aafUInt32  bufSize);


  //****************
  // GetSize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSize
        // @parm [out] the returned size of this integral value, in bytes
        (aafUInt32 *  pSize);


  //****************
  // IsSigned()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsSigned
        // @parm [out] set to true if this integer type is signed; set to false for
    // unsigned
        (aafBool *  pSigned);


  //*************************************************************
  //
  // Overrides from OMType, via inheritace through ImplAAFTypeDef
  //
  //*************************************************************

  virtual void reorder(OMByte* bytes,
                       size_t bytesSize) const;

  virtual size_t externalSize(const OMByte* internalBytes,
                              size_t internalBytesSize) const;

  virtual void externalize(const OMByte* internalBytes,
                           size_t internalBytesSize,
                           OMByte* externalBytes,
                           size_t externalBytesSize,
                           OMByteOrder byteOrder) const;

  virtual size_t internalSize(const OMByte* externalBytes,
                              size_t externalSize) const;

  virtual void internalize(const OMByte* externalBytes,
                           size_t externalBytesSize,
                           OMByte* internalBytes,
                           size_t internalBytesSize,
                           OMByteOrder byteOrder) const;


public:

  // overrides from ImplAAFTypeDef
  //
  aafBool IsFixedSize (void) const;
  size_t PropValSize (void) const;
  aafBool IsRegistered (void) const;
  size_t NativeSize (void) const;

  virtual OMProperty * 
    pvtCreateOMProperty (OMPropertyId pid,
							const wchar_t * name) const;

  virtual AAFRESULT STDMETHODCALLTYPE
    RawAccessType
        (ImplAAFTypeDef ** ppRawTypeDef);


public:
  // Overrides from ImplAAFTypeDef
  virtual bool IsAggregatable () const;
  virtual bool IsStreamable () const;
  virtual bool IsFixedArrayable () const;
  virtual bool IsVariableArrayable () const;
  virtual bool IsStringable () const;


  // override from OMStorable.
  virtual const OMClassId& classId(void) const;

  // Override callbacks from OMStorable
  virtual void onSave(void* clientContext) const;
  virtual void onRestore(void* clientContext) const;

private:
  // the size of this integer type in bytes
  OMFixedSizeProperty<aafUInt8> _size;

  // true if this integer type is signed; false for unsigned
  OMFixedSizeProperty<aafBool>   _isSigned;
};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
// caution! includes assert.h
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFTypeDefInt> ImplAAFTypeDefIntSP;

#endif // ! __ImplAAFTypeDefInt_h__

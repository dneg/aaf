//@doc
//@class    AAFTypeDefInt | Implementation class for AAFTypeDefInt
#ifndef __ImplAAFTypeDefInt_h__
#define __ImplAAFTypeDefInt_h__

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


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
         aafUID_t *  pID,

         // @parm [in] the size of this integer type in bytes
         aafUInt32  intSize,

         // @parm [in] true if this integer type is signed; false for unsigned
         aafBool  isSigned,

         // @parm [in, string] friendly name of this type definition
         wchar_t *  pTypeName);


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


public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFTypeDefInt)

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFTypeDefIntTest.cpp.
  static AAFRESULT test();

  // overrides from ImplAAFTypeDef
  //
  aafBool IsFixedSize (void);
  size_t PropValSize (void);
  aafBool IsRegistered (void);
  size_t NativeSize (void);

private:
  // the size of this integer type in bytes
  OMFixedSizeProperty<aafUInt8> _size;

  // true if this integer type is signed; false for unsigned
  OMFixedSizeProperty<aafBool>   _isSigned;
};

#endif // ! __ImplAAFTypeDefInt_h__

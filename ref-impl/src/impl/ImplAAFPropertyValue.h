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

protected:
  virtual ~ImplAAFPropertyValue ();

public:

  //****************
  //
  // non-published method to set up one of these.  Bits are copied in
  // to buffer allocated locally.
  //
  AAFRESULT Initialize (ImplAAFTypeDef * pType);


  //****************
  //
  // non-published method to get the internal bits, and the size of
  // data required to represent them.  Bits are not copied, but are
  // passed by reference.
  //
  AAFRESULT GetBits (aafMemPtr_t * ppBits);
  AAFRESULT GetBitsSize (aafUInt32 *pBitsSize);
  //
  // Allocates bitsSize bytes, leaving them uninitialized.  Returns
  // the pointer to those bytes in *ppBits if it's non-NULL.
  AAFRESULT AllocateBits (aafUInt32     bitsSize,
						  aafMemPtr_t * ppBits);
  //
  // Allocates bits from another property value's storage.  size bytes
  // are allocated, at byteOffset from the start of the given prop
  // val.  Returns the pointer to those bytes in *ppBits if it's
  // non-NULL.
  AAFRESULT AllocateFromPropVal(ImplAAFPropertyValue * pOwner,
								aafUInt32 byteOffset,
								aafUInt32 size,
								aafMemPtr_t * ppBits);


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


public:
  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFPropertyValueTest.cpp.
  static AAFRESULT test();


private:
  // type of this property
  ImplAAFTypeDef * _pType;

  // pointer to locally-allocated buffer to hold bits
  aafMemPtr_t      _pBits;

  // size of data contained in _pBits
  aafUInt32        _bitsSize;

  // owner of the bits, if non-NULL.  We'll keep a reference to it so
  // it won't get deleted under us.
  ImplAAFPropertyValue *_ownerPropVal;
};

#endif // ! __ImplAAFPropertyValue_h__

//@doc
//@class    AAFPropertyValue | Implementation class for AAFPropValData
#ifndef __ImplAAFPropValData_h__
#define __ImplAAFPropValData_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/


#ifndef __ImplAAFPropertyValue_h__
#include "ImplAAFPropertyValue.h"
#endif


class ImplAAFPropValData : public ImplAAFPropertyValue
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFPropValData ();


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
  AAFRESULT AllocateFromPropVal(ImplAAFPropValData * pOwner,
								aafUInt32 byteOffset,
								aafUInt32 size,
								aafMemPtr_t * ppBits);


  // non-published method to initialize this object.
  AAFRESULT Initialize (ImplAAFTypeDef * pTypeDef);
  
protected:
  virtual ~ImplAAFPropValData ();

private:
  // pointer to locally-allocated buffer to hold bits
  aafMemPtr_t      _pBits;

  // size of data contained in _pBits
  aafUInt32        _bitsSize;

  // owner of the bits, if non-NULL.  We'll keep a reference to it so
  // it won't get deleted under us.
  ImplAAFPropValData *_ownerPropVal;
};

//
// smart pointer
//

#ifndef __ImplAAFSmartPointer_h__
// caution! includes assert.h
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFPropValData> ImplAAFPropValDataSP;

#endif // ! __ImplAAFPropValData_h__

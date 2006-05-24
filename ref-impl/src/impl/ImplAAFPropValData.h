//@doc
//@class    AAFPropertyValue | Implementation class for AAFPropValData
#ifndef __ImplAAFPropValData_h__
#define __ImplAAFPropValData_h__


//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


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

  virtual AAFRESULT STDMETHODCALLTYPE WriteTo(OMProperty* pOmProp);

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
#include "ImplAAFSmartPointer.h"
#endif

typedef ImplAAFSmartPointer<ImplAAFPropValData> ImplAAFPropValDataSP;

#endif // ! __ImplAAFPropValData_h__

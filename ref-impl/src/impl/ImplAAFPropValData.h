//@doc
//@class    AAFPropertyValue | Implementation class for AAFPropValData
#ifndef __ImplAAFPropValData_h__
#define __ImplAAFPropValData_h__


/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 *  prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/


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

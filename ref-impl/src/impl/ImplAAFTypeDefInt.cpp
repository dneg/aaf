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


#ifndef __ImplAAFTypeDefInt_h__
#include "ImplAAFTypeDefInt.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"
#include "ImplAAFObjectCreation.h"
#include "AAFClassIDs.h"

#ifndef __ImplAAFPropValData_h__
#include "ImplAAFPropValData.h"
#endif

#ifndef __AAFTypeDefUIDs_h__
#include "AAFTypeDefUIDs.h"
#endif

#include <assert.h>
#include <string.h>

extern "C" const aafClassID_t CLSID_AAFPropValData;

// Internal utilities to sign-extend or zero-fill.  Current
// implementations only allow integer sizes of 1, 2, 4, and 8 bytes.

//
// LSB-justifies and sign extends the input value, copying the result
// into the output value.  Requires that inVal and outVal are valid
// pointers, and that inValSize is no larger than outValSize.  Also
// requires that in/outValSize are supported values from the set {1,
// 2, 4, 8} bytes.
//
static void pvtSignExtend (const aafMemPtr_t inVal,
						   aafUInt32   inValSize,
						   aafMemPtr_t outVal,
						   aafUInt32   outValSize)
{
  aafInt32 localValue;	// only 4 bytes; see below for why it's OK.

  assert (inVal);
  assert (outVal);
  assert (inValSize <= outValSize);
  assert ((1 == inValSize) ||
		  (2 == inValSize) ||
		  (4 == inValSize) ||
		  (8 == inValSize));
  assert ((1 == outValSize) ||
		  (2 == outValSize) ||
		  (4 == outValSize) ||
		  (8 == outValSize));
  if (inValSize == outValSize)
	{
	  memcpy (outVal, inVal, inValSize);
	}
  else
	{
	  // At this point we know that inSize < outSize; the largest
	  // outSize can be is 8 bytes, so the largest inSize can be is 4
	  // bytes; that's why localValue can work as only a 4-byte int.
	  switch (inValSize)
		{
		case 1:
		  localValue = *((aafInt8*) inVal);
		  break;
		case 2:
		  localValue = *((aafInt16*) inVal);
		  break;
		case 4:
		  localValue = *((aafInt32*) inVal);
		  break;
		case 8:
		  // inval can't be 8 bytes
		  assert (0);
		default:
		  assert (0);
		}

	  switch (outValSize)
		{
		case 1:
		  // inval can't be 1 byte
		  assert (0);
		case 2:
		  *((aafInt16*) outVal) = (aafInt16) localValue;
		  break;
		case 4:
		  *((aafInt32*) outVal) = localValue;
		  break;
		case 8:
		  *((aafInt64*) outVal) = localValue;
		  break;
		default:
		  assert (0);
		}
	}
}


//
// LSB-justifies and zero-fills the input value, copying the result
// into the output value.  Requires that inVal and outVal are valid
// pointers, and that inValSize is no larger than outValSize.  Also
// requires that in/outValSize are supported values from the set {1,
// 2, 4, 8} bytes.
//
static void pvtZeroFill (const aafMemPtr_t inVal,
						   aafUInt32   inValSize,
						   aafMemPtr_t outVal,
						   aafUInt32   outValSize)
{
  aafUInt32 localValue;	// only 4 bytes; see below for why it's OK.

  assert (inVal);
  assert (outVal);
  assert (inValSize <= outValSize);
  assert ((1 == inValSize) ||
		  (2 == inValSize) ||
		  (4 == inValSize) ||
		  (8 == inValSize));
  assert ((1 == outValSize) ||
		  (2 == outValSize) ||
		  (4 == outValSize) ||
		  (8 == outValSize));
  if (inValSize == outValSize)
	{
	  memcpy (outVal, inVal, inValSize);
	}
  else
	{
	  // At this point we know that inSize < outSize; the largest
	  // outSize can be is 8 bytes, so the largest inSize can be is 4
	  // bytes; that's why localValue can work as only a 4-byte int.
	  switch (inValSize)
		{
		case 1:
		  localValue = *((aafUInt8*) inVal);
		  break;
		case 2:
		  localValue = *((aafUInt16*) inVal);
		  break;
		case 4:
		  localValue = *((aafUInt32*) inVal);
		  break;
		case 8:
		  // inval can't be 8 bytes
		  assert (0);
		default:
		  assert (0);
		}

	  switch (outValSize)
		{
		case 1:
		  // inval can't be 1 byte
		  assert (0);
		case 2:
		  *((aafUInt16*) outVal) = (aafUInt16) localValue;
		  break;
		case 4:
		  *((aafUInt32*) outVal) = localValue;
		  break;
		case 8:
		  // hack! we don't have unsigned 64-bit, so we'll depend on
		  // the compiler to sign-extend and zero-fill the unsigned
		  // localValue, even though the dest (64bit) is signed.
		  *((aafInt64*) outVal) = localValue;
		  break;
		default:
		  assert (0);
		}
	}
}


ImplAAFTypeDefInt::ImplAAFTypeDefInt ()
  : _size     ( PID_TypeDefinitionInteger_Size,     "Size"),
    _isSigned ( PID_TypeDefinitionInteger_IsSigned, "IsSigned")
{
  _persistentProperties.put(_size.address());
  _persistentProperties.put(_isSigned.address());
}

ImplAAFTypeDefInt::~ImplAAFTypeDefInt ()
{
}


AAFRESULT STDMETHODCALLTYPE
   ImplAAFTypeDefInt::Initialize (
      const aafUID_t & id,
      aafUInt8  intSize,
      aafBool  isSigned,
      const aafCharacter * pTypeName)
{
  assert (intSize > 0);
  assert (pTypeName);

  if ((1 != intSize) &&
	  (2 != intSize) &&
	  (4 != intSize) &&
	  (8 != intSize))
	return AAFRESULT_BAD_SIZE;

  AAFRESULT hr;

  hr = ImplAAFMetaDefinition::Initialize(id, pTypeName, NULL);
	if (AAFRESULT_FAILED (hr))
    return hr;

  _size = intSize;
  _isSigned = isSigned;

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefInt::GetTypeCategory (
      eAAFTypeCategory_t *  pTid)
{
  if (! pTid)
	return AAFRESULT_NULL_PARAM;

  *pTid = kAAFTypeCatInt;
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefInt::CreateValue (
      aafMemPtr_t  pVal,
      aafUInt32  valSize,
      ImplAAFPropertyValue ** ppPropVal)
{
  if (! pVal)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  if (! ppPropVal)
	{
	  return AAFRESULT_NULL_PARAM;
	}

  if (valSize > _size)
	{
	  return AAFRESULT_BAD_SIZE;
	}

  // current impl only allows 1, 2, 4, and 8-bit ints.
  if ((1 != valSize) &&
	  (2 != valSize) &&
	  (4 != valSize) &&
	  (8 != valSize))
	{
	  return AAFRESULT_BAD_SIZE;
	}

  // sign-extend or zero-fill the value.
  aafUInt8 valBuf[8];
  assert (_size <= sizeof (valBuf));
  if (_isSigned != 0)
	{
	  pvtSignExtend (pVal, valSize, valBuf, _size);
	}
  else
	{
	  pvtZeroFill (pVal, valSize, valBuf, _size);
	}

  // Create a temporary pointer to copy to the smartptr
  ImplAAFPropValData * tmp = (ImplAAFPropValData *)CreateImpl(CLSID_AAFPropValData);
  if (NULL == tmp)
	return AAFRESULT_NOMEMORY;
  ImplAAFPropValDataSP pv;
  pv = tmp;

  // Bobt: Hack bugfix! SmartPointer operator= will automatically
  // AddRef; CreateImpl *also* will addref, so we've got one too
  // many.  Put us back to normal.
  tmp->ReleaseReference(); // we don't need this reference anymore.
  tmp = 0;

  AAFRESULT hr;
  hr = pv->Initialize(this);
  if (! AAFRESULT_SUCCEEDED (hr))
	return hr;

  aafMemPtr_t pBits = NULL;
  hr = pv->AllocateBits (_size, &pBits);
  if (! AAFRESULT_SUCCEEDED (hr))
	return hr;

  assert (pBits);
  memcpy (pBits, valBuf, _size);

  *ppPropVal = pv;
  (*ppPropVal)->AcquireReference ();
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefInt::GetInteger (
      ImplAAFPropertyValue * pPropVal,
      aafMemPtr_t            pVal,
      aafUInt32              valSize)
{
  if (! pPropVal)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  if (! pVal)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  if (valSize < _size)
	{
	  return AAFRESULT_BAD_SIZE;
	}

  ImplAAFPropValDataSP pvd;
  pvd = dynamic_cast<ImplAAFPropValData*>(pPropVal);
  if (!pvd) return AAFRESULT_BAD_TYPE;

  // get the property value's embedded type
  ImplAAFTypeDefSP pPropType;
  AAFRESULT hr;
  hr = pvd->GetType (&pPropType);
  if (! AAFRESULT_SUCCEEDED (hr))
	{
	  return hr;
	}
  assert (pPropType);

  // determine if the property value's embedded type is compatible
  // with this one for reading.  For now, we'll only allow integral
  // type properties to be read by this integral type def.
  //
  // BobT 6/2/1999: allow all types to be read as an integral type
  // (made necessary in order to read Enum types as integral types.)
  // assert (pPropType);
  // if (! dynamic_cast<ImplAAFTypeDefInt *>(pPropType))
  //	{
  //	  return AAFRESULT_BAD_TYPE;
  //	}
  assert (pPropType);

  // current impl only allows 1, 2, 4, and 8-bit ints.
  if ((1 != valSize) &&
	  (2 != valSize) &&
	  (4 != valSize) &&
	  (8 != valSize))
	{
	  return AAFRESULT_BAD_SIZE;
	}

  // sign-extend or zero-fill the value.
  aafUInt8 valBuf[8];
  aafUInt32 bitsSize = 0;
  hr = pvd->GetBitsSize(&bitsSize);
  if (! AAFRESULT_SUCCEEDED (hr))
	{
	  return hr;
	}
  if (_size < bitsSize)
	{
	  return AAFRESULT_BAD_TYPE;
	}

  assert ((1 == bitsSize) ||
		  (2 == bitsSize) ||
		  (4 == bitsSize) ||
		  (8 == bitsSize));
  assert (bitsSize <= sizeof (valBuf));  // I know, redundant test...
  aafMemPtr_t pBits = NULL;
  hr = pvd->GetBits (&pBits);
  if (AAFRESULT_FAILED(hr)) return hr;
  assert (pBits);

  memcpy (valBuf, pBits, bitsSize);

  // BTW, we know that valsize >= bitsSize (from tests above)
  if (_isSigned != 0)
	{
	  pvtSignExtend (valBuf, bitsSize, pVal, valSize);
	}
  else
	{
	  pvtZeroFill (valBuf, bitsSize, pVal, valSize);
	}
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefInt::SetInteger (
      ImplAAFPropertyValue * pPropVal,
      aafUInt8 *             pVal,
      aafUInt32              valSize)
{
  if (! pPropVal)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  if (! pVal)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  if (valSize > _size)
	{
	  return AAFRESULT_BAD_SIZE;
	}

  ImplAAFPropValDataSP pvd;
  pvd = dynamic_cast<ImplAAFPropValData*>(pPropVal);
  if (!pvd) return AAFRESULT_BAD_TYPE;

  // get the property value's embedded type
  ImplAAFTypeDefSP pPropType;
  AAFRESULT hr;
  hr = pvd->GetType (&pPropType);
  if (! AAFRESULT_SUCCEEDED (hr))
	{
	  return hr;
	}

  // determine if the property value's embedded type is compatible
  // with this one for reading.  For now, we'll only allow integral
  // type properties to be read by this integral type def.
  assert (pPropType);
  if (! dynamic_cast<ImplAAFTypeDefInt *>((ImplAAFTypeDef*) pPropType))
	{
	  return AAFRESULT_BAD_TYPE;
	}

  // current impl only allows 1, 2, 4, and 8-bit ints.
  if ((1 != valSize) &&
	  (2 != valSize) &&
	  (4 != valSize) &&
	  (8 != valSize))
	{
	  return AAFRESULT_BAD_SIZE;
	}

  // sign-extend or zero-fill the value.
  aafUInt8 valBuf[8];
  assert (_size <= sizeof (valBuf));
  if (_isSigned != 0)
	{
	  pvtSignExtend (pVal, valSize, valBuf, _size);
	}
  else
	{
	  pvtZeroFill (pVal, valSize, valBuf, _size);
	}

  aafMemPtr_t pBits = NULL;
  hr = pvd->AllocateBits (_size, &pBits);
  if (! AAFRESULT_SUCCEEDED (hr))
	{
	  return hr;
	}
  assert (pBits);
  memcpy (pBits, valBuf, _size);

  if (! AAFRESULT_SUCCEEDED (hr))
	{
	  return hr;
	}
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefInt::GetSize (
      aafUInt32 *  pSize)
{
  if (! pSize)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  *pSize = _size;
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefInt::IsSigned (
      aafBool *  pSigned)
{
  if (! pSigned)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  *pSigned = _isSigned;
  return AAFRESULT_SUCCESS;
}


void ImplAAFTypeDefInt::reorder(OMByte* bytes,
								size_t bytesSize) const

{
  assert (IsFixedSize());
  assert (PropValSize() == bytesSize);
  assert (bytes);
  if (bytesSize > 1)
	reorderInteger (bytes, bytesSize);
}


size_t ImplAAFTypeDefInt::externalSize(OMByte* /*internalBytes*/,
									   size_t /*internalBytesSize*/) const
{
  assert (IsFixedSize());
  return PropValSize();
}


void ImplAAFTypeDefInt::externalize(OMByte* internalBytes,
									size_t internalBytesSize,
									OMByte* externalBytes,
									size_t externalBytesSize,
									OMByteOrder byteOrder) const
{
  assert (internalBytes);
  assert (externalBytes);
  // assert (internalBytesSize == externalBytesSize);
  const size_t thisPropValSize = PropValSize ();
  assert (externalBytesSize == thisPropValSize);

  if (internalBytesSize > externalBytesSize)
	{
	  // contracting
	  contract (internalBytes,
				internalBytesSize,
				externalBytes,
				externalBytesSize,
				byteOrder);
	}

  else if (internalBytesSize < externalBytesSize)
	{
	  // expanding
	  expand (internalBytes,
				internalBytesSize,
				externalBytes,
				externalBytesSize,
				byteOrder);
	}

  else
	{
	  // size remains the same
	  copy (internalBytes,
			externalBytes,
			externalBytesSize);
	}
}


size_t ImplAAFTypeDefInt::internalSize(OMByte* /*externalBytes*/,
									   size_t /*externalSize*/) const
{
  return NativeSize ();
}


void ImplAAFTypeDefInt::internalize(OMByte* externalBytes,
									size_t externalBytesSize,
									OMByte* internalBytes,
									size_t internalBytesSize,
									OMByteOrder byteOrder) const
{
  assert (externalBytes);
  assert (internalBytes);
  // assert (internalBytesSize == externalBytesSize);
  // const size_t thisNativeSize = NativeSize ();
  // assert (internalBytesSize == thisNativeSize);

  if (externalBytesSize > internalBytesSize)
	{
	  // contracting
	  contract (externalBytes,
				externalBytesSize,
				internalBytes,
				internalBytesSize,
				byteOrder);
	}

  else if (externalBytesSize < internalBytesSize)
	{
	  // expanding
	  expand (externalBytes,
				externalBytesSize,
				internalBytes,
				internalBytesSize,
				byteOrder);
	}

  else
	{
	  // size remains the same
	  copy (externalBytes,
			internalBytes,
			internalBytesSize);
	}
}


aafBool ImplAAFTypeDefInt::IsFixedSize (void) const
{
  return kAAFTrue;
}


size_t ImplAAFTypeDefInt::PropValSize (void) const
{
  return _size;
}


aafBool ImplAAFTypeDefInt::IsRegistered (void) const
{
  // int types are registered by default
  return kAAFTrue;
}


size_t ImplAAFTypeDefInt::NativeSize (void) const
{
  // same as property value size
  return PropValSize();
}


OMProperty * ImplAAFTypeDefInt::pvtCreateOMPropertyMBS
  (OMPropertyId pid,
   const char * name) const
{
  assert (name);
  size_t elemSize = PropValSize ();
  OMProperty * result = new OMSimpleProperty (pid, name, elemSize);
  assert (result);
  return result;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefInt::RawAccessType (
      ImplAAFTypeDef ** ppRawTypeDef)
{
  // Return variable array of unsigned char
  return pvtGetUInt8Array8Type (ppRawTypeDef);
}


bool ImplAAFTypeDefInt::IsAggregatable () const
{ return true; }

bool ImplAAFTypeDefInt::IsStreamable () const
{ return true; }

bool ImplAAFTypeDefInt::IsFixedArrayable () const
{ return true; }

bool ImplAAFTypeDefInt::IsVariableArrayable () const
{ return true; }

bool ImplAAFTypeDefInt::IsStringable () const
{ return true; }

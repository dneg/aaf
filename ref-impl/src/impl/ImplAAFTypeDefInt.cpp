/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


#ifndef __ImplAAFTypeDefInt_h__
#include "ImplAAFTypeDefInt.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"
#include "ImplAAFObjectCreation.h"
#include "AAFClassIDs.h"

#ifndef __ImplAAFPropertyValue_h__
#include "ImplAAFPropertyValue.h"
#endif

#include <assert.h>
#include <string.h>

extern "C" const aafClassID_t CLSID_AAFPropertyValue;

// Internal utilities to sign-extend or zero-fill.  Current
// implementations only allow integer sizes of 1, 2, 4, and 8 bytes.

//
// LSB-justifies and sign extends the input value, copying the result
// into the output value.  Requires that inVal and outVal are valid
// pointers, and that inValSize is no larger than outValSize.  Also
// requires that in/outValSize are supported values from the set {1,
// 2, 4, 8} bytes.
//
static void pvtSignExtend (aafMemPtr_t inVal,
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
	  memcpy (inVal, outVal, inValSize);
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
static void pvtZeroFill (aafMemPtr_t inVal,
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
	  memcpy (inVal, outVal, inValSize);
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
{}


AAFRESULT STDMETHODCALLTYPE
   ImplAAFTypeDefInt::Initialize (
      aafUID_t *  pID,
      aafUInt32  intSize,
      aafBool  isSigned,
      wchar_t *  pTypeName)
{
  assert (pID);
  assert (intSize > 0);
  assert (pTypeName);

  if ((1 != intSize) &&
	  (2 != intSize) &&
	  (4 != intSize) &&
	  (8 != intSize))
	return AAFRESULT_BAD_SIZE;

  _size = intSize;
  _isSigned = isSigned;
  AAFRESULT hr = SetName (pTypeName);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;

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

  ImplAAFPropertyValue * pv = NULL;
  pv = (ImplAAFPropertyValue *)CreateImpl(CLSID_AAFPropertyValue);
  if (! pv)
	{
	  return AAFRESULT_NOMEMORY;
	}

  AAFRESULT hr;
  hr = pv->Initialize(this);
  if (! AAFRESULT_SUCCEEDED (hr))
	{
	  pv->ReleaseReference ();
	  return hr;
	}

  aafMemPtr_t pBits = NULL;
  hr = pv->AllocateBits (valSize, &pBits);
  if (! AAFRESULT_SUCCEEDED (hr))
	{
	  pv->ReleaseReference ();
	  return hr;
	}
  assert (pBits);
  memcpy (pBits, valBuf, valSize);

  *ppPropVal = pv;
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

  // get the property value's embedded type
  ImplAAFTypeDef * pPropType;
  AAFRESULT hr;
  hr = pPropVal->GetType (&pPropType);
  if (! AAFRESULT_SUCCEEDED (hr))
	{
	  return hr;
	}

  // determine if the property value's embedded type is compatible
  // with this one for reading.  For now, we'll only allow integral
  // type properties to be read by this integral type def.
  assert (pPropType);
  if (! dynamic_cast<ImplAAFTypeDefInt *>(pPropType))
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
  aafUInt32 bitsSize = 0;
  hr = pPropVal->GetBitsSize(&bitsSize);
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
  hr = pPropVal->GetBits (&pBits);
  if (AAFRESULT_FAILED(hr)) return hr;
  assert (pBits);

  memcpy (pBits, valBuf, bitsSize);

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

  // get the property value's embedded type
  ImplAAFTypeDef * pPropType;
  AAFRESULT hr;
  hr = pPropVal->GetType (&pPropType);
  if (! AAFRESULT_SUCCEEDED (hr))
	{
	  return hr;
	}

  // determine if the property value's embedded type is compatible
  // with this one for reading.  For now, we'll only allow integral
  // type properties to be read by this integral type def.
  assert (pPropType);
  if (! dynamic_cast<ImplAAFTypeDefInt *>(pPropType))
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
  hr = pPropVal->AllocateBits (_size, &pBits);
  if (! AAFRESULT_SUCCEEDED (hr))
	{
	  pPropVal->ReleaseReference ();
	  return hr;
	}
  assert (pBits);
  memcpy (pBits, valBuf, valSize);

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


aafBool ImplAAFTypeDefInt::IsFixedSize (void)
{
  return AAFTrue;
}


size_t ImplAAFTypeDefInt::PropValSize (void)
{
  return _size;
}


OMDEFINE_STORABLE(ImplAAFTypeDefInt, AUID_AAFTypeDefInt)

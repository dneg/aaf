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

#ifndef __ImplAAFPropValData_h__
#include "ImplAAFPropValData.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFPropValData::ImplAAFPropValData ()
  : _pBits (0),
	_bitsSize (0),
	_ownerPropVal (0)
{}


ImplAAFPropValData::~ImplAAFPropValData ()
{
  if (_ownerPropVal)
	{
	  _ownerPropVal->ReleaseReference ();
	  _ownerPropVal = NULL;
	}
  else
	{
	  delete[] _pBits;
	  _pBits = 0;
	}
}


AAFRESULT ImplAAFPropValData::Initialize (
      ImplAAFTypeDef * pTD)
{
  assert (! pvtGetType ());
  return SetType (pTD);
}


AAFRESULT ImplAAFPropValData::AllocateFromPropVal (
      ImplAAFPropValData * pNewOwner,
	  aafUInt32 byteOffset,
	  aafUInt32 size,
	  aafMemPtr_t * ppBits)
{
  if (! pNewOwner) return AAFRESULT_NULL_PARAM;
  assert (pNewOwner->_pBits);
  assert ((byteOffset + size) <= pNewOwner->_bitsSize);

  if (_ownerPropVal)
	{
	  _ownerPropVal->ReleaseReference ();
	  _ownerPropVal = NULL;
	}
  else
	{
	  if (_pBits)
		{
	      delete[] _pBits;
	      _pBits = NULL;
		}
	}
  _ownerPropVal = pNewOwner;
  _ownerPropVal->AcquireReference ();
  _pBits = pNewOwner->_pBits + byteOffset;
  _bitsSize = size;

  if (ppBits)
	*ppBits = _pBits;
  return AAFRESULT_SUCCESS;
}


AAFRESULT ImplAAFPropValData::AllocateBits (
	  aafUInt32     bitsSize,
      aafMemPtr_t * ppBits)
{
  if ((bitsSize != _bitsSize) || (_pBits == 0)) // 2000-12-05 transdel: make sure _pBits are allocated!
	{
	  if (_ownerPropVal)
		{
		  _ownerPropVal->ReleaseReference ();
		  _ownerPropVal = NULL;
		}
	  else
		{
		  delete[] _pBits;
		  _pBits = 0;
		}
	  _pBits = new aafUInt8[bitsSize];
	  if (! _pBits)
		{
		  return AAFRESULT_NOMEMORY;
		}
	  memset (_pBits, 0, bitsSize);
	  _bitsSize = bitsSize;
	}	  
  if (ppBits)
	*ppBits = _pBits;

  return AAFRESULT_SUCCESS;
}


AAFRESULT ImplAAFPropValData::GetBits (
      aafMemPtr_t * ppBits)
{
  if (!ppBits)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  *ppBits = _pBits;
  return AAFRESULT_SUCCESS;
}


AAFRESULT ImplAAFPropValData::GetBitsSize (
	  aafUInt32 * pBitsSize)
{
  if (! pBitsSize)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  *pBitsSize = _bitsSize;
  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE ImplAAFPropValData::WriteTo(
      OMProperty* pOmProp)
{
  aafMemPtr_t bits = 0;
  aafUInt32 bitsSize;
  HRESULT hr = GetBits (&bits);
  if (AAFRESULT_FAILED (hr)) return hr;
//  assert (bits);

  hr = GetBitsSize (&bitsSize);
  if (AAFRESULT_FAILED (hr)) return hr;

  // OMSimpleProperty * pSimpleProp = dynamic_cast <OMSimpleProperty*>(pOmProp);
  // assert (pSimpleProp);
  // pSimpleProp->setBits (bits, bitsSize);
  
  if (bits) 
  pOmProp->setBits (bits, bitsSize);
  return AAFRESULT_SUCCESS;
}

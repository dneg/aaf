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

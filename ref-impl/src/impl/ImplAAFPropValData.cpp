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
  if (bitsSize != _bitsSize)
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

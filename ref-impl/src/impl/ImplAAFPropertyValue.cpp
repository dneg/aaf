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




#ifndef __ImplAAFPropertyValue_h__
#include "ImplAAFPropertyValue.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFPropertyValue::ImplAAFPropertyValue ()
  : _pType (0),
	_pBits (0),
	_bitsSize (0),
	_ownerPropVal (0)
{}


ImplAAFPropertyValue::~ImplAAFPropertyValue ()
{
  if (_pType)
	_pType->ReleaseReference ();

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


AAFRESULT ImplAAFPropertyValue::Initialize (
	  ImplAAFTypeDef * pType)
{
  if (! pType) return AAFRESULT_NULL_PARAM;
  // make sure we haven't been init'd yet
  assert (! _pType);
  assert (!_ownerPropVal);
  assert (!_pBits);

  _pType = pType;
  _pType->AcquireReference ();
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyValue::GetType (
      ImplAAFTypeDef ** ppTypeDef)
{
  if (! ppTypeDef)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  assert (_pType);
  _pType->AcquireReference ();
  *ppTypeDef = _pType;
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyValue::IsDefinedType (
      aafBool *  /*pIsDefined*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT ImplAAFPropertyValue::AllocateFromPropVal (
      ImplAAFPropertyValue * pNewOwner,
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
	  delete[] _pBits;
	  _pBits = NULL;
	}
  _ownerPropVal = pNewOwner;
  _ownerPropVal->AcquireReference ();
  _pBits = pNewOwner->_pBits + byteOffset;
  _bitsSize = size;

  if (ppBits)
	*ppBits = _pBits;
  return AAFRESULT_SUCCESS;
}


AAFRESULT ImplAAFPropertyValue::AllocateBits (
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


AAFRESULT ImplAAFPropertyValue::GetBits (
      aafMemPtr_t * ppBits)
{
  if (!ppBits)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  *ppBits = _pBits;
  return AAFRESULT_SUCCESS;
}


AAFRESULT ImplAAFPropertyValue::GetBitsSize (
	  aafUInt32 * pBitsSize)
{
  if (! pBitsSize)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  *pBitsSize = _bitsSize;
  return AAFRESULT_SUCCESS;
}

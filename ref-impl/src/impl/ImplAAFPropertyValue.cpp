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




#ifndef __ImplAAFPropertyValue_h__
#include "ImplAAFPropertyValue.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFPropertyValue::ImplAAFPropertyValue ()
  : _pType (0)
{}


ImplAAFPropertyValue::~ImplAAFPropertyValue ()
{
  // BobT: type is not to be reference counted.
  // if (_pType)
  //  _pType->ReleaseReference ();
}


AAFRESULT ImplAAFPropertyValue::SetType (
	  ImplAAFTypeDef * pType)
{
  if (! pType) return AAFRESULT_NULL_PARAM;
  // make sure we haven't been init'd yet
  assert (! _pType);

  _pType = pType;
  // BobT: type is not to be reference counted.
  // _pType->AcquireReference ();
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
      aafBool * pIsDefined)
{
  if (! pIsDefined)
	return AAFRESULT_NULL_PARAM;
  // Currently the type returned by GetType() is always the defined type.
  *pIsDefined = AAFTrue;
  return AAFRESULT_SUCCESS;
}


const ImplAAFTypeDef * ImplAAFPropertyValue::pvtGetType (void) const
{
  return _pType;
}


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

#include "ImplEnumAAFProperties.h"

#include "ImplAAFProperty.h"
#include "ImplAAFObjectCreation.h"

#include <assert.h>
#include <string.h>

extern "C" const aafClassID_t CLSID_EnumAAFProperties;


ImplEnumAAFProperties::ImplEnumAAFProperties ()
  : _rep (0)
{}


ImplEnumAAFProperties::~ImplEnumAAFProperties ()
{
  if (_rep)
	delete _rep;
}

AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFProperties::NextOne (
      ImplAAFProperty ** ppProperty)
{
  if (! _rep) return AAFRESULT_NOT_INITIALIZED;
  return _rep->NextOne (ppProperty);
}



AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFProperties::Next (
      aafUInt32  count,
      ImplAAFProperty ** ppProperties,
      aafUInt32 *  pNumFetched)
{
  if (! _rep) return AAFRESULT_NOT_INITIALIZED;
  return _rep->Next (count, ppProperties, pNumFetched);
}



AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFProperties::Skip (
      aafUInt32  count)
{
  if (! _rep) return AAFRESULT_NOT_INITIALIZED;
  return _rep->Skip (count);
}



AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFProperties::Reset ()
{
  if (! _rep) return AAFRESULT_NOT_INITIALIZED;
  return _rep->Reset ();
}



AAFRESULT STDMETHODCALLTYPE
    ImplEnumAAFProperties::Clone (
      ImplEnumAAFProperties ** ppEnum)
{
  ImplEnumAAFProperties * theEnum;

  if (! ppEnum) return AAFRESULT_NULL_PARAM;
  if (! _rep) return AAFRESULT_NOT_INITIALIZED;

  theEnum = (ImplEnumAAFProperties *)CreateImpl(CLSID_EnumAAFProperties);
  if (theEnum == NULL)
	return E_FAIL;
		
  // copy this enumerator
  assert (_rep);
  theEnum->_rep = new OldImplAAFEnumerator<ImplAAFProperty*>(*_rep);
  if (! theEnum->_rep)
	{
	  theEnum->ReleaseReference();
	  theEnum = 0;
	  return AAFRESULT_NOMEMORY;
	}

  assert (ppEnum);
  *ppEnum = theEnum;
  return AAFRESULT_SUCCESS;
}



AAFRESULT
    ImplEnumAAFProperties::Initialize (
      ImplAAFCollection<ImplAAFProperty*> * pProperties)
{
  if (! pProperties)
	return AAFRESULT_NULL_PARAM;

  // make sure it hasn't been init'd before
  assert (! _rep);
  _rep = new OldImplAAFEnumerator<ImplAAFProperty*>(pProperties);
  if (! _rep)
	return AAFRESULT_NOMEMORY;
  assert (_rep);

  return AAFRESULT_SUCCESS;
}

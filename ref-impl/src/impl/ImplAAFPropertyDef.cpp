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
 * prior written permission of Avid Technology, Inc.
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


#ifndef __ImplEnumAAFPropertyValues_h__
#include "ImplEnumAAFPropertyValues.h"
#endif

#ifndef __ImplAAFDictionary_h__
#include "ImplAAFDictionary.h"
#endif

#ifndef __ImplAAFPropertyValue_h__
#include "ImplAAFPropertyValue.h"
#endif

#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif

#ifndef __ImplAAFPropertyDef_h__
#include "ImplAAFPropertyDef.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include <assert.h>
#include <string.h>


ImplAAFPropertyDef::ImplAAFPropertyDef ()
  : _Type(PID_PropertyDefinition_Type, "Type"),
    _IsOptional(PID_PropertyDefinition_IsOptional, "IsOptional"),
    _pid(PID_PropertyDefinition_LocalIdentification, "LocalIdentification"),
    _DefaultValue(PID_PropertyDefinition_DefaultValue, "DefaultValue"),
	_cachedType (0),  // BobT: don't reference count the cached type!
	_bname (0),
	_OMPropCreateFunc (0)
{
  _persistentProperties.put (_Type.address());
  _persistentProperties.put (_IsOptional.address());
  _persistentProperties.put (_pid.address());
  _persistentProperties.put (_DefaultValue.address());
}


ImplAAFPropertyDef::~ImplAAFPropertyDef ()
{
  // BobT: don't reference count the cached type!

  delete[] _bname;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyDef::pvtInitialize (
      const aafUID_t & propertyAuid,
      OMPropertyId omPid,
      const aafCharacter * pPropName,
	  const aafUID_t & typeId,
      aafBool isOptional)
{
  AAFRESULT hr;

  if (! pPropName) return AAFRESULT_NULL_PARAM;

  hr = ImplAAFMetaDefinition::Initialize(propertyAuid, pPropName, NULL);
	if (AAFRESULT_FAILED (hr))
    return hr;

  _Type = typeId;
  _pid = omPid;
  _IsOptional = isOptional;

  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyDef::GetTypeDef (
      ImplAAFTypeDef ** ppTypeDef) const
{
  if (! ppTypeDef) return AAFRESULT_NULL_PARAM;

  if (! _cachedType)
	{
	  ImplAAFDictionarySP pDict;
	  AAFRESULT hr;

	  hr = GetDictionary(&pDict);
	  if (AAFRESULT_FAILED (hr)) return hr;
	  assert (pDict);

	  ImplAAFPropertyDef * pNonConstThis =
		  (ImplAAFPropertyDef *) this;
	  aafUID_t typeId = _Type;

	  // Remember that _cachedType is *not* referenced counted!
	  ImplAAFTypeDef * tmp = 0;
	  hr = pDict->LookupTypeDef (typeId, &tmp);
	  if (AAFRESULT_FAILED (hr))
		return hr;
	  assert (tmp);
	  // If lookup caused this to already be put into the cache, just
	  // throw away the current copy (in tmp)
	  if (! pNonConstThis->_cachedType)
		pNonConstThis->_cachedType = tmp;
	  tmp->ReleaseReference ();
	  tmp = 0;
	}
  assert (ppTypeDef);
  *ppTypeDef = _cachedType;
  assert (*ppTypeDef);
  (*ppTypeDef)->AcquireReference ();

  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyDef::GetIsOptional (
       aafBool * pIsOptional) const
{
  if (! pIsOptional)
	return AAFRESULT_NULL_PARAM;
  *pIsOptional = _IsOptional;
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyDef::GetDefaultValue (
      ImplAAFPropertyValue ** /*ppDataValue*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFPropertyDef::SetDefaultValue (
      ImplAAFPropertyValue * /*pDataValue*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



OMPropertyId ImplAAFPropertyDef::OmPid (void) const
{
  return _pid;
}


const OMType* ImplAAFPropertyDef::type(void) const
{
  AAFRESULT hr;
  ImplAAFTypeDef * ptd = 0;

  hr = GetTypeDef (&ptd);
  assert (AAFRESULT_SUCCEEDED (hr));
  assert (ptd);
  // Don't reference count these!
  aafUInt32 refCount;
  refCount = ptd->ReleaseReference ();
  // make sure our assumption (dict owns a ref) is correct
  assert (refCount > 0);
  return ptd;
}


const char* ImplAAFPropertyDef::name(void) const
{
  if (! _bname)
	{
	  // We'll have to convert the aafCharacter name to regular
	  // byte-sized characters.
	  AAFRESULT hr;
	  aafCharacter * wname = 0;
	  aafUInt32 nameLen;

	  ImplAAFPropertyDef * pNonConstThis =
		(ImplAAFPropertyDef *) this;
	  hr = pNonConstThis->GetNameBufLen (&nameLen);
	  assert (AAFRESULT_SUCCEEDED (hr));
	  wname = (aafCharacter*) new aafUInt8[nameLen];
	  assert (wname);

	  hr = pNonConstThis->GetName (wname, nameLen);
	  assert (AAFRESULT_SUCCEEDED (hr));

	  // Convert the prop name
	  pNonConstThis->_bname = new char [nameLen];
	  assert (_bname);
	  wcstombs (pNonConstThis->_bname, wname, nameLen);
	  delete [] wname;
	  // null terminate.  Don't forget nameLen is in bytes for a
	  // string of aafCharacters, so we'll have to cut it in half in
	  // order to get the proper index for the null terminator.
	  _bname[nameLen/(sizeof (aafCharacter) / sizeof (aafUInt8))]
		= '\0';
	}
  assert (_bname);
  return _bname;
}


OMPropertyId ImplAAFPropertyDef::localIdentification(void) const
{
  return _pid;
}


bool ImplAAFPropertyDef::isOptional(void) const
{
  return (kAAFTrue == _IsOptional) ? true : false;
}


OMProperty * ImplAAFPropertyDef::CreateOMProperty () const
{
  OMProperty * result = 0;

  ImplAAFPropertyDef* pNonConstThis = (ImplAAFPropertyDef*) this;

  if (_OMPropCreateFunc)
	{
	  result = _OMPropCreateFunc (_pid, name());
	}

  if (! result)
	{
	  // Either there was no create func, or an existing one deferred
	  // to the type def.
	  ImplAAFTypeDefSP ptd;
	  AAFRESULT hr = GetTypeDef (&ptd);
	  assert (AAFRESULT_SUCCEEDED (hr));
	  assert (ptd);
	  result = ptd->pvtCreateOMPropertyMBS (_pid, name());
	}

  return result;
}


void ImplAAFPropertyDef::SetOMPropCreateFunc
(ImplAAFOMPropertyCreateFunc_t pFunc)
{
  assert (pFunc);
  _OMPropCreateFunc = pFunc;
}

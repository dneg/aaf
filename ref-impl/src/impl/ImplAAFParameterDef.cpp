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


#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#ifndef __ImplAAFParameterDef_h__
#include "ImplAAFParameterDef.h"
#endif

#include <assert.h>
#include <string.h>
#include "aafErr.h"
#include "ImplAAFDictionary.h"

ImplAAFParameterDef::ImplAAFParameterDef ()
: _typeDef(			PID_ParameterDefinition_Type,					"Type"),
  _displayUnits(	PID_ParameterDefinition_DisplayUnits,			"DisplayUnits")
{
	_persistentProperties.put(_typeDef.address());
	_persistentProperties.put(_displayUnits.address());
}


ImplAAFParameterDef::~ImplAAFParameterDef ()
{
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFParameterDef::GetTypeDef (
      ImplAAFTypeDef **ppTypeDef)
{
	aafUID_t			uid;
	ImplAAFDictionary	*dict = NULL;

	if(ppTypeDef == NULL)
		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		uid = _typeDef;
		CHECK(GetDictionary(&dict));
		CHECK(dict->LookupType (_typeDef, ppTypeDef));
		dict->ReleaseReference();
		dict = NULL;
	}
	XEXCEPT
	{
		if(dict != NULL)
		  dict->ReleaseReference();
		dict = 0;
	}
	XEND;
	
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFParameterDef::SetTypeDef (
      ImplAAFTypeDef * pTypeDef)
{
	aafUID_t	uid;
	AAFRESULT	hr;

	if(pTypeDef == NULL)
		return AAFRESULT_NULL_PARAM;

	hr = pTypeDef->GetAUID(&uid);
	if(hr == AAFRESULT_SUCCESS)
		_typeDef = uid;

	return hr;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFParameterDef::GetDisplayUnits (
      wchar_t *pDisplayUnits,
      aafInt32  bufSize)
{
	if(pDisplayUnits == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(!_displayUnits.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	bool stat;
	
	stat = _displayUnits.copyToBuffer(pDisplayUnits, bufSize);
	if (! stat)
	{
	  return AAFRESULT_SMALLBUF;	// Shouldn't the API have a length parm?
	}

	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFParameterDef::GetDisplayUnitsBufLen (
      aafInt32 * pLen)
{
	if(pLen == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(!_displayUnits.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	*pLen = _displayUnits.size();
	return(AAFRESULT_SUCCESS); 
}




AAFRESULT STDMETHODCALLTYPE
    ImplAAFParameterDef::SetDisplayUnits (
      wchar_t *pDisplayUnits)
{
	if(pDisplayUnits == NULL)
		return(AAFRESULT_NULL_PARAM);

	_displayUnits = pDisplayUnits;

	return(AAFRESULT_SUCCESS); 
}




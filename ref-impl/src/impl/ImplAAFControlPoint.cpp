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

#ifndef __ImplAAFDataDef_h__
#include "ImplAAFDataDef.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#ifndef __ImplAAFControlPoint_h__
#include "ImplAAFControlPoint.h"
#endif

#include "ImplAAFDictionary.h"
#include "ImplAAFDefObject.h"
#include "ImplAAFParameterDef.h"
#include "ImplAAFTypeDef.h"
#include "ImplAAFHeader.h"

#include <assert.h>
#include <string.h>
#include "aafErr.h"


ImplAAFControlPoint::ImplAAFControlPoint ()
: _type(			PID_ControlPoint_Type,			"Type"),
  _time(			PID_ControlPoint_Time,			"Time"),
  _value(			PID_ControlPoint_Value,			"Value"),
  _hint(			PID_ControlPoint_EditHint,		"EditHint")
{
	_persistentProperties.put(_type.address());
	_persistentProperties.put(_time.address());
	_persistentProperties.put(_value.address());
	_persistentProperties.put(_hint.address());
}


ImplAAFControlPoint::~ImplAAFControlPoint ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFControlPoint::GetTime (
      aafRational_t *pTime)
{
	if(pTime == NULL)
		return AAFRESULT_NULL_PARAM;
	*pTime = _time;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFControlPoint::GetEditHint (
      aafEditHint_t *pEditHint)
{
	if(pEditHint == NULL)
		return AAFRESULT_NULL_PARAM;

	if (!_hint.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	aafUInt16	intHint;
		
	intHint = _hint;
	*pEditHint = (aafEditHint_t)intHint;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFControlPoint::GetValueBufLen (
      aafUInt32 * pLen)
{
	if(pLen == NULL)
		return(AAFRESULT_NULL_PARAM);

	*pLen = _value.size();

	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFControlPoint::GetValue (
      aafUInt32  valueSize,
      aafDataBuffer_t  pValue,
      aafUInt32*  bytesRead)
{
	if(pValue == NULL || bytesRead == NULL)
		return(AAFRESULT_NULL_PARAM);

	if (_value.size() > valueSize)
	  return AAFRESULT_SMALLBUF;

	_value.copyToBuffer(pValue, valueSize);
	*bytesRead  = _value.size();

	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFControlPoint::SetTime (
      aafRational_t pTime)
{
	_time = pTime;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFControlPoint::SetEditHint (
      aafEditHint_t  editHint)
{
	_hint = (aafUInt16)editHint;
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFControlPoint::SetValue (
      aafUInt32  valueSize,
      aafDataBuffer_t  pValue)
{
	if(pValue == NULL)
		return(AAFRESULT_NULL_PARAM);

	_value.setValue(pValue, valueSize);

	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFControlPoint::SetTypeDefinition (
      ImplAAFTypeDef*  pTypeDef)
{
	aafUID_t			newUID;
	ImplAAFHeader		*head = NULL;
	ImplAAFDictionary	*dict = NULL;

	if(pTypeDef == NULL)
		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		CHECK(pTypeDef->GetAUID(&newUID));
		CHECK(pTypeDef->MyHeadObject(&head));
		CHECK(head->GetDictionary(&dict));
//		if(dict->LookupType(&newUID, &def) == AAFRESULT_SUCCESS)
//			def->ReleaseReference();

		_type = newUID;
//		pTypeDef->AcquireReference();
		head->ReleaseReference();
		head = NULL;
		dict->ReleaseReference();
		dict = NULL;
	}
	XEXCEPT
	{
		if(head)
		  {
			head->ReleaseReference();
			head = 0;
		  }
		if(dict)
		  {
			dict->ReleaseReference();
			dict = 0;
		  }
	}
	XEND;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFControlPoint::GetTypeDefinition (
      ImplAAFTypeDef **ppTypeDef)
{
	ImplAAFHeader		*head = NULL;
	ImplAAFDictionary	*dict =  NULL;
	aafUID_t			typeID;

	if(ppTypeDef == NULL)
		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		CHECK(MyHeadObject(&head));
		CHECK(head->GetDictionary(&dict));
		typeID = _type;
		CHECK(dict->LookupType(&typeID, ppTypeDef));
//		(*ppTypeDef)->AcquireReference();
		head->ReleaseReference();
		head = NULL;
		dict->ReleaseReference();
		dict = NULL;
	}
	XEXCEPT
	{
		if(head)
		  {
			head->ReleaseReference();
			head = 0;
		  }
		if(dict)
		  {
			dict->ReleaseReference();
			dict = 0;
		  }
	}
	XEND;

	return AAFRESULT_SUCCESS;
}





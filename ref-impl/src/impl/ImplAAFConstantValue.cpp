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

#ifndef __ImplAAFParameter_h__
#include "ImplAAFParameter.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#ifndef __ImplAAFConstantValue_h__
#include "ImplAAFConstantValue.h"
#endif

#include <assert.h>
#include <string.h>


ImplAAFConstantValue::ImplAAFConstantValue ()
: _value(			PID_ConstantValue_Value,			"Value")
{
	_persistentProperties.put(_value.address());
}


ImplAAFConstantValue::~ImplAAFConstantValue ()
{}


	
/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFConstantValue::GetValue (
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
    ImplAAFConstantValue::GetValueBufLen (
      aafUInt32 *pLen)
{
	if(pLen == NULL)
		return(AAFRESULT_NULL_PARAM);

	*pLen = _value.size();

	return(AAFRESULT_SUCCESS); 
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFConstantValue::SetValue (
      aafUInt32  valueSize,
      aafDataBuffer_t  pValue)
{
	if(pValue == NULL)
		return(AAFRESULT_NULL_PARAM);

	_value.setValue(pValue, valueSize);

	return(AAFRESULT_SUCCESS); 
}





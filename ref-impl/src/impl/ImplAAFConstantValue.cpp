/***********************************************\
*                                               *
* Advanced Authoring Format                     *
*                                               *
* Copyright (c) 1998-1999 Avid Technology, Inc. *
*                                               *
\***********************************************/

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


OMDEFINE_STORABLE(ImplAAFConstantValue, AUID_AAFConstantValue);



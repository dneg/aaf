//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
//
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=





#ifndef __ImplAAFEssenceFormat_h__
#include "ImplAAFEssenceFormat.h"
#endif

#include <string.h>

#include "AAFUtils.h"
#define CHUNK_SIZE	10

ImplAAFEssenceFormat::ImplAAFEssenceFormat ()
{
	_elements = 0;
	_elemUsed = 0;
	_elemAllocated = 0;
}


ImplAAFEssenceFormat::~ImplAAFEssenceFormat ()
{
	aafUInt32	n;

	for(n = 0; n < _elemUsed; n++)
	{
		if(_elements[n].parmValue != 0)
			delete [] _elements[n].parmValue;
	}
	delete [] _elements;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceFormat::AddFormatSpecifier (aafUID_t  essenceFormatCode,
                           aafInt32  valueSize,
                           aafDataBuffer_t  value)
{
	oneParm_t		*parm, *tempParm;
	aafDataBuffer_t	temp;
	aafUInt32		n;

	parm = Lookup(essenceFormatCode);
	if(parm == 0)	// New format spec
	{
		if(_elemUsed >= _elemAllocated)	// Allocate more memory
		{
			tempParm = _elements;
			_elements = new oneParm_t[_elemAllocated + CHUNK_SIZE];
			_elemAllocated += CHUNK_SIZE;
			for(n = 0; n < _elemUsed; n++)
				_elements[n] = tempParm[n];
			if(tempParm != 0)
				delete [] tempParm;
		}
		
		parm = _elements + _elemUsed;
		_elemUsed++;
		if(valueSize != 0)
		{
			parm->parmValue = new unsigned char[valueSize];
			memcpy(parm->parmValue, value, valueSize);
		}
		else
			parm->parmValue = 0;
		parm->valueSize = valueSize;
		parm->allocSize = valueSize;
		parm->parmName = essenceFormatCode;
	}
	else
	{
		if(parm->allocSize < (aafUInt32)valueSize)
		{
			temp = parm->parmValue;
			if(valueSize != 0)
			{
				parm->parmValue = new unsigned char[valueSize];
				memcpy(parm->parmValue, temp, valueSize);
			}
			else
				parm->parmValue = 0;
			parm->allocSize = valueSize;
			
			if(temp != 0)
				delete [] temp;
		}
		if(parm->parmValue != 0 && valueSize != 0)
			memcpy(parm->parmValue, value, valueSize);	//!!!
		parm->valueSize = valueSize;
	}

	return(AAFRESULT_SUCCESS);
}

			//@comm The value data is passed in as a void * through the "value"
			// argument.  The size of the value must be passed through the
			// valueSize argument.	


/****/
 AAFRESULT STDMETHODCALLTYPE
   ImplAAFEssenceFormat::GetFormatSpecifier (aafUID_t  essenceFormatCode,
                           aafInt32  bufSize,
                           aafDataBuffer_t  value,
                           aafInt32*  bytesRead)
{
	oneParm_t	*parm;
	
	parm = Lookup(essenceFormatCode);
	if(parm == 0)
		return(AAFRESULT_FORMAT_NOT_FOUND);
	if((aafUInt32)bufSize < parm->valueSize)
		return(AAFRESULT_SMALLBUF);

	if(parm->parmValue != 0 && parm->valueSize != 0)
		memcpy(value, parm->parmValue, parm->valueSize);//!!!
	*bytesRead = parm->valueSize;

	return AAFRESULT_SUCCESS;
}

			//@comm The actual number of bytes read is returned in
			// bytesRead.  If the buffer is not big enough to return the entire
			// value, an error is returned.
/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceFormat::NumFormatSpecifiers (aafInt32* numSpecifiers)
{
	*numSpecifiers = _elemUsed;
	return AAFRESULT_SUCCESS;
}

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceFormat::GetIndexedFormatSpecifier (aafInt32  index,
                           aafUID_t		*essenceFormatCode,
                           aafInt32  bufSize,
                           aafDataBuffer_t  value,
                           aafInt32*  bytesRead)
{	
	if((aafUInt32)index >= _elemUsed)
		return(AAFRESULT_FORMAT_BOUNDS);

	*essenceFormatCode = _elements[index].parmName;
	if(bufSize != 0)
	{
		if((aafUInt32)bufSize < _elements[index].valueSize)
			return(AAFRESULT_SMALLBUF);
		if(_elements[index].valueSize != 0)
			memcpy(value, _elements[index].parmValue, _elements[index].valueSize);
		*bytesRead = _elements[index].valueSize;
	}		

	return AAFRESULT_SUCCESS;
}



//****** Internal
oneParm_t	*ImplAAFEssenceFormat::Lookup(aafUID_t lookup)
{
	aafUInt32	n;

	for(n = 0; n < _elemUsed; n++)
	{
		if(EqualAUID(&_elements[n].parmName, &lookup))
			return(_elements+n);
	}

	return(0);
}



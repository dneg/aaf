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





#ifndef __ImplAAFEssenceFormat_h__
#include "ImplAAFEssenceFormat.h"
#endif

#include <assert.h>
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



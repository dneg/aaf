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

#ifndef __ImplAAFTextLocator_h__
#include "ImplAAFTextLocator.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include "AAFResult.h"
#include <assert.h>
#include <string.h>


ImplAAFTextLocator::ImplAAFTextLocator () :
	_name(PID_TextLocator_Name,		"Text Locator")
{
	_persistentProperties.put(_name.address());
	_name = L"";
}


ImplAAFTextLocator::~ImplAAFTextLocator ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFTextLocator::Initialize ()
{
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTextLocator::GetName (wchar_t* pNameBuf, aafInt32  bufSize)
{
	bool stat;
	if (! pNameBuf)
	{
		return AAFRESULT_NULL_PARAM;
	}
	stat = _name.copyToBuffer(pNameBuf, bufSize);
	if (! stat)
	{
		return AAFRESULT_SMALLBUF;
	}
	
	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTextLocator::GetNameBufLen (aafInt32* pLen)
{
	if (! pLen)
	{
		return AAFRESULT_NULL_PARAM;
	}
	*pLen = _name.size();
	
	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFTextLocator::SetName (wchar_t* pNameBuf)
{
	if (! pNameBuf)
	{
		return AAFRESULT_NULL_PARAM;
	}
	_name = pNameBuf;

	return(AAFRESULT_SUCCESS); 
}






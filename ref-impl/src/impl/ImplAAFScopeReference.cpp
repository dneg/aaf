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



/*************************************************************************
 * 
 * @class AAFScopeReference | AAFScopeReference refers to a section in the 
 *			specified AAFNestedScope slo tor AAFMobSLot.
 *
 * @base public | AAFSegment
 *
 *************************************************************************/



#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#ifndef __ImplAAFScopeReference_h__
#include "ImplAAFScopeReference.h"
#endif

#include <assert.h>
#include <string.h>
#include "AAFResult.h"


ImplAAFScopeReference::ImplAAFScopeReference ():
	_relativeScope(			PID_ScopeReference_RelativeScope,		L"RelativeScope"),
	_relativeSlot(			PID_ScopeReference_RelativeSlot,		L"RelativeSlot")
{
	_persistentProperties.put(		_relativeScope.address());
	_persistentProperties.put(		_relativeSlot.address());
}


ImplAAFScopeReference::~ImplAAFScopeReference ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFScopeReference::Initialize(ImplAAFDataDef *pDataDef,
	aafUInt32 RelativeScope,aafUInt32 RelativeSlot)
{
	if(!pDataDef)
		return(AAFRESULT_NULL_PARAM);

	_relativeScope = RelativeScope;
	_relativeSlot = RelativeSlot;

	return(ImplAAFComponent::SetDataDef(pDataDef));
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFScopeReference::Create (aafUInt32 RelativeScope, aafUInt32 RelativeSlot)
{

	_relativeScope = RelativeScope;
	_relativeSlot = RelativeSlot;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFScopeReference::GetRelativeScope (aafUInt32* pnRelativeScope)
{
	if (pnRelativeScope == NULL)
		return AAFRESULT_NULL_PARAM;

	*pnRelativeScope = _relativeScope;

	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFScopeReference::GetRelativeSlot (aafUInt32* pnRelativeSlot)
{
	if (pnRelativeSlot == NULL)
		return AAFRESULT_NULL_PARAM;

	*pnRelativeSlot = _relativeSlot;

	return AAFRESULT_SUCCESS;
}






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

#ifndef __ImplAAFSourceReference_h__
#include "ImplAAFSourceReference.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include <assert.h>
#include "AAFResult.h"
#include "aafUtils.h"

ImplAAFSourceReference::ImplAAFSourceReference ():
	_sourceID(			PID_SourceReference_SourceID,		"SourceID"),
	_sourceMobSlotId(	PID_SourceReference_SourceMobSlotID,	"SourceMobSlotID")
{
	_persistentProperties.put(		_sourceID.address());
	_persistentProperties.put(		_sourceMobSlotId.address());
}


ImplAAFSourceReference::~ImplAAFSourceReference ()
{}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceReference::GetSourceID (aafMobID_t*	pSourceID)
{
    AAFRESULT aafError = AAFRESULT_SUCCESS;
	if (pSourceID == NULL)
	{
		aafError = AAFRESULT_NULL_PARAM;
	}
	else if (!_sourceID.isPresent())
	{
		aafError = AAFRESULT_PROP_NOT_PRESENT;
	}
	else
	{
		*pSourceID = _sourceID;
	}

	return aafError;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceReference::SetSourceID (aafMobID_constref	sourceID)
{
    AAFRESULT aafError = AAFRESULT_SUCCESS;

	_sourceID = sourceID;

	return aafError;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceReference::GetSourceMobSlotID (aafSlotID_t*	pMobSlotID)
{
    AAFRESULT aafError = AAFRESULT_SUCCESS;
	if (pMobSlotID == NULL)
	{
		aafError = AAFRESULT_NULL_PARAM;
	}
	else
	{
		*pMobSlotID = _sourceMobSlotId;
	}

	return aafError;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFSourceReference::SetSourceMobSlotID (aafSlotID_t		mobSlotID)
{
    AAFRESULT aafError = AAFRESULT_SUCCESS;

	_sourceMobSlotId = mobSlotID;

	return aafError;
}


AAFRESULT ImplAAFSourceReference::ChangeContainedReferences(aafMobID_constref from,
															aafMobID_constref to)
{
	aafMobID_t			myID;

	if(memcmp(&myID, &from, sizeof(aafMobID_t)) == 0)
		SetSourceID(to);

	return AAFRESULT_SUCCESS;
}




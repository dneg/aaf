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

#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFContainerDef_h__
#include "ImplAAFContainerDef.h"
#endif

#include <assert.h>
#include <string.h>
#include "AAFPropertyIDs.h"


ImplAAFContainerDef::ImplAAFContainerDef () :
_isIdentified(		PID_ContainerDefinition_EssenceIsIdentified, L"EssenceIsIdentified")
{
  _persistentProperties.put(_isIdentified.address());
}


ImplAAFContainerDef::~ImplAAFContainerDef ()
{}

  
AAFRESULT STDMETHODCALLTYPE
    ImplAAFContainerDef::Initialize (
      const aafUID_t & id,
	  const aafWChar * pName,
	  const aafWChar * pDesc)
{
	if (pName == NULL || pDesc == NULL)
	{
	  return AAFRESULT_NULL_PARAM;
	}
	else
	{
	  return pvtInitialize(id, pName, pDesc);
	}
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFContainerDef::EssenceIsIdentified (
      aafBool *pEssenceIsIdentified)
{
	
	if (pEssenceIsIdentified == NULL)
	{
		return AAFRESULT_NULL_PARAM;
	}
	
	else if (!_isIdentified.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	else
	{
		*pEssenceIsIdentified = _isIdentified;
	}

	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFContainerDef::SetEssenceIsIdentified (
      aafBool EssenceIsIdentified)
{
	_isIdentified = EssenceIsIdentified;
	return AAFRESULT_SUCCESS;
}





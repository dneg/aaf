/************************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*												*
\************************************************/

#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFContainerDef_h__
#include "ImplAAFContainerDef.h"
#endif

#include <assert.h>
#include <string.h>
#include "AAFPropertyIDs.h"


ImplAAFContainerDef::ImplAAFContainerDef () :
_isIdentified(		PID_ContainerDefinition_EssenceIsIdentified, "EssenceIsIdentified")
{
  _persistentProperties.put(_isIdentified.address());
}


ImplAAFContainerDef::~ImplAAFContainerDef ()
{}


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


OMDEFINE_STORABLE(ImplAAFContainerDef, AUID_AAFContainerDef);



/************************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*												*
\************************************************/


#ifndef __ImplAAFClassDef_h__
#include "ImplAAFClassDef.h"
#endif

#ifndef __ImplEnumAAFClassDefs_h__
#include "ImplEnumAAFClassDefs.h"
#endif

#ifndef __ImplAAFTypeDef_h__
#include "ImplAAFTypeDef.h"
#endif

#ifndef __ImplEnumAAFTypeDefs_h__
#include "ImplEnumAAFTypeDefs.h"
#endif

#ifndef __ImplAAFDataDef_h__
#include "ImplAAFDataDef.h"
#endif

#ifndef __ImplEnumAAFDataDefs_h__
#include "ImplEnumAAFDataDefs.h"
#endif

#ifndef __ImplAAFEffectDef_h__
#include "ImplAAFEffectDef.h"
#endif

#ifndef __ImplEnumAAFEffectDefs_h__
#include "ImplEnumAAFEffectDefs.h"
#endif







#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include "ImplAAFObjectCreation.h"
#include "ImplEnumAAFPluggableDefs.h"

#ifndef __ImplAAFDictionary_h__
#include "ImplAAFDictionary.h"
#endif

#include "ImplAAFMob.h"

#include <assert.h>
#include <string.h>
#include "aafErr.h"
#include "aafUtils.h"

extern "C" const aafClassID_t CLSID_EnumAAFPluggableDefs;

ImplAAFDictionary::ImplAAFDictionary ()
: _pluginDefinitions(PID_Dictionary_PluginDefinitions, "PluginDefinitions")
{
  _persistentProperties.put(_pluginDefinitions.address());
}


ImplAAFDictionary::~ImplAAFDictionary ()
{
	size_t i;
	// Release the pluginDefinitions
	size_t size = _pluginDefinitions.getSize();
	for (i = 0; i < size; i++)
	{
		ImplAAFPluggableDef *pPlug =_pluginDefinitions.setValueAt(0, i);
		if (pPlug)
		{
		  pPlug->ReleaseReference();
		}
	}
}



#ifndef __ImplAAFBaseClassFactory_h__
#include "ImplAAFBaseClassFactory.h"
#endif



OMStorable* ImplAAFDictionary::create(const OMClassId& classId) const
{
  OMStorable *result = NULL;


  // If the given classId is for the dictionary then just return 
  // this instance.
  if (0 == memcmp(&classId, &AUID_AAFDictionary, sizeof(OMClassId)))
  {
    // Bump the reference count before returning this.
    AcquireReference();
    return const_cast<ImplAAFDictionary*>(this);
  }

  // Search the current dictionary...
  // TBD
  

	// If not in then current dictionary then look in the built-in
  // dictionary...
	if (NULL == result)
	{
    // Attempt to lookup the class in the base class factory before looking in the
	  // current file.
	  const aafUID_t* pAUID  = reinterpret_cast<const aafUID_t*>(&classId);
    result = ImplAAFBaseClassFactory::CreateInstance(pAUID);
	}

	return result;
}


// Creates a single uninitialized AAF object of the class associated 
  // with a specified stored object id.
AAFRESULT STDMETHODCALLTYPE 
  ImplAAFDictionary::CreateInstance (
    // Class identifier (AUID) of the stored object. This is the
    // corresponding SMPTE identifier (as a GUID) for all predefined
    // built-in classes.
    aafUID_t * pAUID,

    // Address of output variable that receives the 
    // object pointer requested in pAUID
    ImplAAFObject ** ppvObject)
{
  if (NULL == pAUID || NULL == ppvObject)
    return AAFRESULT_NULL_PARAM;
  
  // Initialize the out parameter.
	*ppvObject = NULL;

  const OMClassId* classId  = reinterpret_cast<const OMClassId*>(pAUID);
  *ppvObject = static_cast<ImplAAFObject *>(create(*classId));

  if (NULL == *ppvObject)
    return AAFRESULT_INVALID_CLASS_ID;
  else
    return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupClass (
      aafUID_t *  /*pClassID*/,
      ImplAAFClassDef ** /*ppClassDef*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::RegisterClass (
      aafUID_t * /*pClassAUID*/,
      ImplAAFClassDef * /*pClassDef*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::GetClassDefinitions (
      ImplEnumAAFClassDefs ** /*ppEnum*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::RegisterType (
      aafUID_t * /*pTypeAUID*/,
      ImplAAFTypeDef * /*pTypeDef*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupType (
      aafUID_t *  /*pTypeID*/,
      ImplAAFTypeDef ** /*ppTypeDef*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::GetTypeDefinitions (
      ImplEnumAAFTypeDefs ** /*ppEnum*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::RegisterDataDefintion (
      ImplAAFDataDef * /*pDataDef*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupDataDefintion (
      aafUID_t *  /*pDataDefintionID*/,
      ImplAAFDataDef ** /*ppDataDef*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::GetDataDefinitions (
      ImplEnumAAFDataDefs ** /*ppEnum*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::RegisterEffectDefinition (
      ImplAAFEffectDef * /*pEffectDef*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupEffectDefinition (
      aafUID_t *  /*effectID*/,
      ImplAAFEffectDef ** /*ppEffectDef*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::GetEffectDefinitions (
      ImplEnumAAFEffectDefs ** /*ppEnum*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::RegisterPluggableDefinition (
      ImplAAFPluggableDef *pPlugDef)
{
	if (NULL == pPlugDef)
		return AAFRESULT_NULL_PARAM;
	
	_pluginDefinitions.appendValue(pPlugDef);
	// trr - We are saving a copy of pointer in _pluginDefinitions
	// so we need to bump its reference count.
	pPlugDef->AcquireReference();
	
	return(AAFRESULT_SUCCESS);
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::GetPluggableDefinitions (
	ImplEnumAAFPluggableDefs **ppEnum)
{
	if (NULL == ppEnum)
		return AAFRESULT_NULL_PARAM;
	*ppEnum = 0;
	
	ImplEnumAAFPluggableDefs *theEnum = (ImplEnumAAFPluggableDefs *)CreateImpl (CLSID_EnumAAFPluggableDefs);
	
	XPROTECT()
	{
		CHECK(theEnum->SetDictionary(this));
		*ppEnum = theEnum;
	}
	XEXCEPT
	{
		if (theEnum)
			theEnum->ReleaseReference();
		return(XCODE());
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT
    ImplAAFDictionary::GetNumPluggableDefs(aafUInt32 *  pNumPluggableDefs)
{
  size_t siz;

  if(pNumPluggableDefs == NULL)
    return AAFRESULT_NULL_PARAM;
  
  _pluginDefinitions.getSize(siz);
  
  *pNumPluggableDefs = siz;
  return AAFRESULT_SUCCESS;
}

AAFRESULT
    ImplAAFDictionary::GetNthPluggableDef (aafInt32 index, ImplAAFPluggableDef **ppPluggableDefs)
{
  if (NULL == ppPluggableDefs)
    return AAFRESULT_NULL_PARAM;

  ImplAAFPluggableDef *obj = NULL;
  _pluginDefinitions.getValueAt(obj, index);
  *ppPluggableDefs = obj;
	
  // trr - We are returning a copy of pointer stored in _mobs so we need
  // to bump its reference count.
  if (obj)
    obj->AcquireReference();
  else
    return AAFRESULT_NO_MORE_OBJECTS;

  return AAFRESULT_SUCCESS;
}

AAFRESULT ImplAAFDictionary::LookupPluggableDef(aafUID_t *defID, ImplAAFPluggableDef **result)
{
	ImplEnumAAFPluggableDefs	*pluggableEnum = NULL;
	ImplAAFPluggableDef			*pluggable = NULL;
	aafBool						defFound;
	AAFRESULT					status;
	aafUID_t					testAUID;

	XPROTECT()
	{
		*result = NULL;
		CHECK(GetPluggableDefinitions (&pluggableEnum));
		status = pluggableEnum->NextOne (&pluggable);
		defFound = AAFFalse;
		while(status == AAFRESULT_SUCCESS && !defFound)
		{
			CHECK(pluggable->GetAUID (&testAUID));
			if(EqualAUID(defID, &testAUID))
			{
				defFound = AAFTrue;
				*result = pluggable;
				pluggable->AcquireReference();
			}
			pluggable->ReleaseReference();
			pluggable = NULL;
			status = pluggableEnum->NextOne (&pluggable);
		}
		if(pluggable != NULL)
		{
			pluggable->ReleaseReference();
			pluggable = NULL;
		}
		pluggableEnum->ReleaseReference();
		pluggableEnum = NULL;
		if(!defFound)
			 RAISE(AAFRESULT_NO_MORE_OBJECTS);
	}
	XEXCEPT
	{
		if(pluggableEnum != NULL)
			pluggableEnum->ReleaseReference();
		if(pluggable != NULL)
			pluggable->ReleaseReference();
	}
	XEND
	
	return(AAFRESULT_SUCCESS);
}



OMDEFINE_STORABLE(ImplAAFDictionary, AUID_AAFDictionary);



/************************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*												*
\************************************************/
#ifndef __ImplAAFDictionary_h__
#include "ImplAAFDictionary.h"
#endif



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

#ifndef __ImplAAFParameterDef_h__
#include "ImplAAFParameterDef.h"
#endif

#ifndef __ImplEnumAAFEffectDefs_h__
#include "ImplEnumAAFEffectDefs.h"
#endif


#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#include "ImplAAFObjectCreation.h"
#include "ImplEnumAAFPluggableDefs.h"


#ifndef __ImplAAFBaseClassFactory_h__
#include "ImplAAFBaseClassFactory.h"
#endif

#ifndef __ImplAAFBuiltins_h__
#include "ImplAAFBuiltins.h"
#endif



#include "ImplAAFMob.h"

#include <assert.h>
#include <string.h>
#include "aafErr.h"
#include "AAFUtils.h"

extern "C" const aafClassID_t CLSID_EnumAAFPluggableDefs;
extern "C" const aafClassID_t CLSID_EnumAAFEffectDefs;
extern "C" const aafClassID_t CLSID_EnumAAFParameterDefs;
extern "C" const aafClassID_t CLSID_EnumAAFTypeDefs;

extern aafUID_t gTypeID_AUID;
extern aafUID_t gTypeID_UInt8;
extern aafUID_t gTypeID_UInt16;
extern aafUID_t gTypeID_Int16;
extern aafUID_t gTypeID_UInt32;
extern aafUID_t gTypeID_Int32;
extern aafUID_t gTypeID_Int8;
extern aafUID_t gTypeID_Int64;
extern aafUID_t gTypeID_ObjRef;
extern aafUID_t gTypeID_ObjRefArray;
extern aafUID_t gTypeID_AUIDArray;
extern aafUID_t gTypeID_UInt8Array;
extern aafUID_t gTypeID_UInt8Array8;


ImplAAFDictionary::ImplAAFDictionary ()
: _pluginDefinitions    (PID_Dictionary_PluginDefinitions,    "PluginDefinitions"),
  _effectDefinitions    (PID_Dictionary_EffectDefinitions,    "EffectDefinitions"), 
  _parameterDefinitions (PID_Dictionary_ParameterDefinitions, "ParameterDefinitions"),
  _typeDefinitions      (PID_Dictionary_TypeDefinitions,      "TypeDefinitions"),
  _builtinsInited (AAFFalse)
{
  _persistentProperties.put (_pluginDefinitions.address());
  _persistentProperties.put (_effectDefinitions.address());
  _persistentProperties.put (_parameterDefinitions.address());
  _persistentProperties.put (_typeDefinitions.address());

  InitBuiltins();
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

	size_t effectDefSize = _effectDefinitions.getSize();
	for (i = 0; i < effectDefSize; i++)
	{
		ImplAAFEffectDef *pPlug =_effectDefinitions.setValueAt(0, i);
		if (pPlug)
		{
		  pPlug->ReleaseReference();
		}
	}

	size_t paramDefSize = _parameterDefinitions.getSize();
	for (i = 0; i < paramDefSize; i++)
	{
		ImplAAFParameterDef *pPlug = _parameterDefinitions.setValueAt(0, i);
		if (pPlug)
		{
		  pPlug->ReleaseReference();
		}
	}
}



//
// Factory function for all built-in classes.
//
static ImplAAFObject* CreateBaseClassInstance(const aafUID_t* pAUID) 
{

  // Lookup the code class id for the given stored object id.
  const aafClassID_t* id = ImplAAFBaseClassFactory::LookupClassID(pAUID);
  if (NULL == id)
    return NULL;
  
  // Attempt to create the corresponding storable object.
  ImplAAFRoot *impl = ::CreateImpl(*id);
  if (NULL == impl)
  { // This is a serious programming error. A stored object id was found in the file
	  // with a known base class id but no base object could be created.
    assert(NULL != impl);
    return NULL;
  }

  // Make sure that the object we created was actually one of our
  // ImplAAFObject derived classes.
  ImplAAFObject* object = dynamic_cast<ImplAAFObject*>(impl);
  if (NULL == object)
  { // Not a valid object. Release the pointer so we don't leak memory.
    impl->ReleaseReference();
    impl = 0;
    assert(NULL != object);
	  return NULL;
  }

  return object;
}




//
// Create an instance of the appropriate derived class, given the class id.
//  This method implements the OMClassFactory interface.
//
OMStorable* ImplAAFDictionary::create(const OMClassId& classId) const
{
	const aafUID_t* pAUID  = reinterpret_cast<const aafUID_t*>(&classId);
  OMStorable *result = NULL;


  // If the given classId is for the dictionary then just return 
  // this instance.
  if (0 == memcmp(pAUID, &AUID_AAFDictionary, sizeof(aafUID_t)))
  {
    // Bump the reference count before returning this.
    AcquireReference();
    return const_cast<ImplAAFDictionary*>(this);
  }


  //
	// Look in the built-in dictionary first.
  //

  // Lookup the code class id for the given stored object id.
  result = ::CreateBaseClassInstance(pAUID);
  if (NULL != result)
    return result;


  //
  // Search the current dictionary...
  // TBD
  //
  
	

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
      ImplAAFTypeDef * pTypeDef)
{
  if (NULL == pTypeDef)
	return AAFRESULT_NULL_PARAM;
	
  _typeDefinitions.appendValue(pTypeDef);
  pTypeDef->AcquireReference();
	
  pTypeDef->SetDict(this);

  return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupType (
      aafUID_t *  pTypeID,
      ImplAAFTypeDef ** ppTypeDef)
{
	ImplEnumAAFTypeDefs		*typeEnum = NULL;
	ImplAAFTypeDef			*typeDef = NULL;
	aafBool						defFound;
	AAFRESULT					status;
	aafUID_t					testAUID;

	XPROTECT()
	{
		CHECK(GetTypeDefinitions (&typeEnum));
		status = typeEnum->NextOne (&typeDef);
		defFound = AAFFalse;
		while(status == AAFRESULT_SUCCESS && !defFound)
		{
			CHECK(typeDef->GetAUID (&testAUID));
			if(EqualAUID(pTypeID, &testAUID))
			{
				defFound = AAFTrue;
				*ppTypeDef = typeDef;
				typeDef->AcquireReference();
				break;
			}
			typeDef->ReleaseReference();
			typeDef = NULL;
			status = typeEnum->NextOne (&typeDef);
		}
		if(typeDef != NULL)
		{
			typeDef->ReleaseReference();
			typeDef = NULL;
		}
		typeEnum->ReleaseReference();
		typeEnum = NULL;
		if(!defFound)
			 RAISE(AAFRESULT_NO_MORE_OBJECTS);
	}
	XEXCEPT
	{
		if(typeEnum != NULL)
			typeEnum->ReleaseReference();
		if(typeDef != NULL)
			typeDef->ReleaseReference();
	}
	XEND
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::GetTypeDefinitions (
      ImplEnumAAFTypeDefs ** ppEnum)
{
	if (NULL == ppEnum)
		return AAFRESULT_NULL_PARAM;
	*ppEnum = 0;
	
	ImplEnumAAFTypeDefs *theEnum = (ImplEnumAAFTypeDefs *)CreateImpl (CLSID_EnumAAFTypeDefs);
	
	XPROTECT()
	{
		CHECK(theEnum->SetEnumStrongProperty(this, &_typeDefinitions));
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
      ImplAAFEffectDef *pEffectDef)
{
	if (NULL == pEffectDef)
		return AAFRESULT_NULL_PARAM;
	
	_effectDefinitions.appendValue(pEffectDef);
	// trr - We are saving a copy of pointer in _pluginDefinitions
	// so we need to bump its reference count.
	pEffectDef->AcquireReference();
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupEffectDefinition (
      aafUID_t *effectID,
      ImplAAFEffectDef **ppEffectDef)
{
	ImplEnumAAFEffectDefs		*effectEnum = NULL;
	ImplAAFEffectDef			*effectDef = NULL;
	aafBool						defFound;
	AAFRESULT					status;
	aafUID_t					testAUID;

	XPROTECT()
	{
		CHECK(GetEffectDefinitions (&effectEnum));
		status = effectEnum->NextOne (&effectDef);
		defFound = AAFFalse;
		while(status == AAFRESULT_SUCCESS && !defFound)
		{
			CHECK(effectDef->GetAUID (&testAUID));
			if(EqualAUID(effectID, &testAUID))
			{
				defFound = AAFTrue;
				*ppEffectDef = effectDef;
				effectDef->AcquireReference();
				break;
			}
			effectDef->ReleaseReference();
			effectDef = NULL;
			status = effectEnum->NextOne (&effectDef);
		}
		if(effectDef != NULL)
		{
			effectDef->ReleaseReference();
			effectDef = NULL;
		}
		effectEnum->ReleaseReference();
		effectEnum = NULL;
		if(!defFound)
			 RAISE(AAFRESULT_NO_MORE_OBJECTS);
	}
	XEXCEPT
	{
		if(effectEnum != NULL)
			effectEnum->ReleaseReference();
		if(effectDef != NULL)
			effectDef->ReleaseReference();
	}
	XEND
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::GetEffectDefinitions (
      ImplEnumAAFEffectDefs **ppEnum)
{
	if (NULL == ppEnum)
		return AAFRESULT_NULL_PARAM;
	*ppEnum = 0;
	
	ImplEnumAAFEffectDefs *theEnum = (ImplEnumAAFEffectDefs *)CreateImpl (CLSID_EnumAAFEffectDefs);
	
	XPROTECT()
	{
		CHECK(theEnum->SetEnumStrongProperty(this, &_effectDefinitions));
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

AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::RegisterParameterDefinition (
      ImplAAFParameterDef *pParameterDef)
{
	if (NULL == pParameterDef)
		return AAFRESULT_NULL_PARAM;
	
	_parameterDefinitions.appendValue(pParameterDef);
	// trr - We are saving a copy of pointer in _pluginDefinitions
	// so we need to bump its reference count.
	pParameterDef->AcquireReference();
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupParameterDefinition (
      aafUID_t *pParameterID,
      ImplAAFParameterDef **ppParameterDef)
{
	ImplEnumAAFParameterDefs		*parameterEnum = NULL;
	ImplAAFParameterDef			*parameterDef = NULL;
	aafBool						defFound;
	AAFRESULT					status;
	aafUID_t					testAUID;

	XPROTECT()
	{
		CHECK(GetParameterDefinitions (&parameterEnum));
		status = parameterEnum->NextOne (&parameterDef);
		defFound = AAFFalse;
		while(status == AAFRESULT_SUCCESS && !defFound)
		{
			CHECK(parameterDef->GetAUID (&testAUID));
			if(EqualAUID(pParameterID, &testAUID))
			{
				defFound = AAFTrue;
				*ppParameterDef = parameterDef;
				parameterDef->AcquireReference();
				break;
			}
			parameterDef->ReleaseReference();
			parameterDef = NULL;
			status = parameterEnum->NextOne (&parameterDef);
		}
		if(parameterDef != NULL)
		{
			parameterDef->ReleaseReference();
			parameterDef = NULL;
		}
		parameterEnum->ReleaseReference();
		parameterEnum = NULL;
		if(!defFound)
			 RAISE(AAFRESULT_NO_MORE_OBJECTS);
	}
	XEXCEPT
	{
		if(parameterEnum != NULL)
			parameterEnum->ReleaseReference();
		if(parameterDef != NULL)
			parameterDef->ReleaseReference();
	}
	XEND
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::GetParameterDefinitions (
      ImplEnumAAFParameterDefs **ppEnum)
{
	if (NULL == ppEnum)
		return AAFRESULT_NULL_PARAM;
	*ppEnum = 0;
	
	ImplEnumAAFParameterDefs *theEnum = (ImplEnumAAFParameterDefs *)CreateImpl (CLSID_EnumAAFParameterDefs);
	
	XPROTECT()
	{
		CHECK(theEnum->SetEnumStrongProperty(this, &_parameterDefinitions));
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
				break;
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



#define REGISTER_BUILTIN(func) \
{ \
  ImplAAFTypeDef * pTD = 0; \
  AAFRESULT hr = func (&pTD); \
  assert (AAFRESULT_SUCCEEDED(hr)); \
  assert (pTD); \
  RegisterType (pTD); \
}


void ImplAAFDictionary::InitBuiltins()
{
  if (_builtinsInited) return;

  // Put built-in types into dictionary.
  REGISTER_BUILTIN(ImplAAFBuiltins::TypeDefAUID);
  REGISTER_BUILTIN(ImplAAFBuiltins::TypeDefUInt8);
  REGISTER_BUILTIN(ImplAAFBuiltins::TypeDefUInt16);
  REGISTER_BUILTIN(ImplAAFBuiltins::TypeDefInt16);
  REGISTER_BUILTIN(ImplAAFBuiltins::TypeDefUInt32);
  REGISTER_BUILTIN(ImplAAFBuiltins::TypeDefInt32);
  REGISTER_BUILTIN(ImplAAFBuiltins::TypeDefInt64);
  REGISTER_BUILTIN(ImplAAFBuiltins::TypeDefObjRef);
  REGISTER_BUILTIN(ImplAAFBuiltins::TypeDefObjRefArray);
  REGISTER_BUILTIN(ImplAAFBuiltins::TypeDefAUIDArray);
  REGISTER_BUILTIN(ImplAAFBuiltins::TypeDefUInt8Array);
  REGISTER_BUILTIN(ImplAAFBuiltins::TypeDefUInt8Array8);
  REGISTER_BUILTIN(ImplAAFBuiltins::TypeDefWCharString);

  _builtinsInited = AAFTrue;
}

OMDEFINE_STORABLE(ImplAAFDictionary, AUID_AAFDictionary);

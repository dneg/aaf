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

#ifndef __ImplAAFPropertyDef_h__
#include "ImplAAFPropertyDef.h"
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

#ifndef __ImplAAFOperationDef_h__
#include "ImplAAFOperationDef.h"
#endif

#ifndef __ImplAAFParameterDef_h__
#include "ImplAAFParameterDef.h"
#endif

#ifndef __ImplEnumAAFOperationDefs_h__
#include "ImplEnumAAFOperationDefs.h"
#endif

#ifndef __ImplEnumAAFCodecDefs_h__
#include "ImplEnumAAFCodecDefs.h"
#endif

#ifndef __ImplEnumAAFPropertyDefs_h__
#include "ImplEnumAAFPropertyDefs.h"
#endif

#ifndef __ImplEnumAAFContainerDefs_h__
#include "ImplEnumAAFContainerDefs.h"
#endif

#ifndef __ImplEnumAAFInterpolationDefs_h__
#include "ImplEnumAAFInterpolationDefs.h"
#endif

#ifndef __AAFTypeDefUIDs_h__
#include "AAFTypeDefUIDs.h"
#endif

#ifndef __ImplAAFBaseClassFactory_h__
#include "ImplAAFBaseClassFactory.h"
#endif

#ifndef __ImplAAFBuiltinClasses_h__
#include "ImplAAFBuiltinClasses.h"
#endif

#ifndef __ImplAAFBuiltinProps_h__
#include "ImplAAFBuiltinProps.h"
#endif

#ifndef __ImplAAFBuiltinTypes_h__
#include "ImplAAFBuiltinTypes.h"
#endif

#include "ImplAAFMob.h"
#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"
#include "ImplAAFObjectCreation.h"


#include <assert.h>
#include <string.h>
#include "aafErr.h"
#include "AAFUtils.h"
#include "AAFDataDefs.h"

extern "C" const aafClassID_t CLSID_EnumAAFClassDefs;
extern "C" const aafClassID_t CLSID_EnumAAFCodecDefs;
extern "C" const aafClassID_t CLSID_EnumAAFContainerDefs;
extern "C" const aafClassID_t CLSID_EnumAAFDataDefs;
extern "C" const aafClassID_t CLSID_EnumAAFDefObjects;
extern "C" const aafClassID_t CLSID_EnumAAFInterpolationDefs;
extern "C" const aafClassID_t CLSID_EnumAAFOperationDefs;
extern "C" const aafClassID_t CLSID_EnumAAFParameterDefs;
extern "C" const aafClassID_t CLSID_EnumAAFPluginDescriptors;
extern "C" const aafClassID_t CLSID_EnumAAFTypeDefs;



ImplAAFDictionary::ImplAAFDictionary ()
: _operationDefinitions(PID_Dictionary_OperationDefinitions, "OperationDefinitions"), 
  _parameterDefinitions(PID_Dictionary_ParameterDefinitions, "ParameterDefinitions"),
  _codecDefinitions(PID_Dictionary_CodecDefinitions, "CodecDefinitions"),
  _containerDefinitions(PID_Dictionary_ContainerDefinitions, "ContainerDefinitions"),
  _typeDefinitions      (PID_Dictionary_TypeDefinitions,      "TypeDefinitions"),
  _classDefinitions      (PID_Dictionary_ClassDefinitions,    "ClassDefinitions"),
  _interpolationDefinitions      (PID_Dictionary_InterpolationDefinitions,    "InterpolationDefinitions"),
  _dataDefinitions      (PID_Dictionary_DataDefinitions,    "DataDefinitions"),
  _pluginDefinitions      (PID_Dictionary_PluginDefinitions,    "PluginDefinitions"),

  _pBuiltinClasses (0),
  _pBuiltinProps (0),
  _pBuiltinTypes (0),

  _initStarted   (AAFFalse),
  _OKToInitProps (AAFFalse)
{
  _persistentProperties.put (_operationDefinitions.address());
  _persistentProperties.put (_parameterDefinitions.address());
  _persistentProperties.put (_typeDefinitions.address());
  _persistentProperties.put (_classDefinitions.address());
  _persistentProperties.put(_codecDefinitions.address());
  _persistentProperties.put(_containerDefinitions.address());
  _persistentProperties.put(_interpolationDefinitions.address());
  _persistentProperties.put(_dataDefinitions.address());
  _persistentProperties.put(_pluginDefinitions.address());

  _pBuiltinClasses = new ImplAAFBuiltinClasses (this);
  _pBuiltinProps   = new ImplAAFBuiltinProps (this);
  _pBuiltinTypes   = new ImplAAFBuiltinTypes (this);
}


ImplAAFDictionary::~ImplAAFDictionary ()
{
  size_t i;
  // Release the _codecDefinitions
  size_t size = _codecDefinitions.getSize();
  for (i = 0; i < size; i++)
	{
		ImplAAFCodecDef *pCodec = _codecDefinitions.setValueAt(0, i);
		if (pCodec)
		{
		  pCodec->ReleaseReference();
		}
	}

	size_t containerDefSize = _containerDefinitions.getSize();
	for (i = 0; i < containerDefSize; i++)
	{
		ImplAAFContainerDef *pContainer = _containerDefinitions.setValueAt(0, i);
		if (pContainer)
		{
		  pContainer->ReleaseReference();
		}
	}

	size_t typeDefSize = _typeDefinitions.getSize();
	for (i = 0; i < typeDefSize; i++)
	{
		ImplAAFTypeDef *pType = _typeDefinitions.setValueAt(0, i);
		if (pType)
		{
		  pType->ReleaseReference();
		}
	}

  size_t classDefSize = _classDefinitions.getSize();
  for (i = 0; i < classDefSize; i++)
	{
	  ImplAAFClassDef *pClass = _classDefinitions.setValueAt(0, i);
	  if (pClass)
		{
		  pClass->ReleaseReference();
		}
	}
  size_t opsDefSize = _operationDefinitions.getSize();
  for (i = 0; i < opsDefSize; i++)
	{
	  ImplAAFOperationDef *pOps = _operationDefinitions.setValueAt(0, i);
	  if (pOps)
		{
		  pOps->ReleaseReference();
		}
	}
  size_t parmDefSize = _parameterDefinitions.getSize();
  for (i = 0; i < parmDefSize; i++)
	{
	  ImplAAFParameterDef *pParm = _parameterDefinitions.setValueAt(0, i);
	  if (pParm)
		{
		  pParm->ReleaseReference();
		}
	}
  size_t interpDefSize = _interpolationDefinitions.getSize();
  for (i = 0; i < interpDefSize; i++)
	{
	  ImplAAFInterpolationDef *pInterp = _interpolationDefinitions.setValueAt(0, i);
	  if (pInterp)
		{
		  pInterp->ReleaseReference();
		}
	}

  size_t pluginDescSize = _pluginDefinitions.getSize();
  for (i = 0; i < pluginDescSize; i++)
	{
	  ImplAAFPluginDescriptor *pDesc = _pluginDefinitions.setValueAt(0, i);
	  if (pDesc)
		{
		  pDesc->ReleaseReference();
		}
	}

  size_t dataDefSize = _dataDefinitions.getSize();
  for (i = 0; i < dataDefSize; i++)
	{
	  ImplAAFDataDef *pDDef = _dataDefinitions.setValueAt(0, i);
	  if (pDDef)
		{
		  pDDef->ReleaseReference();
		}
	}

  assert (_pBuiltinClasses);
  delete _pBuiltinClasses;

  assert (_pBuiltinProps);
  delete _pBuiltinProps;

  assert (_pBuiltinTypes);
  delete _pBuiltinTypes;
}



//
// Factory function for all built-in classes.
//
/*static*/ ImplAAFObject *
ImplAAFDictionary::pvtCreateBaseClassInstance(const aafUID_t* pAUID) 
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


// Factory method for creating a Dictionary.
ImplAAFDictionary *ImplAAFDictionary::CreateDictionary(void)
{
  // Create a dictionary.
  ImplAAFDictionary * pDictionary = NULL;
  
  
  pDictionary = static_cast<ImplAAFDictionary *>(pvtCreateBaseClassInstance(&AUID_AAFDictionary));
  assert(NULL != pDictionary);
  if (NULL != pDictionary)
  {
    // If we created a dictionary then give it a reference to a factory
    // (dictionary) to satisfy the OMStorable class invariant: Every OMStorabe
    // must have a reference to a factory. Since the dictionary is not created
    // by the OMClassFactory interface we just set the factory to "itself".
    //
    pDictionary->setClassFactory(pDictionary);
  }
  
  return pDictionary;
}


//
// Create an instance of the appropriate derived class, given the class id.
//  This method implements the OMClassFactory interface.
//
OMStorable* ImplAAFDictionary::create(const OMClassId& classId) const
{
  const aafUID_t* pAUID  = reinterpret_cast<const aafUID_t*>(&classId);
  OMStorable *result = 0;

  // Is this a request to create the dictionary?
  if (memcmp(pAUID, &AUID_AAFDictionary, sizeof(aafUID_t)) == 0)
  { // The result is just this instance.
    result = const_cast<ImplAAFDictionary*>(this);
    // Bump the reference count.
    AcquireReference();
  } 
  else
  { // Create an instance of the class corresponsing to the given
    // stored object id.

    // Try the built-in dictionary first.
    //
    result = pvtCreateBaseClassInstance(pAUID);
    if (result == 0)
    {
      // Not in the built-in dictionary, try the current dictionary.
      // TBD
      //
    }
  }

  if (NULL != result && ((ImplAAFDictionary *)result != this))
	{
	  // If we created an object then give it a reference to the
	  // factory (dictionary) that was used to created it.
	  //
	  result->setClassFactory(this);
	}

  // Initialize the OM properties on this object.
  //
  ImplAAFObjectSP pObj;
  pObj = static_cast<ImplAAFObject*>(result);
  assert (pObj);
  pvtInitObjectProperties (pObj);

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


// internal utility factory method to create an ImplAAFObject given an auid.
// This method was created to make it simpler to replace calls to "Deprecated"
// call to CreateImpl which should only be used for instanciating transient
// non-ImplAAFObject classes such as an enumerator.
ImplAAFObject *ImplAAFDictionary::CreateImplObject(const aafUID_t& auid)
{
  ImplAAFObject *pObject = NULL;
  AAFRESULT result = AAFRESULT_SUCCESS;
  
  result = CreateInstance(const_cast<aafUID_t *>(&auid), &pObject);
  assert(AAFRESULT_SUCCEEDED(result));
  return pObject;
}



AAFRESULT ImplAAFDictionary::dictLookupClass (
      const aafUID_t * pClassID,
      ImplAAFClassDef ** ppClassDef)
{
  if (!pClassID) return AAFRESULT_NULL_PARAM;
  if (!ppClassDef) return AAFRESULT_NULL_PARAM;

  ImplEnumAAFClassDefs		*classEnum = NULL;
  ImplAAFClassDef			*classDef = NULL;
  aafBool						defFound;
  AAFRESULT					status;
  aafUID_t					testAUID;

  if (! ppClassDef)
	return AAFRESULT_NULL_PARAM;

  XPROTECT()
	{
	  CHECK(GetClassDefinitions (&classEnum));
	  status = classEnum->NextOne (&classDef);
	  defFound = AAFFalse;
	  while(status == AAFRESULT_SUCCESS && !defFound)
		{
		  CHECK(classDef->GetAUID (&testAUID));
		  if(EqualAUID(pClassID, &testAUID))
			{
			  defFound = AAFTrue;
			  *ppClassDef = classDef;
			  classDef->AcquireReference();
			  break;
			}
		  classDef->ReleaseReference();
		  classDef = NULL;
		  status = classEnum->NextOne (&classDef);
		}
	  if(classDef != NULL)
		{
		  classDef->ReleaseReference();
		  classDef = NULL;
		}
	  classEnum->ReleaseReference();
	  classEnum = NULL;
	  if(!defFound)
		{
		  // no recognized class guid in dictionary
		  RAISE(AAFRESULT_NO_MORE_OBJECTS);
		}
	}
  XEXCEPT
	{
	  if(classEnum != NULL)
		classEnum->ReleaseReference();
	  if(classDef != NULL)
		classDef->ReleaseReference();
	}
  XEND
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupClass (
      const aafUID_t * pClassID,
      ImplAAFClassDef ** ppClassDef)
{
  ImplAAFClassDef			*classDef = NULL;
  AAFRESULT					status;

  // First check to see if it's already in the dict
  status = dictLookupClass (pClassID, ppClassDef);
  if (AAFRESULT_SUCCEEDED (status))
	{
	  // Yup, already here. Pass on the good news.
	  return status;
	}

  // Not found in the dict; check to see if it was due to failure
  // other than not being found
  if (AAFRESULT_NO_MORE_OBJECTS != status)
	{
	  // Yup, some other failure. Pass on the bad news.
	  return status;
	}

  // If we're here, it was not found in dict.  Try it in builtins.
  status = _pBuiltinClasses->ImportBuiltinClassDef (*pClassID, &classDef);
  if (AAFRESULT_FAILED (status))
	{
	  // no recognized class guid
	  return AAFRESULT_NO_MORE_OBJECTS;
	}
  assert (classDef);

  // Found it in builtins, and created a new class def object.  It
  // doesn't currently have any properties defined, so we should hook
  // it up.
  _pBuiltinProps->RegisterBuiltinProperties (classDef);
		  
  assert (ppClassDef);
  // addRef of *ppClassDef matches releaseref of classdef
  *ppClassDef = classDef;  // so it doesn't get released later

  // No need to release; we'll pass the addref on to the client.
  classDef = 0;
	
  return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::RegisterClass (
      ImplAAFClassDef * pClassDef)
{
  if (NULL == pClassDef)
	return AAFRESULT_NULL_PARAM;
	
  // Get the AUID of the new class to register.
  aafUID_t newAUID;
  HRESULT hr = pClassDef->GetAUID(&newAUID);
  if (hr != AAFRESULT_SUCCESS)
    return hr;

  // Is this class already registered ?
  ImplAAFClassDef * pExistingClassDef = NULL;
  hr = dictLookupClass(&newAUID, &pExistingClassDef);

  if (hr != AAFRESULT_SUCCESS)
	{
	  // This class is not yet registered, add it to the dictionary.
	  _classDefinitions.appendValue(pClassDef);
	  pClassDef->AcquireReference();
	}
  else
	{
	  // This class is already registered, probably because it was
	  // already in the persisted dictionary.
	  pExistingClassDef->ReleaseReference();
	}
  return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::GetClassDefinitions (
      ImplEnumAAFClassDefs ** ppEnum)
{
  if (NULL == ppEnum)
	return AAFRESULT_NULL_PARAM;
  *ppEnum = 0;
	
  ImplEnumAAFClassDefs *theEnum = (ImplEnumAAFClassDefs *)CreateImpl (CLSID_EnumAAFClassDefs);
	
  XPROTECT()
	{
	  CHECK(theEnum->SetEnumStrongProperty(this, &_classDefinitions));
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
    ImplAAFDictionary::RegisterType (
      ImplAAFTypeDef * pTypeDef)
{
  if (NULL == pTypeDef)
	return AAFRESULT_NULL_PARAM;
	
  // Get the AUID of the new type to register.
  aafUID_t newAUID;
  HRESULT hr = pTypeDef->GetAUID(&newAUID);
  if (hr != AAFRESULT_SUCCESS)
    return hr;

  // Is this type already registered ?
  ImplAAFTypeDef * pExistingTypeDef = NULL;
  hr = dictLookupType(&newAUID, &pExistingTypeDef);

  if (hr != AAFRESULT_SUCCESS) {
    // This type is not yet registered, add it to the dictionary.
    _typeDefinitions.appendValue(pTypeDef);
    pTypeDef->AcquireReference();
    // Set up the (non-persistent) dictionary pointer.
	//
    // BobT 6/15/99: Remove ImplAAFDefObject::Get/SetDict() in favor of
	// ImplAAFObject::GetDictionary().
	// pTypeDef->SetDict(this);
  } else {
    // This type is already registered, probably because it was
    // already in the persisted dictionary.
    // Set up the (non-persistent) dictionary pointer.
	//
    // BobT 6/15/99: Remove ImplAAFDefObject::Get/SetDict() in favor of
	// ImplAAFObject::GetDictionary().
	// pExistingTypeDef->SetDict(this);
    pExistingTypeDef->ReleaseReference();
  }

  return(AAFRESULT_SUCCESS);
}


AAFRESULT ImplAAFDictionary::dictLookupType (
      const aafUID_t *  pTypeID,
      ImplAAFTypeDef ** ppTypeDef)
{
  ImplEnumAAFTypeDefs		*typeEnum = NULL;
  ImplAAFTypeDef			*typeDef = NULL;
  aafBool						defFound;
  AAFRESULT					status;
  aafUID_t					testAUID;

  if (! ppTypeDef)
	return AAFRESULT_NULL_PARAM;

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
		{
		  // Not found in dictionary.  Look in builtins.
		  assert (0 == typeDef);
		  // no recognized type guid
		  RAISE(AAFRESULT_NO_MORE_OBJECTS);
		}
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
    ImplAAFDictionary::LookupType (
      const aafUID_t *  pTypeID,
      ImplAAFTypeDef ** ppTypeDef)
{
  ImplAAFTypeDef			*typeDef = NULL;
  AAFRESULT					status;

  // First check to see if it's already in the dict
  status = dictLookupType (pTypeID, ppTypeDef);
  if (AAFRESULT_SUCCEEDED (status))
	{
	  // Yup, already here. Pass on the good news.
	  return status;
	}

  // Not found in the dict; check to see if it was due to failure
  // other than not being found
  if (AAFRESULT_NO_MORE_OBJECTS != status)
	{
	  // Yup, some other failure. Pass on the bad news.
	  return status;
	}

  // If we're here, it was not found in dict.  Try it in builtins.
  assert (0 == typeDef);
  status = _pBuiltinTypes->ImportBuiltinTypeDef (*pTypeID, &typeDef);
  if (AAFRESULT_FAILED (status))
	{
	  // no recognized type guid
	  return AAFRESULT_NO_MORE_OBJECTS;
	}
  assert (typeDef);
  assert (ppTypeDef);
  // addRef of *ppTypeDef matches releaseref of typedef
  *ppTypeDef = typeDef;

  // No need to release; we'll pass the addref on to the client.
  typeDef = 0;
	
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
      ImplAAFDataDef *pDataDef)
{
  if (NULL == pDataDef)
	return AAFRESULT_NULL_PARAM;
	
  // Get the AUID of the new type to register.
  aafUID_t newAUID;
  HRESULT hr = pDataDef->GetAUID(&newAUID);
  if (hr != AAFRESULT_SUCCESS)
    return hr;

  // Is this type already registered ?
  ImplAAFDataDef * pExistingDataDef = NULL;
  hr = LookupDataDefintion(&newAUID, &pExistingDataDef);

  if (hr != AAFRESULT_SUCCESS) {
    // This type is not yet registered, add it to the dictionary.
    _dataDefinitions.appendValue(pDataDef);
    pDataDef->AcquireReference();
    // Set up the (non-persistent) dictionary pointer.
	//
    // BobT 6/15/99: Remove ImplAAFDefObject::Get/SetDict() in favor of
	// ImplAAFObject::GetDictionary().
	// pDataDef->SetDict(this);
  } else {
    // This type is already registered, probably because it was
    // already in the persisted dictionary.
    // Set up the (non-persistent) dictionary pointer.
	//
    // BobT 6/15/99: Remove ImplAAFDefObject::Get/SetDict() in favor of
	// ImplAAFObject::GetDictionary().
	// pExistingDataDef->SetDict(this);
    pExistingDataDef->ReleaseReference();
  }

  return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupDataDefintion (
      aafUID_t *pDataDefintionID,
      ImplAAFDataDef **ppDataDef)
{
	ImplEnumAAFDataDefs		*dataEnum = NULL;
	ImplAAFDataDef			*dataDef = NULL;
	aafBool						defFound;
	AAFRESULT					status;
	aafUID_t					testAUID;

	XPROTECT()
	{
		CHECK(GetDataDefinitions (&dataEnum));
		status = dataEnum->NextOne (&dataDef);
		defFound = AAFFalse;
		while(status == AAFRESULT_SUCCESS && !defFound)
		{
			CHECK(dataDef->GetAUID (&testAUID));
			if(EqualAUID(pDataDefintionID, &testAUID))
			{
				defFound = AAFTrue;
				*ppDataDef = dataDef;
				dataDef->AcquireReference();
				break;
			}
			dataDef->ReleaseReference();
			dataDef = NULL;
			status = dataEnum->NextOne (&dataDef);
		}
		if(dataDef != NULL)
		{
			dataDef->ReleaseReference();
			dataDef = NULL;
		}
		dataEnum->ReleaseReference();
		dataEnum = NULL;
		if(!defFound)
			 RAISE(AAFRESULT_NO_MORE_OBJECTS);
	}
	XEXCEPT
	{
		if(dataEnum != NULL)
			dataEnum->ReleaseReference();
		if(dataDef != NULL)
			dataDef->ReleaseReference();
	}
	XEND
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::GetDataDefinitions (
      ImplEnumAAFDataDefs **ppEnum)
{
	if (NULL == ppEnum)
		return AAFRESULT_NULL_PARAM;
	*ppEnum = 0;
	
	ImplEnumAAFDataDefs *theEnum = (ImplEnumAAFDataDefs *)CreateImpl (CLSID_EnumAAFDataDefs);
	
	XPROTECT()
	{
		CHECK(theEnum->SetEnumStrongProperty(this, &_dataDefinitions));
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
    ImplAAFDictionary::RegisterOperationDefinition (
      ImplAAFOperationDef *pOperationDef)
{
	if (NULL == pOperationDef)
		return AAFRESULT_NULL_PARAM;

	_operationDefinitions.appendValue(pOperationDef);
	// trr - We are saving a copy of pointer in _pluginDefinitions
	// so we need to bump its reference count.
	pOperationDef->AcquireReference();
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupOperationDefinition (
      aafUID_t *effectID,
      ImplAAFOperationDef **ppOperationDef)
{
	ImplEnumAAFOperationDefs	*effectEnum = NULL;
	ImplAAFOperationDef			*effectDef = NULL;
	aafBool						defFound;
	AAFRESULT					status;
	aafUID_t					testAUID;

	XPROTECT()
	{
		CHECK(GetOperationDefinitions (&effectEnum));
		status = effectEnum->NextOne (&effectDef);
		defFound = AAFFalse;
		while(status == AAFRESULT_SUCCESS && !defFound)
		{
			CHECK(effectDef->GetAUID (&testAUID));
			if(EqualAUID(effectID, &testAUID))
			{
				defFound = AAFTrue;
				*ppOperationDef = effectDef;
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
    ImplAAFDictionary::GetOperationDefinitions (
      ImplEnumAAFOperationDefs **ppEnum)
{
	if (NULL == ppEnum)
		return AAFRESULT_NULL_PARAM;
	*ppEnum = 0;
	
	ImplEnumAAFOperationDefs *theEnum = (ImplEnumAAFOperationDefs *)CreateImpl (CLSID_EnumAAFOperationDefs);
	
	XPROTECT()
	{
		CHECK(theEnum->SetEnumStrongProperty(this, &_operationDefinitions));
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
    ImplAAFDictionary::RegisterCodecDefinition (
      ImplAAFCodecDef *pPlugDef)
{
	if (NULL == pPlugDef)
		return AAFRESULT_NULL_PARAM;

	_codecDefinitions.appendValue(pPlugDef);
	// trr - We are saving a copy of pointer in _pluginDefinitions
	// so we need to bump its reference count.
	pPlugDef->AcquireReference();
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT
    ImplAAFDictionary::GetNumCodecDefs(aafUInt32 *  pNumCodecDefs)
{
  size_t siz;

  if(pNumCodecDefs == NULL)
    return AAFRESULT_NULL_PARAM;
  
  _codecDefinitions.getSize(siz);
  
  *pNumCodecDefs = siz;
  return AAFRESULT_SUCCESS;
}

AAFRESULT
    ImplAAFDictionary::GetNthCodecDef (aafInt32 index, ImplAAFCodecDef **ppPluggableDefs)
{
  if (NULL == ppPluggableDefs)
    return AAFRESULT_NULL_PARAM;

  ImplAAFCodecDef *obj = NULL;
  _codecDefinitions.getValueAt(obj, index);
  *ppPluggableDefs = obj;
	
  // trr - We are returning a copy of pointer stored in _mobs so we need
  // to bump its reference count.
  if (obj)
    obj->AcquireReference();
  else
    return AAFRESULT_NO_MORE_OBJECTS;

  return AAFRESULT_SUCCESS;
}

AAFRESULT ImplAAFDictionary::LookupCodecDefinition(aafUID_t *defID, ImplAAFCodecDef **result)
{
	ImplAAFCodecDef				*codec = NULL;
	aafBool						defFound;
	aafUID_t					testAUID;
	aafUInt32					n, numCodecs;

	XPROTECT()
	{
		*result = NULL;
		CHECK(GetNumCodecDefs(&numCodecs));
		defFound = AAFFalse;
		for(n = 0; n < numCodecs && !defFound; n++)
		{
			CHECK(GetNthCodecDef (n, &codec));
			CHECK(codec->GetAUID (&testAUID));
			if(EqualAUID(defID, &testAUID))
			{
				defFound = AAFTrue;
				*result = codec;
				codec->AcquireReference();
				break;
			}
			codec->ReleaseReference();
			codec = NULL;
		}
		if(codec != NULL)
		{
			codec->ReleaseReference();
			codec = NULL;
		}
		if(!defFound)
			 RAISE(AAFRESULT_NO_MORE_OBJECTS);
	}
	XEXCEPT
	{
		if(codec != NULL)
			codec->ReleaseReference();
	}
	XEND
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::GetCodecDefinitions (
      ImplEnumAAFCodecDefs **ppEnum)
{
	if (NULL == ppEnum)
		return AAFRESULT_NULL_PARAM;
	*ppEnum = 0;
	
	ImplEnumAAFCodecDefs *theEnum = (ImplEnumAAFCodecDefs *)CreateImpl (CLSID_EnumAAFCodecDefs);
	
	XPROTECT()
	{
		CHECK(theEnum->SetEnumStrongProperty(this, &_codecDefinitions));
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
    ImplAAFDictionary::RegisterContainerDefinition (
      ImplAAFContainerDef *pPlugDef)
{
	if (NULL == pPlugDef)
		return AAFRESULT_NULL_PARAM;

	_containerDefinitions.appendValue(pPlugDef);
	// trr - We are saving a copy of pointer in _pluginDefinitions
	// so we need to bump its reference count.
	pPlugDef->AcquireReference();
	
	return(AAFRESULT_SUCCESS);
}

AAFRESULT ImplAAFDictionary::LookupContainerDefinition(aafUID_t *defID, ImplAAFContainerDef **result)
{
	ImplAAFContainerDef			*container = NULL;
	aafBool						defFound;
	aafUID_t					testAUID;
	aafUInt32					n, numContainers;

	XPROTECT()
	{
		*result = NULL;
		CHECK(GetNumContainerDefs(&numContainers));
		defFound = AAFFalse;
		for(n = 0; n < numContainers && !defFound; n++)
		{
			CHECK(GetNthContainerDef (n, &container));
			CHECK(container->GetAUID (&testAUID));
			if(EqualAUID(defID, &testAUID))
			{
				defFound = AAFTrue;
				*result = container;
				container->AcquireReference();
				break;
			}
			container->ReleaseReference();
			container = NULL;
		}
		if(container != NULL)
		{
			container->ReleaseReference();
			container = NULL;
		}
		if(!defFound)
			 RAISE(AAFRESULT_NO_MORE_OBJECTS);
	}
	XEXCEPT
	{
		if(container != NULL)
			container->ReleaseReference();
	}
	XEND
	
	return(AAFRESULT_SUCCESS);
}

AAFRESULT
    ImplAAFDictionary::GetNumContainerDefs(aafUInt32 *  pNumContainerDefs)
{
  size_t siz;

  if(pNumContainerDefs == NULL)
    return AAFRESULT_NULL_PARAM;
  
  _containerDefinitions.getSize(siz);
  
  *pNumContainerDefs = siz;
  return AAFRESULT_SUCCESS;
}

AAFRESULT
    ImplAAFDictionary::GetNthContainerDef (aafInt32 index, ImplAAFContainerDef **ppPluggableDefs)
{
  if (NULL == ppPluggableDefs)
    return AAFRESULT_NULL_PARAM;

  ImplAAFContainerDef *obj = NULL;
  _containerDefinitions.getValueAt(obj, index);
  *ppPluggableDefs = obj;
	
  // trr - We are returning a copy of pointer stored in _mobs so we need
  // to bump its reference count.
  if (obj)
    obj->AcquireReference();
  else
    return AAFRESULT_NO_MORE_OBJECTS;

  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::GetContainerDefinitions (
      ImplEnumAAFContainerDefs **ppEnum)
{
	if (NULL == ppEnum)
		return AAFRESULT_NULL_PARAM;
	*ppEnum = 0;
	
	ImplEnumAAFContainerDefs *theEnum = (ImplEnumAAFContainerDefs *)CreateImpl (CLSID_EnumAAFContainerDefs);
	
	XPROTECT()
	{
		CHECK(theEnum->SetEnumStrongProperty(this, &_containerDefinitions));
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


void ImplAAFDictionary::InitBuiltins()
{
  ImplAAFDataDef	*dataDef = NULL;
  AAFRESULT		hr;
  aafUID_t		uid;

  
  uid = DDEF_Picture;
  hr = LookupDataDefintion (&uid, &dataDef);
  if (AAFRESULT_FAILED (hr))
	{
	  // not already in dictionary
	  hr = CreateInstance (&AUID_AAFDataDef,
						   (ImplAAFObject **)&dataDef);
	  hr = dataDef->Init (&uid, L"Picture", L"Picture data");
	  hr = RegisterDataDefintion (dataDef);
	}
  dataDef->ReleaseReference();
  dataDef = NULL;

  uid = DDEF_Sound;
  hr = LookupDataDefintion (&uid, &dataDef);
  if (AAFRESULT_FAILED (hr))
	{
	  // not already in dictionary
	  hr = CreateInstance (&AUID_AAFDataDef,
						   (ImplAAFObject **)&dataDef);
	  hr = dataDef->Init (&uid, L"Sound", L"Sound data");
	  hr = RegisterDataDefintion (dataDef);
	}
  dataDef->ReleaseReference();
  dataDef = NULL;

  uid = DDEF_Timecode;
  hr = LookupDataDefintion (&uid, &dataDef);
  if (AAFRESULT_FAILED (hr))
	{
	  // not already in dictionary
	  hr = CreateInstance (&AUID_AAFDataDef,
						   (ImplAAFObject **)&dataDef);
	  hr = dataDef->Init (&uid, L"Timecode", L"Timecode data");
	  hr = RegisterDataDefintion (dataDef);
	}
  dataDef->ReleaseReference();
  dataDef = NULL;

  uid = DDEF_Edgecode;
  hr = LookupDataDefintion (&uid, &dataDef);
  if (AAFRESULT_FAILED (hr))
	{
	  // not already in dictionary
	  hr = CreateInstance (&AUID_AAFDataDef,
						   (ImplAAFObject **)&dataDef);
	  hr = dataDef->Init (&uid, L"Edgecode", L"Edgecode data");
	  hr = RegisterDataDefintion (dataDef);
	}
  dataDef->ReleaseReference();
  dataDef = NULL;
  
  uid = DDEF_Matte;
  hr = LookupDataDefintion (&uid, &dataDef);
  if (AAFRESULT_FAILED (hr))
	{
	  // not already in dictionary
	  hr = CreateInstance (&AUID_AAFDataDef,
						   (ImplAAFObject **)&dataDef);
	  hr = dataDef->Init (&uid, L"Matte", L"Matte data");
	  hr = RegisterDataDefintion (dataDef);
	}
  dataDef->ReleaseReference();
  dataDef = NULL;

  uid = DDEF_PictureWithMatte;
  hr = LookupDataDefintion (&uid, &dataDef);
  if (AAFRESULT_FAILED (hr))
	{
	  // not already in dictionary
	  hr = CreateInstance (&AUID_AAFDataDef,
						   (ImplAAFObject **)&dataDef);
	  hr = dataDef->Init (&uid, L"PictureWithMatte", L"PictureWithMatte data");
	  hr = RegisterDataDefintion (dataDef);
	}
  dataDef->ReleaseReference();
  dataDef = NULL;
}

#define check_result(result)     \
  if (AAFRESULT_FAILED (result)) \
	return result;


/* Note!  Will modify argument... */
#define release_if_set(pIfc)     \
{                                \
  if (pIfc)                      \
	{                            \
	  pIfc->ReleaseReference (); \
	  pIfc = 0;                  \
	}                            \
}

/* Note!  Will modify argument... */
#define release_and_zero(pIfc)   \
{                                \
  assert (pIfc);                 \
  pIfc->ReleaseReference ();     \
  pIfc = 0;                      \
}


//
// Looks up a property def by OMPropertyId.  Looks first for classes
// currently registered in the dictionary; looks next in builtin
// classes which may not yet be registered.  If found in builtins,
// will register that class.
//
AAFRESULT
ImplAAFDictionary::LookupPropDefByOMPid
(
 OMPropertyId opid,
 ImplAAFPropertyDef ** ppd
 ) const
{
  AAFRESULT hr;

  // This call may fail if a legal OMPropertyId is specified which
  // corresponds to a prop in a class which is not yet in the
  // dictionary.
  hr = pvtLookupPropDefByOMPid (opid, ppd);
  if (AAFRESULT_SUCCEEDED (hr))
	{
	  // The given OMPropertyId was found.  Return, passing on the
	  // good news.
	  return hr;
	}

  assert (AAFRESULT_FAILED (hr));
  if (AAFRESULT_NO_MORE_OBJECTS != hr)
	{
	  // The lookup failed, but for a reason other than not finding it
	  // in the dictionary.  We can't recover from this.  Return,
	  // passing on the bad news.
	  return hr;
	}

  // Couldn't find it in dict.  Ask builtins to look for it, create
  // it, and register it if it's there.
  aafUID_t classAUID;
  assert (_pBuiltinProps);
  hr = _pBuiltinProps->LookupBuiltinClassFromOMPid (opid, &classAUID);
  if (AAFRESULT_FAILED (hr))
	{
	  // Couldn't find it in builtins.  Can't go any further, so pass
	  // on the bad news.
	  return hr;
	}

  // Found it, and the auid corresponding to the class which includes
  // this pid is in ClassID.
  ImplAAFClassDefSP pClassDef;
  assert (_pBuiltinClasses);
  hr = _pBuiltinClasses->ImportBuiltinClassDef (classAUID, &pClassDef);
  assert (AAFRESULT_SUCCEEDED (hr));
  assert (pClassDef);

  // Created a new class def.  It doesn't currently have any
  // properties defined, so we should hook it up.
  _pBuiltinProps->RegisterBuiltinProperties (pClassDef);

  // *Now* we can look up that OM PID in the dictionary.  This time it
  // had better succeed.
  hr = pvtLookupPropDefByOMPid (opid, ppd);

  // Whether it passed or failed, pass on the result.  We're done
  // either way.
  return hr;
}


//
// Private method to lookup a property def by OMPropertyId.  Will only
// look for that OMPropertyId in classes currently registered in the
// dictionary.
//
AAFRESULT ImplAAFDictionary::pvtLookupPropDefByOMPid
(
 OMPropertyId opid,
 ImplAAFPropertyDef ** ppd
 ) const
{
  if (! ppd) return AAFRESULT_NULL_PARAM;

  ImplEnumAAFClassDefsSP pClassEnum;
  ImplAAFClassDefSP pClassDef;

  //
  // Property defs are owned by class defs, so we'll have to iterate
  // across class defs first, and then within the classes' property
  // defs... 
  //
  check_result (((ImplAAFDictionary*)this)->GetClassDefinitions (&pClassEnum));
  assert (pClassEnum);

  while (AAFRESULT_SUCCEEDED (pClassEnum->NextOne (&pClassDef)))
	{
	  // We've got a class.  Ask it to look up the prop def.
	  assert (pClassDef);
	  assert (ppd);
	  AAFRESULT hr = pClassDef->LookupPropertyDefbyOMPid (opid, ppd);
	  if (AAFRESULT_SUCCEEDED (hr))
		{
		  // We've found the prop def!
		  assert (ppd);
		  assert (*ppd);

		  return AAFRESULT_SUCCESS;
		}
	  // Not in that class; try the next class.  First, get rid of
	  // this class.
	}

  // Class enumerator failed.  If we're here, no more classes
  // registered in the dictionary.  Indicate failure.
  return AAFRESULT_NO_MORE_OBJECTS;
}


//
// Looks up a property def by OMPropertyId.  Looks first for
// properties in built-in types (to avoid recursive dictionary
// lookups).  That's OK since for built-in types the OMPID->TypeGuid
// mapping won't change even with client extensions.
//
// If that OM PID is not found in builtins, will look next in
// registered classes.
//
AAFRESULT
ImplAAFDictionary::LookupPropTypeByOMPid
(
 OMPropertyId opid,
 ImplAAFTypeDef ** ppTypeDef
 ) const
{
  AAFRESULT hr;

  aafUID_t typeAUID;
  assert (_pBuiltinProps);
  hr = _pBuiltinProps->LookupBuiltinPropTypeFromOMPid
	(opid, &typeAUID);
  if (AAFRESULT_SUCCEEDED (hr))
	{
	  // Found a built-in matching that OM PID.  Look up its type.
	  // Cast away constness
	  assert (ppTypeDef);
	  hr = ((ImplAAFDictionary*)this)->LookupType
		(&typeAUID, ppTypeDef);
	  assert (AAFRESULT_SUCCEEDED (hr));
	  assert (ppTypeDef);
	  assert (*ppTypeDef);
	  return AAFRESULT_SUCCESS;
	}

  // Didn't find it in builtins.  Look for it in registered class in
  // the dictionary.
  hr = pvtLookupPropTypeByOMPid (opid, ppTypeDef);
  // Pass or fail, return the result.
  return hr;
}


//
// Private method to lookup a property def by OMPropertyId.  Will only
// look for that OMPropertyId in classes currently registered in the
// dictionary.
//
AAFRESULT ImplAAFDictionary::pvtLookupPropTypeByOMPid
(
 OMPropertyId opid,
 ImplAAFTypeDef ** ppTypeDef
 ) const
{
  if (! ppTypeDef) return AAFRESULT_NULL_PARAM;

  ImplEnumAAFClassDefsSP pClassEnum;
  ImplAAFClassDefSP pClassDef;
  ImplEnumAAFPropertyDefsSP pPropEnum;
  ImplAAFPropertyDefSP pPropDef;

  //
  // Property defs are owned by class defs, so we'll have to iterate
  // across class defs first, and then within the classes' property
  // defs... 
  //
  check_result (((ImplAAFDictionary*)this)->GetClassDefinitions (&pClassEnum));
  assert (pClassEnum);

  while (AAFRESULT_SUCCEEDED (pClassEnum->NextOne (&pClassDef)))
	{
	  // We've got a class.  Iterate across its prop defs.
	  assert (pClassDef);
	  check_result (pClassDef->GetPropertyDefs (&pPropEnum));
	  assert (pPropEnum);

	  while (AAFRESULT_SUCCEEDED (pPropEnum->NextOne (&pPropDef)))
		{
		  // We've got a property definition.  See if it matches
		  // the given OMPropertyId

		  assert (pPropDef);
		  OMPropertyId omPid = pPropDef->OmPid ();
		  if (opid == omPid)
			{
			  // We've found the prop def!
			  assert (pPropDef);
			  assert (ppTypeDef);
			  check_result (pPropDef->GetTypeDef (ppTypeDef));
			  assert (*ppTypeDef);

			  return AAFRESULT_SUCCESS;
			}
		  else
			{
			  // nope, this prop def isn't the one.
			  pPropDef = 0;
			}
		}

	  // Property enumerator failed.  If we're here, no more
	  // properties in this class.  Try the next class.
	  //
	  // Make sure the pPropEnum->NextOne() call didn't get us a
	  // property (pPropDef should have been zeroed the previous
	  // successful time through the loop).
	  assert (!pPropDef);
	}

  // Class enumerator failed.  If we're here, no more classes
  // registered in the dictionary.  Indicate failure.
  return AAFRESULT_NO_MORE_OBJECTS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::RegisterInterpolationDefinition (
      ImplAAFInterpolationDef *pInterpolationDef)
{
	if (NULL == pInterpolationDef)
		return AAFRESULT_NULL_PARAM;

	_interpolationDefinitions.appendValue(pInterpolationDef);
	// trr - We are saving a copy of pointer in _pluginDefinitions
	// so we need to bump its reference count.
	pInterpolationDef->AcquireReference();
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupInterpolationDefinition (
      aafUID_t *pInterpolationID,
      ImplAAFInterpolationDef **ppInterpolationDef)
{
	ImplEnumAAFInterpolationDefs		*InterpolationEnum = NULL;
	ImplAAFInterpolationDef			*InterpolationDef = NULL;
	aafBool						defFound;
	AAFRESULT					status;
	aafUID_t					testAUID;

	XPROTECT()
	{
		CHECK(GetInterpolationDefinitions (&InterpolationEnum));
		status = InterpolationEnum->NextOne (&InterpolationDef);
		defFound = AAFFalse;
		while(status == AAFRESULT_SUCCESS && !defFound)
		{
			CHECK(InterpolationDef->GetAUID (&testAUID));
			if(EqualAUID(pInterpolationID, &testAUID))
			{
				defFound = AAFTrue;
				*ppInterpolationDef = InterpolationDef;
				InterpolationDef->AcquireReference();
				break;
			}
			InterpolationDef->ReleaseReference();
			InterpolationDef = NULL;
			status = InterpolationEnum->NextOne (&InterpolationDef);
		}
		if(InterpolationDef != NULL)
		{
			InterpolationDef->ReleaseReference();
			InterpolationDef = NULL;
		}
		InterpolationEnum->ReleaseReference();
		InterpolationEnum = NULL;
		if(!defFound)
			 RAISE(AAFRESULT_NO_MORE_OBJECTS);
	}
	XEXCEPT
	{
		if(InterpolationEnum != NULL)
			InterpolationEnum->ReleaseReference();
		if(InterpolationDef != NULL)
			InterpolationDef->ReleaseReference();
	}
	XEND
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::GetInterpolationDefinitions (
      ImplEnumAAFInterpolationDefs **ppEnum)
{
	if (NULL == ppEnum)
		return AAFRESULT_NULL_PARAM;
	*ppEnum = 0;
	
	ImplEnumAAFInterpolationDefs *theEnum = (ImplEnumAAFInterpolationDefs *)CreateImpl (CLSID_EnumAAFInterpolationDefs);
	
	XPROTECT()
	{
		CHECK(theEnum->SetEnumStrongProperty(this, &_interpolationDefinitions));
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
    ImplAAFDictionary::RegisterPluginDescriptor (		//!!! Bring this out through the IDL
      ImplAAFPluginDescriptor *pDesc)
{
	if (NULL == pDesc)
		return AAFRESULT_NULL_PARAM;

	_pluginDefinitions.appendValue(pDesc);
	// trr - We are saving a copy of pointer in _pluginDefinitions
	// so we need to bump its reference count.
	pDesc->AcquireReference();
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupPluginDescriptor (		//!!! Bring this out through the IDL
      aafUID_t *pInterpolationID,
      ImplAAFPluginDescriptor **ppPluginDesc)
{
	ImplEnumAAFPluginDescriptors		*pEnum = NULL;
	ImplAAFPluginDescriptor			*pDesc = NULL;
	aafBool						defFound;
	AAFRESULT					status;
	aafUID_t					testAUID;

	XPROTECT()
	{
		CHECK(GetPluginDescriptors (&pEnum));
		status = pEnum->NextOne (&pDesc);
		defFound = AAFFalse;
		while(status == AAFRESULT_SUCCESS && !defFound)
		{
			CHECK(pDesc->GetAUID (&testAUID));
			if(EqualAUID(pInterpolationID, &testAUID))
			{
				defFound = AAFTrue;
				*ppPluginDesc = pDesc;
				pDesc->AcquireReference();
				break;
			}
			pDesc->ReleaseReference();
			pDesc = NULL;
			status = pEnum->NextOne (&pDesc);
		}
		if(pDesc != NULL)
		{
			pDesc->ReleaseReference();
			pDesc = NULL;
		}
		pEnum->ReleaseReference();
		pEnum = NULL;
		if(!defFound)
			 RAISE(AAFRESULT_NO_MORE_OBJECTS);
	}
	XEXCEPT
	{
		if(pEnum != NULL)
			pEnum->ReleaseReference();
		if(pDesc != NULL)
			pDesc->ReleaseReference();
	}
	XEND
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::GetPluginDescriptors (		//!!! Bring this out through the IDL
      ImplEnumAAFPluginDescriptors **ppEnum)
{
	if (NULL == ppEnum)
		return AAFRESULT_NULL_PARAM;
	*ppEnum = 0;
	
	ImplEnumAAFPluginDescriptors *theEnum = (ImplEnumAAFPluginDescriptors *)CreateImpl (CLSID_EnumAAFPluginDescriptors);
	
	XPROTECT()
	{
		CHECK(theEnum->SetEnumStrongProperty(this, &_pluginDefinitions));
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


AAFRESULT ImplAAFDictionary::GenerateOmPid
(
 const aafUID_t & rAuid,
 OMPropertyId & rOutPid
)
{
  OMPropertyId result;
  AAFRESULT hr;

  assert (_pBuiltinClasses);
  hr = _pBuiltinProps->LookupOmPid (rAuid, result);
  if (AAFRESULT_SUCCEEDED (hr))
	{
	  rOutPid = result;
	  return AAFRESULT_SUCCESS;
	}

  // This is where we'd generate an om pid for user-extended
  // properties (either in user-extended classes, or from user-added
  // properties to existing classes).
  //
  // However, for now we're only dealing with builtin types.
  return E_FAIL;
}


// Register the given object to be initialized "at the right time".
// If that time is now, will init immediately.  If not ready yet,
// will put obj on queue to be initialized when everything's ready.
void ImplAAFDictionary::pvtInitObjectProperties
  (ImplAAFObject * pObj) const
{
  assert (pObj);
  if (_OKToInitProps)
	{
	  pObj->InitOMProperties ();
	}
  else
	{
	  // cast away const-ness
	  ((ImplAAFDictionary*)this)->_objsToInit.Append (pObj);
	}
}


const static aafUID_t * sCriticalTypes[] =
{
  &kAAFTypeID_ObjRef,
  &kAAFTypeID_WCharString,
  &kAAFTypeID_AUID,
  &kAAFTypeID_AUIDArray,
  &kAAFTypeID_ObjRefArray,
};


void ImplAAFDictionary::pvtInitCriticalBuiltins (void)
{
  if (_initStarted)
	return;

  // This shouldn't have been set yet.
  assert (! _OKToInitProps);

  // Make sure this code isn't re-entered.
  _initStarted = AAFTrue;

  // Go through critical types; if this dict is associated with a
  // file, force a load of each type without initializing its
  // properties.
  AAFRESULT hr;
  aafUInt32 i;
  const aafUInt32 kNumCritTypes =
	sizeof (sCriticalTypes) / sizeof (sCriticalTypes[0]);
  for (i = 0; i < kNumCritTypes; i++)
	{
	  ImplAAFTypeDef * ptd = 0;
	  hr = LookupType (sCriticalTypes[i], &ptd);
	  if (AAFRESULT_SUCCEEDED (hr))
		{
		  assert (ptd);
		  ptd->ReleaseReference ();
		  ptd = 0;
		}
	}

  // Sucked in the critical types.  Now initialize the properties
  // of each.
  assert (! _OKToInitProps);

  // Note that the queue may grow because pvtInitObjectProperties()
  // gets called during calls to InitOMProperties().
  ImplAAFObjectSP pObj = _objsToInit.GetNext ();
  while (pObj)
	{
	  pObj->InitOMProperties();
	  pObj = _objsToInit.GetNext ();
	}  

  assert (! _OKToInitProps);
  _OKToInitProps = AAFTrue;

  // Make sure the dict itself has had properties init'd.
  InitOMProperties ();
}



//
// Private class implementations
//

ImplAAFDictionary::pvtObjFifo::pvtObjFifo ()
: _putIdx (0),
  _getIdx (0)
{}

ImplAAFObjectSP ImplAAFDictionary::pvtObjFifo::GetNext (void)
{
  ImplAAFObjectSP result;

  if (_getIdx < _putIdx)
	{
	  result = _objs[_getIdx];
	  // This is required so that smart-pointed objects get deleted
	  // immediately.  The problem is that the dictionary itself is
	  // one of these objects; since it maintains a reference count to
	  // itself through this fifo then it never gets deleted unless we
	  // do so explicitly here.
	  _objs[_getIdx++] = 0;
	}

  return result;
}

void ImplAAFDictionary::pvtObjFifo::Append (ImplAAFObject * obj)
{
  assert (obj);
  assert (_putIdx < kPvtMaxInitObjs);
  _objs[_putIdx++] = obj;
}



OMDEFINE_STORABLE(ImplAAFDictionary, AUID_AAFDictionary);

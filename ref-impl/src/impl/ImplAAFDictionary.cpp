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

// Turn on/off experimental object creation code...
#define USE_NEW_OBJECT_CREATION 1


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

#ifndef __ImplAAFBuiltinTypes_h__
#include "ImplAAFBuiltinTypes.h"
#endif

#include "ImplAAFMob.h"
#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"
#include "ImplAAFObjectCreation.h"
#include "ImplAAFBuiltinDefs.h"
#include "AAFClassDefUIDs.h"


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
: _operationDefinitions(PID_Dictionary_OperationDefinitions, "OperationDefinitions", PID_DefinitionObject_Identification), 
  _parameterDefinitions(PID_Dictionary_ParameterDefinitions, "ParameterDefinitions", PID_DefinitionObject_Identification),
  _codecDefinitions(PID_Dictionary_CodecDefinitions, "CodecDefinitions", PID_DefinitionObject_Identification),
  _containerDefinitions(PID_Dictionary_ContainerDefinitions, "ContainerDefinitions", PID_DefinitionObject_Identification),
  _typeDefinitions      (PID_Dictionary_TypeDefinitions,      "TypeDefinitions", PID_DefinitionObject_Identification),
  _classDefinitions      (PID_Dictionary_ClassDefinitions,    "ClassDefinitions", PID_DefinitionObject_Identification),
  _interpolationDefinitions      (PID_Dictionary_InterpolationDefinitions,    "InterpolationDefinitions", PID_DefinitionObject_Identification),
  _dataDefinitions      (PID_Dictionary_DataDefinitions,    "DataDefinitions", PID_DefinitionObject_Identification),
  _pluginDefinitions      (PID_Dictionary_PluginDefinitions,    "PluginDefinitions", PID_PluginDescriptor_Identification),
	_opaqueTypeDefinitions (0),
  _pBuiltinClasses (0),
  _pBuiltinTypes (0),
  _pBuiltinDefs (0),

  _lastGeneratedPid (0),
  _axiomaticTypes (0),
  _OKToAssurePropTypes (false),
  _defRegistrationAllowed (true)
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

  _pBuiltinTypes   = new ImplAAFBuiltinTypes (this);
  _pBuiltinClasses = new ImplAAFBuiltinClasses (this);
}


ImplAAFDictionary::~ImplAAFDictionary ()
{
  // Release the _codecDefinitions
	OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFCodecDef>codecDefinitions(_codecDefinitions);
	while(++codecDefinitions)
	{
		ImplAAFCodecDef *pCodec = codecDefinitions.setValue(0);
		if (pCodec)
		{
		  pCodec->ReleaseReference();
		  pCodec = 0;
		}
	}

	OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFContainerDef>containerDefinitions(_containerDefinitions);
	while(++containerDefinitions)
	{
		ImplAAFContainerDef *pContainer = containerDefinitions.setValue(0);
		if (pContainer)
		{
		  pContainer->ReleaseReference();
		  pContainer = 0;
		}
	}

	OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFTypeDef>typeDefinitions(_typeDefinitions);
	while(++typeDefinitions)
	{
		ImplAAFTypeDef *pType = typeDefinitions.setValue(0);
		if (pType)
		{
		  pType->ReleaseReference();
		  pType = 0;
		}
	}

	OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFClassDef>classDefinitions(_classDefinitions);
	while(++classDefinitions)
	{
		ImplAAFClassDef *pClass = classDefinitions.setValue(0);
		if (pClass)
		{
		  pClass->ReleaseReference();
		  pClass = 0;
		}
	}

	OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFOperationDef>operationDefinitions(_operationDefinitions);
	while(++operationDefinitions)
	{
		ImplAAFOperationDef *pOp = operationDefinitions.setValue(0);
		if (pOp)
		{
		  pOp->ReleaseReference();
		  pOp = 0;
		}
	}

	OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFParameterDef>parameterDefinitions(_parameterDefinitions);
	while(++parameterDefinitions)
	{
		ImplAAFParameterDef *pParm = parameterDefinitions.setValue(0);
		if (pParm)
		{
		  pParm->ReleaseReference();
		  pParm = 0;
		}
	}

	OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFInterpolationDef>interpolateDefinitions(_interpolationDefinitions);
	while(++interpolateDefinitions)
	{
		ImplAAFInterpolationDef *pInterp = interpolateDefinitions.setValue(0);
		if (pInterp)
		{
		  pInterp->ReleaseReference();
		  pInterp = 0;
		}
	}

	OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFPluginDescriptor>pluginDefinitions(_pluginDefinitions);
	while(++pluginDefinitions)
	{
		ImplAAFPluginDescriptor *pPlug = pluginDefinitions.setValue(0);
		if (pPlug)
		{
		  pPlug->ReleaseReference();
		  pPlug = 0;
		}
	}

	OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFDataDef>dataDefinitions(_dataDefinitions);
	while(++dataDefinitions)
	{
		ImplAAFDataDef *pData = dataDefinitions.setValue(0);
		if (pData)
		{
		  pData->ReleaseReference();
		  pData = 0;
		}
	}

  assert (_pBuiltinClasses);
  delete _pBuiltinClasses;
  _pBuiltinClasses = 0;

  assert (_pBuiltinTypes);
  delete _pBuiltinTypes;
  _pBuiltinTypes = 0;

  if (_pBuiltinDefs)
	{
	  delete _pBuiltinDefs;
	  _pBuiltinDefs = 0;
	}

  delete [] _axiomaticTypes;
}



//
// Factory function for all built-in classes.
//
/*static*/ ImplAAFObject *
ImplAAFDictionary::pvtCreateBaseClassInstance(const aafUID_t & auid) 
{

  // Lookup the code class id for the given stored object id.
  const aafClassID_t* id = ImplAAFBaseClassFactory::LookupClassID(auid);
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
  
  
  pDictionary = static_cast<ImplAAFDictionary *>(pvtCreateBaseClassInstance(AUID_AAFDictionary));
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
  
  pDictionary->pvtSetSoid (AUID_AAFDictionary);

  return pDictionary;
}


//
// Create an instance of the appropriate derived class, given the class id.
//  This method implements the OMClassFactory interface.
//
OMStorable* ImplAAFDictionary::create(const OMClassId& classId) const
{
  AAFRESULT hr;
  const aafUID_t * auid  = reinterpret_cast<const aafUID_t*>(&classId);
  ImplAAFDictionary * pNonConstThis = (ImplAAFDictionary*) this;

#if USE_NEW_OBJECT_CREATION
  
  // Call the sample top-level dictionary method that is called
  // by the API client to create objects.
  ImplAAFObject *pObject = NULL;
  hr = pNonConstThis->CreateInstance(*auid, &pObject);
  assert (AAFRESULT_SUCCEEDED (hr));
  return pObject;

#else // #if USE_NEW_OBJECT_CREATION
  
  ImplAAFClassDefSP pcd;
  hr = pNonConstThis->LookupClassDef(*auid, &pcd);
  assert (AAFRESULT_SUCCEEDED (hr));
  
  return CreateAndInit (pcd);

#endif // #else // #if USE_NEW_OBJECT_CREATION
}


ImplAAFObject *
ImplAAFDictionary::CreateAndInit(ImplAAFClassDef * pClassDef) const
{
  assert (pClassDef);
  AAFRESULT hr;
  aafUID_t auid;
  hr = pClassDef->GetAUID(&auid);
  assert (AAFRESULT_SUCCEEDED (hr));

  ImplAAFObject * pNewObject = 0;
  pNewObject = pvtInstantiate (auid);
  if (pNewObject)
	{
	  pClassDef->InitOMProperties (pNewObject);

	  // Attempt to initialize the any class extensions associated
	  // with this object. Only the most derived extension that has an
	  // associated  plugin is created.
	  // QUESTION: How should we "deal with" failure? We really need
	  // an  error/warning log file for this kind of information.
	  AAFRESULT result = pNewObject->InitializeExtensions();
	}

  return pNewObject;
}

ImplAAFObject* ImplAAFDictionary::pvtInstantiate(const aafUID_t & auid) const
{
  ImplAAFObject *result = 0;
  ImplAAFClassDef	*parent;

  // Is this a request to create the dictionary?
  if (memcmp(&auid, &AUID_AAFDictionary, sizeof(aafUID_t)) == 0)
	{ // The result is just this instance.
	  result = const_cast<ImplAAFDictionary*>(this);
	  // Bump the reference count.
	  AcquireReference();
	} 
  else
	{
	  // Create an instance of the class corresponsing to the given
	  // stored object id.  In other words, we instantiate an
	  // implementation object which can represent this stored object
	  // in code.  In the case of built-in types, each one *has* a
	  // code object which can represent it.  However in the case of
	  // client-defined classes, the best we can do is instantiate the
	  // most-derived code object which is an inheritance parent of
	  // the desired class.

	  // First see if this is a built-in class.
	  //
	  result = pvtCreateBaseClassInstance(auid);
	  while (result == 0)
		{
		  aafUID_t parentAUID = auid;
		  aafBool	isRoot;

		  // Not a built-in class; find the nearest built-in parent.
		  // That is, iterate up the inheritance hierarchy until we
		  // find a class which we know how to instantiate.
		  //
		  ImplAAFClassDefSP pcd;
		  AAFRESULT hr;

		  hr = ((ImplAAFDictionary*)this)->LookupClassDef (parentAUID, &pcd);
		  if (AAFRESULT_FAILED (hr))
			{
			  // AUID does not correspond to any class in the
			  // dictionary; bail out with NULL result
			  assert (0 == result);
			  break;
			}
			hr = pcd->IsRoot(&isRoot);
		  if (isRoot || hr != AAFRESULT_SUCCESS)
			{
			  // Class was apparently registered, but no appropriate
			  // parent class found!  This should not happen, as every
			  // registered class must have a registered parent class.
			  // The only exception is AAFObject, which would have
			  // been found by the earlier
			  // pvtCreateBaseClassInstance() call.
			  assert (0);
			}
			hr = pcd->GetParent (&parent);
			hr = parent->GetAUID(&parentAUID);
			parent->ReleaseReference();
		  result = pvtCreateBaseClassInstance(parentAUID);
		}
	}

  if (result)
	{
	  if ((ImplAAFDictionary *)result != this)
		{
		  // If we created an object then give it a reference to the
		  // factory (dictionary) that was used to created it.
		  //
		  result->setClassFactory(this);

		  // Set this object's stored ID.  Be sure to set it to the
		  // requested ID, not the instantiated one.  (These will be
		  // different if the requested ID is a client-supplied
		  // extension class.)
		  result->pvtSetSoid (auid);
		}
	}

  return result;
}


// Creates a single uninitialized AAF object of the class associated 
  // with a specified stored object id.
AAFRESULT STDMETHODCALLTYPE 
  ImplAAFDictionary::CreateInstance (
    // Stored Object ID of the stored object to be created.
    aafUID_constref classId,

    // Address of output variable that receives the 
    // object pointer requested in auid
    ImplAAFObject ** ppvObject)
{
  if (NULL == ppvObject)
    return AAFRESULT_NULL_PARAM;
  
  // Lookup the class definition for the given classId. If the class
  // definition is one of the "built-in" class definitions then the
  // definition will be created and registered with the dictionary
  // if necessary. (TRR 2000-MAR-01)
  ImplAAFClassDefSP pClassDef;
  AAFRESULT hr;
  hr = LookupClassDef (classId, &pClassDef);
  if (AAFRESULT_FAILED (hr))
	return hr;

#if USE_NEW_OBJECT_CREATION

  // The class definition is the factory for the corresponding 
  // "data" object. For example the class definition for MasterMob is the 
  // factory for creating instances of MasterMobs. (TRR 2000-MAR-01)
  return pClassDef->CreateInstance(ppvObject);

#else // #if USE_NEW_OBJECT_CREATION

  *ppvObject = CreateAndInit (pClassDef);

  if (NULL == *ppvObject)
    return AAFRESULT_INVALID_CLASS_ID;
  else
    return AAFRESULT_SUCCESS;

#endif // #else // #if USE_NEW_OBJECT_CREATION
}



AAFRESULT ImplAAFDictionary::dictLookupClassDef (
      const aafUID_t & classID,
      ImplAAFClassDef ** ppClassDef)
{
  if (!ppClassDef) return AAFRESULT_NULL_PARAM;

	AAFRESULT result = AAFRESULT_SUCCESS;
  // NOTE: The following type cast is temporary. It should be removed as soon
	// as the OM has a declarative sytax to include the type
	// of the key used in the set. (trr:2000-FEB-26)
	if (_classDefinitions.find((*reinterpret_cast<const OMObjectIdentification *>(&classID)),
                             *ppClassDef))
	{
		assert(NULL != *ppClassDef);
    (*ppClassDef)->AcquireReference();
	}
	else
	{
    // no recognized class guid in dictionary
    result = AAFRESULT_NO_MORE_OBJECTS;
	}

	return (result);
}

bool ImplAAFDictionary::PvtIsClassPresent (
      const aafUID_t & classID)
{
  // NOTE: The following type cast is temporary. It should be removed as soon
	// as the OM has a declarative sytax to include the type
	// of the key used in the set. (trr:2000-FEB-26)
	return(_classDefinitions.contains((*reinterpret_cast<const OMObjectIdentification *>(&classID))));
}

bool
ImplAAFDictionary::IsAxiomaticClass (const aafUID_t &classID) const
{
  assert (_pBuiltinClasses);
  return _pBuiltinClasses->IsAxiomaticClass (classID);
}


bool
ImplAAFDictionary::pvtLookupAxiomaticClassDef (const aafUID_t &classID,
										   ImplAAFClassDef **
										   ppClassDef)
{
  if (_pBuiltinClasses->IsAxiomaticClass (classID))
	{
	  // It's axiomatic.  
	  assert (ppClassDef);
	  *ppClassDef = _pBuiltinClasses->LookupAxiomaticClass (classID);
	  assert (*ppClassDef); // reference count already incremented by LookupAxiomaticClass.
	  return true;
	}
  return false;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupClassDef (
      const aafUID_t & classID,
      ImplAAFClassDef ** ppClassDef)
{
  AAFRESULT					status;

  if (! ppClassDef) return AAFRESULT_NULL_PARAM;

  if (pvtLookupAxiomaticClassDef (classID, ppClassDef))
  {
	  assert (*ppClassDef);
	  // Yes, this is an axiomatic class.  classDef should be filled
	  // in.  Assure that it's in the dictionary, and return it.
	  
	  // here's where we assure it's in the dictionary.
	  if(_defRegistrationAllowed)
	  {
		  ImplAAFClassDef	*parent;
		  // These classes fail with doubly-opened storages
#if 0
		  if(memcmp(&classID, &kAAFClassID_ClassDefinition, sizeof(aafUID_t)) != 0
		   && memcmp(&classID, &kAAFClassID_TypeDefinitionCharacter, sizeof(aafUID_t)) != 0
		   && memcmp(&classID, &kAAFClassID_TypeDefinitionString, sizeof(aafUID_t)) != 0
		   && memcmp(&classID, &kAAFClassID_TypeDefinitionVariableArray, sizeof(aafUID_t)) != 0
		   && memcmp(&classID, &kAAFClassID_TypeDefinitionRecord, sizeof(aafUID_t)) != 0
		   && memcmp(&classID, &kAAFClassID_TypeDefinitionFixedArray, sizeof(aafUID_t)) != 0
		   && memcmp(&classID, &kAAFClassID_TypeDefinitionInteger, sizeof(aafUID_t)) != 0
		   )
#endif
		  {
			  if(!PvtIsClassPresent(classID))
			  {
				aafBool		isRoot;
				aafUID_t	uid;

				  (*ppClassDef)->IsRoot(&isRoot);
				  if(isRoot)
				  {
					 (*ppClassDef)->SetParent(*ppClassDef);
				  }
				  else
				  {
					 (*ppClassDef)->GetParent(&parent);	// Uses bootstrap parent if set
					 parent->GetAUID(&uid);
					 parent->ReleaseReference();
					 parent = NULL;
					 LookupClassDef (uid, &parent);
					 (*ppClassDef)->SetParent(parent);
					 parent->ReleaseReference();
				  }
				  (*ppClassDef)->SetBootstrapParent(NULL);
				  status = RegisterClassDef(*ppClassDef);
				  assert (AAFRESULT_SUCCEEDED (status));
			  }
		  }
	  }
	  
	  AssurePropertyTypes (*ppClassDef);
	  return AAFRESULT_SUCCESS;
  }

  // Not axiomatic.  Check to see if it's already in the dict.
  status = dictLookupClassDef (classID, ppClassDef);
  if (AAFRESULT_SUCCEEDED (status))
	{
	  // Yup, already here. Pass on the good news.
	  AssurePropertyTypes (*ppClassDef);
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
  status = _pBuiltinClasses->NewBuiltinClassDef (classID, ppClassDef);
  if (AAFRESULT_FAILED (status))
	{
	  // no recognized class guid
	  return AAFRESULT_NO_MORE_OBJECTS;
	}

  // Yup, found it in builtins.  Register it.
  assert (*ppClassDef);
  status = RegisterClassDef (*ppClassDef);
  if (AAFRESULT_FAILED (status))
	return status;
		  
  AssurePropertyTypes (*ppClassDef);
	
  return(AAFRESULT_SUCCESS);
}

#if USE_NEW_OBJECT_CREATION

AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::CreateImplClassDef (
      aafUID_constref classID,
      ImplAAFClassDef * pParentClass,
      aafCharacter_constptr pClassName,
      ImplAAFClassDef ** ppClassDef)
{
  assert(pClassName && ppClassDef);
  AAFRESULT result = AAFRESULT_SUCCESS;
  *ppClassDef = NULL;

  // Lookup the class definitions class definition! This had
  // better not be recursive!
  ImplAAFClassDef *pClassDefsClassDef = GetBuiltinDefs()->cdClassDef();
  ImplAAFClassDef *pClassDef = NULL;

  // Create an instance of a class definition and initialize it.
  pClassDef = (ImplAAFClassDef *)pvtInstantiate(AUID_AAFClassDef);
  if (NULL == pClassDef)
    return AAFRESULT_NOMEMORY;

  result = pClassDef->pvtInitialize(classID, pParentClass, pClassName);
  if (AAFRESULT_FAILED(result))
  {
    // Delete the new object.
    pClassDef->ReleaseReference();
    pClassDef = NULL;
  }
  else
  {
    // Make sure properties are initialized (???)
    pClassDefsClassDef->InitOMProperties (pClassDef);

    // The class definition could be successfully initialized. NOTE: This
    // object has already been reference counted.
    *ppClassDef = pClassDef;
  }

  return result;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::CreateClassDef (
      aafUID_constref classID,
      ImplAAFClassDef *pParentClass,
      aafCharacter_constptr pClassName,
      ImplAAFClassDef ** ppClassDef)
{

 return AAFRESULT_NOT_IMPLEMENTED;
}

#endif // #if USE_NEW_OBJECT_CREATION

AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::RegisterClassDef (
      ImplAAFClassDef * pClassDef)
{
  assert (_defRegistrationAllowed);

  if (NULL == pClassDef)
	return AAFRESULT_NULL_PARAM;
	
  // Get the AUID of the new class to register.
  aafUID_t newAUID;
  HRESULT hr = pClassDef->GetAUID(&newAUID);
  if (hr != AAFRESULT_SUCCESS)
    return hr;

  // Is this class already registered ?
  if (!PvtIsClassPresent(newAUID))
	{
	  // This class is not yet registered, add it to the dictionary.
	  _classDefinitions.appendValue(pClassDef);
	  pClassDef->AcquireReference();
	}

  return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::GetClassDefs (
      ImplEnumAAFClassDefs ** ppEnum)
{
  if (NULL == ppEnum)
	return AAFRESULT_NULL_PARAM;
  *ppEnum = 0;
	
  ImplEnumAAFClassDefs *theEnum = (ImplEnumAAFClassDefs *)CreateImpl (CLSID_EnumAAFClassDefs);
	
  XPROTECT()
	{
		OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFClassDef>* iter = 
			new OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFClassDef>(_classDefinitions);
		if(iter == 0)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(theEnum->SetIterator(this, iter));
	  *ppEnum = theEnum;
	}
  XEXCEPT
	{
	  if (theEnum)
		{
		  theEnum->ReleaseReference();
		  theEnum = 0;
		}
	  return(XCODE());
	}
  XEND;
	
  return(AAFRESULT_SUCCESS);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::CountClassDefs
        (aafUInt32 * pResult)
{
  if (! pResult)
	return AAFRESULT_NULL_PARAM;
	*pResult = _classDefinitions.count();
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::RegisterTypeDef (
      ImplAAFTypeDef * pTypeDef)
{
  assert (_defRegistrationAllowed);

  if (NULL == pTypeDef)
	return AAFRESULT_NULL_PARAM;
	
  // Get the AUID of the new type to register.
  aafUID_t newAUID;
  HRESULT hr = pTypeDef->GetAUID(&newAUID);
  if (hr != AAFRESULT_SUCCESS)
    return hr;

  // Is this type already registered ?
  if (!PvtIsTypePresent(newAUID))
  {
//	if (AAFRESULT_FAILED(hr))
//	{
		// TBD:(transdel 2000-MAR-19) we also nee to check it the
		// given type is an opaque type. if it is then we
		// need to remove it from the opaqueTypeDefinitions set
		// if the registration of successful. This make sence
		// since type can no longer be opaque.
		// hr = LookupOpaqueTypeDef (newAUID, &pExistingTypeDef);
		// etc,,,
		//
//	}


    // This type is not yet registered, add it to the dictionary.
    _typeDefinitions.appendValue(pTypeDef);
    pTypeDef->AcquireReference();
    // Set up the (non-persistent) dictionary pointer.
	//
    // BobT 6/15/99: Remove ImplAAFDefObject::Get/SetDict() in favor of
	// ImplAAFObject::GetDictionary().
	// pTypeDef->SetDict(this);
  }

  return(AAFRESULT_SUCCESS);
}


AAFRESULT ImplAAFDictionary::dictLookupTypeDef (
      const aafUID_t & typeID,
      ImplAAFTypeDef ** ppTypeDef)
{
  if (! ppTypeDef)
    return AAFRESULT_NULL_PARAM;

	
	AAFRESULT result = AAFRESULT_SUCCESS;
  // NOTE: The following type cast is temporary. It should be removed as soon
	// as the OM has a declarative sytax to include the type
	// of the key used in the set. (trr:2000-FEB-26)
	if (_typeDefinitions.find((*reinterpret_cast<const OMObjectIdentification *>(&typeID)),
                             *ppTypeDef))
	{
		assert(NULL != *ppTypeDef);
    (*ppTypeDef)->AcquireReference();
	}
	else
	{
    // no recognized class guid in dictionary
    result = AAFRESULT_NO_MORE_OBJECTS;
	}

	return (result);

}

bool ImplAAFDictionary::PvtIsTypePresent (
      const aafUID_t & typeID)
{
  // NOTE: The following type cast is temporary. It should be removed as soon
	// as the OM has a declarative sytax to include the type
	// of the key used in the set. (trr:2000-FEB-26)
	return(_typeDefinitions.contains((*reinterpret_cast<const OMObjectIdentification *>(&typeID))));
}

/*static*/
const aafUID_t * ImplAAFDictionary::sAxiomaticTypeGuids[] = 
{
  & kAAFTypeID_AUID,
  & kAAFTypeID_AUIDArray,
  & kAAFTypeID_Boolean,
  & kAAFTypeID_Character,
  & kAAFTypeID_DateStruct,
  & kAAFTypeID_TimeStruct,
  & kAAFTypeID_Int16,
  & kAAFTypeID_Int32,
  & kAAFTypeID_Int64,
  & kAAFTypeID_Int64Array,
  & kAAFTypeID_Int8,
  & kAAFTypeID_Character,
  & kAAFTypeID_String,
  & kAAFTypeID_TimeStamp,
  & kAAFTypeID_UInt16,
  & kAAFTypeID_UInt32,
  & kAAFTypeID_UInt8,
  & kAAFTypeID_UInt8Array8,
  & kAAFTypeID_Indirect,
  & kAAFTypeID_VersionType,
  & kAAFTypeID_RGBAComponent,
  & kAAFTypeID_MobID,
  & kAAFTypeID_DataValue,
  & kAAFTypeID_AUIDArray,
 
  & kAAFTypeID_ClassDefinitionStrongReference,
  & kAAFTypeID_ClassDefinitionStrongReferenceSet,
  & kAAFTypeID_ClassDefinitionWeakReference,
  & kAAFTypeID_CodecDefinitionStrongReference,
  & kAAFTypeID_CodecDefinitionStrongReferenceSet,
  & kAAFTypeID_ContainerDefinitionStrongReference,
  & kAAFTypeID_ContainerDefinitionStrongReferenceSet,
  & kAAFTypeID_DataDefinitionStrongReference,
  & kAAFTypeID_DataDefinitionStrongReferenceSet,
  & kAAFTypeID_DataDefinitionWeakReference,
  & kAAFTypeID_DataDefinitionWeakReferenceSet,
  & kAAFTypeID_DefinitionObjectWeakReference,
  & kAAFTypeID_IdentificationWeakReference,
  & kAAFTypeID_InterpolationDefinitionStrongReference,
  & kAAFTypeID_InterpolationDefinitionStrongReferenceSet,
  & kAAFTypeID_LocatorStrongReference,
  & kAAFTypeID_LocatorStrongReferenceSet,
  & kAAFTypeID_MobStrongReferenceSet,
  & kAAFTypeID_NetworkLocatorStrongReference,
  & kAAFTypeID_OperationDefinitionStrongReference,
  & kAAFTypeID_OperationDefinitionStrongReferenceSet,
  & kAAFTypeID_OperationDefinitionWeakReference,
  & kAAFTypeID_OperationDefinitionWeakReferenceVector,
  & kAAFTypeID_ParameterDefinitionStrongReference,
  & kAAFTypeID_ParameterDefinitionStrongReferenceSet,
  & kAAFTypeID_ParameterDefinitionWeakReference,
  & kAAFTypeID_ParameterDefinitionWeakReferenceSet,
  & kAAFTypeID_PluginDescriptorStrongReference,
  & kAAFTypeID_PluginDescriptorStrongReferenceSet,
  & kAAFTypeID_PluginDescriptorWeakReference,
  & kAAFTypeID_PluginDescriptorWeakReferenceSet,
  & kAAFTypeID_PropertyDefinitionStrongReference,
  & kAAFTypeID_PropertyDefinitionStrongReferenceSet,
  & kAAFTypeID_TypeDefinitionStrongReference,
  & kAAFTypeID_TypeDefinitionStrongReferenceSet,
  & kAAFTypeID_TypeDefinitionWeakReference,
  & kAAFTypeID_TypeDefinitionWeakReferenceVector
};

bool
ImplAAFDictionary::pvtLookupAxiomaticTypeDef (const aafUID_t &typeID,
										   ImplAAFTypeDef **
										   ppTypeDef)
{
  static const aafUInt32 ksNumAxiomaticTypes
	= sizeof (sAxiomaticTypeGuids) / sizeof (sAxiomaticTypeGuids[0]);

  if (! _axiomaticTypes)
	{
	  _axiomaticTypes = new ImplAAFTypeDefSP [ksNumAxiomaticTypes];
	  for (aafUInt32 i = 0; i < ksNumAxiomaticTypes; i++)
		_axiomaticTypes[i] = 0;
	}
  assert (_axiomaticTypes);

  for (aafUInt32 i = 0; i < ksNumAxiomaticTypes; i++)
	{
	  if (EqualAUID (&typeID, sAxiomaticTypeGuids[i]))
		{
		  // It's axiomatic.  
		  if (! _axiomaticTypes[i])
			{
			  // We haven't allocated one yet.
			  AAFRESULT hr;
			  ImplAAFTypeDefSP tmp;
			  hr = _pBuiltinTypes-> NewBuiltinTypeDef (typeID, &tmp);
			  assert (AAFRESULT_SUCCEEDED (hr));

			  // In the case where a type is aliased
			  // (e.g. TypeDefWeakRef is aliased to TypeDefAUID) then
			  // in the process of making the referring type, the
			  // ref'd type may already be created.  Check to make
			  // sure that it didn't get created in the process.  If
			  // so, then simply throw the new one away (currently in
			  // tmp).
			  if (! _axiomaticTypes[i])
				_axiomaticTypes[i] = tmp;

			 assert (_axiomaticTypes[i]);
			}
		  assert (_axiomaticTypes[i]);
		  assert (ppTypeDef);
		  *ppTypeDef = _axiomaticTypes[i];
		  (*ppTypeDef)->AcquireReference ();
		  return true;
		}
	}
  return false;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupTypeDef (
      const aafUID_t & typeID,
      ImplAAFTypeDef ** ppTypeDef)
{
  ImplAAFTypeDefSP			typeDef;
  AAFRESULT					status;
//	aafUID_t				foundUID;

  if (! ppTypeDef) return AAFRESULT_NULL_PARAM;

  if (pvtLookupAxiomaticTypeDef (typeID, &typeDef))
  {
	  if(_defRegistrationAllowed && !PvtIsTypePresent(typeID))
	  {
		  status = RegisterTypeDef(typeDef);
		  assert (AAFRESULT_SUCCEEDED (status));
	  }
	  
	  assert (ppTypeDef);
	  *ppTypeDef = typeDef;
	  assert (*ppTypeDef);
	  (*ppTypeDef)->AcquireReference ();
	  return AAFRESULT_SUCCESS;
  }

  // Not axiomatic.  Check to see if it's already in the dict.
  status = dictLookupTypeDef (typeID, ppTypeDef);
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
  assert (0 == (ImplAAFTypeDef*) typeDef);
  status = _pBuiltinTypes->NewBuiltinTypeDef (typeID, &typeDef);
  if (AAFRESULT_FAILED (status))
	{
	  // no recognized type guid
	  return AAFRESULT_NO_MORE_OBJECTS;
	}
  // Yup, found it in builtins.  Register it.
  if(_defRegistrationAllowed)
	 {
	  assert (typeDef);
	 status = RegisterTypeDef (typeDef);
	 if (AAFRESULT_FAILED (status))
		return status;
  }
		  
  assert (ppTypeDef);
  *ppTypeDef = typeDef;
  assert (*ppTypeDef);
  (*ppTypeDef)->AcquireReference ();
	
  return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::GetTypeDefs (
      ImplEnumAAFTypeDefs ** ppEnum)
{
	if (NULL == ppEnum)
		return AAFRESULT_NULL_PARAM;
	*ppEnum = 0;
	
	ImplEnumAAFTypeDefs *theEnum = (ImplEnumAAFTypeDefs *)CreateImpl (CLSID_EnumAAFTypeDefs);
	
	XPROTECT()
	{
		OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFTypeDef>* iter = 
			new OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFTypeDef>(_typeDefinitions);
		if(iter == 0)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(theEnum->SetIterator(this, iter));
		*ppEnum = theEnum;
	}
	XEXCEPT
	{
		if (theEnum)
		  {
			theEnum->ReleaseReference();
			theEnum = 0;
		  }
		return(XCODE());
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::CountTypeDefs
        (aafUInt32 * pResult)
{
  if (! pResult)
	return AAFRESULT_NULL_PARAM;
	*pResult = _typeDefinitions.count();
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::RegisterOpaqueTypeDef (
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
  

  // This type is not yet registered, add it to the dictionary.
//  _opaqueTypeDefinitions.appendValue(pTypeDef);
//  pTypeDef->AcquireReference();

  return (AAFRESULT_NOT_IMPLEMENTED);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupOpaqueTypeDef (
      const aafUID_t & typeID,
      ImplAAFTypeDef ** ppTypeDef)
{
  ImplAAFTypeDefSP			typeDef;
//  AAFRESULT					status;

  if (! ppTypeDef) 
		return AAFRESULT_NULL_PARAM;
//  *ppTypeDef = typeDef;
//  assert (*ppTypeDef);
//  (*ppTypeDef)->AcquireReference ();
	
  return (AAFRESULT_NOT_IMPLEMENTED);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::GetOpaqueTypeDefs (
      ImplEnumAAFTypeDefs ** ppEnum)
{
	if (NULL == ppEnum)
		return AAFRESULT_NULL_PARAM;
	*ppEnum = 0;

#if 0
	ImplEnumAAFTypeDefs *theEnum = (ImplEnumAAFTypeDefs *)CreateImpl (CLSID_EnumAAFTypeDefs);
	
	XPROTECT()
	{
		OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFTypeDef>* iter = 
			new OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFTypeDef>(_typeDefinitions);
		if(iter == 0)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(theEnum->SetIterator(this, iter));
		*ppEnum = theEnum;
	}
	XEXCEPT
	{
		if (theEnum)
		  {
			theEnum->ReleaseReference();
			theEnum = 0;
		  }
		return(XCODE());
	}
	XEND;
	
#endif
  return (AAFRESULT_NOT_IMPLEMENTED);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::CountOpaqueTypeDefs
        (aafUInt32 * pResult)
{
  if (! pResult)
		return AAFRESULT_NULL_PARAM;
//	*pResult = _typeDefinitions.count();
//  return AAFRESULT_SUCCESS;
  return (AAFRESULT_NOT_IMPLEMENTED);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::RegisterDataDef (
      ImplAAFDataDef *pDataDef)
{
  assert (_defRegistrationAllowed);

  if (NULL == pDataDef)
	return AAFRESULT_NULL_PARAM;
	
  // Get the AUID of the new type to register.
  aafUID_t newAUID;
  HRESULT hr = pDataDef->GetAUID(&newAUID);
  if (hr != AAFRESULT_SUCCESS)
    return hr;

  // Is this type already registered ?
  ImplAAFDataDef * pExistingDataDef = NULL;
  hr = LookupDataDef(newAUID, &pExistingDataDef);

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
	pExistingDataDef = 0;
  }

  return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupDataDef (
      const aafUID_t & dataDefinitionID,
      ImplAAFDataDef **ppDataDef)
{
  if (!ppDataDef) return AAFRESULT_NULL_PARAM;

	AAFRESULT result = AAFRESULT_SUCCESS;
  // NOTE: The following type cast is temporary. It should be removed as soon
	// as the OM has a declarative sytax to include the type
	// of the key used in the set. (trr:2000-FEB-29)
	if (_dataDefinitions.find((*reinterpret_cast<const OMObjectIdentification *>(&dataDefinitionID)),
                             *ppDataDef))
	{
		assert(NULL != *ppDataDef);
		(*ppDataDef)->AcquireReference();
	}
	else
	{
		// no recognized class guid in dictionary
		result = AAFRESULT_NO_MORE_OBJECTS;
	}

	return (result);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::GetDataDefs (
      ImplEnumAAFDataDefs **ppEnum)
{
	if (NULL == ppEnum)
		return AAFRESULT_NULL_PARAM;
	*ppEnum = 0;
	
	ImplEnumAAFDataDefs *theEnum = (ImplEnumAAFDataDefs *)CreateImpl (CLSID_EnumAAFDataDefs);
	
	XPROTECT()
	{
		OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFDataDef>* iter = 
			new OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFDataDef>(_dataDefinitions);
		if(iter == 0)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(theEnum->SetIterator(this, iter));
		*ppEnum = theEnum;
	}
	XEXCEPT
	{
		if (theEnum)
		  {
			theEnum->ReleaseReference();
			theEnum = 0;
		  }
		return(XCODE());
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::CountDataDefs
        (aafUInt32 * pResult)
{
  if (! pResult)
	return AAFRESULT_NULL_PARAM;
  *pResult = _dataDefinitions.count();
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::RegisterOperationDef (
      ImplAAFOperationDef *pOperationDef)
{
  assert (_defRegistrationAllowed);

	if (NULL == pOperationDef)
		return AAFRESULT_NULL_PARAM;

	_operationDefinitions.appendValue(pOperationDef);
	// trr - We are saving a copy of pointer in _pluginDefinitions
	// so we need to bump its reference count.
	pOperationDef->AcquireReference();
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupOperationDef (
      const aafUID_t & effectID,
      ImplAAFOperationDef **ppOperationDef)
{
  if (!ppOperationDef) return AAFRESULT_NULL_PARAM;

	AAFRESULT result = AAFRESULT_SUCCESS;
  // NOTE: The following type cast is temporary. It should be removed as soon
	// as the OM has a declarative sytax to include the type
	// of the key used in the set. (trr:2000-FEB-29)
	if (_operationDefinitions.find((*reinterpret_cast<const OMObjectIdentification *>(&effectID)),
                             *ppOperationDef))
	{
		assert(NULL != *ppOperationDef);
		(*ppOperationDef)->AcquireReference();
	}
	else
	{
		// no recognized class guid in dictionary
		result = AAFRESULT_NO_MORE_OBJECTS;
	}

	return (result);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::GetOperationDefs (
      ImplEnumAAFOperationDefs **ppEnum)
{
	if (NULL == ppEnum)
		return AAFRESULT_NULL_PARAM;
	*ppEnum = 0;
	
	ImplEnumAAFOperationDefs *theEnum = (ImplEnumAAFOperationDefs *)CreateImpl (CLSID_EnumAAFOperationDefs);
	
	XPROTECT()
	{
		OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFOperationDef>* iter = 
			new OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFOperationDef>(_operationDefinitions);
		if(iter == 0)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(theEnum->SetIterator(this, iter));
		*ppEnum = theEnum;
	}
	XEXCEPT
	{
		if (theEnum)
		  {
			theEnum->ReleaseReference();
			theEnum = 0;
		  }
		return(XCODE());
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::CountOperationDefs
        (aafUInt32 * pResult)
{
  if (! pResult)
	return AAFRESULT_NULL_PARAM;
  *pResult = _operationDefinitions.count();
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::RegisterParameterDef (
      ImplAAFParameterDef *pParameterDef)
{
  assert (_defRegistrationAllowed);

	if (NULL == pParameterDef)
		return AAFRESULT_NULL_PARAM;
	if (pParameterDef->attached())
		return AAFRESULT_OBJECT_ALREADY_ATTACHED;


	_parameterDefinitions.appendValue(pParameterDef);
	// trr - We are saving a copy of pointer in _pluginDefinitions
	// so we need to bump its reference count.
	pParameterDef->AcquireReference();
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupParameterDef (
      const aafUID_t & parameterID,
      ImplAAFParameterDef **ppParameterDef)
{
  if (!ppParameterDef) return AAFRESULT_NULL_PARAM;

	AAFRESULT result = AAFRESULT_SUCCESS;
  // NOTE: The following type cast is temporary. It should be removed as soon
	// as the OM has a declarative sytax to include the type
	// of the key used in the set. (trr:2000-FEB-29)
	if (_parameterDefinitions.find((*reinterpret_cast<const OMObjectIdentification *>(&parameterID)),
                             *ppParameterDef))
	{
		assert(NULL != *ppParameterDef);
		(*ppParameterDef)->AcquireReference();
	}
	else
	{
		// no recognized class guid in dictionary
		result = AAFRESULT_NO_MORE_OBJECTS;
	}

	return (result);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::GetParameterDefs (
      ImplEnumAAFParameterDefs **ppEnum)
{
	if (NULL == ppEnum)
		return AAFRESULT_NULL_PARAM;
	*ppEnum = 0;
	
	ImplEnumAAFParameterDefs *theEnum = (ImplEnumAAFParameterDefs *)CreateImpl (CLSID_EnumAAFParameterDefs);
	
	XPROTECT()
	{
		OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFParameterDef>* iter = 
			new OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFParameterDef>(_parameterDefinitions);
		if(iter == 0)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(theEnum->SetIterator(this, iter));
		*ppEnum = theEnum;
	}
	XEXCEPT
	{
		if (theEnum)
		  {
			theEnum->ReleaseReference();
			theEnum = 0;
		  }
		return(XCODE());
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::CountParameterDefs
        (aafUInt32 * pResult)
{
  if (! pResult)
	return AAFRESULT_NULL_PARAM;
  *pResult = _parameterDefinitions.count();
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::RegisterCodecDef (
      ImplAAFCodecDef *pPlugDef)
{
  assert (_defRegistrationAllowed);

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
  
  siz = _codecDefinitions.count();
  
  *pNumCodecDefs = siz;
  return AAFRESULT_SUCCESS;
}

AAFRESULT ImplAAFDictionary::LookupCodecDef
  (const aafUID_t & defID,
   ImplAAFCodecDef **ppResult)
{
  if (!ppResult) return AAFRESULT_NULL_PARAM;

	AAFRESULT result = AAFRESULT_SUCCESS;
  // NOTE: The following type cast is temporary. It should be removed as soon
	// as the OM has a declarative sytax to include the type
	// of the key used in the set. (trr:2000-FEB-29)
	if (_codecDefinitions.find((*reinterpret_cast<const OMObjectIdentification *>(&defID)),
                             *ppResult))
	{
		assert(NULL != *ppResult);
		(*ppResult)->AcquireReference();
	}
	else
	{
		// no recognized class guid in dictionary
		result = AAFRESULT_NO_MORE_OBJECTS;
	}

	return (result);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::GetCodecDefs (
      ImplEnumAAFCodecDefs **ppEnum)
{
	if (NULL == ppEnum)
		return AAFRESULT_NULL_PARAM;
	*ppEnum = 0;
	
	ImplEnumAAFCodecDefs *theEnum = (ImplEnumAAFCodecDefs *)CreateImpl (CLSID_EnumAAFCodecDefs);
	
	XPROTECT()
	{
		OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFCodecDef>* iter = 
			new OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFCodecDef>(_codecDefinitions);
		if(iter == 0)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(theEnum->SetIterator(this, iter));
		*ppEnum = theEnum;
	}
	XEXCEPT
	{
		if (theEnum)
		  {
			theEnum->ReleaseReference();
			theEnum = 0;
		  }
		return(XCODE());
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::CountCodecDefs
        (aafUInt32 * pResult)
{
  if (! pResult)
	return AAFRESULT_NULL_PARAM;
  *pResult = _codecDefinitions.count();
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::RegisterContainerDef (
      ImplAAFContainerDef *pPlugDef)
{
  assert (_defRegistrationAllowed);

	if (NULL == pPlugDef)
		return AAFRESULT_NULL_PARAM;

	_containerDefinitions.appendValue(pPlugDef);
	// trr - We are saving a copy of pointer in _pluginDefinitions
	// so we need to bump its reference count.
	pPlugDef->AcquireReference();
	
	return(AAFRESULT_SUCCESS);
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupContainerDef
	  (const aafUID_t & defID,
	   ImplAAFContainerDef **ppResult)
{
  if (!ppResult) return AAFRESULT_NULL_PARAM;

	AAFRESULT result = AAFRESULT_SUCCESS;
  // NOTE: The following type cast is temporary. It should be removed as soon
	// as the OM has a declarative sytax to include the type
	// of the key used in the set. (trr:2000-FEB-29)
	if (_containerDefinitions.find((*reinterpret_cast<const OMObjectIdentification *>(&defID)),
                             *ppResult))
	{
		assert(NULL != *ppResult);
		(*ppResult)->AcquireReference();
	}
	else
	{
		// no recognized class guid in dictionary
		result = AAFRESULT_NO_MORE_OBJECTS;
	}

	return (result);
}

AAFRESULT
    ImplAAFDictionary::GetNumContainerDefs(aafUInt32 *  pNumContainerDefs)
{
  size_t siz;

  if(pNumContainerDefs == NULL)
    return AAFRESULT_NULL_PARAM;
  
  siz = _containerDefinitions.count();
  
  *pNumContainerDefs = siz;
  return AAFRESULT_SUCCESS;
}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::GetContainerDefs (
      ImplEnumAAFContainerDefs **ppEnum)
{
	if (NULL == ppEnum)
		return AAFRESULT_NULL_PARAM;
	*ppEnum = 0;
	
	ImplEnumAAFContainerDefs *theEnum = (ImplEnumAAFContainerDefs *)CreateImpl (CLSID_EnumAAFContainerDefs);
	
	XPROTECT()
	{
		OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFContainerDef>* iter = 
			new OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFContainerDef>(_containerDefinitions);
		if(iter == 0)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(theEnum->SetIterator(this, iter));
		*ppEnum = theEnum;
	}
	XEXCEPT
	{
		if (theEnum)
		  {
			theEnum->ReleaseReference();
			theEnum = 0;
		  }
		return(XCODE());
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::CountContainerDefs
        (aafUInt32 * pResult)
{
  if (! pResult)
	return AAFRESULT_NULL_PARAM;
  *pResult = _containerDefinitions.count();
  return AAFRESULT_SUCCESS;
}



void ImplAAFDictionary::InitBuiltins()
{
  ImplAAFDataDef	*dataDef = NULL;
  AAFRESULT		hr;

  hr = LookupDataDef (DDEF_Picture, &dataDef);
  if (AAFRESULT_FAILED (hr))
	{
	  // not already in dictionary
	  hr = GetBuiltinDefs()->cdDataDef()->
		CreateInstance ((ImplAAFObject **)&dataDef);
	  hr = dataDef->Initialize (DDEF_Picture, L"Picture", L"Picture data");
	  hr = RegisterDataDef (dataDef);
	}
  dataDef->ReleaseReference();
  dataDef = NULL;

  hr = LookupDataDef (DDEF_Sound, &dataDef);
  if (AAFRESULT_FAILED (hr))
	{
	  // not already in dictionary
	  hr = GetBuiltinDefs()->cdDataDef()->
		CreateInstance ((ImplAAFObject **)&dataDef);
	  hr = dataDef->Initialize (DDEF_Sound, L"Sound", L"Sound data");
	  hr = RegisterDataDef (dataDef);
	}
  dataDef->ReleaseReference();
  dataDef = NULL;

  hr = LookupDataDef (DDEF_Timecode, &dataDef);
  if (AAFRESULT_FAILED (hr))
	{
	  // not already in dictionary
	  hr = GetBuiltinDefs()->cdDataDef()->
		CreateInstance ((ImplAAFObject **)&dataDef);
	  hr = dataDef->Initialize (DDEF_Timecode, L"Timecode", L"Timecode data");
	  hr = RegisterDataDef (dataDef);
	}
  dataDef->ReleaseReference();
  dataDef = NULL;

  hr = LookupDataDef (DDEF_Edgecode, &dataDef);
  if (AAFRESULT_FAILED (hr))
	{
	  // not already in dictionary
	  hr = GetBuiltinDefs()->cdDataDef()->
		CreateInstance ((ImplAAFObject **)&dataDef);
	  hr = dataDef->Initialize (DDEF_Edgecode, L"Edgecode", L"Edgecode data");
	  hr = RegisterDataDef (dataDef);
	}
  dataDef->ReleaseReference();
  dataDef = NULL;
  
  hr = LookupDataDef (DDEF_Matte, &dataDef);
  if (AAFRESULT_FAILED (hr))
	{
	  // not already in dictionary
	  hr = GetBuiltinDefs()->cdDataDef()->
		CreateInstance ((ImplAAFObject **)&dataDef);
	  hr = dataDef->Initialize (DDEF_Matte, L"Matte", L"Matte data");
	  hr = RegisterDataDef (dataDef);
	}
  dataDef->ReleaseReference();
  dataDef = NULL;

  hr = LookupDataDef (DDEF_PictureWithMatte, &dataDef);
  if (AAFRESULT_FAILED (hr))
	{
	  // not already in dictionary
	  hr = GetBuiltinDefs()->cdDataDef()->
		CreateInstance ((ImplAAFObject **)&dataDef);
	  hr = dataDef->Initialize (DDEF_PictureWithMatte, L"PictureWithMatte", L"PictureWithMatte data");
	  hr = RegisterDataDef (dataDef);
	}
  dataDef->ReleaseReference();
  dataDef = NULL;

  _pBuiltinClasses;

  AssureClassPropertyTypes ();
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


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::RegisterInterpolationDef (
      ImplAAFInterpolationDef *pInterpolationDef)
{
  assert (_defRegistrationAllowed);

	if (NULL == pInterpolationDef)
		return AAFRESULT_NULL_PARAM;

	_interpolationDefinitions.appendValue(pInterpolationDef);
	// trr - We are saving a copy of pointer in _pluginDefinitions
	// so we need to bump its reference count.
	pInterpolationDef->AcquireReference();
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupInterpolationDef (
      const aafUID_t & interpolationID,
      ImplAAFInterpolationDef **ppInterpolationDef)
{
  if (!ppInterpolationDef) return AAFRESULT_NULL_PARAM;

	AAFRESULT result = AAFRESULT_SUCCESS;
  // NOTE: The following type cast is temporary. It should be removed as soon
	// as the OM has a declarative sytax to include the type
	// of the key used in the set. (trr:2000-FEB-29)
	if (_interpolationDefinitions.find((*reinterpret_cast<const OMObjectIdentification *>(&interpolationID)),
                             *ppInterpolationDef))
	{
		assert(NULL != *ppInterpolationDef);
		(*ppInterpolationDef)->AcquireReference();
	}
	else
	{
		// no recognized class guid in dictionary
		result = AAFRESULT_NO_MORE_OBJECTS;
	}

	return (result);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::GetInterpolationDefs (
      ImplEnumAAFInterpolationDefs **ppEnum)
{
	if (NULL == ppEnum)
		return AAFRESULT_NULL_PARAM;
	*ppEnum = 0;
	
	ImplEnumAAFInterpolationDefs *theEnum = (ImplEnumAAFInterpolationDefs *)CreateImpl (CLSID_EnumAAFInterpolationDefs);
	
	XPROTECT()
	{
		OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFInterpolationDef>* iter = 
			new OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFInterpolationDef>(_interpolationDefinitions);
		if(iter == 0)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(theEnum->SetIterator(this, iter));
		*ppEnum = theEnum;
	}
	XEXCEPT
	{
		if (theEnum)
		  {
			theEnum->ReleaseReference();
			theEnum = 0;
		  }
		return(XCODE());
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::CountInterpolationDefs
        (aafUInt32 * pResult)
{
  if (! pResult)
	return AAFRESULT_NULL_PARAM;
  *pResult = _interpolationDefinitions.count();
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::RegisterPluginDef (		//!!! Bring this out through the IDL
      ImplAAFPluginDescriptor *pDesc)
{
  assert (_defRegistrationAllowed);

	if (NULL == pDesc)
		return AAFRESULT_NULL_PARAM;

	_pluginDefinitions.appendValue(pDesc);
	// trr - We are saving a copy of pointer in _pluginDefinitions
	// so we need to bump its reference count.
	pDesc->AcquireReference();
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupPluginDef (		//!!! Bring this out through the IDL
      const aafUID_t & interpolationID,
      ImplAAFPluginDescriptor **ppPluginDesc)
{
  if (!ppPluginDesc) return AAFRESULT_NULL_PARAM;

	AAFRESULT result = AAFRESULT_SUCCESS;
  // NOTE: The following type cast is temporary. It should be removed as soon
	// as the OM has a declarative sytax to include the type
	// of the key used in the set. (trr:2000-FEB-29)
	if (_pluginDefinitions.find((*reinterpret_cast<const OMObjectIdentification *>(&interpolationID)),
                             *ppPluginDesc))
	{
		assert(NULL != *ppPluginDesc);
		(*ppPluginDesc)->AcquireReference();
	}
	else
	{
		// no recognized class guid in dictionary
		result = AAFRESULT_NO_MORE_OBJECTS;
	}

	return (result);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::GetPluginDefs (		//!!! Bring this out through the IDL
      ImplEnumAAFPluginDescriptors **ppEnum)
{
	if (NULL == ppEnum)
		return AAFRESULT_NULL_PARAM;
	*ppEnum = 0;
	
	ImplEnumAAFPluginDescriptors *theEnum = (ImplEnumAAFPluginDescriptors *)CreateImpl (CLSID_EnumAAFPluginDescriptors);
	
	XPROTECT()
	{
		OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFPluginDescriptor>* iter = 
			new OMStrongReferenceSetIterator<OMUniqueObjectIdentification, ImplAAFPluginDescriptor>(_pluginDefinitions);
		if(iter == 0)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(theEnum->SetIterator(this, iter));
		*ppEnum = theEnum;
	}
	XEXCEPT
	{
		if (theEnum)
		  {
			theEnum->ReleaseReference();
			theEnum = 0;
		  }
		return(XCODE());
	}
	XEND;
	
	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::CountPluginDefs
        (aafUInt32 * pResult)
{
  if (! pResult)
	return AAFRESULT_NULL_PARAM;
  *pResult = _pluginDefinitions.count();
  return AAFRESULT_SUCCESS;
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
  hr = _pBuiltinClasses->LookupOmPid (rAuid, result);
  if (AAFRESULT_SUCCEEDED (hr))
	{
	  rOutPid = result;
	}
  else
	{
	  // Generate an om pid for user-extended properties (either in
	  // user-extended classes, or from user-added properties to
	  // existing classes).
	  //
	  // OM PID rules:
	  // - guaranteed to be unique within this file
	  // - not guaranteed to be unique across files
	  // - all builtin properties have a fixed prop<->PID mapping
	  // - all user properties not guaranted a mapping across files
	  // - all builtin properties have *non-negative* PIDs
	  // - all user properties have *negative* PIDs.
	  // 
	  // Specifics of this implementation:
	  // If _lastGeneratedPid is zero, it means this is the first time
	  // we've tried to do this since this dictionary was instantiated
	  // in this memory space.  That could have occurred when opening
	  // an existing file, which potentially has user PIDs already
	  // allocated.  Therefore we'll have to look through all existing
	  // properties to determine what user PIDs have already been
	  // allocated.
	  //

	  // _lastGeneratedPid cannot be positive!
	  assert (0 >= _lastGeneratedPid);

	  if (!_lastGeneratedPid)
		{
		  // We haven't yet cached the user pids used in this file.  Do it now!
		  assert (! _lastGeneratedPid);

		  // must be signed!
		  aafInt32 tmpUserPid = 0;
		  // Make sure we aren't cheating ourselves
		  assert (sizeof (OMPropertyId) == sizeof (aafInt32));

		  // Need to determine which user PIDs have already been used.
		  ImplEnumAAFClassDefsSP enumClassDefs;

		  hr = GetClassDefs (&enumClassDefs);
		  if (AAFRESULT_FAILED (hr))
			return hr;
		  ImplAAFClassDefSP classDef;
		  while (AAFRESULT_SUCCEEDED
				 (enumClassDefs->NextOne (&classDef)))
			{
			  ImplEnumAAFPropertyDefsSP enumPropDefs;
			  hr = classDef->GetPropertyDefs (&enumPropDefs);
			  if (AAFRESULT_FAILED (hr))
				return hr;
			  ImplAAFPropertyDefSP propDef;
			  while (AAFRESULT_SUCCEEDED
					 (enumPropDefs->NextOne (&propDef)))
				{
				  // must be signed!
				  aafInt32 tmpPid = (aafInt32) propDef->OmPid ();
				  if (tmpPid < tmpUserPid)
					tmpUserPid = tmpPid;
				}
			}
		  assert (tmpUserPid <= 0);
		  _lastGeneratedPid = tmpUserPid;
		}

	  result = (OMPropertyId) --_lastGeneratedPid;
	  assert (((aafInt32) result) < 0);
	  rOutPid = result;
	}
  return AAFRESULT_SUCCESS;
}
void ImplAAFDictionary::pvtAttemptBuiltinSizeRegistration
  (ImplAAFTypeDefEnum * ptde) const
{
  ImplAAFBuiltinTypes::RegisterExistingType (ptde);
}


void ImplAAFDictionary::pvtAttemptBuiltinSizeRegistration
  (ImplAAFTypeDefRecord * ptdr) const
{
  ImplAAFBuiltinTypes::RegisterExistingType (ptdr);
}


void ImplAAFDictionary::AssurePropertyTypes (ImplAAFClassDef * pcd)
{
  assert (pcd);
  if (_OKToAssurePropTypes)
	{
	  pcd->AssurePropertyTypesLoaded ();
	}
}


void ImplAAFDictionary::AssureClassPropertyTypes ()
{
  AAFRESULT hr;
  ImplEnumAAFClassDefsSP enumClassDefs;
  ImplAAFClassDefSP classDef;

  _OKToAssurePropTypes = true;

  hr = GetClassDefs (&enumClassDefs);
  assert (AAFRESULT_SUCCEEDED (hr));

  // do axiomatic classes
  _pBuiltinClasses->AssurePropertyTypes ();

  // do registered (normal) classes
  while (AAFRESULT_SUCCEEDED
		 (enumClassDefs->NextOne (&classDef)))
	{
	  assert (classDef);
	  classDef->AssurePropertyTypesLoaded ();
	  classDef = 0;
	}
}


bool ImplAAFDictionary::SetEnableDefRegistration (bool isEnabled)
{
  bool retval = _defRegistrationAllowed;
  _defRegistrationAllowed = isEnabled;
  return retval;
}


ImplAAFBuiltinDefs * ImplAAFDictionary::GetBuiltinDefs ()
{
  if (! _pBuiltinDefs)
	{
	  _pBuiltinDefs = new ImplAAFBuiltinDefs (this);
	}
  assert (_pBuiltinDefs);
  return _pBuiltinDefs;
}

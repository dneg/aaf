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

  _pBuiltinClasses = new ImplAAFBuiltinClasses (this);
  _pBuiltinTypes   = new ImplAAFBuiltinTypes (this);
}


ImplAAFDictionary::~ImplAAFDictionary ()
{
  size_t i;
  // Release the _codecDefinitions
	OMStrongReferenceSetIterator<ImplAAFCodecDef>codecDefinitions(_codecDefinitions);
	while(++codecDefinitions)
	{
		ImplAAFCodecDef *pCodec = codecDefinitions.setValue(0);
		if (pCodec)
		{
		  pCodec->ReleaseReference();
		  pCodec = 0;
		}
	}

	OMStrongReferenceSetIterator<ImplAAFContainerDef>containerDefinitions(_containerDefinitions);
	while(++containerDefinitions)
	{
		ImplAAFContainerDef *pContainer = containerDefinitions.setValue(0);
		if (pContainer)
		{
		  pContainer->ReleaseReference();
		  pContainer = 0;
		}
	}

	OMStrongReferenceSetIterator<ImplAAFTypeDef>typeDefinitions(_typeDefinitions);
	while(++typeDefinitions)
	{
		ImplAAFTypeDef *pType = typeDefinitions.setValue(0);
		if (pType)
		{
		  pType->ReleaseReference();
		  pType = 0;
		}
	}

	OMStrongReferenceSetIterator<ImplAAFClassDef>classDefinitions(_classDefinitions);
	while(++classDefinitions)
	{
		ImplAAFClassDef *pClass = classDefinitions.setValue(0);
		if (pClass)
		{
		  pClass->ReleaseReference();
		  pClass = 0;
		}
	}

	size_t opsDefSize = _operationDefinitions.getSize();
  for (i = 0; i < opsDefSize; i++)
	{
	  ImplAAFOperationDef *pOps = _operationDefinitions.setValueAt(0, i);
	  if (pOps)
		{
		  pOps->ReleaseReference();
		  pOps = 0;
		}
	}
  size_t parmDefSize = _parameterDefinitions.getSize();
  for (i = 0; i < parmDefSize; i++)
	{
	  ImplAAFParameterDef *pParm = _parameterDefinitions.setValueAt(0, i);
	  if (pParm)
		{
		  pParm->ReleaseReference();
		  pParm = 0;
		}
	}
  size_t interpDefSize = _interpolationDefinitions.getSize();
  for (i = 0; i < interpDefSize; i++)
	{
	  ImplAAFInterpolationDef *pInterp = _interpolationDefinitions.setValueAt(0, i);
	  if (pInterp)
		{
		  pInterp->ReleaseReference();
		  pInterp = 0;
		}
	}

  size_t pluginDescSize = _pluginDefinitions.getSize();
  for (i = 0; i < pluginDescSize; i++)
	{
	  ImplAAFPluginDescriptor *pDesc = _pluginDefinitions.setValueAt(0, i);
	  if (pDesc)
		{
		  pDesc->ReleaseReference();
		  pDesc = 0;
		}
	}

	size_t dataDefSize = _dataDefinitions.getSize();
  for (i = 0; i < dataDefSize; i++)
	{
	  ImplAAFDataDef *pOps = _dataDefinitions.setValueAt(0, i);
	  if (pOps)
		{
		  pOps->ReleaseReference();
		  pOps = 0;
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
  ImplAAFClassDefSP pcd;
  ImplAAFDictionary * pNonConstThis = (ImplAAFDictionary*) this;
  hr = pNonConstThis->LookupClassDef(*auid, &pcd);
  assert (AAFRESULT_SUCCEEDED (hr));
  
  return CreateAndInit (pcd);
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
		  pcd->pvtGetParentAUID (parentAUID);
		  const aafUID_t kNullUID = { 0 } ;
		  if (EqualAUID (&parentAUID, &kNullUID))
			{
			  // Class was apparently registered, but no appropriate
			  // parent class found!  This should not happen, as every
			  // registered class must have a registered parent class.
			  // The only exception is AAFObject, which would have
			  // been found by the earlier
			  // pvtCreateBaseClassInstance() call.
			  assert (0);
			}
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
  
  ImplAAFClassDefSP pClassDef;
  AAFRESULT hr;
  hr = LookupClassDef (classId, &pClassDef);
  if (AAFRESULT_FAILED (hr))
	return hr;

  *ppvObject = CreateAndInit (pClassDef);

  if (NULL == *ppvObject)
    return AAFRESULT_INVALID_CLASS_ID;
  else
    return AAFRESULT_SUCCESS;
}


// internal utility factory method to create an ImplAAFObject given a classdef.
// This method was created to make it simpler to replace calls to "Deprecated"
// call to CreateImpl which should only be used for instanciating transient
// non-ImplAAFObject classes such as an enumerator.
ImplAAFObject *ImplAAFDictionary::CreateImplObject(aafUID_constref classID)
{
  ImplAAFObject *pObject = NULL;
  AAFRESULT result = AAFRESULT_SUCCESS;
  
  result = CreateInstance(classID, &pObject);
  assert(AAFRESULT_SUCCEEDED(result));
  return pObject;
}



AAFRESULT ImplAAFDictionary::dictLookupClassDef (
      const aafUID_t & classID,
      ImplAAFClassDef ** ppClassDef)
{
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
	  CHECK(GetClassDefs (&classEnum));
	  status = classEnum->NextOne (&classDef);
	  defFound = kAAFFalse;
	  while(status == AAFRESULT_SUCCESS && !defFound)
		{
		  CHECK(classDef->GetAUID (&testAUID));
		  if(EqualAUID(&classID, &testAUID))
			{
			  defFound = kAAFTrue;
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
		{
		  classEnum->ReleaseReference();
		  classEnum = 0;
		}
	  if(classDef != NULL)
		{
		  classDef->ReleaseReference();
		  classDef = 0;
		}
	}
  XEND
	
	return(AAFRESULT_SUCCESS);
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
	  assert (*ppClassDef);
	  (*ppClassDef)->AcquireReference ();
	  return true;
	}
  return false;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::LookupClassDef (
      const aafUID_t & classID,
      ImplAAFClassDef ** ppClassDef)
{
  ImplAAFClassDefSP			classDef;
  AAFRESULT					status;

  if (! ppClassDef) return AAFRESULT_NULL_PARAM;

  if (pvtLookupAxiomaticClassDef (classID, &classDef))
	{
	  // Yes, this is an axiomatic class.  classDef should be filled
	  // in.  Assure that it's in the dictionary, and return it.

	  // Future! here's where we assure it's in the dictionary.
	  // if (! class-present-in-dictionary)
	  // {
	  //   put-it-into-the-dictionary();
	  // }

	  AssurePropertyTypes (classDef);

	  assert (ppClassDef);
	  *ppClassDef = classDef;
	  assert (*ppClassDef);
	  (*ppClassDef)->AcquireReference ();
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
  status = _pBuiltinClasses->NewBuiltinClassDef (classID, &classDef);
  if (AAFRESULT_FAILED (status))
	{
	  // no recognized class guid
	  return AAFRESULT_NO_MORE_OBJECTS;
	}

  // Yup, found it in builtins.  Register it.
  assert (classDef);
  status = RegisterClassDef (classDef);
  if (AAFRESULT_FAILED (status))
	return status;
		  
  AssurePropertyTypes (classDef);
  assert (ppClassDef);
  *ppClassDef = classDef;
  (*ppClassDef)->AcquireReference ();
	
  return(AAFRESULT_SUCCESS);
}


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
  ImplAAFClassDef * pExistingClassDef = NULL;
  hr = dictLookupClassDef (newAUID, &pExistingClassDef);

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
	  pExistingClassDef = 0;
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
		OMStrongReferenceSetIterator<ImplAAFClassDef>* iter = 
			new OMStrongReferenceSetIterator<ImplAAFClassDef>(_classDefinitions);
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
  return AAFRESULT_NOT_IMPLEMENTED;
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
  ImplAAFTypeDef * pExistingTypeDef = NULL;
  hr = dictLookupTypeDef (newAUID, &pExistingTypeDef);

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
	pExistingTypeDef = 0;
  }

  return(AAFRESULT_SUCCESS);
}


AAFRESULT ImplAAFDictionary::dictLookupTypeDef (
      const aafUID_t & typeID,
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
	  CHECK(GetTypeDefs (&typeEnum));
	  status = typeEnum->NextOne (&typeDef);
	  defFound = kAAFFalse;
	  while(status == AAFRESULT_SUCCESS && !defFound)
		{
		  CHECK(typeDef->GetAUID (&testAUID));
		  if(EqualAUID(&typeID, &testAUID))
			{
			  defFound = kAAFTrue;
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
		{
		  typeEnum->ReleaseReference();
		  typeEnum = 0;
		}
	  if(typeDef != NULL)
		{
		  typeDef->ReleaseReference();
		  typeDef = 0;
		}
	}
  XEND
	
	return(AAFRESULT_SUCCESS);
}


/*static*/
const aafUID_t * ImplAAFDictionary::sAxiomaticTypeGuids[] = 
{
  & kAAFTypeID_AUID,
  & kAAFTypeID_Boolean,
  & kAAFTypeID_Character,
  & kAAFTypeID_DateStruct,
  & kAAFTypeID_TimeStruct,
  & kAAFTypeID_Int16,
  & kAAFTypeID_Int32,
  & kAAFTypeID_Int64,
  & kAAFTypeID_Int64Array,
  & kAAFTypeID_Int8,
  & kAAFTypeID_String,
  & kAAFTypeID_TimeStamp,
  & kAAFTypeID_UInt16,
  & kAAFTypeID_UInt32,
  & kAAFTypeID_UInt8,
  & kAAFTypeID_UInt8Array8,
  & kAAFTypeID_VersionType,
  & kAAFTypeID_RGBAComponent,
  & kAAFTypeID_MobID,
  & kAAFTypeID_DataValue,
 
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

  if (! ppTypeDef) return AAFRESULT_NULL_PARAM;

  if (pvtLookupAxiomaticTypeDef (typeID, &typeDef))
	{
	  // Yes, this is an axiomatic type.  typeDef should be filled
	  // in.  Assure that it's in the dictionary, and return it.

	  // Future! here's where we assure it's in the dictionary.
	  // if (! type-present-in-dictionary)
	  // {
	  //   put-it-into-the-dictionary();
	  // }

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
  assert (typeDef);
  status = RegisterTypeDef (typeDef);
  if (AAFRESULT_FAILED (status))
	return status;
		  
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
		OMStrongReferenceSetIterator<ImplAAFTypeDef>* iter = 
			new OMStrongReferenceSetIterator<ImplAAFTypeDef>(_typeDefinitions);
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
  return AAFRESULT_NOT_IMPLEMENTED;
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
	ImplEnumAAFDataDefs		*dataEnum = NULL;
	ImplAAFDataDef			*dataDef = NULL;
	aafBool						defFound;
	AAFRESULT					status;
	aafUID_t					testAUID;

	XPROTECT()
	{
		CHECK(GetDataDefs (&dataEnum));
		status = dataEnum->NextOne (&dataDef);
		defFound = kAAFFalse;
		while(status == AAFRESULT_SUCCESS && !defFound)
		{
			CHECK(dataDef->GetAUID (&testAUID));
			if(EqualAUID(&dataDefinitionID, &testAUID))
			{
				defFound = kAAFTrue;
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
		  {
			dataEnum->ReleaseReference();
			dataEnum = 0;
		  }
		if(dataDef != NULL)
		  {
			dataDef->ReleaseReference();
			dataDef = 0;
		  }
	}
	XEND
	
	return(AAFRESULT_SUCCESS);
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
		CHECK(theEnum->SetEnumStrongProperty(this, &_dataDefinitions));
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
  return AAFRESULT_NOT_IMPLEMENTED;
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
	ImplEnumAAFOperationDefs	*effectEnum = NULL;
	ImplAAFOperationDef			*effectDef = NULL;
	aafBool						defFound;
	AAFRESULT					status;
	aafUID_t					testAUID;

	XPROTECT()
	{
		CHECK(GetOperationDefs (&effectEnum));
		status = effectEnum->NextOne (&effectDef);
		defFound = kAAFFalse;
		while(status == AAFRESULT_SUCCESS && !defFound)
		{
			CHECK(effectDef->GetAUID (&testAUID));
			if(EqualAUID(&effectID, &testAUID))
			{
				defFound = kAAFTrue;
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
		  {
			effectEnum->ReleaseReference();
			effectEnum = 0;
		  }
		if(effectDef != NULL)
		  {
			effectDef->ReleaseReference();
			effectDef = 0;
		  }
	}
	XEND
	
	return(AAFRESULT_SUCCESS);
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
		CHECK(theEnum->SetEnumStrongProperty(this, &_operationDefinitions));
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
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFDictionary::RegisterParameterDef (
      ImplAAFParameterDef *pParameterDef)
{
  assert (_defRegistrationAllowed);

	if (NULL == pParameterDef)
		return AAFRESULT_NULL_PARAM;

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
	ImplEnumAAFParameterDefs		*parameterEnum = NULL;
	ImplAAFParameterDef			*parameterDef = NULL;
	aafBool						defFound;
	AAFRESULT					status;
	aafUID_t					testAUID;

	XPROTECT()
	{
		CHECK(GetParameterDefs (&parameterEnum));
		status = parameterEnum->NextOne (&parameterDef);
		defFound = kAAFFalse;
		while(status == AAFRESULT_SUCCESS && !defFound)
		{
			CHECK(parameterDef->GetAUID (&testAUID));
			if(EqualAUID(&parameterID, &testAUID))
			{
				defFound = kAAFTrue;
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
		  {
			parameterEnum->ReleaseReference();
			parameterEnum = 0;
		  }
		if(parameterDef != NULL)
		  {
			parameterDef->ReleaseReference();
			parameterDef = 0;
		  }
	}
	XEND
	
	return(AAFRESULT_SUCCESS);
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
		CHECK(theEnum->SetEnumStrongProperty(this, &_parameterDefinitions));
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
  return AAFRESULT_NOT_IMPLEMENTED;
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
   ImplAAFCodecDef **result)
{
	ImplEnumAAFCodecDefs		*codecEnum = NULL;
	ImplAAFCodecDef				*codecDef = NULL;
	aafBool						defFound;
	AAFRESULT					status;
	aafUID_t					testAUID;

	XPROTECT()
	{
		CHECK(GetCodecDefs (&codecEnum));
		status = codecEnum->NextOne (&codecDef);
		defFound = kAAFFalse;
		while(status == AAFRESULT_SUCCESS && !defFound)
		{
			CHECK(codecDef->GetAUID (&testAUID));
			if(EqualAUID(&defID, &testAUID))
			{
				defFound = kAAFTrue;
				*result = codecDef;
				codecDef->AcquireReference();
				break;
			}
			codecDef->ReleaseReference();
			codecDef = NULL;
			status = codecEnum->NextOne (&codecDef);
		}
		if(codecDef != NULL)
		{
			codecDef->ReleaseReference();
			codecDef = NULL;
		}
		codecEnum->ReleaseReference();
		codecEnum = NULL;
		if(!defFound)
			 RAISE(AAFRESULT_NO_MORE_OBJECTS);
	}
	XEXCEPT
	{
		if(codecEnum != NULL)
		  {
			codecEnum->ReleaseReference();
			codecEnum = 0;
		  }
		if(codecDef != NULL)
		  {
			codecDef->ReleaseReference();
			codecDef = 0;
		  }
	}
	XEND
	
	return(AAFRESULT_SUCCESS);
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
		OMStrongReferenceSetIterator<ImplAAFCodecDef>* iter = 
			new OMStrongReferenceSetIterator<ImplAAFCodecDef>(_codecDefinitions);
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
  return AAFRESULT_NOT_IMPLEMENTED;
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
	   ImplAAFContainerDef **result)
{
  if (!result) return AAFRESULT_NULL_PARAM;

  ImplEnumAAFContainerDefs		*containerEnum = NULL;
  ImplAAFContainerDef			*containerDef = NULL;
  aafBool						defFound;
  AAFRESULT					status;
  aafUID_t					testAUID;

  if (! result)
	return AAFRESULT_NULL_PARAM;

  XPROTECT()
	{
	  CHECK(GetContainerDefs (&containerEnum));
	  status = containerEnum->NextOne (&containerDef);
	  defFound = kAAFFalse;
	  while(status == AAFRESULT_SUCCESS && !defFound)
		{
		  CHECK(containerDef->GetAUID (&testAUID));
		  if(EqualAUID(&defID, &testAUID))
			{
			  defFound = kAAFTrue;
			  *result = containerDef;
			  containerDef->AcquireReference();
			  break;
			}
		  containerDef->ReleaseReference();
		  containerDef = NULL;
		  status = containerEnum->NextOne (&containerDef);
		}
	  if(containerDef != NULL)
		{
		  containerDef->ReleaseReference();
		  containerDef = NULL;
		}
	  containerEnum->ReleaseReference();
	  containerEnum = NULL;
	  if(!defFound)
		{
		  // no recognized class guid in dictionary
		  RAISE(AAFRESULT_NO_MORE_OBJECTS);
		}
	}
  XEXCEPT
	{
	  if(containerEnum != NULL)
		{
		  containerEnum->ReleaseReference();
		  containerEnum = 0;
		}
	  if(containerDef != NULL)
		{
		  containerDef->ReleaseReference();
		  containerDef = 0;
		}
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
		OMStrongReferenceSetIterator<ImplAAFContainerDef>* iter = 
			new OMStrongReferenceSetIterator<ImplAAFContainerDef>(_containerDefinitions);
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
  return AAFRESULT_NOT_IMPLEMENTED;
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
	ImplEnumAAFInterpolationDefs		*InterpolationEnum = NULL;
	ImplAAFInterpolationDef			*InterpolationDef = NULL;
	aafBool						defFound;
	AAFRESULT					status;
	aafUID_t					testAUID;

	XPROTECT()
	{
		CHECK(GetInterpolationDefs (&InterpolationEnum));
		status = InterpolationEnum->NextOne (&InterpolationDef);
		defFound = kAAFFalse;
		while(status == AAFRESULT_SUCCESS && !defFound)
		{
			CHECK(InterpolationDef->GetAUID (&testAUID));
			if(EqualAUID(&interpolationID, &testAUID))
			{
				defFound = kAAFTrue;
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
		  {
			InterpolationEnum->ReleaseReference();
			InterpolationEnum = 0;
		  }
		if(InterpolationDef != NULL)
		  {
			InterpolationDef->ReleaseReference();
			InterpolationDef = 0;
		  }
	}
	XEND
	
	return(AAFRESULT_SUCCESS);
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
		CHECK(theEnum->SetEnumStrongProperty(this, &_interpolationDefinitions));
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
  return AAFRESULT_NOT_IMPLEMENTED;
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
	ImplEnumAAFPluginDescriptors		*pEnum = NULL;
	ImplAAFPluginDescriptor			*pDesc = NULL;
	aafBool						defFound;
	AAFRESULT					status;
	aafUID_t					testAUID;

	XPROTECT()
	{
		CHECK(GetPluginDefs (&pEnum));
		status = pEnum->NextOne (&pDesc);
		defFound = kAAFFalse;
		while(status == AAFRESULT_SUCCESS && !defFound)
		{
			CHECK(pDesc->GetAUID (&testAUID));
			if(EqualAUID(&interpolationID, &testAUID))
			{
				defFound = kAAFTrue;
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
		  {
			pEnum->ReleaseReference();
			pEnum = 0;
		  }
		if(pDesc != NULL)
		  {
			pDesc->ReleaseReference();
			pDesc = 0;
		  }
	}
	XEND
	
	return(AAFRESULT_SUCCESS);
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
		CHECK(theEnum->SetEnumStrongProperty(this, &_pluginDefinitions));
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
  return AAFRESULT_NOT_IMPLEMENTED;
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

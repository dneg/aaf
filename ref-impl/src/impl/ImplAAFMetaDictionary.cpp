/***********************************************************************
 *
 *              Copyright (c) 1998-2000 Avid Technology, Inc.
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


#ifndef __ImplAAFMetaDictionary_h__
#include "ImplAAFMetaDictionary.h"
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


//#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"
//#include "ImplAAFObjectCreation.h"
//#include "AAFClassDefUIDs.h"

#include "ImplAAFObjectCreation.h"
#include "aafErr.h"

#include <assert.h>

extern "C" const aafClassID_t CLSID_EnumAAFClassDefs;
extern "C" const aafClassID_t CLSID_EnumAAFTypeDefs;


ImplAAFMetaDictionary::ImplAAFMetaDictionary () :
  _typeDefinitions  (PID_MetaDictionary_TypeDefinitions,
                     "TypeDefinitions", 
                     PID_MetaDefinition_Identification),
  _classDefinitions (PID_MetaDictionary_ClassDefinitions,
                     "ClassDefinitions", 
                     PID_MetaDefinition_Identification)
{
  _persistentProperties.put (_typeDefinitions.address());
  _persistentProperties.put (_classDefinitions.address());
}


ImplAAFMetaDictionary::~ImplAAFMetaDictionary ()
{
  // Release the _opaqueTypeDefinitions
  OMSetIterator<OMUniqueObjectIdentification, OpaqueTypeDefinition>opaqueTypeDefinitions(_opaqueTypeDefinitions, OMBefore);
  while(++opaqueTypeDefinitions)
  {
    // NOTE: Temporary OpaqueTypeDefinitions are created to hold the type definition
    // pointers.           |
    //                     V
	  ImplAAFTypeDef *pType = opaqueTypeDefinitions.value();
    if (pType)
    {
		  pType->ReleaseReference();
		  pType = 0;
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
}


//
// Create an instance of the appropriate derived class, given the class id.
//  This method implements the OMClassFactory interface.
//
//OMStorable* ImplAAFMetaDictionary::create(const OMClassId& /*classId*/) const
//{
//  return NULL;
//}


//
// Define the symbol for the stored object id
//
//const OMClassId& ImplAAFMetaDictionary::classId(void) const
//{
//  return *reinterpret_cast<const OMClassId* const>(&AUID_AAFMetaDictionary);
//}


//void ImplAAFMetaDictionary::onSave(void* /*clientContext*/) const
//{
//}


//////////////////////////////
//
//////////////////////////////



ImplAAFMetaDictionary::ForwardClassReference::ForwardClassReference()
{
}

ImplAAFMetaDictionary::ForwardClassReference::ForwardClassReference(const ImplAAFMetaDictionary::ForwardClassReference& rhs)
{
  _classId = rhs._classId;
}

ImplAAFMetaDictionary::ForwardClassReference::ForwardClassReference(aafUID_constref classId)
{
  _classId = classId;
}

const OMUniqueObjectIdentification 
  ImplAAFMetaDictionary::ForwardClassReference::identification(void) const
{
  return (*reinterpret_cast<const OMUniqueObjectIdentification *>(&_classId));
}

ImplAAFMetaDictionary::ForwardClassReference& 
  ImplAAFMetaDictionary::ForwardClassReference::operator= (const ImplAAFMetaDictionary::ForwardClassReference& rhs)
{
  _classId = rhs._classId;
  return *this;
}

bool ImplAAFMetaDictionary::ForwardClassReference::operator== (const ImplAAFMetaDictionary::ForwardClassReference& rhs)
{
  if (&rhs == this)
    return true;
  else
    return (0 == memcmp(&_classId, &rhs._classId, sizeof(_classId)));
}

bool ImplAAFMetaDictionary::hasForwardClassReference(aafUID_constref classId)
{
  // Create a temporary test value and then look for the
  // corresponding forward class reference in the set.
  ImplAAFMetaDictionary::ForwardClassReference forwardReference(classId);
  return (_forwardClassReferences.containsValue(forwardReference));
}

// Remove the give classId from the foward class reference set.
void ImplAAFMetaDictionary::RemoveForwardClassReference(aafUID_constref classId)
{
  ImplAAFMetaDictionary::ForwardClassReference forwardReference(classId);
  assert(hasForwardClassReference(classId)); // classId must already be a forward reference!
  _forwardClassReferences.removeValue(forwardReference);
}

//////////////////////////////
//
//////////////////////////////


ImplAAFMetaDictionary::OpaqueTypeDefinition::OpaqueTypeDefinition() :
  _opaqueTypeDef(NULL)
{
}

ImplAAFMetaDictionary::OpaqueTypeDefinition::OpaqueTypeDefinition(const ImplAAFMetaDictionary::OpaqueTypeDefinition& rhs)
{
  _opaqueTypeDef = rhs._opaqueTypeDef;
}

ImplAAFMetaDictionary::OpaqueTypeDefinition::OpaqueTypeDefinition(ImplAAFTypeDef * opaqueTypeDef)
{
  _opaqueTypeDef = opaqueTypeDef;
}

// coersion operator to "transparently" extract the type
// definition pointer. This will be called when the enumerator
// attempts to assign an OpaqueTypeDefinition to an ImplAAFTypeDef *.
ImplAAFMetaDictionary::OpaqueTypeDefinition::operator ImplAAFTypeDef * () const
{
  return _opaqueTypeDef;
}


const OMUniqueObjectIdentification 
  ImplAAFMetaDictionary::OpaqueTypeDefinition::identification(void) const
{
  aafUID_t typeId = {0};
	if (_opaqueTypeDef)
	{
		AAFRESULT result = _opaqueTypeDef->GetAUID(&typeId); // ignore error
		assert (AAFRESULT_SUCCEEDED(result));
	}

	return (*reinterpret_cast<const OMUniqueObjectIdentification *>(&typeId));
}

ImplAAFMetaDictionary::OpaqueTypeDefinition& 
  ImplAAFMetaDictionary::OpaqueTypeDefinition::operator= (const ImplAAFMetaDictionary::OpaqueTypeDefinition& rhs)
{
  _opaqueTypeDef = rhs._opaqueTypeDef;
  return *this;
}

bool ImplAAFMetaDictionary::OpaqueTypeDefinition::operator== (const ImplAAFMetaDictionary::OpaqueTypeDefinition& rhs)
{
  if (&rhs == this)
    return true;
  else
    return (_opaqueTypeDef == rhs._opaqueTypeDef); // pointers are equal.
}



ImplAAFTypeDef * ImplAAFMetaDictionary::findOpaqueTypeDefinition(aafUID_constref typeId)
{
  // NOTE: The following type cast is temporary. It should be removed as soon
	// as the OM has a declarative sytax to include the type
	// of the key used in the set. (trr:2000-FEB-29)
  ImplAAFMetaDictionary::OpaqueTypeDefinition opaqueTypeDefinition;
	if (_opaqueTypeDefinitions.find((*reinterpret_cast<const OMObjectIdentification *>(&typeId)),
                             opaqueTypeDefinition))
  {
    return (ImplAAFTypeDef *)opaqueTypeDefinition;
  }
  else
  {
    return NULL;
  }
}




// These are low-level OMSet tests for containment.
bool ImplAAFMetaDictionary::containsClass(aafUID_constref classId)
{
  // NOTE: The following type cast is temporary. It should be removed as soon
	// as the OM has a declarative sytax to include the type
	// of the key used in the set. (trr:2000-FEB-26)
	return(_classDefinitions.contains((*reinterpret_cast<const OMObjectIdentification *>(&classId))));
}

bool ImplAAFMetaDictionary::containsType(aafUID_constref typeId)
{
  // NOTE: The following type cast is temporary. It should be removed as soon
	// as the OM has a declarative sytax to include the type
	// of the key used in the set. (trr:2000-FEB-26)
	return(_typeDefinitions.contains((*reinterpret_cast<const OMObjectIdentification *>(&typeId))));
}



//////////////////////////////
//
//////////////////////////////

  //****************
  // LookupClassDef()
  //
AAFRESULT STDMETHODCALLTYPE
  ImplAAFMetaDictionary::LookupClassDef(
    aafUID_constref classId,
    ImplAAFClassDef ** ppClassDef)
{
  if (!ppClassDef) return AAFRESULT_NULL_PARAM;

	AAFRESULT result = AAFRESULT_SUCCESS;
  // NOTE: The following type cast is temporary. It should be removed as soon
	// as the OM has a declarative sytax to include the type
	// of the key used in the set. (trr:2000-FEB-26)
	if (_classDefinitions.find((*reinterpret_cast<const OMObjectIdentification *>(&classId)),
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

//****************
// CreateForwardClassReference()
//
AAFRESULT STDMETHODCALLTYPE
  ImplAAFMetaDictionary::CreateForwardClassReference(
    aafUID_constref classId)
{
  AAFRESULT result = AAFRESULT_SUCCESS;

  // First check to see of forward class reference has already
  // been created.
  if (hasForwardClassReference(classId))
    return AAFRESULT_INVALID_PARAM;

  // Create a local forward class reference.
  ImplAAFMetaDictionary::ForwardClassReference forwardReference(classId);

  // If the class has already been registed then we cannot
  // create a foward reference!
  if (_classDefinitions.contains(forwardReference.identification()))
    return AAFRESULT_INVALID_PARAM;

  // It now safe to add the given classId to the set.
  _forwardClassReferences.append(forwardReference); // the set will copy the value.

  return result;
}

//****************
// IsForwardClassReference()
//
AAFRESULT STDMETHODCALLTYPE
  ImplAAFMetaDictionary::HasForwardClassReference(
    aafUID_constref classId,
    aafBoolean_t *pResult)
{
  if (NULL == pResult)
    return AAFRESULT_NULL_PARAM;

  // Call the internal shared implementation.
  *pResult = hasForwardClassReference(classId);

  return AAFRESULT_SUCCESS;
}

//****************
// RegisterClassDef()
//
AAFRESULT STDMETHODCALLTYPE
  ImplAAFMetaDictionary::RegisterClassDef(
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
  if (!containsClass(newAUID))
	{
    if (hasForwardClassReference(newAUID))
    {
      // This class is now defined so it can no longer have
      // a forward reference.
      RemoveForwardClassReference(newAUID);
    }

	  // This class is not yet registered, add it to the dictionary.
	  _classDefinitions.appendValue(pClassDef);
	  pClassDef->AcquireReference();
	}

  return(AAFRESULT_SUCCESS);
}

//****************
// GetClassDefs()
//
AAFRESULT STDMETHODCALLTYPE
  ImplAAFMetaDictionary::GetClassDefs(
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

//****************
// CountClassDefs()
//
AAFRESULT STDMETHODCALLTYPE
  ImplAAFMetaDictionary::CountClassDefs(
    aafUInt32 * pResult)
{
  if (!pResult)
	  return AAFRESULT_NULL_PARAM;
	*pResult = _classDefinitions.count();
  return AAFRESULT_SUCCESS;
}

//****************
// RegisterTypeDef()
//
AAFRESULT STDMETHODCALLTYPE
  ImplAAFMetaDictionary::RegisterTypeDef(
    ImplAAFTypeDef * pTypeDef)
{
  if (NULL == pTypeDef)
	  return AAFRESULT_NULL_PARAM;
  if (pTypeDef->attached())
  {
    if (file() != pTypeDef->file())
      return AAFRESULT_OBJECT_ALREADY_ATTACHED;
  }
	
  // Get the AUID of the new type to register.
  aafUID_t newAUID;
  HRESULT hr = pTypeDef->GetAUID(&newAUID);
  if (hr != AAFRESULT_SUCCESS)
    return hr;

  // Is this type already registered ?
  if (!containsType(newAUID))
  {
		// We also need to check if the given type is an opaque type. if it is then we
		// need to remove it from the opaqueTypeDefinitions set if the registration 
    // of successful. This makes sense because the type can no longer be opaque.
    ImplAAFTypeDef *pOpaqueType = findOpaqueTypeDefinition(newAUID);
    if (pOpaqueType)
    {
      // Remove the type from the opaque list and release the reference.
      // NOTE: This release may actually delete the opaque type from memory
      // since it is possible that older pOpaqueType is not the same
      // object pointer as the newer pTypeDef pointer eventhough they
      // have the same identifier (newAUID).
      _opaqueTypeDefinitions.removeValue(pOpaqueType);
      pOpaqueType->ReleaseReference();
      pOpaqueType = NULL;
    }

    // This type is not yet registered, add it to the dictionary.
    _typeDefinitions.appendValue(pTypeDef);
    pTypeDef->AcquireReference();
  }

  return (hr);
}

//****************
// LookupTypeDef()
//
AAFRESULT STDMETHODCALLTYPE
  ImplAAFMetaDictionary::LookupTypeDef(
    aafUID_constref typeID,
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

//****************
// GetTypeDefs()
//
AAFRESULT STDMETHODCALLTYPE
  ImplAAFMetaDictionary::GetTypeDefs(
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

//****************
// CountTypeDefs()
//
AAFRESULT STDMETHODCALLTYPE
  ImplAAFMetaDictionary::CountTypeDefs(
    aafUInt32 * pResult)
{
  if (! pResult)
	  return AAFRESULT_NULL_PARAM;
	*pResult = _typeDefinitions.count();
  return AAFRESULT_SUCCESS;
}

//****************
// RegisterOpaqueTypeDef()
//
AAFRESULT STDMETHODCALLTYPE
  ImplAAFMetaDictionary::RegisterOpaqueTypeDef(
    ImplAAFTypeDef * pOpaqueTypeDef)
{
  if (NULL == pOpaqueTypeDef)
		return AAFRESULT_NULL_PARAM;
	
  // Get the AUID of the new type to register.
  aafUID_t newAUID;
  HRESULT hr = pOpaqueTypeDef->GetAUID(&newAUID);
  if (hr != AAFRESULT_SUCCESS)
    return hr;

  // Is this type already registered ?
  // NOTE: Behave like all of the other "register" methods: do not
  // fail if the type has already been registered. This is a little
  // ambiguous since the client will not know whether the the given
  // type definition was already public or already opaque...
  // (transdel:2000-APR-03)
  if (!containsType(newAUID) && !findOpaqueTypeDefinition(newAUID))
  {
    // This type is not yet registered, add it to the dictionary.
    ImplAAFMetaDictionary::OpaqueTypeDefinition opaque(pOpaqueTypeDef);
    _opaqueTypeDefinitions.append(opaque);
    pOpaqueTypeDef->AcquireReference();
  }

  return (AAFRESULT_SUCCESS);
}

//****************
// LookupOpaqueTypeDef()
//
AAFRESULT STDMETHODCALLTYPE
  ImplAAFMetaDictionary::LookupOpaqueTypeDef(
    aafUID_constref typeID,
    ImplAAFTypeDef ** ppOpaqueTypeDef)
{
  AAFRESULT status = AAFRESULT_SUCCESS;

  if (! ppOpaqueTypeDef) 
		return AAFRESULT_NULL_PARAM;

  *ppOpaqueTypeDef = findOpaqueTypeDefinition(typeID);
  if (*ppOpaqueTypeDef)
    (*ppOpaqueTypeDef)->AcquireReference ();
  else
    status = AAFRESULT_TYPE_NOT_FOUND;
	
  return (status);
}

//****************
// GetOpaqueTypeDefs()
//
AAFRESULT STDMETHODCALLTYPE
  ImplAAFMetaDictionary::GetOpaqueTypeDefs(
    ImplEnumAAFTypeDefs ** ppEnum)
{
	if (NULL == ppEnum)
		return AAFRESULT_NULL_PARAM;
	*ppEnum = 0;

#if 0
	ImplEnumAAFTypeDefs *theEnum = (ImplEnumAAFTypeDefs *)CreateImpl (CLSID_EnumAAFTypeDefs);
	
	XPROTECT()
	{
		OMSetIterator<OMUniqueObjectIdentification, ImplAAFMetaDictionary::OpaqueTypeDefinition>* iter = 
			new OMSetIterator<OMUniqueObjectIdentification, ImplAAFMetaDictionary::OpaqueTypeDefinition>(_opaqueTypeDefinitions, OMBefore);
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

//****************
// CountOpaqueTypeDefs()
//
AAFRESULT STDMETHODCALLTYPE
  ImplAAFMetaDictionary::CountOpaqueTypeDefs(
    aafUInt32 * pResult)
{
  if (! pResult)
		return AAFRESULT_NULL_PARAM;
	*pResult = _opaqueTypeDefinitions.count();
  return AAFRESULT_SUCCESS;
}


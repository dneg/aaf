/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif

#ifndef __ImplCollection_h__
#include "ImplCollection.h"
#endif

#ifndef __ImplEnumerator_h__
#include "ImplEnumerator.h"
#endif

#include <assert.h>
#include "aafErr.h"
#include "AAFResult.h"
#include "OMFile.h"
#include "ImplAAFHeader.h"
#include "ImplAAFClassDef.h"
#include "ImplAAFProperty.h"
#include "ImplAAFPropertyDef.h"
#include "ImplAAFPropertyValue.h"
#include "ImplEnumAAFProperties.h"
#include "ImplAAFBuiltins.h"

#include "ImplAAFObjectCreation.h"
#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

extern "C" const aafClassID_t CLSID_AAFProperty;
extern "C" const aafClassID_t CLSID_EnumAAFProperties;


// explicit template instantiation
// template class ImplCollection<ImplAAFProperty *>;


//
// Private class for implementing collections of properties.
//
class ImplPropertyCollection : public ImplCollection<ImplAAFProperty *>
{
public:
  ImplPropertyCollection ();
  ~ImplPropertyCollection ();

  // overrides
  virtual AAFRESULT
    GetNumElements
        (aafUInt32 * pCount);

  virtual AAFRESULT
    GetNthElement
        (aafUInt32  index,
         ImplAAFProperty* * pElem);

  AAFRESULT
    ImplPropertyCollection::Init (OMPropertySet * pPropSet);

private:
  AAFRESULT initProperty(aafUInt32 index);

  OMPropertySet *    _pPropSet;
  ImplAAFProperty ** _pProperties;
  aafUInt32          _count;
};



ImplPropertyCollection::ImplPropertyCollection ()
  : _pPropSet (0),
	_pProperties (0),
	_count (0)
{}


AAFRESULT ImplPropertyCollection::Init (OMPropertySet * pPropSet)
{
  // make sure we haven't called this before
  assert (! _pPropSet);
  assert (! _pProperties);

  if (! pPropSet)
	return AAFRESULT_NULL_PARAM;

  _pPropSet = pPropSet;

  assert (_pPropSet);
  _count = _pPropSet->count();

  _pProperties = new ImplAAFProperty * [_count];
  if (! _pProperties)
	return AAFRESULT_NOMEMORY;
  for (aafUInt32 i = 0;
	   i < _count;
	   i++)
	{
	  _pProperties[i] = NULL;
	}
  return AAFRESULT_SUCCESS;
}


ImplPropertyCollection::~ImplPropertyCollection ()
{
  if (_pProperties)
	for (aafUInt32 numProps = 0;
		 numProps < _count;
		 numProps++)
	  {
		if (_pProperties[numProps])
		  _pProperties[numProps]->ReleaseReference();
	  }
}


AAFRESULT ImplPropertyCollection::GetNumElements
(aafUInt32 * pCount)
{
  assert (_pPropSet);
  if (! pCount)
	return AAFRESULT_NULL_PARAM;
  *pCount = _count;
  return AAFRESULT_SUCCESS;
}

AAFRESULT ImplPropertyCollection::GetNthElement
(
 aafUInt32  index,
 ImplAAFProperty ** pElem
)
{
  if (! pElem)
	return AAFRESULT_NULL_PARAM;

  if (index >= _count)
	return AAFRESULT_NO_MORE_OBJECTS;

  if (! _pProperties[index])
	{
	  AAFRESULT hr = initProperty(index);
	  if (! AAFRESULT_SUCCEEDED (hr)) return hr;
	}
  assert (_pProperties[index]);
  assert (pElem);
  *pElem = _pProperties[index];
  (*pElem)->AcquireReference();
  return AAFRESULT_SUCCESS;
}


AAFRESULT ImplPropertyCollection::initProperty(aafUInt32 index)
{
  ImplAAFProperty * pProp = NULL;

  assert (index <= _count);
  assert (! _pProperties[index]);

  assert (_pPropSet);
  size_t ctx = 0;
  OMProperty * pOmProp = NULL;
  for (aafUInt32 i = 0;
       i <= index;
	   i++)
	{
	  // wasteful get of all properties until we get to the one we
	  // want
	  _pPropSet->iterate (ctx, pOmProp);
	  assert (pOmProp);
	}

  // pOmProp now points to the OM property we want.

  // Allocate a property object
  pProp = (ImplAAFProperty*) CreateImpl (CLSID_AAFProperty);
  if (! pProp)
	return E_FAIL;
  assert (pProp);

  // Get the OM's property ID
  OMPropertyId opid = pOmProp->propertyId();

  ImplAAFPropertyDef * pd = NULL;
  AAFRESULT hr = ImplAAFBuiltins::LookupPropDef (opid, &pd);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;
  assert (pd);
  pProp->Initialize (pd);
  pd->ReleaseReference();

  // get the prop value from the prop
  ImplAAFPropertyValue *pVal = NULL;
  hr = pProp->GetValue (&pVal);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;
  assert (pVal);

  // set the storage in the prop value
  size_t bitsSize;
  assert (pOmProp);
  bitsSize = pOmProp->bitsSize ();
  OMByte * pBits = NULL;
  hr = pVal->AllocateBits (bitsSize, &pBits);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;
  assert (pBits);
  pOmProp->getBits (pBits, bitsSize);

  _pProperties[index] = pProp;  
  return AAFRESULT_SUCCESS;
}



ImplAAFObject::ImplAAFObject ()
  : _pProperties (0)
{}


ImplAAFObject::~ImplAAFObject ()
{
  if (_pProperties)
	delete _pProperties;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFObject::SetGeneration (aafUID_t *  /*pGeneration*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFObject::GetGeneration (aafUID_t *  /*pGeneration*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFObject::GetDefinition (ImplAAFClassDef ** /*ppClassDef*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
	ImplAAFObject::GetObjectClass (aafUID_t * pClass)
{
  if (NULL == pClass)
    return AAFRESULT_NULL_PARAM;

  //
  // Take advantage of the virtual method that 
  // returns the stored class id for a given class of OMStorable.
  //
#ifndef _DEBUG
  *pClass = *reinterpret_cast<const aafUID_t *>(&classId());
#else
  // In a debug build copy all of the fields separately so
  // that we can detect changes in the definitions of public aafUID_t
  // and private OMClassId types.
  const OMClassId& id = classId();
  pClass->Data1 = id.Data1;
  pClass->Data2 = id.Data2;
  pClass->Data3 = id.Data3;
  pClass->Data4[0] = id.Data4[0];
  pClass->Data4[1] = id.Data4[1];
  pClass->Data4[2] = id.Data4[2];
  pClass->Data4[3] = id.Data4[3];
  pClass->Data4[4] = id.Data4[4];
  pClass->Data4[5] = id.Data4[5];
  pClass->Data4[6] = id.Data4[6];
  pClass->Data4[7] = id.Data4[7];
#endif

  return AAFRESULT_SUCCESS;
}


AAFRESULT ImplAAFObject::InitProperties ()
{
  if (! _pProperties)
	{
	  _pProperties = new ImplPropertyCollection;
	  if (! _pProperties)
		return AAFRESULT_NOMEMORY;
	  OMPropertySet * ps = propertySet();
	  assert (ps);
	  _pProperties->Init(ps);
	}
  assert (_pProperties);
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
	ImplAAFObject::GetProperties (ImplEnumAAFProperties ** ppEnum)
{
  if (! ppEnum)
	return AAFRESULT_NULL_PARAM;

  if (! _pProperties)
	{
	  AAFRESULT hr = InitProperties();
	  if (! AAFRESULT_SUCCEEDED (hr)) return hr;
	}
  assert (_pProperties);
  
  ImplEnumAAFProperties * pEnum = NULL;
  pEnum = (ImplEnumAAFProperties*) CreateImpl (CLSID_EnumAAFProperties);
  if (! pEnum)
	return E_FAIL;
  assert (pEnum);
  
  AAFRESULT hr = pEnum->Initialize (_pProperties);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;
  
  assert (ppEnum);
  *ppEnum = pEnum;
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
	ImplAAFObject::CountProperties (aafUInt32 * pCount)
{
  if (! pCount)
	return AAFRESULT_NULL_PARAM;

  assert (pCount);
  if (! _pProperties)
	{
	  AAFRESULT hr = InitProperties();
	  if (! AAFRESULT_SUCCEEDED (hr)) return hr;
	}
  assert (_pProperties);
  AAFRESULT hr = _pProperties->GetNumElements (pCount);
  if (! AAFRESULT_SUCCEEDED (hr)) return hr;
  return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
	ImplAAFObject::GetPropertyValue (ImplAAFPropertyDef * /*pPropDef*/,
									 ImplAAFPropertyValue ** /*ppPropVal*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
	ImplAAFObject::SetPropertyValue (ImplAAFPropertyDef * /*pPropDef*/,
									 ImplAAFPropertyValue * /*pPropVal*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
	ImplAAFObject::IsPropertyPresent (ImplAAFPropertyDef * /*pPropDef*/,
									  aafBool * /*pResult*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFObject::GetStoredByteOrder (eAAFByteOrder_t *  /*pOrder*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFObject::GetNativeByteOrder (eAAFByteOrder_t *  /*pOrder*/)
{
  return AAFRESULT_NOT_IMPLEMENTED;
}

//************
// Interfaces ivisible inside the toolkit, but not exposed through the API

// Gets the head object of the file containing this object.
// This function is used to maintain MOB and Definition tables in the
// head object.
AAFRESULT ImplAAFObject::MyHeadObject
	(ImplAAFHeader **header)
{
	OMFile			*myFile;
	OMStorable		*theRoot;
	ImplAAFHeader	*theHeader;

	XPROTECT()
	{
		if(header == NULL)
			RAISE(AAFRESULT_NULL_PARAM);

		if(!attached())
			RAISE(AAFRESULT_OBJECT_NOT_ATTACHED); 

                if(!inFile())
			RAISE(AAFRESULT_OBJECT_NOT_IN_FILE); 

		myFile = file();
		if(myFile == NULL)
			RAISE(AAFRESULT_OBJECT_NOT_IN_FILE);

		theRoot = myFile->root();
		if(theRoot == NULL)
			RAISE(AAFRESULT_BADHEAD);

		theHeader = dynamic_cast<ImplAAFHeader*>(theRoot);
		if(theRoot == NULL)
			RAISE(AAFRESULT_BADHEAD);
		theHeader->AcquireReference();

		*header = theHeader;
	}
	XEXCEPT
	XEND;

	return(AAFRESULT_SUCCESS);
}

//
// Define the symbol for the stored object id
//
OMDEFINE_STORABLE(ImplAAFObject, AUID_AAFObject);

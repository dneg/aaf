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

#ifndef __ImplAAFCollection_h__
#include "ImplAAFCollection.h"
#endif

#ifndef __ImplAAFEnumerator_h__
#include "ImplAAFEnumerator.h"
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
#include "OMProperty.h"

extern "C" const aafClassID_t CLSID_AAFProperty;
extern "C" const aafClassID_t CLSID_EnumAAFProperties;


#define RELEASE_IF_SET(obj) \
    if (obj) { obj->ReleaseReference(); obj = NULL; }


// explicit template instantiation
// template class ImplAAFCollection<ImplAAFProperty *>;


//
// Private class for implementing collections of properties.
//
class ImplPropertyCollection : public ImplAAFCollection<ImplAAFProperty *>
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
    Initialize (ImplAAFObject * pObj,
				OMPropertySet * pOMPropSet);

private:
  //AAFRESULT initProperty(aafUInt32 index,
  //                       ImplAAFPropertyDef * pPropDef);

  // OMPropertySet *    _pOMPropSet;
  ImplAAFProperty ** _pProperties;
  aafUInt32          _count;
};



ImplPropertyCollection::ImplPropertyCollection ()
  : _pProperties (0),
	_count (0)
{}


AAFRESULT ImplPropertyCollection::Initialize
(
 ImplAAFObject * pObj,
 OMPropertySet * pOMPropSet
)
{
  ImplAAFPropertyDef * pPropDef = NULL;
  AAFRESULT rReturned = AAFRESULT_SUCCESS;

  // make sure we haven't called this before
  assert (! _pProperties);

  if (! pOMPropSet)
	return AAFRESULT_NULL_PARAM;

  _count = pOMPropSet->count();

  _pProperties = new ImplAAFProperty * [_count];
  if (! _pProperties)
	return AAFRESULT_NOMEMORY;

  for (aafUInt32 i = 0;
	   i < _count;
	   i++)
	{
	  _pProperties[i] = NULL;
	}

  ImplAAFHeader * pHead = NULL;
  ImplAAFDictionary * pDict = NULL;
  try
	{
	  AAFRESULT hr;
	  assert (pObj);
	  hr = pObj->MyHeadObject(&pHead);
	  if (AAFRESULT_OBJECT_NOT_ATTACHED == hr)
		{
		  // this must be the head object
		  pHead = dynamic_cast<ImplAAFHeader*>(pObj);
		  if (!pHead)
			throw AAFRESULT_OBJECT_NOT_ATTACHED;
		}
	  else
		{
		  if (AAFRESULT_FAILED(hr)) throw hr;
		}
	  assert (pHead);
	  hr = (pHead->GetDictionary(&pDict));
	  if (AAFRESULT_FAILED(hr)) throw hr;
	  assert (pDict);

	  size_t omContext = 0;
	  OMProperty * pOmProp = NULL;
	  for (aafUInt32 i = 0;
		   i < _count;
		   i++)
		{
		  _pProperties[i] = (ImplAAFProperty*) CreateImpl (CLSID_AAFProperty);
		  if (!_pProperties[i])
			throw AAFRESULT_NOMEMORY;

		  pOMPropSet->iterate (omContext, pOmProp);
		  assert (pOmProp);
		  OMPropertyId opid = pOmProp->propertyId ();
		  assert (pDict);
		  AAFRESULT hr = pDict->LookupPropDef (opid, &pPropDef);
		  if (AAFRESULT_FAILED (hr)) throw hr;
		  assert (pPropDef);
		  hr = _pProperties[i]->Initialize (pPropDef, pOmProp);
		  if (AAFRESULT_FAILED (hr)) throw hr;
		  pPropDef->ReleaseReference ();
		  pPropDef = 0;
		}
	}
  catch (AAFRESULT &rCaught)
	{
	  rReturned = rCaught;
	  if (pPropDef)
		{
		  pPropDef->ReleaseReference ();
		  pPropDef = NULL;
		}
	  if (_pProperties)
		{
		  assert (_count);
		  for (aafUInt32 i = 0;
			   i < _count;
			   i++)
			{
			  if (_pProperties[i])
				{
				  _pProperties[i]->ReleaseReference ();;
				  _pProperties[i] = NULL;
				}
			}
		  delete[] _pProperties;
		  _pProperties = NULL;
		}
	}
  RELEASE_IF_SET (pHead)
  RELEASE_IF_SET (pDict);

  return rReturned;
}


ImplPropertyCollection::~ImplPropertyCollection ()
{
  if (_pProperties)
	{
	  for (aafUInt32 numProps = 0;
		   numProps < _count;
		   numProps++)
		{
		  if (_pProperties[numProps])
			{
			  _pProperties[numProps]->ReleaseReference();
			  _pProperties[numProps] = NULL;
			}
		}
	  delete[] _pProperties;
	  _pProperties = NULL;
	}
}


AAFRESULT ImplPropertyCollection::GetNumElements
(aafUInt32 * pCount)
{
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

  assert (_pProperties);
  assert (_pProperties[index]);
  assert (pElem);
  *pElem = _pProperties[index];
  (*pElem)->AcquireReference();
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
	  AAFRESULT hr = _pProperties->Initialize (this, ps);
	  if (AAFRESULT_FAILED (hr)) return hr;
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
OMDEFINE_STORABLE(ImplAAFObject, AUID_AAFObject)

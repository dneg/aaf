// @doc INTERNAL
// @com This file handles bdirectional translation of effect metadata between OMF and AAF

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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <iostream.h>

namespace OMF2
{
#include "omPublic.h"
#include "omMedia.h"
}

#include "AafOmf.h"
#include "AAFDomainUtils.h"
#include "AAFClassDefUIDs.h"
#include "AAFException.h"
#include "AutoRelease.h"

#include "CAAFBuiltinDefs.h"

#define CHECKAAF


// ============================================================================
// Constructor
// ============================================================================
AAFDomainUtils::AAFDomainUtils()
{
}
// ============================================================================
// Destructor
// ============================================================================
AAFDomainUtils::~AAFDomainUtils()
{
}

void AAFDomainUtils::SetDictionary(IAAFDictionary *dict)
{
	_dict = dict;
}

IAAFInterpolationDef *AAFDomainUtils::CreateInterpolationDefinition(IAAFDictionary *dict, aafUID_t interpolationDefID)
{
	IAAFInterpolationDef	*interpDef;
	AAFRESULT				rc;

	CAAFBuiltinDefs defs (dict);

	rc = dict->LookupInterpolationDef(interpolationDefID,&interpDef);
	if(rc == AAFRESULT_SUCCESS && interpDef != NULL)
		return interpDef;

//	dprintf("AEffect::CreateInterpolationDefinition()\n");	//JeffB:

	(void)(defs.cdInterpolationDefinition()->
		   CreateInstance(IID_IAAFInterpolationDef,
						  (IUnknown **)&interpDef));
	if(memcmp(&interpolationDefID, &LinearInterpolator, sizeof(aafUID_t)) == 0)
	{
 		(void)(interpDef->Initialize(interpolationDefID, L"LinearInterp", L"Linear keyframe interpolation"));
		dict->RegisterInterpolationDef(interpDef);
	}
	else
	{
		interpDef->Release();
		interpDef = NULL;
	}

//cleanup:
	return(interpDef);
}

IAAFTypeDef *AAFDomainUtils::CreateTypeDefinition(IAAFDictionary *pDict, aafUID_t typeDefID)
{
	IAAFTypeDef		*typeDef;

	CAAFBuiltinDefs defs (pDict);

//	dprintf("AEffect::CreateTypeDefinition()\n");	//JeffB:
	CHECKAAF(pDict->LookupTypeDef(typeDefID,&typeDef));
	return typeDef;
}

void AAFDomainUtils::AAFAddOnePoint(IAAFDictionary *dict, aafRational_t percentTime, long buflen, void *buf, IAAFTypeDef *typeDef, IAAFVaryingValue *pVVal)
{
	IAAFControlPoint	*pPoint = NULL;
//	AAFRESULT			rc;

	CAAFBuiltinDefs defs (dict);

	CHECKAAF(defs.cdControlPoint()->
			 CreateInstance(IID_IAAFControlPoint,
							(IUnknown **)&pPoint));
	CHECKAAF(pPoint->Initialize (pVVal, percentTime, buflen, (aafDataBuffer_t)buf));
	CHECKAAF(pVVal->AddControlPoint(pPoint));
//cleanup:
	if(pPoint != NULL)
		pPoint->Release();
}

IAAFParameter *AAFDomainUtils::AAFAddConstantVal(IAAFDictionary *dict, IAAFParameterDef *pParameterDef, long buflen, void *buf, IAAFOperationGroup *pGroup)
{
	IAAFConstantValue	*pCVal = NULL;
	IAAFParameter		*pParm = NULL;
//	AAFRESULT			rc;

	CAAFBuiltinDefs defs (dict);

	CHECKAAF(defs.cdConstantValue()->
			 CreateInstance(IID_IAAFConstantValue,
							(IUnknown **)&pCVal));
	CHECKAAF(pCVal->Initialize (pParameterDef, buflen, (unsigned char *)buf));
	CHECKAAF(pCVal->QueryInterface(IID_IAAFParameter, (void **) &pParm));
	CHECKAAF(pGroup->AddParameter(pParm));
//cleanup:
	if(pParm)
		pParm->Release();
	if(pCVal)
		pCVal->Release();

	return(pParm);
}

IAAFVaryingValue *AAFDomainUtils::AAFAddEmptyVaryingVal(IAAFDictionary *dict, IAAFParameterDef *pParameterDef, IAAFOperationGroup *pOutputEffect)
{
	IAAFVaryingValue	*pVVal = NULL;
	IAAFParameter		*pParm = NULL;
//	AAFRESULT			rc;

	CAAFBuiltinDefs defs (dict);

	CHECKAAF(defs.cdVaryingValue()->
			 CreateInstance(IID_IAAFVaryingValue,
							(IUnknown **)&pVVal));
  AutoRelease<IAAFVaryingValue> arVaryingValue(pVVal);
  CHECKAAF(pVVal->Initialize (pParameterDef, CreateInterpolationDefinition(
												dict, LinearInterpolator)));
	CHECKAAF(pVVal->QueryInterface(IID_IAAFParameter, (void **) &pParm));
	CHECKAAF(pOutputEffect->AddParameter(pParm));
//cleanup:
	if(pParm != NULL)
		pParm->Release();

  // Since we are using the "auto release" for the return value that
  // we just created we need to bump the reference count.
  pVVal->AddRef();

	return(pVVal);
}

IAAFParameterDef *AAFDomainUtils::CreateParameterDefinition(IAAFDictionary *pDict, aafUID_t parmDefID)
{
	IAAFParameterDef	*parmDef;
	IAAFTypeDef			*typeDef;
	AAFRESULT			rc;

//	dprintf("AEffect::CreateParameterDefinition()\n");
	rc = pDict->LookupParameterDef(parmDefID,&parmDef);
	if(rc == AAFRESULT_SUCCESS && parmDef != NULL)
		return parmDef;

	CAAFBuiltinDefs defs (pDict);

	CHECKAAF(defs.cdParameterDef()->
			 CreateInstance(IID_IAAFParameterDef,
							(IUnknown **)&parmDef));
  AutoRelease<IAAFParameterDef> arParmDef(parmDef);
	if(memcmp(&parmDefID, &kAAFParameterDefLevel, sizeof(aafUID_t)) == 0)
	{
    	CHECKAAF(pDict->LookupTypeDef(kAAFTypeID_Rational, &typeDef));
    	AutoRelease<IAAFTypeDef> r1( typeDef );
    	CHECKAAF(parmDef->Initialize(parmDefID, L"Level", L"fractional 0-1 inclusive", typeDef));
	}
	else if(memcmp(&parmDefID, &kAAFParameterDefSMPTEWipeNumber, sizeof(aafUID_t)) == 0)
	{
    	CHECKAAF(pDict->LookupTypeDef(kAAFTypeID_Int32, &typeDef));
    	AutoRelease<IAAFTypeDef> r2( typeDef );
    	CHECKAAF(parmDef->Initialize(parmDefID, L"WipeCode", L"SMPTE Wipe Code", typeDef));
	}
  else
  {
    // Unrecognized parameter definition. Should we emit a log entry?
    return NULL;
  }

	if(parmDef != NULL)
	{
		CHECKAAF(pDict->RegisterParameterDef(parmDef));
    // Since we are using the "auto release" for the return value that
    // we just created we need to bump the reference count.
    parmDef->AddRef();
	}
//cleanup:

	return(parmDef);
}

//***********************************************************
//
//	SetIntegerPropOnObject()
//
//	Set an integer property to the AAF object specified by pObj.
//	The value of the property is specified in value.
//
//	Returns:
//
//		On Success: AAFRESULT_SUCCESS
//		On Failure: An exception.
//
HRESULT AAFDomainUtils::SetIntegerPropOnObject(IAAFObject* pObj, aafUID_t* pClassID, aafUID_t* pPropID, const aafUID_t* pIntTypeID,
							   aafMemPtr_t pValue, aafUInt32 ValueSize, IAAFDictionary *dict)
{
	AAFCheck			hr;

	// Create a property value from the supplied value (pValue)
	IAAFTypeDef*		pTD;
	hr = dict->LookupTypeDef(*pIntTypeID, &pTD);
	AutoRelease<IAAFTypeDef> r1( pTD );

	IAAFTypeDefInt*	pTDInt;
	hr = pTD->QueryInterface(IID_IAAFTypeDefInt, (void**)&pTDInt);
	AutoRelease<IAAFTypeDefInt> r2( pTDInt );

	// Now create a property value object with that value.
	IAAFPropertyValue*	pPV;
	hr = pTDInt->CreateValue(pValue, ValueSize, &pPV);
	AutoRelease<IAAFPropertyValue> r3( pPV );

	// Add the property to the target object.
	// Get the class def for the object
	IAAFClassDef*	pCD;
	hr = dict->LookupClassDef(*pClassID, &pCD);
	AutoRelease<IAAFClassDef> r4( pCD );

	IAAFPropertyDef*	pPD;
	hr = pCD->LookupPropertyDef(*pPropID, &pPD);
	AutoRelease<IAAFPropertyDef> r5( pPD );

	// Set the propeter value on the target object
	hr = pObj->SetPropertyValue(pPD, pPV);

	return AAFRESULT_SUCCESS;
}

//***********************************************************
//
//	GetIntegerPropFromObject()
//
//	Get an integer property from the AAF object specified by pObj.
//	The value of the property is returned in pValue.
//
//	Returns:
//
//		On Success: AAFRESULT_SUCCESS
//		On Failure: An exception.
//
HRESULT AAFDomainUtils::GetIntegerPropFromObject(IAAFObject* pObj, const aafUID_t* pClassID, aafUID_t* pPropID, const aafUID_t* pIntTypeID, aafMemPtr_t pValue, aafUInt32 ValueSize, IAAFDictionary *dict)
{
	AAFCheck				hr;

	// Get the property value for the target property
	IAAFClassDef*		pCD;
	hr = dict->LookupClassDef(*pClassID, &pCD);
	AutoRelease<IAAFClassDef> r1(pCD);

	IAAFPropertyDef*	pPD;
	hr = pCD->LookupPropertyDef(*pPropID, &pPD);
	AutoRelease<IAAFPropertyDef> r2( pPD );

	aafBool	present = kAAFFalse;
	pObj->IsPropertyPresent(pPD, &present);
	IAAFPropertyValue*	pPV = NULL;
	AutoRelease<IAAFPropertyValue> r3;

	if (present == kAAFTrue)
	{
		hr = pObj->GetPropertyValue(pPD, &pPV);
		r3 = pPV;
	}
	else
	{
		hr = AAFRESULT_PROP_NOT_PRESENT;
	}

	// Get the type def from the dict with which to interpret this
	// property value.
	IAAFTypeDef* pTD;
	hr = dict->LookupTypeDef(*pIntTypeID, &pTD);
	AutoRelease< IAAFTypeDef > r4 (pTD);

	IAAFTypeDefInt* pTDInt;
	hr = pTD->QueryInterface(IID_IAAFTypeDefInt, (void**)&pTDInt);
	AutoRelease< IAAFTypeDefInt > r5( pTDInt );

	pTDInt->GetInteger(pPV, pValue, ValueSize);

	return AAFRESULT_SUCCESS;
}

//***********************************************************
//
//	AddPropertyToClass()
//
//	Add an integer property to the class specified by ClassID
//	in the AAF Dictionary.
//
//	Returns:
//
//		On Success: AAFRESULT_SUCCESS
//		On Failure: An exception.
//
HRESULT AAFDomainUtils::AddPropertyToClass(IAAFDictionary *dict, const aafUID_t* pClassID, const aafUID_t* pPropTypeID, const aafUID_t* pPropID, aafCharacter*  pName)
{
	AAFCheck			hr;

	// Get the class definition.
	IAAFClassDef*	pCD;
	hr = dict->LookupClassDef(*pClassID, &pCD);
	AutoRelease<IAAFClassDef> r1(pCD);
	IAAFTypeDef*	pTypeDef;
	hr = dict->LookupTypeDef(*pPropTypeID, &pTypeDef);
	AutoRelease< IAAFTypeDef > r2( pTypeDef );
	IAAFPropertyDef*	pPropDef;
	hr = pCD->RegisterOptionalPropertyDef(*pPropID, pName, pTypeDef, &pPropDef);
	AutoRelease< IAAFPropertyDef > r3( pPropDef );	
	return AAFRESULT_SUCCESS;
}

//***********************************************************
//
//	SetObjRefArrayPropOnObject()
//
//	Set an object reference array property on the AAF object
//	specified by pObj.  The value of the property is specified
//	in pArray.
//
//	Returns:
//
//		On Success: S_OK
//		On Failure: A failed HRESULT
//
HRESULT AAFDomainUtils::SetObjRefArrayPropOnObject(IAAFObject* pObj, aafUID_t* pClassID, const aafUID_t* pPropTypeID, const aafUID_t* pElementTypeID, aafUID_t* pPropID, IAAFObject** pArray, aafUInt32 numObjects)
{
	IAAFPropertyValue*	pPVArray = NULL;
	IAAFTypeDef*		pTD;
	HRESULT				hr;

	// For each object in the source array, create a property value
	// and append it to the property value for the variable array.
	hr = _dict->LookupTypeDef(*pPropTypeID, &pTD);
	if (SUCCEEDED(hr))
	{
		IAAFTypeDefVariableArray*	pTDVarArray;

		hr = pTD->QueryInterface(IID_IAAFTypeDefVariableArray, (void**)&pTDVarArray);
		if (SUCCEEDED(hr))
		{
			IAAFTypeDef*	pTDElement;

			hr = _dict->LookupTypeDef(*pElementTypeID, &pTDElement);
			if (SUCCEEDED(hr))
			{
				IAAFTypeDefObjectRef*	pTDObjRef;

				hr = pTDElement->QueryInterface (IID_IAAFTypeDefObjectRef, (void **)&pTDObjRef);
				if (SUCCEEDED(hr))
				{
					for (aafUInt32 i = 0; i < numObjects; i++)
					{
						IAAFPropertyValue* pPVObject;

						hr = pTDObjRef->CreateValue(pArray[i], &pPVObject);
						if (SUCCEEDED(hr))
						{
							if (pPVArray == NULL)
								pTDVarArray->CreateEmptyValue(&pPVArray);

							hr = pTDVarArray->AppendElement(pPVArray, pPVObject);
							pPVObject->Release();
						}
					}
					pTDObjRef->Release();
				}
				pTDElement->Release();
			}
			pTDVarArray->Release();
		}
		pTD->Release();
	}

	// If the object reference array was successfully created,
	// set the property value on the target object.
	if (pPVArray)
	{
		IAAFClassDef*		pCD;

		// Get the class def for the object
		hr = _dict->LookupClassDef(*pClassID, &pCD);
		if (SUCCEEDED(hr))
		{
			IAAFPropertyDef*	pPD;

			hr = pCD->LookupPropertyDef(*pPropID, &pPD);
			if (SUCCEEDED(hr))
			{
				hr = pObj->SetPropertyValue(pPD, pPVArray);
				pPD->Release();
			}
			pCD->Release();
		}
		pPVArray->Release();
	}

	return hr;
}

//***********************************************************
//
//	GetObjRefArrayPropFromObject()
//
//	Get an object reference array property on the AAF object
//	specified by pObj.  The value of the property is returned
//	in pArray.
//
//	Returns:
//
//		On Success: S_OK
//		On Failure: A failed HRESULT
//
HRESULT AAFDomainUtils::GetObjRefArrayPropFromObject(IAAFObject* pObj, aafUID_t* pClassID, const aafUID_t* pPropTypeID, aafUID_t* pPropID, IAAFObject*** pArray, aafUInt32* pNumObjects)
{
	IAAFTypeDefVariableArray*	pTDVarArray = NULL;
	IAAFPropertyValue*			pPVVarArray = NULL;
	IAAFTypeDefObjectRef*		pTDArrayElement = NULL;
	IAAFClassDef*				pCD;
	HRESULT						hr;

	// Get the property value for the target property
	hr = _dict->LookupClassDef(*pClassID, &pCD);
	if (SUCCEEDED(hr))
	{
		IAAFPropertyDef*	pPD;

		hr = pCD->LookupPropertyDef(*pPropID, &pPD);
		if (SUCCEEDED(hr))
		{
			aafBool	present = kAAFFalse;

			pObj->IsPropertyPresent(pPD, &present);
			if (present == kAAFTrue)
				hr = pObj->GetPropertyValue(pPD, &pPVVarArray);
			else
				hr = AAFRESULT_PROP_NOT_PRESENT;

			pPD->Release();
		}
		pCD->Release();
	}

	// Get the property type def from the dictionary to interpret this property value.
	if (SUCCEEDED(hr))
	{
		IAAFTypeDef* pTD;

		hr = _dict->LookupTypeDef(*pPropTypeID, &pTD);
		if (SUCCEEDED(hr))
		{
			hr = pTD->QueryInterface(IID_IAAFTypeDefVariableArray, (void**)&pTDVarArray);
			pTD->Release();
		}
	}

	// Get the array element type def to interpret the element property value.
	if (SUCCEEDED(hr))
	{
		IAAFTypeDef*	pTDElement;

		hr = pTDVarArray->GetType(&pTDElement);
		if (SUCCEEDED(hr))
		{
			pTDElement->QueryInterface(IID_IAAFTypeDefObjectRef, (void **)&pTDArrayElement);
			pTDElement->Release();
		}
	}

	// Get each element out of the property, convert them to an IAAFObject pointer and
	// add them to the array of object which is returned to the user.
	if (SUCCEEDED(hr))
	{
		IAAFObject**	pTempArray;
		aafUInt32		count = 0, numElements = 0;

		pTDVarArray->GetCount(pPVVarArray, &count);
		pTempArray = new IAAFObject* [count];
		if (pTempArray)
		{
			for (aafUInt32 i = 0; i < count; i++)
			{
				IAAFPropertyValue*	pPVElement;

				hr = pTDVarArray->GetElementValue(pPVVarArray, i, &pPVElement);
				if (SUCCEEDED(hr))
				{
					IAAFObject*	pTempObj;

					hr = pTDArrayElement->GetObject(pPVElement, IID_IAAFObject, (IUnknown **)&pTempObj);
					if (SUCCEEDED(hr))
					{
						pTempArray[numElements] = pTempObj;
						numElements++;
					}
					pPVElement->Release();
				}
			}
			if (numElements == 0)
			{
				delete [] pTempArray;
				pTempArray = NULL;
			}

			*pArray = pTempArray;
			*pNumObjects = numElements;
		}
	}

	if (pTDArrayElement) pTDArrayElement->Release();
	if (pTDVarArray) pTDVarArray->Release();
	if (pPVVarArray) pPVVarArray->Release();

	return hr;
}

//***********************************************************
//
//	SetObjRefPropOnObject()
//
//	Set an object reference property on the AAF object specified
//	by pObj.  The value of the property is specified in pObject.
//
//	Returns:
//
//		On Success: S_OK
//		On Failure: A failed HRESULT
//
HRESULT AAFDomainUtils::SetObjRefPropOnObject(IAAFObject* pObj, aafUID_t* pClassID, const aafUID_t* pPropTypeID, aafUID_t* pPropID, IAAFObject* pValue)
{
	IAAFPropertyValue*	pPV = NULL;
	IAAFTypeDef*		pTD;
	HRESULT				hr;

	// Create a property value from the supplied value (pValue)
	hr = _dict->LookupTypeDef(*pPropTypeID, &pTD);
	if (SUCCEEDED(hr))
	{
		IAAFTypeDefObjectRef*	pTDObjRef;

		hr = pTD->QueryInterface(IID_IAAFTypeDefObjectRef, (void**)&pTDObjRef);
		if (SUCCEEDED(hr))
		{
			hr = pTDObjRef->CreateValue(pValue, &pPV);
			pTDObjRef->Release();
		}
		pTD->Release();
	}

	// Add the property to the target object.
	if (SUCCEEDED(hr))
	{
		if (SUCCEEDED(hr))
		{
			IAAFClassDef*		pCD;

			// Get the class def for the object
			hr = _dict->LookupClassDef(*pClassID, &pCD);
			if (SUCCEEDED(hr))
			{
				IAAFPropertyDef*	pPD;

				hr = pCD->LookupPropertyDef(*pPropID, &pPD);
				if (SUCCEEDED(hr))
				{
					// Set the propeter value on the target object
					hr = pObj->SetPropertyValue(pPD, pPV);
					pPD->Release();
				}
				pCD->Release();
			}
		}
	}

	if (pPV) pPV->Release();

	return hr;
}

//***********************************************************
//
//	GetObjRefPropFromObject()
//
//	Get a object reference property on the AAF object specified
//	by pObj.  The value of the property is returned in ppObject.
//
//	Returns:
//
//		On Success: S_OK
//		On Failure: A failed HRESULT
//
HRESULT AAFDomainUtils::GetObjRefPropFromObject(IAAFObject* pObj, aafUID_t* pClassID, const aafUID_t* pPropTypeID, aafUID_t* pPropID, IAAFObject** ppObject)
{
	IAAFPropertyValue*		pPV = NULL;
	IAAFClassDef*			pCD;
	HRESULT					hr;

	// Get the property value for the target property
	hr = _dict->LookupClassDef(*pClassID, &pCD);
	if (SUCCEEDED(hr))
	{
		IAAFPropertyDef*	pPD;

		hr = pCD->LookupPropertyDef(*pPropID, &pPD);
		if (SUCCEEDED(hr))
		{
			aafBool	present = kAAFFalse;

			pObj->IsPropertyPresent(pPD, &present);
			if (present == kAAFTrue)
				hr = pObj->GetPropertyValue(pPD, &pPV);
			else
				hr = AAFRESULT_PROP_NOT_PRESENT;

			pPD->Release();
		}
		pCD->Release();
	}

	// Get the property type def from the dictionary to interpret this property value
	// and return the resulting object.
	if (SUCCEEDED(hr))
	{
		IAAFTypeDef* pTD;

		hr = _dict->LookupTypeDef(*pPropTypeID, &pTD);
		if (SUCCEEDED(hr))
		{
			IAAFTypeDefObjectRef*	pTDObjectRef;

			hr = pTD->QueryInterface(IID_IAAFTypeDefObjectRef, (void**)&pTDObjectRef);
			if (SUCCEEDED(hr))
			{
				IAAFObject*	pTempObj;

				hr = pTDObjectRef->GetObject(pPV, IID_IAAFObject, (IUnknown **)&pTempObj);
				if (SUCCEEDED(hr))
				{
					*ppObject = pTempObj;
				}
				pTDObjectRef->Release();
			}
			pTD->Release();
		}
	}

	if (pPV) pPV->Release();

	return hr;
}


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

#include "CAAFBasicInterp.h"

#include <assert.h>
#include <string.h>
#include "AAFResult.h"

#include "AAF.h"

#include "aafErr.h"
#include "AAFDefUIDs.h"
#include "AAFStoredObjectIDs.h"
#include "AAFRational.h"
#include "AAFInterpolatorDefs.h"
#include "AAFTypeDefUIDs.h"

#include "CAAFBuiltinDefs.h"

const aafProductVersion_t AAFPluginImplementationVersion = {1, 0, 0, 1, kAAFVersionBeta};

const CLSID CLSID_AAFBasicInterp = { 0x5B6C85A1, 0x0EDE, 0x11d3, { 0x80, 0xA9, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };

const aafUID_t BASIC_INTERP_PLUGIN = { 0x5B6C85A2, 0x0EDE, 0x11d3, { 0x80, 0xA9, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };


static bool EqualAUID(const aafUID_t *id1, const aafUID_t * id2)
{
  assert (id1 && id2);
  return (0 == memcmp(id1, id2, sizeof(aafUID_t)));
}

HRESULT STDMETHODCALLTYPE
    CAAFBasicInterp::CountDefinitions (aafUInt32 *pDefCount)
{
	if(pDefCount == NULL)
		return AAFRESULT_NULL_PARAM;
	*pDefCount = 1;
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFBasicInterp::GetIndexedDefinitionID (aafUInt32 index, aafUID_t *pUid)
{
	if(pUid == NULL)
		return AAFRESULT_NULL_PARAM;
	if(index < 0 || index >= 1)
		return AAFRESULT_BADINDEX;
	*pUid = LinearInterpolator;
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFBasicInterp::GetPluginDescriptorID (aafUID_t *uid)
{
	*uid = BASIC_INTERP_PLUGIN;		// UID of the PluginDescriptor
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFBasicInterp::GetIndexedDefinitionObject (aafUInt32 /* index */, IAAFDictionary *dict, IAAFDefObject **def)
{
	IAAFInterpolationDef	*interpDef = NULL;
	IAAFDefObject	*obj = NULL;
	aafUID_t		uid;
	
	if((dict == NULL) || (def == NULL))
		return AAFRESULT_NULL_PARAM;
	
	XPROTECT()
	{
	    CAAFBuiltinDefs defs (dict);
		CHECK(defs.cdInterpolationDefinition()->
			  CreateInstance(IID_IAAFInterpolationDef, 
							 (IUnknown **)&interpDef));
		uid = LinearInterpolator;
		CHECK(interpDef->QueryInterface(IID_IAAFDefObject, (void **)&obj));
		CHECK(interpDef->Initialize(uid, L"Basic Plugins", L"Handles step and linear interpolation."));
		*def = obj;
		interpDef->Release();
		interpDef = NULL;
	}
	XEXCEPT
	{
		if(interpDef != NULL)
			interpDef->Release();
	}
	XEND

	return AAFRESULT_SUCCESS;
}

static wchar_t *manufURL = L"http://www.avid.com";
static wchar_t *downloadURL = L"ftp://ftp.avid.com/pub/";
const aafUID_t MANUF_AVID_TECH = { 0xA6487F21, 0xE78F, 0x11d2, { 0x80, 0x9E, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };
static aafVersionType_t samplePluginVersion = { 0, 1 };

static wchar_t *manufName = L"Avid Technology, Inc.";
static wchar_t *manufRev = L"Rev 0.1";

HRESULT STDMETHODCALLTYPE
    CAAFBasicInterp::CreateDescriptor (IAAFDictionary *dict, IAAFPluginDef **descPtr)
{
	IAAFPluginDef			*desc = NULL;
	IAAFLocator				*pLoc = NULL;
 	IAAFNetworkLocator		*pNetLoc = NULL;
	
	XPROTECT()
	{
	    CAAFBuiltinDefs defs (dict);
		CHECK(defs.cdPluginDef()->
			  CreateInstance(IID_IAAFPluginDef, 
							 (IUnknown **)&desc));
		*descPtr = desc;
		desc->AddRef();
		CHECK(desc->Initialize(BASIC_INTERP_PLUGIN, L"Example interpolators", L"Handles step and linear interpolation."));
		CHECK(desc->SetCategoryClass(AUID_AAFInterpolationDefinition));
		CHECK(desc->SetPluginVersionString(manufRev));
		CHECK(defs.cdNetworkLocator()->
			  CreateInstance(IID_IAAFLocator, 
							 (IUnknown **)&pLoc));
		CHECK(pLoc->SetPath (manufURL));
		CHECK(pLoc->QueryInterface(IID_IAAFNetworkLocator, (void **)&pNetLoc));
		CHECK(desc->SetManufacturerInfo(pNetLoc));
		pNetLoc->Release();
		pNetLoc = NULL;
		pLoc->Release();
		pLoc = NULL;

		CHECK(desc->SetManufacturerID(MANUF_AVID_TECH));
		CHECK(desc->SetPluginManufacturerName(manufName));
		CHECK(desc->SetIsSoftwareOnly(kAAFTrue));
		CHECK(desc->SetIsAccelerated(kAAFFalse));
		CHECK(desc->SetSupportsAuthentication(kAAFFalse));
		
		/**/
		CHECK(defs.cdNetworkLocator()->
			  CreateInstance(IID_IAAFLocator, 
							 (IUnknown **)&pLoc));
		CHECK(pLoc->SetPath (downloadURL));
		CHECK(desc->AppendLocator(pLoc));
		desc->Release();	// We have addRefed for the return value
		desc = NULL;
		pLoc->Release();
		pLoc = NULL;
	}
	XEXCEPT
	{
		if(desc != NULL)
			desc->Release();
		if(pLoc != NULL)
			pLoc->Release();
		if(pNetLoc != NULL)
			pNetLoc->Release();
	}
	XEND

	return AAFRESULT_SUCCESS;
}


CAAFBasicInterp::CAAFBasicInterp (IUnknown * pControllingUnknown)
  : CAAFUnknown (pControllingUnknown)
{
	_typeDef = NULL;
	_parameter = NULL;
}


CAAFBasicInterp::~CAAFBasicInterp ()
{
	if(_typeDef != NULL)
		_typeDef->Release();
	if(_parameter != NULL)
		_parameter->Release();
}

HRESULT STDMETHODCALLTYPE
    CAAFBasicInterp::GetNumTypesSupported(
		/* [out] */aafUInt32*  pCount)
{
	if(pCount == NULL)
		return AAFRESULT_NULL_PARAM;
	*pCount = 1;
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFBasicInterp::GetIndexedSupportedType(
		/* [in] */ aafUInt32  index,
		/* [out] */IAAFTypeDef ** ppType)
{
	if(ppType == NULL)
		return AAFRESULT_NULL_PARAM;
	if(index < 0 || index >= 1)
		return AAFRESULT_BADINDEX;
//!!!	*ppType = kAAFTypeID_Int32;					// temp!!! Use type definition mechanism
	return AAFRESULT_NOT_IMPLEMENTED;
}

HRESULT STDMETHODCALLTYPE
    CAAFBasicInterp::GetTypeDefinition(
		/* [out] */IAAFTypeDef ** ppTypeDef)
{
	if(ppTypeDef == NULL)
		return AAFRESULT_NULL_PARAM;
	*ppTypeDef = _typeDef;
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFBasicInterp::SetTypeDefinition(
		/* [in] */ IAAFTypeDef * pTypeDef)
{
	if(pTypeDef == NULL)
		return AAFRESULT_NULL_PARAM;
	_typeDef = pTypeDef;
	pTypeDef->AddRef();
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFBasicInterp::GetParameter(
    /* [out] */IAAFParameter ** ppParameter)
{
	if(ppParameter == NULL)
		return AAFRESULT_NULL_PARAM;
	*ppParameter = _parameter;
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFBasicInterp::SetParameter(
    /* [in] */ IAAFParameter * pParameter)
{
	if(pParameter == NULL)
		return AAFRESULT_NULL_PARAM;
	if(_parameter != NULL)
		_parameter->Release();
	_parameter = pParameter;
	pParameter->AddRef();
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFBasicInterp::InterpolateOne(
    /* [in] */ aafRational_t *  pInputValue,
	/* [in] */aafUInt32	bufSize,
    /* [out] */aafMemPtr_t pOutputValue,
	/* [in] */aafUInt32	*bytesRead)
{
	AAFRational		timeA, timeB;
	AAFRational		inputTime;
	IAAFDefObject	*pDef = NULL;
  IAAFMetaDefinition * pMetaDefinition = NULL;
	IAAFVaryingValue *pVaryVal = NULL;
	IAAFInterpolationDef *pInterpDef = NULL;
	aafUID_t		defID, interpID;
	
	if(pInputValue == NULL || pOutputValue == NULL)
		return AAFRESULT_NULL_PARAM;
	XPROTECT()
	{
		if(_parameter->QueryInterface(IID_IAAFVaryingValue, (void **)&pVaryVal) == AAFRESULT_SUCCESS)
		{
			CHECK(pVaryVal->GetInterpolationDefinition (&pInterpDef));
 			CHECK(pInterpDef->QueryInterface(IID_IAAFDefObject, (void **)&pDef));
			pDef->GetAUID(&interpID);
			pDef->Release();
			pDef = NULL;
			pInterpDef->Release();
			pInterpDef = NULL;
 			pVaryVal->Release();
			pVaryVal = NULL;
		}
		if(pInputValue->denominator == 0)
			RAISE(AAFRESULT_ZERO_DIVIDE);
		inputTime = (AAFRational)*pInputValue;
		CHECK(_typeDef->QueryInterface(IID_IAAFMetaDefinition, (void **)&pMetaDefinition));
		CHECK(pMetaDefinition->GetAUID (&defID));
		pMetaDefinition->Release();
		pMetaDefinition = NULL;
		if(EqualAUID(&defID, &kAAFTypeID_Int32))
		{
			if(bufSize < sizeof(aafUInt32))
				RAISE(AAFRESULT_SMALLBUF);
			aafInt32	*result = (aafInt32 *)pOutputValue;
			aafInt32	lowerBound, upperBound;
			CHECK(FindBoundValues(*pInputValue, sizeof(aafInt32), &timeA, (aafMemPtr_t)&lowerBound,
				&timeB, (aafMemPtr_t)&upperBound));
			if(EqualAUID(&interpID, &LinearInterpolator))
				*result = (aafInt32)(((inputTime - timeA) / (timeB - timeA)) * (upperBound - lowerBound)) + lowerBound;
			else if(EqualAUID(&interpID, &ConstantInterpolator))
				*result = lowerBound;
			else
				RAISE(AAFRESULT_INVALID_INTERPKIND);
				
			*bytesRead = sizeof(aafUInt32);
		}
		else if(EqualAUID(&defID, &kAAFTypeID_Rational))
		{
			aafRational_t	*result = (aafRational_t *)pOutputValue;
			AAFRational		lowerBound, upperBound, subResult, timeDelta, num, denom;

			if(bufSize < sizeof(aafUInt32))
				RAISE(AAFRESULT_SMALLBUF);
			CHECK(FindBoundValues(*pInputValue, sizeof(aafRational_t), &timeA, (aafMemPtr_t)&lowerBound,
				&timeB, (aafMemPtr_t)&upperBound));
			num = inputTime - timeA;
			denom = timeB - timeA;
			if(denom == 0)
				RAISE(AAFRESULT_ZERO_DIVIDE);
			timeDelta = num / denom;
			subResult = upperBound - lowerBound;

			if(EqualAUID(&interpID, &LinearInterpolator))
				*result = (aafRational_t)((timeDelta * subResult) + lowerBound);
			else if(EqualAUID(&interpID, &ConstantInterpolator))
				*result = lowerBound;
			else
				RAISE(AAFRESULT_INVALID_INTERPKIND);
			*bytesRead = sizeof(aafRational_t);
		}
		else
			RAISE(AAFRESULT_BAD_TYPE);
	}
	XEXCEPT
	{
		if(pDef)
			pDef->Release();
    if (pMetaDefinition)
      pMetaDefinition->Release();
		if(pInterpDef)
			pInterpDef->Release();
 		if(pVaryVal)
			pVaryVal->Release();
	}
	XEND;
	return(AAFRESULT_SUCCESS);
}

HRESULT CAAFBasicInterp::InterpolateMany(
	/* [in] */ aafRational_t *  pStartInputValue,
    /* [in] */ aafRational_t *  pInputStep,
    /* [in] */ aafUInt32  /* pGenerateCount */,
    /* [out] */aafMemPtr_t pOutputValue,
    /* [out] */aafUInt32 *  pResultCount)
{
	if(pStartInputValue == NULL || pInputStep == NULL ||
	   pOutputValue == NULL || pResultCount == NULL)
		return AAFRESULT_NULL_PARAM;
	return AAFRESULT_SUCCESS;
}

HRESULT CAAFBasicInterp::FindBoundValues(aafRational_t point,
										aafUInt32	valueSize,
						AAFRational *lowerBoundTime, aafMemPtr_t lowerBoundValue,
						AAFRational *upperBoundTime, aafMemPtr_t upperBoundValue)
{
	IAAFConstantValue			*pConstValue = NULL;
	IAAFVaryingValue		*pVaryingValue = NULL;
	IEnumAAFControlPoints	*theEnum = NULL;
	IAAFControlPoint		*testPoint = NULL, *prevPoint = NULL;
	aafBool					found;
	AAFRational				inputTime, prevTime, zero(0,1);

	if(lowerBoundTime == NULL || lowerBoundValue == NULL || upperBoundTime == NULL || upperBoundValue == NULL)
		return AAFRESULT_NULL_PARAM;
	XPROTECT()
	{
		if(point.denominator == 0)
			RAISE(AAFRESULT_ZERO_DIVIDE);
		inputTime = (AAFRational)point;
		if(_parameter->QueryInterface(IID_IAAFConstantValue, (void **)&pConstValue) == AAFRESULT_SUCCESS)
		{
			aafUInt32	count;
			CHECK(pConstValue->GetValue (valueSize, lowerBoundValue, &count));
			if(count != valueSize)
				RAISE(AAFRESULT_WRONG_SIZE);
			memcpy(upperBoundValue, lowerBoundValue, valueSize);
			*lowerBoundTime = zero;
			*upperBoundTime = zero;
			pConstValue->Release();
			pConstValue = NULL;
		}
		else if(_parameter->QueryInterface(IID_IAAFVaryingValue, (void **)&pVaryingValue) == AAFRESULT_SUCCESS)
		{
			aafUInt32			count;
			AAFRational			testTime;

			prevTime = zero;
			CHECK(pVaryingValue->GetControlPoints(&theEnum));
			found = kAAFFalse;
			prevPoint = NULL;
			while(!found && (theEnum->NextOne(&testPoint) == AAFRESULT_SUCCESS))
			{
				aafRational_t	testRat;
				CHECK(testPoint->GetTime(&testRat));
				testTime = (AAFRational)testRat;

				if(testRat.denominator == 0)
					RAISE(AAFRESULT_ZERO_DIVIDE);
				found = (testTime > inputTime);
				if(!found)
				{
					if(prevPoint != NULL)
					{
						prevPoint->Release();
					}
					prevPoint = testPoint;
					prevTime = testTime;
					testPoint = NULL;
				}
			}
			theEnum->Release();
			theEnum = NULL;
			
			if((prevPoint== NULL) && (testPoint == NULL))
				RAISE(AAFRESULT_INCONSISTANCY);
			
			if(prevPoint != NULL && testPoint != NULL)		// Real interpolation
			{
				CHECK(prevPoint->GetValue (valueSize, lowerBoundValue, &count));
				if(count != valueSize)
					RAISE(AAFRESULT_WRONG_SIZE);
				CHECK(testPoint->GetValue (valueSize, upperBoundValue, &count));
				if(count != valueSize)
					RAISE(AAFRESULT_WRONG_SIZE);

				*lowerBoundTime = prevTime;
				*upperBoundTime = testTime;
				testPoint->Release();
				testPoint = NULL;
				prevPoint->Release();
				prevPoint = NULL;
			}
			else if(prevPoint == NULL)						// Off the begining
			{
				CHECK(testPoint->GetValue (valueSize, lowerBoundValue, &count));
				if(count != valueSize)
					RAISE(AAFRESULT_WRONG_SIZE);

				memcpy(upperBoundValue, lowerBoundValue, valueSize);
				*lowerBoundTime = testTime;
				*upperBoundTime = testTime;
				testPoint->Release();
				testPoint = NULL;
			}
			else											// Off of the end
			{
				CHECK(prevPoint->GetValue (valueSize, lowerBoundValue, &count));
				if(count != valueSize)
					RAISE(AAFRESULT_WRONG_SIZE);

				memcpy(upperBoundValue, lowerBoundValue, valueSize);
				*lowerBoundTime = prevTime;
				*upperBoundTime = prevTime;
				prevPoint->Release();
				prevPoint = NULL;
			}
			pVaryingValue->Release();
			pVaryingValue = NULL;
		}
		else
			RAISE(AAFRESULT_UNKNOWN_PARAMETER_CLASS);
	}
	XEXCEPT
	{
		if(pConstValue)
			pConstValue->Release();
		if(pVaryingValue)
			pVaryingValue->Release();
		if(theEnum)
			theEnum->Release();
	}
	XEND;

	return(AAFRESULT_SUCCESS);
}

//
// 
// 
inline int EQUAL_UID(const GUID & a, const GUID & b)
{
  return (0 == memcmp((&a), (&b), sizeof (aafUID_t)));
}
HRESULT CAAFBasicInterp::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFInterpolator))
    { 
        *ppvObj = (IAAFInterpolator *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }
    else if (EQUAL_UID(riid,IID_IAAFPlugin)) 
    { 
        *ppvObj = (IAAFPlugin *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFUnknown::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFBasicInterp)







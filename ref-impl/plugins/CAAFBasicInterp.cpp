/***********************************************\
*												*
* Advanced Authoring Format						*
*												*
* Copyright (c) 1998-1999 Avid Technology, Inc. *
* Copyright (c) 1998-1999 Microsoft Corporation *
*												*
\***********************************************/

#include "CAAFBasicInterp.h"

#include <assert.h>
#include "AAFResult.h"

#include "AAF.h"

#include "aafErr.h"
#include "aafUtils.h"
#include "aafCvt.h"
#include "aafDefUIDs.h"
#include "AAFStoredObjectIDs.h"
#include "AAFRational.h"

const aafProductVersion_t AAFPluginImplementationVersion = {1, 0, 0, 1, kVersionBeta};

const CLSID CLSID_AAFBasicInterp = { 0x5B6C85A1, 0x0EDE, 0x11d3, { 0x80, 0xA9, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };

const aafUID_t BASIC_INTERP_PLUGIN = { 0x5B6C85A2, 0x0EDE, 0x11d3, { 0x80, 0xA9, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };


HRESULT STDMETHODCALLTYPE
    CAAFBasicInterp::Start (void)
{
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFBasicInterp::Finish (void)
{
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFBasicInterp::GetNumDefinitions (aafInt32 *pDefCount)
{
	if(pDefCount == NULL)
		return AAFRESULT_NULL_PARAM;
	*pDefCount = 1;
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFBasicInterp::GetIndexedDefinitionID (aafInt32 index, aafUID_t *pUid)
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
    CAAFBasicInterp::GetIndexedDefinitionObject (aafInt32 index, IAAFDictionary *dict, IAAFDefObject **def)
{
	IAAFInterpolationDef	*interpDef = NULL;
	IAAFDefObject	*obj = NULL;
	aafUID_t		uid;
	
	//!!!Add error checking
	XPROTECT()
	{
		//!!!Later, add in dataDefs supported & filedescriptor class
		CHECK(dict->CreateInstance(&AUID_AAFInterpolationDefinition,
							IID_IAAFInterpolationDef, 
							(IUnknown **)&interpDef));
		uid = LinearInterpolator;
		CHECK(interpDef->QueryInterface(IID_IAAFDefObject, (void **)&obj));
		CHECK(obj->Init(&uid, L"Basic Plugins", L"Handles step and linear interpolation."));
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

//!!!Need some real values for the descriptor
static wchar_t *manufURL = L"http://www.avid.com";
static wchar_t *downloadURL = L"ftp://ftp.avid.com/pub/";
const aafUID_t MANUF_AVID_TECH = { 0xA6487F21, 0xE78F, 0x11d2, { 0x80, 0x9E, 0x00, 0x60, 0x08, 0x14, 0x3E, 0x6F } };
static aafVersionType_t samplePluginVersion = { 0, 1 };

static wchar_t *manufName = L"Avid Technology, Inc.";
static wchar_t *manufRev = L"Rev 0.1";

HRESULT STDMETHODCALLTYPE
    CAAFBasicInterp::CreateDescriptor (IAAFDictionary *dict, IAAFPluginDescriptor **descPtr)
{
	IAAFPluginDescriptor	*desc = NULL;
	IAAFLocator				*pLoc = NULL;
 	IAAFNetworkLocator		*pNetLoc = NULL;
	aafUID_t				category = AUID_AAFDefObject, manufacturer = MANUF_AVID_TECH;
	aafUID_t				plugID = BASIC_INTERP_PLUGIN;
	
	XPROTECT()
	{
		CHECK(dict->CreateInstance(&AUID_AAFPluginDescriptor,
			IID_IAAFPluginDescriptor, 
			(IUnknown **)&desc));
		*descPtr = desc;
		desc->AddRef();
		CHECK(desc->Init(&plugID, L"Example interpolators", L"Handles step and linear interpolation."));
		CHECK(desc->SetCategoryClass(&category));
		CHECK(desc->SetPluginVersionString(manufRev));
		CHECK(dict->CreateInstance(&AUID_AAFNetworkLocator,
			IID_IAAFLocator, 
			(IUnknown **)&pLoc));
		CHECK(pLoc->SetPath (manufURL));
		CHECK(pLoc->QueryInterface(IID_IAAFNetworkLocator, (void **)&pNetLoc));
		CHECK(desc->SetManufacturerInfo(pNetLoc));
		pNetLoc->Release();
		pNetLoc = NULL;
		pLoc->Release();
		pLoc = NULL;

		CHECK(desc->SetManufacturerID(&manufacturer));
		CHECK(desc->SetPluginManufacturerName(manufName));
		CHECK(desc->SetIsSoftwareOnly(AAFTrue));
		CHECK(desc->SetIsAccelerated(AAFFalse));
		CHECK(desc->SetSupportsAuthentication(AAFFalse));
		
		/**/
		CHECK(dict->CreateInstance(&AUID_AAFNetworkLocator,
			IID_IAAFLocator, 
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
\

CAAFBasicInterp::CAAFBasicInterp (IUnknown * pControllingUnknown, aafBool doInit)
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
		/* [out] */aafInt32*  pCount)
{
	if(pCount == NULL)
		return AAFRESULT_NULL_PARAM;
	*pCount = 1;
	return AAFRESULT_SUCCESS;
}

HRESULT STDMETHODCALLTYPE
    CAAFBasicInterp::GetIndexedSupportedType(
		/* [in] */ aafInt32  index,
		/* [out] */IAAFTypeDef ** ppType)
{
	if(ppType == NULL)
		return AAFRESULT_NULL_PARAM;
	if(index < 0 || index >= 1)
		return AAFRESULT_BADINDEX;
//!!!	*ppType = kAAFExpLong;					// temp!!! Use type definition mechanism
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
	aafUID_t		defID;
	
	if(pInputValue == NULL || pOutputValue == NULL)
		return AAFRESULT_NULL_PARAM;
	XPROTECT()
	{
		CHECK(_typeDef->QueryInterface(IID_IAAFDefObject, (void **)&pDef));
		if(pInputValue->denominator == 0)
			RAISE(AAFRESULT_ZERO_DIVIDE);
		inputTime = (AAFRational)*pInputValue;
		CHECK(pDef->GetAUID (&defID));
		if(EqualAUID(&defID, &kAAFExpLong))
		{
			if(bufSize < sizeof(aafUInt32))
				RAISE(AAFRESULT_SMALLBUF);
			aafInt32	*result = (aafInt32 *)pOutputValue;
			aafInt32	lowerBound, upperBound;
			CHECK(FindBoundValues(*pInputValue, sizeof(aafInt32), &timeA, (aafMemPtr_t)&lowerBound,
				&timeB, (aafMemPtr_t)&upperBound));
			*result = (aafInt32)(((inputTime - timeA) / (timeB - timeA)) * (upperBound - lowerBound)) + lowerBound;
			*bytesRead = sizeof(aafUInt32);
		}
		else if(EqualAUID(&defID, &kAAFExpRational))
		{
			aafRational_t	*result = (aafRational_t *)pOutputValue;
			AAFRational		lowerBound, upperBound, subResult, timeDelta;

			if(bufSize < sizeof(aafUInt32))
				RAISE(AAFRESULT_SMALLBUF);
			CHECK(FindBoundValues(*pInputValue, sizeof(aafRational_t), &timeA, (aafMemPtr_t)&lowerBound,
				&timeB, (aafMemPtr_t)&upperBound));
//!!!			if(lowerBound.denominator == 0 || upperBound.denominator == 0)
//				RAISE(AAFRESULT_ZERO_DIVIDE);
			timeDelta = ((inputTime - timeA) / (timeB - timeA));
			subResult = upperBound - lowerBound;
			// Find common denominator
			//
			*result = (aafRational_t)((timeDelta * subResult) + lowerBound);
			*bytesRead = sizeof(aafRational_t);
		}
		else
			RAISE(AAFRESULT_BAD_TYPE);
		pDef->Release();
		pDef = NULL;
	}
	XEXCEPT
	{
		if(pDef)
			pDef->Release();
	}
	XEND;
	return(AAFRESULT_SUCCESS);
}

HRESULT CAAFBasicInterp::InterpolateMany(
	/* [in] */ aafRational_t *  pStartInputValue,
    /* [in] */ aafRational_t *  pInputStep,
    /* [in] */ aafInt32  pGenerateCount,
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
			///!!!Assert count == sizeof(val)
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
			found = AAFFalse;
			prevPoint = NULL;
			while(!found && (theEnum->NextOne(&testPoint) == AAFRESULT_SUCCESS))
			{
				aafRational_t	testRat;
				CHECK(testPoint->GetTime(&testRat));
				testTime = (AAFRational)testRat;

//!!!				if(testRat.denominator == 0)
//					RAISE(AAFRESULT_ZERO_DIVIDE);
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
			
			//!!!Assert if prevPoint and testPoint are both NULL
			if(prevPoint != NULL && testPoint != NULL)		// Real interpolation
			{
				//!!! Fail if prevPoint also is NULL
				CHECK(prevPoint->GetValue (valueSize, lowerBoundValue, &count));
				CHECK(testPoint->GetValue (valueSize, upperBoundValue, &count));
				///!!!Assert count == sizeof(val)
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
				///!!!Assert count == sizeof(val)
				memcpy(upperBoundValue, lowerBoundValue, valueSize);
				*lowerBoundTime = testTime;
				*upperBoundTime = testTime;
				testPoint->Release();
				testPoint = NULL;
			}
			else											// Off of the end
			{
				CHECK(prevPoint->GetValue (valueSize, lowerBoundValue, &count));
				///!!!Assert count == sizeof(val)
				memcpy(upperBoundValue, lowerBoundValue, valueSize);
				*lowerBoundTime = prevTime;
				*upperBoundTime = prevTime;
				prevPoint->Release();
				prevPoint = NULL;
			}
			pVaryingValue->Release();
			pVaryingValue = NULL;
		}
		// else assert bad parameter type!!!
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
HRESULT CAAFBasicInterp::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    HRESULT hr = S_OK;

    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (riid == IID_IAAFInterpolator)
    { 
        *ppvObj = (IAAFInterpolator *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }
    else if (riid == IID_IAAFPlugin) 
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
HRESULT CAAFBasicInterp::COMCreate(IUnknown *pUnkOuter, void **ppvObjOut)
{
	*ppvObjOut = NULL;
 	CAAFBasicInterp *pAAFBasicInterp = new CAAFBasicInterp(pUnkOuter);
 	if (NULL == pAAFBasicInterp)
 		return E_OUTOFMEMORY;
 	*ppvObjOut = static_cast<IAAFInterpolator *>(pAAFBasicInterp);
 	((IUnknown *)(*ppvObjOut))->AddRef();
 	return S_OK;
 }


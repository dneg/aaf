// @doc INTERNAL
// @com This file implements the module test for CAAFDefinitionObject
/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/








#include "CEnumAAFComponents.h"
#include "CEnumAAFComponents.h"
#ifndef __CEnumAAFComponents_h__
#error - improperly defined include guard
#endif

#include "AAFResult.h"
#include "AAFDefUIDs.h"
#include <iostream.h>

#define kNumComponents	5

static HRESULT CreateAAFSequence(IAAFSequence** ppSequence)
{
	IAAFSequence*	pSequence;
	HRESULT			hr;
	aafUInt32		i;

 	hr = CoCreateInstance(CLSID_AAFSequence,
						   NULL, 
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFSequence, 
						   (void **)&pSequence);		
 	if (SUCCEEDED(hr))
	{
		pSequence->SetInitialValue((aafUID_t*)&DDEF_Audio);

		//
		//	Add some segments.  Need to test failure conditions
		//	(i.e. starting/ending w/ transition, two trans back
		//	to bacl).
		//
		for(i = 0; i < kNumComponents; i++)
		{
			IAAFComponent*	pComponent;
			aafLength_t		len = 10;

			hr = CoCreateInstance(CLSID_AAFFiller,
									NULL, 
									CLSCTX_INPROC_SERVER, 
									IID_IAAFComponent, 
									(void **)&pComponent);
 			if (FAILED(hr))
				break;

			pComponent->SetDataDef((aafUID_t*)&DDEF_Audio);
			pComponent->SetLength(&len);
			hr = pSequence->AppendComponent(pComponent);

			pComponent->Release();

			if (FAILED(hr))
				break;
		}
	}

	if (SUCCEEDED(hr))
	{
		*ppSequence = pSequence;
	}
	else
	{
		pSequence->Release();
		*ppSequence = NULL;
	}

	return hr;
}

static HRESULT TestEnumerator(IAAFSequence*	pSequence)
{
	IEnumAAFComponents*	pCompIter;
	IEnumAAFComponents*	pCompCloneIter;
	IAAFComponent*		pComp;
#if 0
	IAAFComponent*		pCompArray[kNumComponents];
	aafUInt32			numFetched, i;
#endif
	HRESULT				hr;
	aafInt32			numCpnts;

	pSequence->GetNumComponents(&numCpnts);
	if (numCpnts != kNumComponents)
		return AAFRESULT_TEST_FAILED;

	hr = pSequence->EnumComponents(&pCompIter);
	if (FAILED(hr))
		return AAFRESULT_TEST_FAILED;

	// Test the NextOne method
	// Indirectly tests the Reset method.
	numCpnts = 0;
	while (pCompIter->NextOne(&pComp) != AAFRESULT_NO_MORE_OBJECTS)
	{
		numCpnts++;
		pComp->Release();
	}

	if (numCpnts != kNumComponents)
	{
		hr = AAFRESULT_TEST_FAILED;
		goto Cleanup;
	}

	// Try to Skip past the end of the sequence.  This
	// test should FAIL.
	pCompIter->Reset();
	hr = pCompIter->Skip(kNumComponents);
	if (SUCCEEDED(hr))
	{
		hr = AAFRESULT_TEST_FAILED;
		goto Cleanup;
	}

#if 0
	// Test the Next method with count == 1
	// Indirectly tests the Reset method.
	pCompIter->Reset();
	numCpnts = 0;
	while (pCompIter->Next(1, &pComp, NULL) != AAFRESULT_NO_MORE_OBJECTS)
	{
		numCpnts++;
		pComp->Release();
	}

	if (numCpnts != kNumComponents)
	{
		hr = AAFRESULT_TEST_FAILED;
		goto Cleanup;
	}

	// Test the Next method with count == kNumComponents
	// Indirectly tests the Reset method.
	pCompIter->Reset();
	numCpnts = 0;
	numFetched = 0;
	while (pCompIter->Next(kNumComponents, (IAAFComponent**)&pCompArray, &numFetched) != AAFRESULT_NO_MORE_OBJECTS)
	{
		numCpnts += numFetched;

		for (i = 0; i < numFetched; i++)
			(pCompArray[i])->Release();
	}

	if (numCpnts != kNumComponents)
	{
		hr = AAFRESULT_TEST_FAILED;
		goto Cleanup;
	}
#endif

	// Test the Clone method with with enumerator at begining
	// Indirectly tests the Reset method.
	pCompIter->Reset();
	hr = pCompIter->Clone(&pCompCloneIter);
	if (FAILED(hr))
		goto Cleanup;

	numCpnts = 0;
	while (pCompCloneIter->NextOne(&pComp) != AAFRESULT_NO_MORE_OBJECTS)
	{
		numCpnts++;
		pComp->Release();
	}
	pCompCloneIter->Release();
	if (numCpnts != kNumComponents)
	{
		hr = AAFRESULT_TEST_FAILED;
		goto Cleanup;
	}

	// Test the Clone method with with enumerator at end.
	// Indirectly tests the Skip and Reset methods.
	pCompIter->Reset();
	pCompIter->Skip(kNumComponents-1);
	hr = pCompIter->Clone(&pCompCloneIter);
	if (FAILED(hr))
		goto Cleanup;

	numCpnts = 0;
	while (pCompCloneIter->NextOne(&pComp) != AAFRESULT_NO_MORE_OBJECTS)
	{
		numCpnts++;
		pComp->Release();
	}
	pCompCloneIter->Release();
	if (numCpnts != 1)
	{
		hr = AAFRESULT_TEST_FAILED;
		goto Cleanup;
	}

	// Test the Clone method with with enumerator in the middle.
	// Indirectly tests the Skip and Reset methods.
	pCompIter->Reset();
	pCompIter->Skip(kNumComponents-2);
	hr = pCompIter->Clone(&pCompCloneIter);
	if (FAILED(hr))
		goto Cleanup;

	pCompCloneIter->Skip(1);
	numCpnts = 0;
	while (pCompCloneIter->NextOne(&pComp) != AAFRESULT_NO_MORE_OBJECTS)
	{
		numCpnts++;
		pComp->Release();
	}
	pCompCloneIter->Release();
	if (numCpnts != 1)
	{
		hr = AAFRESULT_TEST_FAILED;
		goto Cleanup;
	}

Cleanup:
	pCompIter->Release();

	return hr;
}

HRESULT CEnumAAFComponents::test()
{
	IAAFSequence*	pSequence;
	HRESULT			hr = AAFRESULT_NOT_IMPLEMENTED;

	try
	{
		hr = CreateAAFSequence(&pSequence);
		if (SUCCEEDED(hr))
		{
			hr = TestEnumerator(pSequence);
			pSequence->Release();
		}
	}
	catch (...)
	{
		cerr << "CEnumAAFComponents::test...Caught general C++ exception!" << endl; 
	}

	// When all of the functionality of this class is tested, we can return success
	if (SUCCEEDED(hr))
		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;


	return hr;
}














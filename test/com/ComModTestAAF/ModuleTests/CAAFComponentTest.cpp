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

#ifndef __CAAFComponent_h__
#include "CAAFComponent.h"
#endif

#include <iostream.h>
#include "AAFResult.h"
#include "AAFDefUIDs.h"

// Temporarily necessary global declarations.
extern "C" const CLSID CLSID_AAFComponent;


HRESULT CAAFComponent::test()
{
	HRESULT			hr = AAFRESULT_NOT_IMPLEMENTED;
	IAAFComponent	*pObject = NULL;
	aafLength_t		inLength, outLength;
	aafUID_t		inDatadef, outDatadef;

	try
	{
		// Attempt to create an AAFComponent.
		hr =  CoCreateInstance(
							 CLSID_AAFComponent,
							 NULL, 
							 CLSCTX_INPROC_SERVER, 
							 IID_IAAFComponent, (void **)&pObject);
		if (FAILED(hr))
		{
			cerr << "CAAFComponent::test...FAILED!";
			cerr << hr;
			cerr << "\tCoCreateInstance(&CLSID_AAFComponent, NULL,"
				" CLSCTX_INPROC_SERVER, &IID_IAAFComponent, ...);" <<
				endl;
			return hr;
		}

		// module-specific tests go here
		// Set values
		inLength = 4294967295;		// Could have been any other value
		hr = pObject->SetLength( &inLength);
		if (FAILED(hr))
		{
			if (pObject)
				pObject->Release();
			return hr;
		}

		inDatadef = DDEF_Video;		// Could have been any other value
		hr = pObject->SetDataDef( &inDatadef );
		if (FAILED(hr))
		{
			if (pObject)
				pObject->Release();
			return hr;
		}

		//	Now Get Values
		hr = pObject->GetLength( &outLength);
		if (FAILED(hr))
		{
			if (pObject)
				pObject->Release();
			return hr;
		}
		// Compare the out value with the in value
		if (inLength != outLength)
		{
			if (pObject)
				pObject->Release();
			hr = AAFRESULT_TEST_FAILED;
			return hr;
		}

		hr = pObject->GetDataDef( &outDatadef);
		if (FAILED(hr))
		{
			if (pObject)
				pObject->Release();
			return hr;
		}
		// Compare the out value with the in value
		if (memcmp(&inDatadef, &outDatadef, sizeof(inDatadef)) != 0)
		{
			if (pObject)
				pObject->Release();
			hr = AAFRESULT_TEST_FAILED;
			return hr;
		}

		if (pObject)
			pObject->Release();
		return AAFRESULT_SUCCESS;

	}
	catch (...)
	{
		cerr << "CAAFComponent::test...Caught general C++"
		" exception!" << endl; 
	}

	// Cleanup our object if it exists.
	if (pObject)
		pObject->Release();
	
	return hr;
}





	







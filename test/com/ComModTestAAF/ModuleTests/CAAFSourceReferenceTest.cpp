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




#ifndef __CAAFSourceReference_h__
#include "CAAFSourceReference.h"
#endif

#include <iostream.h>
#include "AAFResult.h"
#include "AAFDefUIDs.h"


HRESULT CAAFSourceReference::test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	IAAFSourceReference *pObject = NULL;
	aafUID_t		inSourceID;
	aafUID_t		outSourceID;
	aafSlotID_t		inMobSlotID;
	aafSlotID_t		outMobSlotID;

	try
	{
		// Attempt to create an AAFSourceReference.
		hr = CoCreateInstance(
							 CLSID_AAFSourceReference,
							 NULL, 
							 CLSCTX_INPROC_SERVER, 
							 IID_IAAFSourceReference, (void **)&pObject);
		if (FAILED(hr))
		{
			cerr << "CAAFSourceReference::test...FAILED!";
			cerr << hr;
			cerr << "\tCoCreateInstance(&CLSID_AAFSourceReference, NULL,"
			" CLSCTX_INPROC_SERVER, &IID_IAAFSourceReference, ...);" <<
			  endl;
			return hr;
		}

		// module-specific tests go here
		//		Set Values.	

		inSourceID = DDEF_Video;   // Could have been any other value !
		hr = pObject->SetSourceID( inSourceID);
		if (FAILED(hr))
		{
			if (pObject)
				pObject->Release();
			return hr;
		}
		
		inMobSlotID = 100;   // Could have been any other value !
		hr = pObject->SetSourceMobSlotID( inMobSlotID);
		if (FAILED(hr))
		{
			if (pObject)
				pObject->Release();
			return hr;
		}

		//	Now Get Values
		hr = pObject->GetSourceID( &outSourceID);
		if (FAILED(hr))
		{
			if (pObject)
				pObject->Release();
			return hr;
		}
		// Compare value with the one we set
		if (memcmp(&inSourceID, &outSourceID, sizeof(inSourceID)) != 0)
		{
			if (pObject)
				pObject->Release();
			hr = AAFRESULT_TEST_FAILED;
			return hr;
		}

		
		hr = pObject->GetSourceMobSlotID( &outMobSlotID);
		if (FAILED(hr))
		{
			if (pObject)
				pObject->Release();
			return hr;
		}
		// Compare value with the one we set
		if (inMobSlotID != outMobSlotID)
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
		cerr << "CAAFSourceReference::test...Caught general C++"
		" exception!" << endl; 
	}

	// Cleanup our object if it exists.
	if (pObject)
		pObject->Release();

	return hr;
}












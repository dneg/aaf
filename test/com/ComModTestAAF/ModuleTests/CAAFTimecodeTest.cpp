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



#ifndef __CAAFTimecode_h__
#include "CAAFTimecode.h"
#endif

#include <iostream.h>
#include "aafCvt.h"
#include "AAFResult.h"

static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	// IAAFSession *				pSession = NULL;
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;


	IAAFCompositionMob*			pCompMob=NULL;
	IAAFMob						*pMob = NULL;
	IAAFMobSlot					*pNewSlot = NULL;
	IAAFTimecode				*pTimecode = NULL;
	IAAFSegment					*pSeg = NULL;

	aafUID_t					newMobID;
	aafProductIdentification_t	ProductInfo;
	HRESULT						hr;
	aafLength_t					zero;
	aafTimecode_t				startTC;

	CvtInt32toLength(0, zero);
	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"Make AVR Example";
	ProductInfo.productVersion.major = 1;
	ProductInfo.productVersion.minor = 0;
	ProductInfo.productVersion.tertiary = 0;
	ProductInfo.productVersion.patchLevel = 0;
	ProductInfo.productVersion.type = kVersionUnknown;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = -1;
	ProductInfo.platform = NULL;

	/*
	hr = CoCreateInstance(CLSID_AAFSession,
						   NULL, 
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFSession, 
						   (void **)&pSession);
	*/
	hr = CoCreateInstance(CLSID_AAFFile,
						   NULL, 
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFFile, 
						   (void **)&pFile);
	if (AAFRESULT_SUCCESS == hr)
	{
		// We assume the following functions have been tested and they do work
		// The next 3 function calls open the AAF file
	    // hr = pSession->SetDefaultIdentification(&ProductInfo);
		// hr = pSession->CreateFile(pFileName, kAAFRev1, &pFile);
		hr = pFile->Initialize();
	    hr = pFile->OpenNewModify(pFileName, 0, &ProductInfo);
	  	hr = pFile->GetHeader(&pHeader);

		// Create a CompositionMob
		hr = CoCreateInstance(CLSID_AAFCompositionMob,
							NULL, 
							CLSCTX_INPROC_SERVER, 
							IID_IAAFCompositionMob, 
							(void **)&pCompMob);
		if (AAFRESULT_SUCCESS == hr)
		{	
			// Get a MOB interface
			hr = pCompMob->QueryInterface (IID_IAAFMob, (void **)&pMob);
			CoCreateGuid((GUID *)&newMobID);
			hr = pMob->SetMobID(&newMobID);

			hr = pCompMob->Initialize(L"COMPMOB01");

			hr = CoCreateInstance(CLSID_AAFMobSlot,
									NULL, 
									CLSCTX_INPROC_SERVER, 
									IID_IAAFMobSlot, 
									(void **)&pNewSlot);

			hr = CoCreateInstance(CLSID_AAFTimecode,
									NULL, 
									CLSCTX_INPROC_SERVER, 
									IID_IAAFTimecode, 
									(void **)&pTimecode);		

			startTC.startFrame = 108000;	// One hour
			startTC.drop = kTcNonDrop;
			startTC.fps = 30;
			hr = pTimecode->Initialize (zero, &startTC);
			if (AAFRESULT_SUCCESS == hr)
			{
				hr = pTimecode->QueryInterface (IID_IAAFSegment, (void **)&pSeg);

				hr = pMob->AppendNewSlot (pSeg, 0, L"timecode", &pNewSlot);
			}
		}
		
		hr = pHeader->AppendMob(pMob);
	}

	// Cleanup and return
	if (pFile) 
	{
		pFile->Close();
		pFile->Release();
	}

	/*
	if (pSession)
	{
		pSession->EndSession();
		pSession->Release();
	}
	*/

	if (pHeader)
		pHeader->Release();

	if (pCompMob)
		pCompMob->Release();

	if (pMob)
		pMob->Release();

	if (pTimecode)
		pTimecode->Release();

	if (pNewSlot)
		pNewSlot->Release();

	if (pSeg)
		pSeg->Release();

	return hr;
}


static HRESULT ReadAAFFile(aafWChar * pFileName)
{
    // IAAFSession *				pSession = NULL;
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;

	IEnumAAFMobs*				pMobIter = NULL;
	IEnumAAFMobSlots*			pEnum = NULL;
	IAAFMob*					pMob = NULL;
	IAAFCompositionMob*			pCompMob = NULL;
	IAAFMobSlot*				pMobSlot = NULL;
	IAAFSegment*				pSeg = NULL;
	IAAFTimecode*				pTimecode = NULL;
	aafTimecode_t				startTC;

	aafProductIdentification_t	ProductInfo;
	aafNumSlots_t				numMobs;
	HRESULT						hr;

	ProductInfo.companyName = L"AAF Developers Desk. NOT!";
	ProductInfo.productName = L"Make AVR Example. NOT!";
	ProductInfo.productVersion.major = 1;
	ProductInfo.productVersion.minor = 0;
	ProductInfo.productVersion.tertiary = 0;
	ProductInfo.productVersion.patchLevel = 0;
	ProductInfo.productVersion.type = kVersionUnknown;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = -1;
	ProductInfo.platform = NULL;
	  
	/*
	hr = CoCreateInstance(CLSID_AAFSession,
						   NULL, 
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFSession, 
						   (void **)&pSession);
    */
	hr = CoCreateInstance(CLSID_AAFFile,
						   NULL, 
						   CLSCTX_INPROC_SERVER, 
						   IID_IAAFFile, 
						   (void **)&pFile);
	if (AAFRESULT_SUCCESS == hr)
	{
		// We assume the following functions have been tested and they do work
		// The next 3 function calls open the AAF file
		// hr = pSession->SetDefaultIdentification(&ProductInfo);
		// hr = pSession->OpenReadFile(pFileName, &pFile);
		hr = pFile->Initialize();
		hr = pFile->OpenExistingRead(pFileName, 0);
	  	hr = pFile->GetHeader(&pHeader);

		// Get the number of mobs in the file (should be one)
		hr = pHeader->GetNumMobs( kAllMob, &numMobs );
		if (1 == numMobs)
		{
			hr = pHeader->EnumAAFAllMobs( NULL, &pMobIter );
			while (pMobIter && pMobIter->NextOne(&pMob) !=AAFRESULT_NO_MORE_MOBS)
			{

				hr = pMob->EnumAAFAllMobSlots (&pEnum);
				hr = pEnum->NextOne (&pMobSlot);
				if (AAFRESULT_SUCCESS == hr)
				{
					hr = pMobSlot->GetSegment (&pSeg);
					if (AAFRESULT_SUCCESS == hr)
					{
						// Get a Timecode interface 
						hr = pSeg->QueryInterface (IID_IAAFTimecode, (void **)&pTimecode);
 						if (AAFRESULT_SUCCESS == hr)
						{
							hr = pTimecode->GetTimecode (&startTC);
				 			if (AAFRESULT_SUCCESS != hr)
							{
								// Check results !!
								if(startTC.startFrame != 108000)
									hr = AAFRESULT_TEST_FAILED;
								if(startTC.drop != kTcNonDrop)
									hr = AAFRESULT_TEST_FAILED;
								if(startTC.fps != 30)
									hr = AAFRESULT_TEST_FAILED;
							}
						}
						else
						{
							hr = AAFRESULT_TEST_FAILED;
						}
					}
					else
					{
						hr = AAFRESULT_TEST_FAILED;
					}
				}
				else 
				{
					hr = AAFRESULT_TEST_FAILED;
				}
			}
		}
		else
		{
			hr = AAFRESULT_TEST_FAILED;
		}
	}


	// Cleanup and return
	if (pFile) 
	{
		pFile->Close();
		pFile->Release();
	}

	/*
	if (pSession)
	{
		pSession->EndSession();
		pSession->Release();
	}
	*/

	if (pHeader)
		pHeader->Release();

	if (pMob)
		pMob->Release();

	if (pCompMob)
		pCompMob->Release();

	if (pMobIter)
		pMobIter->Release();

	if (pMobSlot)
		pMobSlot->Release();

	if (pEnum)
		pEnum->Release();

	if (pSeg)
		pSeg->Release();

	if (pTimecode)
		pTimecode->Release();

	return hr;
}

HRESULT CAAFTimecode::test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"TimecodeTest.aaf";

	try
	{
		hr = CreateAAFFile(	pFileName );
		if(hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );
	}
	catch (...)
	{
	  cerr << "CAAFTimecodeMob::test...Caught general C++"
		" exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}

  	// When all of the functionality of this class is tested, we can return success
	if(hr == AAFRESULT_SUCCESS)
		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;

	// Cleanup our object if it exists.
	return hr;
}



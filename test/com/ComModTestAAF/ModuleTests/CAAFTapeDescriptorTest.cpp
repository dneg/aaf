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

#ifndef __CAAFTapeDescriptor_h__
#include "CAAFTapeDescriptor.h"
#endif

#include <iostream.h>
#include "AAFResult.h"
#include "AAFDefUIDs.h"

static aafWChar* Manufacturer = L"Sony";
static aafWChar* Model = L"MyModel";
static aafTapeCaseType_t FormFactor = kVHSVideoTape;
static aafVideoSignalType_t VideoSignalType = kPALSignal;
static aafTapeFormatType_t TapeFormat = kVHSFormat;
static aafLength_t TapeLength = 3200 ;

static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	// IAAFSession *				pSession = NULL;
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;

	IAAFSourceMob*				pSourceMob;
	IAAFMob*					pMob;
	IAAFEssenceDescriptor*		pEssDesc;
	IAAFTapeDescriptor*			pTapeDesc;

	aafProductIdentification_t	ProductInfo;
	aafUID_t					newUID;
	HRESULT						hr = AAFRESULT_SUCCESS;


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
		hr = pFile->Initialize();
		if (AAFRESULT_SUCCESS == hr)
		{
			// hr = pSession->CreateFile(pFileName, kAAFRev1, &pFile);
			hr = pFile->OpenNewModify(pFileName, 0, &ProductInfo);
			if (AAFRESULT_SUCCESS == hr)
			{
				hr = pFile->GetHeader(&pHeader);
				if (AAFRESULT_SUCCESS == hr)
				{
					// Create a source mob
					hr = CoCreateInstance(CLSID_AAFSourceMob,
											NULL, 
											CLSCTX_INPROC_SERVER, 
											IID_IAAFSourceMob, 
											(void **)&pSourceMob);
					if (AAFRESULT_SUCCESS == hr)
					{
						hr = pSourceMob->QueryInterface(IID_IAAFMob, (void **)&pMob);
						if (AAFRESULT_SUCCESS == hr)
						{
							newUID.Data1 = 0;
							pMob->SetMobID(&newUID);
							pMob->SetName(L"TapeDescriptorTest");
							hr = CoCreateInstance(CLSID_AAFTapeDescriptor,
													NULL, 
													CLSCTX_INPROC_SERVER, 
													IID_IAAFTapeDescriptor, 
													(void **)&pTapeDesc);		
 							if (AAFRESULT_SUCCESS == hr)
							{
								hr = pTapeDesc->QueryInterface(IID_IAAFEssenceDescriptor, (void **)&pEssDesc);
								if (AAFRESULT_SUCCESS == hr)
								{
									hr = pSourceMob->SetEssenceDescription(pEssDesc);
									if (AAFRESULT_SUCCESS == hr)
									{
										hr = pTapeDesc->SetTapeManufacturer( Manufacturer );
										if (AAFRESULT_SUCCESS == hr )
										{
											hr = pTapeDesc->SetTapeModel( Model );
											if (AAFRESULT_SUCCESS == hr )
											{
												hr = pTapeDesc->SetTapeFormFactor( FormFactor );
												if (AAFRESULT_SUCCESS == hr )
												{
													hr = pTapeDesc->SetSignalType( VideoSignalType );
													if (AAFRESULT_SUCCESS == hr )
													{
														hr = pTapeDesc->SetTapeFormat( TapeFormat );
														if (AAFRESULT_SUCCESS == hr )
															hr = pTapeDesc->SetTapeLength( TapeLength );
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	// Add the MOB to the file
	if (AAFRESULT_SUCCESS == hr)
		hr = pHeader->AppendMob(pMob);

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

	if (pSourceMob)
		pSourceMob->Release();
	
	if (pMob)
		pMob->Release();

	if (pTapeDesc)
		pTapeDesc->Release();
	
	if (pEssDesc)
		pEssDesc->Release();

	return hr;
}

static HRESULT ReadAAFFile(aafWChar * pFileName)
{
	// IAAFSession *				pSession = NULL;
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;

	IAAFSourceMob*				pSourceMob;
	IAAFMob*					pMob;
	IAAFEssenceDescriptor*		pEssDesc;
	IAAFTapeDescriptor*			pTapeDesc;
	IEnumAAFMobs*				pMobIter;

	aafProductIdentification_t	ProductInfo;
	aafNumSlots_t				numMobs;

	aafWChar					readManufacturer[256];
	aafWChar					readModel[256];
	aafTapeCaseType_t			readFormFactor;
	aafVideoSignalType_t		readVideoSignalType;
	aafTapeFormatType_t			readTapeFormat;
	aafLength_t					readTapeLength ;
	aafInt32					length;

	HRESULT						hr = AAFRESULT_SUCCESS;

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
	    hr = pFile->Initialize();
		if (AAFRESULT_SUCCESS == hr)
		{
			// hr = pSession->OpenReadFile(pFileName, &pFile);
			hr = pFile->OpenExistingRead(pFileName, 0);
			if (AAFRESULT_SUCCESS == hr)
			{
				hr = pFile->GetHeader(&pHeader);
				if (AAFRESULT_SUCCESS == hr)
				{
					hr = pHeader->GetNumMobs(kAllMob, &numMobs);
					if (AAFRESULT_SUCCESS == hr)
					{
						if (1 == numMobs )
						{
							// We assume these next few calls WILL complete succesfully
							// since we  are the ones who created the file and know it contents
							
							pHeader->EnumAAFAllMobs(NULL, &pMobIter);
							pMobIter->NextOne(&pMob);
							pMob->QueryInterface(IID_IAAFSourceMob, (void **)&pSourceMob);
												 
							// Back into testing mode
							hr = pSourceMob->GetEssenceDescription(&pEssDesc);
							if (AAFRESULT_SUCCESS == hr)
							{
								// if there is an Essence Descriptor then it MUST be an (essence) TapeDescriptor
								pEssDesc->QueryInterface(IID_IAAFTapeDescriptor, (void **) &pTapeDesc);
								pTapeDesc->GetTapeManBufLen(&length);
								hr = pTapeDesc->GetTapeManufacturer(readManufacturer, length);
								if (AAFRESULT_SUCCESS == hr)
								{
									pTapeDesc->GetTapeModelBufLen(&length);
									hr = pTapeDesc->GetTapeModel(readModel, length);
									if (AAFRESULT_SUCCESS == hr)
									{
										hr = pTapeDesc->GetTapeFormFactor(&readFormFactor);
										hr = pTapeDesc->GetSignalType(&readVideoSignalType);
										hr = pTapeDesc->GetTapeFormat( &readTapeFormat);
										hr = pTapeDesc->GetTapeLength( &readTapeLength);

										if (( wcscmp(Manufacturer, readManufacturer) != 0) ||
											( wcscmp(Model, readModel) != 0) ||
											( FormFactor != readFormFactor) ||
											( VideoSignalType != readVideoSignalType) ||
											( TapeFormat != readTapeFormat ) ||
											( TapeLength != readTapeLength))
											hr = AAFRESULT_TEST_FAILED;
									}
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
			}
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

	if (pSourceMob)
		pSourceMob->Release();
	
	if (pMob)
		pMob->Release();

	if (pTapeDesc)
		pTapeDesc->Release();
	
	if (pEssDesc)
		pEssDesc->Release();

	return hr;
}

HRESULT CAAFTapeDescriptor::test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"TapeDescTest.aaf";

	try
	{
		hr = CreateAAFFile( pFileName );
		if (hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );
	}
	catch (...)
	{
		cerr << "CAAFTapeDescriptor::test...Caught general C++"
			" exception!" << endl; 
	}

	return hr;
}
























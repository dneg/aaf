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
#include <stdio.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "AAFDefUIDs.h"

static aafWChar* Manufacturer = L"Sony";
static aafWChar* Model = L"MyModel";
static aafTapeCaseType_t FormFactor = kVHSVideoTape;
static aafVideoSignalType_t VideoSignalType = kPALSignal;
static aafTapeFormatType_t TapeFormat = kVHSFormat;
static aafLength_t TapeLength = 3200 ;



// Cross-platform utility to delete a file.
static void RemoveTestFile(const wchar_t* pFileName)
{
  const size_t kMaxFileName = 512;
  char cFileName[kMaxFileName];

  size_t status = wcstombs(cFileName, pFileName, kMaxFileName);
  if (status != (size_t)-1)
  { // delete the file.
    remove(cFileName);
  }
}

// convenient error handlers.
inline void checkResult(HRESULT r)
{
  if (FAILED(r))
    throw r;
}
inline void checkExpression(bool expression, HRESULT r)
{
  if (!expression)
    throw r;
}



static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;
  IAAFDictionary*  pDictionary = NULL;
	IAAFSourceMob*				pSourceMob = NULL;
	IAAFMob*					pMob = NULL;
	IAAFEssenceDescriptor*		pEssDesc = NULL;
	IAAFTapeDescriptor*			pTapeDesc = NULL;

	aafProductIdentification_t	ProductInfo;
	aafUID_t					newUID;
	HRESULT						hr = AAFRESULT_SUCCESS;


	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFTapeDescriptor Test";
	ProductInfo.productVersion.major = 1;
	ProductInfo.productVersion.minor = 0;
	ProductInfo.productVersion.tertiary = 0;
	ProductInfo.productVersion.patchLevel = 0;
	ProductInfo.productVersion.type = kVersionUnknown;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = -1;
	ProductInfo.platform = NULL;

  // Remove the previous test file if any.
  RemoveTestFile(pFileName);

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
			hr = pFile->OpenNewModify(pFileName, 0, &ProductInfo);
			if (AAFRESULT_SUCCESS == hr)
			{
				hr = pFile->GetHeader(&pHeader);
				if (AAFRESULT_SUCCESS == hr)
				{
          hr = pHeader->GetDictionary(&pDictionary);
				  if (AAFRESULT_SUCCESS == hr)
				  {
					  // Create a source mob

					  hr = pDictionary->CreateInstance(&AUID_AAFSourceMob,
											  IID_IAAFSourceMob, 
											  (IUnknown **)&pSourceMob);
					  if (AAFRESULT_SUCCESS == hr)
					  {
						  hr = pSourceMob->QueryInterface(IID_IAAFMob, (void **)&pMob);
						  if (AAFRESULT_SUCCESS == hr)
						  {
							  CoCreateGuid((GUID *)&newUID);
							  pMob->SetMobID(&newUID);
							  pMob->SetName(L"TapeDescriptorTest");
							  hr = pDictionary->CreateInstance(&AUID_AAFTapeDescriptor,
													  IID_IAAFTapeDescriptor, 
													  (IUnknown **)&pTapeDesc);		
 							  if (AAFRESULT_SUCCESS == hr)
							  {
								  hr = pTapeDesc->QueryInterface(IID_IAAFEssenceDescriptor, (void **)&pEssDesc);
								  if (AAFRESULT_SUCCESS == hr)
								  {
									  hr = pSourceMob->SetEssenceDescriptor(pEssDesc);
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
									  pEssDesc->Release();
									  pEssDesc = NULL;
								  }
								  pTapeDesc->Release();
								  pTapeDesc = NULL;
							  }

							  // Add the MOB to the file
							  if (AAFRESULT_SUCCESS == hr)
								  hr = pHeader->AppendMob(pMob);

							  pMob->Release();
							  pMob = NULL;
						  }
						  pSourceMob->Release();
						  pSourceMob = NULL;
            }

					  pDictionary->Release();
					  pDictionary = NULL;
					}
					pHeader->Release();
					pHeader = NULL;
				}
				pFile->Save();
				pFile->Close();
			}
		}
		pFile->Release();
		pFile = NULL;
	}



	return hr;
}

static HRESULT ReadAAFFile(aafWChar * pFileName)
{
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;

	IAAFSourceMob*				pSourceMob = NULL;
	IAAFMob*					pMob = NULL;
	IAAFEssenceDescriptor*		pEssDesc = NULL;
	IAAFTapeDescriptor*			pTapeDesc = NULL;
	IEnumAAFMobs*				pMobIter = NULL;

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
	ProductInfo.productName = L"AAFTapeDescriptor Test";
	ProductInfo.productVersion.major = 1;
	ProductInfo.productVersion.minor = 0;
	ProductInfo.productVersion.tertiary = 0;
	ProductInfo.productVersion.patchLevel = 0;
	ProductInfo.productVersion.type = kVersionUnknown;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = -1;
	ProductInfo.platform = NULL;


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
							hr = pHeader->EnumAAFAllMobs(NULL, &pMobIter);
							if (AAFRESULT_SUCCESS == hr)
							{
								hr = pMobIter->NextOne(&pMob);
								if (AAFRESULT_SUCCESS == hr)
								{
									hr = pMob->QueryInterface(IID_IAAFSourceMob, (void **)&pSourceMob);
									if (AAFRESULT_SUCCESS == hr)
									{					 
										// Back into testing mode
										hr = pSourceMob->GetEssenceDescriptor(&pEssDesc);
										if (AAFRESULT_SUCCESS == hr)
										{
											// if there is an Essence Descriptor then it MUST be an (essence) TapeDescriptor
											hr = pEssDesc->QueryInterface(IID_IAAFTapeDescriptor, (void **) &pTapeDesc);
											if (AAFRESULT_SUCCESS == hr)
											{
												hr = pTapeDesc->GetTapeManBufLen(&length);
												if (AAFRESULT_SUCCESS == hr)
												{
													hr = pTapeDesc->GetTapeManufacturer(readManufacturer, length);
													if (AAFRESULT_SUCCESS == hr)
													{
														hr = pTapeDesc->GetTapeModelBufLen(&length);
														if (AAFRESULT_SUCCESS == hr)
														{
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
												}
												pTapeDesc->Release();
												pTapeDesc = NULL;
											}
											pEssDesc->Release();
											pEssDesc = NULL;
										}
										else
										{
											hr = AAFRESULT_TEST_FAILED;
										}
										pSourceMob->Release();
										pSourceMob = NULL;
									}
									pMob->Release();
									pMob = NULL;
								}
								pMobIter->Release();
								pMobIter = NULL;
							}
						}
						else
						{
							hr = AAFRESULT_TEST_FAILED;
						}
					}
					pHeader->Release();
					pHeader = NULL;
				}

				pFile->Close();
			}
		}

		pFile->Release();
		pFile = NULL;
	}

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

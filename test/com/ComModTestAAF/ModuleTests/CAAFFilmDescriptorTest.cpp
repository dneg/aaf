// @doc INTERNAL
// @com This file implements the module test for CAAFFilmDescriptor
/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#include "AAF.h"

#include <iostream.h>
#include <stdio.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "AAFDefUIDs.h"

static aafWChar* Manufacturer = L"Sony";
static aafWChar* Model = L"MyModel";
static aafFilmType_t FilmFormat = kFt35MM;
static aafUInt32 FrameRate = 24;
static aafUInt8 PerfPerFrame = 4;
static aafRational_t	AspectRatio = { 1000, 1 };	// !!!Find a real aspect ratio

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
	IAAFFilmDescriptor*			pFilmDesc = NULL;
	
	aafProductIdentification_t	ProductInfo;
	aafUID_t					newUID;
	HRESULT						hr = AAFRESULT_SUCCESS;
	
	
	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFFilmDescriptor Test";
	ProductInfo.productVersion.major = 1;
	ProductInfo.productVersion.minor = 0;
	ProductInfo.productVersion.tertiary = 0;
	ProductInfo.productVersion.patchLevel = 0;
	ProductInfo.productVersion.type = kVersionUnknown;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = UnitTestProductID;
	ProductInfo.platform = NULL;
	
	// Remove the previous test file if any.
	RemoveTestFile(pFileName);
	
	checkResult(AAFFileOpenNewModify(pFileName, 0, &ProductInfo, &pFile));
	checkResult(pFile->GetHeader(&pHeader));
	checkResult(pHeader->GetDictionary(&pDictionary));

	// Create a film mob
	checkResult(pDictionary->CreateInstance(&AUID_AAFSourceMob,
					IID_IAAFSourceMob, 
					(IUnknown **)&pSourceMob));
	checkResult(pSourceMob->QueryInterface(IID_IAAFMob, (void **)&pMob));
	CoCreateGuid((GUID *)&newUID);
	pMob->SetMobID(&newUID);
	pMob->SetName(L"FilmDescriptorTest");
	checkResult(pDictionary->CreateInstance(&AUID_AAFFilmDescriptor,
							IID_IAAFFilmDescriptor, 
							(IUnknown **)&pFilmDesc));		
	checkResult(pFilmDesc->QueryInterface(IID_IAAFEssenceDescriptor, (void **)&pEssDesc));
	checkResult(pSourceMob->SetEssenceDescriptor(pEssDesc));
	checkResult(pFilmDesc->SetFilmManufacturer( Manufacturer ));
	checkResult(pFilmDesc->SetFilmModel( Model ));
	checkResult(pFilmDesc->SetFilmFormat( FilmFormat ));
	checkResult(pFilmDesc->SetFrameRate( FrameRate ));
	checkResult(pFilmDesc->SetPerfPerFrame( PerfPerFrame ));
	checkResult(pFilmDesc->SetFilmAspectRatio( AspectRatio ));
	pEssDesc->Release();
	pEssDesc = NULL;
	pFilmDesc->Release();
	pFilmDesc = NULL;
	// Add the MOB to the file
	checkResult(pHeader->AppendMob(pMob));
						
	pMob->Release();
	pMob = NULL;
	pSourceMob->Release();
	pSourceMob = NULL;

	pDictionary->Release();
	pDictionary = NULL;
	pHeader->Release();
	pHeader = NULL;

	pFile->Save();
	pFile->Close();
		
	pFile->Release();
	pFile = NULL;	
	
	return AAFRESULT_SUCCESS;
}

static HRESULT ReadAAFFile(aafWChar * pFileName)
{
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;
	
	IAAFSourceMob*				pSourceMob = NULL;
	IAAFMob*					pMob = NULL;
	IAAFEssenceDescriptor*		pEssDesc = NULL;
	IAAFFilmDescriptor*			pFilmDesc = NULL;
	IEnumAAFMobs*				pMobIter = NULL;
	
	aafProductIdentification_t	ProductInfo;
	aafNumSlots_t				numMobs;
	
	aafWChar					readManufacturer[256];
	aafWChar					readModel[256];
	aafFilmType_t				readFilmFormat;
	aafUInt32					readFrameRate;
	aafUInt8					readPerfPerFrame;
	aafRational_t				readAspectRatio;
	aafInt32					length;

	HRESULT						hr = AAFRESULT_SUCCESS;
	
	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFFilmDescriptor Test";
	ProductInfo.productVersion.major = 1;
	ProductInfo.productVersion.minor = 0;
	ProductInfo.productVersion.tertiary = 0;
	ProductInfo.productVersion.patchLevel = 0;
	ProductInfo.productVersion.type = kVersionUnknown;
	ProductInfo.productVersionString = NULL;
	ProductInfo.platform = NULL;
	
	checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
	checkResult(pFile->GetHeader(&pHeader));
	checkResult(pHeader->GetNumMobs(kAllMob, &numMobs));
	checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);
	checkResult(pHeader->EnumAAFAllMobs(NULL, &pMobIter));
	checkResult(pMobIter->NextOne(&pMob));
	checkResult(pMob->QueryInterface(IID_IAAFSourceMob, (void **)&pSourceMob));
	// Back into testing mode
	checkResult(pSourceMob->GetEssenceDescriptor(&pEssDesc));
	// if there is an Essence Descriptor then it MUST be an (essence) FilmDescriptor
	checkResult(pEssDesc->QueryInterface(IID_IAAFFilmDescriptor, (void **) &pFilmDesc));
	checkResult(pFilmDesc->GetFilmManufacturerBufLen(&length));
	checkResult(pFilmDesc->GetFilmManufacturer(readManufacturer, length));
	checkExpression(wcscmp(Manufacturer, readManufacturer) == 0, AAFRESULT_TEST_FAILED);
	checkResult(pFilmDesc->GetFilmModelBufLen(&length));
	checkResult(pFilmDesc->GetFilmModel(readModel, length));
	checkExpression(wcscmp(Model, readModel) == 0, AAFRESULT_TEST_FAILED);
	checkResult(pFilmDesc->GetFilmFormat(&readFilmFormat));
	checkExpression(FilmFormat == readFilmFormat, AAFRESULT_TEST_FAILED);
	checkResult(pFilmDesc->GetFrameRate(&readFrameRate));
	checkExpression(FrameRate == readFrameRate, AAFRESULT_TEST_FAILED);
	checkResult(pFilmDesc->GetPerfPerFrame( &readPerfPerFrame));
	checkExpression(PerfPerFrame == readPerfPerFrame, AAFRESULT_TEST_FAILED);
	checkResult(pFilmDesc->GetFilmAspectRatio( &readAspectRatio));
	checkExpression(AspectRatio.numerator == readAspectRatio.numerator, AAFRESULT_TEST_FAILED);
	checkExpression(AspectRatio.denominator == readAspectRatio.denominator, AAFRESULT_TEST_FAILED);

	pFilmDesc->Release();
	pFilmDesc = NULL;
	pEssDesc->Release();
	pEssDesc = NULL;
	pSourceMob->Release();
	pSourceMob = NULL;
	pMob->Release();
	pMob = NULL;
	pMobIter->Release();
	pMobIter = NULL;
	pHeader->Release();
	pHeader = NULL;
	
	pFile->Close();
	
    pFile->Release();
	pFile = NULL;
	
	return AAFRESULT_SUCCESS;
}

extern "C" HRESULT CAAFFilmDescriptor_test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"AAFFilmDescriptorTest.aaf";

	try
	{
		hr = CreateAAFFile( pFileName );
		if (hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );
	}
	catch (...)
	{
		cerr << "CAAFFilmDescriptor_test...Caught general C++"
			" exception!" << endl; 
	}

//	if (SUCCEEDED(hr))
//	{
//		cout << "The following IEnumAAFPluginDescriptor methods have not been tested:" << endl;       
//		cout << "     SetFilmManufacturer" << endl; 
//		cout << "     SetFilmModel" << endl; 
//		cout << "     SetFilmFormat" << endl; 
//		cout << "     SetFrameRate" << endl; 
//		cout << "     SetPerfPerFrame" << endl; 
//		cout << "     SetFilmAspectRatio" << endl; 
//		cout << "     GetFilmManufacturer" << endl; 
//		cout << "     GetFilmManufacturerBufLen" << endl; 
//		cout << "     GetFilmModel" << endl; 
//		cout << "     GetFilmModelBufLen" << endl; 
//		cout << "     GetFilmFormat" << endl; 
//		cout << "     GetFrameRate" << endl; 
//		cout << "     GetPerfPerFrame" << endl; 
//		cout << "     GetFilmAspectRatio" << endl; 
//		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
//	}

	return hr;
}



// @doc INTERNAL
// @com This file implements the module test for CAAFFilmDescriptor
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

#include "AAF.h"

#include <iostream.h>
#include <stdio.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"

static aafWChar* Manufacturer = L"Sony";
static aafWChar* Model = L"MyModel";
static aafFilmType_t FilmFormat = kAAFFt35MM;
static aafUInt32 FrameRate = 24;
static aafUInt8 PerfPerFrame = 4;
static aafRational_t	AspectRatio = { 1000, 1 };	// !!!Find a real aspect ratio

static const 	aafMobID_t	TEST_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x302e420e, 0x03ff, 0x11d4, 0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}};

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
	HRESULT						hr = AAFRESULT_SUCCESS;
	
	
	aafProductVersion_t v;
	v.major = 1;
	v.minor = 0;
	v.tertiary = 0;
	v.patchLevel = 0;
	v.type = kAAFVersionUnknown;
	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFFilmDescriptor Test";
	ProductInfo.productVersion = &v;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = UnitTestProductID;
	ProductInfo.platform = NULL;
	
	// Remove the previous test file if any.
	RemoveTestFile(pFileName);
	
	checkResult(AAFFileOpenNewModify(pFileName, 0, &ProductInfo, &pFile));
	checkResult(pFile->GetHeader(&pHeader));
	checkResult(pHeader->GetDictionary(&pDictionary));
	CAAFBuiltinDefs defs (pDictionary);

	// Create a film mob
	checkResult(defs.cdSourceMob()->
				CreateInstance(IID_IAAFSourceMob, 
							   (IUnknown **)&pSourceMob));
	checkResult(pSourceMob->QueryInterface(IID_IAAFMob, (void **)&pMob));
	pMob->SetMobID(TEST_MobID);
	pMob->SetName(L"FilmDescriptorTest");
	checkResult(defs.cdFilmDescriptor()->
				CreateInstance(IID_IAAFFilmDescriptor, 
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
	checkResult(pHeader->AddMob(pMob));
						
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
	aafUInt32					length;

	HRESULT						hr = AAFRESULT_SUCCESS;
	
	aafProductVersion_t v;
	v.major = 1;
	v.minor = 0;
	v.tertiary = 0;
	v.patchLevel = 0;
	v.type = kAAFVersionUnknown;
	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFFilmDescriptor Test";
	ProductInfo.productVersion = &v;
	ProductInfo.productVersionString = NULL;
	ProductInfo.platform = NULL;
	
	checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
	checkResult(pFile->GetHeader(&pHeader));
	checkResult(pHeader->CountMobs(kAAFAllMob, &numMobs));
	checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);
	checkResult(pHeader->GetMobs(NULL, &pMobIter));
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
			 << " exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
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



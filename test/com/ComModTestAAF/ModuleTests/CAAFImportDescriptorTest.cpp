// @doc INTERNAL
// @com This file implements the module test for CAAFImportDescriptor
//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
//
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include "AAF.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
using namespace std;

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"

static const 	aafMobID_t	TEST_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x37c13606, 0x0405, 0x11d4, 0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}};


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



static HRESULT OpenAAFFile(
    aafWChar*           pFileName,
    aafMediaOpenMode_t  mode,
    IAAFFile**          ppFile,
    IAAFHeader**        ppHeader)
{
    aafProductIdentification_t    ProductInfo;
    HRESULT                        hr = AAFRESULT_SUCCESS;

    aafProductVersion_t v;
    v.major = 1;
    v.minor = 0;
    v.tertiary = 0;
    v.patchLevel = 0;
    v.type = kAAFVersionUnknown;
    ProductInfo.companyName = L"AAF Developers Desk";
    ProductInfo.productName = L"AAFSoundDescriptor Test";
    ProductInfo.productVersion = &v;
    ProductInfo.productVersionString = NULL;
    ProductInfo.productID = UnitTestProductID;
    ProductInfo.platform = NULL;

    *ppFile = NULL;

    switch (mode)
    {
        case kAAFMediaOpenReadOnly:
            hr = AAFFileOpenExistingRead(pFileName, 0, ppFile);
            break;

        case kAAFMediaOpenAppend:
            hr = AAFFileOpenNewModify(pFileName, 0, &ProductInfo, ppFile);
            break;

        default:
            hr = AAFRESULT_TEST_FAILED;
            break;
    }

    if (FAILED(hr))
    {
        if (*ppFile)
        {
            (*ppFile)->Release();
            *ppFile = NULL;
        }
        return hr;
    }
  
    hr = (*ppFile)->GetHeader(ppHeader);
    if (FAILED(hr))
    {
        (*ppFile)->Release();
        *ppFile = NULL;
        return hr;
    }
     
    return hr;
}



static HRESULT CreateAAFFile(
    aafWChar * pFileName )
{
	IAAFFile *					pFile = NULL;
	IAAFHeader *				pHeader = NULL;
  IAAFDictionary*  pDictionary = NULL;
	IAAFSourceMob*				pSourceMob = NULL;
	IAAFMob*					pMob = NULL;
	IAAFEssenceDescriptor*		pEssDesc = NULL;
	IAAFImportDescriptor*			pImportDesc = NULL;

	HRESULT						hr = AAFRESULT_SUCCESS;


	// Remove the previous test file if any.
	RemoveTestFile(pFileName);

	hr = OpenAAFFile(pFileName, kAAFMediaOpenAppend, &pFile, &pHeader);

	if (AAFRESULT_SUCCESS == hr)
	{
	  hr = pHeader->GetDictionary(&pDictionary);
	  if (AAFRESULT_SUCCESS == hr)
		{
		// Create a source mob

		  CAAFBuiltinDefs defs (pDictionary);
			hr = defs.cdSourceMob()->
			  CreateInstance(IID_IAAFSourceMob, 
							 (IUnknown **)&pSourceMob);
			if (AAFRESULT_SUCCESS == hr)
			{
				hr = pSourceMob->QueryInterface(IID_IAAFMob, (void **)&pMob);
				if (AAFRESULT_SUCCESS == hr)
				{
					pMob->SetMobID(TEST_MobID);
					pMob->SetName(L"ImportDescriptorTest");
					hr = defs.cdImportDescriptor()->
					  CreateInstance(IID_IAAFImportDescriptor, 
									 (IUnknown **)&pImportDesc);		
 					if (AAFRESULT_SUCCESS == hr)
					{
						hr = pImportDesc->QueryInterface(IID_IAAFEssenceDescriptor, (void **)&pEssDesc);
						if (AAFRESULT_SUCCESS == hr)
						{
							hr = pSourceMob->SetEssenceDescriptor(pEssDesc);
							pEssDesc->Release();
							pEssDesc = NULL;
						}
						pImportDesc->Release();
						pImportDesc = NULL;
					}

					// Add the MOB to the file
					if (AAFRESULT_SUCCESS == hr)
						hr = pHeader->AddMob(pMob);

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

		pFile->Save();
		pFile->Close();

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
	IAAFImportDescriptor*		pImportDesc = NULL;
	IEnumAAFMobs*				pMobIter = NULL;

	aafNumSlots_t				numMobs;

	HRESULT						hr = AAFRESULT_SUCCESS;

	hr = AAFFileOpenExistingRead(pFileName, 0, &pFile);

	if (AAFRESULT_SUCCESS == hr)
	{
		hr = pFile->GetHeader(&pHeader);
		if (AAFRESULT_SUCCESS == hr)
		{
			hr = pHeader->CountMobs(kAAFAllMob, &numMobs);
			if (AAFRESULT_SUCCESS == hr)
			{
				if (1 == numMobs )
				{
					hr = pHeader->GetMobs(NULL, &pMobIter);
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

    pFile->Release();
		pFile = NULL;
	}

	return hr;
}

extern "C" HRESULT CAAFImportDescriptor_test(
    testMode_t mode);
extern "C" HRESULT CAAFImportDescriptor_test(
    testMode_t mode)
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	const size_t fileNameBufLen = 128;
	aafWChar* pFileName = L"AAFImportDescriptorTest.aaf";

	try
	{
		if(mode == kAAFUnitTestReadWrite)
			hr = CreateAAFFile(pFileName);
		else
			hr = AAFRESULT_SUCCESS;
		if (hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );
	}
	catch (...)
	{
		cerr << "CAAFImportDescriptor_test...Caught general C++"
			 << " exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}

	return hr;
}

// @doc INTERNAL
// @com This file implements the module test for CAAFTapeDescriptor
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
static aafTapeCaseType_t FormFactor = kAAFVHSVideoTape;
static aafVideoSignalType_t VideoSignalType = kAAFPALSignal;
static aafTapeFormatType_t TapeFormat = kAAFVHSFormat;
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
	aafMobID_t					newMobID;
	HRESULT						hr = AAFRESULT_SUCCESS;


	aafProductVersion_t v;
	v.major = 1;
	v.minor = 0;
	v.tertiary = 0;
	v.patchLevel = 0;
	v.type = kAAFVersionUnknown;
	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFTapeDescriptor Test";
	ProductInfo.productVersion = &v;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = UnitTestProductID;
	ProductInfo.platform = NULL;

	// Remove the previous test file if any.
	RemoveTestFile(pFileName);

	hr = AAFFileOpenNewModify(pFileName, 0, &ProductInfo, &pFile);

	if (AAFRESULT_SUCCESS == hr)
	{
		hr = pFile->GetHeader(&pHeader);
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
						CoCreateGuid((GUID *)&newMobID);
						pMob->SetMobID(newMobID);
						pMob->SetName(L"TapeDescriptorTest");
						hr = defs.cdTapeDescriptor()->
						  CreateInstance(IID_IAAFTapeDescriptor, 
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
		}
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
	aafUInt32					length;

	HRESULT						hr = AAFRESULT_SUCCESS;

	aafProductVersion_t v;
	v.major = 1;
	v.minor = 0;
	v.tertiary = 0;
	v.patchLevel = 0;
	v.type = kAAFVersionUnknown;
	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFTapeDescriptor Test";
	ProductInfo.productVersion = &v;
	ProductInfo.productVersionString = NULL;
	ProductInfo.platform = NULL;

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
									// if there is an Essence Descriptor then it MUST be an (essence) TapeDescriptor
									hr = pEssDesc->QueryInterface(IID_IAAFTapeDescriptor, (void **) &pTapeDesc);
									if (AAFRESULT_SUCCESS == hr)
									{
										hr = pTapeDesc->GetTapeManufacturerBufLen(&length);
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

    pFile->Release();
		pFile = NULL;
	}

	return hr;
}

extern "C" HRESULT CAAFTapeDescriptor_test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"AAFTapeDescriptorTest.aaf";

	try
	{
		hr = CreateAAFFile( pFileName );
		if (hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );
	}
	catch (...)
	{
		cerr << "CAAFTapeDescriptor_test...Caught general C++"
			" exception!" << endl; 
	}

	return hr;
}

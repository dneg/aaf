// @doc INTERNAL
// @com This file implements the module test for CAAFTIFFDescriptor
//=---------------------------------------------------------------------=
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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include "AAF.h"

#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "AAFTypes.h"
#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"

#define	TIFF_VERSION	42

#define	TIFF_BIGENDIAN		0x4d4d
#define	TIFF_LITTLEENDIAN	0x4949

typedef	struct
{
	unsigned short tiff_byteOrder;		
	unsigned short tiff_identNumber;	
	unsigned long  tiff_firstIFD;	/* byte offset to first directory */
} TIFFHeader;

typedef	struct {
	unsigned short tdir_tag;	/* see below */
	unsigned short tdir_type;	/* data type; see below */
	unsigned long  tdir_count;	/* number of items; length in spec */
	unsigned long  tdir_offset;	/* byte offset to field data */
} TIFFDirEntry;
/*
 * Tag data type information.
 *
 * Note: RATIONALs are the ratio of two 32-bit integer values.
 */
typedef	enum {
	TIFF_NOTYPE	= 0,	/* placeholder */
	TIFF_BYTE	= 1,	/* 8-bit unsigned integer */
	TIFF_ASCII	= 2,	/* 8-bit bytes w/ last byte null */
	TIFF_SHORT	= 3,	/* 16-bit unsigned integer */
	TIFF_LONG	= 4,	/* 32-bit unsigned integer */
	TIFF_RATIONAL	= 5,	/* 64-bit unsigned fraction */
	TIFF_SBYTE	= 6,	/* !8-bit signed integer */
	TIFF_UNDEFINED	= 7,	/* !8-bit untyped data */
	TIFF_SSHORT	= 8,	/* !16-bit signed integer */
	TIFF_SLONG	= 9,	/* !32-bit signed integer */
	TIFF_SRATIONAL	= 10,	/* !64-bit signed fraction */
	TIFF_FLOAT	= 11,	/* !32-bit IEEE floating point */
	TIFF_DOUBLE	= 12	/* !64-bit IEEE floating point */
} TIFFDataType;

#define	TIFFTAG_IMAGEWIDTH		256	/* image width in pixels */
#define	TIFFTAG_IMAGELENGTH		257	/* image height in pixels */

static TIFFHeader	tiffHeader;
static TIFFDirEntry tagImageWidth;
static TIFFDirEntry tagImageLength;

static const 	aafMobID_t	TEST_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0xdd1e509e, 0x0404, 0x11d4, 0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}};


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

static HRESULT OpenAAFFile(aafWChar*			pFileName,
						   aafMediaOpenMode_t	mode,
						   IAAFFile**			ppFile,
						   IAAFHeader**			ppHeader)
{
	aafProductIdentification_t	ProductInfo;
	HRESULT						hr = AAFRESULT_SUCCESS;

	aafProductVersion_t v;
	v.major = 1;
	v.minor = 0;
	v.tertiary = 0;
	v.patchLevel = 0;
	v.type = kAAFVersionUnknown;
	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFTIFFDescriptor Test";
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

static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	IAAFFile*				pFile = NULL;
	IAAFHeader*				pHeader = NULL;
	IAAFDictionary*			pDictionary = NULL;
	IAAFSourceMob*			pSourceMob = NULL;
	IAAFMob*				pMob = NULL;
	IAAFTIFFDescriptor*		pTIFFDesc = NULL;
	IAAFEssenceDescriptor*	pEssDesc = NULL;
	HRESULT					hr = AAFRESULT_SUCCESS;
	aafUInt8				summary[512];
	aafUInt16				numEntries = 2;
	unsigned long			nOffset;

#if defined( OS_WINDOWS )
	tiffHeader.tiff_byteOrder = TIFF_LITTLEENDIAN;
#else
	tiffHeader.tiff_byteOrder = TIFF_BIGENDIAN;
#endif
	tiffHeader.tiff_identNumber = TIFF_VERSION;
	tiffHeader.tiff_firstIFD = 0;
	
	memcpy(summary, (void *)&tiffHeader, sizeof(tiffHeader));
	nOffset = sizeof(tiffHeader);

	tagImageWidth.tdir_tag = TIFFTAG_IMAGEWIDTH;
	tagImageWidth.tdir_type = TIFF_LONG;
	tagImageWidth.tdir_count = 1;
	tagImageWidth.tdir_offset = 320;

	tagImageLength.tdir_tag = TIFFTAG_IMAGELENGTH;
	tagImageLength.tdir_type = TIFF_LONG;
	tagImageLength.tdir_count = 1;
	tagImageLength.tdir_offset = 640;

	// Write IFD data
	memcpy(summary+nOffset, &numEntries, sizeof(numEntries));
	nOffset += sizeof(numEntries);
	memcpy((void *)(summary+nOffset), (void *)&tagImageWidth, sizeof(tagImageWidth));
	nOffset += sizeof(tagImageWidth);
	memcpy((void *)(summary+nOffset), (void *)&tagImageLength, sizeof(tagImageLength));
	nOffset += sizeof(tagImageLength);

	try
	{
		// Remove the previous test file if any.
		RemoveTestFile(pFileName);


		// Create the AAF file
		checkResult(OpenAAFFile(pFileName, kAAFMediaOpenAppend, &pFile, &pHeader));

		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
		CAAFBuiltinDefs defs (pDictionary);
 		
		// Create a source mob
		checkResult(defs.cdSourceMob()->
					CreateInstance(IID_IAAFSourceMob, 
								   (IUnknown **)&pSourceMob));
		checkResult(pSourceMob->QueryInterface(IID_IAAFMob, (void **)&pMob));

		checkResult(pMob->SetMobID(TEST_MobID));
		checkResult(pMob->SetName(L"TIFFDescriptorTest"));
		checkResult(defs.cdTIFFDescriptor()->
					CreateInstance(IID_IAAFTIFFDescriptor, 
								   (IUnknown **)&pTIFFDesc));		
		checkResult(pTIFFDesc->QueryInterface(IID_IAAFEssenceDescriptor, (void **)&pEssDesc));
		checkResult(pTIFFDesc->SetIsUniform(kAAFFalse));
		checkResult(pTIFFDesc->SetIsContiguous(kAAFTrue));
		checkResult(pTIFFDesc->SetLeadingLines((aafInt32)10));
		checkResult(pTIFFDesc->SetTrailingLines((aafInt32)20));
		checkResult(pTIFFDesc->SetJPEGTableID((aafJPEGTableID_t)0));
		checkResult(pTIFFDesc->SetSummary((aafUInt32)nOffset, (aafDataValue_t) summary));

		checkResult(pSourceMob->SetEssenceDescriptor(pEssDesc));

		// Add the MOB to the file
		checkResult(pHeader->AddMob(pMob));
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}
	
  
	// Cleanup and return
	if (pTIFFDesc)
	   pTIFFDesc->Release();

	if (pDictionary)
	   pDictionary->Release();

	if (pEssDesc)
		pEssDesc->Release();

	if (pMob)
		pMob->Release();

	if (pSourceMob)
		pSourceMob->Release();

	if (pHeader)
		pHeader->Release();

	if (pFile)
	{
		pFile->Save();
		pFile->Close();
		pFile->Release();
	}

	return hr;
}

static HRESULT ReadAAFFile(aafWChar * pFileName)
{
	IAAFFile*				pFile = NULL;
	IAAFHeader*				pHeader = NULL;
	IEnumAAFMobs*			pMobIter = NULL;
	IAAFMob*				pMob = NULL;
	IAAFSourceMob*			pSourceMob = NULL;
	IAAFEssenceDescriptor*	pEssDesc = NULL;
	IAAFTIFFDescriptor*		pTIFFDesc = NULL;
	aafNumSlots_t			numMobs = 0;
	HRESULT					hr = AAFRESULT_SUCCESS;
	aafBool					isContiguous, isUniform;
	aafInt32				leadingLines, trailingLines;
	aafUInt32				size;
	aafUInt8				summary[512];

	try
	{
		// Open the AAF file
		checkResult(OpenAAFFile(pFileName, kAAFMediaOpenReadOnly, &pFile, &pHeader));

		checkResult(pHeader->CountMobs(kAAFAllMob, &numMobs));
		checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);

		checkResult(pHeader->GetMobs(NULL, &pMobIter));
		checkResult(pMobIter->NextOne(&pMob));
		checkResult(pMob->QueryInterface(IID_IAAFSourceMob, (void **)&pSourceMob));
		
		// Back into testing mode
		checkResult(pSourceMob->GetEssenceDescriptor(&pEssDesc));
		
		checkResult(pEssDesc->QueryInterface(IID_IAAFTIFFDescriptor, (void **)&pTIFFDesc));
		checkResult(pTIFFDesc->GetIsContiguous(&isContiguous));
		checkResult(pTIFFDesc->GetIsUniform(&isUniform));
		checkResult(pTIFFDesc->GetLeadingLines(&leadingLines));
		checkResult(pTIFFDesc->GetTrailingLines(&trailingLines));
		checkResult(pTIFFDesc->GetSummaryBufferSize(&size));
		checkExpression(size == 34, AAFRESULT_TEST_FAILED);
		checkResult(pTIFFDesc->GetSummary(size, summary));
		checkExpression(isContiguous == kAAFTrue, AAFRESULT_TEST_FAILED);
		checkExpression(isUniform == kAAFFalse, AAFRESULT_TEST_FAILED);
		checkExpression(leadingLines == 10, AAFRESULT_TEST_FAILED);
		checkExpression(trailingLines == 20, AAFRESULT_TEST_FAILED);
// The next statement is not true when doing cross-platform tests
//#if defined( OS_WINDOWS )
//		checkExpression(memcmp(summary, "II", 2) == 0, AAFRESULT_TEST_FAILED);
//#else
//		checkExpression(memcmp(summary, "MM", 2) == 0, AAFRESULT_TEST_FAILED);
//#endif

    // NOTE: The elements in the summary structure need to be byte swapped
		//       on Big Endian system (i.e. the MAC).
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}
	
  
	// Cleanup and return
	if (pEssDesc)
		pEssDesc->Release();

	if (pTIFFDesc)
		pTIFFDesc->Release();

	if (pMob)
		pMob->Release();

	if (pSourceMob)
		pSourceMob->Release();

	if (pHeader)
	    pHeader->Release();

	if (pMobIter)
	    pMobIter->Release();

	if (pFile)
	{
		pFile->Close();
		pFile->Release();
	}

	return hr;
}

extern "C" HRESULT CAAFTIFFDescriptor_test(testMode_t mode);
extern "C" HRESULT CAAFTIFFDescriptor_test(testMode_t mode)
{
	aafWChar*	pFileName = L"AAFTIFFDescriptorTest.aaf";
	HRESULT		hr = AAFRESULT_NOT_IMPLEMENTED;

	try
	{
		if(mode == kAAFUnitTestReadWrite)
			hr = CreateAAFFile(pFileName);
		else
			hr = AAFRESULT_SUCCESS;
		if (SUCCEEDED(hr))
			hr = ReadAAFFile(pFileName);
	}
	catch (...)
	{
		cerr << "CAAFTIFFDescriptor_test..."
			 << "Caught general C++ exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}

  return hr;
}

































// @doc INTERNAL
// @com This file implements the module test for CAAFMob
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
#include <wchar.h>
#include <string.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDefUIDs.h"

#include "CAAFBuiltinDefs.h"

typedef IAAFSmartPointer<IAAFMob>					IAAFMobSP;
typedef IAAFSmartPointer<IAAFFile>					IAAFFileSP;

static aafWChar *slotNames[8] = { L"SLOT1", L"SLOT2", L"SLOT3", L"SLOT4",
								  L"SLOT5", L"SLOT6", L"SLOT7", L"SLOT8" };

static aafWChar *mobName = L"MOBTest";

static aafWChar *pComment[5] = { L"TestComment1", L"TestComment2", 
								  L"TestComment3", L"TestComment4", L"TestComment5"}; 

static aafWChar *pCategory[5] = { L"TestCategory1", L"TestCategory2", 
								  L"TestCategory3", L"TestCategory4", L"TestCategory5"}; 

static const aafTimeStamp_t creationTimeStamp = { 1941, 12, 7, 5, 31, 12, 1 };
static const aafTimeStamp_t modificationTimeStamp = { 1941, 12, 7, 5, 31, 12, 2 };

// {21F2083C-B260-11d3-BFFE-00104BC9156D}
static const aafMobID_t MOBTestID = 
{ 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01,
	 0x01, 0x01, 0x04, 0x02, 0x13, 0x00, 0x00, 0x00,
	0x21f2083c, 0xb260, 0x11d3, { 0xbf, 0xfe, 0x0, 0x10, 0x4b, 0xc9, 0x15, 0x6d } };
// {81831638-EDF4-11d3-A353-009027DFCA6A}
static const aafMobID_t MOBTestID2 = 
{ 0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01,
	 0x01, 0x01, 0x04, 0x02, 0x13, 0x00, 0x00, 0x00,
	0x81831638, 0xedf4, 0x11d3, { 0xa3, 0x53, 0x0, 0x90, 0x27, 0xdf, 0xca, 0x6a } };

// {2DFB8BB2-0972-11d4-A357-009027DFCA6A}
static const aafUID_t TEST_KLV = 
{ 0x2dfb8bb2, 0x972, 0x11d4, { 0xa3, 0x57, 0x0, 0x90, 0x27, 0xdf, 0xca, 0x6a } };

// {CC77FBF0-4DFA-11d4-870E-00105A234509}
static const aafMobID_t MOBTestID3 = 
{  0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01,
	 0x01, 0x01, 0x04, 0x02, 0x13, 0x00, 0x00, 0x00,
	0xcc77fbf0, 0x4dfa, 0x11d4, { 0x87, 0xe, 0x0, 0x10, 0x5a, 0x23, 0x45, 0x9 } };

// {CC77FBF1-4DFA-11d4-870E-00105A234509}
static const aafMobID_t MOBTestID4 = 
{  0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01,
	 0x01, 0x01, 0x04, 0x02, 0x13, 0x00, 0x00, 0x00,
	0xcc77fbf1, 0x4dfa, 0x11d4, { 0x87, 0xe, 0x0, 0x10, 0x5a, 0x23, 0x45, 0x9 } };

static const char KLVsmiley[] =        /* 16x16 smiley face */
  "      ****      "
  "    ********    "
  "   **********   "
  "  ************  "
  " ***  ****  *** "
  " ***  ****  *** "
  "****************"
  "****************"
  "****************"
  "****************"
  " ** ******** ** "
  " *** ****** *** "
  "  ***  **  ***  "
  "   ****  ****   "
  "    ********    "
  "      ****     ";

static const char KLVfrowney[] =        /* 16x16 frowney face */
  "      ****      "
  "    ********    "
  "   **********   "
  "  ************  "
  " ***  ****  *** "
  " ***  ****  *** "
  "****************"
  "****************"
  "*******  *******"
  "*****  **  *****"
  " *** ****** *** "
  " ** ******** ** "
  "  ************  "
  "   **********   "
  "    ********    "
  "      ****     ";


static aafFrameOffset_t 	TCstartFrame = 108000;	// One hour
static aafDropType_t 		TCdrop = kAAFTcNonDrop;
static aafUInt16		 	TCfps = 30;


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
  bool 							bFileOpen = false;
  IAAFHeader *        			pHeader = NULL;
  IAAFDictionary*  				pDictionary = NULL;
  IAAFMob						*pMob = NULL;
  IAAFMob						*pMob2 = NULL;
  IAAFTimelineMobSlot 			*newSlot = NULL;
  IAAFSegment					*seg = NULL;
  IAAFSourceClip				*sclp = NULL;
  IAAFComponent*				pComponent = NULL;
  aafProductIdentification_t	ProductInfo;
  HRESULT						hr = S_OK;
  aafNumSlots_t					numMobs;
  aafUInt32 					bufLen = 0;
  aafUInt32 					bytesRead = 0;
  aafUInt32 					numComments = 0;
  aafUInt32						numFound = 0;
  aafWChar						name[500];
  aafWChar						value[500];
  IEnumAAFTaggedValues 			*enumTaggedVal = NULL;
  IAAFTaggedValue				*taggedVal = NULL;
  IAAFMobSlot 					*mSlot = NULL;
  IAAFFiller 					*filler = NULL;
  IAAFKLVData					*pKLVData = NULL;
  IAAFTypeDef*					pBaseType = NULL;
  IAAFSourceReference 			*pSourceRef = NULL;
  IAAFTimecode					*pTimecode = NULL;
  aafTimecode_t					timecode;
  int 							i;

  aafProductVersion_t v;
  v.major = 1;
  v.minor = 0;
  v.tertiary = 0;
  v.patchLevel = 0;
  v.type = kAAFVersionUnknown;
  ProductInfo.companyName = L"AAF Developers Desk";
  ProductInfo.productName = L"AAFMob Test";
  ProductInfo.productVersion = &v;
  ProductInfo.productVersionString = NULL;
  ProductInfo.productID = UnitTestProductID;
  ProductInfo.platform = NULL;

  try
	{
	  // Remove the previous test file if any.
	  RemoveTestFile(pFileName);

	  // Create the file.
	  checkResult(AAFFileOpenNewModify(pFileName, 0, &ProductInfo, &pFile));
	  bFileOpen = true;
 
	  // We can't really do anthing in AAF without the header.
	  checkResult(pFile->GetHeader(&pHeader));

	  // Get the AAF Dictionary so that we can create valid AAF objects.
	  checkResult(pHeader->GetDictionary(&pDictionary));
 		
	  CAAFBuiltinDefs defs (pDictionary);

	  //Make the first mob
	  long	test;

	  // Create a concrete subclass of Mob
	  checkResult(defs.cdMasterMob()->
				  CreateInstance(IID_IAAFMob, 
								 (IUnknown **)&pMob));

	  checkResult(pMob->SetMobID(MOBTestID));
      checkExpression(pMob->GetNameBufLen(&bufLen) == AAFRESULT_PROP_NOT_PRESENT, AAFRESULT_TEST_FAILED);
      checkExpression(pMob->GetName(name, 0) == AAFRESULT_PROP_NOT_PRESENT, AAFRESULT_TEST_FAILED);
	  checkExpression(pMob->SetName(NULL) == AAFRESULT_NULL_PARAM, AAFRESULT_TEST_FAILED);
	  checkResult(pMob->SetName(mobName));
	  
	  checkResult(pMob->SetCreateTime(creationTimeStamp));
	  checkResult(pMob->SetModTime(modificationTimeStamp));

	  // Add some slots
	  for(test = 1; test < 6; test++)
		{
		  checkResult(defs.cdSourceClip()->
					  CreateInstance(IID_IAAFSourceClip, 
									 (IUnknown **)&sclp));		
		  checkResult(sclp->QueryInterface(IID_IAAFSourceReference, (void **)&pSourceRef));
		  checkResult(pSourceRef->SetSourceID(MOBTestID3));

		  checkResult(sclp->QueryInterface(IID_IAAFComponent, (void **)&pComponent));
		  checkResult(pComponent->SetDataDef(defs.ddPicture()));

		  checkResult(sclp->QueryInterface (IID_IAAFSegment, (void **)&seg));

		  aafRational_t editRate = { 0, 1};
		  checkResult(pMob->AppendNewTimelineSlot (editRate,
												   seg,
												   test+1,
												   slotNames[test],
												   0,
												   &newSlot));


		  if(test == 5)
		  {
			  checkExpression(pMob->AppendNewTimelineSlot (editRate,
										   NULL,
										   test+1,
										   slotNames[test],
										   0,
										   &newSlot) == AAFRESULT_NULL_PARAM, 
										   AAFRESULT_TEST_FAILED);

			  checkExpression(pMob->AppendNewTimelineSlot (editRate,
										   seg,
										   test+1,
										   NULL,
										   0,
										   &newSlot) == AAFRESULT_NULL_PARAM, 
										   AAFRESULT_TEST_FAILED);

			  checkExpression(pMob->AppendNewTimelineSlot (editRate,
										   seg,
										   test+1,
										   slotNames[test],
										   0,
										   NULL) == AAFRESULT_NULL_PARAM, 
										   AAFRESULT_TEST_FAILED);

			  pMob->RemoveSlotAt(4);
			  checkExpression(pMob->RemoveSlotAt(test+1) == AAFRESULT_BADINDEX,
			  							   AAFRESULT_TEST_FAILED);
		  }

		  newSlot->Release();
		  newSlot = NULL;

		  seg->Release();
		  seg = NULL;

		  sclp->Release();
		  sclp = NULL;
		  
		  pComponent->Release();
		  pComponent = NULL;
		  
		  pSourceRef->Release();
		  pSourceRef = NULL;
		}

		// PrependSlot
 		checkResult(defs.cdStaticMobSlot()->
					  CreateInstance(IID_IAAFMobSlot, 
									 (IUnknown **)&mSlot));		
 		checkResult(defs.cdFiller()->
					  CreateInstance(IID_IAAFFiller, 
									 (IUnknown **)&filler));		

		checkResult(filler->Initialize(defs.ddSound(), 10));
		checkResult(filler->QueryInterface (IID_IAAFSegment, (void **)&seg));

		checkResult(mSlot->SetName(slotNames[0]));
		checkResult(mSlot->SetPhysicalNum(1));
		checkResult(mSlot->SetSlotID(1));
		checkResult(mSlot->SetSegment(seg));

		checkResult(pMob->PrependSlot(mSlot));
		checkExpression(pMob->PrependSlot(mSlot) == AAFRESULT_OBJECT_ALREADY_ATTACHED,
															AAFRESULT_TEST_FAILED);			
		checkExpression(pMob->PrependSlot(NULL) == AAFRESULT_NULL_PARAM, 
												  AAFRESULT_TEST_FAILED);
		mSlot->Release();
		mSlot = NULL;
		
		seg->Release();
		seg = NULL;
		
		filler->Release();
		filler = NULL;

		// AppendSlot
 		checkResult(defs.cdStaticMobSlot()->
					  CreateInstance(IID_IAAFMobSlot, 
									 (IUnknown **)&mSlot));		
 		checkResult(defs.cdFiller()->
					  CreateInstance(IID_IAAFFiller, 
									 (IUnknown **)&filler));		

		checkResult(filler->Initialize(defs.ddSound(), 10));
		checkResult(filler->QueryInterface (IID_IAAFSegment, (void **)&seg));

		checkResult(mSlot->SetName(slotNames[6]));
		checkResult(mSlot->SetPhysicalNum(1));
		checkResult(mSlot->SetSlotID(7));
		checkResult(mSlot->SetSegment(seg));

		checkResult(pMob->AppendSlot(mSlot));
		checkExpression(pMob->AppendSlot(mSlot) == AAFRESULT_OBJECT_ALREADY_ATTACHED,
															AAFRESULT_TEST_FAILED);			
		checkExpression(pMob->AppendSlot(NULL) == AAFRESULT_NULL_PARAM, 
												  AAFRESULT_TEST_FAILED);
		mSlot->Release();
		mSlot = NULL;
		
		seg->Release();
		seg = NULL;

		filler->Release();
		filler = NULL;

 		// InsertSlotAt -- insert a timecode object for OffsetToMobTimecode() testing
		timecode.startFrame = TCstartFrame;	// One hour
		timecode.drop = TCdrop;
		timecode.fps = TCfps;

		checkResult(defs.cdTimecode()->
					  CreateInstance(IID_IAAFTimecode, 
									 (IUnknown **)&pTimecode));
		checkResult(pTimecode->Initialize(100, &timecode));		
		checkResult(pTimecode->QueryInterface (IID_IAAFSegment, (void **)&seg));

 		checkResult(defs.cdStaticMobSlot()->
					  CreateInstance(IID_IAAFMobSlot, 
									 (IUnknown **)&mSlot));		

		checkResult(mSlot->SetName(slotNames[5]));
		checkResult(mSlot->SetPhysicalNum(1));
		checkResult(mSlot->SetSlotID(6));
		checkResult(mSlot->SetSegment(seg));

		checkExpression(pMob->InsertSlotAt(8, mSlot) == AAFRESULT_BADINDEX, 
												  AAFRESULT_TEST_FAILED);
		checkResult(pMob->InsertSlotAt(5, mSlot));

		checkExpression(pMob->InsertSlotAt(4, mSlot) == AAFRESULT_OBJECT_ALREADY_ATTACHED,
															AAFRESULT_TEST_FAILED);			
		checkExpression(pMob->InsertSlotAt(1, NULL) == AAFRESULT_NULL_PARAM, 
												  AAFRESULT_TEST_FAILED);
		mSlot->Release();
		mSlot = NULL;

		seg->Release();
		seg = NULL;
		
		pTimecode->Release();
		pTimecode = NULL;
		
		// Try CountKLVData before any have been attached
		numFound = 1;
		checkResult(pMob->CountKLVData(&numFound));
		checkExpression(numFound == 0, AAFRESULT_TEST_FAILED);
		checkExpression(pMob->CountKLVData(NULL) == AAFRESULT_NULL_PARAM,
		  												AAFRESULT_TEST_FAILED);		  
		// AppendKLVData - attach some objects
		checkResult(pDictionary->LookupTypeDef (kAAFTypeID_UInt8Array, &pBaseType));
		checkResult(pDictionary->RegisterKLVDataKey(TEST_KLV, pBaseType));
		pBaseType->Release();
		pBaseType = NULL;

		checkExpression(pMob->AppendKLVData(NULL) == AAFRESULT_NULL_PARAM,
		  												AAFRESULT_TEST_FAILED);	
 		checkResult(defs.cdKLVData()->
					  CreateInstance(IID_IAAFKLVData, 
									 (IUnknown **)&pKLVData));									 		
		checkResult(pKLVData->Initialize(TEST_KLV, sizeof(KLVfrowney), 
		  													(unsigned char *)KLVfrowney));
		checkResult(pMob->AppendKLVData(pKLVData));		  
		pKLVData->Release();
		pKLVData = NULL;
		  
 		checkResult(defs.cdKLVData()->
					  CreateInstance(IID_IAAFKLVData, 
									 (IUnknown **)&pKLVData));
		checkResult(pKLVData->Initialize(TEST_KLV, sizeof(KLVfrowney), 
		  													(unsigned char *)KLVfrowney));
		checkResult(pMob->AppendKLVData(pKLVData));		  
		checkExpression(pMob->AppendKLVData(pKLVData) == AAFRESULT_OBJECT_ALREADY_ATTACHED,
		  														AAFRESULT_TEST_FAILED);		  
		  												
		// RemoveKLVData - remove object #2
		checkExpression(pMob->RemoveKLVData(NULL) == AAFRESULT_NULL_PARAM,
		  												AAFRESULT_TEST_FAILED);		  
 		checkResult(pMob->CountKLVData(&numFound));
		checkExpression(numFound == 2, AAFRESULT_TEST_FAILED);
 		checkResult(pMob->RemoveKLVData(pKLVData));
 		checkResult(pMob->CountKLVData(&numFound));
		checkExpression(numFound == 1, AAFRESULT_TEST_FAILED);
		pKLVData->Release();
		pKLVData = NULL;
 		  
		// Try removing an object that is not attached
 		checkResult(defs.cdKLVData()->
					  CreateInstance(IID_IAAFKLVData, 
									 (IUnknown **)&pKLVData));
		checkResult(pKLVData->Initialize(TEST_KLV, sizeof(KLVsmiley), 
		  													(unsigned char *)KLVsmiley));
		checkExpression(pMob->RemoveKLVData(pKLVData) == AAFRESULT_OBJECT_NOT_ATTACHED, 
		  												AAFRESULT_TEST_FAILED);
		// Attach it to replace the one removed												
		checkResult(pMob->AppendKLVData(pKLVData));

		pKLVData->Release();
		pKLVData = NULL;

		// Comments
		checkExpression(pMob->GetComments(&enumTaggedVal) == AAFRESULT_PROP_NOT_PRESENT,
													AAFRESULT_TEST_FAILED);
		// Check CountComments()
		checkExpression(pMob->CountComments(NULL) == AAFRESULT_NULL_PARAM,
															AAFRESULT_TEST_FAILED);
		numComments = 1;
		checkResult(pMob->CountComments(&numComments));
		checkExpression(numComments == 0, AAFRESULT_TEST_FAILED);
		
		// Check AppendComments()
		checkExpression(pMob->AppendComment(NULL, pComment[0]) == AAFRESULT_NULL_PARAM,
																	AAFRESULT_TEST_FAILED);
		checkExpression(pMob->AppendComment(pCategory[0], NULL) == AAFRESULT_NULL_PARAM,
																	AAFRESULT_TEST_FAILED);
		for (i = 0; i < 5; ++i)
		{
		checkResult(pMob->AppendComment(pCategory[i], pComment[i]));
		}

		checkResult(pMob->CountComments(&numComments));
		checkExpression(numComments == 5, AAFRESULT_TEST_FAILED);

		// Check GetComments()
		checkExpression(pMob->GetComments(NULL) == AAFRESULT_NULL_PARAM,
													AAFRESULT_TEST_FAILED);
		checkResult(pMob->GetComments(&enumTaggedVal));
		for (i = 0; i < 5; ++i)
		{						
			checkResult(enumTaggedVal->NextOne(&taggedVal));
			
			checkResult(taggedVal->GetNameBufLen(&bufLen));
			checkResult(taggedVal->GetName(name, bufLen));
			checkExpression(wcscmp(name, pCategory[i]) == 0, AAFRESULT_TEST_FAILED);
			
			checkResult(taggedVal->GetValueBufLen(&bufLen));
			checkResult(taggedVal->GetValue(bufLen, (aafDataBuffer_t)value, &bytesRead));
			checkExpression(wcscmp(value, pComment[i]) == 0, AAFRESULT_TEST_FAILED);

			taggedVal->Release();
			taggedVal = NULL;
		}		

		
 		checkResult(defs.cdTaggedValue()->
					  CreateInstance(IID_IAAFTaggedValue, 
									 (IUnknown **)&taggedVal));		
						
		checkExpression(pMob->RemoveComment(taggedVal) == AAFRESULT_OBJECT_NOT_ATTACHED, 
													AAFRESULT_TEST_FAILED);
		taggedVal->Release();
		taggedVal = NULL;

		enumTaggedVal->Reset();
		enumTaggedVal->Skip(2);
		checkResult(enumTaggedVal->NextOne(&taggedVal));
		checkExpression(pMob->RemoveComment(NULL) == AAFRESULT_NULL_PARAM, 
													AAFRESULT_TEST_FAILED);
		checkResult(pMob->RemoveComment(taggedVal));
		taggedVal->Release();
		taggedVal = NULL;
			
		enumTaggedVal->Reset();
		for (i = 0; i < 5; ++i)
		{
 			if (i==2)
 				continue;
 										
			checkResult(enumTaggedVal->NextOne(&taggedVal));
			
			checkResult(taggedVal->GetNameBufLen(&bufLen));
			checkResult(taggedVal->GetName(name, bufLen));
			checkExpression(wcscmp(name, pCategory[i]) == 0, AAFRESULT_TEST_FAILED);
			
			checkResult(taggedVal->GetValueBufLen(&bufLen));
			checkResult(taggedVal->GetValue(bufLen, (aafDataBuffer_t)value, &bytesRead));
			checkExpression(wcscmp(value, pComment[i]) == 0, AAFRESULT_TEST_FAILED);

			taggedVal->Release();
			taggedVal = NULL;
		}		

		enumTaggedVal->Release();
		enumTaggedVal = NULL;
		
	  // Add the mob to the file.
	  checkResult(pHeader->AddMob(pMob));

	  // Create another Mob, check mob count, then delete and recheck count
	  checkResult(defs.cdMasterMob()->
				  CreateInstance(IID_IAAFMob, 
								 (IUnknown **)&pMob2));

	  checkResult(pMob2->SetMobID(MOBTestID2));
	  checkResult(pMob2->SetName(mobName));
	  
	  checkResult(pMob2->SetCreateTime(creationTimeStamp));
	  checkResult(pMob2->SetModTime(modificationTimeStamp));
	  // Add the mob to the file.
	  checkResult(pHeader->AddMob(pMob2));
	  checkResult(pHeader->CountMobs(kAAFAllMob, &numMobs));
	  checkExpression(numMobs == 2, AAFRESULT_TEST_FAILED);
	  checkResult(pHeader->RemoveMob(pMob2));
	  checkResult(pHeader->CountMobs(kAAFAllMob, &numMobs));
	  checkExpression(numMobs == 1, AAFRESULT_TEST_FAILED);

	  //try Copy() ... it is not implemented
	  const aafCharacter *copy_name = L"Name of Copied Mob";
	  IAAFMobSP spCopiedMob;
	  hr = pMob->Copy(copy_name, &spCopiedMob);
	  checkExpression(hr == AAFRESULT_NOT_IMPLEMENTED || hr == AAFRESULT_NOT_IN_CURRENT_VERSION, AAFRESULT_TEST_FAILED);
	  hr = AAFRESULT_SUCCESS; //move on

	  //try CloneExternal ... it is not implemented
	  IAAFMobSP spClonedMob;
	  IAAFFileSP spDestFile;
	  aafCharacter dest_filename[128];
	  wcscpy(dest_filename, pFileName);
	  wcscat(dest_filename, L"_clone");
	  // Remove the previous test file if any.
	  RemoveTestFile(dest_filename);
	  checkResult(AAFFileOpenNewModify(dest_filename, 0, &ProductInfo, &spDestFile));
	  hr = pMob->CloneExternal(kAAFNoFollowDepend, kAAFNoIncludeMedia, spDestFile, &spClonedMob);
	  checkExpression(hr == AAFRESULT_NOT_IMPLEMENTED || hr == AAFRESULT_NOT_IN_CURRENT_VERSION, AAFRESULT_TEST_FAILED);
	  spDestFile->Close();
	  hr = AAFRESULT_SUCCESS; //move on

	}
  catch (HRESULT& rResult)
	{
	  hr = rResult;
	}


  // Cleanup and return
  if (enumTaggedVal)
    enumTaggedVal->Release();

  if (newSlot)
    newSlot->Release();

  if (seg)
    seg->Release();

  if (pComponent)
    pComponent->Release();

  if (sclp)
    sclp->Release();

  if (pMob)
    pMob->Release();

  if (pMob2)
    pMob2->Release();

  if (pDictionary)
    pDictionary->Release();

  if (pHeader)
    pHeader->Release();
      
  if (pFile)
	{  // Close file
	  if (bFileOpen)
		{
		  pFile->Save();
		  pFile->Close();
		}
	  pFile->Release();
	}

  return hr;
}

static void checkTimeStampsAreEqual(aafTimeStamp_constref ts1, aafTimeStamp_constref ts2)
{
  // Check the time stamp: should be 7 December, 1941 at 05:13:12.01
  checkExpression (ts1.date.year == ts2.date.year, AAFRESULT_TEST_FAILED);
  checkExpression (ts1.date.month == ts2.date.month, AAFRESULT_TEST_FAILED);
  checkExpression (ts1.date.day == ts2.date.day, AAFRESULT_TEST_FAILED);
  checkExpression (ts1.time.hour == ts2.time.hour, AAFRESULT_TEST_FAILED);
  checkExpression (ts1.time.minute == ts2.time.minute, AAFRESULT_TEST_FAILED);
  checkExpression (ts1.time.second == ts2.time.second, AAFRESULT_TEST_FAILED);
  checkExpression (ts1.time.fraction == ts2.time.fraction, AAFRESULT_TEST_FAILED);
}

static HRESULT ReadAAFFile(aafWChar * pFileName)
{
  IAAFDictionary		*pDictionary = NULL;
  IAAFFile 				*pFile = NULL;
  bool 					bFileOpen = false;
  IAAFHeader 			*pHeader = NULL;
  IEnumAAFMobs 			*mobIter = NULL;
  IAAFMob				*aMob = NULL;
  IEnumAAFMobSlots		*slotIter = NULL;
  IAAFMobSlot			*slot = NULL;
  aafNumSlots_t			numMobs, n, s;
  HRESULT				hr = S_OK;
  aafUInt32 			bufLen = 0;
  aafUInt32 			bytesRead = 0;
  aafUInt32				numFound = 0;
  aafWChar				value[500];
  IEnumAAFTaggedValues 	*enumTaggedVal = NULL;
  IAAFTaggedValue		*taggedVal = NULL;
  aafUID_t				testKey;
  IEnumAAFKLVData		*klvEnum = NULL;
  IAAFKLVData			*pKLVData = NULL;
  IAAFTypeDef*			pBaseType = NULL;
  IAAFSourceClip		*pSourceClip = NULL;
  IAAFSourceReference	*pSourceRef = NULL;
  IAAFSegment			*pSegment = NULL;
  aafMobID_t			sourceID;
  int 					i;


  try
	{
	  // Open the file
	  checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
	  bFileOpen = true;

	  // We can't really do anthing in AAF without the header.
	  checkResult(pFile->GetHeader(&pHeader));
	  checkResult(pHeader->GetDictionary(&pDictionary));
 	  CAAFBuiltinDefs defs (pDictionary);


	  checkResult(pHeader->CountMobs(kAAFAllMob, &numMobs));
	  checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);


	  aafSearchCrit_t		criteria;
	  criteria.searchTag = kAAFNoSearch;
	  checkResult(pHeader->GetMobs (&criteria, &mobIter));

	  for(n = 0; n < numMobs; n++)
		{
		  aafWChar		name[500], slotName[500];
		  aafNumSlots_t	numSlots;
		  aafMobID_t		mobID;
		  aafSlotID_t		trackID;
		  aafUInt32 nameBufLen = 0;

		  checkResult(mobIter->NextOne (&aMob));
      
	      // Check GetNameBufLen and GetName
	      checkExpression(aMob->GetNameBufLen(NULL) == AAFRESULT_NULL_PARAM, AAFRESULT_TEST_FAILED);
	      checkResult(aMob->GetNameBufLen(&nameBufLen));
	      checkExpression(((wcslen(mobName) + 1) * sizeof(aafCharacter)) == nameBufLen, AAFRESULT_TEST_FAILED);
		  checkExpression(aMob->GetName (NULL, nameBufLen) == AAFRESULT_NULL_PARAM, AAFRESULT_TEST_FAILED);
		  checkExpression(aMob->GetName (name, 4) == AAFRESULT_SMALLBUF, AAFRESULT_TEST_FAILED);
		  checkResult(aMob->GetName (name, nameBufLen));
		  checkExpression (wcscmp(mobName, name) == 0, AAFRESULT_TEST_FAILED);

	      // Check GetMobID
		  checkExpression(aMob->GetMobID (NULL) == AAFRESULT_NULL_PARAM, AAFRESULT_TEST_FAILED);
		  checkResult(aMob->GetMobID (&mobID));
		  checkExpression(memcmp(&MOBTestID, &mobID, sizeof(mobID)) == 0, AAFRESULT_TEST_FAILED);

		  // Check the time stamps
		  aafTimeStamp_t created = { 0 };
		  checkExpression(aMob->GetCreateTime(NULL) == AAFRESULT_NULL_PARAM, AAFRESULT_TEST_FAILED);
		  checkResult(aMob->GetCreateTime(&created));
		  checkTimeStampsAreEqual(creationTimeStamp, created);
		  aafTimeStamp_t modified = { 0 };
		  checkExpression(aMob->GetModTime(NULL) == AAFRESULT_NULL_PARAM, AAFRESULT_TEST_FAILED);
		  checkResult(aMob->GetModTime(&modified));
		  checkTimeStampsAreEqual(modificationTimeStamp, modified);

	      // Check the GetMobInfo data.
	      memset(&created, 0, sizeof(created));
	      memset(&modified, 0, sizeof(modified));
	      checkExpression(aMob->GetMobInfo(NULL, &created, 
      								   name, sizeof(name)) == AAFRESULT_NULL_PARAM,
      														  AAFRESULT_TEST_FAILED);
	      checkExpression(aMob->GetMobInfo(&modified, NULL, 
      								   name, sizeof(name)) == AAFRESULT_NULL_PARAM,
      														  AAFRESULT_TEST_FAILED);
	      checkExpression(aMob->GetMobInfo(&modified, &created, 
      								   NULL, sizeof(name)) == AAFRESULT_NULL_PARAM,
      														  AAFRESULT_TEST_FAILED);
		  checkExpression(aMob->GetMobInfo(&modified, &created, 
      								   name, 1) == AAFRESULT_SMALLBUF,
      												AAFRESULT_TEST_FAILED);
	      checkResult(aMob->GetMobInfo(&modified, &created, name, sizeof(name)));
		  checkTimeStampsAreEqual(creationTimeStamp, created);
		  checkTimeStampsAreEqual(modificationTimeStamp, modified);
		  checkExpression (wcscmp(mobName, name) == 0, AAFRESULT_TEST_FAILED);

		  checkExpression(aMob->CountSlots (NULL) == AAFRESULT_NULL_PARAM, AAFRESULT_TEST_FAILED);
		  checkResult(aMob->CountSlots (&numSlots));
		  checkExpression(7 == numSlots, AAFRESULT_TEST_FAILED);

		  checkExpression(aMob->GetSlots(NULL) == AAFRESULT_NULL_PARAM, AAFRESULT_TEST_FAILED);
		  checkResult(aMob->GetSlots(&slotIter));

		  for(s = 0; s < numSlots; s++)
			{
			  checkResult(slotIter->NextOne (&slot));
			  checkResult(slot->GetNameBufLen(&bufLen));
			  checkResult(slot->GetName (slotName, bufLen));
			  checkResult(slot->GetSlotID(&trackID));
			  checkExpression (wcscmp(slotName, slotNames[s]) == 0, AAFRESULT_TEST_FAILED);
			  checkExpression(trackID == (s+1), AAFRESULT_TEST_FAILED);

			  slot->Release();
			  slot = NULL;
			  bufLen = 0;
			  trackID = 0;
			}
			slotIter->Release();
			slotIter = NULL;
			
			// GetSlotAt
			checkExpression(aMob->GetSlotAt(1, NULL) == AAFRESULT_NULL_PARAM,
														AAFRESULT_TEST_FAILED);
			checkExpression(aMob->GetSlotAt(7, &slot) == AAFRESULT_BADINDEX,
														AAFRESULT_TEST_FAILED);
			for (s = 0; s < numSlots; ++s)
			{
				checkResult(aMob->GetSlotAt(s, &slot));
				checkResult(slot->GetNameBufLen(&bufLen));
				checkResult(slot->GetName(slotName, bufLen));
				checkResult(slot->GetSlotID(&trackID));
				checkExpression(wcscmp(slotName, slotNames[s]) == 0, AAFRESULT_TEST_FAILED);
				checkExpression(trackID == (s+1), AAFRESULT_TEST_FAILED);	

				if ( 0 < s && s < 5 ) // These are the SourceClips
				{
					checkResult(slot->GetSegment(&pSegment));
			  		checkResult(pSegment->QueryInterface (IID_IAAFSourceClip, (void **)&pSourceClip));
				  	checkResult(pSourceClip->QueryInterface (IID_IAAFSourceReference, (void **)&pSourceRef));
				    checkResult(pSourceRef->GetSourceID(&sourceID));
				    checkExpression(memcmp(&sourceID, &MOBTestID3, sizeof(aafMobID_t)) == 0,
				    													 AAFRESULT_TEST_FAILED);
			  		pSourceRef->Release();
			  		pSourceRef = NULL;
			  		pSourceClip->Release();
            pSourceClip = NULL;
			  		pSegment->Release();
            pSegment = NULL;
				}
				
				slot->Release();
				slot = NULL;
				bufLen = 0;
				trackID = 0;
			}			

		  // ChangeRef
		  checkResult(aMob->ChangeRef(MOBTestID3, MOBTestID4));

		  for (s = 1; s < 5; ++s) // These are the SourceClips
		  {
				checkResult(aMob->GetSlotAt(s, &slot));
				checkResult(slot->GetSegment(&pSegment));
  		  		checkResult(pSegment->QueryInterface (IID_IAAFSourceClip, (void **)&pSourceClip));
	  	  		checkResult(pSourceClip->QueryInterface (IID_IAAFSourceReference, (void **)&pSourceRef));
	      		checkResult(pSourceRef->GetSourceID(&sourceID));
	      		checkExpression(memcmp(&sourceID, &MOBTestID4, sizeof(aafMobID_t)) == 0,
		   													 AAFRESULT_TEST_FAILED);
			  	pSourceRef->Release();
			  	pSourceRef = NULL;
			  	pSourceClip->Release();
			  	pSourceClip = NULL;
			  	pSegment->Release();
			  	pSegment = NULL;
		  		slot->Release();
			  	slot = NULL;
		  }

		  // try it again with a MobID it won't find.  Make sure nothing changes
		  checkResult(aMob->ChangeRef(MOBTestID3, MOBTestID2));

		  for (s = 1; s < 5; ++s) // These are the SourceClips
		  {
				checkResult(aMob->GetSlotAt(s, &slot));
				checkResult(slot->GetSegment(&pSegment));
  		  		checkResult(pSegment->QueryInterface (IID_IAAFSourceClip, (void **)&pSourceClip));
	  	  		checkResult(pSourceClip->QueryInterface (IID_IAAFSourceReference, (void **)&pSourceRef));
	      		checkResult(pSourceRef->GetSourceID(&sourceID));
	      		checkExpression(memcmp(&sourceID, &MOBTestID4, sizeof(aafMobID_t)) == 0,
		   													 AAFRESULT_TEST_FAILED);
			  	pSourceRef->Release();
			  	pSourceRef = NULL;
			  	pSourceClip->Release();
			  	pSourceClip = NULL;
			  	pSegment->Release();
			  	pSegment = NULL;
		  		slot->Release();
			  	slot = NULL;
		  }

		  // LookUpSlot
		  checkResult(aMob->LookupSlot(4, &slot));
		  checkResult(slot->GetSegment(&pSegment));
		  checkResult(pSegment->QueryInterface (IID_IAAFSourceClip, (void **)&pSourceClip));
		  checkResult(pSourceClip->QueryInterface (IID_IAAFSourceReference, (void **)&pSourceRef));
		  checkResult(pSourceRef->GetSourceID(&sourceID));

		  checkResult(slot->GetNameBufLen(&bufLen));
		  checkResult(slot->GetName(slotName, bufLen));
		  checkResult(slot->GetSlotID(&trackID));
		  checkExpression(wcscmp(slotName, slotNames[3]) == 0, AAFRESULT_TEST_FAILED);
		  checkExpression(trackID == (4), AAFRESULT_TEST_FAILED);	

		  pSourceRef->Release();
		  pSourceRef = NULL;		
		  pSourceClip->Release();
		  pSourceClip = NULL;		
		  pSegment->Release();
		  pSegment = NULL;		
		  slot->Release();
		  slot = NULL;		

      
      checkExpression(aMob->LookupSlot(9, &slot) == AAFRESULT_SLOT_NOT_FOUND,
													AAFRESULT_TEST_FAILED);

		  checkExpression(aMob->LookupSlot(0, NULL) == AAFRESULT_NULL_PARAM,
													AAFRESULT_TEST_FAILED);
					  												
		  // OffsetToMobTimecode
		  // 7/5/00  - this method is broken so the tests that will 
		  // make it fail have been commented out.  This module test 
		  // will still report Partial Success until implementation has been fixed
		  aafPosition_t		offset = 1;
		  aafTimecode_t		timecode;
		  // initialize timecode values
		  timecode.startFrame = 0;
		  timecode.drop = kAAFTcDrop;
		  timecode.fps = 24;

		  checkResult(aMob->LookupSlot(5, &slot));
		  checkResult(slot->GetSegment(&pSegment));

//		  checkResult(aMob->OffsetToMobTimecode(pSegment, &offset, &timecode));
//		  checkExpression(timecode.startFrame == (TCstartFrame + offset), AAFRESULT_TEST_FAILED);
//		  checkExpression(timecode.drop == TCdrop, AAFRESULT_TEST_FAILED);
//		  checkExpression(timecode.fps == TCfps, AAFRESULT_TEST_FAILED);

		  checkExpression(aMob->OffsetToMobTimecode(pSegment, NULL, &timecode) == AAFRESULT_NULL_PARAM,
															AAFRESULT_TEST_FAILED);
		  checkExpression(aMob->OffsetToMobTimecode(pSegment, &offset, NULL) == AAFRESULT_NULL_PARAM,
															AAFRESULT_TEST_FAILED);
															
		  // reinitialize values
		  timecode.startFrame = 0;
		  timecode.drop = kAAFTcDrop;
		  timecode.fps = 24;

		  // According to IDL this should search for the slot containing the timecode segment
//		  checkResult(aMob->OffsetToMobTimecode(NULL, &offset, &timecode));
//		  checkExpression(timecode.startFrame == (TCstartFrame + offset), AAFRESULT_TEST_FAILED);
//		  checkExpression(timecode.drop == TCdrop, AAFRESULT_TEST_FAILED);
//		  checkExpression(timecode.fps == TCfps, AAFRESULT_TEST_FAILED);
															
		  pSegment->Release();
		  pSegment = NULL;
		  slot->Release();
		  slot = NULL;
		  
		  // Pass in a segment that is not a timecode and make sure it returns correct hr
		  checkResult(aMob->LookupSlot(1, &slot));
		  checkResult(slot->GetSegment(&pSegment));
//		  checkExpression(aMob->OffsetToMobTimecode(pSegment, &offset, &timecode) == AAFRESULT_TIMECODE_NOT_FOUND,
//															AAFRESULT_TEST_FAILED);
		  pSegment->Release();
		  pSegment = NULL;
		  slot->Release();
		  slot = NULL;

		  // GetKLVData
		  checkResult(pDictionary->LookupTypeDef (kAAFTypeID_UInt8Array, &pBaseType));
		  checkResult(pDictionary->RegisterKLVDataKey(TEST_KLV, pBaseType));
		  pBaseType->Release();
		  pBaseType = NULL;

		  checkResult(aMob->CountKLVData(&numFound));
		  checkExpression(numFound == 2, AAFRESULT_TEST_FAILED);
		  checkResult(aMob->GetKLVData(&klvEnum));
		  
		  checkResult(klvEnum->NextOne(&pKLVData));
		  checkResult(pKLVData->GetValueBufLen(&bufLen));
		  checkExpression(sizeof(KLVfrowney) == bufLen, AAFRESULT_TEST_FAILED);
		  checkResult(pKLVData->GetValue( sizeof(value), (unsigned char *)value, &bytesRead));
		  checkExpression(memcmp(value, KLVfrowney, sizeof(KLVfrowney)) == 0, AAFRESULT_TEST_FAILED);
		  checkResult(pKLVData->GetKey(&testKey));
		  checkExpression(memcmp(&testKey, &TEST_KLV, sizeof(aafUID_t)) == 0, AAFRESULT_TEST_FAILED);
		  pKLVData->Release();
		  pKLVData = NULL;

		  checkResult(klvEnum->NextOne(&pKLVData));
		  checkResult(pKLVData->GetValueBufLen(&bufLen));
		  checkExpression(sizeof(KLVsmiley) == bufLen, AAFRESULT_TEST_FAILED);
		  checkResult(pKLVData->GetValue( sizeof(value), (unsigned char *)value, &bytesRead));
		  checkExpression(memcmp(value, KLVsmiley, sizeof(KLVsmiley)) == 0, AAFRESULT_TEST_FAILED);
		  checkResult(pKLVData->GetKey(&testKey));
		  checkExpression(memcmp(&testKey, &TEST_KLV, sizeof(aafUID_t)) == 0, AAFRESULT_TEST_FAILED);
		  pKLVData->Release();
		  pKLVData = NULL;

		  klvEnum->Release();
		  klvEnum = NULL;

		  checkExpression(aMob->GetKLVData(NULL) == AAFRESULT_NULL_PARAM,
		  												AAFRESULT_TEST_FAILED);	  


		  // Check the comments 
		  checkResult(aMob->GetComments(&enumTaggedVal));
		  for (i = 0; i < 5; ++i)
		  {
 			if (i==2)
 				continue;
 										
			checkResult(enumTaggedVal->NextOne(&taggedVal));
			
			checkResult(taggedVal->GetNameBufLen(&bufLen));
			checkResult(taggedVal->GetName(name, bufLen));
			checkExpression(wcscmp(name, pCategory[i]) == 0, AAFRESULT_TEST_FAILED);
			
			checkResult(taggedVal->GetValueBufLen(&bufLen));
			checkResult(taggedVal->GetValue(bufLen, (aafDataBuffer_t)value, &bytesRead));
			checkExpression(wcscmp(value, pComment[i]) == 0, AAFRESULT_TEST_FAILED);

			taggedVal->Release();
			taggedVal = NULL;
		  }		

		  enumTaggedVal->Release();
		  enumTaggedVal = NULL;
			
		  aMob->Release();
		  aMob = NULL;
		}
	}
  catch (HRESULT& rResult)
	{
	  hr = rResult;
	}

  // Cleanup object references
  if (slot)
    slot->Release();

  if (slotIter)
    slotIter->Release();

  if (aMob)
    aMob->Release();

  if (mobIter)
    mobIter->Release();

  if (pHeader)
    pHeader->Release();

  if (pDictionary)
    pDictionary->Release();
      
  if (pFile)
	{  // Close file
	  if (bFileOpen)
		pFile->Close();
	  pFile->Release();
	}

  return hr;
}
 

extern "C" HRESULT CAAFMob_test(testMode_t mode);
extern "C" HRESULT CAAFMob_test(testMode_t mode)
{
	HRESULT hr = AAFRESULT_SUCCESS;
 	aafWChar * pFileName = L"AAFMobTest.aaf";

	try
	{
		if(mode == kAAFUnitTestReadWrite)
		{
			hr = CreateAAFFile(pFileName);
		}
		else
			hr = AAFRESULT_SUCCESS;
		if(hr == AAFRESULT_SUCCESS)
		{
			hr = ReadAAFFile( pFileName );
		}
	}
	catch (...)
	{
	  cerr << "CAAFMob_test...Caught general C++"
		   << " exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}

  // Cleanup our object if it exists.

	// When all of the functionality of this class is tested, we can return success.
	// When a method and its unit test have been implemented, remove it from the list.
	if (SUCCEEDED(hr))
	{
		cout << "The following AAFMob methods have not been implemented:" << endl; 
		cout << "     OffsetToMobTimecode - Implementation not complete - NOT_IN_CURRENT_VERSION" << endl; 
		cout << "     CloneExternal - NOT_IN_CURRENT_VERSION" << endl; 
		cout << "     Copy - NOT_IN_CURRENT_VERSION" << endl; 
		hr = AAFRESULT_NOT_IN_CURRENT_VERSION;
	}

  return hr;
}

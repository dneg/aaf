// @doc INTERNAL
// @com This file implements the module test for CAAFObject
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
#include "AAFResult.h"
#include "AAFStoredObjectIDs.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"

#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "CAAFBuiltinDefs.h"

#include "AAFSmartPointer.h"
typedef IAAFSmartPointer<IAAFCompositionMob>	IAAFCompositionMobSP;
typedef IAAFSmartPointer<IAAFFile>				IAAFFileSP;
typedef IAAFSmartPointer<IAAFHeader>			IAAFHeaderSP;
typedef IAAFSmartPointer<IAAFIdentification>	IAAFIdentificationSP;
typedef IAAFSmartPointer<IAAFMob>				IAAFMobSP;
typedef IAAFSmartPointer<IAAFMobSlot>			IAAFMobSlotSP;
typedef IAAFSmartPointer<IAAFObject>			IAAFObjectSP;
typedef IAAFSmartPointer<IAAFPropertyDef>		IAAFPropertyDefSP;
typedef IAAFSmartPointer<IAAFPropertyValue>		IAAFPropertyValueSP;
typedef IAAFSmartPointer<IAAFSegment>			IAAFSegmentSP;
typedef IAAFSmartPointer<IAAFSequence>			IAAFSequenceSP;
typedef IAAFSmartPointer<IAAFTimelineMobSlot>	IAAFTimelineMobSlotSP;
typedef IAAFSmartPointer<IAAFTypeDefInt>		IAAFTypeDefIntSP;

static aafUID_t    fillerUID = DDEF_Timecode;
static aafLength_t  fillerLength = 3200;

// convenient error handlers.
inline void checkResult(HRESULT r)
{
  if (FAILED(r))
    throw r;
}
inline void checkExpression(bool expression, HRESULT r=AAFRESULT_TEST_FAILED)
{
  if (!expression)
    throw r;
}

static wchar_t testFileName[] = L"AAFObjectTest.aaf";

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

// AUIDs of optional properties we will create
static const aafUID_t AUID_OptionalProperty = 
{ 0xacf15840, 0x58d6, 0x11d4, { 0x92, 0x2a, 0x0, 0x50, 0x4, 0x9c, 0x3b, 0x9d } };

static const aafUID_t AUID_OptionalProperty2 = 
{ 0xecde5700, 0x58e8, 0x11d4, { 0x92, 0x2a, 0x0, 0x50, 0x4, 0x9c, 0x3b, 0x9d } };

// IDs of Mobs we will create
static const aafMobID_t	kTestMobID =
	{{0x07, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
	0x13, 0x00, 0x00, 0x00,
	{0x49394200, 0x58ed, 0x11d4,0x92, 0x2a, 0x0, 0x50, 0x4, 0x9c, 0x3b, 0x9d}};

static const aafMobID_t	kTestMobID2 =
	{{0x07, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
	0x13, 0x00, 0x00, 0x00,
	{0x4d540120, 0x5900, 0x11d4,0x92, 0x2a, 0x0, 0x50, 0x4, 0x9c, 0x3b, 0x9d}};

static HRESULT ObjectWriteTest ()
{
  HRESULT hr = AAFRESULT_TEST_FAILED;

  aafProductIdentification_t ProductInfo;
  IAAFFile* pFile = NULL;
  IAAFHeader * pHeader = NULL;
  IAAFDictionary * pDict = NULL;
  IAAFCompositionMob * pCMob = NULL;
  IAAFMob * pMob = NULL;
  IAAFObject * pObj = NULL;
  IEnumAAFProperties * pEnum = NULL;

  try
	{
	  aafProductVersion_t v;
	  v.major = 1;
	  v.minor = 0;
	  v.tertiary = 0;
	  v.patchLevel = 0;
	  v.type = kAAFVersionUnknown;
	  ProductInfo.companyName = L"AAF Developers Desk";
	  ProductInfo.productName = L"AAFObject Test";
	  ProductInfo.productVersion = &v;
	  ProductInfo.productVersionString = NULL;
	  ProductInfo.productID = UnitTestProductID;
	  ProductInfo.platform = NULL;

	  RemoveTestFile (testFileName);
	  checkResult (AAFFileOpenNewModify(testFileName,
										0,
										&ProductInfo,
										&pFile));
	  assert (pFile);
	  checkResult (pFile->GetHeader (&pHeader));
	  assert (pHeader);
	  checkResult (pHeader->GetDictionary (&pDict));
	  assert (pDict);
	  CAAFBuiltinDefs defs (pDict);

	  // Add an optional property to the class definition for AAFCompositionMob
	  // (pOptionalPropertyDef)
	  IAAFClassDefSP pClassDef;
	  checkResult(pDict->LookupClassDef(AUID_AAFCompositionMob,&pClassDef));
      IAAFPropertyDefSP pOptionalPropertyDef;
      checkResult(pClassDef->RegisterOptionalPropertyDef(AUID_OptionalProperty,
			L"New property",
			defs.tdUInt16(),
			&pOptionalPropertyDef));
      checkResult(pDict->RegisterClassDef(pClassDef));

      // Add an optional property to the class definition for AAFSequence
	  // (pOptionalPropertyDef2)
	  checkResult(pDict->LookupClassDef(AUID_AAFSequence,&pClassDef));
      IAAFPropertyDefSP pOptionalPropertyDef2;
      checkResult(pClassDef->RegisterOptionalPropertyDef(AUID_OptionalProperty2,
			L"New property",
			defs.tdUInt16(),
			&pOptionalPropertyDef2));
      checkResult(pDict->RegisterClassDef(pClassDef));

	  // Create an instance of AAFCompositionMob using the newly registered
	  // class definition.
	  checkResult(pDict->LookupClassDef(AUID_AAFCompositionMob,&pClassDef));
	  checkResult (pClassDef->CreateInstance (IID_IAAFCompositionMob,
		  (IUnknown **) &pCMob));
	  assert (pCMob);

	  checkResult (pCMob->QueryInterface (IID_IAAFMob,
										  (void **) &pMob));
	  assert (pMob);
      
	  checkResult(pMob->SetMobID(kTestMobID));
	  checkResult (pCMob->Initialize (L"TestMob"));
	  checkResult (pHeader->AddMob (pMob));

	  checkResult (pCMob->QueryInterface (IID_IAAFObject,
										  (void **) &pObj));
	  assert (pObj);

      // Before the optional property is set, calls to RemoveOptionalProperty()
	  // should return AAFRESULT_PROP_NOT_PRESENT
      checkExpression(pObj->RemoveOptionalProperty(pOptionalPropertyDef)
		  ==AAFRESULT_PROP_NOT_PRESENT);

      // Set the optional property we just created
	  IAAFTypeDefIntSP pTypeDefInt;
	  checkResult(defs.tdUInt16()->QueryInterface(IID_IAAFTypeDefInt,
		  (void**)&pTypeDefInt));
	  IAAFPropertyValueSP pOptionalPropertyValue;
	  aafUInt16 five=5;
	  checkResult(pTypeDefInt->CreateValue((aafMemPtr_t)&five,sizeof(aafUInt16),
		  &pOptionalPropertyValue));
      checkResult(pObj->SetPropertyValue(pOptionalPropertyDef,
		  pOptionalPropertyValue));

	  // Now try getting the optional property value from the object
      IAAFPropertyValueSP pReturnedPropertyValue;
	  checkResult(pObj->GetPropertyValue(pOptionalPropertyDef,
		  &pReturnedPropertyValue));

      // Perform some negative tests on RemoveOptionalProperty()

	  // RemoveOptionalProperty() with NULL property definition should return
	  // AAFRESULT_NULL_PARAM
	  checkExpression(pObj->RemoveOptionalProperty(0)==AAFRESULT_NULL_PARAM);

      // RemoveOptionalProperty() with property def. for a property that is not
	  // in the class should return AAFRESULT_BAD_PROP
      checkExpression(pObj->RemoveOptionalProperty(pOptionalPropertyDef2)
		  ==AAFRESULT_BAD_PROP);

      // Count number of properties before removing optional property value
      aafUInt32 oldNumProperties=0;
      checkResult(pObj->CountProperties(&oldNumProperties));
	  checkExpression(oldNumProperties>=1);

	  // Remove the optional property value
	  checkResult(pObj->RemoveOptionalProperty(pOptionalPropertyDef));

      // Verify that property count is reduced by 1.
	  aafUInt32 newNumProperties=(aafUInt32)(-1);
	  checkResult(pObj->CountProperties(&newNumProperties));
	  checkExpression(newNumProperties==oldNumProperties-1);

	  // Now we should not be able to get the optional property value from the
	  // object
	  checkExpression(pObj->GetPropertyValue(pOptionalPropertyDef,
		  &pReturnedPropertyValue)==AAFRESULT_PROP_NOT_PRESENT);

	  checkResult (pObj->GetProperties (&pEnum));
	  checkExpression (pEnum != 0, AAFRESULT_TEST_FAILED);
	  while (newNumProperties--)
	  {
		  IAAFProperty * pProp = NULL;
		  checkResult (pEnum->NextOne (&pProp));
		  checkExpression (pProp != 0, AAFRESULT_TEST_FAILED);
		  pProp->Release();
	  }

      // Now use CreateOptionalPropertyValue() to add an optional property to
	  // an instance of AAFSequence

	  // Create an instance of AAFSequence & initialize it
	  checkResult(pDict->LookupClassDef(AUID_AAFSequence,&pClassDef));
	  IAAFSequenceSP pSequence;
	  checkResult (pClassDef->CreateInstance (IID_IAAFSequence,
		  (IUnknown **)&pSequence));
	  checkResult(pSequence->Initialize(defs.ddSound()));

	  // Add sequence to composition Mob
      IAAFSegmentSP pSegment;
      checkResult(pSequence->QueryInterface(IID_IAAFSegment,(void **)&pSegment));
      aafRational_t editRate = { 0, 1};
      IAAFTimelineMobSlotSP pTimelineMobSlot;
      checkResult(pMob->AppendNewTimelineSlot(editRate,pSegment,1,
        L"AAF Test Sequence",0,&pTimelineMobSlot));

  	  IAAFObjectSP pObj2;
	  checkResult(pSequence->QueryInterface(IID_IAAFObject,(void**)&pObj2));

      // Test CreateOptionalPropertyValue()

      // First perform some negative tests on CreateOptionalPropertyValue()
	  IAAFPropertyValueSP pCreatedPropVal;
	  checkExpression(pObj2->CreateOptionalPropertyValue(0,&pCreatedPropVal)
		  ==AAFRESULT_NULL_PARAM);
	  checkExpression(pObj2->CreateOptionalPropertyValue(pOptionalPropertyDef2,0)
		  ==AAFRESULT_NULL_PARAM);
	  checkExpression(pObj2->CreateOptionalPropertyValue(pOptionalPropertyDef,
		  &pCreatedPropVal)==AAFRESULT_BAD_PROP);
	  checkExpression(pCreatedPropVal==0);
      
      // Now use CreateOptionalPropertyValue() to create a property value and
	  // set it as an optional property of an instance of AAFSequence.
	  checkResult(pObj2->CreateOptionalPropertyValue(pOptionalPropertyDef2,
		  &pCreatedPropVal));

	  IAAFTypeDefSP pTypeDef;
	  checkResult(pCreatedPropVal->GetType(&pTypeDef));
	  checkResult(pTypeDef->QueryInterface(IID_IAAFTypeDefInt,
		  (void**)&pTypeDefInt));
	  aafUInt16 ten=10;
	  checkResult(pTypeDefInt->SetInteger(pCreatedPropVal,(aafMemPtr_t)&ten,
		  sizeof(aafUInt16)));

      checkResult(pObj2->SetPropertyValue(pOptionalPropertyDef2,
		  pCreatedPropVal));

	  //
	  // Check Generation functionality.
	  //

	  // Test IsGenerationTracked(), DisableGenerationTracking()
	  aafBoolean_t b;
	  checkResult (pObj->IsGenerationTracked (&b));
	  // Generation is not tracked by default.
	  checkExpression (b == kAAFFalse, AAFRESULT_TEST_FAILED);

	  // check for redundant disables
	  checkResult (pObj->DisableGenerationTracking());
	  checkResult (pObj->IsGenerationTracked (&b));
	  checkExpression (b == kAAFFalse, AAFRESULT_TEST_FAILED);

	  // Change the object (using the CompMob interface) so that
	  // subsequent Save() will know it's a dirty object.
	  aafRational_t eu = { 1, 1 };
	  checkResult (pCMob->SetDefaultFade (0, kAAFFadeNone, eu));

	  // Perform one save and verify that generation was not tracked.
	  checkResult (pFile->Save());

	  AAFRESULT testhr;
	  IAAFIdentificationSP pIdent;
	  aafUID_t identAuid;
	  testhr = pObj->GetGeneration(&pIdent);
	  checkExpression (AAFRESULT_INVALID_PARAM == testhr,
					   AAFRESULT_TEST_FAILED);
	  testhr = pObj->GetGenerationAUID(&identAuid);
	  checkExpression (AAFRESULT_INVALID_PARAM == testhr,
					   AAFRESULT_TEST_FAILED);
	  // make sure gen still isn't tracked
	  checkResult (pObj->IsGenerationTracked (&b));
	  checkExpression (b == kAAFFalse, AAFRESULT_TEST_FAILED);

	  // Test EnableGenerationTracking()
	  checkResult (pObj->EnableGenerationTracking());
	  checkResult (pObj->IsGenerationTracked (&b));
	  checkExpression (b == kAAFTrue, AAFRESULT_TEST_FAILED);

	  checkResult (pObj->DisableGenerationTracking());
	  checkResult (pObj->IsGenerationTracked (&b));
	  checkExpression (b == kAAFFalse, AAFRESULT_TEST_FAILED);

	  // Check for redundant enables
	  checkResult (pObj->EnableGenerationTracking());
	  checkResult (pObj->EnableGenerationTracking());
	  checkResult (pObj->IsGenerationTracked (&b));
	  checkExpression (b == kAAFTrue, AAFRESULT_TEST_FAILED);

	  // Now that generation tracking is enabled, try a couple of
	  // save()s to see if the latest ident is saved.
	  //
	  // First, add a new identification.  We'll keep track of the
	  // identifications we're putting in by using a bogus productID
	  // AUID, and put small integers in the aafUID_t.Data1 field.
	  //
	  // Here's where we define two version AUIDs for our use.
	  aafUID_t versionUid1 = { 0 };
	  versionUid1.Data1 = 1;
	  aafUID_t versionUid2 = { 0 };
	  versionUid2.Data1 = 2;

	  // Now make a new identification with the first version AUID,
	  // and append it to the header.
	  IAAFIdentificationSP pNewIdent;
	  checkResult (defs.cdIdentification()->
				   CreateInstance (IID_IAAFIdentification,
								   (IUnknown **) &pNewIdent));
	  assert (pNewIdent);
	  checkResult (pNewIdent->Initialize (L"Avid",
										  L"Test File",
										  L"First Test Version",
										  versionUid1));
	  checkResult (pHeader->AppendIdentification (pNewIdent));

	  // With Version1 identification in the header, do a save and see
	  // if that generation made it to our file.
	  //
	  // First, change object to make sure Save() will save it
	  checkResult (pCMob->SetDefaultFade (1, kAAFFadeNone, eu));

	  checkResult (pFile->Save());
	  IAAFIdentificationSP pReadIdent1;
	  checkResult (pObj->GetGeneration(&pReadIdent1));
	  aafUID_t readAuid1 = { 0 };
	  checkResult (pReadIdent1->GetProductID(&readAuid1));
	  checkExpression (1 == readAuid1.Data1, AAFRESULT_TEST_FAILED);

	  // Now make a new identification with the second version AUID,
	  // and append it to the header.
	  checkResult (defs.cdIdentification()->
				   CreateInstance (IID_IAAFIdentification,
								   (IUnknown **) &pNewIdent));
	  assert (pNewIdent);
	  aafUID_t junkAuid;
	  // make sure this isn't allowed before initialization
	  HRESULT temphr = pNewIdent->GetProductID(&junkAuid);
	  checkExpression (AAFRESULT_NOT_INITIALIZED == temphr,
					   AAFRESULT_TEST_FAILED);

	  checkResult (pNewIdent->Initialize (L"Avid",
										  L"Test File",
										  L"Second Test Version",
										  versionUid2));

	  // this should now work after initialization
	  checkResult (pNewIdent->GetProductID(&junkAuid));

	  checkResult (pHeader->AppendIdentification (pNewIdent));

	  // With Version2 identification in the header, do a save and see
	  // if that generation made it to our file.
	  //
	  // First, change object to make sure Save() will save it
	  checkResult (pCMob->SetDefaultFade (2, kAAFFadeNone, eu));
	  checkResult (pFile->Save());
	  IAAFIdentificationSP pReadIdent2;
	  checkResult (pObj->GetGeneration(&pReadIdent2));
	  aafUID_t readAuid2 = { 0 };
	  checkResult (pReadIdent2->GetProductID(&readAuid2));
	  checkExpression (2 == readAuid2.Data1, AAFRESULT_TEST_FAILED);

	  hr = AAFRESULT_SUCCESS;
	}
  catch (HRESULT & rResult)
	{
	  hr = rResult;
	}

  if (pEnum) pEnum->Release();
  if (pCMob) pCMob->Release();
  if (pMob) pCMob->Release();
  if (pObj) pObj->Release();
  if (pDict) pDict->Release();
  if (pHeader) pHeader->Release();
  if (pFile)
	{
	  AAFRESULT temphr;
	  temphr = pFile->Save();
	  if (! SUCCEEDED (temphr)) return temphr;
	  temphr = pFile->Close();
	  if (! SUCCEEDED (temphr)) return temphr;
	  pFile->Release();
	}
  
  return hr;
}


static HRESULT ObjectReadTest ()
{
  HRESULT hr = AAFRESULT_TEST_FAILED;
	IAAFFile * pFile = NULL;

  try
	{
	  checkResult (AAFFileOpenExistingRead(testFileName,
										   0,
										   &pFile));
	  assert (pFile);
	  IAAFHeaderSP pHeader;
	  checkResult (pFile->GetHeader (&pHeader));
	  assert (pHeader);

      IAAFDictionarySP pDict;
	  checkResult(pHeader->GetDictionary(&pDict));

	  IAAFIdentificationSP pReadIdent;
	  checkResult (pHeader->GetLastIdentification(&pReadIdent));

	  // ident should be initialized, so this should work 
	  aafUID_t junkAuid;
	  checkResult (pReadIdent->GetProductID(&junkAuid));

      // Look up the (extended) class definition for AAFCompositionMob, and get
	  // the definition for the optional property we added.
	  IAAFClassDefSP pClassDef;
	  checkResult(pDict->LookupClassDef(AUID_AAFCompositionMob,&pClassDef));
	  IAAFPropertyDefSP pOptionalPropertyDef;
	  checkResult(pClassDef->LookupPropertyDef(AUID_OptionalProperty,
		  &pOptionalPropertyDef));

      // Also look up the optional property we added to AAFSequence
	  checkResult(pDict->LookupClassDef(AUID_AAFSequence,&pClassDef));
	  IAAFPropertyDefSP pOptionalPropertyDef2;
	  checkResult(pClassDef->LookupPropertyDef(AUID_OptionalProperty2,
		  &pOptionalPropertyDef2));

	  // Look up the Mob we created
	  IAAFMobSP pMob;
	  checkResult(pHeader->LookupMob(kTestMobID,&pMob));

	  // Verify that this is a composition Mob
	  IAAFCompositionMobSP pCompositionMob;
	  checkResult(pMob->QueryInterface(IID_IAAFCompositionMob,
		  (void**)&pCompositionMob));

      IAAFObjectSP pObject;
	  checkResult(pMob->QueryInterface(IID_IAAFObject,(void**)&pObject));

	  // Property should *not* be present in the object, since we removed it via
	  // RemoveOptionalProperty()
	  IAAFPropertyValueSP pOptionalPropertyValue;
	  checkExpression(pObject->GetPropertyValue(pOptionalPropertyDef,
		  &pOptionalPropertyValue)==AAFRESULT_PROP_NOT_PRESENT);

	  // RemoveOptionalProperty() should fail since the property is not present
	  checkExpression(pObject->RemoveOptionalProperty(pOptionalPropertyDef)
		  ==AAFRESULT_PROP_NOT_PRESENT);

      // Look up the sequence we created & get the property value we added
      IAAFMobSlotSP pSlot;
      checkResult(pMob->GetSlotAt(0,&pSlot));
      IAAFObjectSP pObj2;
	  IAAFSegmentSP pSegment;
	  checkResult(pSlot->GetSegment(&pSegment));
      checkResult(pSegment->QueryInterface(IID_IAAFObject,(void**)&pObj2));
      checkResult(pObj2->GetPropertyValue(pOptionalPropertyDef2,
		  &pOptionalPropertyValue));

      // Verify optional property value created via CreateOptionalPropertyValue()
	  IAAFTypeDefSP pTypeDef;
	  checkResult(pOptionalPropertyValue->GetType(&pTypeDef));
	  IAAFTypeDefIntSP pTypeDefInt;
	  checkResult(pTypeDef->QueryInterface(IID_IAAFTypeDefInt,
		  (void**)&pTypeDefInt));
	  aafUInt16 storedValue;
	  checkResult(pTypeDefInt->GetInteger(pOptionalPropertyValue,
		  (aafMemPtr_t)&storedValue,sizeof(aafUInt16)));
      checkExpression(storedValue==10);

	  hr = AAFRESULT_SUCCESS;
	}

  catch (HRESULT & rResult)
	{
	  hr = rResult;
	}

  if (pFile)
  {
	  AAFRESULT temphr;
	  temphr = pFile->Close();
	  if (! SUCCEEDED (temphr)) return temphr;
    pFile->Release();
    pFile = NULL;
  }

  return hr;
}


extern "C" HRESULT CAAFObject_test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;

  try
	{
	  hr = ObjectWriteTest ();
	  if (FAILED(hr))
		{
		  cerr << "CAAFObject_test...FAILED!" << endl;
		  return hr;
		}

	  hr = ObjectReadTest ();
	  if (FAILED(hr))
		{
		  cerr << "CAAFObject_test...FAILED!" << endl;
		  return hr;
		}
	}
  catch (...)
	{
	  cerr << "CAAFObject_test...Caught general C++"
		   << " exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}

  return hr;
}

// @doc INTERNAL
// @com This file implements the module test for CAAFTypeDefRecord
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

#include <iostream>
using namespace std;
#include <stdio.h>
#include <stddef.h>
#include <assert.h>
#include <stdlib.h>
#include <wchar.h>

#include "AAF.h"
#include "AAFResult.h"
#include "AAFFileMode.h"
#include "ModuleTest.h"
#include "AAFDefUIDs.h"
#include "AAFClassDefUIDs.h"

#include "CAAFBuiltinDefs.h"
#include "AAFSmartPointer.h"
typedef IAAFSmartPointer<IAAFCompositionMob> IAAFCompositionMobSP;
typedef IAAFSmartPointer<IAAFDictionary>     IAAFDictionarySP;
typedef IAAFSmartPointer<IAAFFile>           IAAFFileSP;
typedef IAAFSmartPointer<IAAFHeader>         IAAFHeaderSP;
typedef IAAFSmartPointer<IAAFMob>            IAAFMobSP;
typedef IAAFSmartPointer<IAAFObject>         IAAFObjectSP;
typedef IAAFSmartPointer<IAAFPropertyDef>    IAAFPropertyDefSP;
typedef IAAFSmartPointer<IAAFPropertyValue>  IAAFPropertyValueSP;
typedef IAAFSmartPointer<IAAFTypeDef>        IAAFTypeDefSP;
typedef IAAFSmartPointer<IAAFTypeDefInt>     IAAFTypeDefIntSP;
typedef IAAFSmartPointer<IAAFTypeDefRecord>  IAAFTypeDefRecordSP;
typedef IAAFSmartPointer<IEnumAAFMobs>       IEnumAAFMobsSP;

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


#define COMPANY_NAME		L"AAF Developers Desk"
#define PRODUCT_NAME		L"AAFTypeDefRecord Test"
#define TEST_VERSION		L"TEST VERSION"

static aafProductVersion_t testVersion =
{ 1, 0, 0, 0, kAAFVersionUnknown };

//
// Type IDs
//
//
// {5BC8A140-2BE4-11d4-B80D-0000863F2C27}
static const aafUID_t sTypeId_Rational8 = 
{ 0x5bc8a140, 0x2be4, 0x11d4,
  { 0xb8, 0xd, 0x0, 0x0, 0x86, 0x3f, 0x2c, 0x27 }
};
//
// {5BC8A141-2BE4-11d4-B80D-0000863F2C27}
static const aafUID_t sTypeId_Rational8_pair = 
{ 0x5bc8a141, 0x2be4, 0x11d4,
  { 0xb8, 0xd, 0x0, 0x0, 0x86, 0x3f, 0x2c, 0x27 }
};

//
// Property IDs
//
// {5311FC01-2D78-11d4-B80D-0000863F2C27}
static const aafUID_t sPropertyId_positionA = 
{ 0x5311fc01, 0x2d78, 0x11d4,
  { 0xb8, 0xd, 0x0, 0x0, 0x86, 0x3f, 0x2c, 0x27 }
};
//
// {5FF22A60-36DF-11d4-B80D-0000863F2C27}
static const aafUID_t sPropertyId_positionB = 
{ 0x5ff22a60, 0x36df, 0x11d4,
  { 0xb8, 0xd, 0x0, 0x0, 0x86, 0x3f, 0x2c, 0x27 }
};
//
// {11DDAB4A-371D-11d4-935E-006094EB75CB}
static const aafUID_t sPropertyId_positionC = 
{ 0x11ddab4a, 0x371d, 0x11d4,
  { 0x93, 0x5e, 0x0, 0x60, 0x94, 0xeb, 0x75, 0xcb }
};


struct rational8_t
{
  aafInt8  Numerator;
  aafUInt8 Denominator;
};

struct rational8pair_t
{
  rational8_t X_Position;
  rational8_t Y_Position;
};


static HRESULT RegisterRational8Offsets (IAAFTypeDefRecord * ptd)
{
  assert (ptd);
  aafUInt32 offsets[] =
  {
	offsetof (rational8_t, Numerator),
	offsetof (rational8_t, Denominator)
  };
  return ptd->RegisterMembers (offsets,
							   2,
							   sizeof (rational8_t));
}


static HRESULT RegisterRational8PairOffsets (IAAFTypeDefRecord * ptd)
{
  assert (ptd);
  aafUInt32 offsets[] =
  {
	offsetof (rational8pair_t, X_Position),
	offsetof (rational8pair_t, Y_Position)
  };
  return ptd->RegisterMembers (offsets,
							   2,
							   sizeof (rational8pair_t));
}


static HRESULT WriteRecord (const aafWChar * pFileName)
{
  HRESULT hr = E_FAIL;
  aafProductIdentification_t ProductInfo;
  ProductInfo.companyName = COMPANY_NAME;
  ProductInfo.productName = PRODUCT_NAME;
  ProductInfo.productVersionString = TEST_VERSION;
  ProductInfo.productID = UnitTestProductID;
  ProductInfo.productVersion = &testVersion;
  IAAFFileSP   pFile;

  try 
	{
	  HRESULT temphr;

	  // Remove the previous test file if any.
	  RemoveTestFile(pFileName);

	  // Create the file and get the new file's header.
	  checkResult(AAFFileOpenNewModify(pFileName, 0, &ProductInfo, &pFile));
	  IAAFHeaderSP pHeader;
  	  checkResult(pFile->GetHeader(&pHeader));
	  IAAFDictionarySP pDict;
	  checkResult (pHeader->GetDictionary(&pDict));
	  CAAFBuiltinDefs defs(pDict);


	  // Create, initialize, and register the Rational8 type, to
	  // consist of an Int8 numerator and a UInt8 denominator.
	  IAAFTypeDef* tdr8MemberTypes[] =
	  {defs.tdInt8(),
	   defs.tdUInt8()};
	  aafString_t tdr8MemberNames[] =
	  {L"Numerator",
	   L"Denominator"};
	  IAAFTypeDefRecordSP ptdr8;
	  checkResult (pDict->
				   CreateMetaInstance (kAAFClassID_TypeDefRecord,
									   IID_IAAFTypeDefRecord,
									   (IUnknown**) &ptdr8));

	  checkResult (ptdr8->Initialize (sTypeId_Rational8,
									  tdr8MemberTypes,
									  tdr8MemberNames,
									  2,
									  L"Rational8"));

	  // check for duplicate initialization
	  temphr = ptdr8->Initialize (sTypeId_Rational8,
								  tdr8MemberTypes,
								  tdr8MemberNames,
								  2,
								  L"Rational8");
	  checkExpression (AAFRESULT_ALREADY_INITIALIZED == temphr,
					   AAFRESULT_TEST_FAILED);

	  IAAFTypeDefSP ptd;
	  checkResult (ptdr8->QueryInterface (IID_IAAFTypeDef, (void**) &ptd));
	  checkResult (pDict->RegisterTypeDef (ptd));

	  // Create, initialize, and register the Rational8Position type,
	  // to consist of a Rational8 X and Rational8 Y coordinates.

	  IAAFTypeDefRecordSP ptdr8p;
	  //checkResult (defs.cdTypeDefRecord()->
	  // CreateInstance (IID_IAAFTypeDefRecord,
	  // (IUnknown**)&ptdr8p));
	  checkResult (pDict->
				   CreateMetaInstance (kAAFClassID_TypeDefRecord,
									   IID_IAAFTypeDefRecord,
									   (IUnknown**) &ptdr8p));

	  IAAFTypeDef * pTempTd = 0;
	  checkResult (ptdr8->QueryInterface (IID_IAAFTypeDef,
										  (void**) &pTempTd));	  
	  IAAFTypeDef* tdr8pMemberTypes[] =
	  {pTempTd,
	   pTempTd};
	  aafString_t tdr8pMemberNames[] =
	  {L"X Position",
	   L"Y Position"};
	  checkResult (ptdr8p->Initialize (sTypeId_Rational8_pair,
									   tdr8pMemberTypes,
									   tdr8pMemberNames,
									   2,
									   L"Rational8Pair"));
	  // check for duplicate initialization
	  temphr = ptdr8p->Initialize (sTypeId_Rational8,
								   tdr8pMemberTypes,
								   tdr8pMemberNames,
								   2,
								   L"Rational8Pair");
	  checkExpression (AAFRESULT_ALREADY_INITIALIZED == temphr,
					   AAFRESULT_TEST_FAILED);

	  pTempTd->Release ();
	  pTempTd = 0;
	  checkResult (ptdr8p->QueryInterface (IID_IAAFTypeDef, (void**) &ptd));
	  checkResult (pDict->RegisterTypeDef (ptd));

	  // Now attempt to register offsets of rational8_pair.  This
	  // should fail, because offsets of its members (of type
	  // rational8) have not yet been registered.
	  temphr = RegisterRational8PairOffsets (ptdr8p);
	  checkExpression (temphr == AAFRESULT_NOT_REGISTERED,
					   AAFRESULT_TEST_FAILED);

	  // Now let's attempt registration the right way.
	  checkResult (RegisterRational8Offsets (ptdr8));
	  checkResult (RegisterRational8PairOffsets (ptdr8p));

	  // Create a new property on Composition mob (called Position)
	  // whose type is rational8_pair.
	  checkResult (ptdr8p->QueryInterface (IID_IAAFTypeDef,
										   (void**) &ptd));
	  IAAFPropertyDefSP pPropDefPosA;
	  IAAFPropertyDefSP pPropDefPosB;
	  IAAFPropertyDefSP pPropDefPosC;
	  checkResult (defs.cdCompositionMob()->RegisterOptionalPropertyDef
				   (sPropertyId_positionA,
					L"PositionA",
					ptd,
					&pPropDefPosA));
	  checkResult (defs.cdCompositionMob()->RegisterOptionalPropertyDef
				   (sPropertyId_positionB,
					L"PositionB",
					ptd,
					&pPropDefPosB));
	  checkResult (defs.cdCompositionMob()->RegisterOptionalPropertyDef
				   (sPropertyId_positionC,
					L"PositionC",
					ptd,
					&pPropDefPosC));

	  // Create one of our new CompositionMobs, and add a values for
	  // the Position property.
	  IAAFCompositionMobSP pcm;
	  checkResult (defs.cdCompositionMob()->
				   CreateInstance (IID_IAAFCompositionMob,
								   (IUnknown**)&pcm));

	  // Add property value for PositionA using CreateValueFromValues
	  // methods.  The constituent property values will be created
	  // from CreateValueFromStruct, and from SetValueFromStruct.

	  const rational8_t v_12 = {1, 2};
	  const rational8_t v_34 = {3, 4};
	  IAAFPropertyValue * pValRat[2] = { 0 };

	  checkResult (ptdr8->
				   CreateValueFromStruct ((aafMemPtr_t) &v_12,
										  sizeof (v_12),
										  &pValRat[0]));
	  // create it with wrong data, and use SetStruct to give it right
	  // data
	  checkResult (ptdr8->
				   CreateValueFromStruct ((aafMemPtr_t) &v_12,
										  sizeof (v_12),
										  &pValRat[1]));
	  checkResult (ptdr8->
				   SetStruct (pValRat[1],
							  (aafMemPtr_t) &v_34,
							  sizeof (v_34)));

	  // Create the rational8_pair property value from the
	  // rational8 property values.
	  IAAFPropertyValueSP pRat8PairVal_1234;
	  temphr = ptdr8p->CreateValueFromValues (0,
											  2,
											  &pRat8PairVal_1234);
	  checkExpression (AAFRESULT_NULL_PARAM == temphr,
					   AAFRESULT_TEST_FAILED);
	  temphr = ptdr8p->CreateValueFromValues (pValRat,
											  1,
											  &pRat8PairVal_1234);
	  checkExpression (AAFRESULT_ILLEGAL_VALUE == temphr,
					   AAFRESULT_TEST_FAILED);
	  checkResult (ptdr8p->CreateValueFromValues (pValRat,
												  2,
												  &pRat8PairVal_1234));
	  pValRat[0]->Release ();
	  pValRat[0] = 0;
	  pValRat[1]->Release ();
	  pValRat[1] = 0;

	  IAAFObjectSP pObj;
	  checkResult (pcm->QueryInterface (IID_IAAFObject,
										(void**) &pObj));
	  checkResult (pObj->SetPropertyValue (pPropDefPosA,
										   pRat8PairVal_1234));


	  // Add property value for PositionB using CreateValueFromStruct
	  // methods.
	  const rational8_t r8_56 = {5, 6};
	  const rational8_t r8_78 = {7, 8};
	  rational8pair_t r8p_5678;
	  r8p_5678.X_Position = r8_56;
	  r8p_5678.Y_Position = r8_78;
	  IAAFPropertyValueSP pRat8PairVal_5678;
	  temphr = ptdr8p->
		CreateValueFromStruct (0,
							   sizeof (r8p_5678),
							   &pRat8PairVal_5678);
	  checkExpression (AAFRESULT_NULL_PARAM == temphr,
					   AAFRESULT_TEST_FAILED);
	  temphr = ptdr8p->
		CreateValueFromStruct ((aafMemPtr_t) &r8p_5678,
							   sizeof (r8p_5678),
							   0);
	  checkExpression (AAFRESULT_NULL_PARAM == temphr,
					   AAFRESULT_TEST_FAILED);
	  temphr = ptdr8p->
		CreateValueFromStruct ((aafMemPtr_t) &r8p_5678,
							   sizeof (r8p_5678)+1,
							   &pRat8PairVal_5678);
	  checkExpression (AAFRESULT_ILLEGAL_VALUE == temphr,
					   AAFRESULT_TEST_FAILED);
	  checkResult (ptdr8p->
				   CreateValueFromStruct ((aafMemPtr_t) &r8p_5678,
										  sizeof (r8p_5678),
										  &pRat8PairVal_5678));
	  checkResult (pObj->SetPropertyValue (pPropDefPosB,
										   pRat8PairVal_5678));

	  // Add property value for PositionC using CreateValueFromStruct
	  // methods, and then use SetStruct to set the value.
	  const rational8_t r8_9a = {9, 10};
	  const rational8_t r8_bc = {11, 12};
	  rational8pair_t r8p_9abc;
	  r8p_9abc.X_Position = r8_9a;
	  r8p_9abc.Y_Position = r8_bc;
	  IAAFPropertyValueSP pRat8PairVal_9abc;
	  checkResult (ptdr8p->
				   CreateValueFromStruct ((aafMemPtr_t) &r8p_5678,
										  sizeof (r8p_5678),
										  &pRat8PairVal_9abc));
	  temphr = ptdr8p->SetStruct (0,
								  (aafMemPtr_t) &r8p_9abc,
								  sizeof (r8p_9abc));
	  checkExpression (AAFRESULT_NULL_PARAM == temphr,
					   AAFRESULT_TEST_FAILED);
	  temphr = ptdr8p->SetStruct (pRat8PairVal_9abc,
								  0,
								  sizeof (r8p_9abc));
	  checkExpression (AAFRESULT_NULL_PARAM == temphr,
					   AAFRESULT_TEST_FAILED);
	  temphr = ptdr8p->SetStruct (pRat8PairVal_9abc,
								  (aafMemPtr_t) &r8p_9abc,
								  sizeof (r8p_9abc)-1);
	  checkExpression (AAFRESULT_ILLEGAL_VALUE == temphr,
					   AAFRESULT_TEST_FAILED);
	  checkResult (ptdr8p->SetStruct (pRat8PairVal_9abc,
									  (aafMemPtr_t) &r8p_9abc,
										  sizeof (r8p_9abc)));
	  checkResult (pObj->SetPropertyValue (pPropDefPosC,
										   pRat8PairVal_9abc));


	  //Put the modified comp mob into the file.
	  IAAFMobSP pMob;
	  checkResult (pcm->QueryInterface (IID_IAAFMob,
										(void**) &pMob));
	  checkResult (pHeader->AddMob(pMob));

	  // Attempt to save the file.
	  checkResult(pFile->Save());
		
	  // Attempt to close the file.
	  checkResult(pFile->Close());
	  IAAFFileSP nullFile;
	  pFile = nullFile;  // zeros the pFile, and releases it.

	  hr = AAFRESULT_SUCCESS;
	}
  catch (HRESULT& rResult)
	{
	  hr = rResult;
	}
	
  // cleanup
  if (pFile)
	{
	  pFile->Save();
	  pFile->Close();
	}
	
  return hr;
}


static HRESULT ReadRecord (const aafWChar * pFileName)
{
  HRESULT hr = E_FAIL;
  IAAFFileSP   pFile;

  try 
	{
	  // Open the file, and get the dictionary.
	  // Test fails when openened in default loading mode. So set mode to
	  // lazy loading
	  checkResult(AAFFileOpenExistingRead(pFileName, AAF_FILE_MODE_LAZY_LOADING, &pFile));
	  IAAFHeaderSP pHeader;
	  checkResult(pFile->GetHeader(&pHeader));
	  IAAFDictionarySP pDict;
	  checkResult (pHeader->GetDictionary(&pDict));
	  CAAFBuiltinDefs defs(pDict);

	  // Get the type definitions for our new types.
	  IAAFTypeDefSP ptd;
	  checkResult (pDict->LookupTypeDef (sTypeId_Rational8,
										 &ptd));
	  IAAFTypeDefRecordSP ptdr8;
	  checkResult (ptd->QueryInterface (IID_IAAFTypeDefRecord,
										(void**) &ptdr8));

	  // check for duplicate initialization
	  aafUID_t nullUid = { 0 };
	  HRESULT temphr;
	  temphr = ptdr8->Initialize (nullUid,
								  0,
								  0,
								  1,
								  L"foo");
	  checkExpression (AAFRESULT_ALREADY_INITIALIZED == temphr,
					   AAFRESULT_TEST_FAILED);

	  aafUInt32 count = 0;
	  checkResult (ptdr8->GetCount (&count));
	  checkExpression (2 == count, AAFRESULT_TEST_FAILED);

	  aafUInt32  nameLen = 0;
	  aafCharacter nameBuf [100];
	  checkResult (ptdr8->GetMemberNameBufLen (0, &nameLen));
	  checkExpression (((wcslen(L"Numerator")+1)*sizeof(aafCharacter))
					   == nameLen,
					   AAFRESULT_TEST_FAILED);
	  checkExpression (nameLen < 100,
					   AAFRESULT_TEST_FAILED);
	  checkResult (ptdr8->GetMemberName (0,
										 nameBuf,
										 sizeof (nameBuf)));
	  checkExpression (0 == wcscmp (L"Numerator", nameBuf),
					   AAFRESULT_TEST_FAILED);

	  checkResult (ptdr8->GetMemberNameBufLen (1, &nameLen));
	  checkExpression (((wcslen(L"Denominator")+1)*sizeof (aafCharacter))
					   == nameLen,
					   AAFRESULT_TEST_FAILED);
	  checkExpression (nameLen < 100,
					   AAFRESULT_TEST_FAILED);
	  checkResult (ptdr8->GetMemberName (1,
										 nameBuf,
										 sizeof (nameBuf)));
	  checkExpression (0 == wcscmp (L"Denominator", nameBuf),
					   AAFRESULT_TEST_FAILED);

	  temphr = ptdr8->GetMemberNameBufLen (2, &nameLen);
	  checkExpression (AAFRESULT_ILLEGAL_VALUE == temphr,
					   AAFRESULT_TEST_FAILED);

	  temphr = ptdr8->GetMemberName (2,
									 nameBuf,
									 sizeof (nameBuf));
	  checkExpression (AAFRESULT_ILLEGAL_VALUE == temphr,
					   AAFRESULT_TEST_FAILED);

	  checkResult (pDict->LookupTypeDef (sTypeId_Rational8_pair,
										 &ptd));
	  IAAFTypeDefRecordSP ptdr8p;
	  checkResult (ptd->QueryInterface (IID_IAAFTypeDefRecord,
										(void**) &ptdr8p));

	  // check for duplicate initialization
	  temphr = ptdr8p->Initialize (nullUid,
								   0,
								   0,
								   1,
								   L"foo");
	  checkExpression (AAFRESULT_ALREADY_INITIALIZED == temphr,
					   AAFRESULT_TEST_FAILED);

	  count = 0;
	  checkResult (ptdr8p->GetCount (&count));
	  checkExpression (2 == count, AAFRESULT_TEST_FAILED);

	  // Now attempt to register offsets of rational8_pair.  This
	  // should fail, because offsets of its members (of type
	  // rational8) have not yet been registered.
	  temphr = RegisterRational8PairOffsets (ptdr8p);
	  checkExpression (temphr == AAFRESULT_NOT_REGISTERED,
					   AAFRESULT_TEST_FAILED);

	  // Now let's attempt registration the right way.
	  checkResult (RegisterRational8Offsets (ptdr8));
	  checkResult (RegisterRational8PairOffsets (ptdr8p));
		
	  IAAFTypeDef * pMemberTd = 0;
	  IAAFTypeDef * pTempTd = 0;
	  checkResult (ptdr8->QueryInterface (IID_IAAFTypeDef,
										(void**) &pTempTd));
	  checkResult (ptdr8p->GetMemberType (0, &pMemberTd));
	  checkExpression (pMemberTd == pTempTd,
					   AAFRESULT_TEST_FAILED);
	  pMemberTd->Release ();
	  pMemberTd = 0;

	  checkResult (ptdr8p->GetMemberType (1, &pMemberTd));
	  checkExpression (pMemberTd == pTempTd,
					   AAFRESULT_TEST_FAILED);
	  pMemberTd->Release ();
	  pMemberTd = 0;
	  pTempTd->Release ();
	  pTempTd = 0;

	  temphr = ptdr8p->GetMemberType (2, &pMemberTd);
	  checkExpression (temphr == AAFRESULT_ILLEGAL_VALUE,
					   AAFRESULT_TEST_FAILED);

	  // Now read the CompositionMob to which we added some optional
	  // properties.
	  IEnumAAFMobsSP pEnumMobs;
	  checkResult (pHeader->GetMobs (0, &pEnumMobs));

	  IAAFMobSP pMob;
	  checkResult (pEnumMobs->NextOne (&pMob));
	  IAAFObjectSP pObj;
	  checkResult (pMob->QueryInterface (IID_IAAFObject,
										 (void**) &pObj));

	  // get the property definitions for the added properties
	  IAAFPropertyDefSP pPdPosA;
	  checkResult (defs.cdCompositionMob()->
				   LookupPropertyDef (sPropertyId_positionA,
									  &pPdPosA));
	  IAAFPropertyDefSP pPdPosB;
	  checkResult (defs.cdCompositionMob()->
				   LookupPropertyDef (sPropertyId_positionB,
									  &pPdPosB));
	  IAAFPropertyDefSP pPdPosC;
	  checkResult (defs.cdCompositionMob()->
				   LookupPropertyDef (sPropertyId_positionC,
									  &pPdPosC));

	  IAAFPropertyValueSP pPVa;
	  checkResult (pObj->GetPropertyValue (pPdPosA, &pPVa));
	  IAAFPropertyValueSP pPVb;
	  checkResult (pObj->GetPropertyValue (pPdPosB, &pPVb));
	  IAAFPropertyValueSP pPVc;
	  checkResult (pObj->GetPropertyValue (pPdPosC, &pPVc));

	  // Try to read the first one with GetStruct.
	  rational8pair_t valA = { 0 };
	  checkResult (ptdr8p->GetStruct (pPVa,
									  (aafMemPtr_t) &valA,
									  sizeof (valA)));
	  checkExpression (1 == valA.X_Position.Numerator,
					   AAFRESULT_TEST_FAILED);
	  checkExpression (2 == valA.X_Position.Denominator,
					   AAFRESULT_TEST_FAILED);
	  checkExpression (3 == valA.Y_Position.Numerator,
					   AAFRESULT_TEST_FAILED);
	  checkExpression (4 == valA.Y_Position.Denominator,
					   AAFRESULT_TEST_FAILED);

	  // Try to read the second one by decomposing with GetValue.
	  IAAFPropertyValueSP pPvX;
	  checkResult (ptdr8p->GetValue (pPVb,
									 0,
									 &pPvX));
	  IAAFPropertyValueSP pPvY;
	  checkResult (ptdr8p->GetValue (pPVb,
									 1,
									 &pPvY));

	  IAAFPropertyValueSP pPvXn;
	  checkResult (ptdr8->GetValue (pPvX,
									0,
									&pPvXn));
	  IAAFPropertyValueSP pPvXd;
	  checkResult (ptdr8->GetValue (pPvX,
									1,
									&pPvXd));
	  IAAFPropertyValueSP pPvYn;
	  checkResult (ptdr8->GetValue (pPvY,
									0,
									&pPvYn));
	  IAAFPropertyValueSP pPvYd;
	  checkResult (ptdr8->GetValue (pPvY,
									1,
									&pPvYd));
	  IAAFPropertyValueSP junkPv;
	  temphr = ptdr8->GetValue (pPvY,
								2,
								&junkPv);
	  checkExpression (AAFRESULT_ILLEGAL_VALUE == temphr,
					   AAFRESULT_TEST_FAILED);
	  temphr = ptdr8->GetValue (0,
								1,
								&junkPv);
	  checkExpression (AAFRESULT_NULL_PARAM == temphr,
					   AAFRESULT_TEST_FAILED);
	  temphr = ptdr8->GetValue (pPvY,
								1,
								0);
	  checkExpression (AAFRESULT_NULL_PARAM == temphr,
					   AAFRESULT_TEST_FAILED);

	  // Get type def for uint8; use it to read the individual values
	  IAAFTypeDefIntSP ptdui8;
	  checkResult (defs.tdUInt8()->
				   QueryInterface (IID_IAAFTypeDefInt,
								   (void**) &ptdui8));
	  aafUInt32 val;
	  checkResult (ptdui8->GetInteger (pPvXn,
									   (aafMemPtr_t) &val,
									   sizeof (val)));
	  checkExpression (5 == val, AAFRESULT_TEST_FAILED);
	  checkResult (ptdui8->GetInteger (pPvXd,
									   (aafMemPtr_t) &val,
									   sizeof (val)));
	  checkExpression (6 == val, AAFRESULT_TEST_FAILED);
	  checkResult (ptdui8->GetInteger (pPvYn,
									   (aafMemPtr_t) &val,
									   sizeof (val)));
	  checkExpression (7 == val, AAFRESULT_TEST_FAILED);
	  checkResult (ptdui8->GetInteger (pPvYd,
									   (aafMemPtr_t) &val,
									   sizeof (val)));
	  checkExpression (8 == val, AAFRESULT_TEST_FAILED);

	  // Read the last two with GetStruct just to get it over with. ;)
	  rational8pair_t valC = { 0 };
	  temphr = ptdr8p->GetStruct (pPVc,
								  (aafMemPtr_t) &valC,
								  sizeof (valC)-1);
	  checkExpression (AAFRESULT_ILLEGAL_VALUE == temphr,
					   AAFRESULT_TEST_FAILED);
	  temphr = ptdr8p->GetStruct (0,
								  (aafMemPtr_t) &valC,
								  sizeof (valC));
	  checkExpression (AAFRESULT_NULL_PARAM == temphr,
					   AAFRESULT_TEST_FAILED);
	  temphr = ptdr8p->GetStruct (pPVc,
								  0,
								  sizeof (valC));
	  checkExpression (AAFRESULT_NULL_PARAM == temphr,
					   AAFRESULT_TEST_FAILED);
	  checkResult (ptdr8p->GetStruct (pPVc,
									  (aafMemPtr_t) &valC,
									  sizeof (valC)));
	  checkExpression (9 == valC.X_Position.Numerator,
					   AAFRESULT_TEST_FAILED);
	  checkExpression (10 == valC.X_Position.Denominator,
					   AAFRESULT_TEST_FAILED);
	  checkExpression (11 == valC.Y_Position.Numerator,
					   AAFRESULT_TEST_FAILED);
	  checkExpression (12 == valC.Y_Position.Denominator,
					   AAFRESULT_TEST_FAILED);

	  // Attempt to close the file.
	  checkResult(pFile->Close());
	  IAAFFileSP nullFile;
	  pFile = nullFile;  // zeros the pFile, and releases it.

	  hr = AAFRESULT_SUCCESS;
	}
  catch (HRESULT& rResult)
	{
	  hr = rResult;
	}
	
  // cleanup
  if (pFile)
	{
	  pFile->Close();
	}
	
  return hr;
}


extern "C" HRESULT CAAFTypeDefRecord_test(testMode_t mode);
extern "C" HRESULT CAAFTypeDefRecord_test(testMode_t mode)
{
  HRESULT hr = AAFRESULT_TEST_FAILED;
  const aafWChar * pFileName = L"AAFTypeDefRecordTest.aaf";

  try
    {
      if(mode == kAAFUnitTestReadWrite)
		hr =  WriteRecord(pFileName);
	  if (SUCCEEDED (hr) || mode != mode == kAAFUnitTestReadWrite)
		{
		  hr = AAFRESULT_TEST_FAILED;
		  hr = ReadRecord(pFileName);
		}
	}
  catch (...)
    {
      cerr << "CAAFTypeDefInt_test...Caught general C++"
		   << " exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
    }

  return hr;
}

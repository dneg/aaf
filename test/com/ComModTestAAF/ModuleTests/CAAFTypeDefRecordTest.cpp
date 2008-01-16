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
// The Original Code of this file is Copyright 1998-2005, Licensor of the
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
typedef IAAFSmartPointer<IAAFTypeDefVariableArray>		IAAFTypeDefVariableArraySP;
typedef IAAFSmartPointer<IEnumAAFMobs>       IEnumAAFMobsSP;

// convenient version manipulater
inline aafUInt32 versionUInt( aafProductVersion_t testRev )
{
	return ((testRev.major*100 + testRev.minor)*100 + testRev.tertiary)*100 + testRev.patchLevel;
}
inline aafUInt32 versionUInt( aafUInt16 major, aafUInt16 minor, aafUInt16 tertiary =0, aafUInt16 patchLevel =0 )
{
	return ((major*100 + minor)*100 + tertiary)*100 + patchLevel;
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


//
// Type IDs
//
//
// {5BC8A140-2BE4-11d4-B80D-0000863F2C27}
static const aafUID_t sTypeId_Rational16 = 
{ 0x5bc8a140, 0x2be4, 0x11d4,
  { 0xb8, 0xd, 0x0, 0x0, 0x86, 0x3f, 0x2c, 0x27 }
};
//
// {5BC8A141-2BE4-11d4-B80D-0000863F2C27}
static const aafUID_t sTypeId_Rational16_pair = 
{ 0x5bc8a141, 0x2be4, 0x11d4,
  { 0xb8, 0xd, 0x0, 0x0, 0x86, 0x3f, 0x2c, 0x27 }
};
//
// {2FB711E7-35DA-4722-9F3E-B3130CD57AC3}
static const aafUID_t sTypeId_Rational16_array = 
{ 0x2fb711e7, 0x35da, 0x4722, 
  { 0x9f, 0x3e, 0xb3, 0x13, 0xc, 0xd5, 0x7a, 0xc3 }
};
//
// {ed9cbe2f-1a42-420c-952e-7b23adbbc479}
static const aafUID_t sTypeId_Mixed = 
{ 0xed9cbe2f, 0x1a42, 0x420c, 
  { 0x95, 0x2e, 0x7b, 0x23, 0xad, 0xbb, 0xc4, 0x79 }
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
//
// {0B876F07-EA1B-4c78-A4BF-EDC35E583F29}
static const aafUID_t sPropertyId_positionN = 
{ 0xb876f07, 0xea1b, 0x4c78, 
  { 0xa4, 0xbf, 0xed, 0xc3, 0x5e, 0x58, 0x3f, 0x29 } 
};
//
// {08dc656c-de12-4dfb-a047-bb687bda41da}
static const aafUID_t sPropertyId_velocity = 
{ 0x08dc656c, 0xde12, 0x4dfb, 
  { 0xa0, 0x47, 0xbb, 0x68, 0x7b, 0xda, 0x41, 0xda } 
};


struct Rational16_t
{
  aafInt16  Numerator;
  aafUInt16 Denominator;
};

struct Rational16pair_t
{
  Rational16_t X_Position;
  Rational16_t Y_Position;
};

struct Mixed_t
{
  aafInt8	angle;
  aafUInt32	speed;
};


static HRESULT RegisterRational16Offsets (IAAFTypeDefRecord * ptd)
{
  assert (ptd);
  aafUInt32 offsets[] =
  {
	offsetof (Rational16_t, Numerator),
	offsetof (Rational16_t, Denominator)
  };
  return ptd->RegisterMembers (offsets,
							   2,
							   sizeof (Rational16_t));
}


static HRESULT RegisterRational16PairOffsets (IAAFTypeDefRecord * ptd)
{
  assert (ptd);
  aafUInt32 offsets[] =
  {
	offsetof (Rational16pair_t, X_Position),
	offsetof (Rational16pair_t, Y_Position)
  };
  return ptd->RegisterMembers (offsets,
							   2,
							   sizeof (Rational16pair_t));
}

static HRESULT RegisterMixedOffsets (IAAFTypeDefRecord * ptd)
{
  assert (ptd);
  aafUInt32 offsets[] =
  {
	offsetof (Mixed_t, angle),
	offsetof (Mixed_t, speed)
  };
  return ptd->RegisterMembers (offsets,
							   2,
							   sizeof (Mixed_t));
}

static HRESULT WriteRecord (
    const aafWChar * pFileName,
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID)
{
  HRESULT hr = E_FAIL;
  IAAFFileSP   pFile;

  try 
	{
	  HRESULT temphr;

	  // Remove the previous test file if any.
	  RemoveTestFile(pFileName);

	  // Create the file and get the new file's header.
	  checkResult(CreateTestFile( pFileName, fileKind, rawStorageType, productID, &pFile ));
	  IAAFHeaderSP pHeader;
  	  checkResult(pFile->GetHeader(&pHeader));
	  IAAFDictionarySP pDict;
	  checkResult (pHeader->GetDictionary(&pDict));
	  CAAFBuiltinDefs defs(pDict);

	  // get the SDK version against which we are testing
      aafProductVersion_t			testVer;
      checkResult(pHeader->GetRefImplVersion(&testVer));

	  // Create, initialize, and register the Rational16 type, to
	  // consist of an Int16 numerator and a UInt16 denominator.
	  IAAFTypeDef* tdr16MemberTypes[] =
	  {defs.tdInt16(),
	   defs.tdUInt16()};
	  aafWChar numerator[] = L"Numerator";
	  aafWChar denominator[] = L"Denominator";
	  aafString_t tdr16MemberNames[] =
	  {numerator,
	   denominator};
	  IAAFTypeDefRecordSP ptdr16;
	  checkResult (pDict->
				   CreateMetaInstance (kAAFClassID_TypeDefRecord,
									   IID_IAAFTypeDefRecord,
									   (IUnknown**) &ptdr16));

	  checkResult (ptdr16->Initialize (sTypeId_Rational16,
									  tdr16MemberTypes,
									  tdr16MemberNames,
									  2,
									  L"Rational16"));

	  // check for duplicate initialization
	  temphr = ptdr16->Initialize (sTypeId_Rational16,
								  tdr16MemberTypes,
								  tdr16MemberNames,
								  2,
								  L"Rational16");
	  checkExpression (AAFRESULT_ALREADY_INITIALIZED == temphr,
					   AAFRESULT_TEST_FAILED);

	  IAAFTypeDefSP ptd;
	  checkResult (ptdr16->QueryInterface (IID_IAAFTypeDef, (void**) &ptd));
	  checkResult (pDict->RegisterTypeDef (ptd));

	  // Create, initialize, and register the Rational16Position type,
	  // to consist of a Rational16 X and Rational16 Y coordinates.

	  IAAFTypeDefRecordSP ptdr16p;
	  //checkResult (defs.cdTypeDefRecord()->
	  // CreateInstance (IID_IAAFTypeDefRecord,
	  // (IUnknown**)&ptdr16p));
	  checkResult (pDict->
				   CreateMetaInstance (kAAFClassID_TypeDefRecord,
									   IID_IAAFTypeDefRecord,
									   (IUnknown**) &ptdr16p));

	  IAAFTypeDef * pTempTd = 0;
	  checkResult (ptdr16->QueryInterface (IID_IAAFTypeDef,
										  (void**) &pTempTd));	  
	  IAAFTypeDef* tdr16pMemberTypes[] =
	  {pTempTd,
	   pTempTd};
	  aafWChar xpos[] = L"X Position";
	  aafWChar ypos[] = L"Y Position";
	  aafString_t tdr16pMemberNames[] =
	  {xpos,
	   ypos};
	  checkResult (ptdr16p->Initialize (sTypeId_Rational16_pair,
									   tdr16pMemberTypes,
									   tdr16pMemberNames,
									   2,
									   L"Rational16Pair"));
	  // check for duplicate initialization
	  temphr = ptdr16p->Initialize (sTypeId_Rational16,
								   tdr16pMemberTypes,
								   tdr16pMemberNames,
								   2,
								   L"Rational16Pair");
	  checkExpression (AAFRESULT_ALREADY_INITIALIZED == temphr,
					   AAFRESULT_TEST_FAILED);

	  pTempTd->Release ();
	  pTempTd = 0;
	  checkResult (ptdr16p->QueryInterface (IID_IAAFTypeDef, (void**) &ptd));
	  checkResult (pDict->RegisterTypeDef (ptd));

	  // Now attempt to register offsets of Rational16_pair.  This
	  // should fail, because offsets of its members (of type
	  // Rational16) have not yet been registered.
	  temphr = RegisterRational16PairOffsets (ptdr16p);
	  checkExpression (temphr == AAFRESULT_NOT_REGISTERED,
					   AAFRESULT_TEST_FAILED);

	  // Now let's attempt registration the right way.
	  checkResult (RegisterRational16Offsets (ptdr16));
	  checkResult (RegisterRational16PairOffsets (ptdr16p));

	  // register variable array of Rational16Pair records
	  IAAFTypeDefVariableArraySP ptdvaarpr;
	  IAAFTypeDefSP ptdarpr;
	  // perform this part only for specified versions
	  if( versionUInt(testVer) >= versionUInt(1,1,1,0) )
	  {
		//Create a Variable Array
		checkResult(pDict->CreateMetaInstance (AUID_AAFTypeDefVariableArray, IID_IAAFTypeDefVariableArray, (IUnknown **) &ptdvaarpr));
		
		//IAAFTypeDefVariableArray::Initialize
		checkResult(ptdvaarpr->Initialize(sTypeId_Rational16_array, ptd, L"Rational16PairArray"));
		
		//  Register our new VA type def :
		checkResult(ptdvaarpr->QueryInterface(IID_IAAFTypeDef, (void**)&ptdarpr));
		checkResult(pDict->RegisterTypeDef(ptdarpr));
	  }

	  // Create a new property on Composition mob (called Position)
	  // whose type is Rational16_pair.
	  checkResult (ptdr16p->QueryInterface (IID_IAAFTypeDef,
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

	  // register property of type variable array of Rational16Pair records
	  IAAFPropertyDefSP pPropDefPosN;
	  // perform this part only for specified versions
	  if( versionUInt(testVer) >= versionUInt(1,1,1,0) )
	  {
		checkResult (defs.cdCompositionMob()->RegisterOptionalPropertyDef
					(sPropertyId_positionN,
						L"PositionN",
						ptdarpr,
						&pPropDefPosN));
	  }


	  // Register the Mixed_t type
	  IAAFTypeDef* tdMixedMemberTypes[] = {defs.tdInt8(), defs.tdUInt32()};
	  aafWChar str_angle[] = L"Angle";
	  aafWChar str_speed[] = L"Speed";
	  aafString_t tdMixedMemberNames[] = {str_angle, str_speed};
	  IAAFTypeDefRecordSP ptdrmixed;
	  checkResult(pDict->CreateMetaInstance(kAAFClassID_TypeDefRecord, IID_IAAFTypeDefRecord, (IUnknown**) &ptdrmixed));
	  checkResult(ptdrmixed->Initialize(sTypeId_Mixed, tdMixedMemberTypes, tdMixedMemberNames, 2, L"Mixed"));
	  //  Register Mixed type def
	  IAAFTypeDefSP ptdmixed;
	  checkResult(ptdrmixed->QueryInterface(IID_IAAFTypeDef, (void**)&ptdmixed));
	  checkResult(pDict->RegisterTypeDef(ptdmixed));
	  checkResult(RegisterMixedOffsets(ptdrmixed));
	  // Added Velocity property to Composition Mob
	  IAAFPropertyDefSP pPropDefVelocity;
	  checkResult (defs.cdCompositionMob()->RegisterOptionalPropertyDef
				   (sPropertyId_velocity,
					L"Velocity",
					ptdmixed,
					&pPropDefVelocity));


	  // Create one of our new CompositionMobs, and add a values for
	  // the Position property.
	  IAAFCompositionMobSP pcm;
	  checkResult (defs.cdCompositionMob()->
				   CreateInstance (IID_IAAFCompositionMob,
								   (IUnknown**)&pcm));

	  // Add property value for PositionA using CreateValueFromValues
	  // methods.  The constituent property values will be created
	  // from CreateValueFromStruct, and from SetValueFromStruct.

	  const Rational16_t v_12 = {1, 2};
	  const Rational16_t v_34 = {3, 4};
	  IAAFPropertyValue * pValRat[2] = { 0 };

	  checkResult (ptdr16->
				   CreateValueFromStruct ((aafMemPtr_t) &v_12,
										  sizeof (v_12),
										  &pValRat[0]));
	  // create it with wrong data, and use SetStruct to give it right
	  // data
	  checkResult (ptdr16->
				   CreateValueFromStruct ((aafMemPtr_t) &v_12,
										  sizeof (v_12),
										  &pValRat[1]));
	  checkResult (ptdr16->
				   SetStruct (pValRat[1],
							  (aafMemPtr_t) &v_34,
							  sizeof (v_34)));

	  // Create the Rational16_pair property value from the
	  // Rational16 property values.
	  IAAFPropertyValueSP pRat16PairVal_1234;
	  temphr = ptdr16p->CreateValueFromValues (0,
											  2,
											  &pRat16PairVal_1234);
	  checkExpression (AAFRESULT_NULL_PARAM == temphr,
					   AAFRESULT_TEST_FAILED);
	  temphr = ptdr16p->CreateValueFromValues (pValRat,
											  1,
											  &pRat16PairVal_1234);
	  checkExpression (AAFRESULT_ILLEGAL_VALUE == temphr,
					   AAFRESULT_TEST_FAILED);
	  checkResult (ptdr16p->CreateValueFromValues (pValRat,
												  2,
												  &pRat16PairVal_1234));
	  pValRat[0]->Release ();
	  pValRat[0] = 0;
	  pValRat[1]->Release ();
	  pValRat[1] = 0;

	  IAAFObjectSP pObj;
	  checkResult (pcm->QueryInterface (IID_IAAFObject,
										(void**) &pObj));
	  checkResult (pObj->SetPropertyValue (pPropDefPosA,
										   pRat16PairVal_1234));


	  // Add property value for PositionB using CreateValueFromStruct
	  // methods.
	  const Rational16_t r16_56 = {5, 6};
	  const Rational16_t r16_78 = {7, 8};
	  Rational16pair_t r16p_5678;
	  r16p_5678.X_Position = r16_56;
	  r16p_5678.Y_Position = r16_78;
	  IAAFPropertyValueSP pRat16PairVal_5678;
	  temphr = ptdr16p->
		CreateValueFromStruct (0,
							   sizeof (r16p_5678),
							   &pRat16PairVal_5678);
	  checkExpression (AAFRESULT_NULL_PARAM == temphr,
					   AAFRESULT_TEST_FAILED);
	  temphr = ptdr16p->
		CreateValueFromStruct ((aafMemPtr_t) &r16p_5678,
							   sizeof (r16p_5678),
							   0);
	  checkExpression (AAFRESULT_NULL_PARAM == temphr,
					   AAFRESULT_TEST_FAILED);
	  temphr = ptdr16p->
		CreateValueFromStruct ((aafMemPtr_t) &r16p_5678,
							   sizeof (r16p_5678)+1,
							   &pRat16PairVal_5678);
	  checkExpression (AAFRESULT_ILLEGAL_VALUE == temphr,
					   AAFRESULT_TEST_FAILED);
	  checkResult (ptdr16p->
				   CreateValueFromStruct ((aafMemPtr_t) &r16p_5678,
										  sizeof (r16p_5678),
										  &pRat16PairVal_5678));
	  checkResult (pObj->SetPropertyValue (pPropDefPosB,
										   pRat16PairVal_5678));

	  // Add property value for PositionC using CreateValueFromStruct
	  // methods, and then use SetStruct to set the value.
	  const Rational16_t r16_9a = {9, 10};
	  const Rational16_t r16_bc = {11, 12};
	  Rational16pair_t r16p_9abc;
	  r16p_9abc.X_Position = r16_9a;
	  r16p_9abc.Y_Position = r16_bc;
	  IAAFPropertyValueSP pRat16PairVal_9abc;
	  checkResult (ptdr16p->
				   CreateValueFromStruct ((aafMemPtr_t) &r16p_5678,
										  sizeof (r16p_5678),
										  &pRat16PairVal_9abc));
	  temphr = ptdr16p->SetStruct (0,
								  (aafMemPtr_t) &r16p_9abc,
								  sizeof (r16p_9abc));
	  checkExpression (AAFRESULT_NULL_PARAM == temphr,
					   AAFRESULT_TEST_FAILED);
	  temphr = ptdr16p->SetStruct (pRat16PairVal_9abc,
								  0,
								  sizeof (r16p_9abc));
	  checkExpression (AAFRESULT_NULL_PARAM == temphr,
					   AAFRESULT_TEST_FAILED);
	  temphr = ptdr16p->SetStruct (pRat16PairVal_9abc,
								  (aafMemPtr_t) &r16p_9abc,
								  sizeof (r16p_9abc)-1);
	  checkExpression (AAFRESULT_ILLEGAL_VALUE == temphr,
					   AAFRESULT_TEST_FAILED);
	  checkResult (ptdr16p->SetStruct (pRat16PairVal_9abc,
									  (aafMemPtr_t) &r16p_9abc,
										  sizeof (r16p_9abc)));
	  checkResult (pObj->SetPropertyValue (pPropDefPosC,
										   pRat16PairVal_9abc));

	  // add a value of PropertyN
	  IAAFPropertyValueSP spArrayPropertyValue;
	  // perform this part only for specified versions
	  if( versionUInt(testVer) >= versionUInt(1,1,1,0) )
	  {
	    // Create an empty array and then fill it by appending elements...
  		checkResult( ptdvaarpr->CreateEmptyValue (&spArrayPropertyValue) );
		
	    // intentionally in permuted order
		checkResult( ptdvaarpr->AppendElement(spArrayPropertyValue, pRat16PairVal_5678) );

		checkResult( ptdvaarpr->AppendElement(spArrayPropertyValue, pRat16PairVal_9abc) );

		checkResult( ptdvaarpr->AppendElement(spArrayPropertyValue, pRat16PairVal_1234) );

		// set PropertyN
		checkResult ( pObj->SetPropertyValue( pPropDefPosN, spArrayPropertyValue ) );
	  }

	  // Set the value on the new Velocity property (of Mixed_t type)
	  const Mixed_t velocty = {15, 2001};
	  IAAFPropertyValueSP pVelocity;
	  checkResult (ptdrmixed->CreateValueFromStruct((aafMemPtr_t) &velocty, sizeof(velocty), &pVelocity));
	  checkResult (pObj->SetPropertyValue(pPropDefVelocity, pVelocity));


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


/* throws on error */
static void CheckMemberTypeEqual (IAAFTypeDefRecordSP rec, 
                                     int member, IAAFTypeDef *td)
{
  IAAFTypeDef * pMemberTd = 0;
  checkResult (rec->GetMemberType (member, &pMemberTd));
  checkExpression (pMemberTd == td,
				   AAFRESULT_TEST_FAILED);
  pMemberTd->Release ();
}



/* Throws on error */
static void CheckRational16ByValues 
         (IAAFTypeDefRecordSP pTDr16,
          IAAFPropertyValueSP pv,    
          Rational16_t expected)
{
  IAAFPropertyValueSP pvMemNum, pvMemDen;
  IAAFPropertyValueSP junkPv;
  checkResult (pTDr16->GetValue (pv, 0, &pvMemNum));
  checkResult (pTDr16->GetValue (pv, 1, &pvMemDen));

  // Check that there are no more members
  checkExpression (
    pTDr16->GetValue (pv, 2, &junkPv) == AAFRESULT_ILLEGAL_VALUE,
    AAFRESULT_TEST_FAILED
  );


  IAAFTypeDefSP td;
  IAAFTypeDefInt *tdint;
  aafInt16  num;
  aafUInt16 den;
    
  checkResult (pTDr16->GetMemberType (0, &td));
  checkResult (td->QueryInterface (IID_IAAFTypeDefInt,
                              (void **) &tdint));
  checkResult (tdint->GetInteger (pvMemNum, 
                               (aafMemPtr_t)(&num), sizeof(num)));

  checkResult (pTDr16->GetMemberType (1, &td));
  checkResult (td->QueryInterface (IID_IAAFTypeDefInt,
                              (void **) &tdint));
  checkResult (tdint->GetInteger (pvMemDen, 
                               (aafMemPtr_t)(&den), sizeof(den)));
  
  checkExpression(expected.Numerator == num && expected.Denominator == den,
                  AAFRESULT_TEST_FAILED);
}


/* Throws on error */
static void CheckRational16PairByValues 
         (IAAFTypeDefRecordSP pTDr16pair,
          IAAFTypeDefRecordSP pTDr16,
          IAAFPropertyValueSP pv, 
          Rational16pair_t expected)
{
  IAAFPropertyValueSP pvMemX, pvMemY;
  IAAFPropertyValueSP junkPv;
  
  checkResult (pTDr16pair->GetValue (pv, 0, &pvMemX));
  checkResult (pTDr16pair->GetValue (pv, 1, &pvMemY));

  // Check that there are no more members
  checkExpression (
    pTDr16pair->GetValue (pv, 2, &junkPv) == AAFRESULT_ILLEGAL_VALUE,
    AAFRESULT_TEST_FAILED
  );
  
  CheckRational16ByValues(pTDr16, pvMemX, expected.X_Position);
  CheckRational16ByValues(pTDr16, pvMemY, expected.Y_Position);
}


static HRESULT ReadRecord (const aafWChar * pFileName, int loadingMode)
{
  HRESULT hr = E_FAIL;
  IAAFFileSP   pFile;

  try 
	{
	  // Open the file, and get the dictionary.
	  checkResult(AAFFileOpenExistingRead(pFileName, loadingMode, &pFile));
	  IAAFHeaderSP pHeader;
	  checkResult(pFile->GetHeader(&pHeader));
	  IAAFDictionarySP pDict;
	  checkResult (pHeader->GetDictionary(&pDict));
	  CAAFBuiltinDefs defs(pDict);

	  // get the SDK version against which we are testing
	  aafProductVersion_t			testVer;
	  checkResult(pHeader->GetRefImplVersion(&testVer));

	  // Get the type definitions for our new types.
	  IAAFTypeDefSP ptd;
	  checkResult (pDict->LookupTypeDef (sTypeId_Rational16,
										 &ptd));
	  IAAFTypeDefRecordSP ptdr16;
	  checkResult (ptd->QueryInterface (IID_IAAFTypeDefRecord,
										(void**) &ptdr16));

	  // check for duplicate initialization
	  aafUID_t nullUid = { 0 };
	  HRESULT temphr;
	  temphr = ptdr16->Initialize (nullUid,
								  0,
								  0,
								  1,
								  L"foo");
	  checkExpression (AAFRESULT_ALREADY_INITIALIZED == temphr,
					   AAFRESULT_TEST_FAILED);

	  aafUInt32 count = 0;
	  checkResult (ptdr16->GetCount (&count));
	  checkExpression (2 == count, AAFRESULT_TEST_FAILED);

	  aafUInt32  nameLen = 0;
	  aafCharacter nameBuf [100];
	  checkResult (ptdr16->GetMemberNameBufLen (0, &nameLen));
	  checkExpression (((wcslen(L"Numerator")+1)*sizeof(aafCharacter))
					   == nameLen,
					   AAFRESULT_TEST_FAILED);
	  checkExpression (nameLen < 100,
					   AAFRESULT_TEST_FAILED);
	  checkResult (ptdr16->GetMemberName (0,
										 nameBuf,
										 sizeof (nameBuf)));
	  checkExpression (0 == wcscmp (L"Numerator", nameBuf),
					   AAFRESULT_TEST_FAILED);

	  checkResult (ptdr16->GetMemberNameBufLen (1, &nameLen));
	  checkExpression (((wcslen(L"Denominator")+1)*sizeof (aafCharacter))
					   == nameLen,
					   AAFRESULT_TEST_FAILED);
	  checkExpression (nameLen < 100,
					   AAFRESULT_TEST_FAILED);
	  checkResult (ptdr16->GetMemberName (1,
										 nameBuf,
										 sizeof (nameBuf)));
	  checkExpression (0 == wcscmp (L"Denominator", nameBuf),
					   AAFRESULT_TEST_FAILED);

	  temphr = ptdr16->GetMemberNameBufLen (2, &nameLen);
	  checkExpression (AAFRESULT_ILLEGAL_VALUE == temphr,
					   AAFRESULT_TEST_FAILED);

	  temphr = ptdr16->GetMemberName (2,
									 nameBuf,
									 sizeof (nameBuf));
	  checkExpression (AAFRESULT_ILLEGAL_VALUE == temphr,
					   AAFRESULT_TEST_FAILED);

	  checkResult (pDict->LookupTypeDef (sTypeId_Rational16_pair,
										 &ptd));
	  IAAFTypeDefRecordSP ptdr16p;
	  checkResult (ptd->QueryInterface (IID_IAAFTypeDefRecord,
										(void**) &ptdr16p));

	  // check for duplicate initialization
	  temphr = ptdr16p->Initialize (nullUid,
								   0,
								   0,
								   1,
								   L"foo");
	  checkExpression (AAFRESULT_ALREADY_INITIALIZED == temphr,
					   AAFRESULT_TEST_FAILED);

	  count = 0;
	  checkResult (ptdr16p->GetCount (&count));
	  checkExpression (2 == count, AAFRESULT_TEST_FAILED);

	  // Now attempt to register offsets of Rational16_pair.  This
	  // should fail, because offsets of its members (of type
	  // Rational16) have not yet been registered.
	  temphr = RegisterRational16PairOffsets (ptdr16p);
	  checkExpression (temphr == AAFRESULT_NOT_REGISTERED,
					   AAFRESULT_TEST_FAILED);

	  // Now let's attempt registration the right way.
	  checkResult (RegisterRational16Offsets (ptdr16));
	  checkResult (RegisterRational16PairOffsets (ptdr16p));
		
	  // Setup to read the Velocity property which is of typed Mixed_t
	  checkResult (pDict->LookupTypeDef (sTypeId_Mixed, &ptd));
	  IAAFTypeDefRecordSP ptdrmixed;
	  checkResult (ptd->QueryInterface (IID_IAAFTypeDefRecord, (void**) &ptdrmixed));
	  temphr = RegisterMixedOffsets(ptdrmixed);
	  /* It is ok to return default already used due to current limitation
           * in implementation that default registration may occur and tie us
           * to a specific structural layout before we get the ability to
           * register own offsets/layout. This is slated for fixing in
           * future rev of the SDK.
           */
	  if (temphr == AAFRESULT_DEFAULT_ALREADY_USED)
	      throw AAFRESULT_SUCCESS;

	  checkResult (temphr);

	  IAAFTypeDef * pTempTd = 0;
	  checkResult (ptdr16->QueryInterface (IID_IAAFTypeDef,
										(void**) &pTempTd));
	  CheckMemberTypeEqual (ptdr16p, 0, pTempTd);
	  CheckMemberTypeEqual (ptdr16p, 1, pTempTd);
	  pTempTd->Release ();
	  pTempTd = 0;

	  temphr = ptdr16p->GetMemberType (2, &pTempTd);
	  checkExpression (temphr == AAFRESULT_ILLEGAL_VALUE,
					   AAFRESULT_TEST_FAILED);

	  // register variable array of Rational16Pair records
	  IAAFTypeDefVariableArraySP ptdvaarpr;
	  IAAFTypeDefSP ptdarpr;
	  // perform this part only for specified versions
	  if( versionUInt(testVer) >= versionUInt(1,1,1,0) )
	  {
		checkResult (pDict->LookupTypeDef (sTypeId_Rational16_array,&ptdarpr));
		checkResult (ptdarpr->QueryInterface (IID_IAAFTypeDefVariableArray,(void**) &ptdvaarpr));
	  }

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

	  // Read back the value of the Velocity property
	  IAAFPropertyDefSP pPdvelocity;
	  checkResult (defs.cdCompositionMob()->LookupPropertyDef(sPropertyId_velocity, &pPdvelocity));
	  IAAFPropertyValueSP pPVvelocity;
	  checkResult (pObj->GetPropertyValue (pPdvelocity, &pPVvelocity));
	  Mixed_t velocity = {0, 0};
	  checkResult (ptdrmixed->GetStruct (pPVvelocity, (aafMemPtr_t) &velocity, sizeof (velocity)));
	  checkExpression (15 == velocity.angle, AAFRESULT_TEST_FAILED);
	  checkExpression (2001 == velocity.speed, AAFRESULT_TEST_FAILED);

	  // Try to read the first one with GetStruct.
	  Rational16pair_t valA = { {0,0},{0,0} };
	  checkResult (ptdr16p->GetStruct (pPVa,
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
	  Rational16pair_t valB = { {5, 6}, {7, 8} };
	  CheckRational16PairByValues (ptdr16p, ptdr16, pPVb, valB);

	  // Read the last two with GetStruct just to get it over with. ;)
	  Rational16pair_t valC = { {0,0},{0,0} };
	  temphr = ptdr16p->GetStruct (pPVc,
								  (aafMemPtr_t) &valC,
								  sizeof (valC)-1);
	  checkExpression (AAFRESULT_ILLEGAL_VALUE == temphr,
					   AAFRESULT_TEST_FAILED);
	  temphr = ptdr16p->GetStruct (0,
								  (aafMemPtr_t) &valC,
								  sizeof (valC));
	  checkExpression (AAFRESULT_NULL_PARAM == temphr,
					   AAFRESULT_TEST_FAILED);
	  temphr = ptdr16p->GetStruct (pPVc,
								  0,
								  sizeof (valC));
	  checkExpression (AAFRESULT_NULL_PARAM == temphr,
					   AAFRESULT_TEST_FAILED);
	  checkResult (ptdr16p->GetStruct (pPVc,
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

	  // test variable array of records
	  // perform this part only for specified versions
	  if( versionUInt(testVer) >= versionUInt(1,1,1,0) )
	  {
		IAAFPropertyDefSP pPdPosN;
		checkResult (defs.cdCompositionMob()->LookupPropertyDef (sPropertyId_positionN,	&pPdPosN));
		IAAFPropertyValueSP pPVN;
		checkResult (pObj->GetPropertyValue (pPdPosN, &pPVN));

		// get the middle element of the array, 9abc
		IAAFPropertyValueSP pPVN1;
		checkResult (ptdvaarpr->GetElementValue (pPVN, 1, &pPVN1));

		// Read the value with GetStruct
		Rational16pair_t valN1 = { {0,0},{0,0} };
		checkResult (ptdr16p->GetStruct (pPVN1,
										(aafMemPtr_t) &valN1,
										sizeof (valN1)));
		checkExpression (9 == valN1.X_Position.Numerator,
						AAFRESULT_TEST_FAILED);
		checkExpression (10 == valN1.X_Position.Denominator,
						AAFRESULT_TEST_FAILED);
		checkExpression (11 == valN1.Y_Position.Numerator,
						AAFRESULT_TEST_FAILED);
		checkExpression (12 == valN1.Y_Position.Denominator,
						AAFRESULT_TEST_FAILED);
	  }

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

static HRESULT ReadRecordNoStructs (const aafWChar * pFileName, int loadingMode)
{
  HRESULT hr = E_FAIL;
  HRESULT temphr;
  IAAFFileSP   pFile;

  try 
	{
	  // Open the file, and get the dictionary.
	  checkResult(AAFFileOpenExistingRead(pFileName, loadingMode, &pFile));
	  IAAFHeaderSP pHeader;
	  checkResult(pFile->GetHeader(&pHeader));
	  IAAFDictionarySP pDict;
	  checkResult (pHeader->GetDictionary(&pDict));
	  CAAFBuiltinDefs defs(pDict);

	  // get the SDK version against which we are testing
	  aafProductVersion_t			testVer;
	  checkResult(pHeader->GetRefImplVersion(&testVer));

	  // Get the type definitions for our new types.
	  IAAFTypeDefSP ptd;
	  checkResult (pDict->LookupTypeDef (sTypeId_Rational16,
										 &ptd));
	  IAAFTypeDefRecordSP ptdr16;
	  checkResult (ptd->QueryInterface (IID_IAAFTypeDefRecord,
										(void**) &ptdr16));

	  checkResult (pDict->LookupTypeDef (sTypeId_Rational16_pair,
										 &ptd));
	  IAAFTypeDefRecordSP ptdr16p;
	  checkResult (ptd->QueryInterface (IID_IAAFTypeDefRecord,
										(void**) &ptdr16p));

	  // Setup to read the Velocity property which is of typed Mixed_t
	  checkResult (pDict->LookupTypeDef (sTypeId_Mixed, &ptd));
	  IAAFTypeDefRecordSP ptdrmixed;
	  checkResult (ptd->QueryInterface (IID_IAAFTypeDefRecord, (void**) &ptdrmixed));

	  IAAFTypeDef * pTempTd = 0;
	  checkResult (ptdr16->QueryInterface (IID_IAAFTypeDef,
										(void**) &pTempTd));
	  CheckMemberTypeEqual (ptdr16p, 0, pTempTd);
          CheckMemberTypeEqual (ptdr16p, 1, pTempTd);
	  pTempTd->Release ();
	  pTempTd = 0;

	  temphr = ptdr16p->GetMemberType (2, &pTempTd);
	  checkExpression (temphr == AAFRESULT_ILLEGAL_VALUE,
					   AAFRESULT_TEST_FAILED);

	  // register variable array of Rational16Pair records
	  IAAFTypeDefVariableArraySP ptdvaarpr;
	  IAAFTypeDefSP ptdarpr;
	  // perform this part only for specified versions
	  if( versionUInt(testVer) >= versionUInt(1,1,1,0) )
	  {
		checkResult (pDict->LookupTypeDef (sTypeId_Rational16_array,&ptdarpr));
		checkResult (ptdarpr->QueryInterface (IID_IAAFTypeDefVariableArray,(void**) &ptdvaarpr));
	  }

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

	  // Read back the value of the Velocity property
	  IAAFPropertyDefSP pPdvelocity;
	  checkResult (defs.cdCompositionMob()->LookupPropertyDef(sPropertyId_velocity, &pPdvelocity));
	  IAAFPropertyValueSP pPVvelocity;
	  checkResult (pObj->GetPropertyValue (pPdvelocity, &pPVvelocity));

	  IAAFPropertyValueSP pPVvelocityAngle, pPVvelocitySpeed;
	  checkResult (ptdrmixed->GetValue (pPVvelocity, 0, &pPVvelocityAngle));
	  checkResult (ptdrmixed->GetValue (pPVvelocity, 1, &pPVvelocitySpeed));
      
	  Mixed_t velocity = {0, 0};
	  IAAFTypeDefSP pTDmem;
	  IAAFTypeDefIntSP pTDIntMem;
	  checkResult (ptdrmixed->GetMemberType (0, &pTDmem));
	  checkResult (pTDmem->QueryInterface (IID_IAAFTypeDefInt,
                                  (void **) &pTDIntMem));
	  checkResult (pTDIntMem->GetInteger (pPVvelocityAngle, 
                                   (aafMemPtr_t)(&velocity.angle), 
                                   sizeof(velocity.angle)));
	  checkResult (ptdrmixed->GetMemberType (1, &pTDmem));
	  checkResult (pTDmem->QueryInterface (IID_IAAFTypeDefInt,
                                      (void **) &pTDIntMem));
	  checkResult (pTDIntMem->GetInteger (pPVvelocitySpeed, 
	                           (aafMemPtr_t)(&velocity.speed),
	                           sizeof(velocity.speed)));

	  checkExpression (15 == velocity.angle, AAFRESULT_TEST_FAILED);
	  checkExpression (2001 == velocity.speed, AAFRESULT_TEST_FAILED);

	  // Try to read the first one
	  Rational16pair_t valA = { {1, 2}, {3, 4} };
	  CheckRational16PairByValues (ptdr16p, ptdr16, pPVa, valA);


	  // Try to read the second one
	  Rational16pair_t valB = { {5, 6}, {7, 8} };
	  CheckRational16PairByValues (ptdr16p, ptdr16, pPVb, valB);

          // Check misc. error return conditions for TypeDefRecord GetValue
          IAAFPropertyValueSP junkPv;
	  temphr = ptdr16->GetValue (0, 1, &junkPv);
	  checkExpression (AAFRESULT_NULL_PARAM == temphr,
					   AAFRESULT_TEST_FAILED);
	  temphr = ptdr16p->GetValue (pPVb, 1, 0);
	  checkExpression (AAFRESULT_NULL_PARAM == temphr,
					   AAFRESULT_TEST_FAILED);

	  // Read the last two with GetStruct just to get it over with. ;)
	  Rational16pair_t valC = { {9, 10}, {11, 12} };
	  CheckRational16PairByValues (ptdr16p, ptdr16, pPVc, valC);

	  // test variable array of records
	  // perform this part only for specified versions
	  if( versionUInt(testVer) >= versionUInt(1,1,1,0) )
	  {
		IAAFPropertyDefSP pPdPosN;
		checkResult (defs.cdCompositionMob()->LookupPropertyDef (sPropertyId_positionN,	&pPdPosN));
		IAAFPropertyValueSP pPVN;
		checkResult (pObj->GetPropertyValue (pPdPosN, &pPVN));

		// get the middle element of the array, 9abc
		IAAFPropertyValueSP pPVN1;
		checkResult (ptdvaarpr->GetElementValue (pPVN, 1, &pPVN1));

		// Read the value with GetStruct
		Rational16pair_t valN1 = { {9,10},{11,12} };
		CheckRational16PairByValues (ptdr16p, ptdr16, pPVN1, valN1);
	  }

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


extern "C" HRESULT CAAFTypeDefRecord_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CAAFTypeDefRecord_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID)
{
  HRESULT hr = AAFRESULT_TEST_FAILED;
  const size_t fileNameBufLen = 128;
  aafWChar pFileName[ fileNameBufLen ] = L"";
  GenerateTestFileName( productID.productName, fileKind, fileNameBufLen, pFileName );

  try
    {
      if(mode == kAAFUnitTestReadWrite)
		hr =  WriteRecord(pFileName, fileKind, rawStorageType, productID);
	  if (SUCCEEDED (hr) || mode == kAAFUnitTestReadOnly)
		{
		  hr = AAFRESULT_TEST_FAILED;
		  // Read back using only GetValue()-style reading since
		  // default record offsets registered during file loading
		  // may preclude us from being able to register our native
		  // struct representations (see notes in ReadRecord())
		  hr = ReadRecordNoStructs(pFileName, AAF_FILE_MODE_LAZY_LOADING);
		  
		  // Try again with eager loading
		  if (SUCCEEDED(hr))
		      hr = ReadRecordNoStructs(pFileName, AAF_FILE_MODE_LAZY_LOADING);
		  
		  
		  // Test first in lazy loading mode. Lazy loading with SS
		  // avoids default offset registration during file loading,
		  // allowing us to test GetStruct method.
		  if (SUCCEEDED(hr))
		      hr = ReadRecord(pFileName, AAF_FILE_MODE_LAZY_LOADING);

		  // Again with eager loading
		  if (SUCCEEDED(hr))
		      hr = ReadRecord(pFileName, 0);
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

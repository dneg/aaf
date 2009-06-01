// @doc INTERNAL
// @com This file implements the module test for CEnumAAFParameters
//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include <iostream>
using namespace std;
#include <stdio.h>
#include <wchar.h>

#include "AAF.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFSmartPointer.h"

#include "CEnumeratorTest.h"

typedef IAAFSmartPointer<IAAFConstantValue> IAAFConstantValueSP;
typedef IAAFSmartPointer<IAAFOperationDef> IAAFOperationDefSP;
typedef IAAFSmartPointer<IAAFParameter> IAAFParameterSP;
typedef IAAFSmartPointer<IAAFParameterDef> IAAFParameterDefSP;

#define NUM_TEST_PARAMETERS 10

static const aafUID_t gpTestParameterIDs[NUM_TEST_PARAMETERS]={
	// {3F821DE1-4915-11d4-921E-0050049C3B9D}
	{ 0x3f821de1, 0x4915, 0x11d4, 
		{ 0x92, 0x1e, 0x0, 0x50, 0x4, 0x9c, 0x3b, 0x9d } },
	// {3F821DE2-4915-11d4-921E-0050049C3B9D}
	{ 0x3f821de2, 0x4915, 0x11d4, 
		{ 0x92, 0x1e, 0x0, 0x50, 0x4, 0x9c, 0x3b, 0x9d } },
	// {3F821DE3-4915-11d4-921E-0050049C3B9D}
	{ 0x3f821de3, 0x4915, 0x11d4, 
		{ 0x92, 0x1e, 0x0, 0x50, 0x4, 0x9c, 0x3b, 0x9d } },
	// {3F821DE4-4915-11d4-921E-0050049C3B9D}
	{ 0x3f821de4, 0x4915, 0x11d4, 
		{ 0x92, 0x1e, 0x0, 0x50, 0x4, 0x9c, 0x3b, 0x9d } },
	// {3F821DE5-4915-11d4-921E-0050049C3B9D}
	{ 0x3f821de5, 0x4915, 0x11d4, 
		{ 0x92, 0x1e, 0x0, 0x50, 0x4, 0x9c, 0x3b, 0x9d } },
	// {3F821DE6-4915-11d4-921E-0050049C3B9D}
	{ 0x3f821de6, 0x4915, 0x11d4, 
		{ 0x92, 0x1e, 0x0, 0x50, 0x4, 0x9c, 0x3b, 0x9d } },
	// {3F821DE7-4915-11d4-921E-0050049C3B9D}
	{ 0x3f821de7, 0x4915, 0x11d4, 
		{ 0x92, 0x1e, 0x0, 0x50, 0x4, 0x9c, 0x3b, 0x9d } },
	// {3F821DE8-4915-11d4-921E-0050049C3B9D}
	{ 0x3f821de8, 0x4915, 0x11d4, 
		{ 0x92, 0x1e, 0x0, 0x50, 0x4, 0x9c, 0x3b, 0x9d } },
	// {3F821DE9-4915-11d4-921E-0050049C3B9D}
	{ 0x3f821de9, 0x4915, 0x11d4, 
		{ 0x92, 0x1e, 0x0, 0x50, 0x4, 0x9c, 0x3b, 0x9d } },
	// {3F821DEA-4915-11d4-921E-0050049C3B9D}
	{ 0x3f821dea, 0x4915, 0x11d4,
		{ 0x92, 0x1e, 0x0, 0x50, 0x4, 0x9c, 0x3b, 0x9d } }
};

// {3F821DE0-4915-11d4-921E-0050049C3B9D}
static const aafUID_t kTestEffectID = 
{ 0x3f821de0, 0x4915, 0x11d4, { 0x92, 0x1e, 0x0, 0x50, 0x4, 0x9c, 0x3b, 0x9d } };

// {3D887451-4916-11d4-921E-0050049C3B9D}
static const aafUID_t kTestCategoryID = 
{ 0x0d010102, 0x0101, 0x0100, { 0x06, 0x0e, 0x2b, 0x34, 0x04, 0x01, 0x01, 0x01 } };

class CEnumAAFParametersTest: public 
	CEnumeratorTest<IEnumAAFParameters,IAAFParameter>
{
public:
	CEnumAAFParametersTest()
	{
		_pOperationGroup=0;
	}
	// Helper function to create operation group on which to test enumerator
	void CreateOperationGroup(IAAFDictionary *pDictionary);
	HRESULT CountItems(IAAFDictionary *pDictionary,aafUInt32 *piCount)
	{
		CreateOperationGroup(pDictionary);
		return(_pOperationGroup->CountParameters(piCount));
	}
	HRESULT GetItems(IAAFDictionary *pDictionary,
						IEnumAAFParameters **ppEnumerator)
	{
		CreateOperationGroup(pDictionary);
		return(_pOperationGroup->GetParameters(ppEnumerator));
	}
	aafBool ItemIsPresent(IAAFDictionary *pDictionary,aafUID_t& Id)
	{
		CreateOperationGroup(pDictionary);
		IAAFSmartPointer<IAAFParameter> pParameter;
		return(_pOperationGroup->LookupParameter(Id,&pParameter)
			==AAFRESULT_SUCCESS?kAAFTrue:kAAFFalse);
	}
	virtual ~CEnumAAFParametersTest()
	{
		if(_pOperationGroup)
			_pOperationGroup->Release();
	}

private:
	void GetAUID(IAAFParameter *pParameter,aafUID_t *pAUID);
	IAAFOperationGroup *_pOperationGroup;
};

// Before performing any testing, we create an operation group, so we can perform 
// our test by enumerating over its parameters.
void CEnumAAFParametersTest::CreateOperationGroup(IAAFDictionary *pDictionary)
{
	if(_pOperationGroup)
		return;

	CAAFBuiltinDefs defs (pDictionary);

	// Create operation definition
	IAAFOperationDefSP pOperationDef;
	checkResult(defs.cdOperationDef()->CreateInstance(
		IID_IAAFOperationDef,(IUnknown **)&pOperationDef));

	checkResult(pOperationDef->Initialize (kTestEffectID,L"An effect name",
		L"An effect description"));

	checkResult(pOperationDef->SetDataDef(defs.ddkAAFPicture()));
	checkResult(pOperationDef->SetIsTimeWarp (kAAFFalse));
	checkResult(pOperationDef->SetNumberInputs(3));
	checkResult(pOperationDef->SetCategory(kTestCategoryID));
	checkResult(pOperationDef->SetBypass(1));

	// Add parameters to operation definition
	aafUInt32 n;
	IAAFParameterDefSP pParameterDefs[NUM_TEST_PARAMETERS];
	for(n=0;n<NUM_TEST_PARAMETERS;n++)
	{
		checkResult(defs.cdParameterDef()->CreateInstance(IID_IAAFParameterDef,
			(IUnknown **)&(pParameterDefs[n])));	
		checkResult(pParameterDefs[n]->Initialize(gpTestParameterIDs[n],
			L"A Parameter",L"A Description",defs.tdRational()));
		checkResult(pParameterDefs[n]->SetDisplayUnits(L"Some units"));		
		checkResult(pDictionary->RegisterParameterDef(pParameterDefs[n]));
		checkResult(pOperationDef->AddParameterDef(pParameterDefs[n]));
	}

	checkResult(pDictionary->RegisterOperationDef(pOperationDef));

	// Create & initialize operation group
	checkResult(defs.cdOperationGroup()->CreateInstance(
		IID_IAAFOperationGroup,(IUnknown **)&_pOperationGroup));
	checkResult(_pOperationGroup->Initialize(defs.ddkAAFPicture(),60,
		pOperationDef));

	// Add parameters to operation group
	for(n=0;n<NUM_TEST_PARAMETERS;n++)
	{
		IAAFParameterSP pParameter;
		IAAFConstantValueSP pConstantValue;
		aafRational_t testLevel = {1,2};

		checkResult(defs.cdConstantValue()->CreateInstance(
			IID_IAAFConstantValue,(IUnknown **)&pConstantValue));
		checkResult(pConstantValue->Initialize(pParameterDefs[n],
			sizeof(testLevel),(aafDataBuffer_t)&testLevel));
		checkResult(pConstantValue->SetValue(sizeof(testLevel),
			(aafDataBuffer_t)&testLevel));
		checkResult(pConstantValue->QueryInterface(IID_IAAFParameter,
			(void **)&pParameter));

		checkResult(_pOperationGroup->AddParameter(pParameter));
	}
}

void CEnumAAFParametersTest::GetAUID(IAAFParameter *pParameter,aafUID_t *pAUID)
{
	IAAFParameterDefSP pParameterDef;
	checkResult(pParameter->GetParameterDefinition(&pParameterDef));
	IAAFDefObjectSP pDefObject;
	checkResult(pParameterDef->QueryInterface(IID_IAAFDefObject,
		(void**)&pDefObject));
	checkResult(pDefObject->GetAUID(pAUID));
}

extern "C" HRESULT CEnumAAFParameters_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CEnumAAFParameters_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID)
{
	try
	{
		CEnumAAFParametersTest Test;
  		if(mode == kAAFUnitTestReadWrite)
			Test.Run(mode, fileKind, rawStorageType, productID); // !!! This test requires create & verify intermixed
	}
	catch(HRESULT& rResult)
	{
		return(rResult);
	}

	return AAFRESULT_SUCCESS;
}

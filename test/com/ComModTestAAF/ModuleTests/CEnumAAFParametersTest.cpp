// @doc INTERNAL
// @com This file implements the module test for CEnumAAFParameters
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
 *  prior written permission of Avid Technology, Inc.
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

#include <iostream.h>
#include <stdio.h>
#include <wchar.h>

#include "AAF.h"
#include "AAFResult.h"
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
{ 0x3d887451, 0x4916, 0x11d4, { 0x92, 0x1e, 0x0, 0x50, 0x4, 0x9c, 0x3b, 0x9d } };

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
	~CEnumAAFParametersTest()
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

	checkResult(pOperationDef->SetDataDef(defs.ddPicture()));
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
	checkResult(_pOperationGroup->Initialize(defs.ddPicture(),60,
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

extern "C" HRESULT CEnumAAFParameters_test()
{
	try
	{
		CEnumAAFParametersTest Test;
		Test.Run();
	}
	catch(HRESULT& rResult)
	{
		return(rResult);
	}

	return AAFRESULT_SUCCESS;
}

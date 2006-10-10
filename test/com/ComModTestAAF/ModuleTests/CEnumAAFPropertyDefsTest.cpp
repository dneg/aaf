// @doc INTERNAL
// @com This file implements the module test for CEnumAAFPropertyDefs
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
#include <wchar.h>

#include "AAF.h"
#include "AAFResult.h"
#include "ModuleTest.h"

#include "CEnumeratorTest.h"

class CEnumAAFPropertyDefsTest: public 
	CEnumeratorTest<IEnumAAFPropertyDefs,IAAFPropertyDef>
{
public:
	CEnumAAFPropertyDefsTest()
	{
		_pKnownClassDef=0;
	}
	// Before performing any testing, we loop up a known class definition, so we
	// can perform our test by enumerating over its properties.
	void LookupKnownClassDef(IAAFDictionary *pDictionary)
	{
		if(!_pKnownClassDef)
			checkResult(pDictionary->LookupClassDef(AUID_AAFTapeDescriptor,
			&_pKnownClassDef));
	}
	HRESULT CountItems(IAAFDictionary *pDictionary,aafUInt32 *piCount)
	{
		LookupKnownClassDef(pDictionary);
		return(_pKnownClassDef->CountPropertyDefs(piCount));
	}
	HRESULT GetItems(IAAFDictionary *pDictionary,
						IEnumAAFPropertyDefs **ppEnumerator)
	{
		LookupKnownClassDef(pDictionary);
		return(_pKnownClassDef->GetPropertyDefs(ppEnumerator));
	}
	aafBool ItemIsPresent(IAAFDictionary *pDictionary,aafUID_t& Id)
	{
		LookupKnownClassDef(pDictionary);
		IAAFSmartPointer<IAAFPropertyDef> pPropertyDef;
		return(_pKnownClassDef->LookupPropertyDef(Id,&pPropertyDef)
			==AAFRESULT_SUCCESS?kAAFTrue:kAAFFalse);
	}
	~CEnumAAFPropertyDefsTest()
	{
		if(_pKnownClassDef)
			_pKnownClassDef->Release();
	}

private:
	IAAFClassDef *_pKnownClassDef;
};

extern "C" HRESULT CEnumAAFPropertyDefs_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);
extern "C" HRESULT CEnumAAFPropertyDefs_test(
    testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID)
{
	try
	{
		CEnumAAFPropertyDefsTest Test;
  		if(mode == kAAFUnitTestReadWrite)
			Test.Run(mode, fileKind, rawStorageType, productID);		// !!! This test requires create & verify intermixed
	}
	catch(HRESULT& rResult)
	{
		return(rResult);
	}

	return AAFRESULT_SUCCESS;
}






// @doc INTERNAL
// @com This file implements the module test for CEnumAAFClassDefs
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

#include <iostream>
using namespace std;
#include <stdio.h>
#include <wchar.h>
#include "AAF.h"
#include "AAFResult.h"
#include "ModuleTest.h"

#include "CEnumeratorTest.h"

class CEnumAAFClassDefsTest: public CEnumeratorTest<IEnumAAFClassDefs,IAAFClassDef>
{
public:
	HRESULT CountItems(IAAFDictionary *pDictionary,aafUInt32 *piCount)
	{
		return(pDictionary->CountClassDefs(piCount));
	}
	HRESULT GetItems(IAAFDictionary *pDictionary,IEnumAAFClassDefs **ppEnumerator)
	{
		return(pDictionary->GetClassDefs(ppEnumerator));
	}
	aafBool ItemIsPresent(IAAFDictionary *pDictionary,aafUID_t& Id)
	{
		IAAFClassDefSP pClassDef;
		return(pDictionary->LookupClassDef(Id,&pClassDef)==AAFRESULT_SUCCESS?
			kAAFTrue:kAAFFalse);
	}
};

extern "C" HRESULT CEnumAAFClassDefs_test(testMode_t mode);
extern "C" HRESULT CEnumAAFClassDefs_test(testMode_t mode)
{
	try
	{
		CEnumAAFClassDefsTest Test;
		Test.Run(mode);
	}
	catch(HRESULT& rResult)
	{
		return(rResult);
	}

	return AAFRESULT_SUCCESS;
}

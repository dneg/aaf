// @doc INTERNAL
// @com This file implements the module test for CEnumAAFPropertyDefs
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

extern "C" HRESULT CEnumAAFPropertyDefs_test(testMode_t mode);
extern "C" HRESULT CEnumAAFPropertyDefs_test(testMode_t mode)
{
	try
	{
		CEnumAAFPropertyDefsTest Test;
  		if(mode == kAAFUnitTestReadWrite)
			Test.Run(mode);		// !!! This test requires create & verify intermixed
	}
	catch(HRESULT& rResult)
	{
		return(rResult);
	}

	return AAFRESULT_SUCCESS;
}






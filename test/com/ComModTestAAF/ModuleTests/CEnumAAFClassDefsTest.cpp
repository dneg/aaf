// @doc INTERNAL
// @com This file implements the module test for CEnumAAFClassDefs
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

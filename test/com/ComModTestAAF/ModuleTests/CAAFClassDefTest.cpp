// @doc INTERNAL
// @com This file implements the module test for CAAFClassDef
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

#include "AAF.h"
#include "AAFResult.h"
#include "AAFSmartPointer.h"
#include "AAFDefUIDs.h"
#include "AAFTypeDefUIDs.h"
#include "AAFStoredObjectIDs.h"

#include <iostream.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

typedef IAAFSmartPointer<IAAFClassDef>             IAAFClassDefSP;
typedef IAAFSmartPointer<IAAFDictionary>           IAAFDictionarySP;
typedef IAAFSmartPointer<IAAFFile>                 IAAFFileSP;
typedef IAAFSmartPointer<IAAFHeader>               IAAFHeaderSP;
typedef IAAFSmartPointer<IAAFPropertyDef>          IAAFPropertyDefSP;
typedef IAAFSmartPointer<IAAFTypeDef>              IAAFTypeDefSP;
typedef IAAFSmartPointer<IEnumAAFPropertyDefs>     IEnumAAFPropertyDefsSP;

// Structure used internally by class CAAFModTestLog below
class CAAFModTestLogEntry
{
public:
	const char *_pName;
	aafBool _bTested,_bTestPassed;
};

// The template class CAAFModTestLog provides a way to keep track of which methods in a class have been
// tested, and whether the tests have passed or failed.  The parameter class T should be set to an 
// enumeration type giving a named index for each method to be tested (see usage in CAAFClassDef_test() 
// below).
template<class T>
class CAAFModTestLog
{
public:
	CAAFModTestLog(int iMethods,const char **ppMethodNames);
	// Mark a specific method as having been tested.  Note that T is always effectively an integer type.
	void MarkAsTested(T Method)
	{
		_pLog[(int)Method]._bTested=kAAFTrue;
	}
	// Mark a specific method's test as having failed.
	void MarkTestFailed(T Method)
	{
		_pLog[(int)Method]._bTestPassed=kAAFFalse;
	}
	void PrintSummary(void);
	int MethodsFailed();
	int MethodsNotTested();

private:
	int _iMethods;
	CAAFModTestLogEntry *_pLog;
};

// CAAFModTestLog allocates array of CAAFModTestLogEntry in which to keep track of tests.
template <class T>
CAAFModTestLog<T>::CAAFModTestLog(int iMethods,const char **ppMethodNames)
{
	_iMethods=iMethods;
	_pLog=new CAAFModTestLogEntry[_iMethods];
	for(int n=0;n<_iMethods;n++)
	{
		_pLog[n]._bTested=kAAFFalse;
		_pLog[n]._bTestPassed=kAAFTrue;
		_pLog[n]._pName=ppMethodNames[n];
	}
}

// PrintSummary() prints a summary of the results of the tests that were carried out for the
// methods of an interface -- which methods passed, which methods failed, and (possibly) which
// methods could not be tested due to earlier failures.
template <class T>
void CAAFModTestLog<T>::PrintSummary()
{
	// Print out names of all methods (if any) which were not tested
	for(int n=0;n<_iMethods;n++)
	{
		if(_pLog[n]._bTested==kAAFFalse)
		{
			cout << "     " << _pLog[n]._pName << "() was not tested.\n";
		}
	}

	// Print out names of all methods (if any) whose tests failed
	for(n=0;n<_iMethods;n++)
	{
		if(_pLog[n]._bTested==kAAFTrue && _pLog[n]._bTestPassed==kAAFFalse)
		{
			cout << "     " << _pLog[n]._pName << "() FAILED!\n";
		}
	}

	// Flush cout stream -- useful for single stepping
	cout.flush();
}

// Return number of method tests which have been marked as having failed.
template<class T>
int CAAFModTestLog<T>::MethodsFailed()
{
	int iMethodsFailed=0;
	for(int n=0;n<_iMethods;n++)
	{
		if(_pLog[n]._bTested==kAAFTrue && _pLog[n]._bTestPassed==kAAFFalse)
			iMethodsFailed++;
	}
	return(iMethodsFailed);
}

// Return number of method tests which have not yet been marked as having been completed.
template<class T>
int CAAFModTestLog<T>::MethodsNotTested()
{
	int iMethodsNotTested=0;
	for(int n=0;n<_iMethods;n++)
	{
		if(_pLog[n]._bTested==kAAFFalse)
			iMethodsNotTested++;
	}
	return(iMethodsNotTested);
}

// Enumeration of methods we will test in IAFFClassDef
enum IAAFClassDefMethodsEnum
{
	REGISTER_NEW_PROPERTY_DEF=0,
	REGISTER_OPTIONAL_PROPERTY_DEF,
	COUNT_PROPERTY_DEFS,
	GET_NAME,
	GET_NAME_BUF_LEN,
	GET_PARENT,
	GET_PROPERTY_DEFS,
	INITIALIZE,
	LOOKUP_PROPERTY_DEF,
	NUM_IAAFCLASSDEF_METHODS
};

// Array of names of methods in IAFFClassDef -- constants in enum IAFFClassDefMethodsEnum are indices
// into this array.
char *ppIAAFMethodNames[NUM_IAAFCLASSDEF_METHODS]=
{
	"RegisterNewPropertyDef",
	"RegisterOptionalPropertyDef",
	"CountPropertyDefs",
	"GetName",
	"GetNameBufLen",
	"GetParent",
	"GetPropertyDefs",
	"Initialize",
	"LookupPropertyDef",
};

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


static void ClassDefTest(CAAFModTestLog<IAAFClassDefMethodsEnum>& Log)
{
	aafProductIdentification_t ProductInfo;
	IAAFFileSP       pFile;
	IAAFHeaderSP     pHeader;
	IAAFDictionarySP pDict;

	// Various GUIDs we will use in this test

	// {135A6380-30C3-11d4-9211-0050049C3B9D}
	static const aafUID_t kClassAUID_NewClass = 
	{ 0x135a6380, 0x30c3, 0x11d4, { 0x92, 0x11, 0x0, 0x50, 0x4, 0x9c, 0x3b, 0x9d} };

	// {CADEF283-6D3C-11d3-8449-00600832ACB8}
	static const aafUID_t ourPid1 = 
	{ 0xcadef283, 0x6d3c, 0x11d3, { 0x84, 0x49, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

	// {CADEF284-6D3C-11d3-8449-00600832ACB8}
	static const aafUID_t ourPid2 = 
	{ 0xcadef284, 0x6d3c, 0x11d3, { 0x84, 0x49, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

	// {CADEF285-6D3C-11d3-8449-00600832ACB8}
	static const aafUID_t ourPid3 = 
	{ 0xcadef285, 0x6d3c, 0x11d3, { 0x84, 0x49, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

	// {CADEF286-6D3C-11d3-8449-00600832ACB8}
	static const aafUID_t ourPid4 = 
	{ 0xcadef286, 0x6d3c, 0x11d3, { 0x84, 0x49, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

	// {CADEF287-6D3C-11d3-8449-00600832ACB8}
	static const aafUID_t ourPid5 = 
	{ 0xcadef287, 0x6d3c, 0x11d3, { 0x84, 0x49, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

	static wchar_t testFileName[] = L"AAFClassDef.aaf";

	aafProductVersion_t v;
	v.major = 1;
	v.minor = 0;
	v.tertiary = 0;
	v.patchLevel = 0;
	v.type = kAAFVersionUnknown;
	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFClassDef Test";
	ProductInfo.productVersion = &v;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = UnitTestProductID;
	ProductInfo.platform = NULL;

	RemoveTestFile (testFileName);
	assert(AAFFileOpenNewModify(testFileName, 0, &ProductInfo, &pFile)
		==AAFRESULT_SUCCESS);
	assert (pFile);

	assert(pFile->GetHeader (&pHeader)==AAFRESULT_SUCCESS);
	assert (pHeader);

	assert(pHeader->GetDictionary (&pDict)==AAFRESULT_SUCCESS);
	assert (pDict);
	IAAFTypeDefSP ptd;
	assert(pDict->LookupTypeDef (kAAFTypeID_UInt8, &ptd)==AAFRESULT_SUCCESS);
	
	// Try to extend a typeDefInt.  Should fail.
	IAAFClassDefSP badClass1;
	assert(pDict->LookupClassDef (AUID_AAFTypeDefInt, &badClass1)
		==AAFRESULT_SUCCESS);
	
	IAAFPropertyDefSP propDef1;
	if(badClass1->RegisterOptionalPropertyDef (ourPid1,L"First prop",ptd,
		&propDef1)!=AAFRESULT_NOT_EXTENDABLE)
		Log.MarkTestFailed(REGISTER_OPTIONAL_PROPERTY_DEF);
	
	// Try to extend an AAFObject.  Should fail, for now at least.
	IAAFClassDefSP badClass2;
	assert(pDict->LookupClassDef (AUID_AAFObject, &badClass2)==AAFRESULT_SUCCESS);
	
	IAAFPropertyDefSP propDef2;
	if(badClass2->RegisterOptionalPropertyDef (ourPid2,L"Second prop",ptd,
		&propDef2)!=AAFRESULT_NOT_EXTENDABLE)
		Log.MarkTestFailed(REGISTER_OPTIONAL_PROPERTY_DEF);

	// Try to extend an AAFSequence.  Should succeed.
	IAAFClassDefSP goodClass;
	assert(pDict->LookupClassDef (AUID_AAFSequence, &goodClass)
		==AAFRESULT_SUCCESS);
	IAAFPropertyDefSP propDef3;
	if(goodClass->RegisterOptionalPropertyDef (ourPid3,L"Third prop",ptd,
		&propDef3)!=AAFRESULT_SUCCESS)
		Log.MarkTestFailed(REGISTER_OPTIONAL_PROPERTY_DEF);

	// We should not be able to add a new property to the class definition for AAFSequence
	if(goodClass->RegisterNewPropertyDef (ourPid3,L"Third prop",ptd,kAAFFalse,kAAFFalse,&propDef3)
		==AAFRESULT_SUCCESS)
		Log.MarkTestFailed(REGISTER_NEW_PROPERTY_DEF);

	// Create a new class definition

	// Look up a known class for the parent
	IAAFClassDefSP pParentClass;
    assert(pDict->LookupClassDef(AUID_AAFFiller,&pParentClass)==AAFRESULT_SUCCESS);
			
	IAAFClassDefSP pNewClass;
	assert(pDict->CreateMetaInstance(AUID_AAFClassDef,IID_IAAFClassDef,
		(IUnknown **)&pNewClass)==AAFRESULT_SUCCESS);
	assert(pNewClass);

	static aafCharacter_constptr pNewClassName=L"New Class";
	if(pNewClass->Initialize(kClassAUID_NewClass,pParentClass,pNewClassName,
		kAAFTrue)!=AAFRESULT_SUCCESS)
		Log.MarkTestFailed(INITIALIZE);

	Log.MarkAsTested(INITIALIZE);
	
	// Make sure GetName() and GetNameBufLen() return correct values
	aafUInt32 iNameBufLen=-1;
	if(pNewClass->GetNameBufLen(&iNameBufLen)!=S_OK)
		Log.MarkTestFailed(GET_NAME_BUF_LEN);
	if(iNameBufLen!=2*(wcslen(pNewClassName)+1))
		Log.MarkTestFailed(GET_NAME_BUF_LEN);

	Log.MarkAsTested(GET_NAME_BUF_LEN);
	
	aafCharacter pReturnedClassName[100];
	if(pNewClass->GetName(pReturnedClassName,100)!=S_OK)
		Log.MarkTestFailed(GET_NAME);
	if(wcscmp(pReturnedClassName,pNewClassName))
		Log.MarkTestFailed(GET_NAME);

	Log.MarkAsTested(GET_NAME);
	
	// Make sure GetParent() returns correct result
	if(pNewClass->GetParent(0)!=AAFRESULT_NULL_PARAM)
		Log.MarkTestFailed(GET_PARENT);
	IAAFClassDefSP pReturnedParent;
	if(pNewClass->GetParent(&pReturnedParent)!=AAFRESULT_SUCCESS)
		Log.MarkTestFailed(GET_PARENT);
	if(pReturnedParent!=pParentClass)
		Log.MarkTestFailed(GET_PARENT);
	
	Log.MarkAsTested(GET_PARENT);
	
	// CountPropertyDefs() should return 0 for new class definition
	aafUInt32 iPropertyDefs=-1;
	if(pNewClass->CountPropertyDefs(&iPropertyDefs)!=S_OK)
		Log.MarkTestFailed(COUNT_PROPERTY_DEFS);
	if(iPropertyDefs!=0)
		Log.MarkTestFailed(COUNT_PROPERTY_DEFS);
		
	// GetPropertyDefs() should return empty enumerator
	IEnumAAFPropertyDefsSP pEnumerator;
	if(pNewClass->GetPropertyDefs(&pEnumerator)!=AAFRESULT_SUCCESS)
	{
		Log.MarkTestFailed(GET_PROPERTY_DEFS);
	}
	else
	{
		// If enumerator is empty, first call to NextOne() should fail
		IAAFPropertyDefSP pPropertyDef;
		if(pEnumerator->NextOne(&pPropertyDef)==AAFRESULT_SUCCESS)
			Log.MarkTestFailed(GET_PROPERTY_DEFS);
	}

	// Add a new property to our new class definition
	IAAFPropertyDefSP propDef4;
	if(pNewClass->RegisterNewPropertyDef(ourPid4,L"Fourth prop",ptd,kAAFFalse,
		kAAFFalse,&propDef4)!=AAFRESULT_SUCCESS)
		Log.MarkTestFailed(REGISTER_NEW_PROPERTY_DEF);

	Log.MarkAsTested(REGISTER_NEW_PROPERTY_DEF);

	// CountPropertyDefs() should now return 1
	iPropertyDefs=-1;
	if(pNewClass->CountPropertyDefs(&iPropertyDefs)!=S_OK)
		Log.MarkTestFailed(COUNT_PROPERTY_DEFS);
	if(iPropertyDefs!=1)
		Log.MarkTestFailed(COUNT_PROPERTY_DEFS);

	// Add an optional property to our new class definition
	IAAFPropertyDefSP propDef5;
	if(pNewClass->RegisterOptionalPropertyDef(ourPid5,L"Fifth prop",ptd,&propDef5)
		!=AAFRESULT_SUCCESS)
		Log.MarkTestFailed(REGISTER_OPTIONAL_PROPERTY_DEF);

	Log.MarkAsTested(REGISTER_OPTIONAL_PROPERTY_DEF);
	
	// CountPropertyDefs() should now return 2
	iPropertyDefs=-1;
	if(pNewClass->CountPropertyDefs(&iPropertyDefs)!=S_OK)
		Log.MarkTestFailed(COUNT_PROPERTY_DEFS);
	if(iPropertyDefs!=2)
		Log.MarkTestFailed(COUNT_PROPERTY_DEFS);

	Log.MarkAsTested(COUNT_PROPERTY_DEFS);

	// GetPropertyDefs() should now return enumerator with 2 elements
	if(pNewClass->GetPropertyDefs(&pEnumerator)!=AAFRESULT_SUCCESS)
	{
		Log.MarkTestFailed(GET_PROPERTY_DEFS);
	}
	else
	{
		int iProperties=0;
		IAAFPropertyDefSP pPropertyDef;
		while(pEnumerator->NextOne(&pPropertyDef)==AAFRESULT_SUCCESS)
			iProperties++;
		if(iProperties!=2)
			Log.MarkTestFailed(GET_PROPERTY_DEFS);
	}

	Log.MarkAsTested(GET_PROPERTY_DEFS);
	
	// Use LookupPropertyDef() to look up the new property we created
	IAAFPropertyDefSP pReturnedProperty;
	if(pNewClass->LookupPropertyDef(ourPid4,&pReturnedProperty)!=AAFRESULT_SUCCESS)
	{
		Log.MarkTestFailed(LOOKUP_PROPERTY_DEF);
	}
	else
	{
		if(pReturnedProperty!=propDef4)
			Log.MarkTestFailed(LOOKUP_PROPERTY_DEF);
	}

	// Use LookupPropertyDef() to look up the optional property we created
	if(pNewClass->LookupPropertyDef(ourPid5,&pReturnedProperty)!=AAFRESULT_SUCCESS)
	{
		Log.MarkTestFailed(LOOKUP_PROPERTY_DEF);
	}
	else
	{
		if(pReturnedProperty!=propDef5)
			Log.MarkTestFailed(LOOKUP_PROPERTY_DEF);
	}

	Log.MarkAsTested(LOOKUP_PROPERTY_DEF);

	assert(pFile->Save()==AAFRESULT_SUCCESS);
	assert(pFile->Close()==AAFRESULT_SUCCESS);
}

extern "C" HRESULT CAAFClassDef_test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;

	// Create test log
	CAAFModTestLog<IAAFClassDefMethodsEnum> Log(NUM_IAAFCLASSDEF_METHODS,
		(const char**)ppIAAFMethodNames);

	// Execute test, catching any exceptions
	try
	{
		ClassDefTest(Log);
	}
	catch (...)
	{
		cout << "CAAFClassDef_test"
			<< "...Caught general C++ exception!" << endl; 
	}

	// Print test summary
	Log.PrintSummary();

	// Return appropriate test code
	if(Log.MethodsFailed())
		return(AAFRESULT_TEST_FAILED);
	else if(Log.MethodsNotTested())
		return(AAFRESULT_TEST_PARTIAL_SUCCESS);
	else
		return(AAFRESULT_SUCCESS);
}

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
#include "AAFSmartPointer.h"
#include "AAFDefUIDs.h"
#include "AAFTypeDefUIDs.h"
#include "AAFStoredObjectIDs.h"

#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>

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
	int n;
	for(n=0;n<_iMethods;n++)
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

typedef CAAFModTestLog<IAAFClassDefMethodsEnum> CAAFClassDefTestLog;

// Various GUIDs we will use in this test

// {CADEF286-6D3C-11d3-8449-00600832ACB8}
static const aafUID_t newPropertyPid = 
{ 0xcadef286, 0x6d3c, 0x11d3, { 0x84, 0x49, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

// {CADEF287-6D3C-11d3-8449-00600832ACB8}
static const aafUID_t optionalPropertyPid = 
{ 0xcadef287, 0x6d3c, 0x11d3, { 0x84, 0x49, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

// {135A6380-30C3-11d4-9211-0050049C3B9D}
static const aafUID_t kClassAUID_NewClass = 
{ 0x135a6380, 0x30c3, 0x11d4, { 0x92, 0x11, 0x0, 0x50, 0x4, 0x9c, 0x3b, 0x9d} };

// Names for new class definition and properties we will create
static aafCharacter_constptr pNewClassName=L"New Class";
static aafCharacter_constptr pNewPropertyName=L"New Property";
static aafCharacter_constptr pOptionalPropertyName=L"Optional Property";

// Use existing class AAFFiller as parent for the new class definition we will 
// create
static aafUID_t AUID_ParentClass=AUID_AAFFiller;

static wchar_t testFileName[] = L"AAFClassDef.aaf";

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

// convenient error handlers
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

// Function to compare COM interface pointers, taken from 
// CAAFTypeDefFixedArrayTest.cpp.
template <class T1, class T2>
aafBoolean_t  AreUnksSame(T1& cls1, T2& cls2)
{
	IAAFSmartPointer<IUnknown>    spUnk1, spUnk2;
	
	checkResult(cls1->QueryInterface(IID_IUnknown, (void **)&spUnk1));
	checkResult(cls2->QueryInterface(IID_IUnknown, (void **)&spUnk2));
	
	if (spUnk1 == spUnk2)
		return kAAFTrue;
	else
		return kAAFFalse;
}

// This function is called to verify that an AAF file, either created in memory
// or read from disk, contains the data we expect it to
static void VerifyAAFFile(CAAFClassDefTestLog& Log,IAAFFileSP pFile)
{
	IAAFDictionarySP pDict;
	IAAFHeaderSP     pHeader;

	checkExpression(pFile!=0);
	checkResult(pFile->GetHeader(&pHeader));
	checkExpression(pHeader!=0);
	checkResult(pHeader->GetDictionary(&pDict));
	checkExpression(pDict!=0);

	// Look up the new class we created
	IAAFClassDefSP pNewClass;
	checkResult(pDict->LookupClassDef(kClassAUID_NewClass,&pNewClass));
	checkExpression(pNewClass!=0);

	// CountPropertyDefs() should now return 2
	aafUInt32 iPropertyDefs=-1;
	if(pNewClass->CountPropertyDefs(&iPropertyDefs)!=S_OK)
		Log.MarkTestFailed(COUNT_PROPERTY_DEFS);
	if(iPropertyDefs!=2)
		Log.MarkTestFailed(COUNT_PROPERTY_DEFS);

	// GetPropertyDefs() should now return enumerator with 2 elements
	IEnumAAFPropertyDefsSP pEnumerator;
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

	// Use LookupPropertyDef() to look up the new property we created
	IAAFPropertyDefSP pReturnedProperty;
	if(pNewClass->LookupPropertyDef(newPropertyPid,&pReturnedProperty)
		!=AAFRESULT_SUCCESS)
	{
		Log.MarkTestFailed(LOOKUP_PROPERTY_DEF);
	}
	else
	{
		// Returned property should have the same name it did when we created it
		wchar_t propertyName[100];
		if(pReturnedProperty->GetName(propertyName,100*sizeof(wchar_t))
			!=AAFRESULT_SUCCESS)
			Log.MarkTestFailed(LOOKUP_PROPERTY_DEF);
		else if(wcscmp(propertyName,pNewPropertyName))
			Log.MarkTestFailed(LOOKUP_PROPERTY_DEF);
	}

	// Make sure GetName() and GetNameBufLen() return correct values
	aafUInt32 iNameBufLen=-1;
	if(pNewClass->GetNameBufLen(&iNameBufLen)!=S_OK)
		Log.MarkTestFailed(GET_NAME_BUF_LEN);
	if(iNameBufLen!=sizeof(wchar_t)*(wcslen(pNewClassName)+1))
		Log.MarkTestFailed(GET_NAME_BUF_LEN);

	aafCharacter pReturnedClassName[100];
	if(pNewClass->GetName(pReturnedClassName,100)!=S_OK)
		Log.MarkTestFailed(GET_NAME);
	if(wcscmp(pReturnedClassName,pNewClassName))
		Log.MarkTestFailed(GET_NAME);

	// Use LookupPropertyDef() to look up the optional property we created
	if(pNewClass->LookupPropertyDef(optionalPropertyPid,&pReturnedProperty)
		!=AAFRESULT_SUCCESS)
	{
		Log.MarkTestFailed(LOOKUP_PROPERTY_DEF);
	}
	else
	{
		if(!pReturnedProperty)
		{
			Log.MarkTestFailed(LOOKUP_PROPERTY_DEF);
		}
		else
		{
			// Returned property should have the same name it did when created 
			wchar_t propertyName[100];
			if(pReturnedProperty->GetName(propertyName,100*sizeof(wchar_t))
				!=AAFRESULT_SUCCESS)
				Log.MarkTestFailed(LOOKUP_PROPERTY_DEF);
			else if(wcscmp(propertyName,pOptionalPropertyName))
				Log.MarkTestFailed(LOOKUP_PROPERTY_DEF);
		}
	}

	// Look up parent class
	IAAFClassDefSP pParentClass;
	checkResult(pDict->LookupClassDef(AUID_ParentClass,&pParentClass));
	checkExpression(pParentClass!=0);

	// Make sure GetParent() returns correct result
	IAAFClassDefSP pReturnedParent;
	if(pNewClass->GetParent(&pReturnedParent)!=AAFRESULT_SUCCESS)
		Log.MarkTestFailed(GET_PARENT);
	if(!pReturnedParent)
	{
		Log.MarkTestFailed(GET_PARENT);
	}
	else
	{
		// COM interface pointers pReturnedParent and pParentClass should be equal
		if(AreUnksSame(pReturnedParent,pParentClass)==kAAFFalse)
			Log.MarkTestFailed(GET_PARENT);
	}
}

// This function creates the AAF file we will use for our test, and also performs
// some simple tests on the IAAFClassDef interface
static void CreateAAFFile(CAAFClassDefTestLog& Log)
{
	// GUIDs we will use locally in this test

	// {CADEF283-6D3C-11d3-8449-00600832ACB8}
	aafUID_t ourPid1 = 
	{ 0xcadef283, 0x6d3c, 0x11d3, { 0x84, 0x49, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

	// {CADEF284-6D3C-11d3-8449-00600832ACB8}
	aafUID_t ourPid2 = 
	{ 0xcadef284, 0x6d3c, 0x11d3, { 0x84, 0x49, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

	// {CADEF285-6D3C-11d3-8449-00600832ACB8}
	aafUID_t ourPid3 = 
	{ 0xcadef285, 0x6d3c, 0x11d3, { 0x84, 0x49, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

  	aafProductIdentification_t ProductInfo;
	IAAFFileSP       pFile;
	IAAFHeaderSP     pHeader;
	IAAFDictionarySP pDict;

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

	RemoveTestFile(testFileName);
	checkResult(AAFFileOpenNewModify(testFileName, 0, &ProductInfo, &pFile));
	checkExpression(pFile!=0);

	checkResult(pFile->GetHeader(&pHeader));
	checkExpression(pHeader!=0);

	checkResult(pHeader->GetDictionary(&pDict));
	checkExpression(pDict!=0);

	IAAFTypeDefSP ptd;
	checkResult(pDict->LookupTypeDef(kAAFTypeID_UInt8, &ptd));
	
	// Try to extend a typeDefInt.  Should fail.
	IAAFClassDefSP badClass1;
	checkResult(pDict->LookupClassDef(AUID_AAFTypeDefInt, &badClass1));
	
	IAAFPropertyDefSP propDef1;
	if(badClass1->RegisterOptionalPropertyDef (ourPid1,L"First prop",ptd,
		&propDef1)!=AAFRESULT_NOT_EXTENDABLE)
		Log.MarkTestFailed(REGISTER_OPTIONAL_PROPERTY_DEF);
	
	// Try to extend an AAFObject.  Should fail, for now at least.
	IAAFClassDefSP badClass2;
	checkResult(pDict->LookupClassDef (AUID_AAFObject, &badClass2));
	
	IAAFPropertyDefSP propDef2;
	if(badClass2->RegisterOptionalPropertyDef (ourPid2,L"Second prop",ptd,
		&propDef2)!=AAFRESULT_NOT_EXTENDABLE)
		Log.MarkTestFailed(REGISTER_OPTIONAL_PROPERTY_DEF);

	// Try to extend an AAFSequence.  Should succeed.
	IAAFClassDefSP goodClass;
	checkResult(pDict->LookupClassDef (AUID_AAFSequence, &goodClass));
	IAAFPropertyDefSP propDef3;
	if(goodClass->RegisterOptionalPropertyDef (ourPid3,L"Third prop",ptd,
		&propDef3)!=AAFRESULT_SUCCESS)
		Log.MarkTestFailed(REGISTER_OPTIONAL_PROPERTY_DEF);

	// We should not be able to add a new property to the class definition for 
	// AAFSequence
	if(goodClass->RegisterNewPropertyDef (ourPid3,L"Third prop",ptd,kAAFFalse,
		kAAFFalse,&propDef3)==AAFRESULT_SUCCESS)
		Log.MarkTestFailed(REGISTER_NEW_PROPERTY_DEF);

	// Create a new class definition

	// Look up a known class for the parent
	IAAFClassDefSP pParentClass;
	checkResult(pDict->LookupClassDef(AUID_ParentClass,&pParentClass));
	checkExpression(pParentClass!=0);
			
	IAAFClassDefSP pNewClass;
	checkResult(pDict->CreateMetaInstance(AUID_AAFClassDef,IID_IAAFClassDef,
		(IUnknown **)&pNewClass));
	checkExpression(pNewClass!=0);

	if(pNewClass->Initialize(kClassAUID_NewClass,pParentClass,pNewClassName,
		kAAFTrue)!=AAFRESULT_SUCCESS)
		Log.MarkTestFailed(INITIALIZE);

	Log.MarkAsTested(INITIALIZE);

	// Make sure GetParent() detects NULL parameter
	if(pNewClass->GetParent(0)!=AAFRESULT_NULL_PARAM)
		Log.MarkTestFailed(GET_PARENT);

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
	IAAFPropertyDefSP pNewProperty;	
	if(pNewClass->RegisterNewPropertyDef(newPropertyPid,pNewPropertyName,
		ptd,kAAFFalse,kAAFFalse,&pNewProperty)!=AAFRESULT_SUCCESS)
		Log.MarkTestFailed(REGISTER_NEW_PROPERTY_DEF);

	// CountPropertyDefs() should now return 1
	iPropertyDefs=-1;
	if(pNewClass->CountPropertyDefs(&iPropertyDefs)!=S_OK)
		Log.MarkTestFailed(COUNT_PROPERTY_DEFS);
	if(iPropertyDefs!=1)
		Log.MarkTestFailed(COUNT_PROPERTY_DEFS);

	// Add an optional property to our new class definition
	IAAFPropertyDefSP pOptionalProperty;	
	if(pNewClass->RegisterOptionalPropertyDef(optionalPropertyPid,
		pOptionalPropertyName,ptd,&pOptionalProperty)!=AAFRESULT_SUCCESS)
		Log.MarkTestFailed(REGISTER_OPTIONAL_PROPERTY_DEF);

	// Register our new class definition in the dictionary.
	checkResult(pDict->RegisterClassDef(pNewClass));

	VerifyAAFFile(Log,pFile);

	checkResult(pFile->Save());
	checkResult(pFile->Close());
}

// This function reads the AAF file from disk that was created by CreateAAFFile(),
// then calls VerifyAAFFile() to make sure the file contains the correct data
static void ReadAAFFile(CAAFClassDefTestLog& Log)
{
	IAAFFileSP       pFile;
	IAAFHeaderSP     pHeader;

	checkResult(AAFFileOpenExistingRead(testFileName,0,&pFile));
	
	VerifyAAFFile(Log,pFile);
	
	// Now that we have called VerifyAAFFile() for a file loaded from disk, we can
	// check off the various methods that it tests
	Log.MarkAsTested(COUNT_PROPERTY_DEFS);
	Log.MarkAsTested(GET_PROPERTY_DEFS);
	Log.MarkAsTested(GET_NAME);
	Log.MarkAsTested(GET_NAME_BUF_LEN);
	Log.MarkAsTested(LOOKUP_PROPERTY_DEF);
	Log.MarkAsTested(GET_PARENT);
	Log.MarkAsTested(REGISTER_NEW_PROPERTY_DEF);
	Log.MarkAsTested(REGISTER_OPTIONAL_PROPERTY_DEF);

	checkResult(pFile->Close());
}

static void ClassDefTest(CAAFClassDefTestLog& Log)
{
	CreateAAFFile(Log);
	ReadAAFFile(Log);
}

extern "C" HRESULT CAAFClassDef_test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;

	// Create test log
	CAAFClassDefTestLog Log(NUM_IAAFCLASSDEF_METHODS,
		(const char**)ppIAAFMethodNames);

	// Execute test, catching any exceptions
	aafBool bException=kAAFFalse;
	try
	{
		ClassDefTest(Log);
	}
	catch (...)
	{
		cout << "CAAFClassDef_test"
			<< "...Caught general C++ exception!" << endl; 
		bException=kAAFTrue;
	}

	// Print test summary
	Log.PrintSummary();

	// Return appropriate test code
	if(bException==kAAFTrue || Log.MethodsFailed())
		return(AAFRESULT_TEST_FAILED);
	else if(Log.MethodsNotTested())
		return(AAFRESULT_TEST_PARTIAL_SUCCESS);
	else
		return(AAFRESULT_SUCCESS);
}

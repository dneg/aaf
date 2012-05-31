// @doc INTERNAL
// @com This file implements a simple sanity test that verify file
// @extension independence for a variety of encodings.  This is motivated
// @by the SDK's official need to support both .aaf and .isr as valid
// @AAF file extensions.
//=---------------------------------------------------------------------=
//
// $Id$
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

#include "AAF.h"
#include "AAFTypes.h"
#include "AAFResult.h"
#include "AAFDefUIDs.h"
#include "AAFFileMode.h"
#include "AAFFileKinds.h"

#include <iostream>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>

using namespace std;

void checkResult(HRESULT r)
{
	if (FAILED(r))
		throw r;
}

void RemoveTestFile(const aafWChar* pFileName)
{
	const size_t kMaxFileName = 512;
	char cFileName[kMaxFileName];

	size_t status = wcstombs(cFileName, pFileName, kMaxFileName);
	if (status != (size_t)-1) {
		remove(cFileName);
	}
}

void RenameTestFile(const wchar_t* pName, const wchar_t* pNewName)
{
  char cTmpName[FILENAME_MAX];
  char cTmpNewName[FILENAME_MAX];

  size_t status = wcstombs(cTmpName, pName, FILENAME_MAX);
  if (status == (size_t)-1) {
	throw AAFRESULT_NOT_AAF_FILE;
  }

  status = wcstombs(cTmpNewName, pNewName, FILENAME_MAX);
  if (status == (size_t)-1) {
	throw AAFRESULT_NOT_AAF_FILE;
  }

  remove(cTmpNewName);
  if (rename(cTmpName, cTmpNewName) != 0) {
	throw AAFRESULT_NOT_AAF_FILE;
  }
}

class AbstractTest 
{
protected:

	aafProductIdentification_t testProductID;

        static aafCharacter companyName[];
        static aafCharacter productName[];

private:
	static aafProductVersion_t TestVersion;

public:
	AbstractTest()
	{	
	        testProductID.companyName = companyName;
	        testProductID.productName = productName;
		testProductID.productVersion = &TestVersion;
		testProductID.productVersionString = NULL;
		testProductID.productID = UnitTestProductID;
		testProductID.platform = NULL;
	}

	virtual ~AbstractTest()
	{}

	void Execute()
	{
		wcout << this->GetTestName() << L": " << this->GetFileName() << endl;
		this->Create();
		this->Modify();
		this->Read();
		this->IsAAF();
	}

protected:

	virtual const wchar_t* GetTestName() = 0;
	virtual const wchar_t* GetFileName() = 0;
	virtual void Create() = 0;
	virtual void Modify() = 0;
	virtual void Read() = 0;
	virtual void IsAAF() = 0;
};
aafProductVersion_t AbstractTest::TestVersion = { 1, 1, 0, 0, kAAFVersionUnknown };
aafCharacter AbstractTest::companyName[] = L"AMW Association";
aafCharacter AbstractTest::productName[] = L"ComFileExtensionTest";

// BasicTest - simple test possible - just create a file, open for modify, and open for read.
// The basic AAF api calls are used to create and open the file.
// The caller ivokes with multiple file extensions to validate extension behavior.
class BasicTest : public AbstractTest
{
protected:
	// input
	aafCharacter* filename;
	aafUInt32  modeFlags;

public:

	BasicTest(const aafCharacter* filename,
		aafUInt32  modeFlags)
		: filename(0),
		  modeFlags(0)
	{
		assert(modeFlags == 0 || modeFlags == AAF_FILE_MODE_USE_LARGE_SS_SECTORS);
		this->SetFileName(filename);
		this->modeFlags = modeFlags;
	}

protected:

	virtual const wchar_t* GetTestName() {
		return L"BasicTest";
	}

	virtual const wchar_t* GetFileName() {
		return this->filename;
	}

	void SetFileName(const aafCharacter* newFilename) {
		if (filename != 0) {
			delete filename;
		}
		
		filename = new aafCharacter[wcslen(newFilename)+1];
		wcscpy(filename, newFilename);
	}

	virtual void Create()
	{
		IAAFFile* pFile = 0;
		RemoveTestFile(filename);
		checkResult(AAFFileOpenNewModify(this->filename, this->modeFlags, &this->testProductID, &pFile));
		checkResult(pFile->Save());
		checkResult(pFile->Close());
	}

	virtual void Modify()
	{
		IAAFFile* pFile = 0;
		checkResult(AAFFileOpenExistingModify(this->filename, 0, &this->testProductID, &pFile));
		checkResult(pFile->Save());
		checkResult(pFile->Close());
	}

	virtual void Read()
	{
		IAAFFile* pFile = 0;
		checkResult(AAFFileOpenExistingRead(this->filename, 0, &pFile));
		checkResult(pFile->Close());
	}

	virtual void IsAAF()
	{
		aafBool isAafFile = false;

		checkResult(AAFFileIsAAFFile(this->filename, const_cast<aafUID_t*>(&aafFileKindDontCare), &isAafFile));

		if (!isAafFile) {
			throw AAFRESULT_NOT_AAF_FILE;
		}
	}
};

// Same as BasicTest but uses the extended interface to create the file.
class BasicExTest : public BasicTest
{
	const aafUID_t* pFileKind;

public:
	BasicExTest(const aafCharacter* filename, const aafUID_t* pFileKind) 
		: BasicTest(filename, 0)
	{
		this->pFileKind = pFileKind;
	}

	virtual void Create()
	{
		RemoveTestFile(filename);
		IAAFFile* pFile = 0;
		checkResult(AAFFileOpenNewModifyEx(this->filename, this->pFileKind, this->modeFlags, &this->testProductID, &pFile));
		checkResult(pFile->Save());
		checkResult(pFile->Close());
	}

};

// Same as basic test but changes the file extenion after it is created and ensures
// it can be opened after the rename.
class BasicRenameTest : public BasicTest
{
private:
	const wchar_t* newExt;

public:
	BasicRenameTest(const aafCharacter* filename,
		aafUInt32  modeFlags, const wchar_t* newExt) 
		: BasicTest(filename, modeFlags),
		newExt(newExt)
	{}

protected:
	virtual const wchar_t* GetTestName() {
		return L"BasicRenameTest";
	}

	virtual void Create()
	{
		BasicTest::Create();
		aafWChar tmp[FILENAME_MAX];
		wcscpy(tmp, this->GetFileName());
		wcscat(tmp, this->newExt);
		RenameTestFile(this->GetFileName(), tmp);
		this->SetFileName(tmp);
	}
};


class FileExtensionTest
{

public:

	FileExtensionTest() {
	}

	void Execute() {

		AbstractTest* tests[] = {
			new BasicTest(L"basicTest.aaf", 0),
			new BasicTest(L"basicTest.4k.aaf", AAF_FILE_MODE_USE_LARGE_SS_SECTORS),
			new BasicTest(L"basicTest.isr", 0),
			new BasicTest(L"basicTest.4k.isr", AAF_FILE_MODE_USE_LARGE_SS_SECTORS),
			new BasicRenameTest(L"renameTest.aaf", 0, L".isr"),
			new BasicRenameTest(L"renameTest.4k.aaf", AAF_FILE_MODE_USE_LARGE_SS_SECTORS, L".isr"),

			new BasicExTest(L"basicExTest.512.aaf", &kAAFFileKind_Aaf512Binary),
			new BasicExTest(L"basicExTest.512.aaf", &kAAFFileKind_Aaf512Binary),

			new BasicExTest(L"basicExTest.4K.aaf", &kAAFFileKind_Aaf4KBinary),
			new BasicExTest(L"basicExTest.4K.isr", &kAAFFileKind_Aaf4KBinary),

#ifdef OS_WINDOWS
			new BasicExTest(L"basicExTest.M512.aaf", &kAAFFileKind_AafM512Binary),
			new BasicExTest(L"basicExTest.M512.isr", &kAAFFileKind_AafM512Binary),

			new BasicExTest(L"basicExTest.M4K.aaf", &kAAFFileKind_AafM4KBinary),
			new BasicExTest(L"basicExTest.M4K.isr", &kAAFFileKind_AafM4KBinary),
#endif
			new BasicExTest(L"basicExTest.S512K.aaf", &kAAFFileKind_AafS512Binary),
			new BasicExTest(L"basicExTest.S512K.isr", &kAAFFileKind_AafS512Binary),

			new BasicExTest(L"basicExTest.S4K.aaf", &kAAFFileKind_AafS4KBinary),
			new BasicExTest(L"basicExTest.S4K.isr", &kAAFFileKind_AafS4KBinary),

			new BasicExTest(L"basicExTest.klv",     &kAAFFileKind_AafKlvBinary),
			new BasicExTest(L"basicExTest.klv.aaf", &kAAFFileKind_AafKlvBinary),
			new BasicExTest(L"basicExTest.klv.isr", &kAAFFileKind_AafKlvBinary),

			new BasicExTest(L"basicExTest.xml",     &kAAFFileKind_AafXmlText),
			new BasicExTest(L"basicExTest.xml.aaf", &kAAFFileKind_AafXmlText),
			new BasicExTest(L"basicExTest.xml.isr", &kAAFFileKind_AafXmlText),
			0  
		};

		for(int i = 0; tests[i] != 0; ++i) {
			tests[i]->Execute();
		}

	}
};

void printUsage(const char* progName)
{
}

int main(int argc, char *argv[])
{
	bool deleteFiles = false;
	int i = 1;

	if (argc > 1)
	{
		while (i < argc)
		{
			if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help"))
			{
				printUsage(argv[0]);
				return 0;
			}
			else if (!strcmp(argv[i], "-d"))
			{
				deleteFiles = true;
				i++;
			}
			else
			{
				printUsage(argv[0]);
				return 1;
			}
		}
	}

	try {
		FileExtensionTest test;
		test.Execute();
	}
	catch (HRESULT& r)
	{
		wcerr << L"Error : Caught HRESULT 0x" << hex << r << endl;
		return 1;
	}



	return 0;
}

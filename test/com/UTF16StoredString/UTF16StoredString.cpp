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
// Stuart Cunningham and Anuradha Suraparaju.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
#include <locale.h>

#include "AAF.h"
#include "AAFResult.h"
#include "AAFStoredObjectIDs.h"

#if defined (_MSC_VER)
#include <windows.h>
#endif

#define check(a) \
{ HRESULT check_result_tmp_ = a; \
	if (!SUCCEEDED(check_result_tmp_)) \
	{ \
		aafCharacter buf[200] = L""; \
		AAFResultToText(check_result_tmp_, buf, sizeof(buf)); \
		fprintf(stderr, "failed at %s:%d, result=0x%08x %ls\n", __FILE__, __LINE__, check_result_tmp_, buf); \
		exit(1); \
	} \
}

const aafUID_t TestProductID = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };
const aafMobID_t TEST_MobID = {{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00}, 0x13, 0x00, 0x00, 0x00, {0xfd3cc302, 0x03fe, 0x11d4, {0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}}};

static char *hexDump(const void *inp, int len)
{
	static char result[250];
	aafUInt8 *data = (aafUInt8*)inp;

	char *p = result;
	int i;
	for (i = 0; i < len; i++)
	{
		int written = sprintf(p, "%02x", data[i]);
		p += written;
		if (i != len - 1)
		{
			strcat(p, ",");
			p++;
		}
	}
	return result;
}

extern int main(int argc, char *argv[])
{
	const char *filename_cstr = "test.aaf";

#ifndef _MSC_VER
	setlocale (LC_ALL, "en_US.UTF-8");
#endif

	if (argc >= 2)
	{
		filename_cstr = argv[1];
	}

	// convert C str to wide string
	aafWChar filename[FILENAME_MAX];
	size_t status = mbstowcs(filename, filename_cstr, sizeof(filename));
	if (status == (size_t)-1) {
		fprintf(stderr, "mbstowcs failed for \"%s\"\n", filename_cstr);
		return 1;
	}
	remove(filename_cstr);

	IAAFFile *pFile = NULL;
	int mode = 0;

	aafProductIdentification_t productID;
	aafProductVersion_t TestVersion = {1, 1, 0, 0, kAAFVersionUnknown};
	productID.companyName = (aafCharacter*)L"HSC";
	productID.productName = (aafCharacter*)L"String Tester";
	productID.productVersion = &TestVersion;
	productID.productVersionString = NULL;
	productID.productID = TestProductID;
	productID.platform = (aafCharacter*)L"Linux";

	// Create new AAF file
	check(AAFFileOpenNewModify(filename, mode, &productID, &pFile));

	// Create a simple Mob
	IAAFClassDef	*classDef = NULL;
	IAAFMob			*pMob = NULL;
	IAAFHeader		*pHeader = NULL;
	IAAFDictionary	*pDictionary = NULL;

	check(pFile->GetHeader(&pHeader));
	check(pHeader->GetDictionary(&pDictionary));
	check(pDictionary->LookupClassDef(AUID_AAFMasterMob, &classDef));
	check(classDef->CreateInstance(IID_IAAFMob, (IUnknown **)&pMob));
	classDef->Release();

	
	check(pMob->SetMobID(TEST_MobID));

	// UTF-8 for codepoint U+1D11E (musical G Clef): 0xf0,0x9d,0x84,0x9e
	// UTF-8 for codepoint U+1D122 (musical F Clef): 0xf0,0x9d,0x84,0xa2
	// http://unicode.org/charts/PDF/U1D100.pdf 
	// http://en.wikipedia.org/wiki/UTF-8
	aafCharacter *mobname;
	const unsigned char inputStr[] = {	0xf0,0x9d,0x84,0x9e,	// U+1D11E
										0xf0,0x9d,0x84,0xa2,	// U+1D122
										0x4d, 0x6f, 0x62,		// 'M' 'o' 'b'
										0x0 };

	// Convert UTF-8 inputStr to native wchar_t representation (UTF-32 Unix, UTF-16 Windows)
	int wlen = 0, n;
#ifndef _MSC_VER
	int ret;
	char *p = (char *)inputStr;
	while ((ret = mblen(p, 4)) > 0)
	{ ++wlen; p+=ret; }
	mobname = new aafCharacter[wlen+1];
	n = mbstowcs(mobname, (const char *)inputStr, wlen+1);

	if (n == -1)
	{
		fprintf (stderr, "mbstowcs returned -1. Invalid multibyte string\n");
		exit(1);
	}
#else
	// Under Windows we must use MultiByteToWideChar() to get correct UTF-8 conversion to UTF-16
	// since mbstowcs() is broken for UTF-8.
	wlen = MultiByteToWideChar (CP_UTF8, MB_ERR_INVALID_CHARS, (LPCSTR)inputStr, -1, NULL, 0);
	if (wlen == 0)
	{
		fprintf (stderr, "MultiByteToWideChar returned 0. Invalid multibyte string\n");
		exit(1);
	}
	mobname = new aafCharacter[wlen];
	n = MultiByteToWideChar (CP_UTF8, MB_ERR_INVALID_CHARS, (LPCSTR)inputStr, -1, mobname, wlen);
	if (n == 0)
	{
		fprintf (stderr, "MultiByteToWideChar returned 0. Invalid multibyte string\n");
		exit(1);
	}
#endif

	// SetName() calls OMSimpleProperty::set() which does a memcpy of the mobname string
	// to an OMByte* variable 'bits()' at OMProperty.cpp:399
	// Found by setting an rwatch on mobname address.
	check(pMob->SetName(mobname));
	aafUInt32 size_before = 0;
	check(pMob->GetNameBufLen(&size_before));
	check(pHeader->AddMob(pMob));
	pMob->Release();
	pHeader->Release();
	pDictionary->Release();

	// All the work of storing to disk happens during Save()
	// The bits() variable is next read in OMType::contract() at OMType.cpp:137
	// which is called by ImplAAFTypeDefCharacter::externalize() at ImplAAFTypeDefCharacter.cpp:307
	// which is called by ImplAAFTypeDefString::externalize() at ImplAAFTypeDefString.cpp:584
	// which is called by OMSSStoredObject::save() at OMSSStoredObject.cpp:382
	check(pFile->Save());
	check(pFile->Close());
	pFile->Release();

	// OMCharacterStringProperty<CharacterType>::stringLength() at OMVariableSizePropertyT.h:80
	// calculates string length of AAF string properties

	// Read AAF file back in
	check(AAFFileOpenExistingRead(filename, mode, &pFile));

	// Get the Mob
	check(pFile->GetHeader(&pHeader));
	check(pHeader->LookupMob(TEST_MobID, &pMob));
	aafUInt32 size_after = 0;
	check(pMob->GetNameBufLen(&size_after));
	aafCharacter *mobname_after = new aafCharacter[size_after];
	check(pMob->GetName(mobname_after, size_after));

	// Compare Mob name before storing to disk with Mob name read back from disk
	int test_result = 0;
	if (size_before != size_after) {
		printf("size_before=%d != size_after=%d\n", size_before, size_after);
		test_result = 1;
	}
	else {
		if (memcmp(mobname, mobname_after, size_before) != 0) {
			printf("wchar_t* mobname and wchar_t* mobname_after differ:\n");
			printf("  %s\n", hexDump(mobname, size_before));
			printf("  %s\n", hexDump(mobname_after, size_after));
			test_result = 1;
		}
	}

	// Check if the multibyte (UTF-8) versions of mobname and mobname_after match.
	char *outputStr;
#ifndef _MSC_VER
	wlen = wcslen(mobname_after)*sizeof(aafCharacter) + 1;
	outputStr = new char [wlen];
	n = wcstombs (outputStr, mobname_after, wlen);
	if (n == -1)
	{
		fprintf(stderr, "Could not convert mobname_after to multibyte str\n");
		exit(1);
	}
#else
	wlen = WideCharToMultiByte(CP_UTF8, 0, mobname_after, -1, NULL, 0, NULL, NULL);
	if (wlen == 0)
	{
		fprintf (stderr, "Failed to convert mobname_after to multibyte string\n");
		exit(1);
	}
	outputStr = new char[wlen];

	wlen = WideCharToMultiByte(CP_UTF8, 0, mobname_after, -1, outputStr, wlen, NULL, NULL);
#endif

	if (strlen((char *)inputStr) != strlen(outputStr))
	{
		fprintf(stderr, "UTF-8 version of string: input length(%d) != output length(%d)\n", (int)strlen((char *)inputStr), (int)strlen(outputStr));
		test_result = 1;
	}

	if (strcmp((char *)inputStr, outputStr) != 0)
	{
		fprintf(stderr, "UTF-8 version of string: input and output strings differ\n");
		printf("  %s\n", hexDump(inputStr, strlen((char *)inputStr)));
		printf("  %s\n", hexDump(outputStr, strlen(outputStr)));
		test_result = 1;
	}

	pMob->Release();
	pHeader->Release();
	check(pFile->Close());
	pFile->Release();

	delete [] mobname;
	delete [] mobname_after;
	return test_result;
}

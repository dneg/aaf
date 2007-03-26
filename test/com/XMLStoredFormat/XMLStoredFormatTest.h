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
// The Original Code of this file is Copyright 1998-2005, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is British Broadcasting Corporation.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef __TEST_H__
#define __TEST_H__

#include <AAF.h>
#include <AAFTypes.h>
#include <AAFResult.h>
#include <AAFFileKinds.h>
#include <AAFClassDefUIDs.h>
#include <AAFPropertyDefs.h>
#include <wchar.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define checkResult(a)  \
{ \
    HRESULT _hr = a; \
    if (!SUCCEEDED(_hr)) \
    { \
        fprintf(stderr, "\nError '%0x' returned at line %d in %s\n", _hr, \
            __LINE__, __FILE__); \
        throw _hr; \
    } \
}

#define release(p) \
if (p != 0) \
{ \
    p->Release(); \
    p = 0; \
}


void report(bool passed);
IAAFFile* openFileForReading(aafCharacter* fileName);
void replaceCharacter(wchar_t* str, wchar_t c, wchar_t r);
void saveCopy(aafCharacter* inFileName, aafCharacter* outFileName);
void copyAndTouch(aafCharacter* inFileName, aafCharacter* outFileName);
bool diff(aafCharacter* fileNameA, aafCharacter* fileNameB);


// the tests
bool testCharacter();
bool testEnum();
bool testExtEnum();
bool testFixedArray();
bool testIndirect();
bool testInteger();
bool testOpaque();
bool testRecord();
bool testRename();
bool testSet();
bool testStream();
bool testString();
bool testVarArray();
bool testWeakRef();

bool testExampleFiles();

#endif



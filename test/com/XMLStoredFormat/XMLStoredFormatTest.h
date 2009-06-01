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
// British Broadcasting Corporation.
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
IAAFFile* openFileForReading(const aafCharacter* fileName);
void replaceCharacter(wchar_t* str, wchar_t c, wchar_t r);
void saveCopy(const aafCharacter* inFileName, const aafCharacter* outFileName);
void copyAndTouch(const aafCharacter* inFileName, const aafCharacter* outFileName);
bool diff(const aafCharacter* fileNameA, const aafCharacter* fileNameB);


// the tests
// returns 0 when passed, 1 when there are expected failures only, otherwise 2 when there are failures
int testCharacter();
int testEnum();
int testExtEnum();
int testFixedArray();
int testIndirect();
int testInteger();
int testOpaque();
int testRecord();
int testRename();
int testSet();
int testStream();
int testString();
int testVarArray();
int testWeakRef();

int testExampleFiles();

#endif



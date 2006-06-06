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
// The Original Code of this file is Copyright 1998-2006, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// Author : Tim Bingham (Tim_Bingham@avid.com)

#include "headerGenUtils.h"

#include <iostream>
#include <string>
using namespace std;
#include <assert.h>

void comment(const char* s)
{
  cout << "/* ";
  cout << s;
  cout << " */";
}

void section(const char* s)
{
  cout << endl;
  comment(s);
  cout << endl;
}

void genCode(const char* name, unsigned long int val, const char* desc)
{
  if (strlen(desc) != 0) {
    cout << "        ";
    comment(desc);
    cout << endl;
  }
  cout << "#define AAFRESULT_" << name;
  size_t len = strlen(name);
  const size_t codeAlign = 32;
  if (len < codeAlign) {
    size_t spaces = codeAlign - len;
    for (size_t i = 0; i < spaces; i++) {
      cout << " ";
    }
  } else {
    cout << " ";
  }
  cout << "((HRESULT)0x" << hex << val + 0x80000000 + 0x120000 <<")";
  cout << endl;
}

void success(const char* name, unsigned long int val)
{
  cout << "#define ";
  printName(name, 52, cout);
  cout << val << endl;
}

void alias(const char* fullOldName, const char* name)
{
  cout << "#define " << fullOldName;
  size_t len = strlen(fullOldName);
  const size_t codeAlign = 32 + strlen("AAFRESULT_");
  if (len < codeAlign) {
    size_t spaces = codeAlign - len;
    for (size_t i = 0; i < spaces; i++) {
      cout << " ";
    }
  } else {
    cout << " ";
  }
  cout << "AAFRESULT_" << name << endl;
}

static void doFile(const char* moduleName)
{
  assert(moduleName);
  printBeginGuard(moduleName, cout);
  printCopyright(cout);

  cout << "#define AAFRESULT_FAILED(Status) ((AAFRESULT)(Status)<0)";
  cout << endl;
  cout << "#define AAFRESULT_SUCCEEDED(Status) (!(AAFRESULT_FAILED(Status)))";
  cout << endl;

  cout << endl;

  cout << "#define _FACILITY_AAF 0x12";
  cout << endl;

  cout << "#define MAKE_AAFHRESULT( code ) \\";
  cout << endl;
  cout << "    ((HRESULT) (((aafUInt32)(1)             <<31) | \\";
  cout << endl;
  cout << "                ((aafUInt32)(_FACILITY_AAF) <<16) | \\";
  cout << endl;
  cout << "                ((aafUInt32)(code))) )";
  cout << endl;

  cout << endl;

  cout << "/* Non-AAF codes */";
  cout << endl;
  cout << "#define AAFRESULT_NOT_IMPLEMENTED                 ((HRESULT)0x80004001L)";
  cout << endl;

#define AAF_ERROR_SECTION(s) section(s);
#define AAF_DEFINE_ERROR(name, val, desc) genCode(#name, val, desc);
#define AAF_DEFINE_SUCCESS(name, code) success("AAFRESULT_"#name, code);
#define AAF_DEFINE_ERROR_ALIAS(fullOldName, name) alias(#fullOldName, #name);
#include "AAFMetaResult.h"

  printEndGuard(moduleName, cout);
}

int main(int argc, char** argv)
{
  char* moduleName = 0;
  validateArgs(argc, argv, moduleName);
  assert(moduleName);
  doFile(moduleName);
  return 0;
}


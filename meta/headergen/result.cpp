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
  cout << "0x" << hex << val + 0x80000000 + 0x120000;
  cout << endl;
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

#define AAF_ERROR_SECTION(s) section(s);
#define AAF_DEFINE_ERROR(name, val, desc) genCode(#name, val, desc);
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


//=---------------------------------------------------------------------=
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
// The Original Code of this file is Copyright 1998-2002, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// Author : Tim Bingham (Tim_Bingham@avid.com) - after an initial
// implementation by Bob Tillman.

#include <iostream.h>
#include <assert.h>
#include <stdlib.h>

#include "headerGenUtils.h"

const char* prefix = "kAAFPropID_";

#define AAF_TABLE_BEGIN() {
#define AAF_PROPERTY(name, id, tag, type, mandatory, uid, container) \
{#container "_" #name, id},
#define AAF_TABLE_END()   };

typedef struct {
  char* name;
  aafUID_t identifier;
} Property_t;

Property_t properties[] =
#include "AAFMetaDictionary.h"

static void doFile(const char* moduleName)
{
  assert(moduleName);
  printBeginGuard(moduleName, cout);
  printCopyright(cout);
  cout << endl;
  cout << "#include \"AAFTypes.h\"" << endl;
  cout << endl;

  cout << "// AAF property definition UIDs." << endl
       << "//" << endl << endl;

  size_t i = 0;
  for (i = 0; i < sizeof(properties)/sizeof(properties[0]); i++){
    printDefinition("const aafUID_t",
                    prefix,
                    properties[i].name,
                    0,
                    properties[i].identifier,
                    cout);
  }
  Property_t special[] = {
{"Root_MetaDictionary",
{0x0D010301, 0x0101, 0x0100,{0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02}}},
{"Root_Header",
{0x0D010301, 0x0102, 0x0100, {0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02}}}
  };

  cout << "// Special property definition ids used for specifying the" << endl;
  cout << "// starting strong reference in the target list of a" << endl;
  cout << "// weak reference." << endl;
  cout << "//" << endl;

  for (i = 0; i < sizeof(special)/sizeof(special[0]); i++){
    printDefinition("const aafUID_t",
                    prefix,
                    special[i].name,
                    0,
                    special[i].identifier,
                    cout);
  }

  printEndGuard(moduleName, cout);
}


void main(int argc, char** argv)
{
  char* moduleName = 0;
  validateArgs(argc, argv, moduleName);
  assert(moduleName);
  doFile(moduleName);
  exit(0);
}

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
#include <string.h>

#include "AAFTypes.h"

#include "headerGenUtils.h"

const char* prefix = "kAAFClassID_";

#define AAF_ALIAS_TABLE_BEGIN() {
#define AAF_CLASS_ALIAS(name, alias) {#name, #alias}
#define AAF_ALIAS_SEPARATOR() ,
#define AAF_ALIAS_TABLE_END()   };

struct {
  char* name;
  char* alias;
} aliases [] =
#include "AAFMetaDictionary.h"

#define AAF_TABLE_BEGIN() {
#define AAF_CLASS(name, id, parent, concrete) {#name, id}
#define AAF_CLASS_SEPARATOR() ,
#define AAF_TABLE_END()   };

struct {
  char* name;
  aafUID_t identifier;
} classes [] =
#include "AAFMetaDictionary.h"

static void doFile (const char * moduleName)
{
  assert(moduleName);
  printBeginGuard(moduleName, cout);
  printCopyright(cout);
  cout << endl;
  cout << "#include \"AAFTypes.h\"" << endl;
  cout << endl;

  cout << "// AAF class definition UIDs." << endl
       << "//" << endl << endl;

  cout << "// The AAF reference implementation uses shorter names than" << endl
       << "// SMPTE. The names are shortened by the following aliases." << endl
       << "//" << endl;

  size_t maxNameLength = 0;
  size_t i = 0;
  for (i = 0; i < sizeof(aliases)/sizeof(aliases[0]); i++){
    size_t length = strlen(aliases[i].alias);
    if (length > maxNameLength) {
      maxNameLength = length;
    }
  }
  size_t width = maxNameLength;
  for (i = 0; i < sizeof(aliases)/sizeof(aliases[0]); i++){
    cout << "#define " << prefix;
    printName(aliases[i].alias, width, cout);
    cout << " " << prefix;
    printName(aliases[i].name, 0, cout);
    cout << endl;
  }
  cout << endl;

  for (i = 0; i < sizeof(classes)/sizeof(classes[0]); i++){
    printDefinition("const aafUID_t",
                    prefix,
                    classes[i].name,
                    0,
                    classes[i].identifier,
                    cout);
  }

  printEndGuard(moduleName, cout);
}


void main (int argc, char ** argv)
{
  char* moduleName = 0;
  validateArgs(argc, argv, moduleName);
  assert(moduleName);
  doFile(moduleName);
  exit(0);
}

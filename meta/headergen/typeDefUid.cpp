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

const char* prefix = "kAAFTypeID_";

#define TYPE_GUID_NAME(type) #type

#define AAF_TYPE_DEFINITION_INTEGER(name, id, size, signed) \
{TYPE_GUID_NAME(name), id},

#define AAF_TYPE_DEFINITION_ENUMERATION(name, id, type) \
{TYPE_GUID_NAME(name), id},

#define AAF_TYPE_DEFINITION_RECORD(name, id) \
{TYPE_GUID_NAME(name), id},

#define AAF_TYPE_DEFINITION_VARYING_ARRAY(name, id, type) \
{TYPE_GUID_NAME(name), id},

#define AAF_TYPE_DEFINITION_FIXED_ARRAY(name, id, type, count) \
{TYPE_GUID_NAME(name), id},

#define AAF_TYPE_DEFINITION_RENAME(name, id, type) \
{TYPE_GUID_NAME(name), id},

#define AAF_TYPE_DEFINITION_STRING(name, id, type) \
{TYPE_GUID_NAME(name), id},

#define AAF_TYPE_DEFINITION_INDIRECT(name, id) \
{TYPE_GUID_NAME(name), id},

#define AAF_TYPE_DEFINITION_OPAQUE(name, id) \
{TYPE_GUID_NAME(name), id},

#define AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION(name, id) \
{TYPE_GUID_NAME(name), id},

#define AAF_TYPE_DEFINITION_CHARACTER(name, id) \
{TYPE_GUID_NAME(name), id},

#define AAF_TYPE_DEFINITION_SET(name, id, type) \
{TYPE_GUID_NAME(name), id},

#define AAF_TYPE_DEFINITION_STRONG_REFERENCE(name, id, type) \
{TYPE_GUID_NAME(name), id},

#define AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(name, id, type) \
{TYPE_GUID_NAME(name), id},

#define AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR(name, id, type) \
{TYPE_GUID_NAME(name), id},

#define AAF_TYPE_DEFINITION_WEAK_REFERENCE(name, id, type) \
{TYPE_GUID_NAME(name), id},

#define AAF_TYPE_DEFINITION_WEAK_REFERENCE_SET(name, id, type) \
{TYPE_GUID_NAME(name), id},

#define AAF_TYPE_DEFINITION_WEAK_REFERENCE_VECTOR(name, id, type) \
{TYPE_GUID_NAME(name), id},

#define AAF_TYPE_DEFINITION_STRONG_REFERENCE(name, id, type) \
{TYPE_GUID_NAME(name), id},

#define AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(name, id, type) \
{TYPE_GUID_NAME(name), id},

#define AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR(name, id, type) \
{TYPE_GUID_NAME(name), id},

#define AAF_TYPE_DEFINITION_WEAK_REFERENCE(name, id, type) \
{TYPE_GUID_NAME(name), id},

#define AAF_TYPE_DEFINITION_WEAK_REFERENCE_SET(name, id, type) \
{TYPE_GUID_NAME(name), id},

#define AAF_TYPE_DEFINITION_WEAK_REFERENCE_VECTOR(name, id, type) \
{TYPE_GUID_NAME(name), id},

#define AAF_TYPE_DEFINITION_STREAM(name, id) \
{TYPE_GUID_NAME(name), id},

#define AAF_TYPE(name) name
#define AAF_REFERENCE_TYPE(type, target)      AAF_TYPE(target##type)
#define AAF_REFERENCE_TYPE_NAME(type, target) AAF_TYPE(target##type)

struct {
  char* name;
  aafUID_t identifier;
} types [] = {
#include "AAFMetaDictionary.h"
};

static void doFile(const char* moduleName)
{
  assert(moduleName);
  printBeginGuard(moduleName, cout);
  printCopyright(cout);
  cout << endl;
  cout << "#include \"AAFTypes.h\"" << endl;
  cout << endl;

  cout << "// AAF type definition UIDs." << endl
       << "//" << endl << endl;

  size_t i = 0;
  for (i = 0; i < sizeof(types)/sizeof(types[0]); i++){
    printDefinition("const aafUID_t",
                    prefix,
                    types[i].name,
                    0,
                    types[i].identifier,
                    cout);
  }

  cout << endl;
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

#include <iostream.h>
#include <iomanip.h>
#include <assert.h>
#include <stdlib.h>

#include "headerGenUtils.h"

const char* prefix = "kAAFTypeID_";

#define TYPE_GUID_NAME(type) #type

#define AAF_TYPE_DEFINITION_INTEGER(name, id, size, signed) \
{TYPE_GUID_NAME(name) \
,id},

#define AAF_TYPE_DEFINITION_ENUMERATION(name, id, type) \
{TYPE_GUID_NAME(name) \
,id},

#define AAF_TYPE_DEFINITION_RECORD(name, id) \
{TYPE_GUID_NAME(name) \
,id},

#define AAF_TYPE_DEFINITION_VARYING_ARRAY(name, id, type) \
{TYPE_GUID_NAME(name) \
,id},

#define AAF_TYPE_DEFINITION_FIXED_ARRAY(name, id, type, count) \
{TYPE_GUID_NAME(name) \
,id},

#define AAF_TYPE_DEFINITION_RENAME(name, id, type) \
{TYPE_GUID_NAME(name) \
,id},

#define AAF_TYPE_DEFINITION_STRING(name, id, type) \
{TYPE_GUID_NAME(name) \
,id},

#define AAF_TYPE_DEFINITION_INDIRECT(name, id) \
{TYPE_GUID_NAME(name) \
,id},

#define AAF_TYPE_DEFINITION_OPAQUE(name, id) \
{TYPE_GUID_NAME(name) \
,id},

#define AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION(name, id) \
{TYPE_GUID_NAME(name) \
,id},

#define AAF_TYPE_DEFINITION_CHARACTER(name, id) \
{TYPE_GUID_NAME(name) \
,id},

#define AAF_TYPE_DEFINITION_SET(name, id, type) \
{TYPE_GUID_NAME(name) \
,id},

#define AAF_TYPE_DEFINITION_STRONG_REFERENCE(name, id, type) \
{TYPE_GUID_NAME(name) \
,id},

#define AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(name, id, type) \
{TYPE_GUID_NAME(name) \
,id},

#define AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR(name, id, type) \
{TYPE_GUID_NAME(name) \
,id},

#define AAF_TYPE_DEFINITION_WEAK_REFERENCE(name, id, type) \
{TYPE_GUID_NAME(name) \
,id},

#define AAF_TYPE_DEFINITION_WEAK_REFERENCE_SET(name, id, type) \
{TYPE_GUID_NAME(name) \
,id},

#define AAF_TYPE_DEFINITION_WEAK_REFERENCE_VECTOR(name, id, type) \
{TYPE_GUID_NAME(name) \
,id},

#define AAF_TYPE_DEFINITION_STRONG_REFERENCE(name, id, type) \
{TYPE_GUID_NAME(name) \
,id},

#define AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(name, id, type) \
{TYPE_GUID_NAME(name) \
,id},

#define AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR(name, id, type) \
{TYPE_GUID_NAME(name) \
,id},

#define AAF_TYPE_DEFINITION_WEAK_REFERENCE(name, id, type) \
{TYPE_GUID_NAME(name) \
,id},

#define AAF_TYPE_DEFINITION_WEAK_REFERENCE_SET(name, id, type) \
{TYPE_GUID_NAME(name) \
,id},

#define AAF_TYPE_DEFINITION_WEAK_REFERENCE_VECTOR(name, id, type) \
{TYPE_GUID_NAME(name) \
,id},

#define AAF_TYPE_DEFINITION_STREAM(name, id) \
{TYPE_GUID_NAME(name) \
,id},

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

  // Some one is depending on this from the old way of implementing
  // the headers.
  cout << "#define TYPE_GUID_NAME(type) kAAFTypeID_##type" << endl;
  cout << endl;

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

#include <iostream.h>
#include <iomanip.h>
#include <assert.h>
#include <stdlib.h>

#include "headerGenUtils.h"

#define TYPE_GUID_NAME(type) cout << "kAAFTypeID_"; \
cout << #type;

#define AAF_TYPE_DEFINITION_INTEGER(name, id, size, signed) \
cout << endl; \
cout << "const aafUID_t "; TYPE_GUID_NAME(name) \
cout << " =" << endl; \
id

#define AAF_TYPE_DEFINITION_ENUMERATION(name, id, type) \
cout << endl; \
cout << "const aafUID_t " ; TYPE_GUID_NAME(name) \
cout << " =" << endl; \
id

#define AAF_TYPE_DEFINITION_RECORD(name, id) \
cout << endl; \
cout << "const aafUID_t " ; TYPE_GUID_NAME(name) \
cout << " =" << endl; \
id

#define AAF_TYPE_DEFINITION_VARYING_ARRAY(name, id, type) \
cout << endl; \
cout << "const aafUID_t " ; TYPE_GUID_NAME(name) \
cout << " =" << endl; \
id

#define AAF_TYPE_DEFINITION_FIXED_ARRAY(name, id, type, count) \
cout << endl; \
cout << "const aafUID_t " ; TYPE_GUID_NAME(name) \
cout << " =" << endl; \
id

#define AAF_TYPE_DEFINITION_RENAME(name, id, type) \
cout << endl; \
cout << "const aafUID_t " ; TYPE_GUID_NAME(name) \
cout << " =" << endl; \
id

#define AAF_TYPE_DEFINITION_STRING(name, id, type) \
cout << endl; \
cout << "const aafUID_t " ; TYPE_GUID_NAME(name) \
cout << " =" << endl; \
id

#define AAF_TYPE_DEFINITION_INDIRECT(name, id) \
cout << endl; \
cout << "const aafUID_t " ; TYPE_GUID_NAME(name) \
cout << " =" << endl; \
id

#define AAF_TYPE_DEFINITION_OPAQUE(name, id) \
cout << endl; \
cout << "const aafUID_t " ; TYPE_GUID_NAME(name) \
cout << " =" << endl; \
id

#define AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION(name, id) \
cout << endl; \
cout << "const aafUID_t " ; TYPE_GUID_NAME(name) \
cout << " =" << endl; \
id

#define AAF_TYPE_DEFINITION_CHARACTER(name, id) \
cout << endl; \
cout << "const aafUID_t " ; TYPE_GUID_NAME(name) \
cout << " =" << endl; \
id

#define AAF_TYPE_DEFINITION_SET(name, id, type) \
cout << endl; \
cout << "const aafUID_t " ; TYPE_GUID_NAME(name) \
cout << " =" << endl; \
id

#define AAF_TYPE_DEFINITION_STRONG_REFERENCE(name, id, type) \
cout << endl; \
cout << "const aafUID_t " ; TYPE_GUID_NAME(name) \
cout << " =" << endl; \
id

#define AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(name, id, type) \
cout << endl; \
cout << "const aafUID_t " ; TYPE_GUID_NAME(name) \
cout << " =" << endl; \
id

#define AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR(name, id, type) \
cout << endl; \
cout << "const aafUID_t " ; TYPE_GUID_NAME(name) \
cout << " =" << endl; \
id

#define AAF_TYPE_DEFINITION_WEAK_REFERENCE(name, id, type) \
cout << endl; \
cout << "const aafUID_t " ; TYPE_GUID_NAME(name) \
cout << " =" << endl; \
id

#define AAF_TYPE_DEFINITION_WEAK_REFERENCE_SET(name, id, type) \
cout << endl; \
cout << "const aafUID_t " ; TYPE_GUID_NAME(name) \
cout << " =" << endl; \
id

#define AAF_TYPE_DEFINITION_WEAK_REFERENCE_VECTOR(name, id, type) \
cout << endl; \
cout << "const aafUID_t " ; TYPE_GUID_NAME(name) \
cout << " =" << endl; \
id

#define AAF_TYPE_DEFINITION_STRONG_REFERENCE(name, id, type) \
cout << endl; \
cout << "const aafUID_t " ; TYPE_GUID_NAME(name) \
cout << " =" << endl; \
id

#define AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(name, id, type) \
cout << endl; \
cout << "const aafUID_t " ; TYPE_GUID_NAME(name) \
cout << " =" << endl; \
id

#define AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR(name, id, type) \
cout << endl; \
cout << "const aafUID_t " ; TYPE_GUID_NAME(name) \
cout << " =" << endl; \
id

#define AAF_TYPE_DEFINITION_WEAK_REFERENCE(name, id, type) \
cout << endl; \
cout << "const aafUID_t " ; TYPE_GUID_NAME(name) \
cout << " =" << endl; \
id

#define AAF_TYPE_DEFINITION_WEAK_REFERENCE_SET(name, id, type) \
cout << endl; \
cout << "const aafUID_t " ; TYPE_GUID_NAME(name) \
cout << " =" << endl; \
id

#define AAF_TYPE_DEFINITION_WEAK_REFERENCE_VECTOR(name, id, type) \
cout << endl; \
cout << "const aafUID_t " ; TYPE_GUID_NAME(name) \
cout << " =" << endl; \
id

#define AAF_TYPE_DEFINITION_STREAM(name, id) \
cout << endl; \
cout << "const aafUID_t " ; TYPE_GUID_NAME(name) \
cout << " =" << endl; \
id

#define AAF_TYPE(name) name
#define AAF_REFERENCE_TYPE(type, target)      AAF_TYPE(target##type)
#define AAF_REFERENCE_TYPE_NAME(type, target) AAF_TYPE(target##type)

#include "literalAuidDef.h"

static void doFile (const char * moduleName)
{
  assert (moduleName);
  cout << "#ifndef __" << moduleName << "_h__" << endl;
  cout << "#define __" << moduleName << "_h__" << endl;
  printCopyright (cout);
  cout << endl;
  cout << "#include \"AAFTypes.h\"" << endl;
  cout << endl;
#include "AAFMetaDictionary.h"
  cout << endl;
  cout << "#endif // ! __" << moduleName << "_h__" << endl;
}


void main (int argc, char ** argv)
{
  char * moduleName = 0;
  validateArgs (argc, argv, moduleName);
  // If validateArgs() returned (without exit()ing) then moduleName
  // must be properly filled in.
  assert (moduleName);
  doFile (moduleName);
  exit (0);
}

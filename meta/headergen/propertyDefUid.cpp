#include <iostream.h>
#include <assert.h>
#include <stdlib.h>

#include "headerGenUtils.h"

#define AAF_PROPERTY(name, id, tag, type, mandatory, uid, container) \
cout << endl; \
cout << "const aafUID_t kAAFPropID_" << #container \
     << "_" << #name << " =" << endl; \
id

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

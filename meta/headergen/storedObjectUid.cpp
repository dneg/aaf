#include <iostream.h>
#include <iomanip.h>
#include <assert.h>
#include <stdlib.h>

#include "headerGenUtils.h"

#define AAF_CLASS(name, dataid, parent, concrete) \
cout << endl; \
cout << "const aafUID_t AUID_AAF"; \
cout << #name; \
cout << " =" << endl; \
dataid

#include "literalAuidDef.h"

#ifdef AAF_CLASS_ALIAS
#undef AAF_CLASS_ALIAS
#endif

#define AAF_CLASS_ALIAS(name, alias) \
cout << endl; \
cout << "#define AUID_AAF" << #name << " AUID_AAF" << #alias << endl;

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

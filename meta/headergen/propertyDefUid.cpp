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
  cout << endl;
  cout << "//" << endl;
  cout << "// Special property ids used for specifying the starting strong reference in the" << endl;
  cout << "// targe list for weak references." << endl;
  cout << "//" << endl;
  cout << endl;
  cout << "// SMPTE Label: 06-0E-2B-34-01-01-01-02-0D-01-03-01-01-01-01-00" << endl;
  cout << "const aafUID_t kAAFPropID_Root_MetaDictionary =" << endl;
  cout << "{0x0D010301, 0x0101, 0x0100, {0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02}};" << endl;
  cout << endl;
  cout << "// SMPTE Label: 06-0E-2B-34-01-01-01-02-0D-01-03-01-01-02-01-00" << endl;
  cout << "const aafUID_t kAAFPropID_Root_Header =" << endl;
  cout << "{0x0D010301, 0x0102, 0x0100, {0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02}};" << endl;
  cout << endl;
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

#include <iostream.h>
#include <iomanip.h>
#include <assert.h>
#include <stdlib.h>

#include "headerGenUtils.h"

const char* prefix = "";

#define AAF_TYPE_TABLE_BEGIN() {
#define AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION(name, id) \
{1, #name, id},
#define AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_MEMBER(name,auid,container)\
{2, #name, auid},
#define AAF_TYPE_TABLE_END()   {0,0,{0}}};

struct {
  int kind;
  char* name;
  aafUID_t identifier;
} members [] =
#include "AAFMetaDictionary.h"

static void doFile (const char * moduleName)
{
  assert (moduleName);
  printBeginGuard(moduleName, cout);
  printCopyright (cout);

  cout << "// AAF extensible enumeration member UIDs." << endl
       << "//" << endl << endl;

  size_t i = 0;
  for (i = 0; i < sizeof(members)/sizeof(members[0]); i++) {
    if (members[i].kind == 1) {
      if ((i > 0) && (members[i-1].kind == 1)) {
        cout << "// None currently defined" << endl;
        cout << endl;
      }
      cout << "// Members of " << members[i].name << endl;
      cout << "//" << endl;
    } else if (members[i].kind == 2) {
      printDefinition("const aafUID_t",
                      prefix,
                      members[i].name,
                      members[i].identifier,
                      cout);
    }
  }

  printEndGuard(moduleName, cout);
}


void main (int argc, char ** argv)
{
  char * moduleName = 0;
  validateArgs (argc, argv, moduleName);
  assert (moduleName);
  doFile (moduleName);
  exit (0);
}

#include <iostream.h>
#include <iomanip.h>
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
    cout.flags(ios::left);
    cout.fill(' ');
    cout << "#define "
         << prefix << setw(width) << aliases[i].alias
         << " "
         << prefix << setw(width) << aliases[i].name
         << endl;
  }
  cout << endl;

  for (i = 0; i < sizeof(classes)/sizeof(classes[0]); i++){
    printDefinition("const aafUID_t",
                    prefix,
                    classes[i].name,
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

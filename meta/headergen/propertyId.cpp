#include <iostream.h>
#include <iomanip.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "headerGenUtils.h"

#define AAF_TABLE_BEGIN() {
#define AAF_PROPERTY(name, id, tag, type, mandatory, uid, container) \
{#container "_" #name, tag},
#define AAF_TABLE_END()   };

typedef struct {
  char* name;
  int identifier;
} Property_t;

Property_t properties[] =
#include "AAFMetaDictionary.h"

static void doFile(const char* moduleName)
{
  assert(moduleName);
  printBeginGuard(moduleName, cout);
  printCopyright(cout);
  cout << endl;

  size_t maxNameLength = 0;
  size_t i = 0;
  for (i = 0; i < sizeof(properties)/sizeof(properties[0]); i++) {
    size_t length = strlen(properties[i].name);
    if (length > maxNameLength) {
      maxNameLength = length;
    }
  }
  for (i = 0; i < sizeof(properties)/sizeof(properties[0]); i++) {
    printDefinition("const int",
                    "PID_",
                    properties[i].name,
                    maxNameLength,
                    properties[i].identifier,
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

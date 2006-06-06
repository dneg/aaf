#include <iostream>
#include <string>
using namespace std;

void genCode(const char* name, unsigned long int val, const char* desc)
{
  if (strlen(desc) != 0) {
    cout << "        ";
    cout << "/* ";
    cout << desc;
    cout << " */";
    cout << endl;
  }
  cout << "#define AAFRESULT_" << name;
  size_t len = strlen(name);
  const size_t codeAlign = 32;
  if (len < codeAlign) {
    size_t spaces = codeAlign - len;
    for (size_t i = 0; i < spaces; i++) {
      cout << " ";
    }
  } else {
    cout << " ";
  }
  cout << "0x" << hex << val + 0x80000000 + 0x120000;
  cout << endl;
}

int main()
{
#define COMMENT(s)
#define ERROR(name, val, desc) genCode(#name, val, desc);
#include "AAFErrorDefs.h"
}

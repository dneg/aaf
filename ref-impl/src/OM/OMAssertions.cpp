#include "OMAssertions.h"

#include <string.h>

#include <iostream.h>
#include <stdlib.h>

void reportAssertionFailure(char* kind,
                            char* name,
                            char* expressionString,
                            char* routine,
                            char* fileName,
                            size_t lineNumber)
{
  cerr << kind << " \"" << name << "\" failed in routine \""
       << routine  << "\"." << endl;
  cerr << "The failure occurred at line " << lineNumber
       << " in file \"" << fileName << "\"." << endl;
  cerr << "The condition \"" << expressionString << "\" was violated." << endl;
  exit(FAILURE);
}

bool validString(const char* string)
{
  return ((string != 0) && (strlen(string) > 0));
}

bool validWideString(const wchar_t* string)
{
  return (string != 0);
}

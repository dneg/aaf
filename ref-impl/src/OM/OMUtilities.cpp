#include <iostream.h>

#include "OMUtilities.h"

const char* programName = "Unknown program";

void setProgramName(const char* name)
{
  programName = name;
}

const char* getProgramName(void)
{
  return programName;
}

#include <iostream.h>
#include <iomanip.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "headerGenUtils.h"

void printBeginGuard(const char* moduleName, ostream& s)
{
  assert (moduleName);
  s << "#ifndef __" << moduleName << "_h__" << endl;
  s << "#define __" << moduleName << "_h__" << endl;
  s << endl;
}

void printEndGuard(const char* moduleName, ostream& s)
{
  assert(moduleName);
  s << endl;
  s << "#endif // ! __" << moduleName << "_h__" << endl;
}

void printCopyright (ostream & s)
{
  s <<
"//=---------------------------------------------------------------------=\n"
"//\n"
"// This file was GENERATED for the AAF SDK\n"
"//\n"
"// The contents of this file are subject to the AAF SDK Public\n"
"// Source License Agreement (the \"License\"); You may not use this file\n"
"// except in compliance with the License.  The License is available in\n"
"// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF\n"
"// Association or its successor.\n"
"//\n"
"// Software distributed under the License is distributed on an \"AS IS\"\n"
"// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See\n"
"// the License for the specific language governing rights and limitations\n"
"// under the License.\n"
"//\n"
"// The Original Code of this file is Copyright 1998-2002, Licensor of the\n"
"// AAF Association.\n"
"//\n"
"// The Initial Developer of the Original Code of this file and the\n"
"// Licensor of the AAF Association is Avid Technology.\n"
"// All rights reserved.\n"
"//\n"
"//=---------------------------------------------------------------------=\n";
  s << endl;
}


static void usage (const char * progname)
{
  assert (progname);

  cerr << "usage: " << progname << "moduleName" << endl;
}


void validateArgs (int argc, char** argv, char*& moduleName)
{
  const char * progname = argv[0];

  if (argc < 2) {
    usage (progname);
    exit (1);
  }

  moduleName = argv[1];
}

// print a GUID like this -
//{0x0D010101, 0x0101, 0x0100, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01)}
void print(const aafUID_t& id, ostream& s)
{
  s.setf(ios::uppercase);
  s.flags(ios::right);
  s << hex << setfill('0')
    << "{"
    << "0x" << setw(8) << id.Data1 << ", "
    << "0x" << setw(4) << id.Data2 << ", "
    << "0x" << setw(4) << id.Data3 << ", "
    << "{"
    << "0x" << setw(2) << (int)id.Data4[0] << ", "
    << "0x" << setw(2) << (int)id.Data4[1] << ", "
    << "0x" << setw(2) << (int)id.Data4[2] << ", "
    << "0x" << setw(2) << (int)id.Data4[3] << ", "
    << "0x" << setw(2) << (int)id.Data4[4] << ", "
    << "0x" << setw(2) << (int)id.Data4[5] << ", "
    << "0x" << setw(2) << (int)id.Data4[6] << ", "
    << "0x" << setw(2) << (int)id.Data4[7]
    << "}"
    << "}";
}

// print a GUID like this -
//{0101010D-0101-0100-060E-2B3402060101}
void printReg(const aafUID_t& id, ostream& s)
{
  s.setf(ios::uppercase);
  s.flags(ios::right);
  s << hex << setfill('0')
    << "{"
    << setw(8) << id.Data1 << "-"
    << setw(4) << id.Data2 << "-"
    << setw(4) << id.Data3 << "-"
    << setw(2) << (int)id.Data4[0] << setw(2) << (int)id.Data4[1] << "-"
    << setw(2) << (int)id.Data4[2]
    << setw(2) << (int)id.Data4[3]
    << setw(2) << (int)id.Data4[4]
    << setw(2) << (int)id.Data4[5]
    << setw(2) << (int)id.Data4[6]
    << setw(2) << (int)id.Data4[7]
    << "}";
}

void printDefinition(const char* type,
                     const char* prefix,
                     const char* name,
                     size_t width,
                     const aafUID_t& identifier,
                     ostream& s)
{
  s << "//";
  printReg(identifier, s);
  s << endl;
  s << type;
  s << " ";
  s << prefix;
  s << name;
  size_t actualWidth = strlen(prefix) + strlen(name);
  for (size_t i = actualWidth; i < width; i++) {
    s << " ";
  }
  s << " =" << endl;
  print(identifier, cout);
  s << ";" << endl << endl;
}

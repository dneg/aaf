#ifndef __headerGenUtils_h__
#define __headerGenUtils_h__
//=---------------------------------------------------------------------=
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
//
// The Original Code of this file is Copyright 1998-2002, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// Author : Tim Bingham (Tim_Bingham@avid.com) - after an initial
// implementation by Bob Tillman.


#include "AAFTypes.h"

#include <iostream.h>

void printBeginGuard(const char* module, ostream& s);

void printEndGuard(const char* module, ostream& s);

void printCopyright(ostream& s);

// Doesn't return if an error is found.
void validateArgs(int argc, char** argv, char*& moduleName);

// print a GUID like this -
//{0x0D010101, 0x0101, 0x0100,
//{0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01}}
void print(const aafUID_t& id, ostream& s);

// print a GUID like this -
//{0101010D-0101-0100-060E-2B3402060101}
void printReg(const aafUID_t& id, ostream& s);

void printDefinition(const char* type,
                     const char* prefix,
                     const char* name,
                     size_t width,
                     const aafUID_t& identifier,
                     ostream& s);

void printDefinition(const char* type,
                     const char* prefix,
                     const char* name,
                     size_t width,
                     int identifier,
                     ostream& s);

void printName(const char* name,
               size_t width,
               ostream& s);

#endif // ! __headerGenUtils_h__

#ifndef __headerGenUtils_h__
#define __headerGenUtils_h__

#include "AAFTypes.h"

class ostream;

void printBeginGuard(const char* module, ostream& s);

void printEndGuard(const char* module, ostream& s);

void printCopyright(ostream& s);

// Doesn't return if an error is found.
void validateArgs(int argc, char** argv, char*& moduleName);

// print a GUID like this -
//{0x0D010101, 0x0101, 0x0100, 0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01)}
void print(const aafUID_t& id, ostream& s);

// print a GUID like this -
//{0101010D-0101-0100-060E-2B3402060101}
void printReg(const aafUID_t& id, ostream& s);

void printDefinition(const char* type,
                     const char* prefix,
                     const char* name,
                     const aafUID_t& identifier,
                     ostream& s);

#endif // ! __headerGenUtils_h__

#ifndef OMPORTABILITY_H
#define OMPORTABILITY_H

#if defined (_MSC_VER) // Microsoft Visual C++

#if _MSC_VER <= 1000   // <= 1000 means earlier than version 5.0

#define typename class
typedef int bool;
const bool false = 0;
const bool true = 1;
#include <stdlib.h> // for size_t

#endif

typedef unsigned long DWORD;
typedef long HRESULT;

#elif defined (__MWERKS__)  // CodeWarrior

#include <stdlib.h> // for size_t

#elif defined (__GNUC__)  // GNU C++

#include <stdlib.h> // for size_t

#elif defined (__sgi)  // SGI C++

#define typename class
typedef int bool;
const bool false = 0;
const bool true = 1;
#include <stdlib.h> // for size_t

#endif

#endif


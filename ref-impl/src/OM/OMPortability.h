#ifndef OMPORTABILITY_H
#define OMPORTABILITY_H


#if _MSC_VER <= 1000
// <= 1000 means earlier than version 5.0

#define typename class
typedef int bool;
const bool false = 0;
const bool true = 1;

#endif

#elif defined (__MWERKS__)  // CodeWarrior


#elif defined (__GNUC__)    // GNU C++


#elif defined (__sgi)       // SGI C++

#define typename class
typedef int bool;
const bool false = 0;
const bool true = 1;

#endif

#endif


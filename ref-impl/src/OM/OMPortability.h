#ifndef OMPORTABILITY_H
#define OMPORTABILITY_H

#if defined (_MSC_VER)
#if _MSC_VER <= 1000
#define typename class
typedef int bool;
const bool false = 0;
const bool true = 1;
#include <stdlib.h>
#endif
#endif

#endif


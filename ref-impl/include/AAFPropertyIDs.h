#ifndef _AAFPropertyIDs_H_
#define _AAFPropertyIDs_H_

// AAF PID (property id) definitions.
//
// A property is actually identified by an AUID (a pseudo-GUID derived
// from the SMPTE unique identifier for the property). To save space in
// an AAF file we don't persist the 16 byte AUID with each property,
// instead we persist a PID.
//

// Define the AAF_PROPERTY macros to create PID definitions.
// Include the meta dictionary.
//
#define AAF_PROPERTY(name, id, tag, type, mandatory, container) \
const int PID_##container##_##name = tag;

#include "AAFMetaDictionary.h"

#undef AAF_PROPERTY

#endif

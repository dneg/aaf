#ifndef _AAFPropertyIDs_H_
#define _AAFPropertyIDs_H_

// AAF PID (property id) definitions.
//
// A property is actually identified by an AUID (a pseudo-GUID derived
// from the SMPTE unique identifier for the property). To save space in
// an AAF file we don't persist the 16 byte AUID with each property,
// instead we persist a PID.
//

// Define the AAF_PROPERTY macros to create PID definitions and then
// include the meta dictionary.
//
#define AAF_PROPERTY(name, id, tag, type, mandatory, container) \
const int PID_##container##_##name = tag;
#include "AAFMetaDictionary.h"
#undef AAF_PROPERTY

// Missing PIDs.
//
// Please don't add anything to this list. All PIDS should come from
// AAFMetaDictionary.h, these few are currently used in the reference
// implemetation but not defined in AAFMetaDictionary.h
//
const int PID_SOURCECLIP_FADEINPRESENT  = 0x12fe;
const int PID_SOURCECLIP_FADEOUTPRESENT = 0x12ff;
const int PID_TIMELINEMOBSLOT_ORIGIN    = 0x47ff;
const int PID_TIMECODE_TC               = 0x15ff;
const int PID_ESSENCEDATA_MEDIADATA     = 0x23ff;

#endif

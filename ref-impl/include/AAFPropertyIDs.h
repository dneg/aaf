#ifndef _AAFPropertyIDs_H_
#define _AAFPropertyIDs_H_

// Missing PIDs
//
const int PID_SOURCECLIP_FADEINPRESENT = 6;
const int PID_SOURCECLIP_FADEOUTPRESENT = 9;
const int PID_TIMELINEMOBSLOT_ORIGIN = 5;
const int PID_TIMECODE_TC = 5;
const int PID_ESSENCEDATA_MEDIADATA = 1;

#define AAF_PROPERTY(name, id, tag, type, mandatory, container) \
const int PID_##container##_##name = tag;
#include "AAFMetaDictionary.h"
#undef AAF_PROPERTY

#endif

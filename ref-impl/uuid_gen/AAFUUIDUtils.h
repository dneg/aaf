//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
//=---------------------------------------------------------------------=

#ifndef AAFUUIDUtils_h
#define AAFUUIDUtils_h

#include "AAFTypes.h"
#undef aafUUID_t


typedef struct
{
	aafInt8 nodeID[6];
} aafUUID_Node_t;


typedef struct _aafUUID_t
{
    aafUInt32         time_low;
    aafUInt16         time_mid;
    aafUInt16         time_hi_and_version;
    aafUInt8          clock_seq_hi_and_reserved;
    aafUInt8          clock_seq_low;
    aafUInt8          node[6];
} aafUUID_t;

/* aafUUIDCreate -- generate a UUID */
extern int aafUUIDCreate(aafUUID_t * uuid);

/* aafUUIDCreateFromName -- create a UUID using a "name" from a "name space" */
#ifndef OS_WINDOWS
extern void aafUUIDCreateFromName
(
	aafUUID_t * uuid,        /* resulting UUID */
	aafUUID_t nsid,          /* UUID to serve as context, so identical
                             names from different name spaces generate
                             different UUIDs */
	const void * name,          /* the name from which to generate a UUID */
	int namelen           /* the length of the name */
);
#endif

/* aafUUIDCompare --  Compare two UUID's "lexically" and return
          -1   u1 is lexically before u2
           0   u1 is equal to u2
           1   u1 is lexically after u2
     Note:   lexical ordering is not temporal ordering!
*/
extern int aafUUIDCompare(aafUUID_t *u1, aafUUID_t *u2);
#endif


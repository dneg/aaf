/************************************************************************
* StructuredStorage.h
* Public Header file for Structured Storage library
*
* (c) Schema Software Inc., 2001-2004
* Authors: Bob Sabiston, Yuri Khramov, Mark Ambachtsheer
************************************************************************

$Revision$
$Date$
*/
#include <StructuredStorage.h>
#include "OMRawStorage.h"


extern "C" int openStructuredStorageInOMRawStorage (
    const OMRawStorage* in_pRaw,
    StgMode in_accessMode,
    RootStorage** out_ppRoot);

extern "C" int createStructuredStorageInOMRawStorageEx (
    const OMRawStorage* in_pRaw,
    StgMode in_accessMode,
    RootStorage** out_ppRoot,
    unsigned long in_sectorSize);



/*****************************************************************************\
*                                                                             *
* initguid.h -	Definitions for controlling GUID initialization				  *
*                                                                             *
*               OLE Version 2.0                                               *
*                                                                             *
*               Copyright (c) 1992-1997, Microsoft Corp. All rights reserved. *
*                                                                             *
\*****************************************************************************/
/*
// Include after compobj.h to enable GUID initialization.  This
//              must be done once per exe/dll.
//
// After this file, include one or more of the GUID definition files.
//
// NOTE: ole2.lib contains references to all GUIDs defined by OLE.
*/

#define __INITGUID__
#define _INITGUID_H_

#ifndef DEFINE_GUID
#pragma error "initguid: must include compobj.h first."
#endif

#undef DEFINE_GUID

// Bug in the DR2 release of Metrowerks does not allow for extern "C" of data

#if defined(__MWERKS__)

#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
    extern const GUID name = { l, w1, w2, b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 }

#elif 1

#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
    EXTERN_C const GUID name = { l, w1, w2, b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 }

#else

//#pragma code_seg("_FAR_DATA")
//#pragma data_seg("_FAR_DATA")

#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) \
    EXTERN_C const __declspec(allocate("_FAR_DATA")) GUID name = { l, w1, w2, b1, b2,  b3,  b4,  b5,  b6,  b7,  b8 }

#endif

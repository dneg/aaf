#ifndef __AxStorageErrors_h__
#define __AxStorageErrors_h__

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

/*
 +---------------------------------------------------------------
 *
 *  Microsoft Windows
 *  Copyright (C) Microsoft Corporation, 1992 - 1996.
 *
 *  File:       storage.h
 *
 *  Contents:   This is the main file to include to use the reference
 *              implementation.
 *
 *  Note:       Some of the defintions have been changed to TCHAR
 *              instead of WCHAR, to add support for ANSI APIs.
 *              TCHAR becomes WCHAR with _UNICODE defined, and char
 *              otherwise. (See tchar.h for details).
 *
 *--------------------------------------------------------------- 
 */

// The following error code definitions were copied from
// AAF/ss-impl/ref/h/storage.h The are used to build the AxHrMap.  The
// storage.h file is problematic to include directly.  It has macro
// definitions that conflict with those in AAFCOMPlatform.h, uses
// undefined types, pulls in numerous other header files, etc - all of
// which are unnecessary insofar as these error codes are concerned.

#if !defined(OS_WINDOWS)

// Pick up the MAKE_SCODE macro from AAFResult.h
#include <AAFResult.h>

// The following from ss-impl/ref/h/ref.hxx
#define FACILITY_STORAGE    0x0003 // storage errors (STG_E_*)

/****** Storage Error Codes *************************************************/

/* DOS-based error codes */
#define STG_E_INVALIDFUNCTION \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x01)

#define STG_E_FILENOTFOUND \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x02)

#define STG_E_PATHNOTFOUND \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x03)

#define STG_E_TOOMANYOPENFILES \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x04)

#define STG_E_ACCESSDENIED \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x05)

#define STG_E_INVALIDHANDLE \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x06)

#define STG_E_INSUFFICIENTMEMORY \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x08)

#define STG_E_INVALIDPOINTER \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x09)

#define STG_E_NOMOREFILES \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x12)

#define STG_E_DISKISWRITEPROTECTED \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x13)

#define STG_E_SEEKERROR \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x19)

#define STG_E_SHAREVIOLATION \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x20)

#define STG_E_WRITEFAULT \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x1d)

#define STG_E_READFAULT \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x1e)

#define STG_E_LOCKVIOLATION \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x21)

#define STG_E_FILEALREADYEXISTS \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x50)

#define STG_E_INVALIDPARAMETER \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x57)

#define STG_E_MEDIUMFULL \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x70)

#define STG_E_ABNORMALAPIEXIT \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0xfa)

#define STG_E_INVALIDHEADER \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0xfb)

#define STG_E_INVALIDNAME \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0xfc)

#define STG_E_UNKNOWN \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0xfd)

#define STG_E_UNIMPLEMENTEDFUNCTION\
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0xfe)

#define STG_E_INVALIDFLAG \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0xff)

/* Standard storage error codes */
#define STG_E_INUSE \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x100)

#define STG_E_NOTCURRENT \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x101)

#define STG_E_REVERTED \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x102)

#define STG_E_CANTSAVE \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x103)

#define STG_E_OLDFORMAT \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x104)
	
#define STG_E_OLDDLL \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x105)

#define STG_E_SHAREREQUIRED \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x106)

#define STG_E_NOTFILEBASEDSTORAGE \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x107)

#define STG_E_DOCFILECORRUPT \
    MAKE_SCODE(SEVERITY_ERROR, FACILITY_STORAGE, 0x109)

/* Information returns */
#define STG_S_CONVERTED \
    MAKE_SCODE(SEVERITY_SUCCESS, FACILITY_STORAGE, 0x200)


// endif !defined(OS_WINDOWS)
#endif

// These two don't seem to be defined in the Windoze header files.
#define STG_S_BUFFEROVERFLOW \
    MAKE_SCODE(SEVERITY_SUCCESS, FACILITY_STORAGE, 0x201)

#define STG_S_TRYOVERWRITE \
    MAKE_SCODE(SEVERITY_SUCCESS, FACILITY_STORAGE, 0x202)    


#endif

/*****************************************************************************\
*                                                                             *
* ole2guid.c 												               	  *
*                                                                             *
*               OLE Version 2.0                                               *
*                                                                             *
*               Copyright (c) 1992-1997, Microsoft Corp. All rights reserved. *
*                                                                             *
\*****************************************************************************/



#define INITGUID

#if !defined(_MAC)
#define _MAC
#endif // _MAC

#if !defined(CDECL) && defined(_MSC_VER)
#define CDECL	_cdecl
#endif // CDECL

#if !defined(FAR)
#define FAR
#endif

#include "compobj.h"
#include "initguid.h"
#include "coguid.h"
#include "oleguid.h"
#include "dispatch.h"


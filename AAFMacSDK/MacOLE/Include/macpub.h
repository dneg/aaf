/*****************************************************************************\
*                                                                             *
* macpub.h - 	Includes standard Macintosh header files.               	  *
*                                                                             *
*               OLE Version 2.0                                               *
*                                                                             *
*               Copyright (c) 1992-1997, Microsoft Corp. All rights reserved. *
*                                                                             *
\*****************************************************************************/


#if !defined(__MACPUB__) && !defined(_MACPUB_H_)
#define __MACPUB__
#define _MACPUB_H_

/* for those who don't want includes , define NO_INCLUDES */
#ifndef NO_INCLUDES

#ifndef __STDLIB__
#include <stdlib.h>
#endif

#ifndef __FILES__
#include <files.h>
#endif

#ifndef __MENUS__
#include <menus.h>
#endif

#ifndef __PROCESSES__
#ifdef _MSC_VER
#include <processe.h>
#else
#include <processes.h>
#endif
#endif

#ifndef __APPLEEVENTS__
#ifdef _MSC_VER
#include <AppleEve.h>
#else
#include <AppleEvents.h>
#endif
#endif


#ifndef __MACTYPES__
#include <MacTypes.h>
#endif

#ifndef __QUICKDRAW__
#ifdef _MSC_VER
#include <Quickdra.h>
#else
#include <Quickdraw.h>
#endif
#endif


#ifndef __EVENTS__
#include <events.h>
#endif

#ifndef __DRAG__
#include <drag.h>
#endif

#ifndef _MSC_VER
#ifndef THINK_C
#ifndef __SC__
#ifndef __STRINGS__
#include <strings.h>
#endif
#endif
#endif
#endif

#ifndef __STRING__
#include <string.h>
#endif

#endif /* NO_INCLUDES */


#ifndef __MACDEF__
#include <macdef.h>
#endif

#endif

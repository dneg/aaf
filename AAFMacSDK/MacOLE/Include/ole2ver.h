/*****************************************************************************\
*                                                                             *
* ole2ver.h - 	OLE version information.                                	  *
*                                                                             *
*               OLE Version 2.0                                               *
*                                                                             *
*               Copyright (c) 1992-1997, Microsoft Corp. All rights reserved. *
*                                                                             *
\*****************************************************************************/



#if !defined(__OLE2VER__) && !defined(_OLE2VER_H_)
#define __OLE2VER__
#define _OLE2VER_H_

/* these are internal build numbers
// the hiword changes when glue or headers are incompatible w/ previous drops
// the loword increments between builds.
*/
#define OLE_VERS_HIWORD	29

#ifdef __powerc
#define OLE_VERS_LOWORD	12
#else
#define OLE_VERS_LOWORD	3
#endif // __powerc

#define rmm		OLE_VERS_HIWORD
#define rup		OLE_VERS_LOWORD	/* this must fit in ONE byte */

#ifdef _REZ

#define OLE_STAGE	final
#define OLE_VERSTRING	"2.24"

#define majorRev		2
#define minorRev		0x24
#define nonfinalRev		0

#ifdef USE_OLE2_VERS
resource 'vers' (1) {
	majorRev, minorRev, OLE_STAGE, nonfinalRev,
	verUS,
	OLE_VERSTRING,
	OLE_VERSTRING", Copyright © 1992-98 Microsoft Corporation"
};
resource 'vers' (2) {
	majorRev, minorRev, OLE_STAGE, nonfinalRev,
	verUS,
	OLE_VERSTRING,
	"Microsoft OLE 2.2"
};
#endif /* USE_OLE2_VERS */

#endif /* _REZ */

#endif /* !__OLE2VER__ */

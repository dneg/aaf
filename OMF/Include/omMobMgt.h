/***********************************************************************
 *
 *              Copyright (c) 1996 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and to distribute
 * and sublicense application software incorporating this software for
 * any purpose is hereby granted, provided that (i) the above
 * copyright notice and this permission notice appear in all copies of
 * the software and related documentation, and (ii) the name Avid
 * Technology, Inc. may not be used in any advertising or publicity
 * relating to the software without the specific, prior written
 * permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT, CONSEQUENTIAL OR OTHER DAMAGES OF
 * ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE, INCLUDING, 
 * WITHOUT  LIMITATION, DAMAGES RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, AND WHETHER OR NOT ADVISED OF THE POSSIBILITY OF
 * DAMAGE, REGARDLESS OF THE THEORY OF LIABILITY.
 *
 ************************************************************************/

#ifndef _OMF_MOB_MNGT_
#define _OMF_MOB_MNGT_ 1

#include "omErr.h"
#include "omTypes.h"

#if PORT_LANG_CPLUSPLUS
extern "C" 
  {
#endif

/* start of prototypes */

OMF_EXPORT omfErr_t omfiMobCopy(
	omfHdl_t file,          /* IN - File Handle */
	omfMobObj_t srcMob,     /* IN - Source Mob */
	omfString destMobName,  /* IN - Destination Mob Name (optional) */
	omfMobObj_t *destMob);  /* OUT - Destination Mob */

OMF_EXPORT omfErr_t omfiMobCopyExternal(
    omfHdl_t srcFile,                /* IN - Source File Handle */
	omfMobObj_t srcMob,              /* IN - Source Mob */
	omfDepend_t resolveDependencies, /* IN - Whether to copy dependent mobs */
	omfIncMedia_t includeMedia,      /* IN - Whether to include media data */
	omfString destMobName,           /* IN - Destination Mob Name */
	omfHdl_t destFile,               /* IN - Destination File Handle */
	omfObject_t *destMedia,          /* OUT - Destination Media Data */
	omfMobObj_t *destMob);           /* OUT - Destination Mob */

OMF_EXPORT omfErr_t omfiMobCloneExternal(
    omfHdl_t srcFile,                 /* IN - Source File Handle */
	omfMobObj_t srcMob,               /* IN - Source Mob */
	omfDepend_t resolveDependencies,  /* IN - Whether to clone dependent mobs*/
	omfIncMedia_t includeMedia,       /* IN - Whether to include media data */
	omfHdl_t destFile,                /* IN - Destination File Handle */
	omfMobObj_t *destMob);            /* OUT - Destination Mob */

OMF_EXPORT omfErr_t omfiMobDelete(
	omfHdl_t file,   /* IN - File Handle */
	omfUID_t mobID); /* IN - Mob ID of mob to delete */

OMF_EXPORT omfErr_t omfiObjectDeleteTree(
    omfHdl_t file,    /* IN - File Handle */
	omfObject_t obj); /* IN - Object to delete */

OMF_EXPORT omfErr_t omfiObjectCopyTree(
	omfHdl_t file,        /* IN - File Handle */
	omfObject_t srcObj,   /* IN - Object to copy */
	omfObject_t *destObj);/* OUT - Destination object */

OMF_EXPORT omfErr_t omfiObjectCopyTreeExternal(
    omfHdl_t srcFile,      /* IN - Source File Handle */
	omfObject_t srcObj,    /* IN - Source object */
	omfIncMedia_t includeMedia,  /* IN - Whether to include media data */
    omfDepend_t resolveDependencies, /* IN - Whether to resolve mob dependencies */
	omfHdl_t destFile,     /* IN - Destination File Handle */
	omfObject_t *destObj); /* OUT - Destination object */

OMF_EXPORT omfBool omfiIsMobInFile(
	omfHdl_t file,       /* IN - File Handle */
	omfUID_t mobID,      /* IN - Mob ID */
	omfObject_t *mob,    /* OUT - Found mob */
	omfErr_t *omfError); /* OUT - Error status */

OMF_EXPORT omfErr_t omfiFileGetNextDupMobs(
    omfIterHdl_t iterHdl,   /* IN - Iterator Handle */
	omfUID_t *mobID,        /* OUT - Mob ID of duplicate mobs */
    omfInt32 *numMatches,   /* OUT - Number of duplicate mobs with mobID */
	omfMobObj_t **mobList);  /* OUT - List of Object References to mobs */

OMF_EXPORT omfErr_t omfiMobChangeRef(
    omfHdl_t file,        /* IN - File Handle */
	omfMobObj_t mob,      /* IN - Mob to traverse */
	omfUID_t oldMobID,    /* IN - Old Mob ID reference in source clip */
	omfUID_t newMobID);   /* IN - New Mob ID reference in source clip */

/* end of prototypes */

#if PORT_LANG_CPLUSPLUS
  }
#endif
#endif /* _OMF_MOB_MNGT_ */

/* INDENT OFF */
/*
;;; Local Variables: ***
;;; tab-width:4 ***
;;; End: ***
*/

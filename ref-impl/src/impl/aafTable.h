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

#ifndef _AAF_TABLE_API_
#define _AAF_TABLE_API_ 1

#include "aafErr.h"
#include "AAFTypes.h"

#if PORT_LANG_CPLUSPLUS
extern          "C"
{
#endif

#define USE_DEFAULT		0

typedef enum
{
	kOmTableDupError,
	kOmTableDupReplace,
	kOmTableDupAddDup
} omTableDuplicate_t;

typedef enum
{
	kTableSrchAny,
	kTableSrchMatch,
	kTableSrchUnique
} omTableSearch_t;

typedef aafInt32	(*omTblMapProc)( void *key);
typedef aafBool	(*omTblCompareProc)( void *key1, void *key2);
typedef void	(*omTblDisposeProc)(void *valuePtr);

typedef struct omTable omTable_t;
typedef struct aafTableLink tableLink_t;

typedef struct
{
	aafInt32				cookie;		/* Private */
	omTable_t			*table;		/* Private */
	aafInt32				curHash;	/* Private */
	tableLink_t			*nextEntry;	/* Private */
	omTableSearch_t		srch;		/* Private */
	void				*srchKey;	/* Private */
	/* */
	void				*key;		/* OUT */
	aafInt32				keylen;		/* OUT */
	void				*valuePtr;	/* OUT */
	aafInt32				valueLen;	/* OUT */
} omTableIterate_t;

/************************************************************************
 *
 * Root Table Functions (used to implement specializations below)
 *
 * If you need a hash table, create a specialized type, either
 * below or in your code.  A compare callback and a hash callback will
 * need to be provided when the table is created if you use NewTable
 * directly
 *
 ************************************************************************/

aafErr_t NewTable(
			AAFFile * file,
			aafInt16 initKeySize,
			omTblMapProc myMap,
			omTblCompareProc myCompare,
			aafInt32 numBuckets,
			omTable_t **resultPtr);
			
aafErr_t SetTableDispose(
			omTable_t *table,
			omTblDisposeProc proc);
			
aafErr_t TableAddValuePtr(
			omTable_t *table,
			void *key,
			aafInt16 keyLen,
			void *value,
			omTableDuplicate_t dup);
			
aafErr_t TableAddValueBlock(
			omTable_t *table,
			void *key,
			short keyLen,
			void *value,
			aafInt32 valueLen,
			omTableDuplicate_t dup);
			
aafErr_t TableRemove(
			omTable_t *table,
			void *key);
			
aafBool TableIncludesKey(
			omTable_t *table,
			void *key);

void *TableLookupPtr(
			omTable_t *table,
			void *key);
			
aafErr_t TableLookupBlock(
			omTable_t *table,
			void *key,
			aafInt32 valueLen,
			void *valuePtr,
			aafBool *found);
			
aafErr_t TableFirstEntry(
			omTable_t *table,
			omTableIterate_t *iter,
			aafBool *found);
			
aafErr_t TableFirstEntryMatching(
			omTable_t *table,
			omTableIterate_t *iter,
			void *key,
			aafBool *found);
			
aafErr_t TableFirstEntryUnique(
			omTable_t *table,
			omTableIterate_t *iter,
			aafBool *found);
			
aafErr_t TableNextEntry(
			omTableIterate_t *iter,
			aafBool *found);

aafInt32 TableNumEntriesMatching(
			omTable_t *table,
			void *key);
			
aafErr_t TableSearchDataValue(
			omTable_t *table,
			aafInt32 valueLen,
			void *value,
			aafInt32 keyLen,
			void *key,
			aafBool *found);
			
aafErr_t TableDispose(
			omTable_t *table);
			
aafErr_t TableDisposeAll(
			omTable_t *table);
			
aafErr_t TableDisposeItems(
			omTable_t *table);


/************************************************************************
 *
 * String Table Functions
 *
 ************************************************************************/

aafErr_t NewStringTable(
			AAFFile * file,
			aafBool caseSensistive,
			omTblCompareProc myCompare,
			aafInt32 numBuckets,
			omTable_t **resultPtr);
			
aafErr_t TableAddString(
			omTable_t *table,
			char *key,
			void *value,
			omTableDuplicate_t dup);
			
aafErr_t TableAddStringBlock(
			omTable_t *table,
			char *key,
			void *value,
			aafInt32 valueLen,
			omTableDuplicate_t dup);

/************************************************************************
 *
 * UID Table Functions
 *
 ************************************************************************/

aafErr_t NewUIDTable(
			AAFFile * file,
			aafInt32 numBuckets,
			omTable_t **result);
			
aafErr_t TableAddUID(
			omTable_t *table,
			aafUID_t key,
			void *value,
			omTableDuplicate_t dup);
			
aafErr_t TableRemoveUID(
			omTable_t *table,
			aafUID_t key);
			
aafBool TableIncludesUID(
			omTable_t *table,
			aafUID_t key);
			
void *TableUIDLookupPtr(
			omTable_t *table,
			aafUID_t key);

/************************************************************************
 *
 * ClassID Table Functions
 *
 ************************************************************************/

#if FULL_TOOLKIT
aafErr_t NewClassIDTable(
			AAFFile * file,
			aafInt32 numBuckets,
			omTable_t **result);

aafErr_t TableAddClassID(
			omTable_t *table,
			aafClassIDPtr_t key,
			void *value,
			aafInt32 valueLen);
			
aafErr_t TableClassIDLookup(
			omTable_t *table,
			aafClassIDPtr_t key,
			aafInt32 valueLen,
			void *valuePtr,
			aafBool *found);
#endif
/************************************************************************
 *
 * TrackID Table Functions
 *
 ************************************************************************/

aafErr_t NewTrackIDTable(
			AAFFile * file,
			aafInt32 numBuckets,
			omTable_t **result);
			
aafErr_t TableAddTrackID(
			omTable_t *table,
			aafTrackID_t key,
			void *value,
			aafInt32 valueLen);
			
aafErr_t TableTrackIDLookup(
			omTable_t *table,
			aafTrackID_t key,
			void *value,
			aafInt32 valueLen,
			aafBool *found);

/************************************************************************
 *
 * aafProperty_t Table Functions
 *
 ************************************************************************/

#if FULL_TOOLKIT
aafErr_t NewPropertyTable(
			AAFFile * file,
			aafInt32 numBuckets,
			omTable_t **result);
			
aafErr_t TableAddProperty(
			omTable_t *table,
			aafProperty_t key,
			void *value, 
			aafInt32 valueLen,
			omTableDuplicate_t dup);
			
aafErr_t TablePropertyLookup(
			omTable_t *table,
			aafProperty_t key,
			aafInt32 valueLen,
			void *valuePtr,
			aafBool *found);
#endif

/************************************************************************
 *
 * aafType_t Table Functions
 *
 ************************************************************************/

#if FULL_TOOLKIT
aafErr_t NewTypeTable(
			AAFFile * file,
			aafInt32 numBuckets,
			omTable_t **result);
			
aafErr_t TableAddType(
			omTable_t *table,
			aafType_t key,
			void *value,
			aafInt32 valueLen);
			
aafErr_t TableTypeLookup(
			omTable_t *table,
			aafType_t key,
			aafInt32 valueLen,
			void *valuePtr,
			aafBool *found);
#endif

/************************************************************************
 *
 * Definition (Datakind and EffectDef) Table Functions
 *
 ************************************************************************/

#if FULL_TOOLKIT
aafErr_t NewDefTable(
			AAFFile * file,
			aafInt32 numBuckets,
			omTable_t **result);
			
aafErr_t TableAddDef(
			omTable_t *table,
			aafUniqueName_t key,
			void *value);
			
void *TableDefLookup(
			omTable_t *table,
			aafUniqueName_t key);
#endif

#ifdef AAF_SELF_TEST
void testOmTable(void);
#endif

#if PORT_LANG_CPLUSPLUS
}
#endif
#endif				/* _AAF_TABLE_API_ */

/* INDENT OFF */
/*
;;; Local Variables: ***
;;; tab-width:4 ***
;;; End: ***
*/

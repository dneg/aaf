/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 *  prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/

#ifndef _AAF_TABLE_API_
#define _AAF_TABLE_API_ 1

#include "aafErr.h"
#include "AAFTypes.h"

#ifdef __cplusplus
extern          "C"
{
#endif

#define USE_DEFAULT		0

typedef enum
{
	kAafTableDupError,
	kAafTableDupReplace,
	kAafTableDupAddDup
} aafTableDuplicate_t;

typedef enum
{
	kTableSrchAny,
	kTableSrchMatch,
	kTableSrchUnique
} aafTableSearch_t;

typedef aafInt32	(*aafTblMapProc)( void *key);
typedef aafBool	(*aafTblCompareProc)( void *key1, void *key2);
typedef void	(*aafTblDisposeProc)(void *valuePtr);

typedef struct aafTable aafTable_t;
typedef struct aafTableLink tableLink_t;

typedef struct
{
	aafInt32				cookie;		/* Private */
	aafTable_t			*table;		/* Private */
	aafInt32				curHash;	/* Private */
	tableLink_t			*nextEntry;	/* Private */
	aafTableSearch_t		srch;		/* Private */
	void				*srchKey;	/* Private */
	/* */
	void				*key;		/* OUT */
	aafInt32				keylen;		/* OUT */
	void				*valuePtr;	/* OUT */
	aafInt32				valueLen;	/* OUT */
} aafTableIterate_t;

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
			aafTblMapProc myMap,
			aafTblCompareProc myCompare,
			aafInt32 numBuckets,
			aafTable_t **resultPtr);
			
aafErr_t SetTableDispose(
			aafTable_t *table,
			aafTblDisposeProc proc);
			
aafErr_t TableAddValuePtr(
			aafTable_t *table,
			void *key,
			aafInt16 keyLen,
			void *value,
			aafTableDuplicate_t dup);
			
aafErr_t TableAddValueBlock(
			aafTable_t *table,
			void *key,
			short keyLen,
			void *value,
			aafInt32 valueLen,
			aafTableDuplicate_t dup);
			
aafErr_t TableRemove(
			aafTable_t *table,
			void *key);
			
aafBool TableIncludesKey(
			aafTable_t *table,
			void *key);

void *TableLookupPtr(
			aafTable_t *table,
			void *key);
			
aafErr_t TableLookupBlock(
			aafTable_t *table,
			void *key,
			aafInt32 valueLen,
			void *valuePtr,
			aafBool *found);
			
aafErr_t TableFirstEntry(
			aafTable_t *table,
			aafTableIterate_t *iter,
			aafBool *found);
			
aafErr_t TableFirstEntryMatching(
			aafTable_t *table,
			aafTableIterate_t *iter,
			void *key,
			aafBool *found);
			
aafErr_t TableFirstEntryUnique(
			aafTable_t *table,
			aafTableIterate_t *iter,
			aafBool *found);
			
aafErr_t TableNextEntry(
			aafTableIterate_t *iter,
			aafBool *found);

aafInt32 TableNumEntriesMatching(
			aafTable_t *table,
			void *key);
			
aafErr_t TableSearchDataValue(
			aafTable_t *table,
			aafInt32 valueLen,
			void *value,
			aafInt32 keyLen,
			void *key,
			aafBool *found);
			
aafErr_t TableDispose(
			aafTable_t *table);
			
aafErr_t TableDisposeAll(
			aafTable_t *table);
			
aafErr_t TableDisposeItems(
			aafTable_t *table);


/************************************************************************
 *
 * String Table Functions
 *
 ************************************************************************/

aafErr_t NewStringTable(
			AAFFile * file,
			aafBool caseSensistive,
			aafTblCompareProc myCompare,
			aafInt32 numBuckets,
			aafTable_t **resultPtr);
			
aafErr_t TableAddString(
			aafTable_t *table,
			char *key,
			void *value,
			aafTableDuplicate_t dup);
			
aafErr_t TableAddStringBlock(
			aafTable_t *table,
			char *key,
			void *value,
			aafInt32 valueLen,
			aafTableDuplicate_t dup);

/************************************************************************
 *
 * UID Table Functions
 *
 ************************************************************************/

aafErr_t NewUIDTable(
			AAFFile * file,
			aafInt32 numBuckets,
			aafTable_t **result);
			
aafErr_t TableAddUID(
			aafTable_t *table,
			aafUID_t key,
			void *value,
			aafTableDuplicate_t dup);
			
aafErr_t TableAddUIDBlock(
			aafTable_t *table,
			aafUID_t key,
			void *value,
			aafInt32 valueLen,
			aafTableDuplicate_t dup);
			
aafErr_t TableRemoveUID(
			aafTable_t *table,
			aafUID_t key);
			
aafBool TableIncludesUID(
			aafTable_t *table,
			aafUID_t key);
			
void *TableUIDLookupPtr(
			aafTable_t *table,
			aafUID_t key);

void TableUIDLookupBlock(
			aafTable_t *table,
			aafUID_t key,
			aafInt32 valueLen,
			void *valuePtr,
			aafBool *found);

/************************************************************************
 *
 * SlotID Table Functions
 *
 ************************************************************************/

aafErr_t NewSlotIDTable(
			AAFFile * file,
			aafInt32 numBuckets,
			aafTable_t **result);
			
aafErr_t TableAddSlotID(
			aafTable_t *table,
			aafSlotID_t key,
			void *value,
			aafInt32 valueLen);
			
aafErr_t TableSlotIDLookup(
			aafTable_t *table,
			aafSlotID_t key,
			void *value,
			aafInt32 valueLen,
			aafBool *found);

#ifdef AAF_SELF_TEST
void testaafTable(void);
#endif

#ifdef __cplusplus
}
#endif
#endif				/* _AAF_TABLE_API_ */

/* INDENT OFF */
/*
;;; Local Variables: ***
;;; tab-width:4 ***
;;; End: ***
*/

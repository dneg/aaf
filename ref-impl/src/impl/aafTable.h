//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


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
 * MobID Table Functions
 *
 ************************************************************************/

aafErr_t NewMobIDTable(
			aafInt32 numBuckets,
			aafTable_t **result);
			
aafErr_t TableAddMobID(
			aafTable_t *table,
			aafMobID_constref key,
			void *value,
			aafTableDuplicate_t dup);
			
aafErr_t TableAddMobIDBlock(
			aafTable_t *table,
			aafMobID_constref key,
			void *value,
			aafInt32 valueLen,
			aafTableDuplicate_t dup);
			
aafErr_t TableRemoveMobID(
			aafTable_t *table,
			aafMobID_constref key);
			
aafBool TableIncludesMobID(
			aafTable_t *table,
			aafMobID_constref key);
			
void *TableMobIDLookupPtr(
			aafTable_t *table,
			aafMobID_constref key);

void TableMobIDLookupBlock(
			aafTable_t *table,
			aafMobID_constref key,
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

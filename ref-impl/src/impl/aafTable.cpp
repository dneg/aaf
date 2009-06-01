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


/*
 * Name: aafTable.c
 *
 * Function: Internal functions which implement a hash table with extensions.
 *			This hash table implementation can be made to allow duplicate
 *			entries, and can iterate over all unique entries (skipping
 *			duplicates), or iterate over all entries matching a particular
 *			key.  In addition, functions exist to iterate over all entries
 *			in the has table, or to search for a particular data value and
 *			return the key.
 *
 * Audience: Toolkit internal.
 *
 * General error codes returned:
 */

//#include "masterhd.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>

//#include "aafPublic.h"
#include "aafTable.h"
#include "AAFResult.h"

#define TABLE_COOKIE		0x5461626C
#define TABLE_ITER_COOKIE	0x54424C49

// On both 32bit and 64bit CPUs, unsigned long can hold a void *
#define _lookup(table,key) 	( (table->map ? (unsigned long)table->map(key) : (unsigned long)key) % table->hashTableSize)

typedef enum
{
	valueIsPtr, valueIsBlock
} valueType_t;

struct aafTableLink
{
	tableLink_t			*link;
	tableLink_t			*dup;
	void				*data;
	aafInt16			keyLen;
	aafInt32			valueLen;
	valueType_t			type;
	char				local[1];
};

struct aafTable
{
	AAFFile *			file;			/* Optional: If set aafOptMalloc/Free calls will optimize */
	aafInt32			cookie;
	aafInt16			defaultSize;	/* default size of keys */
	tableLink_t		**hashTable;
	aafInt32			hashTableSize;
	aafInt32			numItems;
				
	aafTblMapProc		map;			/* the mapping function			*/
	aafTblCompareProc	compare;		/* the comparison function		*/
	aafTblDisposeProc	entryDispose;
};
	
static aafErr_t DisposeList(aafTable_t *table, aafBool itemsAlso);

aafErr_t NewDefTable(
			AAFFile * file,
			aafInt32 numBuckets,
			aafTable_t **result);

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

/************************
 * name
 *
 * 		WhatIt(Internal)Does
 *
 * Argument Notes:
 *		StuffNeededBeyondNotesInDefinition.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
aafErr_t NewTable(
			AAFFile * file,
			aafInt16 initKeySize,
			aafTblMapProc myMap,
			aafTblCompareProc mycompare,
			aafInt32 numBuckets,
			aafTable_t **resultPtr)
{
	aafTable_t	*result;
	
	XPROTECT()
	{
		result = new aafTable_t;
		XASSERT(result != NULL, AAFRESULT_NOMEMORY);
		
		result->cookie = TABLE_COOKIE;
		result->file = file;
		result->map = myMap;
		result->compare = mycompare;
		result->entryDispose = NULL;
		result->defaultSize = initKeySize;
		result->hashTableSize = numBuckets;
		result->hashTable = (tableLink_t **)new char[result->hashTableSize * sizeof(tableLink_t *)];
		memset(result->hashTable, 0, result->hashTableSize * sizeof(tableLink_t *));
		result->numItems = 0;
	}
	XEXCEPT
	{
	}
	XEND

	*resultPtr = result;
	return(AAFRESULT_SUCCESS);
}	

/* Called once for each entry, used to dispose only memory referenced by the entry */
aafErr_t SetTableDispose(aafTable_t *table, aafTblDisposeProc proc)
{
	XPROTECT()
	{
		XASSERT((table != NULL) && (table->cookie == TABLE_COOKIE), 
				AAFRESULT_TABLE_BAD_HDL);
		table->entryDispose = proc;
	}
	XEXCEPT
	XEND
	
	return(AAFRESULT_SUCCESS);
}

/************************
 * name
 *
 * 		WhatIt(Internal)Does
 *
 * Argument Notes:
 *		StuffNeededBeyondNotesInDefinition.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
aafErr_t TableAddValuePtr(
			aafTable_t *table,
			void *key,
			aafInt16 keyLen,
			void *value,
			aafTableDuplicate_t dup)
{
	tableLink_t		*entry, *srch;
  	aafInt32			hash;
				
	XPROTECT()
	{
		XASSERT((table != NULL) && (table->cookie == TABLE_COOKIE), 
				AAFRESULT_TABLE_BAD_HDL);
		entry = NULL;
	
		if(keyLen == USE_DEFAULT)
			keyLen = table->defaultSize;
		if((dup == kAafTableDupError) && TableIncludesKey(table, key))
			return(AAFRESULT_TABLE_DUP_KEY);
	
		if((dup == kAafTableDupReplace) && TableIncludesKey(table, key))
		{
			CHECK(TableRemove(table, key));
		}
			
		entry = (tableLink_t *)new char[keyLen + sizeof(tableLink_t) - 1];
		XASSERT(entry != NULL, AAFRESULT_NOMEMORY);
	
		hash = _lookup(table,key);
		entry->type = valueIsPtr;
		entry->dup = NULL;
		/* Add to the head of the dup list for that key */
		if(dup == kAafTableDupAddDup)
		{
			srch = table->hashTable[hash];
			while(srch != NULL)
			{
				if(table->compare(key, srch->local))
				{
					entry->dup = srch;
					break;
				}
			srch = srch->link;
			}
		}
	
		entry->link = table->hashTable[hash];
		table->hashTable[hash] = entry;
		
		memcpy(entry->local, (char *)key, keyLen);
	
		entry->data = value;
		entry->keyLen = keyLen;
		entry->valueLen = 0;
		table->numItems++;
	}
	XEXCEPT
	{
	}
	XEND

	return(AAFRESULT_SUCCESS);
}

/************************
 * name
 *
 * 		WhatIt(Internal)Does
 *
 * Argument Notes:
 *		StuffNeededBeyondNotesInDefinition.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
aafErr_t TableAddValueBlock(
			aafTable_t *table,
			void *key,
			short keyLen,
			void *value,
			aafInt32 valueLen,
			aafTableDuplicate_t dup)
{
	tableLink_t		*entry, *srch;
  	aafInt32			hash;
				
	XPROTECT()
	{
		XASSERT((table != NULL) && (table->cookie == TABLE_COOKIE), 
				AAFRESULT_TABLE_BAD_HDL);
		entry = NULL;
	
		if(keyLen == USE_DEFAULT)
			keyLen = table->defaultSize;
	
		if((dup == kAafTableDupError) && TableIncludesKey(table, key))
			return(AAFRESULT_TABLE_DUP_KEY);

		if((dup == kAafTableDupReplace) && TableIncludesKey(table, key))
			CHECK(TableRemove(table, key));
	
		entry = (tableLink_t *)new char [keyLen + valueLen + sizeof(tableLink_t) - 1];
		if(entry == NULL)
			return(AAFRESULT_NOMEMORY);
		hash = _lookup(table,key);
		entry->type = valueIsBlock;
		entry->dup = NULL;
		/* Add to the head of the dup list for that key */
		
		if(dup == kAafTableDupAddDup)
		{
			srch = table->hashTable[hash];
			while(srch != NULL)
			{
				if(table->compare(key, srch->local))
				{
					entry->dup = srch;
					break;
				}
				srch = srch->link;
			}
		}
	
		entry->link = table->hashTable[hash];
		table->hashTable[hash] = entry;
	
		memcpy(entry->local, (char *)key, keyLen);
		memcpy(entry->local+keyLen, (char *)value, valueLen);
	
		entry->data = NULL;
		entry->keyLen = keyLen;
		entry->valueLen = valueLen;
		table->numItems++;
	}
	XEXCEPT
	{
	}
	XEND

	return(AAFRESULT_SUCCESS);
}

/************************
 * name
 *
 * 		WhatIt(Internal)Does
 *
 * Argument Notes:
 *		StuffNeededBeyondNotesInDefinition.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
aafErr_t TableRemove(
			aafTable_t *table,
			void *key)
{
	aafInt32		index;
	tableLink_t		*entry, *prevEntry = NULL;
	char		    *tmpMem;
	
	XPROTECT()
	{
	  XASSERT((table != NULL) && (table->cookie == TABLE_COOKIE), 
		  AAFRESULT_TABLE_BAD_HDL);
	  XASSERT(table->compare != NULL, AAFRESULT_TABLE_MISSING_COMPARE);
	
	  index = _lookup(table, key);	
	  entry = table->hashTable[index];
	
	  /* Free ALL entries which match the key, even duplicates */
	  while( entry != NULL)
	    {
	      if( table->compare( key, entry->local))
			{
			  if( prevEntry)
				prevEntry->link = entry->link;
			  else
				table->hashTable[index] = entry->link;
		  
			  if (table->entryDispose != NULL)
				{
				  if(entry->type == valueIsPtr)
					{
			      /* Use entryDispose callback to free client allocated entry data.
			       */
			      if(entry->data != NULL)
			        (*table->entryDispose)(entry->data);
					}
				  else
					{
			      /* Use entryDispose callback to free internal
			       * entry data.
			       */
					  tmpMem = (char *)new char[entry->valueLen];
						
					  /* Force data alignment */
					  memcpy(tmpMem, entry->local+
							 entry->keyLen, entry->valueLen);
					  (*table->entryDispose)(tmpMem);
					  delete[] tmpMem; // tomr 99-11-17 : use array delete to match new char[] allocator.
					}
				}

			  delete[] entry; // tomr 99-11-17 : use array delete to match new char[] allocator.

			  table->numItems--;
			  entry = NULL;
			}
	      else
			{
			  prevEntry = entry;
			  entry = entry->link;
			}
	    }
	}
	XEXCEPT
	{
	}
	XEND
	
	return(AAFRESULT_SUCCESS);
}
	
/************************
 * name
 *
 * 		WhatIt(Internal)Does
 *
 * Argument Notes:
 *		StuffNeededBeyondNotesInDefinition.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
aafBool TableIncludesKey(
			aafTable_t *table,
			void *key)
{
	aafInt32		n;
	tableLink_t	*entry;
	aafBool		result;
	
	if((table == NULL) || (table->cookie != TABLE_COOKIE))
		return(kAAFFalse);
	if(table->compare == NULL)
		return(kAAFFalse);

	result = kAAFFalse;
	n = _lookup(table, key);
	entry = table->hashTable[n];
	while(entry != NULL)
	{
		if (table->compare( key, entry->local))
		{
			result = kAAFTrue;
			break;
		}

		entry = entry->link;
	}

	return(result);
}

/************************
 * name
 *
 * 		WhatIt(Internal)Does
 *
 * Argument Notes:
 *		StuffNeededBeyondNotesInDefinition.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
void *TableLookupPtr(
			aafTable_t *table,
			void *key)
{
	aafInt32		n;
	tableLink_t	*entry;
	void		*result;
	
	if((table == NULL) || (table->cookie != TABLE_COOKIE))
		return(NULL);
	if(table->compare == NULL)
		return(NULL);

	result = NULL;
	n = _lookup(table, key);
	entry = table->hashTable[n];
	while(entry != NULL)
	{
		if (table->compare( key, entry->local))
		{
			if(entry->type == valueIsPtr)
				result = entry->data;
			else
				return(NULL);
			/* 	result = entry->local+entry->keyLen;	*/
			break;
		}

		entry = entry->link;
	}

	return(result);
}

/************************
 * name
 *
 * 		WhatIt(Internal)Does
 *
 * Argument Notes:
 *		StuffNeededBeyondNotesInDefinition.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
aafErr_t TableLookupBlock(
			aafTable_t *table,
			void *key,
			aafInt32 valueLen,
			void *valuePtr,
			aafBool *found)
{
  aafInt32		n;
  tableLink_t	*entry;
	
  if((table == NULL) || (table->cookie != TABLE_COOKIE))
    return(AAFRESULT_TABLE_BAD_HDL);
  if(table->compare == NULL)
    return(AAFRESULT_TABLE_MISSING_COMPARE);

  *found = kAAFFalse;
  n = _lookup(table, key);
  entry = table->hashTable[n];
  while((entry != NULL) && !(*found))
    {
      if (table->compare( key, entry->local))
	{
	  if(entry->type == valueIsBlock)
	    {
	      memcpy(valuePtr, ((char *)entry->local)+entry->keyLen, valueLen);
	      *found = kAAFTrue;
	    }
	  /* 	result = entry->local+entry->keyLen;	*/
	  break;
	}

      entry = entry->link;
    }

  return(AAFRESULT_SUCCESS);
}

/************************
 * name
 *
 * 		WhatIt(Internal)Does
 *
 * Argument Notes:
 *		StuffNeededBeyondNotesInDefinition.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
aafErr_t TableFirstEntry(
			aafTable_t *table,
			aafTableIterate_t *iter,
			aafBool *found) 
{
  XPROTECT()
    {
      XASSERT((table != NULL) && (table->cookie == TABLE_COOKIE), 
	      AAFRESULT_TABLE_BAD_HDL);
      XASSERT(iter != NULL, AAFRESULT_TABLE_BAD_ITER);
      
      iter->cookie = TABLE_ITER_COOKIE;
      iter->table = table;
      iter->curHash = -1;
      iter->nextEntry = NULL;
      iter->srch = kTableSrchAny;
      iter->srchKey = NULL;
      CHECK(TableNextEntry(iter, found));
    }
  XEXCEPT
  XEND
	
      return(AAFRESULT_SUCCESS);
}

/************************
 * name
 *
 * 		WhatIt(Internal)Does
 *
 * Argument Notes:
 *		StuffNeededBeyondNotesInDefinition.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
aafErr_t TableFirstEntryMatching(
			aafTable_t *table,
			aafTableIterate_t *iter,
			void *key,
			aafBool *found) 
{
	XPROTECT()
	{
		XASSERT((table != NULL) && (table->cookie == TABLE_COOKIE), 
				AAFRESULT_TABLE_BAD_HDL);
		XASSERT(iter != NULL, AAFRESULT_TABLE_BAD_ITER);
	
		iter->cookie = TABLE_ITER_COOKIE;
		iter->table = table;
		iter->curHash = _lookup(table, key);
		iter->nextEntry = table->hashTable[iter->curHash];
		iter->srch = kTableSrchMatch;
		iter->srchKey = key;
		CHECK(TableNextEntry(iter, found));
	}
	XEXCEPT
	XEND
	
	return(AAFRESULT_SUCCESS);
}

/************************
 * name
 *
 * 		WhatIt(Internal)Does
 *
 * Argument Notes:
 *		StuffNeededBeyondNotesInDefinition.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
aafErr_t TableFirstEntryUnique(
			aafTable_t *table,
			aafTableIterate_t *iter,
			aafBool *found) 
{
	XPROTECT()
	{
		XASSERT((table != NULL) && (table->cookie == TABLE_COOKIE), 
				AAFRESULT_TABLE_BAD_HDL);
		XASSERT(iter != NULL, AAFRESULT_TABLE_BAD_ITER);
	
		iter->cookie = TABLE_ITER_COOKIE;
		iter->table = table;
		iter->curHash = -1;
		iter->nextEntry = NULL;
		iter->srch = kTableSrchUnique;
		iter->srchKey = NULL;
		CHECK(TableNextEntry(iter, found));
	}
	XEXCEPT
	XEND
	
	return(AAFRESULT_SUCCESS);
}

/************************
 * name
 *
 * 		WhatIt(Internal)Does
 *
 * Argument Notes:
 *		StuffNeededBeyondNotesInDefinition.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
aafErr_t TableNextEntry(
			aafTableIterate_t *iter,
			aafBool *foundPtr) 
{
	aafTable_t		*table;
	tableLink_t		*entry = NULL;
	
	XPROTECT()
	{
		XASSERT(foundPtr != NULL, AAFRESULT_NULL_PARAM);
		*foundPtr = kAAFFalse;
	
		table = iter->table;
		XASSERT((table != NULL) && (table->cookie == TABLE_COOKIE), 
				AAFRESULT_TABLE_BAD_HDL);
		XASSERT((iter != NULL) && (iter->cookie == TABLE_ITER_COOKIE), 
				AAFRESULT_TABLE_BAD_ITER);

		if(iter->srch == kTableSrchMatch)
		{
			while(!*foundPtr && (iter->nextEntry != NULL))
			{
				entry = iter->nextEntry;
				if (table->compare(iter->srchKey, entry->local))
					*foundPtr = kAAFTrue;
				iter->nextEntry = entry->link;
			}
		}
		else
		{
			while(!*foundPtr && (iter->curHash < table->hashTableSize))
			{
				while(!*foundPtr && (iter->nextEntry != NULL))
				{
					entry = iter->nextEntry;
					if (iter->srch == kTableSrchAny)
						*foundPtr = kAAFTrue;
					/* NOTE: entry->dup will be != NULL for every duplicate entry EXCEPT the
					 *			last entry, satisfying the one of each unique requirement
					 */
					else if((iter->srch == kTableSrchUnique) && (entry->dup == NULL))
						*foundPtr = kAAFTrue;
					iter->nextEntry = entry->link;
				}
				if(!*foundPtr)
				{
					++iter->curHash;
					if(iter->curHash < table->hashTableSize)
						iter->nextEntry = table->hashTable[iter->curHash];
					else
						iter->nextEntry = NULL;
				}
			}
		}
		if(*foundPtr)
		{
			iter->valueLen = (entry->type == valueIsPtr ? sizeof(void *) : entry->valueLen);
			if(entry->type == valueIsPtr)
				iter->valuePtr = entry->data;
			else
				iter->valuePtr = entry->local + entry->keyLen;
			iter->key = entry->local;
		}
	}
	XEXCEPT
	{
	}
	XEND
	
	return(AAFRESULT_SUCCESS);
}

/************************
 * name
 *
 * 		WhatIt(Internal)Does
 *
 * Argument Notes:
 *		StuffNeededBeyondNotesInDefinition.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
aafInt32 TableNumEntriesMatching(
			aafTable_t *table,
			void *key)
{
	aafInt32				numMatches;
	aafBool				more;
	aafTableIterate_t	iter;
	
	XPROTECT()
	{
		XASSERT((table != NULL) && (table->cookie == TABLE_COOKIE), 
				AAFRESULT_TABLE_BAD_HDL);
		numMatches = 0;
		CHECK(TableFirstEntryMatching(table, &iter, key, &more));
		while(more)
		{
			numMatches++;
			CHECK(TableNextEntry(&iter, &more));
		}
	}
	XEXCEPT
	{
	}
	XEND_SPECIAL(0)
	
	return(numMatches);
}

/************************
 * name
 *
 * 		WhatIt(Internal)Does
 *
 * Argument Notes:
 *		StuffNeededBeyondNotesInDefinition.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
aafErr_t TableSearchDataValue(
			aafTable_t *table,
			aafInt32 valueLen,
			void *value,
			aafInt32 keyLen,
			void *key,
			aafBool *foundPtr)
{
	aafBool				more;
	aafTableIterate_t	iter;
	
	XPROTECT()
	{
		XASSERT(foundPtr != NULL, AAFRESULT_NULL_PARAM);
		*foundPtr = kAAFFalse;
		XASSERT((table != NULL) && (table->cookie == TABLE_COOKIE), 
				AAFRESULT_TABLE_BAD_HDL);
	
		CHECK(TableFirstEntry(table, &iter, &more));
			
		while(more && !*foundPtr)
		{
			if((valueLen == iter.valueLen) && 
				(memcmp(value, iter.valuePtr, iter.valueLen) == 0))
			{
				*foundPtr = kAAFTrue;
				memcpy(key, iter.key, keyLen);
			}
			CHECK(TableNextEntry(&iter, &more));
		}
	}
	XEXCEPT
	{
	}
	XEND
	
	return(AAFRESULT_SUCCESS); 
}


/************************
 * name
 *
 * 		WhatIt(Internal)Does
 *
 * Argument Notes:
 *		StuffNeededBeyondNotesInDefinition.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
aafErr_t TableDispose(
			aafTable_t *table)
{
	XPROTECT()
	{
		XASSERT((table != NULL) && (table->cookie == TABLE_COOKIE), 
				AAFRESULT_TABLE_BAD_HDL);
		DisposeList(table, kAAFFalse);
	
		if(table->hashTable != NULL)
			delete[] table->hashTable; // tomr 99-11-17 : use array delete to match new char[] allocator.

		delete table;
	}
	XEXCEPT
	XEND
	
	return(AAFRESULT_SUCCESS);
}

/************************
 * name
 *
 * 		WhatIt(Internal)Does
 *
 * Argument Notes:
 *		StuffNeededBeyondNotesInDefinition.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
aafErr_t TableDisposeAll(
			aafTable_t *table)
{
	aafErr_t	status;
	
	status = TableDisposeItems(table);
	if(status == AAFRESULT_SUCCESS)
		TableDispose(table);
	
	return(status);
}

/************************
 * name
 *
 * 		WhatIt(Internal)Does
 *
 * Argument Notes:
 *		StuffNeededBeyondNotesInDefinition.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
aafErr_t TableDisposeItems(
			aafTable_t *table)
{
	XPROTECT()
	{
		XASSERT((table != NULL) && (table->cookie == TABLE_COOKIE), 
				AAFRESULT_TABLE_BAD_HDL);
		DisposeList(table, kAAFTrue);
	}
	XEXCEPT
	XEND
	
	return(AAFRESULT_SUCCESS);
}
	
/***************************************************/
/****			   Private Routines				****/
/***************************************************/

/************************
 * name
 *
 * 		WhatIt(Internal)Does
 *
 * Argument Notes:
 *		StuffNeededBeyondNotesInDefinition.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
static aafErr_t DisposeList(
			aafTable_t *table,
			aafBool itemsAlso)
{
	aafInt32		n;
	tableLink_t	*entry, *entryNext;
	char		*tmpMem;

	XPROTECT()
	{
		XASSERT((table != NULL) && (table->cookie == TABLE_COOKIE), 
			AAFRESULT_TABLE_BAD_HDL);
		for (n = 0; n < table->hashTableSize; n++)
		{
			entry = table->hashTable[n];
			while(entry != NULL)
			{
				entryNext = entry->link;

				if((table->entryDispose != NULL) && itemsAlso)
				{
				  if(entry->type == valueIsPtr)
				  {
			      /* Use entryDispose callback to free client allocated entry data.
			       */
			      if(entry->data != NULL)
			        (*table->entryDispose)(entry->data);
				  }
				  else
				    {
				      /* Use entryDispose callback to free internal
				       * entry data.
				       */
				      tmpMem = (char *)new char[entry->valueLen];
						
				      /* Force data alignment */
				      memcpy(tmpMem, entry->local+
					     entry->keyLen, entry->valueLen);
				      (*table->entryDispose)(tmpMem);
				      delete[] tmpMem; // tomr 99-11-17 : use array delete to match new char[] allocator.
				    }
				}
	
				delete[] entry; // tomr 99-11-17 : use array delete to match new char[] allocator.
				entry = entryNext;
			}
			
			table->hashTable[n] = NULL;
		}
		table->numItems = 0;
	}
	XEXCEPT
	XEND
	
	return(AAFRESULT_SUCCESS);
}

/************************************************************************
 *
 * String Table Functions
 *
 ************************************************************************/
	

/************************
 * name
 *
 * 		WhatIt(Internal)Does
 */
static aafInt32 StrMap( void *temp1)
{
	aafInt16	n, keyLen;
	aafInt32	hashVal;
	char	*key = (char *)temp1;
	
	/* don't dereference NULL pointers, return 0 instead.	Don't worry,
	 * key won't match any items in chain, so 0 is as good as any.
	 */
	if (temp1 == NULL)
		return(0);
		
	keyLen = (aafInt16)strlen(key)+1;
	for(n = 0, hashVal = 0; n < keyLen; n++, key++)
		hashVal = (hashVal << 1) + toupper(*key);
		
	return(hashVal);
}

static aafBool cmpSensitive( void *temp1, void *temp2)
{
	char *a = (char *)temp1;
	char *b = (char *)temp2;

	return(strcmp(a, b) == 0 ? kAAFTrue : kAAFFalse);
}
	
static aafBool cmpInsensitive( void *temp1, void *temp2)
{
	register char *a = (char *)temp1;
	register char *b = (char *)temp2;

	for ( ; (*a != '\0') && (*b != '\0'); a++, b++)
	{	
        if (tolower(*a) != tolower (*b))
			return (kAAFFalse);
	}

	if ((*b != '\0') || (*a != '\0'))
		return (kAAFFalse);
		
	return (kAAFTrue);
}

/************************
 * name
 *
 * 		WhatIt(Internal)Does
 *
 * Argument Notes:
 *		StuffNeededBeyondNotesInDefinition.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
aafErr_t NewStringTable(
			AAFFile * file,
			aafBool caseSensistive,
			aafTblCompareProc mycompare,
			aafInt32 numBuckets,
			aafTable_t **resultPtr)
{
	if(mycompare == NULL)
		mycompare = (caseSensistive ? cmpSensitive : cmpInsensitive);

	return(NewTable(file, 0, StrMap, mycompare, numBuckets, resultPtr));
}	

/************************
 * name
 *
 * 		WhatIt(Internal)Does
 *
 * Argument Notes:
 *		StuffNeededBeyondNotesInDefinition.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
aafErr_t TableAddString(
			aafTable_t *table,
			char *key,
			void *value,
			aafTableDuplicate_t dup)
{
  	aafInt16		keyLen;
				
	keyLen = (aafInt16)strlen(key)+1;
	return(TableAddValuePtr(table, key, keyLen, value, dup));
}

/************************
 * name
 *
 * 		WhatIt(Internal)Does
 *
 * Argument Notes:
 *		StuffNeededBeyondNotesInDefinition.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
aafErr_t TableAddStringBlock(
			aafTable_t *table,
			char *key,
			void *value,
			aafInt32 valueLen,
			aafTableDuplicate_t dup)
{
  aafInt16		keyLen;
				
  keyLen = (aafInt16)strlen(key)+1;
  return(TableAddValueBlock(table, key, keyLen, value, valueLen, dup));
}
	
/************************************************************************
 *
 * UID Table Functions
 *
 ************************************************************************/

/************************
 * name
 *
 * 		WhatIt(Internal)Does
 */
static aafInt32 UidMap(void *temp)
{
  aafUID_t *key = (aafUID_t *)temp;

//!!! Need to include in the hash
//!!!  aafUInt8  Data4[8];
  return(key->Data1+key->Data2+key->Data3);
}

static aafBool	Uidcompare(void *temp1, void *temp2)
{
  aafUID_t *key1 = (aafUID_t *)temp1;
  aafUID_t *key2 = (aafUID_t *)temp2;

  return( (key1->Data1 == key2->Data1) && (key1->Data2 == key2->Data2) &&
	 (key1->Data3 == key2->Data3) && (memcmp(key1->Data4, key2->Data4, 8) == 0)
	 ? kAAFTrue : kAAFFalse);
}


/************************
 * name
 *
 * 		WhatIt(Internal)Does
 *
 * Argument Notes:
 *		StuffNeededBeyondNotesInDefinition.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
aafErr_t NewUIDTable(
			AAFFile * file,
			 aafInt32 numBuckets,
			 aafTable_t **result)
{
  return(NewTable(file, sizeof(aafUID_t), UidMap, Uidcompare, numBuckets, 
		      result));
}	

/************************
 * name
 *
 * 		WhatIt(Internal)Does
 *
 * Argument Notes:
 *		StuffNeededBeyondNotesInDefinition.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
aafErr_t TableAddUID(
			aafTable_t *table,
			aafUID_t key,
			void *value,
			aafTableDuplicate_t dup)
{
  return(TableAddValuePtr(table, &key, sizeof(aafUID_t), value, dup));
}
		
/************************
 * name
 *
 * 		WhatIt(Internal)Does
 *
 * Argument Notes:
 *		StuffNeededBeyondNotesInDefinition.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
aafErr_t TableAddUIDBlock(
			aafTable_t *table,
			aafUID_t key,
			void *value,
			aafInt32 valueLen,
			aafTableDuplicate_t dup)
{
  return(TableAddValueBlock(table, &key, sizeof(aafUID_t), value, valueLen, dup));
}
		
/************************
 * name
 *
 * 		WhatIt(Internal)Does
 *
 * Argument Notes:
 *		StuffNeededBeyondNotesInDefinition.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
aafErr_t TableRemoveUID(
			aafTable_t *table,
			aafUID_t key)
{
  return(TableRemove(table, &key));
}
	
/************************
 * name
 *
 * 		WhatIt(Internal)Does
 *
 * Argument Notes:
 *		StuffNeededBeyondNotesInDefinition.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
aafBool TableIncludesUID(
			aafTable_t *table,
			aafUID_t key)
{
  return(TableIncludesKey(table, &key));
}
	
/************************
 * name
 *
 * 		WhatIt(Internal)Does
 *
 * Argument Notes:
 *		StuffNeededBeyondNotesInDefinition.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
void *TableUIDLookupPtr(
			aafTable_t *table,
			aafUID_t key)
{
  return(TableLookupPtr(table, &key));
}

void TableUIDLookupBlock(
			aafTable_t *table,
			aafUID_t key,
			aafInt32 valueLen,
			void *valuePtr,
			aafBool *found)
{
	TableLookupBlock(table, &key, valueLen, valuePtr, found);
}



	
/************************************************************************
 *
 * MobID Table Functions
 *
 ************************************************************************/

/************************
 * name
 *
 * 		WhatIt(Internal)Does
 */
static aafInt32 MobidMap(void *temp)
{
  aafMobID_t *key = (aafMobID_t *)temp;

//!!! Need to include in the hash
//!!!  aafUInt8  Data4[8];
  return(key->material.Data1+key->material.Data2+key->material.Data3);
}

static aafBool	Mobidcompare(void *temp1, void *temp2)
{
  aafMobID_t *key1 = (aafMobID_t *)temp1;
  aafMobID_t *key2 = (aafMobID_t *)temp2;

  return( (key1->material.Data1 == key2->material.Data1) &&
		  (key1->material.Data2 == key2->material.Data2) &&
		  (key1->material.Data3 == key2->material.Data3) &&
		  (memcmp(key1->material.Data4, key2->material.Data4, 8) == 0) && 
		  (memcmp(key1->SMPTELabel, key2->SMPTELabel, 12) == 0) && 
		  (key1->length == key2->length) &&
		  (key1->instanceHigh == key2->instanceHigh) &&
		  (key1->instanceMid == key2->instanceMid) &&
		  (key1->instanceLow == key2->instanceLow)
	 ? kAAFTrue : kAAFFalse);
}


/************************
 * name
 *
 * 		WhatIt(Internal)Does
 *
 * Argument Notes:
 *		StuffNeededBeyondNotesInDefinition.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
aafErr_t NewMobIDTable(
			 aafInt32 numBuckets,
			 aafTable_t **result)
{
  return(NewTable(NULL, sizeof(aafMobID_t), MobidMap, Mobidcompare, numBuckets, 
		      result));
}	

/************************
 * name
 *
 * 		WhatIt(Internal)Does
 *
 * Argument Notes:
 *		StuffNeededBeyondNotesInDefinition.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
aafErr_t TableAddMobID(
			aafTable_t *table,
			aafMobID_constref key,
			void *value,
			aafTableDuplicate_t dup)
{
  return(TableAddValuePtr(table, (void *)&key, sizeof(aafMobID_t), value, dup));
}
		
/************************
 * name
 *
 * 		WhatIt(Internal)Does
 *
 * Argument Notes:
 *		StuffNeededBeyondNotesInDefinition.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
aafErr_t TableAddMobIDBlock(
			aafTable_t *table,
			aafMobID_constref key,
			void *value,
			aafInt32 valueLen,
			aafTableDuplicate_t dup)
{
  return(TableAddValueBlock(table, (void *)&key, sizeof(aafMobID_t), value, valueLen, dup));
}
		
/************************
 * name
 *
 * 		WhatIt(Internal)Does
 *
 * Argument Notes:
 *		StuffNeededBeyondNotesInDefinition.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
aafErr_t TableRemoveMobID(
			aafTable_t *table,
			aafMobID_constref key)
{
  return(TableRemove(table, (void *)&key));
}
	
/************************
 * name
 *
 * 		WhatIt(Internal)Does
 *
 * Argument Notes:
 *		StuffNeededBeyondNotesInDefinition.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
aafBool TableIncludesMobID(
			aafTable_t *table,
			aafMobID_constref key)
{
  return(TableIncludesKey(table, (void *)&key));
}
	
/************************
 * name
 *
 * 		WhatIt(Internal)Does
 *
 * Argument Notes:
 *		StuffNeededBeyondNotesInDefinition.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
void *TableMobIDLookupPtr(
			aafTable_t *table,
			aafMobID_constref key)
{
	return(TableLookupPtr(table, (void *)&key));
}

void TableMobIDLookupBlock(
			aafTable_t *table,
			aafMobID_constref key,
			aafInt32 valueLen,
			void *valuePtr,
			aafBool *found)
{
	TableLookupBlock(table, (void *)(&key), valueLen, valuePtr, found);
}




/************************************************************************
 *
 * SlotID Table Functions
 *
 ************************************************************************/
 
/************************
 * name
 *
 * 		WhatIt(Internal)Does
 */
static aafInt32 SlotIDMap(void *temp)
{
  aafInt32 key1 = *((aafInt32 *)temp);

  return(key1);
}

static aafBool	SlotIDcompare(void *temp1, void *temp2)
{
  aafInt32 key1 = *((aafInt32 *)temp1);
  aafInt32 key2 = *((aafInt32 *)temp2);

  return( (key1 == key2) ? kAAFTrue : kAAFFalse);
}

/************************
 * name
 *
 * 		WhatIt(Internal)Does
 *
 * Argument Notes:
 *		StuffNeededBeyondNotesInDefinition.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
aafErr_t NewSlotIDTable(
			AAFFile * file,
			aafInt32 numBuckets,
			aafTable_t **result)
{
  return(NewTable(file, sizeof(aafSlotID_t), SlotIDMap, SlotIDcompare, 
		      numBuckets, result));
}	

/************************
 * name
 *
 * 		WhatIt(Internal)Does
 *
 * Argument Notes:
 *		StuffNeededBeyondNotesInDefinition.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
aafErr_t TableAddSlotID(
			aafTable_t *table,
			aafSlotID_t key,
			void *value,
			aafInt32 valueLen)
{
  return(TableAddValueBlock(table, &key, sizeof(aafSlotID_t), value, 
				valueLen, kAafTableDupError));
}
	
/************************
 * name
 *
 * 		WhatIt(Internal)Does
 *
 * Argument Notes:
 *		StuffNeededBeyondNotesInDefinition.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
aafErr_t TableSlotIDLookup(
			aafTable_t *table,
			aafSlotID_t key,
			void *value,
			aafInt32 valueLen,
			aafBool *found)
{
  return(TableLookupBlock(table, &key, valueLen, value, found));
}

/************************************************************************
 *
 * Definition (Datakind and EffectDef) Table Functions
 *
 ************************************************************************/

/************************
 * name
 *
 * 		WhatIt(Internal)Does
 *
 * Argument Notes:
 *		StuffNeededBeyondNotesInDefinition.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
aafErr_t NewDefTable(
			AAFFile * file,
			aafInt32 numBuckets,
			aafTable_t **result)
{
  return(NewTable(file, 0, StrMap, cmpSensitive, numBuckets, result));
}	


#ifdef AAF_SELF_TEST
/************************
 * name
 *
 * 		WhatIt(Internal)Does
 *
 * Argument Notes:
 *		StuffNeededBeyondNotesInDefinition.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
void testaafTable(void)
{
  aafTable_t			*test;
  aafInt32				val, n;
  aafBool				found;
  aafTableIterate_t	iter;
  char				name[4];
  char				*keys[4] = { "foo", "bar", "baz", "foo" };
	
  printf("aafTable Tests\n");

  /******************************/
  printf("    Creating the test table\n");
  NewStringTable(NULL, kAAFFalse, NULL, 32, &test);
  for(val = 0; val < 4; val++)
    TableAddStringBlock(test, keys[val], &val, sizeof(val), 
			    kaafTableDupAddDup);

  /******************************/
  printf("    Testing TableIncludesKey\n");
  if(!TableIncludesKey(test, "foo"))
    printf("Missing Key\n");
  if(TableIncludesKey(test, "aaa"))
    printf("Extra Key\n");
  
  /******************************/
  printf("    Testing TableNumEntriesMatching\n");
  if(TableNumEntriesMatching(test, "bar") != 1)
    printf("Incorrect entries matching #1\n");
  if(TableNumEntriesMatching(test, "foo") != 2)
    printf("Incorrect entries matching #2\n");
  if(TableNumEntriesMatching(test, "bbb") != 0)
    printf("Incorrect entries matching #3\n");
  
  /******************************/
  printf("    Testing TableFirst/NextEntry\n");
  TableFirstEntry(test, &iter, &found);
  for(n = 1; n <= 4; n++)
    {
      if(found)
	{
	  val = *((aafInt32 *)iter.valuePtr);
	  if((val >= 4) || strcmp(iter.key, keys[val]) != 0)
	    printf("invalid iterate result #%ld\n", n);
	}
      else
	printf("Missing iterate key #%ld\n", n);
      
      TableNextEntry(&iter, &found);
    }
  if(found)
    printf("Extra iterate key #%ld\n", n);
  
  /******************************/
  printf("    Testing TableFirst/NextEntryMatching\n");
  TableFirstEntryMatching(test, &iter, "foo", &found);
  for(n = 1; n <= 2; n++)
    {
      if(found)
	{
	  val = *((aafInt32 *)iter.valuePtr);
	  if((val >= 4) || strcmp(iter.key, keys[val]) != 0)
	    printf("invalid iterateMatch result #%ld\n", n);
	}
      else
	printf("Missing iterateMatch key #%ld\n", n);
      TableNextEntry(&iter, &found);
    }
  if(found)
    printf("Extra iterateMatch key #1\n");
  
  /******************************/
  printf("    Testing TableFirst/NextEntryUnique\n");
  TableFirstEntryUnique(test, &iter, &found);
  for(n = 1; n <= 3; n++)
    {
      if(found)
	{
	  val = *((aafInt32 *)iter.valuePtr);
	  if((val >= 4) || strcmp(iter.key, keys[val]) != 0)
	    printf("invalid iterateUnique result #%ld\n", n);
	}
      else
	printf("Missing iterateUnique key #%d\n", n);
      TableNextEntry(&iter, &found);
    }
  if(found)
    printf("Extra iterateMatch key #1\n");
  
  /******************************/
  val = 0;
  TableSearchDataValue(test, sizeof(aafInt32), &val, 4, name, &found);
  if(!found)
    printf("Failed backsearch #1\n");
  if(strcmp(name, "foo") != 0)
    printf("Failed key test #1\n");
  
  val = 1;
  TableSearchDataValue(test, sizeof(aafInt32), &val, 4, name, &found);
  if(!found)
    printf("Failed backsearch #2\n");
  if(strcmp(name, "bar") != 0)
    printf("Failed key test #2\n");
  
  val = 2;
  TableSearchDataValue(test, sizeof(aafInt32), &val, 4, name, &found);
  if(!found)
    printf("Failed backsearch #3\n");
  if(strcmp(name, "baz") != 0)
    printf("Failed key test #3\n");
  
  val = 3;
  TableSearchDataValue(test, sizeof(aafInt32), &val, 4, name, &found);
  if(!found)
    printf("Failed backsearch #4\n");
  if(strcmp(name, "foo") != 0)
    printf("Failed key test #4\n");
  
  printf("Finished aafTable tests\n");
}
#endif

/* INDENT OFF */
/*
;;; Local Variables: ***
;;; tab-width:4 ***
;;; End: ***
*/

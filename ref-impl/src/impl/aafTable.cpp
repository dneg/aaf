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

/*
 * Name: omTable.c
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

//#include "omPublic.h"
#include "aafTable.h"
#include "AAFFile.h"
//#include "omPvt.h"

#define TABLE_COOKIE		0x5461626C
#define TABLE_ITER_COOKIE	0x54424C49

#define _lookup(table,key) 	( (table->map ? (aafUInt32)table->map(key) : (aafUInt32)key) % table->hashTableSize)

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

struct omTable
{
	AAFFile *			file;			/* Optional: If set omOptMalloc/Free calls will optimize */
	aafInt32			cookie;
	aafInt16			defaultSize;	/* default size of keys */
	tableLink_t		**hashTable;
	aafInt32			hashTableSize;
	aafInt32			numItems;
				
	omTblMapProc		map;			/* the mapping function			*/
	omTblCompareProc	compare;		/* the comparison function		*/
	omTblDisposeProc	entryDispose;
};
	
static aafErr_t DisposeList(omTable_t *table, aafBool itemsAlso);

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
			omTblMapProc myMap,
			omTblCompareProc myCompare,
			aafInt32 numBuckets,
			omTable_t **resultPtr)
{
	omTable_t	*result;
	
	XPROTECT(NULL)
	{
		result = (omTable_t *)file->omOptMalloc(sizeof(omTable_t));
		XASSERT(result != NULL, OM_ERR_NOMEMORY);
		
		result->cookie = TABLE_COOKIE;
		result->file = file;
		result->map = myMap;
		result->compare = myCompare;
		result->entryDispose = NULL;
		result->defaultSize = initKeySize;
		result->hashTableSize = numBuckets;
		result->hashTable = (tableLink_t **)file->omOptMalloc(result->hashTableSize * sizeof(tableLink_t *));
		memset(result->hashTable, 0, result->hashTableSize * sizeof(tableLink_t *));
		result->numItems = 0;
	}
	XEXCEPT
	{
		return(XCODE());
	}
	XEND

	*resultPtr = result;
	return(OM_ERR_NONE);
}	

/* Called once for each entry, used to dispose only memory referenced by the entry */
aafErr_t omfsSetTableDispose(omTable_t *table, omTblDisposeProc proc)
{
	XPROTECT(NULL)
	{
		XASSERT((table != NULL) && (table->cookie == TABLE_COOKIE), 
				OM_ERR_TABLE_BAD_HDL);
		table->entryDispose = proc;
	}
	XEXCEPT
	XEND
	
	return(OM_ERR_NONE);
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
			omTable_t *table,
			void *key,
			aafInt16 keyLen,
			void *value,
			omTableDuplicate_t dup)
{
	tableLink_t		*entry, *srch;
  	aafInt32			hash;
				
	XPROTECT(NULL)
	{
		XASSERT((table != NULL) && (table->cookie == TABLE_COOKIE), 
				OM_ERR_TABLE_BAD_HDL);
		entry = NULL;
	
		if(keyLen == USE_DEFAULT)
			keyLen = table->defaultSize;
		if((dup == kOmTableDupError) && TableIncludesKey(table, key))
			return(OM_ERR_TABLE_DUP_KEY);
	
		if((dup == kOmTableDupReplace) && TableIncludesKey(table, key))
		{
			CHECK(TableRemove(table, key));
		}
			
		entry = (tableLink_t *)table->file->omOptMalloc(keyLen + sizeof(tableLink_t) - 1);
		XASSERT(entry != NULL, OM_ERR_NOMEMORY);
	
		hash = _lookup(table,key);
		entry->type = valueIsPtr;
		entry->dup = NULL;
		/* Add to the head of the dup list for that key */
		if(dup == kOmTableDupAddDup)
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
		return(XCODE());
	}
	XEND

	return(OM_ERR_NONE);
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
			omTable_t *table,
			void *key,
			short keyLen,
			void *value,
			aafInt32 valueLen,
			omTableDuplicate_t dup)
{
	tableLink_t		*entry, *srch;
  	aafInt32			hash;
				
	XPROTECT(NULL)
	{
		XASSERT((table != NULL) && (table->cookie == TABLE_COOKIE), 
				OM_ERR_TABLE_BAD_HDL);
		entry = NULL;
	
		if(keyLen == USE_DEFAULT)
			keyLen = table->defaultSize;
	
		if((dup == kOmTableDupError) && TableIncludesKey(table, key))
			return(OM_ERR_TABLE_DUP_KEY);

		if((dup == kOmTableDupReplace) && TableIncludesKey(table, key))
			CHECK(TableRemove(table, key));
	
		entry = (tableLink_t *)table->file->omOptMalloc(keyLen + valueLen + sizeof(tableLink_t) - 1);
		if(entry == NULL)
			return(OM_ERR_NOMEMORY);
		hash = _lookup(table,key);
		entry->type = valueIsBlock;
		entry->dup = NULL;
		/* Add to the head of the dup list for that key */
		
		if(dup == kOmTableDupAddDup)
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
		return(XCODE());
	}
	XEND

	return(OM_ERR_NONE);
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
			omTable_t *table,
			void *key)
{
	aafInt32		index;
	tableLink_t		*entry, *prevEntry = NULL;
	char		    *tmpMem;
	
	XPROTECT(NULL)
	{
	  XASSERT((table != NULL) && (table->cookie == TABLE_COOKIE), 
		  OM_ERR_TABLE_BAD_HDL);
	  XASSERT(table->compare != NULL, OM_ERR_TABLE_MISSING_COMPARE);
	
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
		  
			  /* Use entryDispose callback to free internal
			   * entry data.
			   */
			  if (table->entryDispose != NULL)
				{
				  if(entry->type == valueIsPtr)
					{
					  (*table->entryDispose)(entry->data);
					  if(entry->data != NULL)
						table->file->omOptFree(entry->data);
					}
				  else
					{
					  tmpMem = (char *)table->file->omOptMalloc(entry->valueLen);
						
					  /* Force data alignment */
					  memcpy(tmpMem, entry->local+
							 entry->keyLen, entry->valueLen);
					  (*table->entryDispose)(tmpMem);
					  table->file->omOptFree(tmpMem);
					}
				}

			  table->file->omOptFree(entry);

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
		return(XCODE());
	}
	XEND
	
	return(OM_ERR_NONE);
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
			omTable_t *table,
			void *key)
{
	aafInt32		n;
	tableLink_t	*entry;
	aafBool		result;
	
	if((table == NULL) || (table->cookie != TABLE_COOKIE))
		return(FALSE);
	if(table->compare == NULL)
		return(FALSE);

	result = FALSE;
	n = _lookup(table, key);
	entry = table->hashTable[n];
	while(entry != NULL)
	{
		if (table->compare( key, entry->local))
		{
			result = TRUE;
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
			omTable_t *table,
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
			omTable_t *table,
			void *key,
			aafInt32 valueLen,
			void *valuePtr,
			aafBool *found)
{
  aafInt32		n;
  tableLink_t	*entry;
	
  if((table == NULL) || (table->cookie != TABLE_COOKIE))
    return(OM_ERR_TABLE_BAD_HDL);
  if(table->compare == NULL)
    return(OM_ERR_TABLE_MISSING_COMPARE);

  *found = FALSE;
  n = _lookup(table, key);
  entry = table->hashTable[n];
  while((entry != NULL) && !(*found))
    {
      if (table->compare( key, entry->local))
	{
	  if(entry->type == valueIsBlock)
	    {
	      memcpy(valuePtr, ((char *)entry->local)+entry->keyLen, valueLen);
	      *found = TRUE;
	    }
	  /* 	result = entry->local+entry->keyLen;	*/
	  break;
	}

      entry = entry->link;
    }

  return(OM_ERR_NONE);
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
			omTable_t *table,
			omTableIterate_t *iter,
			aafBool *found) 
{
  XPROTECT(NULL)
    {
      XASSERT((table != NULL) && (table->cookie == TABLE_COOKIE), 
	      OM_ERR_TABLE_BAD_HDL);
      XASSERT(iter != NULL, OM_ERR_TABLE_BAD_ITER);
      
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
	
      return(OM_ERR_NONE);
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
			omTable_t *table,
			omTableIterate_t *iter,
			void *key,
			aafBool *found) 
{
	XPROTECT(NULL)
	{
		XASSERT((table != NULL) && (table->cookie == TABLE_COOKIE), 
				OM_ERR_TABLE_BAD_HDL);
		XASSERT(iter != NULL, OM_ERR_TABLE_BAD_ITER);
	
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
	
	return(OM_ERR_NONE);
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
			omTable_t *table,
			omTableIterate_t *iter,
			aafBool *found) 
{
	XPROTECT(NULL)
	{
		XASSERT((table != NULL) && (table->cookie == TABLE_COOKIE), 
				OM_ERR_TABLE_BAD_HDL);
		XASSERT(iter != NULL, OM_ERR_TABLE_BAD_ITER);
	
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
	
	return(OM_ERR_NONE);
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
			omTableIterate_t *iter,
			aafBool *foundPtr) 
{
	omTable_t		*table;
	tableLink_t		*entry;
	
	XPROTECT(NULL)
	{
		XASSERT(foundPtr != NULL, OM_ERR_NULL_PARAM);
		*foundPtr = FALSE;
	
		table = iter->table;
		XASSERT((table != NULL) && (table->cookie == TABLE_COOKIE), 
				OM_ERR_TABLE_BAD_HDL);
		XASSERT((iter != NULL) && (iter->cookie == TABLE_ITER_COOKIE), 
				OM_ERR_TABLE_BAD_ITER);

		if(iter->srch == kTableSrchMatch)
		{
			while(!*foundPtr && (iter->nextEntry != NULL))
			{
				entry = iter->nextEntry;
				if (table->compare(iter->srchKey, entry->local))
					*foundPtr = TRUE;
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
						*foundPtr = TRUE;
					/* NOTE: entry->dup will be != NULL for every duplicate entry EXCEPT the
					 *			last entry, satisfying the one of each unique requirement
					 */
					else if((iter->srch == kTableSrchUnique) && (entry->dup == NULL))
						*foundPtr = TRUE;
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
		return(XCODE());
	}
	XEND
	
	return(OM_ERR_NONE);
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
			omTable_t *table,
			void *key)
{
	aafInt32				numMatches;
	aafBool				more;
	omTableIterate_t	iter;
	
	XPROTECT(NULL)
	{
		XASSERT((table != NULL) && (table->cookie == TABLE_COOKIE), 
				OM_ERR_TABLE_BAD_HDL);
		numMatches = 0;
		CHECK(TableFirstEntryMatching(table, &iter, key, &more));
		while(more)
		{
			numMatches++;
			CHECK(TableNextEntry(&iter, &more));
		}
	}
	XEXCEPT
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
			omTable_t *table,
			aafInt32 valueLen,
			void *value,
			aafInt32 keyLen,
			void *key,
			aafBool *foundPtr)
{
	aafBool				more;
	omTableIterate_t	iter;
	
	XPROTECT(NULL)
	{
		XASSERT(foundPtr != NULL, OM_ERR_NULL_PARAM);
		*foundPtr = FALSE;
		XASSERT((table != NULL) && (table->cookie == TABLE_COOKIE), 
				OM_ERR_TABLE_BAD_HDL);
	
		CHECK(TableFirstEntry(table, &iter, &more));
			
		while(more && !*foundPtr)
		{
			if((valueLen == iter.valueLen) && 
				(memcmp(value, iter.valuePtr, iter.valueLen) == 0))
			{
				*foundPtr = TRUE;
				memcpy(key, iter.key, keyLen);
			}
			CHECK(TableNextEntry(&iter, &more));
		}
	}
	XEXCEPT
	{
		return(XCODE());
	}
	XEND
	
	return(OM_ERR_NONE); 
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
			omTable_t *table)
{
	XPROTECT(NULL)
	{
		XASSERT((table != NULL) && (table->cookie == TABLE_COOKIE), 
				OM_ERR_TABLE_BAD_HDL);
		DisposeList(table, FALSE);
	
		if(table->hashTable != NULL)
			table->file->omOptFree(table->hashTable);

		table->file->omOptFree(table);
	}
	XEXCEPT
	XEND
	
	return(OM_ERR_NONE);
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
			omTable_t *table)
{
	aafErr_t	status;
	
	status = TableDisposeItems(table);
	if(status == OM_ERR_NONE)
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
			omTable_t *table)
{
	XPROTECT(NULL)
	{
		XASSERT((table != NULL) && (table->cookie == TABLE_COOKIE), 
				OM_ERR_TABLE_BAD_HDL);
		DisposeList(table, TRUE);
	}
	XEXCEPT
	XEND
	
	return(OM_ERR_NONE);
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
			omTable_t *table,
			aafBool itemsAlso)
{
	aafInt32		n;
	tableLink_t	*entry, *entryNext;
	char		*tmpMem;

	XPROTECT(NULL)
	{
		XASSERT((table != NULL) && (table->cookie == TABLE_COOKIE), 
			OM_ERR_TABLE_BAD_HDL);
		for (n = 0; n < table->hashTableSize; n++)
		{
			entry = table->hashTable[n];
			while(entry != NULL)
			{
				entryNext = entry->link;

				/* Use entryDispose callback to free internal
				 * entry data.
				 */
				if((table->entryDispose != NULL) && itemsAlso)
				{
				  if(entry->type == valueIsPtr)
				  {
					(*table->entryDispose)(entry->data);
					if(entry->data != NULL)
					  table->file->omOptFree(entry->data);
				  }
				  else
				    {
				      tmpMem = (char *)table->file->omOptMalloc(entry->valueLen);
						
				      /* Force data alignment */
				      memcpy(tmpMem, entry->local+
					     entry->keyLen, entry->valueLen);
				      (*table->entryDispose)(tmpMem);
				      table->file->omOptFree(tmpMem);
				    }
				}
	
				table->file->omOptFree(entry);
				entry = entryNext;
			}
			
			table->hashTable[n] = NULL;
		}
		table->numItems = 0;
	}
	XEXCEPT
	XEND
	
	return(OM_ERR_NONE);
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
		
	keyLen = strlen(key)+1;
	for(n = 0, hashVal = 0; n < keyLen; n++, key++)
		hashVal = (hashVal << 1) + toupper(*key);
		
	return(hashVal);
}

static aafBool cmpSensitive( void *temp1, void *temp2)
{
	char *a = (char *)temp1;
	char *b = (char *)temp2;

	return(strcmp(a, b) == 0 ? TRUE : FALSE);
}
	
static aafBool cmpInsensitive( void *temp1, void *temp2)
{
	register char *a = (char *)temp1;
	register char *b = (char *)temp2;

	for ( ; (*a != '\0') && (*b != '\0'); a++, b++)
	{	
        if (tolower(*a) != tolower (*b))
			return (FALSE);
	}

	if ((*b != '\0') || (*a != '\0'))
		return (FALSE);
		
	return (TRUE);
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
			omTblCompareProc myCompare,
			aafInt32 numBuckets,
			omTable_t **resultPtr)
{
	if(myCompare == NULL)
		myCompare = (caseSensistive ? cmpSensitive : cmpInsensitive);

	return(NewTable(file, 0, StrMap, myCompare, numBuckets, resultPtr));
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
			omTable_t *table,
			char *key,
			void *value,
			omTableDuplicate_t dup)
{
  	aafInt16		keyLen;
				
	keyLen = strlen(key)+1;
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
			omTable_t *table,
			char *key,
			void *value,
			aafInt32 valueLen,
			omTableDuplicate_t dup)
{
  aafInt16		keyLen;
				
  keyLen = strlen(key)+1;
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
#if FULL_TOOLKIT
static aafInt32 MobMap(void *temp)
{
  aafUID_t *key = (aafUID_t *)temp;

  return(key->prefix+key->major+key->minor);
}

static aafBool	MobCompare(void *temp1, void *temp2)
{
  aafUID_t *key1 = (aafUID_t *)temp1;
  aafUID_t *key2 = (aafUID_t *)temp2;

  return( (key1->prefix == key2->prefix) && (key1->major == key2->major) &&
	 (key1->minor == key2->minor) ? TRUE : FALSE);
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
			 omTable_t **result)
{
  return(NewTable(file, sizeof(aafUID_t), MobMap, MobCompare, numBuckets, 
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
			omTable_t *table,
			aafUID_t key,
			void *value,
			omTableDuplicate_t dup)
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
aafErr_t TableRemoveUID(
			omTable_t *table,
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
			omTable_t *table,
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
			omTable_t *table,
			aafUID_t key)
{
  return(TableLookupPtr(table, &key));
}
#endif

/************************************************************************
 *
 * ClassID Table Functions
 *
 ************************************************************************/
 
/************************
 * name
 *
 * 		WhatIt(Internal)Does
 */
static aafInt32 ClassIDMap(void *temp)
{
  unsigned char		*src = (unsigned char *)temp;
  /* These are only valid for the first 4 characters */
  aafInt32 key1 = src[0] << 24L | src[1] << 16L | src[2] << 8L | src[3];		

  return(key1);
}

static aafBool	ClassIDCompare(void *temp1, void *temp2)
{
  aafInt16	n;
  unsigned char		*src1 = (unsigned char *)temp1;
  unsigned char		*src2 = (unsigned char *)temp2;

  /* These are only valid for the first 4 characters */
  for(n = 0; n <= 3; n++)
  {
  	if(src1[n] != src2[n])
  		return(FALSE);
  }
  return(TRUE);
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
aafErr_t NewClassIDTable(
			AAFFile * file,
			aafInt32 numBuckets,
			omTable_t **result)
{
  return(NewTable(file, sizeof(aafClassID_t), ClassIDMap, ClassIDCompare, 
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
aafErr_t TableAddClassID(
			omTable_t *table,
			aafClassIDPtr_t key,
			void *value,
			aafInt32 valueLen)
{
  return(TableAddValueBlock(table, key, sizeof(aafClassID_t), value, 
				valueLen, kOmTableDupError));
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
aafErr_t TableClassIDLookup(
			omTable_t *table,
			aafClassIDPtr_t key,
			aafInt32 valueLen,
			void *valuePtr,
			aafBool *found)
{
  return(TableLookupBlock(table, key, valueLen, valuePtr, found));
}

/************************************************************************
 *
 * TrackID Table Functions
 *
 ************************************************************************/
 
/************************
 * name
 *
 * 		WhatIt(Internal)Does
 */
static aafInt32 TrackIDMap(void *temp)
{
  aafInt32 key1 = *((aafInt32 *)temp);

  return(key1);
}

static aafBool	TrackIDCompare(void *temp1, void *temp2)
{
  aafInt32 key1 = *((aafInt32 *)temp1);
  aafInt32 key2 = *((aafInt32 *)temp2);

  return( (key1 == key2) ? TRUE : FALSE);
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
aafErr_t NewTrackIDTable(
			AAFFile * file,
			aafInt32 numBuckets,
			omTable_t **result)
{
  return(NewTable(file, sizeof(aafTrackID_t), TrackIDMap, TrackIDCompare, 
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
aafErr_t TableAddTrackID(
			omTable_t *table,
			aafTrackID_t key,
			void *value,
			aafInt32 valueLen)
{
  return(TableAddValueBlock(table, &key, sizeof(aafTrackID_t), value, 
				valueLen, kOmTableDupError));
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
aafErr_t TableTrackIDLookup(
			omTable_t *table,
			aafTrackID_t key,
			void *value,
			aafInt32 valueLen,
			aafBool *found)
{
  return(TableLookupBlock(table, &key, valueLen, value, found));
}

/************************************************************************
 *
 * aafProperty_t Table Functions
 *
 ************************************************************************/
 
/************************
 * name
 *
 * 		WhatIt(Internal)Does
 */
static aafInt32 PropertyMap(void *temp)
{
  aafProperty_t *key1 = (aafProperty_t *)temp;
  
  return((aafInt32)*key1);
}

static aafBool	PropertyCompare(void *temp1, void *temp2)
{
  aafProperty_t *key1 = (aafProperty_t *)temp1;
  aafProperty_t *key2 = (aafProperty_t *)temp2;

  return( (*key1 == *key2) ? TRUE : FALSE);
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
aafErr_t NewPropertyTable(
			AAFFile * file,
			aafInt32 numBuckets,
			omTable_t **result)
{
  return(NewTable(file, sizeof(aafProperty_t), PropertyMap, PropertyCompare, 
		      numBuckets, result));
}	

#if FULL_TOOLKIT
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
aafErr_t TableAddProperty(
			omTable_t *table,
			aafProperty_t key,
			void *value, 
			aafInt32 valueLen,
			omTableDuplicate_t dup)
{
  return(TableAddValueBlock(table, &key, sizeof(aafProperty_t), value, 
				valueLen, dup));
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
aafErr_t TablePropertyLookup(
			omTable_t *table,
			aafProperty_t key,
			aafInt32 valueLen,
			void *valuePtr,
			aafBool *found)
{
  return(TableLookupBlock(table, &key, valueLen, valuePtr, found));
}

/************************************************************************
 *
 * aafType_t Table Functions
 *
 ************************************************************************/
 
/************************
 * name
 *
 * 		WhatIt(Internal)Does
 */
static aafInt32 TypeMap(void *temp)
{
  aafType_t *key1 = (aafType_t *)temp;

  return((aafInt32)*key1);
}

static aafBool TypeCompare(void *temp1, void *temp2)
{
  aafType_t *key1 = (aafType_t *)temp1;
  aafType_t *key2 = (aafType_t *)temp2;

  return( (*key1 == *key2) ? TRUE : FALSE);
}
#endif

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
aafErr_t NewTypeTable(
			AAFFile * file,
			aafInt32 numBuckets,
			omTable_t **result)
{
  return(NewTable(file, sizeof(aafType_t), TypeMap, TypeCompare, numBuckets, 
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
aafErr_t TableAddType(
			omTable_t *table,
			aafType_t key,
			void *value,
			aafInt32 valueLen)
{
  return(TableAddValueBlock(table, &key, sizeof(aafType_t), value, 
				valueLen, kOmTableDupError));
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
aafErr_t TableTypeLookup(
			omTable_t *table,
			aafType_t key,
			aafInt32 valueLen,
			void *valuePtr,
			aafBool *found)
{
  return(TableLookupBlock(table, &key, valueLen, valuePtr, found));
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
			omTable_t **result)
{
  return(NewTable(file, 0, StrMap, cmpSensitive, numBuckets, result));
}	

#if FULL_TOOLKIT
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
aafErr_t TableAddDef(
			omTable_t *table,
			aafUniqueName_t key,
			void *value)
{
  aafInt16 keyLen;

  keyLen = strlen(key)+1;
  return(TableAddValuePtr(table, key, keyLen, value, kOmTableDupError));
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
void *TableDefLookup(
			omTable_t *table,
			aafUniqueName_t key)
{
  return(TableLookupPtr(table, key));
}
#endif

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
void testOmTable(void)
{
  omTable_t			*test;
  aafInt32				val, n;
  aafBool				found;
  omTableIterate_t	iter;
  char				name[4];
  char				*keys[4] = { "foo", "bar", "baz", "foo" };
	
  printf("OMTable Tests\n");

  /******************************/
  printf("    Creating the test table\n");
  NewStringTable(NULL, FALSE, NULL, 32, &test);
  for(val = 0; val < 4; val++)
    TableAddStringBlock(test, keys[val], &val, sizeof(val), 
			    kOmTableDupAddDup);

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
  
  printf("Finished OMTable tests\n");
}
#endif

/* INDENT OFF */
/*
;;; Local Variables: ***
;;; tab-width:4 ***
;;; End: ***
*/

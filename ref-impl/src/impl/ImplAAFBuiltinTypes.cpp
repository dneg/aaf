/***********************************************************************
 *
 *              Copyright (c) 1998-2000 Avid Technology, Inc.
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


#ifndef __ImplAAFBuiltinTypes_h__
#include "ImplAAFBuiltinTypes.h"
#endif

#ifndef __ImplAAFDictionary_h__
#include "ImplAAFDictionary.h"
#endif

#ifndef __ImplAAFTypeDefRecord_h__
#include "ImplAAFTypeDefRecord.h"
#endif

#ifndef __ImplAAFTypeDefEnum_h__
#include "ImplAAFTypeDefEnum.h"
#endif

#ifndef __ImplAAFTypeDefExtEnum_h__
#include "ImplAAFTypeDefExtEnum.h"
#endif

#ifndef __ImplAAFTypeDefVariableArray_h__
#include "ImplAAFTypeDefVariableArray.h"
#endif

#ifndef __ImplAAFTypeDefFixedArray_h__
#include "ImplAAFTypeDefFixedArray.h"
#endif

#ifndef __ImplAAFTypeDefRename_h__
#include "ImplAAFTypeDefRename.h"
#endif

#ifndef __ImplAAFTypeDefInt_h__
#include "ImplAAFTypeDefInt.h"
#endif

#ifndef __ImplAAFTypeDefCharacter_h__
#include "ImplAAFTypeDefCharacter.h"
#endif

#ifndef __ImplAAFTypeDefString_h__
#include "ImplAAFTypeDefString.h"
#endif

#ifndef __ImplAAFTypeDefIndirect_h__
#include "ImplAAFTypeDefIndirect.h"
#endif

#ifndef __ImplAAFTypeDefOpaque_h__
#include "ImplAAFTypeDefOpaque.h"
#endif

#ifndef __ImplAAFTypeDefStrongObjRef_h__
#include "ImplAAFTypeDefStrongObjRef.h"
#endif

#ifndef __ImplAAFTypeDefWeakObjRef_h__
#include "ImplAAFTypeDefWeakObjRef.h"
#endif

#ifndef __ImplAAFTypeDefStream_h__
#include "ImplAAFTypeDefStream.h"
#endif

#ifndef __ImplAAFTypeDefSet_h__
#include "ImplAAFTypeDefSet.h"
#endif

#ifndef __AAFStoredObjectIDs_h__
#include "AAFStoredObjectIDs.h"
#endif

#ifndef __AAFClassDefUIDs_h__
#include "AAFClassDefUIDs.h"
#endif

#ifndef __AAFTypeDefUIDs_h__
#include "AAFTypeDefUIDs.h"
#endif

#include "ImplAAFBuiltinDefs.h"


// Some types which haven't been synchronized yet
#define aafAUID_t          aafUID_t
#define aafRectangle_t     aafRect_t

// aafRational_t struct field hax 
#define Numerator   numerator
#define Denominator denominator

// aafRect_t struct field hax
#define upperLeftHorizontal  xOffset
#define lowerRightHorizontal xSize
#define upperLeftVertical    yOffset
#define lowerRightVertical   ySize

#include "ImplAAFTypeDefsGen.h"

#include <assert.h>

// Weak references may not be in v1.0...
#ifndef ENABLE_WEAK_REFERENCES
#define ENABLE_WEAK_REFERENCES 1
#endif


static AAFRESULT CreateNewIntegerType (const aafUID_t & idToCreate,
								  ImplAAFDictionary * pDict,
								  ImplAAFTypeDef ** ppCreatedTypeDef)
{
  assert (pDict);
  AAFRESULT hr;

  // Go through the integer list, attempting to identify the requested
  // ID.
  TypeInteger * curInteger = s_AAFAllTypeIntegers;
  while (curInteger->isValid)
	{
	  // Check to see if the current ID matches the ID of the type
	  // def we want to create.
	  if (! memcmp (&idToCreate, &curInteger->typeID, sizeof (aafUID_t)))
		{		
		  // Yes, this is the one.
		  // Create an impl typedefinteger object (as yet uninitialized)
		  ImplAAFTypeDefInt * ptd = 0;
		  hr = pDict->CreateMetaInstance (AUID_AAFTypeDefInt, (ImplAAFMetaDefinition **) &ptd);
		  if (AAFRESULT_FAILED (hr))
			return hr;
		  assert (ptd);

		  AAFRESULT hr = ptd->Initialize (curInteger->typeID,
										  curInteger->size,
										  curInteger->isSigned,
										  curInteger->typeName);
		  assert (AAFRESULT_SUCCEEDED (hr));

		  assert (ppCreatedTypeDef);
		  *ppCreatedTypeDef = ptd;
		  (*ppCreatedTypeDef)->AcquireReference ();
		  ptd->ReleaseReference ();
		  ptd = 0;
		  return AAFRESULT_SUCCESS;
		}

	  curInteger++;
	}
  return AAFRESULT_NO_MORE_OBJECTS;
}


//
// We have the following structures to work with, defined in
// AAFTypeDefsGen.h:
//
//
// One member of a enumeration typedef:
//
// struct TypeEnumerationMember
// {                              
//   wchar_t *        memberName; 
//   aafInt64         memberValue; 
// };                             
//                                
//
// A enumeration, containing a null-terminated list of TypeEnumerationMembers:
//
// struct TypeEnumeration              
// {                              
//   aafUID_t   typeID;           
//   wchar_t *  typeName;         
//   aafUID_t * elementType;
//   size_t     size;
//   TypeEnumerationMember * members;
// };
//
//
// A null-terminated array of TypeEnumerations for all enumeration typedefs:
//
// static TypeEnumeration * s_AAFAllTypeEnumerations[];
//

//
// Looks up idToCreate in the structures.  If found, creates and
// initializes a type def to match (using the supplied dictionary),
// and returns it in ppCreatedTypeDef.  Returns true if successful;
// returns false if not found.  Does not register the new type.
//
static AAFRESULT CreateNewEnumerationType (const aafUID_t & idToCreate,
									  ImplAAFDictionary * pDict,
									  ImplAAFTypeDef ** ppCreatedTypeDef)
{
  assert (pDict);
  AAFRESULT hr;

  // Go through the enumeration list, attempting to identify the requested
  // ID.
  TypeEnumeration ** curEnumeration = s_AAFAllTypeEnumerations;
  while (*curEnumeration)
	{
	  // Check to see if the current enumeration matches the ID of the type
	  // def we want to create.
	  if (! memcmp (&idToCreate, &(*curEnumeration)->typeID, sizeof (aafUID_t)))
		{		
		  // Yes, this is the one.

		  // Create an impl enumeration object (as yet uninitialized)
		  ImplAAFTypeDefEnum * ptd = 0;
		  hr = pDict->CreateMetaInstance (AUID_AAFTypeDefEnum, (ImplAAFMetaDefinition **) &ptd);
		  if (AAFRESULT_FAILED (hr))
			return hr;
		  assert (ptd);

		  // count up how many members in this enumeration
		  aafUInt32 numMembers = 0;
		  TypeEnumerationMember ** pMember = (*curEnumeration)->members;
		  while (*pMember)
			{
			  numMembers++;
			  pMember++;
			}
	  
		  ImplAAFTypeDefSP pElemType;
		  // Look up the type of this enumeration
		  hr = pDict->LookupTypeDef(*(*curEnumeration)->pElementTypeId, &pElemType);
		  assert (AAFRESULT_SUCCEEDED (hr));
		  assert (pElemType);

		  // allocate arrays to hold memberTypes pointers and memberNames.
		  aafInt64 * memberValues =	new aafInt64 [numMembers];
		  assert (memberValues);
	  
		  aafString_t * memberNames = 
			new aafString_t[numMembers];
		  assert (memberNames);

		  // fill the types and names arrays.
		  aafUInt32 i;
		  for (i = 0; i < numMembers; i++)
			{
			  memberValues[i] = (*curEnumeration)->members[i]->memberValue;
			  memberNames[i] = (*curEnumeration)->members[i]->memberName;
			  assert (memberNames[i]);
			}

		  // use those arrays to initialize the type def
		  hr = ptd->Initialize ((*curEnumeration)->typeID,
								pElemType,
								memberValues,
								memberNames,
								numMembers,
								(*curEnumeration)->typeName);
		  assert (AAFRESULT_SUCCEEDED (hr));

		  hr = ptd->RegisterSize ((*curEnumeration)->size);
		  assert (AAFRESULT_SUCCEEDED (hr));

		  // clean up
		  delete[] memberValues;
		  delete[] memberNames;

		  assert (ppCreatedTypeDef);
		  *ppCreatedTypeDef = ptd;
		  (*ppCreatedTypeDef)->AcquireReference ();
		  ptd->ReleaseReference ();
		  ptd = 0;
		  return AAFRESULT_SUCCESS;
		}

	  curEnumeration++;
	}
  return AAFRESULT_NO_MORE_OBJECTS;
}

//
// Looks up idToCreate in the structures.  If found, creates and
// initializes a type def to match (using the supplied dictionary),
// and returns it in ppCreatedTypeDef.  Returns true if successful;
// returns false if not found.  Does not register the new type.
//
static AAFRESULT CreateNewExtEnumerationType (const aafUID_t & idToCreate,
									  ImplAAFDictionary * pDict,
									  ImplAAFTypeDef ** ppCreatedTypeDef)
{
  assert (pDict);
  AAFRESULT hr;

  // Go through the enumeration list, attempting to identify the requested
  // ID.
  TypeEnumeration ** curEnumeration = s_AAFAllTypeEnumerations;
  while (*curEnumeration)
	{
	  // Check to see if the current enumeration matches the ID of the type
	  // def we want to create.
	  if (! memcmp (&idToCreate, &(*curEnumeration)->typeID, sizeof (aafUID_t)))
		{		
		  // Yes, this is the one.

		  // Create an impl enumeration object (as yet uninitialized)
		  ImplAAFTypeDefEnum * ptd = 0;
		  hr = pDict->CreateMetaInstance (AUID_AAFTypeDefEnum, (ImplAAFMetaDefinition **) &ptd);
		  assert (AAFRESULT_SUCCEEDED (hr));
		  assert (ptd);

		  // count up how many members in this enumeration
		  aafUInt32 numMembers = 0;
		  TypeEnumerationMember ** pMember = (*curEnumeration)->members;
		  while (*pMember)
			{
			  numMembers++;
			  pMember++;
			}
	  
		  ImplAAFTypeDefSP pElemType;
		  // Look up the type of this enumeration
		  hr = pDict->LookupTypeDef(*(*curEnumeration)->pElementTypeId, &pElemType);
		  assert (AAFRESULT_SUCCEEDED (hr));
		  assert (pElemType);

		  // allocate arrays to hold memberTypes pointers and memberNames.
		  aafInt64 * memberValues =	new aafInt64 [numMembers];
		  assert (memberValues);
	  
		  aafString_t * memberNames = 
			new aafString_t[numMembers];
		  assert (memberNames);

		  // fill the types and names arrays.
		  aafUInt32 i;
		  for (i = 0; i < numMembers; i++)
			{
			  memberValues[i] = (*curEnumeration)->members[i]->memberValue;
			  memberNames[i] = (*curEnumeration)->members[i]->memberName;
			  assert (memberNames[i]);
			}

		  // use those arrays to initialize the type def
		  hr = ptd->Initialize ((*curEnumeration)->typeID,
								pElemType,
								memberValues,
								memberNames,
								numMembers,
								(*curEnumeration)->typeName);
		  assert (AAFRESULT_SUCCEEDED (hr));

		  hr = ptd->RegisterSize ((*curEnumeration)->size);
		  assert (AAFRESULT_SUCCEEDED (hr));

		  // clean up
		  delete[] memberValues;
		  delete[] memberNames;

		  assert (ppCreatedTypeDef);
		  *ppCreatedTypeDef = ptd;
		  (*ppCreatedTypeDef)->AcquireReference ();
		  ptd->ReleaseReference ();
		  ptd = 0;
		  return AAFRESULT_SUCCESS;
		}

	  curEnumeration++;
	}
  return AAFRESULT_NO_MORE_OBJECTS;
}



//
// We have the following structures to work with, defined in
// AAFTypeDefsGen.h:
//
//
// One member of an extendible enumeration typedef:
//
// struct TypeExtEnumerationMember
// {                              
//   wchar_t *        memberName; 
//   aafUID_t         memberValue; 
// };                             
//                                
//
// A extendible enumeration, containing a null-terminated list of TypeExtEnumerationMembers:
//
// struct TypeExtEnumeration              
// {                              
//   aafUID_t   typeID;           
//   wchar_t *  typeName;         
//   TypeExtEnumerationMember * members;
// };
//
//
// A null-terminated array of TypeEnumerations for all enumeration typedefs:
//
// static TypeExtEnumeration * s_AAFAllTypeExtEnumerations[];
//

//
// Looks up idToCreate in the structures.  If found, creates and
// initializes a type def to match (using the supplied dictionary),
// and returns it in ppCreatedTypeDef.  Returns true if successful;
// returns false if not found.  Does not register the new type.
//
static AAFRESULT CreateNewExtendibleEnumerationType (const aafUID_t & idToCreate,
													 ImplAAFDictionary * pDict,
													 ImplAAFTypeDef ** ppCreatedTypeDef)
{
  assert (pDict);
  AAFRESULT hr;

  // Go through the extendible enumeration list, attempting to
  // identify the requested ID.
  TypeExtEnumeration ** curExtEnumeration = s_AAFAllTypeExtEnumerations;
  while (*curExtEnumeration)
	{
	  // Check to see if the current extendible enumeration matches
	  // the ID of the type def we want to create.
	  if (! memcmp (&idToCreate, &(*curExtEnumeration)->typeID, sizeof (aafUID_t)))
		{		
		  // Yes, this is the one.

		  // Create an impl enumeration object (as yet uninitialized)
		  ImplAAFTypeDefExtEnum * ptd = 0;
		  hr = pDict->CreateMetaInstance (AUID_AAFTypeDefExtEnum, (ImplAAFMetaDefinition **) &ptd);
		  if (AAFRESULT_FAILED (hr))
			return hr;
		  assert (ptd);

		  // Initialize the type def
		  hr = ptd->Initialize ((*curExtEnumeration)->typeID,
								(*curExtEnumeration)->typeName);
		  assert (AAFRESULT_SUCCEEDED (hr));

		  // Add any pre-initialized members
		  TypeExtEnumerationMember ** pMember = (*curExtEnumeration)->members;
		  while (*pMember)
			{
			  hr = ptd->AppendElement ((*pMember)->memberValue,
									   (*pMember)->memberName);
			  assert (AAFRESULT_SUCCEEDED (hr));
			  pMember++;
			}
	  
		  assert (ppCreatedTypeDef);
		  *ppCreatedTypeDef = ptd;
		  (*ppCreatedTypeDef)->AcquireReference ();
		  ptd->ReleaseReference ();
		  ptd = 0;
		  return AAFRESULT_SUCCESS;
		}

	  curExtEnumeration++;
	}
  return AAFRESULT_NO_MORE_OBJECTS;
}




//
// We have the following structures to work with, defined in
// AAFTypeDefsGen.h:
//
//
// One member of a record typedef:
//
// struct TypeRecordMember
// {                              
//   aafUID_t *       memberType; 
//   wchar_t *        memberName; 
//   eAAFTypeCategory typeCat;    
// };                             
//                                
//
// A record, containing a null-terminated list of TypeRecordMembers:
//
// struct TypeRecord              
// {                              
//   aafUID_t   typeID;           
//   wchar_t *  typeName;         
//   TypeRecordMember * members;
// };
//
//
// A null-terminated array of TypeRecords for all record typedefs:
//
// static TypeRecord * s_AAFAllTypeRecords[];
//

//
// Looks up idToCreate in the structures.  If found, creates and
// initializes a type def to match (using the supplied dictionary),
// and returns it in ppCreatedTypeDef.  Returns true if successful;
// returns false if not found.  Does not register the new type.
//
static AAFRESULT CreateNewRecordType (const aafUID_t & idToCreate,
								 ImplAAFDictionary * pDict,
								 ImplAAFTypeDef ** ppCreatedTypeDef)
{
  assert (pDict);
  AAFRESULT hr;

  // Go through the record list, attempting to identify the requested
  // ID.
  TypeRecord ** curRecord = s_AAFAllTypeRecords;
  while (*curRecord)
	{
	  // Check to see if the current record matches the ID of the type
	  // def we want to create.
	  if (! memcmp (&idToCreate, &(*curRecord)->typeID, sizeof (aafUID_t)))
		{		
		  // Yes, this is the one.

		  // Create an impl record object (as yet uninitialized)
		  ImplAAFTypeDefRecord * ptd = 0;
		  hr = pDict->CreateMetaInstance (AUID_AAFTypeDefRecord, (ImplAAFMetaDefinition **) &ptd);
		  if (AAFRESULT_FAILED (hr))
			return hr;
		  assert (ptd);

		  // count up how many members in this record
		  aafUInt32 numMembers = 0;
		  TypeRecordMember ** pMember = (*curRecord)->members;
		  while (*pMember)
			{
			  numMembers++;
			  pMember++;
			}
	  
		  // allocate arrays to hold memberTypes, memberNames, and
		  // memberOffsets.
		  ImplAAFTypeDef ** memberTypes =
			new ImplAAFTypeDef * [numMembers];
		  assert (memberTypes);
	  
		  aafString_t * memberNames = 
			new aafString_t[numMembers];
		  assert (memberNames);

		  aafUInt32 * memberOffsets =
			new aafUInt32[numMembers];

		  // fill the types, names, and offsets arrays.
		  aafUInt32 i;
		  for (i = 0; i < numMembers; i++)
			{
			  memberTypes[i] = 0;
			  hr = pDict->LookupTypeDef(*(*curRecord)->members[i]->pMemberTypeId,
									 &memberTypes[i]);
			  assert (AAFRESULT_SUCCEEDED (hr));
			  assert (memberTypes[i]);

			  memberNames[i] = (*curRecord)->members[i]->memberName;
			  assert (memberNames[i]);

			  memberOffsets[i] = (*curRecord)->members[i]->memberOffset;
			}

		  // use those arrays to initialize the type def
		  hr = ptd->Initialize ((*curRecord)->typeID,
								memberTypes,
								memberNames,
								numMembers,
								(*curRecord)->typeName);
		  assert (AAFRESULT_SUCCEEDED (hr));

		  hr = ptd->RegisterMembers (memberOffsets,
									 numMembers,
									 (*curRecord)->size);
		  assert (AAFRESULT_SUCCEEDED (hr));

		  // clean up
		  for (i = 0; i < numMembers; i++)
			{
			  memberTypes[i]->ReleaseReference ();
			  memberTypes[i] = 0;
			}
		  delete[] memberTypes;
		  delete[] memberNames;
		  delete[] memberOffsets;

		  assert (ppCreatedTypeDef);
		  *ppCreatedTypeDef = ptd;
		  (*ppCreatedTypeDef)->AcquireReference ();
		  ptd->ReleaseReference ();
		  ptd = 0;
		  return AAFRESULT_SUCCESS;
		}

	  curRecord++;
	}
  return AAFRESULT_NO_MORE_OBJECTS;
}


static AAFRESULT CreateNewVaryingArrayType (const aafUID_t & idToCreate,
									   ImplAAFDictionary * pDict,
									   ImplAAFTypeDef ** ppCreatedTypeDef)
{
  assert (pDict);
  AAFRESULT hr;

  // Go through the varying array list, attempting to identify the
  // requested ID.
  TypeVaryingArray * curElem = s_AAFAllTypeVaryingArrays;
  while (curElem->isValid)
	{
	  // Check to see if the current ID matches the ID of the type
	  // def we want to create.
	  if (! memcmp (&idToCreate, &curElem->typeId, sizeof (aafUID_t)))
		{		
		  // Yes, this is the one.
		  // Create an impl typedefvaryingArray object (as yet uninitialized)
		  ImplAAFTypeDefVariableArray * ptd = 0;
		  hr = pDict->CreateMetaInstance (AUID_AAFTypeDefVariableArray, (ImplAAFMetaDefinition **) &ptd);
		  if (AAFRESULT_FAILED (hr))
			return hr;
		  assert (ptd);

		  ImplAAFTypeDefSP pElemType;
		  hr = pDict->LookupTypeDef(*curElem->pBaseTypeId, &pElemType);
		  assert (AAFRESULT_SUCCEEDED (hr));
		  assert (pElemType);

		  AAFRESULT hr = ptd->Initialize (curElem->typeId,
										  pElemType,
										  curElem->typeName);
		  assert (AAFRESULT_SUCCEEDED (hr));

		  assert (ppCreatedTypeDef);
		  *ppCreatedTypeDef = ptd;
		  (*ppCreatedTypeDef)->AcquireReference ();
		  ptd->ReleaseReference ();
		  ptd = 0;
		  return AAFRESULT_SUCCESS;
		}

	  curElem++;
	}
  return AAFRESULT_NO_MORE_OBJECTS;
}


static AAFRESULT CreateNewFixedArrayType (const aafUID_t & idToCreate,
									 ImplAAFDictionary * pDict,
									 ImplAAFTypeDef ** ppCreatedTypeDef)
{
  assert (pDict);
  AAFRESULT hr;

  // Go through the fixed array list, attempting to identify the
  // requested ID.
  TypeFixedArray * curElem = s_AAFAllTypeFixedArrays;
  while (curElem->isValid)
	{
	  // Check to see if the current ID matches the ID of the type
	  // def we want to create.
	  if (! memcmp (&idToCreate, &curElem->typeId, sizeof (aafUID_t)))
		{		
		  // Yes, this is the one.
		  // Create an impl typedeffixedArray object (as yet uninitialized)
		  ImplAAFTypeDefFixedArray * ptd = 0;
		  hr = pDict->CreateMetaInstance (AUID_AAFTypeDefFixedArray, (ImplAAFMetaDefinition **) &ptd);
		  if (AAFRESULT_FAILED (hr))
			return hr;
		  assert (ptd);

		  ImplAAFTypeDefSP pElemType;
		  hr = pDict->LookupTypeDef(*curElem->pBaseTypeId, &pElemType);
		  assert (AAFRESULT_SUCCEEDED (hr));
		  assert (pElemType);

		  AAFRESULT hr = ptd->Initialize (curElem->typeId,
										  pElemType,
										  curElem->count,
										  curElem->typeName);
		  assert (AAFRESULT_SUCCEEDED (hr));

		  assert (ppCreatedTypeDef);
		  *ppCreatedTypeDef = ptd;
		  (*ppCreatedTypeDef)->AcquireReference ();
		  ptd->ReleaseReference ();
		  ptd = 0;
		  return AAFRESULT_SUCCESS;
		}

	  curElem++;
	}
  return AAFRESULT_NO_MORE_OBJECTS;
}


static AAFRESULT CreateNewRenameType (const aafUID_t & idToCreate,
								 ImplAAFDictionary * pDict,
								 ImplAAFTypeDef ** ppCreatedTypeDef)
{
  assert (pDict);
  AAFRESULT hr;

  // Go through the rename list, attempting to identify the
  // requested ID.
  TypeRename * curElem = s_AAFAllTypeRenames;
  while (curElem->isValid)
	{
	  // Check to see if the current ID matches the ID of the type
	  // def we want to create.
	  if (! memcmp (&idToCreate, &curElem->typeId, sizeof (aafUID_t)))
		{		
		  // Yes, this is the one.
		  // Create an impl typedefRename object (as yet uninitialized)
		  ImplAAFTypeDefRename * ptd = 0;
		  hr = pDict->CreateMetaInstance (AUID_AAFTypeDefRename, (ImplAAFMetaDefinition **) &ptd);
		  if (AAFRESULT_FAILED (hr))
			return hr;
		  assert (ptd);

		  ImplAAFTypeDefSP pBaseType;
		  hr = pDict->LookupTypeDef(*curElem->pBaseTypeId, &pBaseType);
		  assert (AAFRESULT_SUCCEEDED (hr));
		  assert (pBaseType);

		  AAFRESULT hr = ptd->Initialize (curElem->typeId,
										  pBaseType,
										  curElem->typeName);
		  assert (AAFRESULT_SUCCEEDED (hr));

		  assert (ppCreatedTypeDef);
		  *ppCreatedTypeDef = ptd;
		  (*ppCreatedTypeDef)->AcquireReference ();
		  ptd->ReleaseReference ();
		  ptd = 0;
		  return AAFRESULT_SUCCESS;
		}

	  curElem++;
	}
  return AAFRESULT_NO_MORE_OBJECTS;
}


static AAFRESULT CreateNewStringType (const aafUID_t & idToCreate,
								 ImplAAFDictionary * pDict,
								 ImplAAFTypeDef ** ppCreatedTypeDef)
{
  assert (pDict);
  AAFRESULT hr;

  // Go through the string list, attempting to identify the
  // requested ID.
  TypeString * curElem = s_AAFAllTypeStrings;
  while (curElem->isValid)
	{
	  // Check to see if the current ID matches the ID of the type
	  // def we want to create.
	  if (! memcmp (&idToCreate, &curElem->typeId, sizeof (aafUID_t)))
		{		
		  // Yes, this is the one.
		  // Create an impl typedefString object (as yet uninitialized)
		  ImplAAFTypeDefString * ptd = 0;
		  hr = pDict->CreateMetaInstance (AUID_AAFTypeDefString, (ImplAAFMetaDefinition **) &ptd);
		  if (AAFRESULT_FAILED (hr))
			return hr;
		  assert (ptd);

		  ImplAAFTypeDefSP pElemType;
		  hr = pDict->LookupTypeDef(*curElem->pBaseTypeId, &pElemType);
		  assert (AAFRESULT_SUCCEEDED (hr));
		  assert (pElemType);

		  AAFRESULT hr = ptd->Initialize (curElem->typeId,
										  pElemType,
										  curElem->typeName);
		  assert (AAFRESULT_SUCCEEDED (hr));

		  assert (ppCreatedTypeDef);
		  *ppCreatedTypeDef = ptd;
		  (*ppCreatedTypeDef)->AcquireReference ();
		  ptd->ReleaseReference ();
		  ptd = 0;
		  return AAFRESULT_SUCCESS;
		}

	  curElem++;
	}
  return AAFRESULT_NO_MORE_OBJECTS;
}


static AAFRESULT CreateNewCharacterType (const aafUID_t & idToCreate,
									ImplAAFDictionary * pDict,
									ImplAAFTypeDef ** ppCreatedTypeDef)
{
  assert (pDict);
  AAFRESULT hr;

  // Go through the character list, attempting to identify the requested
  // ID.
  TypeCharacter * curCharacter = s_AAFAllTypeCharacters;
  while (curCharacter->isValid)
	{
	  // Check to see if the current ID matches the ID of the type
	  // def we want to create.
	  if (! memcmp (&idToCreate, &curCharacter->typeID, sizeof (aafUID_t)))
		{		
      assert(curCharacter->size == 2); // we only persist 2 byte unicode characters.

		  // Yes, this is the one.
		  // Create an impl typedefinteger object (as yet uninitialized)
		  ImplAAFTypeDefCharacter * ptd = 0;
		  hr = pDict->CreateMetaInstance (AUID_AAFTypeDefCharacter, (ImplAAFMetaDefinition **) &ptd);
		  if (AAFRESULT_FAILED (hr))
			return hr;
		  assert (ptd);

		  AAFRESULT hr = ptd->pvtInitialize (curCharacter->typeID,
										  curCharacter->typeName);
		  assert (AAFRESULT_SUCCEEDED (hr));

		  assert (ppCreatedTypeDef);
		  *ppCreatedTypeDef = ptd;
		  (*ppCreatedTypeDef)->AcquireReference ();
		  ptd->ReleaseReference ();
		  ptd = 0;
		  return AAFRESULT_SUCCESS;
		}

	  curCharacter++;
	}
  return AAFRESULT_NO_MORE_OBJECTS;
}


static AAFRESULT CreateNewIndirectType (const aafUID_t & idToCreate,
									ImplAAFDictionary * pDict,
									ImplAAFTypeDef ** ppCreatedTypeDef)
{
  assert (pDict);
  AAFRESULT hr;

  // Go through the character list, attempting to identify the requested
  // ID.
  TypeIndirect * curIndirect = s_AAFAllTypeIndirects;
  while (curIndirect->isValid)
	{
	  // Check to see if the current ID matches the ID of the type
	  // def we want to create.
	  if (! memcmp (&idToCreate, &curIndirect->typeID, sizeof (aafUID_t)))
		{		
		  // Yes, this is the one.
		  // Create an impl typedefinteger object (as yet uninitialized)
		  ImplAAFTypeDefIndirect * ptd = 0;
		  hr = pDict->CreateMetaInstance (AUID_AAFTypeDefIndirect, (ImplAAFMetaDefinition **) &ptd);
		  if (AAFRESULT_FAILED (hr))
			return hr;
		  assert (ptd);

		  AAFRESULT hr = ptd->Initialize (curIndirect->typeID, curIndirect->typeName);
		  assert (AAFRESULT_SUCCEEDED (hr));

		  assert (ppCreatedTypeDef);
		  *ppCreatedTypeDef = ptd;
		  (*ppCreatedTypeDef)->AcquireReference ();
		  ptd->ReleaseReference ();
		  ptd = 0;
		  return AAFRESULT_SUCCESS;
		}

	  curIndirect++;
	}
  return AAFRESULT_NO_MORE_OBJECTS;
}


static AAFRESULT CreateNewOpaqueType (const aafUID_t & idToCreate,
									ImplAAFDictionary * pDict,
									ImplAAFTypeDef ** ppCreatedTypeDef)
{
  assert (pDict);
  AAFRESULT hr;

  // Go through the list, attempting to identify the requested
  // ID.
  TypeOpaque * curOpaque = s_AAFAllTypeOpaques;
  while (curOpaque->isValid)
	{
	  // Check to see if the current ID matches the ID of the type
	  // def we want to create.
	  if (! memcmp (&idToCreate, &curOpaque->typeID, sizeof (aafUID_t)))
		{		
		  // Yes, this is the one.
		  ImplAAFTypeDefOpaque * ptd = 0;
		  hr = pDict->CreateMetaInstance (AUID_AAFTypeDefOpaque, (ImplAAFMetaDefinition **) &ptd);
		  if (AAFRESULT_FAILED (hr))
			return hr;
		  assert (ptd);

		  AAFRESULT hr = ptd->Initialize (curOpaque->typeID,
										  curOpaque->typeName);
		  assert (AAFRESULT_SUCCEEDED (hr));

		  assert (ppCreatedTypeDef);
		  *ppCreatedTypeDef = ptd;
		  (*ppCreatedTypeDef)->AcquireReference ();
		  ptd->ReleaseReference ();
		  ptd = 0;
		  return AAFRESULT_SUCCESS;
		}

	  curOpaque++;
	}
  return AAFRESULT_NO_MORE_OBJECTS;
}


static AAFRESULT CreateNewStreamType (const aafUID_t & idToCreate,
									ImplAAFDictionary * pDict,
									ImplAAFTypeDef ** ppCreatedTypeDef)
{
  assert (pDict);
  AAFRESULT hr = AAFRESULT_SUCCESS;
  aafUInt32 index, count;
  assert (ppCreatedTypeDef);

  // Go through the list, attempting to identify the requested
  // ID.
  TypeStream * curStream = &s_AAFAllTypeStreams[0];
  for (index = 0, count = (sizeof(s_AAFAllTypeStreams) / sizeof(TypeStream)); index < count; ++index)
  {
    // Check to see if the current ID matches the ID of the type
    // def we want to create.
    if (! memcmp (&idToCreate, curStream->typeID, sizeof (aafUID_t)))
    {          
      // Yes, this is the one.
      ImplAAFTypeDefStream * ptd = 0;
      hr = pDict->CreateMetaInstance (AUID_AAFTypeDefStream, (ImplAAFMetaDefinition **) &ptd);
      if (AAFRESULT_FAILED (hr))
        return hr;

      hr = ptd->pvtInitialize (*curStream->typeID,
                               curStream->typeName);
      assert (AAFRESULT_SUCCEEDED (hr));
      if (AAFRESULT_SUCCEEDED (hr))
      {
        *ppCreatedTypeDef = ptd; // refcount is 1.
        ptd = NULL;
      }
      else
      {
        ptd->ReleaseReference ();
      }
      return hr;
    }

    curStream++;
  }
  return AAFRESULT_NO_MORE_OBJECTS;
}


static AAFRESULT CreateNewStrongRefType (const aafUID_t & idToCreate,
									ImplAAFDictionary * pDict,
									ImplAAFTypeDef ** ppCreatedTypeDef)
{
  assert (pDict);
  AAFRESULT hr;

  // Go through the StrongRef list, attempting to identify the
  // requested ID.
  TypeStrongRef * curElem = s_AAFAllTypeStrongRefs;
  while (curElem->isValid)
	{
	  // Check to see if the current ID matches the ID of the type
	  // def we want to create.
	  if (! memcmp (&idToCreate, &curElem->typeId, sizeof (aafUID_t)))
		{		
		  // Yes, this is the one.
		  // Create an impl typedefStrongRef object (as yet uninitialized)
		  ImplAAFTypeDefStrongObjRef * ptd = 0;
		  hr = pDict->CreateMetaInstance (AUID_AAFTypeDefStrongObjRef, (ImplAAFMetaDefinition **) &ptd);
		  if (AAFRESULT_FAILED (hr))
			return hr;
		  assert (ptd);

		  ImplAAFClassDefSP pBaseClass;
		  hr = pDict->LookupClassDef(*curElem->pRefdTypeId, &pBaseClass);
		  assert (AAFRESULT_SUCCEEDED (hr));
		  assert (pBaseClass);

		  AAFRESULT hr = ptd->Initialize (curElem->typeId,
										  pBaseClass,
										  curElem->typeName);
		  assert (AAFRESULT_SUCCEEDED (hr));

		  assert (ppCreatedTypeDef);
		  *ppCreatedTypeDef = ptd;
		  (*ppCreatedTypeDef)->AcquireReference ();
		  ptd->ReleaseReference ();
		  ptd = 0;
		  return AAFRESULT_SUCCESS;
		}

	  curElem++;
	}
  return AAFRESULT_NO_MORE_OBJECTS;
}


static AAFRESULT CreateNewStrongRefSetType (const aafUID_t & idToCreate,
									   ImplAAFDictionary * pDict,
									   ImplAAFTypeDef ** ppCreatedTypeDef)
{
  assert (pDict);
  AAFRESULT hr;

  // Go through the StrongRefSet list, attempting to identify the
  // requested ID.
  TypeStrongRefSet * curElem = s_AAFAllTypeStrongRefSets;
  while (curElem->isValid)
	{
	  // Check to see if the current ID matches the ID of the type
	  // def we want to create.
	  if (! memcmp (&idToCreate, &curElem->typeId, sizeof (aafUID_t)))
		{		
#if 0
		  // Yes, this is the one.
		  // Create an impl typedefvariablearray object (as yet uninitialized)
		  ImplAAFTypeDefVariableArray * ptd = 0;
		  hr = pDict->CreateMetaInstance (AUID_AAFTypeDefVariableArray, (ImplAAFMetaDefinition **) &ptd);
		  if (AAFRESULT_FAILED (hr))
			return hr;
		  assert (ptd);
#else
			ImplAAFTypeDefSet* ptd = NULL;
		  hr = pDict->CreateMetaInstance (AUID_AAFTypeDefSet, (ImplAAFMetaDefinition **) &ptd);
		  if (AAFRESULT_FAILED (hr))
				return hr;
		  assert (ptd);
#endif
		  ImplAAFTypeDefSP pRefdType;
		  hr = pDict->LookupTypeDef(*curElem->pRefdTypeId, &pRefdType);
		  assert (AAFRESULT_SUCCEEDED (hr));
		  assert (pRefdType);

		  AAFRESULT hr = ptd->Initialize (curElem->typeId,
										  pRefdType,
										  curElem->typeName);
		  assert (AAFRESULT_SUCCEEDED (hr));

		  assert (ppCreatedTypeDef);
		  *ppCreatedTypeDef = ptd;
		  (*ppCreatedTypeDef)->AcquireReference ();
		  ptd->ReleaseReference ();
		  ptd = 0;
		  return AAFRESULT_SUCCESS;
		}

	  curElem++;
	}
  return AAFRESULT_NO_MORE_OBJECTS;
}


static AAFRESULT CreateNewStrongRefVectorType (const aafUID_t & idToCreate,
										  ImplAAFDictionary * pDict,
										  ImplAAFTypeDef ** ppCreatedTypeDef)
{
  assert (pDict);
  AAFRESULT hr;

  // Go through the StrongRefVector list, attempting to identify the
  // requested ID.
  TypeStrongRefVector * curElem = s_AAFAllTypeStrongRefVectors;
  while (curElem->isValid)
	{
	  // Check to see if the current ID matches the ID of the type
	  // def we want to create.
	  if (! memcmp (&idToCreate, &curElem->typeId, sizeof (aafUID_t)))
		{		
		  // Yes, this is the one.
		  // Create an impl typedefvariablearray object (as yet uninitialized)
		  ImplAAFTypeDefVariableArray * ptd = 0;
		  hr = pDict->CreateMetaInstance (AUID_AAFTypeDefVariableArray, (ImplAAFMetaDefinition **) &ptd);
		  if (AAFRESULT_FAILED (hr))
			return hr;
		  assert (ptd);

		  ImplAAFTypeDefSP pRefdType;
		  hr = pDict->LookupTypeDef(*curElem->pRefdTypeId, &pRefdType);
		  assert (AAFRESULT_SUCCEEDED (hr));
		  assert (pRefdType);

		  AAFRESULT hr = ptd->Initialize (curElem->typeId,
										  pRefdType,
										  curElem->typeName);
		  assert (AAFRESULT_SUCCEEDED (hr));

		  assert (ppCreatedTypeDef);
		  *ppCreatedTypeDef = ptd;
		  (*ppCreatedTypeDef)->AcquireReference ();
		  ptd->ReleaseReference ();
		  ptd = 0;
		  return AAFRESULT_SUCCESS;
		}

	  curElem++;
	}
  return AAFRESULT_NO_MORE_OBJECTS;
}


static AAFRESULT CreateNewWeakRefType
 (const aafUID_t & idToCreate,
								  ImplAAFDictionary * pDict,
								  ImplAAFTypeDef ** ppCreatedTypeDef)
{
  assert (pDict);
  AAFRESULT hr;

  // Go through the WeakRef list, attempting to identify the
  // requested ID.
  TypeWeakRef * curElem = s_AAFAllTypeWeakRefs;
  while (curElem->isValid)
	{
	  // Check to see if the current ID matches the ID of the type
	  // def we want to create.
	  if (! memcmp (&idToCreate, &curElem->typeId, sizeof (aafUID_t)))
		{		
#if ENABLE_WEAK_REFERENCES  // weak refs not implemented yet
      ImplAAFClassDefSP pTargetClass;
      hr = pDict->LookupClassDef (*curElem->pRefdTypeId, &pTargetClass);
      assert (AAFRESULT_SUCCEEDED(hr));
      if (AAFRESULT_FAILED (hr))
        return hr;

		  // Yes, this is the one.
		  // Create an impl typedefWeakRef object (as yet uninitialized)
		  ImplAAFTypeDefWeakObjRef * ptd = 0;
		  hr = pDict->CreateMetaInstance (AUID_AAFTypeDefWeakObjRef, (ImplAAFMetaDefinition **) &ptd);
		  if (AAFRESULT_FAILED (hr))
			return hr;
		  assert (ptd);
		  
		  

      assert(curElem->size > 0);
      aafUID_t * targetSet = new aafUID_t[curElem->size];
      if (NULL == targetSet)
        return AAFRESULT_NOMEMORY;
      
      // Copy the property ids to the targetSet array.  
      aafUInt32 index;
      for (index = 0; index < curElem->size; ++index)
      {
        memcpy(&targetSet[index], curElem->members[index].propertyId, sizeof(aafUID_t));
      }

		  AAFRESULT hr = ptd->Initialize (curElem->typeId,
										  pTargetClass,
										  curElem->typeName,
										  curElem->size,
										  targetSet);
			delete [] targetSet;
			targetSet = NULL;
			if (AAFRESULT_FAILED(hr))
			  return hr;
#else
		  ImplAAFTypeDef * ptd = 0;
		  // Instead, alias to an auid
		  if(memcmp(curElem->pRefdTypeId, &kAAFClassID_Mob, sizeof(aafUID_t)) == 0)
		  {
			hr = pDict->LookupTypeDef (kAAFTypeID_MobID, &ptd);
		  }
		  else
		  {
			hr = pDict->LookupTypeDef (kAAFTypeID_AUID, &ptd);
		  }
		  assert (AAFRESULT_SUCCEEDED (hr));
#endif

		  assert (ppCreatedTypeDef);
		  *ppCreatedTypeDef = ptd;
		  (*ppCreatedTypeDef)->AcquireReference ();
		  ptd->ReleaseReference ();
		  ptd = 0;
		  return AAFRESULT_SUCCESS;
		}

	  curElem++;
	}
  return AAFRESULT_NO_MORE_OBJECTS;
}


static AAFRESULT CreateNewWeakRefSetType (const aafUID_t & idToCreate,
									 ImplAAFDictionary * pDict,
									 ImplAAFTypeDef ** ppCreatedTypeDef)
{
  assert (pDict);
  AAFRESULT hr;

  // Go through the WeakRefSet list, attempting to identify the
  // requested ID.
  TypeWeakRefSet * curElem = s_AAFAllTypeWeakRefSets;
  while (curElem->isValid)
	{
	  // Check to see if the current ID matches the ID of the type
	  // def we want to create.
	  if (! memcmp (&idToCreate, &curElem->typeId, sizeof (aafUID_t)))
		{		
#if ENABLE_WEAK_REFERENCES  // weak refs not implemented yet
			ImplAAFTypeDefSet* ptd = NULL;
		  hr = pDict->CreateMetaInstance (AUID_AAFTypeDefSet, (ImplAAFMetaDefinition **) &ptd);
		  if (AAFRESULT_FAILED (hr))
				return hr;
		  assert (ptd);

      ImplAAFTypeDefSP pRefdType;
		  hr = pDict->LookupTypeDef(*curElem->pRefdTypeId, &pRefdType);
#else
      // Yes, this is the one.
		  // Create an impl typedefvariablearray object (as yet uninitialized)
		  ImplAAFTypeDefVariableArray * ptd = 0;
		  hr = pDict->CreateMetaInstance (AUID_AAFTypeDefVariableArray, (ImplAAFMetaDefinition **) &ptd);
		  if (AAFRESULT_FAILED (hr))
			return hr;
		  assert (ptd);
      
      // Temporarily pretend that weak reference sets are still implemented as arrays
      // of AUIDs
      ImplAAFTypeDefSP pRefdType;
		  hr = pDict->LookupTypeDef(kAAFTypeID_AUID, &pRefdType);
#endif

		  assert (AAFRESULT_SUCCEEDED (hr));
		  assert (pRefdType);

		  AAFRESULT hr = ptd->Initialize (curElem->typeId,
										  pRefdType,
										  curElem->typeName);
		  assert (AAFRESULT_SUCCEEDED (hr));

		  assert (ppCreatedTypeDef);
		  *ppCreatedTypeDef = ptd;
		  (*ppCreatedTypeDef)->AcquireReference ();
		  ptd->ReleaseReference ();
		  ptd = 0;
		  return AAFRESULT_SUCCESS;
		}

	  curElem++;
	}
  return AAFRESULT_NO_MORE_OBJECTS;
}

static AAFRESULT CreateNewWeakRefVectorType (const aafUID_t & idToCreate,
										ImplAAFDictionary * pDict,
										ImplAAFTypeDef ** ppCreatedTypeDef)
{
  assert (pDict);
  AAFRESULT hr;

  // Go through the WeakRefVector list, attempting to identify the
  // requested ID.
  TypeWeakRefVector * curElem = s_AAFAllTypeWeakRefVectors;
  while (curElem->isValid)
	{
	  // Check to see if the current ID matches the ID of the type
	  // def we want to create.
	  if (! memcmp (&idToCreate, &curElem->typeId, sizeof (aafUID_t)))
		{		
		  // Yes, this is the one.
		  // Create an impl typedefvariablearray object (as yet uninitialized)
		  ImplAAFTypeDefVariableArray * ptd = 0;
		  hr = pDict->CreateMetaInstance (AUID_AAFTypeDefVariableArray, (ImplAAFMetaDefinition **) &ptd);
		  if (AAFRESULT_FAILED (hr))
			return hr;
		  assert (ptd);

		  ImplAAFTypeDefSP pRefdType;
#if ENABLE_WEAK_REFERENCES
		  hr = pDict->LookupTypeDef(*curElem->pRefdTypeId, &pRefdType);
#else     
		  // Temporarily pretend that weak reference sets are still implemented as arrays
		  // of AUIDs
		  hr = pDict->LookupTypeDef(kAAFTypeID_AUID, &pRefdType);
#endif
		  assert (AAFRESULT_SUCCEEDED (hr));
		  assert (pRefdType);

		  AAFRESULT hr = ptd->Initialize (curElem->typeId,
										  pRefdType,
										  curElem->typeName);
		  assert (AAFRESULT_SUCCEEDED (hr));

		  assert (ppCreatedTypeDef);
		  *ppCreatedTypeDef = ptd;
		  (*ppCreatedTypeDef)->AcquireReference ();
		  ptd->ReleaseReference ();
		  ptd = 0;
		  return AAFRESULT_SUCCESS;
		}

	  curElem++;
	}
  return AAFRESULT_NO_MORE_OBJECTS;
}


ImplAAFBuiltinTypes::ImplAAFBuiltinTypes (ImplAAFDictionary* dictionary) :
  _dictionary(dictionary)
{}


AAFRESULT ImplAAFBuiltinTypes::ImportBuiltinTypeDef
(const aafUID_t & idToCreate,
 ImplAAFTypeDef ** ppResult)
{
  AAFRESULT hr;
  ImplAAFUID popped;

  assert (! _lookupStack.isPresent (idToCreate));

  _lookupStack.push (idToCreate);

  hr = NewBuiltinTypeDef (idToCreate, ppResult);
  if (AAFRESULT_SUCCEEDED (hr))
	{
	  assert (*ppResult);
	  assert (_dictionary);
	  AAFRESULT hr = _dictionary->RegisterTypeDef (*ppResult);
	  if (AAFRESULT_FAILED (hr))
		{
		  assert (*ppResult);
		  (*ppResult)->ReleaseReference ();
		  *ppResult = 0;
		}
	}

  popped = _lookupStack.pop ();
  assert (popped == idToCreate);
  return hr;
}


AAFRESULT ImplAAFBuiltinTypes::NewBuiltinTypeDef
(const aafUID_t & idToCreate,
 ImplAAFTypeDef ** ppCreatedTypeDef)
{
  AAFRESULT hr;

  hr = CreateNewIntegerType (idToCreate,
							 _dictionary,
							 ppCreatedTypeDef);
  if (AAFRESULT_SUCCEEDED (hr))	return hr;

  hr = CreateNewEnumerationType (idToCreate,
								 _dictionary,
								 ppCreatedTypeDef);
  if (AAFRESULT_SUCCEEDED (hr))	return hr;

  hr = CreateNewExtendibleEnumerationType (idToCreate,
										   _dictionary,
										   ppCreatedTypeDef);
  if (AAFRESULT_SUCCEEDED (hr))	return hr;

  hr = CreateNewRecordType (idToCreate,
							_dictionary,
							ppCreatedTypeDef);
  if (AAFRESULT_SUCCEEDED (hr))	return hr;

  hr = CreateNewVaryingArrayType (idToCreate,
								  _dictionary,
								  ppCreatedTypeDef);
  if (AAFRESULT_SUCCEEDED (hr))	return hr;

  hr = CreateNewFixedArrayType (idToCreate,
								_dictionary,
								ppCreatedTypeDef);
  if (AAFRESULT_SUCCEEDED (hr))	return hr;

  hr = CreateNewRenameType (idToCreate,
							_dictionary,
							ppCreatedTypeDef);
  if (AAFRESULT_SUCCEEDED (hr))	return hr;

  hr = CreateNewCharacterType (idToCreate,
							   _dictionary,
							   ppCreatedTypeDef);
  if (AAFRESULT_SUCCEEDED (hr))	return hr;

  hr = CreateNewStringType (idToCreate,
							_dictionary,
							ppCreatedTypeDef);
  if (AAFRESULT_SUCCEEDED (hr))	return hr;

  hr = CreateNewIndirectType (idToCreate,
							   _dictionary,
							   ppCreatedTypeDef);
  if (AAFRESULT_SUCCEEDED (hr))	return hr;

  hr = CreateNewOpaqueType (idToCreate,
							   _dictionary,
							   ppCreatedTypeDef);
  if (AAFRESULT_SUCCEEDED (hr))	return hr;

  hr = CreateNewStrongRefType (idToCreate,
							   _dictionary,
							   ppCreatedTypeDef);
  if (AAFRESULT_SUCCEEDED (hr))	return hr;

  hr = CreateNewStrongRefSetType (idToCreate,
								  _dictionary,
								  ppCreatedTypeDef);
  if (AAFRESULT_SUCCEEDED (hr))	return hr;

  hr = CreateNewStrongRefVectorType (idToCreate,
									 _dictionary,
									 ppCreatedTypeDef);
  if (AAFRESULT_SUCCEEDED (hr))	return hr;

  hr = CreateNewWeakRefType (idToCreate,
							 _dictionary,
							 ppCreatedTypeDef);
  if (AAFRESULT_SUCCEEDED (hr))	return hr;

  hr = CreateNewWeakRefSetType (idToCreate,
								_dictionary,
								ppCreatedTypeDef);
  if (AAFRESULT_SUCCEEDED (hr))	return hr;

  hr = CreateNewWeakRefVectorType (idToCreate,
								   _dictionary,
								   ppCreatedTypeDef);
  if (AAFRESULT_SUCCEEDED (hr))	return hr;

  hr = CreateNewStreamType (idToCreate,
                            _dictionary,
                            ppCreatedTypeDef);
  if (AAFRESULT_SUCCEEDED (hr))	return hr;

  // all known types failed
  return AAFRESULT_NO_MORE_OBJECTS;
}


/*static*/
void ImplAAFBuiltinTypes::RegisterExistingType
  (ImplAAFTypeDefEnum * ptde)
{
  assert (ptde);

  AAFRESULT hr;
  aafUID_t idToRegister;

  hr = ptde->GetAUID (&idToRegister);
  assert (AAFRESULT_SUCCEEDED (hr));

  // Go through the enumeration list, attempting to identify the requested
  // ID.
  TypeEnumeration ** curEnumeration = s_AAFAllTypeEnumerations;
  while (*curEnumeration)
	{
	  // Check to see if the current enumeration matches the ID of the type
	  // def we want to create.
	  if (! memcmp (&idToRegister, &(*curEnumeration)->typeID, sizeof (aafUID_t)))
		{		
		  // Yes, this is the one.

		  hr = ptde->RegisterSize ((*curEnumeration)->size);
		  assert (AAFRESULT_SUCCEEDED (hr));
		  return;
		}
	  curEnumeration++;
	}
}


/*static*/
void ImplAAFBuiltinTypes::RegisterExistingType
  (ImplAAFTypeDefRecord * ptdr)
{
  assert (ptdr);

  AAFRESULT hr;
  aafUID_t idToRegister;

  hr = ptdr->GetAUID (&idToRegister);
  assert (AAFRESULT_SUCCEEDED (hr));

  // Go through the enumeration list, attempting to identify the requested
  // ID.
  TypeRecord ** curRecord = s_AAFAllTypeRecords;
  while (*curRecord)
	{
	  // Check to see if the current record matches the ID of the type
	  // def we want to create.
	  if (! memcmp (&idToRegister, &(*curRecord)->typeID, sizeof (aafUID_t)))
		{		
		  // Yes, this is the one.

		  // count up how many members in this record
		  aafUInt32 numMembers = 0;
		  TypeRecordMember ** pMember = (*curRecord)->members;
		  while (*pMember)
			{
			  numMembers++;
			  pMember++;
			}
	  
		  // allocate array to hold memberOffsets
		  aafUInt32 * memberOffsets =
			new aafUInt32[numMembers];

		  // fill the offsets array
		  aafUInt32 i;
		  for (i = 0; i < numMembers; i++)
			{
			  memberOffsets[i] = (*curRecord)->members[i]->memberOffset;
			}

		  hr = ptdr->RegisterMembers (memberOffsets,
									  numMembers,
									 (*curRecord)->size);
		  assert (AAFRESULT_SUCCEEDED (hr));

		  // clean up
		  delete[] memberOffsets;
		  return;
		}
	  curRecord++;
	}
}

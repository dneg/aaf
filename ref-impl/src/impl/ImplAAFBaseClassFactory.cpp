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

#include <stdio.h>
#include <stdlib.h>


#include "ImplAAFBaseClassFactory.h"

#include "ImplAAFObject.h"
#include "OMFile.h"
#include "OMClassFactory.h"

#include "ImplAAFObjectCreation.h"


#include <assert.h>


//
// Initialize the AUID's.
#define INIT_AUID
#include "AAFStoredObjectIDs.h"
#undef INIT_AUID

//
// Declare the corresponding CLSID's external.
#define AAF_CLASS(name, dataid, parent, concrete)\
  extern "C" aafClassID_t CLSID_AAF##name;

// The AAF reference implementation is still not quite in sync with SMPTE
// so we have to alias some of the SMPTE names to their corresponding
// name in AAF.
//
#define CLSID_AAFClassDefinition CLSID_AAFClassDef
#define CLSID_AAFDataDefinition CLSID_AAFDataDef
#define CLSID_AAFDefinitionObject CLSID_AAFDefObject
#define CLSID_AAFEdgeCode CLSID_AAFEdgecode
#define CLSID_AAFOperationDefinition CLSID_AAFOperationDef
#define CLSID_AAFInterchangeObject CLSID_AAFObject
#define CLSID_AAFJPEGImageData CLSID_AAFJPEGData
#define CLSID_AAFMIDIFileData CLSID_AAFMIDIData
#define CLSID_AAFParameterDefinition CLSID_AAFParameterDef
#define CLSID_AAFPropertyDefinition CLSID_AAFPropertyDef
#define CLSID_AAFTypeDefinition CLSID_AAFTypeDef
#define CLSID_AAFTypeDefinitionCharacter CLSID_AAFTypeDefCharacter
#define CLSID_AAFTypeDefinitionEnumeration CLSID_AAFTypeDefEnum
#define CLSID_AAFTypeDefinitionExtendibleEnumeration CLSID_AAFTypeDefExtEnum
#define CLSID_AAFTypeDefinitionFixedArray CLSID_AAFTypeDefFixedArray
#define CLSID_AAFTypeDefinitionInteger CLSID_AAFTypeDefInt
#define CLSID_AAFTypeDefinitionRecord CLSID_AAFTypeDefRecord
#define CLSID_AAFTypeDefinitionRename CLSID_AAFTypeDefRename
#define CLSID_AAFTypeDefinitionSet CLSID_AAFTypeDefSet
#define CLSID_AAFTypeDefinitionStream CLSID_AAFTypeDefStream
#define CLSID_AAFTypeDefinitionString CLSID_AAFTypeDefString
#define CLSID_AAFTypeDefinitionIndirect CLSID_AAFTypeDefIndirect
#define CLSID_AAFTypeDefinitionStrongObjectReference CLSID_AAFTypeDefStrongObjRef
#define CLSID_AAFTypeDefinitionVariableArray CLSID_AAFTypeDefVariableArray
#define CLSID_AAFTypeDefinitionWeakObjectReference CLSID_AAFTypeDefWeakObjRef
#define CLSID_AAFUNIXLocator CLSID_AAFUnixLocator
#define CLSID_AAFCodecDefinition CLSID_AAFCodecDef
#define CLSID_AAFContainerDefinition CLSID_AAFContainerDef
#define CLSID_AAFInterpolationDefinition CLSID_AAFInterpolationDef

//
// Include the AAF macro invocations.
#include "AAFMetaDictionary.h"



//
// Initialize the AUID to CLSID class table.
typedef struct tagAAFObjectEntry_t
{
	const char * pClassName;
	const aafUID_t* pAUID;
	const aafClassID_t* pClassID;
} AAFObjectEntry_t;

#define AAF_TABLE_BEGIN() static AAFObjectEntry_t gAAFObjectTable[] = {

#define AAF_CLASS(name, id, parent, concrete)\
{ #name, &AUID_AAF##name, &CLSID_AAF##name }

#define AAF_CLASS_SEPARATOR() ,

#define AAF_TABLE_END() };
//
// Include the AAF macro invocations.
// This will define all of the entries in the gAAFObjectTable.
#include "AAFMetaDictionary.h"

// Define a table of pointers to entries on the gAAFObjectTable. We use 
// this table to sort and search for AUID's in the gAAFObjectTable.
const size_t kTotalAUIDCount = sizeof(gAAFObjectTable)/sizeof(AAFObjectEntry_t);
static AAFObjectEntry_t * g_AUIDTable[kTotalAUIDCount] = {0};


// Declare a global instance of the factory so that the initialization
// code will only be called once.
static ImplAAFBaseClassFactory s_AAFBaseClassFactory;


// Compare proc for sort and search.
static int CompareTableEntries(const AAFObjectEntry_t **elem1,
                               const AAFObjectEntry_t **elem2)
{
  const aafUID_t &auid1 = *((**elem1).pAUID);
  const aafUID_t &auid2 = *((**elem2).pAUID);

  // Compare the unsigned long member
  if (auid1.Data1 < auid2.Data1)
    return -1;
  else if (auid1.Data1 > auid2.Data1)
    return 1;
  // Compare the first unsigned short member
  else if (auid1.Data2 < auid2.Data2)
    return -1;
  else if (auid1.Data2 > auid2.Data2)
    return 1;
  // Compare the second unsigned short member
  else if (auid1.Data3 < auid2.Data3)
    return -1;
  else if (auid1.Data3 > auid2.Data3)
    return 1;
  else
  // Compare the last 8 bytes.
    return memcmp(auid1.Data4, auid2.Data4, 8);
}

static void InitializeAUIDTable(void)
{
  for (size_t i = 0; kTotalAUIDCount > i; ++i)
  {
    g_AUIDTable[i] = &gAAFObjectTable[i];
  }

  // Now sort the table.
  qsort(g_AUIDTable, kTotalAUIDCount, sizeof(AAFObjectEntry_t *),
        (int (*)(const void*, const void*))CompareTableEntries); 
}


ImplAAFBaseClassFactory::ImplAAFBaseClassFactory(void)
{
  // Initialize our lookup table for the built-in base class auids.
  if (0 == g_AUIDTable[0])
    InitializeAUIDTable();
}

ImplAAFBaseClassFactory::~ImplAAFBaseClassFactory(void)
{
}



// Global function that looksup the built-in code class id for the corresponding
// auid.
const aafClassID_t* ImplAAFBaseClassFactory::LookupClassID(const aafUID_t & auid)
{
  // Return NULL if the given AUID cannot be found.
  const aafClassID_t *pClassID = NULL;

  // Lookup the class id in the predefined "base class" table.
  AAFObjectEntry_t **ppResult = NULL;
  AAFObjectEntry_t key = {"KEY", &auid, NULL};
  AAFObjectEntry_t *pKey = &key;
  
  // Use standard library's binary search routine.
  ppResult = (AAFObjectEntry_t **)bsearch(&pKey, g_AUIDTable, kTotalAUIDCount,
               sizeof(AAFObjectEntry_t *),
               (int (*)(const void*, const void*))CompareTableEntries);

  // Return the corresponding class id.
  if (NULL != ppResult)
    pClassID = (**ppResult).pClassID;

  return (pClassID);
}

#include <stdio.h>
#include <stdlib.h>
#include "Container.h"
#include "ImplAAFObject.h"
#include "AAFUtils.h"
#include "aafCvt.h"

#include "ImplAAFHeader.h"
#include "OMFile.h"
#include "OMClassFactory.h"
#include "ImplAAFIdentification.h"
#include "ImplAAFContentStorage.h"

#include "ImplAAFObjectCreation.h"


#include <assert.h>


//
// Initialize the AUID's.
#define INIT_AUID
#include "AAFStoredObjectIDs.h"
#undef INIT_AUID

//
// Declare the corresponding CLSID's external.
#define AAF_CLASS(name, dataid, parent)\
  extern "C" aafClassID_t CLSID_AAF##name;

// The AAF reference implementation is still not quite in sync with SMPTE
// so we have to alias some of the SMPTE names to their corresponding
// name in AAF.
//
#define CLSID_AAFClassDefinition CLSID_AAFClassDef
#define CLSID_AAFConstantValue CLSID_AAFConstValue
#define CLSID_AAFDataDefinition CLSID_AAFDataDef
#define CLSID_AAFDefinitionObject CLSID_AAFDefObject
#define CLSID_AAFEdgeCode CLSID_AAFEdgecode
#define CLSID_AAFEffectDefinition CLSID_AAFEffectDef
#define CLSID_AAFEssenceGroup CLSID_AAFMediaGroup
#define CLSID_AAFFilmDescriptor CLSID_AAFMediaFilmDescriptor
#define CLSID_AAFGroup CLSID_AAFEffectInvocation
#define CLSID_AAFInterchangeObject CLSID_AAFObject
#define CLSID_AAFJPEGImageData CLSID_AAFJPEGData
#define CLSID_AAFMIDIFileData CLSID_AAFMIDIData
#define CLSID_AAFMIDIFileDescriptor CLSID_AAFMIDIDescriptor
#define CLSID_AAFParameterDefinition CLSID_AAFParameterDef
#define CLSID_AAFPropertyDefinition CLSID_AAFPropertyDef
#define CLSID_AAFTypeDefinition CLSID_AAFTypeDef
#define CLSID_AAFUNIXLocator CLSID_AAFUnixLocator

//
// Include the AAF macro invocations.
#include "AAFMetaDictionary.h"
// cleanup symbols.
#undef AAF_CLASS



//
// Initialize the AUID to CLSID class table.
typedef struct tagAAFObjectEntry_t
{
	const char * pClassName;
	const aafUID_t* pAUID;
	const aafClassID_t* pClassID;
} AAFObjectEntry_t;

// cleanup symbols.
#undef AAF_TABLE_BEGIN
#undef AAF_CLASS
#undef AAF_CLASS_SEPARATOR
#undef AAF_TABLE_END
#define AAF_TABLE_BEGIN() static AAFObjectEntry_t gAAFObjectTable[] = {

#define AAF_CLASS(name, id, parent)\
{ #name, &AUID_AAF##name, &CLSID_AAF##name }

#define AAF_CLASS_SEPARATOR() ,

#define AAF_TABLE_END() };
//
// Include the AAF macro invocations.
// This will define all of the entries in the gAAFObjectTable.
#include "AAFMetaDictionary.h"
// cleanup symbols.
#undef AAF_TABLE_BEGIN
#undef AAF_CLASS
#undef AAF_CLASS_SEPARATOR
#undef AAF_TABLE_END

// Define a table of pointers to entries on the gAAFObjectTable. We use 
// this table to sort and search for AUID's in the gAAFObjectTable.
const size_t kTotalAUIDCount = sizeof(gAAFObjectTable)/sizeof(AAFObjectEntry_t);
static AAFObjectEntry_t * g_AUIDTable[kTotalAUIDCount] = {0};


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


//!This file should be merged into other files over time

OMContainer::OMContainer(void)
  : _file(0)
{
  // Initialize out lookup table for the built-in base class auids.
  if (0 == g_AUIDTable[0])
    InitializeAUIDTable();
}

OMContainer::~OMContainer(void)
{
  // cleanup the file reference if on exists.
  if (_file)
  {
    delete _file;
    _file = 0;
  }
}




// Close and save the file
void OMContainer::OMLCloseContainer(void)
{
  _file->save();
  _file->close();

  // cleanup the file reference. We need to do this since
  // every open of the container creates a new file.
  if (_file)
  {
    delete _file;
    _file = 0;
  }

}

// Close without saving the file
void OMContainer::OMLAbortContainer(void)
{
  // cleanup the file reference. We need to do this since
  // every open of the container creates a new file.
  if (_file)
  {
    delete _file;
    _file = 0;
  }
}

// Temporary function that looksup the code class id for the corresponding
// auid.
const aafClassID_t* lookupClassID(const aafUID_t* pAUID)
{
  // Return NULL if the given AUID cannot be found.
  const aafClassID_t *pClassID = NULL;

  // Lookup the class id in the predefined "base class" table.
  AAFObjectEntry_t **ppResult = NULL;
  AAFObjectEntry_t key = {"KEY", pAUID, NULL};
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

// Utility function for creating objects. This function hides the type
// "aafClassID_t" from the OM.
//
static OMStorable* createObject(const OMClassId& classId)
{
  const aafUID_t* pAUID  = reinterpret_cast<const aafUID_t*>(&classId);

  // Lookup the code class id for the given stored object id.
  const aafClassID_t* id = lookupClassID(pAUID);
  if (NULL == id)
    return NULL;
  
  // Attempt to create the corresponding storable object.
  OMStorable* result = dynamic_cast<OMStorable*>(CreateImpl(*id));
  return result;
}


// Utility function for registering a given class id as legal in a
// given file.This function hides the type "aafClassID_t" from the OM.
//
static void registerClass(OMFile* file, const aafClassID_t& classId)
{
  file->classFactory()->add((const OMClassId&)(classId),
                            createObject);
}

static void registerPredefinedClasses(OMFile* file)
{
#if 1
  // Use the table to register the builtin classes.
  for (size_t i = 0; kTotalAUIDCount > i; ++i)
  {
    registerClass(file, *gAAFObjectTable[i].pAUID);
  }
#else
  registerClass(file, AUID_AAFClassDef);
  registerClass(file, AUID_AAFComponent);
  registerClass(file, AUID_AAFCompositionMob);
  registerClass(file, AUID_AAFConstValue);
  registerClass(file, AUID_AAFContentStorage);
  registerClass(file, AUID_AAFControlPoint);
  registerClass(file, AUID_AAFDataDef);
  registerClass(file, AUID_AAFDefObject);
  registerClass(file, AUID_AAFDictionary);
  registerClass(file, AUID_AAFEdgecode);
  registerClass(file, AUID_AAFEffectDef);
  registerClass(file, AUID_AAFEffectInvocation);
  registerClass(file, AUID_AAFEssenceData);
  registerClass(file, AUID_AAFEssenceDescriptor);
  registerClass(file, AUID_AAFFileDescriptor);
  registerClass(file, AUID_AAFFiller);
  registerClass(file, AUID_AAFHeader);
  registerClass(file, AUID_AAFIdentification);
  registerClass(file, AUID_AAFLocator);
  registerClass(file, AUID_AAFMacLocator);
  registerClass(file, AUID_AAFMasterMob);
  registerClass(file, AUID_AAFMediaFilmDescriptor);
  registerClass(file, AUID_AAFMediaGroup);
  registerClass(file, AUID_AAFTapeDescriptor);
  registerClass(file, AUID_AAFWAVEDescriptor);
  registerClass(file, AUID_AAFDigitalImageDescriptor);
  registerClass(file, AUID_AAFCDCIDescriptor);
  registerClass(file, AUID_AAFMob);
  registerClass(file, AUID_AAFMobSlot);
  registerClass(file, AUID_AAFNestedScope);
  registerClass(file, AUID_AAFNetworkLocator);
  registerClass(file, AUID_AAFObject);
  registerClass(file, AUID_AAFParameter);
  registerClass(file, AUID_AAFParameterDef);
  registerClass(file, AUID_AAFPropertyDef);
  registerClass(file, AUID_AAFPulldown);
  registerClass(file, AUID_AAFReferenceValue);
  registerClass(file, AUID_AAFScopeReference);
  registerClass(file, AUID_AAFSegment);
  registerClass(file, AUID_AAFSequence);
  registerClass(file, AUID_AAFSourceClip);
  registerClass(file, AUID_AAFSourceMob);
  registerClass(file, AUID_AAFSourceReference);
  registerClass(file, AUID_AAFTimecode);
  registerClass(file, AUID_AAFTimecodeStream);
  registerClass(file, AUID_AAFTimecodeStream12M);
  registerClass(file, AUID_AAFTimelineMobSlot);
  registerClass(file, AUID_AAFTransition);
  registerClass(file, AUID_AAFTypeDef);
  registerClass(file, AUID_AAFUnixLocator);
  registerClass(file, AUID_AAFVaryingValue);
  registerClass(file, AUID_AAFWindowsLocator);
#endif
}

// Open a file
void OMContainer::OMLOpenContainer(aafWChar* stream,
                                 OMLSession sessionData,
                                 OMLRefCon attributes,
                                 OMLconst_OMLGlobalName typeName, 
                                 OMLContainerUseMode useFlags,
                                 ImplAAFHeader*& header)
{
  assert(0 ==_file);
  _file = OMFile::openRead(stream);

  registerPredefinedClasses(_file);

  OMStorable* head = _file->restore();
  header = dynamic_cast<ImplAAFHeader *>(head);
}

//Will remove this!
void OMContainer::OMLSetContainerVersion1(void)
{
}

//Create a file
void OMContainer::OMLOpenNewContainer(aafWChar* stream,
                                  ImplAAFHeader* head,
                                  OMLSession sessionData,
                                  OMLRefCon attributes,
                                  OMLconst_OMLGlobalName typeName, 
                                  OMLContainerUseMode useFlags,
                                  OMLGeneration generation,
                                  OMLContainerFlags containerFlags, ...)
{
  assert(0 ==_file);
  _file = OMFile::createModify(stream, head);
}


















// DEcrement the reference count
void OMContainer::OMLReleaseObject(OMLObject theObject)
{
}




// 'Nuff said
void aafOMLError(OMLSession sessionData, OMLErrorString message, ...)
{
}

// 'Nuff said
OMLErrorString OMLVGetErrorString(OMLErrorString errorString, aafInt32 maxLength, 
                                             OMLErrorNbr errorNumber, va_list inserts)
{
	return(0);
}



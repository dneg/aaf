#include <stdio.h>
#include <stdlib.h>
#include "Container.h"
#include "ImplAAFObject.h"
#include "AAFUtils.h"
#include "aafansic.h"
#include "aafCvt.h"

#include "ImplAAFHeader.h"
#include "OMFile.h"
#include "OMClassFactory.h"
#include "ImplAAFIdentification.h"
#include "ImplAAFContentStorage.h"

#include "ImplAAFObjectCreation.h"

#include <assert.h>

//!This file should be merged into other files over time

OMContainer::OMContainer(void)
  : _file(0)
{
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

// Utility function for creating objects. This function hides the type
// "aafClassID_t" from the OM.
//
static OMStorable* createObject(const OMClassId& classId)
{
  const aafClassID_t* id = reinterpret_cast<const aafClassID_t*>(&classId);

  OMStorable* result = dynamic_cast<OMStorable*>(CreateImpl(*id));
  return result;
}

// Class ids
//
extern "C" const aafClassID_t CLSID_AAFAttribute;
extern "C" const aafClassID_t CLSID_AAFClassDef;
extern "C" const aafClassID_t CLSID_AAFComponent;
extern "C" const aafClassID_t CLSID_AAFCompositionMob;
extern "C" const aafClassID_t CLSID_AAFConstValue;
extern "C" const aafClassID_t CLSID_AAFContentStorage;
extern "C" const aafClassID_t CLSID_AAFControlCodeDef;
extern "C" const aafClassID_t CLSID_AAFControlPoint;
extern "C" const aafClassID_t CLSID_AAFDataDef;
extern "C" const aafClassID_t CLSID_AAFDefObject;
extern "C" const aafClassID_t CLSID_AAFDictionary;
extern "C" const aafClassID_t CLSID_AAFEdgecode;
extern "C" const aafClassID_t CLSID_AAFEffectDef;
extern "C" const aafClassID_t CLSID_AAFEffectInvocation;
extern "C" const aafClassID_t CLSID_AAFEssenceDescriptor;
extern "C" const aafClassID_t CLSID_AAFFile;
extern "C" const aafClassID_t CLSID_AAFFileDescriptor;
extern "C" const aafClassID_t CLSID_AAFFiller;
extern "C" const aafClassID_t CLSID_AAFFindSourceInfo;
extern "C" const aafClassID_t CLSID_AAFHeader;
extern "C" const aafClassID_t CLSID_AAFIdentification;
extern "C" const aafClassID_t CLSID_AAFIntegerAttribute;
extern "C" const aafClassID_t CLSID_AAFLocator;
extern "C" const aafClassID_t CLSID_AAFMacLocator;
extern "C" const aafClassID_t CLSID_AAFMasterMob;
extern "C" const aafClassID_t CLSID_AAFMedia;
extern "C" const aafClassID_t CLSID_AAFMediaFilmDescriptor;
extern "C" const aafClassID_t CLSID_AAFMediaGroup;
extern "C" const aafClassID_t CLSID_AAFTapeDescriptor;
extern "C" const aafClassID_t CLSID_AAFMob;
extern "C" const aafClassID_t CLSID_AAFMobSlot;
extern "C" const aafClassID_t CLSID_AAFNestedScope;
extern "C" const aafClassID_t CLSID_AAFNetworkLocator;
extern "C" const aafClassID_t CLSID_AAFObject;
extern "C" const aafClassID_t CLSID_AAFObjectAttribute;
extern "C" const aafClassID_t CLSID_AAFParameter;
extern "C" const aafClassID_t CLSID_AAFParameterSlot;
extern "C" const aafClassID_t CLSID_AAFPropertyDef;
extern "C" const aafClassID_t CLSID_AAFPulldown;
extern "C" const aafClassID_t CLSID_AAFReferenceValue;
extern "C" const aafClassID_t CLSID_AAFRoot;
extern "C" const aafClassID_t CLSID_AAFScopeReference;
extern "C" const aafClassID_t CLSID_AAFSegment;
extern "C" const aafClassID_t CLSID_AAFSequence;
extern "C" const aafClassID_t CLSID_AAFSourceClip;
extern "C" const aafClassID_t CLSID_AAFSourceMob;
extern "C" const aafClassID_t CLSID_AAFSourceReference;
extern "C" const aafClassID_t CLSID_AAFStringAttribute;
extern "C" const aafClassID_t CLSID_AAFTimecode;
extern "C" const aafClassID_t CLSID_AAFTimecodeStream12M;
extern "C" const aafClassID_t CLSID_AAFTimecodeStream;
extern "C" const aafClassID_t CLSID_AAFTimelineMobSlot;
extern "C" const aafClassID_t CLSID_AAFTransition;
extern "C" const aafClassID_t CLSID_AAFTypeDef;
extern "C" const aafClassID_t CLSID_AAFUnixLocator;
extern "C" const aafClassID_t CLSID_AAFVaryingValue;
extern "C" const aafClassID_t CLSID_AAFWindowsLocator;
extern "C" const aafClassID_t CLSID_EnumAAFAttributes;
extern "C" const aafClassID_t CLSID_EnumAAFClassDefs;
extern "C" const aafClassID_t CLSID_EnumAAFComponents;
extern "C" const aafClassID_t CLSID_EnumAAFControlCodeDefs;
extern "C" const aafClassID_t CLSID_EnumAAFControlPoints;
extern "C" const aafClassID_t CLSID_EnumAAFDataDefs;
extern "C" const aafClassID_t CLSID_EnumAAFDefs;
extern "C" const aafClassID_t CLSID_EnumAAFEffectDefs;
extern "C" const aafClassID_t CLSID_EnumAAFIdentifications;
extern "C" const aafClassID_t CLSID_EnumAAFLocators;
extern "C" const aafClassID_t CLSID_EnumAAFMedia;
extern "C" const aafClassID_t CLSID_EnumAAFMobComments;
extern "C" const aafClassID_t CLSID_EnumAAFMobSlots;
extern "C" const aafClassID_t CLSID_EnumAAFMobs;
extern "C" const aafClassID_t CLSID_EnumAAFParameterSlots;
extern "C" const aafClassID_t CLSID_EnumAAFPropertyDefs;
extern "C" const aafClassID_t CLSID_EnumAAFProperties;
extern "C" const aafClassID_t CLSID_EnumAAFReferenceValues;
extern "C" const aafClassID_t CLSID_EnumAAFSegments;
extern "C" const aafClassID_t CLSID_EnumAAFTypeDefs;

// Utility function for registering a given class id as legal in a
// given file.This function hides the type "aafClassID_t" from the OM.
//
static void registerClass(OMFile* file, const aafClassID_t& classId)
{
  file->classFactory()->add((const OMClassId&)(classId),
                            createObject);
}

void registerPredefinedClasses(OMFile* file)
{
  registerClass(file, CLSID_AAFAttribute);
  registerClass(file, CLSID_AAFClassDef);
  registerClass(file, CLSID_AAFComponent);
  registerClass(file, CLSID_AAFCompositionMob);
  registerClass(file, CLSID_AAFConstValue);
  registerClass(file, CLSID_AAFContentStorage);
  registerClass(file, CLSID_AAFControlCodeDef);
  registerClass(file, CLSID_AAFControlPoint);
  registerClass(file, CLSID_AAFDataDef);
  registerClass(file, CLSID_AAFDefObject);
  registerClass(file, CLSID_AAFDictionary);
  registerClass(file, CLSID_AAFEdgecode);
  registerClass(file, CLSID_AAFEffectDef);
  registerClass(file, CLSID_AAFEffectInvocation);
  registerClass(file, CLSID_AAFEssenceDescriptor);
  registerClass(file, CLSID_AAFFile);
  registerClass(file, CLSID_AAFFileDescriptor);
  registerClass(file, CLSID_AAFFiller);
  registerClass(file, CLSID_AAFFindSourceInfo);
  registerClass(file, CLSID_AAFHeader);
  registerClass(file, CLSID_AAFIdentification);
  registerClass(file, CLSID_AAFIntegerAttribute);
  registerClass(file, CLSID_AAFLocator);
  registerClass(file, CLSID_AAFMacLocator);
  registerClass(file, CLSID_AAFMasterMob);
  registerClass(file, CLSID_AAFMedia);
  registerClass(file, CLSID_AAFMediaFilmDescriptor);
  registerClass(file, CLSID_AAFMediaGroup);
  registerClass(file, CLSID_AAFTapeDescriptor);
  registerClass(file, CLSID_AAFMob);
  registerClass(file, CLSID_AAFMobSlot);
  registerClass(file, CLSID_AAFNestedScope);
  registerClass(file, CLSID_AAFNetworkLocator);
  registerClass(file, CLSID_AAFObject);
  registerClass(file, CLSID_AAFObjectAttribute);
  registerClass(file, CLSID_AAFParameter);
  registerClass(file, CLSID_AAFParameterSlot);
  registerClass(file, CLSID_AAFPropertyDef);
  registerClass(file, CLSID_AAFPulldown);
  registerClass(file, CLSID_AAFReferenceValue);
  registerClass(file, CLSID_AAFRoot);
  registerClass(file, CLSID_AAFScopeReference);
  registerClass(file, CLSID_AAFSegment);
  registerClass(file, CLSID_AAFSequence);
  registerClass(file, CLSID_AAFSourceClip);
  registerClass(file, CLSID_AAFSourceMob);
  registerClass(file, CLSID_AAFSourceReference);
  registerClass(file, CLSID_AAFStringAttribute);
  registerClass(file, CLSID_AAFTimecode);
  registerClass(file, CLSID_AAFTimecodeStream);
  registerClass(file, CLSID_AAFTimecodeStream12M);
  registerClass(file, CLSID_AAFTimelineMobSlot);
  registerClass(file, CLSID_AAFTransition);
  registerClass(file, CLSID_AAFTypeDef);
  registerClass(file, CLSID_AAFUnixLocator);
  registerClass(file, CLSID_AAFVaryingValue);
  registerClass(file, CLSID_AAFWindowsLocator);
  registerClass(file, CLSID_EnumAAFAttributes);
  registerClass(file, CLSID_EnumAAFClassDefs);
  registerClass(file, CLSID_EnumAAFComponents);
  registerClass(file, CLSID_EnumAAFControlCodeDefs);
  registerClass(file, CLSID_EnumAAFControlPoints);
  registerClass(file, CLSID_EnumAAFDataDefs);
  registerClass(file, CLSID_EnumAAFDefs);
  registerClass(file, CLSID_EnumAAFEffectDefs);
  registerClass(file, CLSID_EnumAAFIdentifications);
  registerClass(file, CLSID_EnumAAFLocators);
  registerClass(file, CLSID_EnumAAFMedia);
  registerClass(file, CLSID_EnumAAFMobComments);
  registerClass(file, CLSID_EnumAAFMobSlots);
  registerClass(file, CLSID_EnumAAFMobs);
  registerClass(file, CLSID_EnumAAFParameterSlots);
  registerClass(file, CLSID_EnumAAFPropertyDefs);
  registerClass(file, CLSID_EnumAAFProperties);
  registerClass(file, CLSID_EnumAAFReferenceValues);
  registerClass(file, CLSID_EnumAAFSegments);
  registerClass(file, CLSID_EnumAAFTypeDefs);
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



/******************************************\
*                                          *
* File: AAFStoredObjectIDs.h               *
*                                          *
* Purpose: Definitions for controlling     * 
*          AUID initialization             *
*                                          *
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/
#ifndef __AAFStoredObjectIDs_h__

#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif

// Define the AAF_CLASS macro to produce the const stored object AUID's
#ifndef INIT_AUID 
#define AAF_CLASS(name, dataid, parent) extern "C" aafUID_t AUID_AAF##name;
#else
#define AAF_CLASS(name, dataid, parent) extern "C" aafUID_t AUID_AAF##name = dataid;
#endif
// The AAF reference implementation is still not quite in sync with SMPTE
// so we have to alias some of the SMPTE names to their corresponding
// name in AAF.
//
#define AUID_AAFClassDefinition AUID_AAFClassDef
#define AUID_AAFConstantValue AUID_AAFConstValue
#define AUID_AAFDataDefinition AUID_AAFDataDef
#define AUID_AAFDefinitionObject AUID_AAFDefObject
#define AUID_AAFEdgeCode AUID_AAFEdgecode
#define AUID_AAFEffectDefinition AUID_AAFEffectDef
#define AUID_AAFEssenceGroup AUID_AAFMediaGroup
#define AUID_AAFFilmDescriptor AUID_AAFMediaFilmDescriptor
#define AUID_AAFInterchangeObject AUID_AAFObject
#define AUID_AAFJPEGImageData AUID_AAFJPEGData
#define AUID_AAFMIDIFileData AUID_AAFMIDIData
#define AUID_AAFMIDIFileDescriptor AUID_AAFMIDIDescriptor
#define AUID_AAFParameterDefinition AUID_AAFParameterDef
#define AUID_AAFPropertyDefinition AUID_AAFPropertyDef
#define AUID_AAFTypeDefinition AUID_AAFTypeDef
#define AUID_AAFUNIXLocator AUID_AAFUnixLocator
#define AUID_AAFCodecDefinition AUID_AAFCodecDef
#define AUID_AAFContainerDefinition AUID_AAFContainerDef
#define AUID_AAFPluggableDefinition AUID_AAFPluggableDef


// Include the inside-out macro files that invokes the AAF_CLASS macros.
#include "AAFMetaDictionary.h"

// cleanup any defined macros.
#undef AAF_TABLE_BEGIN
#undef AAF_TABLE_END
#undef AAF_CLASS
#undef AAF_CLASS_END
#undef AAF_CLASS_SEPARATOR
#undef AAF_PROPERTY


#endif // __AAFStoredObjectIDs_h__

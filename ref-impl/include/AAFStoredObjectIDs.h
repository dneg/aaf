/***********************************************\
*                                               *
* File: AAFStoredObjectIDs.h                    *
*                                               *
* Purpose: Definitions for controlling          *
*          AUID initialization                  *
*                                               *
*                                               *
* Advanced Authoring Format                     *
*                                               *
* Copyright (c) 1998-1999 Avid Technology, Inc. *
*                                               *
\***********************************************/
#ifndef __AAFStoredObjectIDs_h__

#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif

// Define the AAF_CLASS macro to produce the const stored object AUID's
#ifndef INIT_AUID 
#define AAF_CLASS(name, dataid, parent, concrete) extern "C" aafUID_t AUID_AAF##name;
#else
#define AAF_CLASS(name, dataid, parent, concrete) extern "C" aafUID_t AUID_AAF##name = dataid;
#endif
// The AAF reference implementation is still not quite in sync with SMPTE
// so we have to alias some of the SMPTE names to their corresponding
// name in AAF.
//
#define AUID_AAFClassDefinition AUID_AAFClassDef
#define AUID_AAFDataDefinition AUID_AAFDataDef
#define AUID_AAFDefinitionObject AUID_AAFDefObject
#define AUID_AAFEdgeCode AUID_AAFEdgecode
#define AUID_AAFOperationDefinition AUID_AAFOperationDef
#define AUID_AAFInterchangeObject AUID_AAFObject
#define AUID_AAFJPEGImageData AUID_AAFJPEGData
#define AUID_AAFMIDIFileData AUID_AAFMIDIData
#define AUID_AAFParameterDefinition AUID_AAFParameterDef
#define AUID_AAFPropertyDefinition AUID_AAFPropertyDef
#define AUID_AAFTypeDefinition AUID_AAFTypeDef
#define AUID_AAFTypeDefinitionEnumeration AUID_AAFTypeDefEnum
#define AUID_AAFTypeDefinitionExtendibleEnumeration AUID_AAFTypeDefExtEnum
#define AUID_AAFTypeDefinitionFixedArray AUID_AAFTypeDefFixedArray
#define AUID_AAFTypeDefinitionInteger AUID_AAFTypeDefInt
#define AUID_AAFTypeDefinitionRecord AUID_AAFTypeDefRecord
#define AUID_AAFTypeDefinitionRename AUID_AAFTypeDefRename
#define AUID_AAFTypeDefinitionSet AUID_AAFTypeDefSet
#define AUID_AAFTypeDefinitionStream AUID_AAFTypeDefStream
#define AUID_AAFTypeDefinitionString AUID_AAFTypeDefString
#define AUID_AAFTypeDefinitionStrongObjectReference AUID_AAFTypeDefStrongObjRef
#define AUID_AAFTypeDefinitionVariableArray AUID_AAFTypeDefVariableArray
#define AUID_AAFTypeDefinitionWeakObjectReference AUID_AAFTypeDefWeakObjRef
#define AUID_AAFUNIXLocator AUID_AAFUnixLocator
#define AUID_AAFCodecDefinition AUID_AAFCodecDef
#define AUID_AAFContainerDefinition AUID_AAFContainerDef
#define AUID_AAFPluggableDefinition AUID_AAFPluggableDef

// Include the inside-out macro files that invokes the AAF_CLASS macros.
#include "AAFMetaDictionary.h"

#endif // __AAFStoredObjectIDs_h__

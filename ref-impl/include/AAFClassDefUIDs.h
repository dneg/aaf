#ifndef __AAFClassDefUIDs_h__
#define __AAFClassDefUIDs_h__
/***********************************************\
*                                               *
* Advanced Authoring Format                     *
*                                               *
* Copyright (c) 1998-1999 Avid Technology, Inc. *
*                                               *
\***********************************************/

//
// GUIDs for built-in class definitions
//

// Define the AAF_CLASS macro to create class GUID definitions.
// Include the meta dictionary.
//
#define AAF_CLASS(name, id, parent, concrete) \
const aafUID_t kAAFClassID_##name = \
id;

// The AAF reference implementation is still not quite in sync with SMPTE
// so we have to alias some of the SMPTE names to their corresponding
// name in AAF.
//
#define kAAFClassID_ClassDefinition kAAFClassID_ClassDef
#define kAAFClassID_DataDefinition kAAFClassID_DataDef
#define kAAFClassID_DefinitionObject kAAFClassID_DefObject
#define kAAFClassID_EdgeCode kAAFClassID_Edgecode
#define kAAFClassID_OperationDefinition kAAFClassID_OperationDef
#define kAAFClassID_InterchangeObject kAAFClassID_Object
#define kAAFClassID_JPEGImageData kAAFClassID_JPEGData
#define kAAFClassID_MIDIFileData kAAFClassID_MIDIData
#define kAAFClassID_ParameterDefinition kAAFClassID_ParameterDef
#define kAAFClassID_PropertyDefinition kAAFClassID_PropertyDef
#define kAAFClassID_TypeDefinition kAAFClassID_TypeDef
#define kAAFClassID_TypeDefinitionEnumeration kAAFClassID_TypeDefEnum
#define kAAFClassID_TypeDefinitionExtendibleEnumeration kAAFClassID_TypeDefExtEnum
#define kAAFClassID_TypeDefinitionFixedArray kAAFClassID_TypeDefFixedArray
#define kAAFClassID_TypeDefinitionInteger kAAFClassID_TypeDefInt
#define kAAFClassID_TypeDefinitionRecord kAAFClassID_TypeDefRecord
#define kAAFClassID_TypeDefinitionRename kAAFClassID_TypeDefRename
#define kAAFClassID_TypeDefinitionSet kAAFClassID_TypeDefSet
#define kAAFClassID_TypeDefinitionStream kAAFClassID_TypeDefStream
#define kAAFClassID_TypeDefinitionString kAAFClassID_TypeDefString
#define kAAFClassID_TypeDefinitionStrongObjectReference kAAFClassID_TypeDefStrongObjRef
#define kAAFClassID_TypeDefinitionVariableArray kAAFClassID_TypeDefVariableArray
#define kAAFClassID_TypeDefinitionWeakObjectReference kAAFClassID_TypeDefWeakObjRef
#define kAAFClassID_UNIXLocator kAAFClassID_UnixLocator
#define kAAFClassID_CodecDefinition kAAFClassID_CodecDef
#define kAAFClassID_ContainerDefinition kAAFClassID_ContainerDef
#define kAAFClassID_PluggableDefinition kAAFClassID_PluggableDef

#include "AAFMetaDictionary.h"

#endif // ! __AAFClassDefUIDs_h__

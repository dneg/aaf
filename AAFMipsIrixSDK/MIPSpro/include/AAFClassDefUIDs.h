#ifndef __AAFClassDefUIDs_h__
#define __AAFClassDefUIDs_h__
//=---------------------------------------------------------------------=
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
// 
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
// 
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

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
#define kAAFClassID_TypeDefinitionCharacter kAAFClassID_TypeDefCharacter
#define kAAFClassID_TypeDefinitionEnumeration kAAFClassID_TypeDefEnum
#define kAAFClassID_TypeDefinitionExtendibleEnumeration kAAFClassID_TypeDefExtEnum
#define kAAFClassID_TypeDefinitionFixedArray kAAFClassID_TypeDefFixedArray
#define kAAFClassID_TypeDefinitionInteger kAAFClassID_TypeDefInt
#define kAAFClassID_TypeDefinitionRecord kAAFClassID_TypeDefRecord
#define kAAFClassID_TypeDefinitionRename kAAFClassID_TypeDefRename
#define kAAFClassID_TypeDefinitionSet kAAFClassID_TypeDefSet
#define kAAFClassID_TypeDefinitionStream kAAFClassID_TypeDefStream
#define kAAFClassID_TypeDefinitionString kAAFClassID_TypeDefString
#define kAAFClassID_TypeDefinitionIndirect kAAFClassID_TypeDefIndirect
#define kAAFClassID_TypeDefinitionOpaque kAAFClassID_TypeDefOpaque
#define kAAFClassID_TypeDefinitionStrongObjectReference kAAFClassID_TypeDefStrongObjRef
#define kAAFClassID_TypeDefinitionVariableArray kAAFClassID_TypeDefVariableArray
#define kAAFClassID_TypeDefinitionWeakObjectReference kAAFClassID_TypeDefWeakObjRef
#define kAAFClassID_UNIXLocator kAAFClassID_UnixLocator
#define kAAFClassID_CodecDefinition kAAFClassID_CodecDef
#define kAAFClassID_ContainerDefinition kAAFClassID_ContainerDef
#define kAAFClassID_PluginDefinition kAAFClassID_PluginDef
#define kAAFClassID_PluggableDefinition kAAFClassID_PluggableDef

#include "AAFMetaDictionary.h"

#endif // ! __AAFClassDefUIDs_h__

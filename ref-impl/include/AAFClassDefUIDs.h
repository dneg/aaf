#ifndef __AAFClassDefUIDs_h__
#define __AAFClassDefUIDs_h__
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
#define kAAFClassID_TypeDefinitionStrongObjectReference kAAFClassID_TypeDefStrongObjRef
#define kAAFClassID_TypeDefinitionVariableArray kAAFClassID_TypeDefVariableArray
#define kAAFClassID_TypeDefinitionWeakObjectReference kAAFClassID_TypeDefWeakObjRef
#define kAAFClassID_UNIXLocator kAAFClassID_UnixLocator
#define kAAFClassID_CodecDefinition kAAFClassID_CodecDef
#define kAAFClassID_ContainerDefinition kAAFClassID_ContainerDef
#define kAAFClassID_PluggableDefinition kAAFClassID_PluggableDef

#include "AAFMetaDictionary.h"

#endif // ! __AAFClassDefUIDs_h__

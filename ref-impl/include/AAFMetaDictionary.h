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
* prior written permission of Avid Technology, Inc.
*
* THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
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
// Meta definition of the AAF object model.
//
// This file contains invocations of the macros described below.
//
// To use this file - 
//   1) #define those macros required to suit your usage
//   2) #include this file
//
// This file will provide empty definitions for those macros you
// choose not to define.
//
// After invoking the macros, this file will also undefine them.
// All macros will be undefined - those you chose to define
// and those defined by this file.
//
// This is an automatically generated file.
//
// If you edit this file your changes will be lost
// the next time this file is generated.
//
// The following source files were used as input : 
//
// DictP18-277-1847BMaster.csv 
//
// This file was generated on Mon Apr 10 17:02:47 EDT 2000
// by user transdel on system TRANSDEL.
//
// Key to macros.
//
//   For formal arguments marked with [*] the actual arguments
//   supplied by the macro invocations in this file are
//   themselves macro invocations.
//
// AAF_TABLE_BEGIN()
//
//   Begin a table of AAF class and property definitions.
//
// AAF_TABLE_END()
//
//   End a table of AAF class and property definitions.
//
// AAF_CLASS(name, id, parent, concrete)
//
//   Define an AAF class.
//
//     name      = the name of the class
//     id        = the auid used to identify the class [*]
//     parent    = the immediate ancestor class
//     concrete  = true if the class is concrete
//                 false if the class is abstract
//
// AAF_CLASS_END(name)
//
//   End an AAF class definition.
//
//     name      = the name of the class
//
// AAF_CLASS_SEPARATOR()
//
//   Separate one AAF class definition from another.
//
// AAF_PROPERTY(name, id, tag, type, mandatory, uid, container)
//
//   Define an AAF property.
//
//     name      = the name of the property
//     id        = the auid used to identify the property [*]
//     tag       = the short form of the id
//     type      = the type of the property [*]
//     mandatory = true if the property is mandatory
//                 false if the property is optional
//     uid       = true if the property is the unique identifier for this class
//                 false if the property is not the unique identifier for this class
//     container = the class that defines this property
//
// AAF_TYPE(type)
//
//   Define an AAF type.
//
//     type      = the type
//
// AAF_REFERENCE_TYPE(type, target)
//
//   Define an AAF reference type.
//
//     type      = the reference type
//     target    = the class of the target of the reference
//
// AAF_REFERENCE_TYPE_NAME(type, target)
//
//   Define the name of an AAF reference type.
//
//     type      = the reference type
//     target    = the class of the target of the reference
//
// AAF_TYPE_TABLE_BEGIN()
//
//   Begin a table of AAF type definitions.
//
// AAF_TYPE_TABLE_END()
//
//   End a table of AAF type definitions.
//
// AAF_TYPE_DEFINITION_INTEGER(name, id, size, signed)
//
//   Define an AAF integral type.
//
//     name      = the name of the type
//     id        = the auid used to identify the type [*]
//     size      = the size (in bytes) of the type
//     signed    = true if the type is signed, false otherwise
//
// AAF_TYPE_DEFINITION_ENUMERATION(name, id, type)
//
//   Define an AAF enumerated type.
//
//     name      = the name of the type
//     id        = the auid used to identify the type [*]
//     type      = the type of each enumeration element [*]
//
// AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(name, value, container)
//
//   Define a member of an AAF enumerated type.
//
//     name      = the name of the type
//     value     = the value of this enumeration member
//     container = the name of the containing enumerated type
//
// AAF_TYPE_DEFINITION_ENUMERATION_END(name)
//
//   End an AAF enumerated type definition.
//
//     name      = the name of the type
//
// AAF_TYPE_DEFINITION_RECORD(name, id)
//
//   Define an AAF record type.
//
//     name      = the name of the type
//     id        = the auid used to identify the type [*]
//
// AAF_TYPE_DEFINITION_RECORD_FIELD(name, type, parent)
//
//   Define a field of an AAF record type.
//
//     name      = the name of the field
//     type      = the type of the field [*]
//     container = the name of the containing record type
//
// AAF_TYPE_DEFINITION_RECORD_END(name)
//
//   End an AAF record type definition.
//
//     name      = the name of the type
//
// AAF_TYPE_DEFINITION_VARYING_ARRAY(name, id, type)
//
//   Define an AAF varying array type.
//
//     name      = the name of the type
//     id        = the auid used to identify the type [*]
//     type      = the element type [*]
//
// AAF_TYPE_DEFINITION_FIXED_ARRAY(name, id, type, count)
//
//   Define an AAF fixed array type.
//
//     name      = the name of the type
//     id        = the auid used to identify the type [*]
//     type      = the element type [*]
//     count     = the number of elements
//
// AAF_TYPE_DEFINITION_RENAME(name, id, type)
//
//   Define an AAF renamed type.
//   Define "name" as a new name for "type".
//
//     name      = the name of the type
//     id        = the auid used to identify the type [*]
//     type      = the type [*]
//
// AAF_TYPE_DEFINITION_STRING(name, id, type)
//
//   Define an AAF string type.
//
//     name      = the name of the type
//     id        = the auid used to identify the type [*]
//     type      = the element type [*]
//
// AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION(name, id)
//
//   Define an AAF extendible enumerated type.
//
//     name      = the name of the type
//     id        = the auid used to identify the type [*]
//
// AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_MEMBER(name, auid, container)
//
//   Define a member of an AAF extendible enumerated type.
//
//     name      = the name of the type
//     auid      = the auid of this enumeration member [*]
//     container = the name of the containing extendible enumerated type
//
// AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_END(name)
//
//   End an AAF extendible enumerated type definition.
//
//     name      = the name of the type
//
//
// AAF_TYPE_DEFINITION_CHARACTER(name, id)
//
//   Define an AAF character type.
//
//     name      = the name of the type
//     id        = the auid used to identify the type [*]
//
// AAF_TYPE_DEFINITION_INDIRECT(name, id)
//
//   Define an AAF indirect type.
//
//     name      = the name of the type
//     id        = the auid used to identify the type [*]
//
// AAF_TYPE_DEFINITION_OPAQUE(name, id)
//
//   Define an AAF opque type.
//
//     name      = the name of the type
//     id        = the auid used to identify the type [*]
//
// AAF_TYPE_DEFINITION_SET(name, id, type)
//
//   Define an AAF set type.
//
//     name      = the name of the type
//     id        = the auid used to identify the type [*]
//     type      = the element type [*]
//
//AAF_TYPE_DEFINITION_STRONG_REFERENCE(name, id, type)
//
//   Define an AAF strong reference type.
//
//     name      = the name of the type
//     id        = the auid used to identify the type [*]
//     type      = the target type [*]
//
// AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(name, id, type)
//
//   Define an AAF strong reference set type.
//
//     name      = the name of the type [*]
//     id        = the auid used to identify the type [*]
//     type      = the target type [*]
//
// AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR(name, id, type)
//
//   Define an AAF strong reference vector type.
//
//     name      = the name of the type [*]
//     id        = the auid used to identify the type [*]
//     type      = the target type [*]
//
//AAF_TYPE_DEFINITION_WEAK_REFERENCE(name, id, type)
//
//   Define an AAF weak reference type.
//
//     name      = the name of the type
//     id        = the auid used to identify the type [*]
//     type      = the target type [*]
//
// AAF_TYPE_DEFINITION_WEAK_REFERENCE_SET(name, id, type)
//
//   Define an AAF weak reference set type.
//
//     name      = the name of the type [*]
//     id        = the auid used to identify the type [*]
//     type      = the target type [*]
//
// AAF_TYPE_DEFINITION_WEAK_REFERENCE_VECTOR(name, id, type)
//
//   Define an AAF weak reference vector type.
//
//     name      = the name of the type [*]
//     id        = the auid used to identify the type [*]
//     type      = the target type [*]
//
//AAF_TYPE_DEFINITION_STREAM(name, id)
//
//   Define an AAF stream type.
//
//     name      = the name of the type
//     id        = the auid used to identify the type [*]
//
// AAF_TYPE_SEPARATOR()
//
//   Separate one AAF type definition from another.
//
//

// Default empty definitions so that you only have to define
// those macros you actually want to use.
//
#ifndef AAF_TABLE_BEGIN
#define AAF_TABLE_BEGIN()
#endif

#ifndef AAF_TABLE_END
#define AAF_TABLE_END()
#endif

#ifndef AAF_CLASS
#define AAF_CLASS(name, id, parent, concrete)
#endif

#ifndef AAF_CLASS_END
#define AAF_CLASS_END(name)
#endif

#ifndef AAF_CLASS_SEPARATOR
#define AAF_CLASS_SEPARATOR()
#endif

#ifndef AAF_PROPERTY
#define AAF_PROPERTY(name, id, tag, type, mandatory, uid, container)
#endif

#ifndef AAF_TYPE
#define AAF_TYPE(type)
#endif

#ifndef AAF_REFERENCE_TYPE
#define AAF_REFERENCE_TYPE(type, target)
#endif

#ifndef AAF_REFERENCE_TYPE_NAME
#define AAF_REFERENCE_TYPE_NAME(type, target)
#endif

#ifndef AAF_TYPE_TABLE_BEGIN
#define AAF_TYPE_TABLE_BEGIN()
#endif

#ifndef AAF_TYPE_TABLE_END
#define AAF_TYPE_TABLE_END()
#endif

#ifndef AAF_TYPE_DEFINITION_INTEGER
#define AAF_TYPE_DEFINITION_INTEGER(name, id, size, signed)
#endif

#ifndef AAF_TYPE_DEFINITION_ENUMERATION
#define AAF_TYPE_DEFINITION_ENUMERATION(name, id, type)
#endif

#ifndef AAF_TYPE_DEFINITION_ENUMERATION_MEMBER
#define AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(name, value, container)
#endif

#ifndef AAF_TYPE_DEFINITION_ENUMERATION_END
#define AAF_TYPE_DEFINITION_ENUMERATION_END(name)
#endif

#ifndef AAF_TYPE_DEFINITION_RECORD
#define AAF_TYPE_DEFINITION_RECORD(name, id)
#endif

#ifndef AAF_TYPE_DEFINITION_RECORD_FIELD
#define AAF_TYPE_DEFINITION_RECORD_FIELD(name, type, parent)
#endif

#ifndef AAF_TYPE_DEFINITION_RECORD_END
#define AAF_TYPE_DEFINITION_RECORD_END(name)
#endif

#ifndef AAF_TYPE_DEFINITION_VARYING_ARRAY
#define AAF_TYPE_DEFINITION_VARYING_ARRAY(name, id, type)
#endif

#ifndef AAF_TYPE_DEFINITION_FIXED_ARRAY
#define AAF_TYPE_DEFINITION_FIXED_ARRAY(name, id, type, count)
#endif

#ifndef AAF_TYPE_DEFINITION_RENAME
#define AAF_TYPE_DEFINITION_RENAME(name, id, type)
#endif

#ifndef AAF_TYPE_DEFINITION_STRING
#define AAF_TYPE_DEFINITION_STRING(name, id, type)
#endif

#ifndef AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION
#define AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION(name, id)
#endif

#ifndef AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_MEMBER
#define AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_MEMBER(name, guid, container)
#endif

#ifndef AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_END
#define AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_END(name)
#endif

#ifndef AAF_TYPE_DEFINITION_CHARACTER
#define AAF_TYPE_DEFINITION_CHARACTER(name, id)
#endif

#ifndef AAF_TYPE_DEFINITION_INDIRECT
#define AAF_TYPE_DEFINITION_INDIRECT(name, id)
#endif

#ifndef AAF_TYPE_DEFINITION_OPAQUE
#define AAF_TYPE_DEFINITION_OPAQUE(name, id)
#endif

#ifndef AAF_TYPE_DEFINITION_SET
#define AAF_TYPE_DEFINITION_SET(name, id, type)
#endif

#ifndef AAF_TYPE_DEFINITION_STRONG_REFERENCE
#define AAF_TYPE_DEFINITION_STRONG_REFERENCE(name, id, type)
#endif

#ifndef AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET
#define AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(name, id, type)
#endif

#ifndef AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR
#define AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR(name, id, type)
#endif

#ifndef AAF_TYPE_DEFINITION_WEAK_REFERENCE
#define AAF_TYPE_DEFINITION_WEAK_REFERENCE(name, id, type)
#endif

#ifndef AAF_TYPE_DEFINITION_WEAK_REFERENCE_SET
#define AAF_TYPE_DEFINITION_WEAK_REFERENCE_SET(name, id, type)
#endif

#ifndef AAF_TYPE_DEFINITION_WEAK_REFERENCE_VECTOR
#define AAF_TYPE_DEFINITION_WEAK_REFERENCE_VECTOR(name, id, type)
#endif

#ifndef AAF_TYPE_DEFINITION_STREAM
#define AAF_TYPE_DEFINITION_STREAM(name, id)
#endif

#ifndef AAF_TYPE_SEPARATOR
#define AAF_TYPE_SEPARATOR()
#endif

#ifndef AAF_LITERAL_AUID
#define AAF_LITERAL_AUID(l, w1, w2,  b1, b2, b3, b4, b5, b6, b7, b8) \
                        {l, w1, w2, {b1, b2, b3, b4, b5, b6, b7, b8}}
#endif

// AAF Object model
//

AAF_TABLE_BEGIN()

// InterchangeObject
//
AAF_CLASS(InterchangeObject,
  AAF_LITERAL_AUID(0x06000000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  Root,
  false)
  AAF_PROPERTY(ObjClass,
    AAF_LITERAL_AUID(0x06010000,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x0101,
    AAF_REFERENCE_TYPE(WeakReference, ClassDefinition),
    true,
    false,
    InterchangeObject)
  AAF_PROPERTY(Generation,
    AAF_LITERAL_AUID(0x06020000,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x0102,
    AAF_TYPE(AUID),
    false,
    false,
    InterchangeObject)
AAF_CLASS_END(InterchangeObject)
AAF_CLASS_SEPARATOR()

// Component
//
AAF_CLASS(Component,
  AAF_LITERAL_AUID(0x06410000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  InterchangeObject,
  false)
  AAF_PROPERTY(DataDefinition,
    AAF_LITERAL_AUID(0x06410100,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x0201,
    AAF_REFERENCE_TYPE(WeakReference, DataDefinition),
    true,
    false,
    Component)
  AAF_PROPERTY(Length,
    AAF_LITERAL_AUID(0x06410200,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x0202,
    AAF_TYPE(Length),
    false,
    false,
    Component)
  AAF_PROPERTY(KLVData,
    AAF_LITERAL_AUID(0x06410300,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x0203,
    AAF_REFERENCE_TYPE(StrongReferenceVector, KLVData),
    false,
    false,
    Component)
AAF_CLASS_END(Component)
AAF_CLASS_SEPARATOR()

// Segment
//
AAF_CLASS(Segment,
  AAF_LITERAL_AUID(0x06414100,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  Component,
  false)
AAF_CLASS_END(Segment)
AAF_CLASS_SEPARATOR()

// EdgeCode
//
AAF_CLASS(EdgeCode,
  AAF_LITERAL_AUID(0x06414142,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  Segment,
  true)
  AAF_PROPERTY(Start,
    AAF_LITERAL_AUID(0x06414142,
      0x0100, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x0401,
    AAF_TYPE(Position),
    true,
    false,
    EdgeCode)
  AAF_PROPERTY(FilmKind,
    AAF_LITERAL_AUID(0x06414142,
      0x0200, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x0402,
    AAF_TYPE(FilmType),
    true,
    false,
    EdgeCode)
  AAF_PROPERTY(CodeFormat,
    AAF_LITERAL_AUID(0x06414142,
      0x0300, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x0403,
    AAF_TYPE(EdgeType),
    true,
    false,
    EdgeCode)
  AAF_PROPERTY(Header,
    AAF_LITERAL_AUID(0x06414142,
      0x0400, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x0404,
    AAF_TYPE(DataValue),
    false,
    false,
    EdgeCode)
AAF_CLASS_END(EdgeCode)
AAF_CLASS_SEPARATOR()

// EssenceGroup
//
AAF_CLASS(EssenceGroup,
  AAF_LITERAL_AUID(0x06414143,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  Segment,
  true)
  AAF_PROPERTY(Choices,
    AAF_LITERAL_AUID(0x06414143,
      0x0100, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x0501,
    AAF_REFERENCE_TYPE(StrongReferenceVector, SourceReference),
    true,
    false,
    EssenceGroup)
  AAF_PROPERTY(StillFrame,
    AAF_LITERAL_AUID(0x06414143,
      0x0200, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x0502,
    AAF_REFERENCE_TYPE(StrongReference, SourceReference),
    false,
    false,
    EssenceGroup)
AAF_CLASS_END(EssenceGroup)
AAF_CLASS_SEPARATOR()

// Event
//
AAF_CLASS(Event,
  AAF_LITERAL_AUID(0x06414144,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  Segment,
  false)
  AAF_PROPERTY(Position,
    AAF_LITERAL_AUID(0x06414144,
      0x0100, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x0601,
    AAF_TYPE(Position),
    true,
    false,
    Event)
  AAF_PROPERTY(Comment,
    AAF_LITERAL_AUID(0x06414144,
      0x0200, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x0602,
    AAF_TYPE(String),
    false,
    false,
    Event)
AAF_CLASS_END(Event)
AAF_CLASS_SEPARATOR()

// GPITrigger
//
AAF_CLASS(GPITrigger,
  AAF_LITERAL_AUID(0x06414144,
    0x4200, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  Event,
  true)
  AAF_PROPERTY(ActiveState,
    AAF_LITERAL_AUID(0x06414144,
      0x4201, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x0801,
    AAF_TYPE(Boolean),
    true,
    false,
    GPITrigger)
AAF_CLASS_END(GPITrigger)
AAF_CLASS_SEPARATOR()

// CommentMarker
//
AAF_CLASS(CommentMarker,
  AAF_LITERAL_AUID(0x06414144,
    0x4300, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  Event,
  true)
  AAF_PROPERTY(Annotation,
    AAF_LITERAL_AUID(0x06414144,
      0x4301, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x0901,
    AAF_REFERENCE_TYPE(StrongReference, SourceReference),
    false,
    false,
    CommentMarker)
AAF_CLASS_END(CommentMarker)
AAF_CLASS_SEPARATOR()

// Filler
//
AAF_CLASS(Filler,
  AAF_LITERAL_AUID(0x06414145,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  Segment,
  true)
AAF_CLASS_END(Filler)
AAF_CLASS_SEPARATOR()

// OperationGroup
//
AAF_CLASS(OperationGroup,
  AAF_LITERAL_AUID(0x06414146,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  Segment,
  true)
  AAF_PROPERTY(OperationDefinition,
    AAF_LITERAL_AUID(0x06414146,
      0x0100, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x0b01,
    AAF_REFERENCE_TYPE(WeakReference, OperationDefinition),
    true,
    false,
    OperationGroup)
  AAF_PROPERTY(InputSegments,
    AAF_LITERAL_AUID(0x06414146,
      0x0200, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x0b02,
    AAF_REFERENCE_TYPE(StrongReferenceVector, Segment),
    false,
    false,
    OperationGroup)
  AAF_PROPERTY(Parameters,
    AAF_LITERAL_AUID(0x06414146,
      0x0300, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x0b03,
    AAF_REFERENCE_TYPE(StrongReferenceVector, Parameter),
    false,
    false,
    OperationGroup)
  AAF_PROPERTY(BypassOverride,
    AAF_LITERAL_AUID(0x06414146,
      0x0400, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x0b04,
    AAF_TYPE(UInt32),
    false,
    false,
    OperationGroup)
  AAF_PROPERTY(Rendering,
    AAF_LITERAL_AUID(0x06414146,
      0x0500, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x0b05,
    AAF_REFERENCE_TYPE(StrongReference, SourceReference),
    false,
    false,
    OperationGroup)
AAF_CLASS_END(OperationGroup)
AAF_CLASS_SEPARATOR()

// NestedScope
//
AAF_CLASS(NestedScope,
  AAF_LITERAL_AUID(0x06414147,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  Segment,
  true)
  AAF_PROPERTY(Slots,
    AAF_LITERAL_AUID(0x06414147,
      0x0100, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x0c01,
    AAF_REFERENCE_TYPE(StrongReferenceVector, Segment),
    true,
    false,
    NestedScope)
AAF_CLASS_END(NestedScope)
AAF_CLASS_SEPARATOR()

// Pulldown
//
AAF_CLASS(Pulldown,
  AAF_LITERAL_AUID(0x06414148,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  Segment,
  true)
  AAF_PROPERTY(InputSegment,
    AAF_LITERAL_AUID(0x06414148,
      0x0100, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x0d01,
    AAF_REFERENCE_TYPE(StrongReference, Segment),
    true,
    false,
    Pulldown)
  AAF_PROPERTY(PulldownKind,
    AAF_LITERAL_AUID(0x06414148,
      0x0200, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x0d02,
    AAF_TYPE(PulldownKindType),
    true,
    false,
    Pulldown)
  AAF_PROPERTY(PulldownDirection,
    AAF_LITERAL_AUID(0x06414148,
      0x0300, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x0d03,
    AAF_TYPE(PulldownDirectionType),
    true,
    false,
    Pulldown)
  AAF_PROPERTY(PhaseFrame,
    AAF_LITERAL_AUID(0x06414148,
      0x0400, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x0d04,
    AAF_TYPE(PhaseFrameType),
    true,
    false,
    Pulldown)
AAF_CLASS_END(Pulldown)
AAF_CLASS_SEPARATOR()

// ScopeReference
//
AAF_CLASS(ScopeReference,
  AAF_LITERAL_AUID(0x06414149,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  Segment,
  true)
  AAF_PROPERTY(RelativeScope,
    AAF_LITERAL_AUID(0x06414149,
      0x0100, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x0e01,
    AAF_TYPE(UInt32),
    true,
    false,
    ScopeReference)
  AAF_PROPERTY(RelativeSlot,
    AAF_LITERAL_AUID(0x06414149,
      0x0200, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x0e02,
    AAF_TYPE(UInt32),
    true,
    false,
    ScopeReference)
AAF_CLASS_END(ScopeReference)
AAF_CLASS_SEPARATOR()

// Selector
//
AAF_CLASS(Selector,
  AAF_LITERAL_AUID(0x0641414A,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  Segment,
  true)
  AAF_PROPERTY(Selected,
    AAF_LITERAL_AUID(0x0641414A,
      0x0100, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x0f01,
    AAF_REFERENCE_TYPE(StrongReference, Segment),
    true,
    false,
    Selector)
  AAF_PROPERTY(Alternates,
    AAF_LITERAL_AUID(0x0641414A,
      0x0200, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x0f02,
    AAF_REFERENCE_TYPE(StrongReferenceVector, Segment),
    false,
    false,
    Selector)
AAF_CLASS_END(Selector)
AAF_CLASS_SEPARATOR()

// Sequence
//
AAF_CLASS(Sequence,
  AAF_LITERAL_AUID(0x0641414B,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  Segment,
  true)
  AAF_PROPERTY(Components,
    AAF_LITERAL_AUID(0x0641414B,
      0x0100, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x1001,
    AAF_REFERENCE_TYPE(StrongReferenceVector, Component),
    true,
    false,
    Sequence)
AAF_CLASS_END(Sequence)
AAF_CLASS_SEPARATOR()

// SourceReference
//
AAF_CLASS(SourceReference,
  AAF_LITERAL_AUID(0x0641414C,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  Segment,
  false)
  AAF_PROPERTY(SourceID,
    AAF_LITERAL_AUID(0x0641414C,
      0x0100, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x1101,
    AAF_REFERENCE_TYPE(WeakReference, Mob),
    false,
    false,
    SourceReference)
  AAF_PROPERTY(SourceMobSlotID,
    AAF_LITERAL_AUID(0x0641414C,
      0x0200, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x1102,
    AAF_TYPE(UInt32),
    true,
    false,
    SourceReference)
AAF_CLASS_END(SourceReference)
AAF_CLASS_SEPARATOR()

// SourceClip
//
AAF_CLASS(SourceClip,
  AAF_LITERAL_AUID(0x0641414C,
    0x4100, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  SourceReference,
  true)
  AAF_PROPERTY(StartTime,
    AAF_LITERAL_AUID(0x0641414C,
      0x4101, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x1201,
    AAF_TYPE(Position ),
    false,
    false,
    SourceClip)
  AAF_PROPERTY(FadeInLength,
    AAF_LITERAL_AUID(0x0641414C,
      0x4102, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x1202,
    AAF_TYPE(Length),
    false,
    false,
    SourceClip)
  AAF_PROPERTY(FadeInType,
    AAF_LITERAL_AUID(0x0641414C,
      0x4103, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x1203,
    AAF_TYPE(FadeType),
    false,
    false,
    SourceClip)
  AAF_PROPERTY(FadeOutLength,
    AAF_LITERAL_AUID(0x0641414C,
      0x4104, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x1204,
    AAF_TYPE(Length),
    false,
    false,
    SourceClip)
  AAF_PROPERTY(FadeOutType,
    AAF_LITERAL_AUID(0x0641414C,
      0x4105, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x1205,
    AAF_TYPE(FadeType),
    false,
    false,
    SourceClip)
AAF_CLASS_END(SourceClip)
AAF_CLASS_SEPARATOR()

// TextClip
//
AAF_CLASS(TextClip,
  AAF_LITERAL_AUID(0x0641414C,
    0x4200, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  SourceReference,
  false)
AAF_CLASS_END(TextClip)
AAF_CLASS_SEPARATOR()

// HTMLClip
//
AAF_CLASS(HTMLClip,
  AAF_LITERAL_AUID(0x0641414C,
    0x4201, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  TextClip,
  true)
  AAF_PROPERTY(BeginAnchor,
    AAF_LITERAL_AUID(0x0641414C,
      0x4241, 0x0100,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x1401,
    AAF_TYPE(String),
    false,
    false,
    HTMLClip)
  AAF_PROPERTY(EndAnchor,
    AAF_LITERAL_AUID(0x0641414C,
      0x4241, 0x0200,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x1402,
    AAF_TYPE(String),
    false,
    false,
    HTMLClip)
AAF_CLASS_END(HTMLClip)
AAF_CLASS_SEPARATOR()

// Timecode
//
AAF_CLASS(Timecode,
  AAF_LITERAL_AUID(0x0641414D,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  Segment,
  true)
  AAF_PROPERTY(Start,
    AAF_LITERAL_AUID(0x0641414D,
      0x0100, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x1501,
    AAF_TYPE(Position),
    true,
    false,
    Timecode)
  AAF_PROPERTY(FPS,
    AAF_LITERAL_AUID(0x0641414D,
      0x0200, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x1502,
    AAF_TYPE(UInt16),
    true,
    false,
    Timecode)
  AAF_PROPERTY(Drop,
    AAF_LITERAL_AUID(0x0641414D,
      0x0300, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x1503,
    AAF_TYPE(Boolean),
    true,
    false,
    Timecode)
AAF_CLASS_END(Timecode)
AAF_CLASS_SEPARATOR()

// TimecodeStream
//
AAF_CLASS(TimecodeStream,
  AAF_LITERAL_AUID(0x0641414E,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  Segment,
  false)
  AAF_PROPERTY(SampleRate,
    AAF_LITERAL_AUID(0x0641414E,
      0x0100, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x1601,
    AAF_TYPE(Rational),
    true,
    false,
    TimecodeStream)
  AAF_PROPERTY(Source,
    AAF_LITERAL_AUID(0x0641414E,
      0x0200, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x1602,
    AAF_TYPE(DataStream),
    true,
    false,
    TimecodeStream)
  AAF_PROPERTY(SourceType,
    AAF_LITERAL_AUID(0x0641414E,
      0x0300, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x1603,
    AAF_TYPE(TCSource),
    true,
    false,
    TimecodeStream)
AAF_CLASS_END(TimecodeStream)
AAF_CLASS_SEPARATOR()

// TimecodeStream12M
//
AAF_CLASS(TimecodeStream12M,
  AAF_LITERAL_AUID(0x0641414E,
    0x4100, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  TimecodeStream,
  true)
  AAF_PROPERTY(IncludeSync,
    AAF_LITERAL_AUID(0x0641414E,
      0x4101, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x1701,
    AAF_TYPE(Boolean),
    true,
    false,
    TimecodeStream12M)
AAF_CLASS_END(TimecodeStream12M)
AAF_CLASS_SEPARATOR()

// Transition
//
AAF_CLASS(Transition,
  AAF_LITERAL_AUID(0x06414200,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  Component,
  true)
  AAF_PROPERTY(OperationGroup,
    AAF_LITERAL_AUID(0x06414201,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x1801,
    AAF_REFERENCE_TYPE(StrongReference, OperationGroup),
    true,
    false,
    Transition)
  AAF_PROPERTY(CutPoint,
    AAF_LITERAL_AUID(0x06414202,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x1802,
    AAF_TYPE(Position),
    true,
    false,
    Transition)
AAF_CLASS_END(Transition)
AAF_CLASS_SEPARATOR()

// ContentStorage
//
AAF_CLASS(ContentStorage,
  AAF_LITERAL_AUID(0x06420000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  InterchangeObject,
  true)
  AAF_PROPERTY(Mobs,
    AAF_LITERAL_AUID(0x06420100,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x1901,
    AAF_REFERENCE_TYPE(StrongReferenceSet, Mob),
    true,
    false,
    ContentStorage)
  AAF_PROPERTY(EssenceData,
    AAF_LITERAL_AUID(0x06420200,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x1902,
    AAF_REFERENCE_TYPE(StrongReferenceSet, EssenceData),
    false,
    false,
    ContentStorage)
AAF_CLASS_END(ContentStorage)
AAF_CLASS_SEPARATOR()

// ControlPoint
//
AAF_CLASS(ControlPoint,
  AAF_LITERAL_AUID(0x06430000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  InterchangeObject,
  true)
  AAF_PROPERTY(Value,
    AAF_LITERAL_AUID(0x06430200,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x1a02,
    AAF_TYPE(Indirect),
    true,
    false,
    ControlPoint)
  AAF_PROPERTY(Time,
    AAF_LITERAL_AUID(0x06430300,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x1a03,
    AAF_TYPE(Rational),
    true,
    false,
    ControlPoint)
  AAF_PROPERTY(EditHint,
    AAF_LITERAL_AUID(0x06430400,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x1a04,
    AAF_TYPE(EditHintType),
    false,
    false,
    ControlPoint)
AAF_CLASS_END(ControlPoint)
AAF_CLASS_SEPARATOR()

// DefinitionObject
//
AAF_CLASS(DefinitionObject,
  AAF_LITERAL_AUID(0x06440000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  InterchangeObject,
  false)
  AAF_PROPERTY(Identification,
    AAF_LITERAL_AUID(0x06440100,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x1b01,
    AAF_TYPE(AUID),
    true,
    true,
    DefinitionObject)
  AAF_PROPERTY(Name,
    AAF_LITERAL_AUID(0x06440200,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x1b02,
    AAF_TYPE(String),
    true,
    false,
    DefinitionObject)
  AAF_PROPERTY(Description,
    AAF_LITERAL_AUID(0x06440300,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x1b03,
    AAF_TYPE(String),
    false,
    false,
    DefinitionObject)
AAF_CLASS_END(DefinitionObject)
AAF_CLASS_SEPARATOR()

// ClassDefinition
//
AAF_CLASS(ClassDefinition,
  AAF_LITERAL_AUID(0x06444100,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  MetaDefinition,
  true)
  AAF_PROPERTY(ParentClass,
    AAF_LITERAL_AUID(0x06444101,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x1c01,
    AAF_REFERENCE_TYPE(WeakReference, ClassDefinition),
    true,
    false,
    ClassDefinition)
  AAF_PROPERTY(Properties,
    AAF_LITERAL_AUID(0x06444102,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x1c02,
    AAF_REFERENCE_TYPE(StrongReferenceSet, PropertyDefinition),
    false,
    false,
    ClassDefinition)
AAF_CLASS_END(ClassDefinition)
AAF_CLASS_SEPARATOR()

// DataDefinition
//
AAF_CLASS(DataDefinition,
  AAF_LITERAL_AUID(0x06444200,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  DefinitionObject,
  true)
AAF_CLASS_END(DataDefinition)
AAF_CLASS_SEPARATOR()

// OperationDefinition
//
AAF_CLASS(OperationDefinition,
  AAF_LITERAL_AUID(0x06444300,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  DefinitionObject,
  true)
  AAF_PROPERTY(DataDefinition,
    AAF_LITERAL_AUID(0x06444301,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x1e01,
    AAF_REFERENCE_TYPE(WeakReference, DataDefinition),
    true,
    false,
    OperationDefinition)
  AAF_PROPERTY(IsTimeWarp,
    AAF_LITERAL_AUID(0x06444302,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x1e02,
    AAF_TYPE(Boolean),
    false,
    false,
    OperationDefinition)
  AAF_PROPERTY(DegradeTo,
    AAF_LITERAL_AUID(0x06444303,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x1e03,
    AAF_REFERENCE_TYPE(WeakReferenceVector, OperationDefinition),
    false,
    false,
    OperationDefinition)
  AAF_PROPERTY(Category,
    AAF_LITERAL_AUID(0x06444306,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x1e06,
    AAF_TYPE(AUID),
    false,
    false,
    OperationDefinition)
  AAF_PROPERTY(NumberInputs,
    AAF_LITERAL_AUID(0x06444307,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x1e07,
    AAF_TYPE(Int32),
    true,
    false,
    OperationDefinition)
  AAF_PROPERTY(Bypass,
    AAF_LITERAL_AUID(0x06444308,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x1e08,
    AAF_TYPE(UInt32),
    false,
    false,
    OperationDefinition)
  AAF_PROPERTY(ParametersDefined,
    AAF_LITERAL_AUID(0x06444309,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x1e09,
    AAF_REFERENCE_TYPE(WeakReferenceSet, ParameterDefinition),
    false,
    false,
    OperationDefinition)
AAF_CLASS_END(OperationDefinition)
AAF_CLASS_SEPARATOR()

// ParameterDefinition
//
AAF_CLASS(ParameterDefinition,
  AAF_LITERAL_AUID(0x06444400,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  DefinitionObject,
  true)
  AAF_PROPERTY(Type,
    AAF_LITERAL_AUID(0x06444401,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x1f01,
    AAF_REFERENCE_TYPE(WeakReference, TypeDefinition),
    true,
    false,
    ParameterDefinition)
  AAF_PROPERTY(DisplayUnits,
    AAF_LITERAL_AUID(0x06444403,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x1f03,
    AAF_TYPE(String),
    false,
    false,
    ParameterDefinition)
AAF_CLASS_END(ParameterDefinition)
AAF_CLASS_SEPARATOR()

// PropertyDefinition
//
AAF_CLASS(PropertyDefinition,
  AAF_LITERAL_AUID(0x06444500,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  MetaDefinition,
  true)
  AAF_PROPERTY(Type,
    AAF_LITERAL_AUID(0x06444501,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x2001,
    AAF_REFERENCE_TYPE(WeakReference, TypeDefinition),
    true,
    false,
    PropertyDefinition)
  AAF_PROPERTY(IsOptional,
    AAF_LITERAL_AUID(0x06444505,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x2005,
    AAF_TYPE(Boolean),
    true,
    false,
    PropertyDefinition)
  AAF_PROPERTY(DefaultValue,
    AAF_LITERAL_AUID(0x06444506,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x2006,
    AAF_TYPE(Indirect),
    false,
    false,
    PropertyDefinition)
  AAF_PROPERTY(LocalIdentification,
    AAF_LITERAL_AUID(0x06444507,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x2007,
    AAF_TYPE(UInt16),
    true,
    false,
    PropertyDefinition)
AAF_CLASS_END(PropertyDefinition)
AAF_CLASS_SEPARATOR()

// TypeDefinition
//
AAF_CLASS(TypeDefinition,
  AAF_LITERAL_AUID(0x06444600,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  MetaDefinition,
  false)
AAF_CLASS_END(TypeDefinition)
AAF_CLASS_SEPARATOR()

// PluginDefinition
//
AAF_CLASS(PluginDefinition,
  AAF_LITERAL_AUID(0x06444700,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  DefinitionObject,
  true)
  AAF_PROPERTY(CategoryClass,
    AAF_LITERAL_AUID(0x06444704,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x2203,
    AAF_TYPE(AUID),
    true,
    false,
    PluginDefinition)
  AAF_PROPERTY(VersionNumber,
    AAF_LITERAL_AUID(0x06444705,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x2204,
    AAF_TYPE(VersionType),
    true,
    false,
    PluginDefinition)
  AAF_PROPERTY(VersionString,
    AAF_LITERAL_AUID(0x06444706,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x2205,
    AAF_TYPE(String),
    false,
    false,
    PluginDefinition)
  AAF_PROPERTY(Manufacturer,
    AAF_LITERAL_AUID(0x06444707,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x2206,
    AAF_TYPE(String),
    false,
    false,
    PluginDefinition)
  AAF_PROPERTY(ManufacturerInfo,
    AAF_LITERAL_AUID(0x06444708,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x2207,
    AAF_REFERENCE_TYPE(StrongReference, NetworkLocator),
    false,
    false,
    PluginDefinition)
  AAF_PROPERTY(ManufacturerID,
    AAF_LITERAL_AUID(0x06444709,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x2208,
    AAF_TYPE(AUID),
    false,
    false,
    PluginDefinition)
  AAF_PROPERTY(Platform,
    AAF_LITERAL_AUID(0x0644470A,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x2209,
    AAF_TYPE(AUID),
    false,
    false,
    PluginDefinition)
  AAF_PROPERTY(MinPlatformVersion,
    AAF_LITERAL_AUID(0x0644470B,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x220a,
    AAF_TYPE(VersionType),
    false,
    false,
    PluginDefinition)
  AAF_PROPERTY(MaxPlatformVersion,
    AAF_LITERAL_AUID(0x0644470C,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x220b,
    AAF_TYPE(VersionType),
    false,
    false,
    PluginDefinition)
  AAF_PROPERTY(Engine,
    AAF_LITERAL_AUID(0x0644470D,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x220c,
    AAF_TYPE(AUID),
    false,
    false,
    PluginDefinition)
  AAF_PROPERTY(MinEngineVersion,
    AAF_LITERAL_AUID(0x0644470E,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x220d,
    AAF_TYPE(VersionType),
    false,
    false,
    PluginDefinition)
  AAF_PROPERTY(MaxEngineVersion,
    AAF_LITERAL_AUID(0x0644470F,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x220e,
    AAF_TYPE(VersionType),
    false,
    false,
    PluginDefinition)
  AAF_PROPERTY(PluginAPI,
    AAF_LITERAL_AUID(0x06444710,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x220f,
    AAF_TYPE(AUID),
    false,
    false,
    PluginDefinition)
  AAF_PROPERTY(MinPluginAPI,
    AAF_LITERAL_AUID(0x06444711,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x2210,
    AAF_TYPE(VersionType),
    false,
    false,
    PluginDefinition)
  AAF_PROPERTY(MaxPluginAPI,
    AAF_LITERAL_AUID(0x06444712,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x2211,
    AAF_TYPE(VersionType),
    false,
    false,
    PluginDefinition)
  AAF_PROPERTY(SoftwareOnly,
    AAF_LITERAL_AUID(0x06444713,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x2212,
    AAF_TYPE(Boolean),
    false,
    false,
    PluginDefinition)
  AAF_PROPERTY(Accelerator,
    AAF_LITERAL_AUID(0x06444714,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x2213,
    AAF_TYPE(Boolean),
    false,
    false,
    PluginDefinition)
  AAF_PROPERTY(Locators,
    AAF_LITERAL_AUID(0x06444715,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x2214,
    AAF_REFERENCE_TYPE(StrongReferenceVector, Locator),
    false,
    false,
    PluginDefinition)
  AAF_PROPERTY(Authentication,
    AAF_LITERAL_AUID(0x06444716,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x2215,
    AAF_TYPE(Boolean),
    false,
    false,
    PluginDefinition)
  AAF_PROPERTY(DefinitionObject,
    AAF_LITERAL_AUID(0x06444717,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x2216,
    AAF_TYPE(AUID),
    false,
    false,
    PluginDefinition)
AAF_CLASS_END(PluginDefinition)
AAF_CLASS_SEPARATOR()

// CodecDefinition
//
AAF_CLASS(CodecDefinition,
  AAF_LITERAL_AUID(0x06444800,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  DefinitionObject,
  true)
  AAF_PROPERTY(FileDescriptorClass,
    AAF_LITERAL_AUID(0x06444801,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x2301,
    AAF_REFERENCE_TYPE(WeakReference, ClassDefinition),
    true,
    false,
    CodecDefinition)
  AAF_PROPERTY(DataDefinitions,
    AAF_LITERAL_AUID(0x06444802,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x2302,
    AAF_REFERENCE_TYPE(WeakReferenceSet, DataDefinition),
    true,
    false,
    CodecDefinition)
AAF_CLASS_END(CodecDefinition)
AAF_CLASS_SEPARATOR()

// ContainerDefinition
//
AAF_CLASS(ContainerDefinition,
  AAF_LITERAL_AUID(0x06444900,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  DefinitionObject,
  true)
  AAF_PROPERTY(EssenceIsIdentified,
    AAF_LITERAL_AUID(0x06444901,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x2401,
    AAF_TYPE(Boolean),
    false,
    false,
    ContainerDefinition)
AAF_CLASS_END(ContainerDefinition)
AAF_CLASS_SEPARATOR()

// InterpolationDefinition
//
AAF_CLASS(InterpolationDefinition,
  AAF_LITERAL_AUID(0x06444A00,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  DefinitionObject,
  true)
AAF_CLASS_END(InterpolationDefinition)
AAF_CLASS_SEPARATOR()

// Dictionary
//
AAF_CLASS(Dictionary,
  AAF_LITERAL_AUID(0x06455000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  MetaDictionary,
  true)
  AAF_PROPERTY(ClassDefinitions,
    AAF_LITERAL_AUID(0x06455001,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x2601,
    AAF_REFERENCE_TYPE(StrongReferenceSet, ClassDefinition),
    false,
    false,
    Dictionary)
  AAF_PROPERTY(TypeDefinitions,
    AAF_LITERAL_AUID(0x06455002,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x2602,
    AAF_REFERENCE_TYPE(StrongReferenceSet, TypeDefinition),
    false,
    false,
    Dictionary)
  AAF_PROPERTY(OperationDefinitions,
    AAF_LITERAL_AUID(0x06455003,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x2603,
    AAF_REFERENCE_TYPE(StrongReferenceSet, OperationDefinition),
    false,
    false,
    Dictionary)
  AAF_PROPERTY(ParameterDefinitions,
    AAF_LITERAL_AUID(0x06455004,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x2604,
    AAF_REFERENCE_TYPE(StrongReferenceSet, ParameterDefinition),
    false,
    false,
    Dictionary)
  AAF_PROPERTY(DataDefinitions,
    AAF_LITERAL_AUID(0x06455005,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x2605,
    AAF_REFERENCE_TYPE(StrongReferenceSet, DataDefinition),
    false,
    false,
    Dictionary)
  AAF_PROPERTY(PluginDefinitions,
    AAF_LITERAL_AUID(0x06455006,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x2606,
    AAF_REFERENCE_TYPE(StrongReferenceSet, PluginDefinition),
    false,
    false,
    Dictionary)
  AAF_PROPERTY(CodecDefinitions,
    AAF_LITERAL_AUID(0x06455007,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x2607,
    AAF_REFERENCE_TYPE(StrongReferenceSet, CodecDefinition),
    false,
    false,
    Dictionary)
  AAF_PROPERTY(ContainerDefinitions,
    AAF_LITERAL_AUID(0x06455008,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x2608,
    AAF_REFERENCE_TYPE(StrongReferenceSet, ContainerDefinition),
    false,
    false,
    Dictionary)
  AAF_PROPERTY(InterpolationDefinitions,
    AAF_LITERAL_AUID(0x06455009,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x2609,
    AAF_REFERENCE_TYPE(StrongReferenceSet, InterpolationDefinition),
    false,
    false,
    Dictionary)
AAF_CLASS_END(Dictionary)
AAF_CLASS_SEPARATOR()

// EssenceData
//
AAF_CLASS(EssenceData,
  AAF_LITERAL_AUID(0x06460000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  InterchangeObject,
  true)
  AAF_PROPERTY(MobID,
    AAF_LITERAL_AUID(0x06460100,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x2701,
    AAF_TYPE(MobID),
    true,
    true,
    EssenceData)
  AAF_PROPERTY(Data,
    AAF_LITERAL_AUID(0x06460200,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x2702,
    AAF_TYPE(DataStream),
    true,
    false,
    EssenceData)
  AAF_PROPERTY(SampleIndex,
    AAF_LITERAL_AUID(0x06460300,
      0x0100, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x2b01,
    AAF_TYPE(DataStream),
    false,
    false,
    EssenceData)
AAF_CLASS_END(EssenceData)
AAF_CLASS_SEPARATOR()

// EssenceDescriptor
//
AAF_CLASS(EssenceDescriptor,
  AAF_LITERAL_AUID(0x06470000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  InterchangeObject,
  false)
  AAF_PROPERTY(Locator,
    AAF_LITERAL_AUID(0x06470100,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x2f01,
    AAF_REFERENCE_TYPE(StrongReferenceVector, Locator),
    false,
    false,
    EssenceDescriptor)
AAF_CLASS_END(EssenceDescriptor)
AAF_CLASS_SEPARATOR()

// FileDescriptor
//
AAF_CLASS(FileDescriptor,
  AAF_LITERAL_AUID(0x06474100,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  EssenceDescriptor,
  false)
  AAF_PROPERTY(SampleRate,
    AAF_LITERAL_AUID(0x06474101,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3001,
    AAF_TYPE(Rational),
    true,
    false,
    FileDescriptor)
  AAF_PROPERTY(Length,
    AAF_LITERAL_AUID(0x06474102,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3002,
    AAF_TYPE(Length),
    true,
    false,
    FileDescriptor)
  AAF_PROPERTY(ContainerFormat,
    AAF_LITERAL_AUID(0x06474104,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3004,
    AAF_REFERENCE_TYPE(WeakReference, ContainerDefinition),
    false,
    false,
    FileDescriptor)
  AAF_PROPERTY(CodecDefinition,
    AAF_LITERAL_AUID(0x06474105,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3005,
    AAF_REFERENCE_TYPE(WeakReference, CodecDefinition),
    false,
    false,
    FileDescriptor)
AAF_CLASS_END(FileDescriptor)
AAF_CLASS_SEPARATOR()

// AIFCDescriptor
//
AAF_CLASS(AIFCDescriptor,
  AAF_LITERAL_AUID(0x06474141,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  FileDescriptor,
  true)
  AAF_PROPERTY(Summary,
    AAF_LITERAL_AUID(0x06474141,
      0x0100, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3101,
    AAF_TYPE(DataValue),
    true,
    false,
    AIFCDescriptor)
AAF_CLASS_END(AIFCDescriptor)
AAF_CLASS_SEPARATOR()

// DigitalImageDescriptor
//
AAF_CLASS(DigitalImageDescriptor,
  AAF_LITERAL_AUID(0x06474142,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  FileDescriptor,
  false)
  AAF_PROPERTY(Compression,
    AAF_LITERAL_AUID(0x06474142,
      0x0100, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3201,
    AAF_TYPE(AUID),
    false,
    false,
    DigitalImageDescriptor)
  AAF_PROPERTY(StoredHeight,
    AAF_LITERAL_AUID(0x06474142,
      0x0200, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3202,
    AAF_TYPE(UInt32),
    true,
    false,
    DigitalImageDescriptor)
  AAF_PROPERTY(StoredWidth,
    AAF_LITERAL_AUID(0x06474142,
      0x0300, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3203,
    AAF_TYPE(UInt32),
    true,
    false,
    DigitalImageDescriptor)
  AAF_PROPERTY(SampledHeight,
    AAF_LITERAL_AUID(0x06474142,
      0x0400, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3204,
    AAF_TYPE(UInt32),
    false,
    false,
    DigitalImageDescriptor)
  AAF_PROPERTY(SampledWidth,
    AAF_LITERAL_AUID(0x06474142,
      0x0500, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3205,
    AAF_TYPE(UInt32),
    false,
    false,
    DigitalImageDescriptor)
  AAF_PROPERTY(SampledXOffset,
    AAF_LITERAL_AUID(0x06474142,
      0x0600, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3206,
    AAF_TYPE(Int32),
    false,
    false,
    DigitalImageDescriptor)
  AAF_PROPERTY(SampledYOffset,
    AAF_LITERAL_AUID(0x06474142,
      0x0700, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3207,
    AAF_TYPE(Int32),
    false,
    false,
    DigitalImageDescriptor)
  AAF_PROPERTY(DisplayHeight,
    AAF_LITERAL_AUID(0x06474142,
      0x0800, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3208,
    AAF_TYPE(UInt32),
    false,
    false,
    DigitalImageDescriptor)
  AAF_PROPERTY(DisplayWidth,
    AAF_LITERAL_AUID(0x06474142,
      0x0900, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3209,
    AAF_TYPE(UInt32),
    false,
    false,
    DigitalImageDescriptor)
  AAF_PROPERTY(DisplayXOffset,
    AAF_LITERAL_AUID(0x06474142,
      0x0A00, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x320a,
    AAF_TYPE(Int32),
    false,
    false,
    DigitalImageDescriptor)
  AAF_PROPERTY(DisplayYOffset,
    AAF_LITERAL_AUID(0x06474142,
      0x0B00, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x320b,
    AAF_TYPE(Int32),
    false,
    false,
    DigitalImageDescriptor)
  AAF_PROPERTY(FrameLayout,
    AAF_LITERAL_AUID(0x06474142,
      0x0C00, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x320c,
    AAF_TYPE(LayoutType),
    true,
    false,
    DigitalImageDescriptor)
  AAF_PROPERTY(VideoLineMap,
    AAF_LITERAL_AUID(0x06474142,
      0x0D00, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x320d,
    AAF_TYPE(Int32Array),
    true,
    false,
    DigitalImageDescriptor)
  AAF_PROPERTY(ImageAspectRatio,
    AAF_LITERAL_AUID(0x06474142,
      0x0E00, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x320e,
    AAF_TYPE(Rational),
    true,
    false,
    DigitalImageDescriptor)
  AAF_PROPERTY(AlphaTransparency,
    AAF_LITERAL_AUID(0x06474142,
      0x0F00, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x320f,
    AAF_TYPE(AlphaTransparency),
    false,
    false,
    DigitalImageDescriptor)
  AAF_PROPERTY(Gamma,
    AAF_LITERAL_AUID(0x06474142,
      0x1000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3210,
    AAF_TYPE(AUID),
    false,
    false,
    DigitalImageDescriptor)
  AAF_PROPERTY(ImageAlignmentFactor,
    AAF_LITERAL_AUID(0x06474142,
      0x1100, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3211,
    AAF_TYPE(UInt32),
    false,
    false,
    DigitalImageDescriptor)
AAF_CLASS_END(DigitalImageDescriptor)
AAF_CLASS_SEPARATOR()

// CDCIDescriptor
//
AAF_CLASS(CDCIDescriptor,
  AAF_LITERAL_AUID(0x06474142,
    0x4100, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  DigitalImageDescriptor,
  true)
  AAF_PROPERTY(ComponentWidth,
    AAF_LITERAL_AUID(0x06474142,
      0x4101, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3301,
    AAF_TYPE(UInt32),
    true,
    false,
    CDCIDescriptor)
  AAF_PROPERTY(HorizontalSubsampling,
    AAF_LITERAL_AUID(0x06474142,
      0x4102, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3302,
    AAF_TYPE(UInt32),
    true,
    false,
    CDCIDescriptor)
  AAF_PROPERTY(ColorSiting,
    AAF_LITERAL_AUID(0x06474142,
      0x4103, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3303,
    AAF_TYPE(ColorSitingType),
    false,
    false,
    CDCIDescriptor)
  AAF_PROPERTY(BlackReferenceLevel,
    AAF_LITERAL_AUID(0x06474142,
      0x4104, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3304,
    AAF_TYPE(UInt32),
    false,
    false,
    CDCIDescriptor)
  AAF_PROPERTY(WhiteReferenceLevel,
    AAF_LITERAL_AUID(0x06474142,
      0x4105, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3305,
    AAF_TYPE(UInt32),
    false,
    false,
    CDCIDescriptor)
  AAF_PROPERTY(ColorRange,
    AAF_LITERAL_AUID(0x06474142,
      0x4106, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3306,
    AAF_TYPE(UInt32),
    false,
    false,
    CDCIDescriptor)
  AAF_PROPERTY(PaddingBits,
    AAF_LITERAL_AUID(0x06474142,
      0x4107, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3307,
    AAF_TYPE(Int16),
    false,
    false,
    CDCIDescriptor)
  AAF_PROPERTY(VerticalSubsampling,
    AAF_LITERAL_AUID(0x06474142,
      0x4108, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3308,
    AAF_TYPE(UInt32),
    false,
    false,
    CDCIDescriptor)
AAF_CLASS_END(CDCIDescriptor)
AAF_CLASS_SEPARATOR()

// RGBADescriptor
//
AAF_CLASS(RGBADescriptor,
  AAF_LITERAL_AUID(0x06474142,
    0x4200, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  DigitalImageDescriptor,
  true)
  AAF_PROPERTY(PixelLayout,
    AAF_LITERAL_AUID(0x06474142,
      0x4201, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3401,
    AAF_TYPE(RGBALayout),
    true,
    false,
    RGBADescriptor)
  AAF_PROPERTY(Palette,
    AAF_LITERAL_AUID(0x06474142,
      0x4203, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3403,
    AAF_TYPE(DataValue),
    false,
    false,
    RGBADescriptor)
  AAF_PROPERTY(PaletteLayout,
    AAF_LITERAL_AUID(0x06474142,
      0x4204, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3404,
    AAF_TYPE(RGBALayout),
    false,
    false,
    RGBADescriptor)
AAF_CLASS_END(RGBADescriptor)
AAF_CLASS_SEPARATOR()

// HTMLDescriptor
//
AAF_CLASS(HTMLDescriptor,
  AAF_LITERAL_AUID(0x06474143,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  FileDescriptor,
  true)
AAF_CLASS_END(HTMLDescriptor)
AAF_CLASS_SEPARATOR()

// TIFFDescriptor
//
AAF_CLASS(TIFFDescriptor,
  AAF_LITERAL_AUID(0x06474145,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  FileDescriptor,
  true)
  AAF_PROPERTY(IsUniform,
    AAF_LITERAL_AUID(0x06474145,
      0x0100, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3701,
    AAF_TYPE(Boolean),
    true,
    false,
    TIFFDescriptor)
  AAF_PROPERTY(IsContiguous,
    AAF_LITERAL_AUID(0x06474145,
      0x0200, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3702,
    AAF_TYPE(Boolean),
    true,
    false,
    TIFFDescriptor)
  AAF_PROPERTY(LeadingLines,
    AAF_LITERAL_AUID(0x06474145,
      0x0300, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3703,
    AAF_TYPE(Int32),
    false,
    false,
    TIFFDescriptor)
  AAF_PROPERTY(TrailingLines,
    AAF_LITERAL_AUID(0x06474145,
      0x0400, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3704,
    AAF_TYPE(Int32),
    false,
    false,
    TIFFDescriptor)
  AAF_PROPERTY(JPEGTableID,
    AAF_LITERAL_AUID(0x06474145,
      0x0500, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3705,
    AAF_TYPE(JPEGTableIDType),
    false,
    false,
    TIFFDescriptor)
  AAF_PROPERTY(Summary,
    AAF_LITERAL_AUID(0x06474145,
      0x0600, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3706,
    AAF_TYPE(DataValue),
    true,
    false,
    TIFFDescriptor)
AAF_CLASS_END(TIFFDescriptor)
AAF_CLASS_SEPARATOR()

// WAVEDescriptor
//
AAF_CLASS(WAVEDescriptor,
  AAF_LITERAL_AUID(0x06474146,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  FileDescriptor,
  true)
  AAF_PROPERTY(Summary,
    AAF_LITERAL_AUID(0x06474146,
      0x0100, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3801,
    AAF_TYPE(DataValue),
    true,
    false,
    WAVEDescriptor)
AAF_CLASS_END(WAVEDescriptor)
AAF_CLASS_SEPARATOR()

// FilmDescriptor
//
AAF_CLASS(FilmDescriptor,
  AAF_LITERAL_AUID(0x06474200,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  EssenceDescriptor,
  true)
  AAF_PROPERTY(FilmFormat,
    AAF_LITERAL_AUID(0x06474201,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3901,
    AAF_TYPE(FilmType),
    false,
    false,
    FilmDescriptor)
  AAF_PROPERTY(FrameRate,
    AAF_LITERAL_AUID(0x06474202,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3902,
    AAF_TYPE(UInt32),
    false,
    false,
    FilmDescriptor)
  AAF_PROPERTY(PerforationsPerFrame,
    AAF_LITERAL_AUID(0x06474203,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3903,
    AAF_TYPE(UInt8),
    false,
    false,
    FilmDescriptor)
  AAF_PROPERTY(FilmAspectRatio,
    AAF_LITERAL_AUID(0x06474204,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3904,
    AAF_TYPE(Rational),
    false,
    false,
    FilmDescriptor)
  AAF_PROPERTY(Manufacturer,
    AAF_LITERAL_AUID(0x06474205,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3905,
    AAF_TYPE(String),
    false,
    false,
    FilmDescriptor)
  AAF_PROPERTY(Model,
    AAF_LITERAL_AUID(0x06474206,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3906,
    AAF_TYPE(String),
    false,
    false,
    FilmDescriptor)
AAF_CLASS_END(FilmDescriptor)
AAF_CLASS_SEPARATOR()

// TapeDescriptor
//
AAF_CLASS(TapeDescriptor,
  AAF_LITERAL_AUID(0x06474300,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  EssenceDescriptor,
  true)
  AAF_PROPERTY(FormFactor,
    AAF_LITERAL_AUID(0x06474301,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3a01,
    AAF_TYPE(TapeCaseType),
    false,
    false,
    TapeDescriptor)
  AAF_PROPERTY(VideoSignal,
    AAF_LITERAL_AUID(0x06474302,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3a02,
    AAF_TYPE(VideoSignalType),
    false,
    false,
    TapeDescriptor)
  AAF_PROPERTY(TapeFormat,
    AAF_LITERAL_AUID(0x06474303,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3a03,
    AAF_TYPE(TapeFormatType),
    false,
    false,
    TapeDescriptor)
  AAF_PROPERTY(Length,
    AAF_LITERAL_AUID(0x06474304,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3a04,
    AAF_TYPE(UInt32),
    false,
    false,
    TapeDescriptor)
  AAF_PROPERTY(ManufacturerID,
    AAF_LITERAL_AUID(0x06474305,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3a05,
    AAF_TYPE(String),
    false,
    false,
    TapeDescriptor)
  AAF_PROPERTY(Model,
    AAF_LITERAL_AUID(0x06474306,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3a06,
    AAF_TYPE(String),
    false,
    false,
    TapeDescriptor)
AAF_CLASS_END(TapeDescriptor)
AAF_CLASS_SEPARATOR()

// Header
//
AAF_CLASS(Header,
  AAF_LITERAL_AUID(0x06480000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  InterchangeObject,
  true)
  AAF_PROPERTY(ByteOrder,
    AAF_LITERAL_AUID(0x06480100,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3b01,
    AAF_TYPE(Int16),
    true,
    false,
    Header)
  AAF_PROPERTY(LastModified,
    AAF_LITERAL_AUID(0x06480200,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3b02,
    AAF_TYPE(TimeStamp),
    true,
    false,
    Header)
  AAF_PROPERTY(Content,
    AAF_LITERAL_AUID(0x06480300,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3b03,
    AAF_REFERENCE_TYPE(StrongReference, ContentStorage),
    true,
    false,
    Header)
  AAF_PROPERTY(Dictionary,
    AAF_LITERAL_AUID(0x06480400,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3b04,
    AAF_REFERENCE_TYPE(StrongReference, Dictionary),
    true,
    false,
    Header)
  AAF_PROPERTY(Version,
    AAF_LITERAL_AUID(0x06480500,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3b05,
    AAF_TYPE(VersionType),
    true,
    false,
    Header)
  AAF_PROPERTY(IdentificationList,
    AAF_LITERAL_AUID(0x06480600,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3b06,
    AAF_REFERENCE_TYPE(StrongReferenceVector, Identification),
    true,
    false,
    Header)
  AAF_PROPERTY(ObjectModelVersion,
    AAF_LITERAL_AUID(0x06480700,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3b07,
    AAF_TYPE(UInt32),
    false,
    false,
    Header)
AAF_CLASS_END(Header)
AAF_CLASS_SEPARATOR()

// Identification
//
AAF_CLASS(Identification,
  AAF_LITERAL_AUID(0x06490000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  InterchangeObject,
  true)
  AAF_PROPERTY(CompanyName,
    AAF_LITERAL_AUID(0x06490100,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3c01,
    AAF_TYPE(String),
    true,
    false,
    Identification)
  AAF_PROPERTY(ProductName,
    AAF_LITERAL_AUID(0x06490200,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3c02,
    AAF_TYPE(String),
    true,
    false,
    Identification)
  AAF_PROPERTY(ProductVersion,
    AAF_LITERAL_AUID(0x06490300,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3c03,
    AAF_TYPE(ProductVersion),
    false,
    false,
    Identification)
  AAF_PROPERTY(ProductVersionString,
    AAF_LITERAL_AUID(0x06490400,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3c04,
    AAF_TYPE(String),
    true,
    false,
    Identification)
  AAF_PROPERTY(ProductID,
    AAF_LITERAL_AUID(0x06490500,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3c05,
    AAF_TYPE(AUID),
    true,
    false,
    Identification)
  AAF_PROPERTY(Date,
    AAF_LITERAL_AUID(0x06490600,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3c06,
    AAF_TYPE(TimeStamp),
    true,
    false,
    Identification)
  AAF_PROPERTY(ToolkitVersion,
    AAF_LITERAL_AUID(0x06490700,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3c07,
    AAF_TYPE(ProductVersion),
    false,
    false,
    Identification)
  AAF_PROPERTY(Platform,
    AAF_LITERAL_AUID(0x06490800,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3c08,
    AAF_TYPE(String),
    false,
    false,
    Identification)
  AAF_PROPERTY(GenerationAUID,
    AAF_LITERAL_AUID(0x06490900,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x3c09,
    AAF_TYPE(AUID),
    true,
    false,
    Identification)
AAF_CLASS_END(Identification)
AAF_CLASS_SEPARATOR()

// Locator
//
AAF_CLASS(Locator,
  AAF_LITERAL_AUID(0x064A0000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  InterchangeObject,
  false)
AAF_CLASS_END(Locator)
AAF_CLASS_SEPARATOR()

// NetworkLocator
//
AAF_CLASS(NetworkLocator,
  AAF_LITERAL_AUID(0x064A4300,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  Locator,
  true)
  AAF_PROPERTY(URLString,
    AAF_LITERAL_AUID(0x064A4301,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x4001,
    AAF_TYPE(String),
    true,
    false,
    NetworkLocator)
AAF_CLASS_END(NetworkLocator)
AAF_CLASS_SEPARATOR()

// TextLocator
//
AAF_CLASS(TextLocator,
  AAF_LITERAL_AUID(0x064A4400,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  Locator,
  true)
  AAF_PROPERTY(Name,
    AAF_LITERAL_AUID(0x064A4401,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x4101,
    AAF_TYPE(String),
    true,
    false,
    TextLocator)
AAF_CLASS_END(TextLocator)
AAF_CLASS_SEPARATOR()

// Mob
//
AAF_CLASS(Mob,
  AAF_LITERAL_AUID(0x064B0000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  InterchangeObject,
  false)
  AAF_PROPERTY(MobID,
    AAF_LITERAL_AUID(0x064B0100,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x4401,
    AAF_TYPE(MobID),
    true,
    true,
    Mob)
  AAF_PROPERTY(Name,
    AAF_LITERAL_AUID(0x064B0200,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x4402,
    AAF_TYPE(String),
    false,
    false,
    Mob)
  AAF_PROPERTY(Slots,
    AAF_LITERAL_AUID(0x064B0300,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x4403,
    AAF_REFERENCE_TYPE(StrongReferenceVector, MobSlot),
    true,
    false,
    Mob)
  AAF_PROPERTY(LastModified,
    AAF_LITERAL_AUID(0x064B0400,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x4404,
    AAF_TYPE(TimeStamp),
    true,
    false,
    Mob)
  AAF_PROPERTY(CreationTime,
    AAF_LITERAL_AUID(0x064B0500,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x4405,
    AAF_TYPE(TimeStamp),
    true,
    false,
    Mob)
  AAF_PROPERTY(UserComments,
    AAF_LITERAL_AUID(0x064B0600,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x4406,
    AAF_REFERENCE_TYPE(StrongReferenceVector, TaggedValue),
    false,
    false,
    Mob)
  AAF_PROPERTY(KLVData,
    AAF_LITERAL_AUID(0x064B0700,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x4407,
    AAF_REFERENCE_TYPE(StrongReferenceVector, KLVData),
    false,
    false,
    Mob)
AAF_CLASS_END(Mob)
AAF_CLASS_SEPARATOR()

// CompositionMob
//
AAF_CLASS(CompositionMob,
  AAF_LITERAL_AUID(0x064B4100,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  Mob,
  true)
  AAF_PROPERTY(DefaultFadeLength,
    AAF_LITERAL_AUID(0x064B4101,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x4501,
    AAF_TYPE(Length),
    false,
    false,
    CompositionMob)
  AAF_PROPERTY(DefFadeType,
    AAF_LITERAL_AUID(0x064B4102,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x4502,
    AAF_TYPE(FadeType),
    false,
    false,
    CompositionMob)
  AAF_PROPERTY(DefFadeEditUnit,
    AAF_LITERAL_AUID(0x064B4103,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x4503,
    AAF_TYPE(Rational),
    false,
    false,
    CompositionMob)
AAF_CLASS_END(CompositionMob)
AAF_CLASS_SEPARATOR()

// MasterMob
//
AAF_CLASS(MasterMob,
  AAF_LITERAL_AUID(0x064B4200,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  Mob,
  true)
AAF_CLASS_END(MasterMob)
AAF_CLASS_SEPARATOR()

// SourceMob
//
AAF_CLASS(SourceMob,
  AAF_LITERAL_AUID(0x064B4300,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  Mob,
  true)
  AAF_PROPERTY(EssenceDescription,
    AAF_LITERAL_AUID(0x064B4301,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x4701,
    AAF_REFERENCE_TYPE(StrongReference, EssenceDescriptor),
    true,
    false,
    SourceMob)
AAF_CLASS_END(SourceMob)
AAF_CLASS_SEPARATOR()

// MobSlot
//
AAF_CLASS(MobSlot,
  AAF_LITERAL_AUID(0x064C0000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  InterchangeObject,
  false)
  AAF_PROPERTY(SlotID,
    AAF_LITERAL_AUID(0x064C0100,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x4801,
    AAF_TYPE(Int32),
    true,
    false,
    MobSlot)
  AAF_PROPERTY(SlotName,
    AAF_LITERAL_AUID(0x064C0200,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x4802,
    AAF_TYPE(String),
    false,
    false,
    MobSlot)
  AAF_PROPERTY(Segment,
    AAF_LITERAL_AUID(0x064C0300,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x4803,
    AAF_REFERENCE_TYPE(StrongReference, Segment),
    true,
    false,
    MobSlot)
  AAF_PROPERTY(PhysicalTrackNumber,
    AAF_LITERAL_AUID(0x064C0400,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x4804,
    AAF_TYPE(UInt32),
    false,
    false,
    MobSlot)
AAF_CLASS_END(MobSlot)
AAF_CLASS_SEPARATOR()

// EventMobSlot
//
AAF_CLASS(EventMobSlot,
  AAF_LITERAL_AUID(0x064C4100,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  MobSlot,
  true)
  AAF_PROPERTY(EditRate,
    AAF_LITERAL_AUID(0x064C4101,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x4901,
    AAF_TYPE(Rational),
    true,
    false,
    EventMobSlot)
AAF_CLASS_END(EventMobSlot)
AAF_CLASS_SEPARATOR()

// StaticMobSlot
//
AAF_CLASS(StaticMobSlot,
  AAF_LITERAL_AUID(0x064C4200,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  MobSlot,
  true)
AAF_CLASS_END(StaticMobSlot)
AAF_CLASS_SEPARATOR()

// TimelineMobSlot
//
AAF_CLASS(TimelineMobSlot,
  AAF_LITERAL_AUID(0x064C4300,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  MobSlot,
  true)
  AAF_PROPERTY(EditRate,
    AAF_LITERAL_AUID(0x064C4301,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x4b01,
    AAF_TYPE(Rational),
    true,
    false,
    TimelineMobSlot)
  AAF_PROPERTY(Origin,
    AAF_LITERAL_AUID(0x064C4302,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x4b02,
    AAF_TYPE(Position),
    true,
    false,
    TimelineMobSlot)
AAF_CLASS_END(TimelineMobSlot)
AAF_CLASS_SEPARATOR()

// Parameter
//
AAF_CLASS(Parameter,
  AAF_LITERAL_AUID(0x064D0000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  InterchangeObject,
  false)
  AAF_PROPERTY(Definition,
    AAF_LITERAL_AUID(0x064D0100,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x4c01,
    AAF_REFERENCE_TYPE(WeakReference, ParameterDefinition),
    true,
    false,
    Parameter)
AAF_CLASS_END(Parameter)
AAF_CLASS_SEPARATOR()

// ConstantValue
//
AAF_CLASS(ConstantValue,
  AAF_LITERAL_AUID(0x064D4100,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  Parameter,
  true)
  AAF_PROPERTY(Value,
    AAF_LITERAL_AUID(0x064D4101,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x4d01,
    AAF_TYPE(Indirect),
    true,
    false,
    ConstantValue)
AAF_CLASS_END(ConstantValue)
AAF_CLASS_SEPARATOR()

// VaryingValue
//
AAF_CLASS(VaryingValue,
  AAF_LITERAL_AUID(0x064D4200,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  Parameter,
  true)
  AAF_PROPERTY(Interpolation,
    AAF_LITERAL_AUID(0x064D4201,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x4e01,
    AAF_REFERENCE_TYPE(WeakReference, InterpolationDefinition),
    true,
    false,
    VaryingValue)
  AAF_PROPERTY(PointList,
    AAF_LITERAL_AUID(0x064D4202,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x4e02,
    AAF_REFERENCE_TYPE(StrongReferenceVector, ControlPoint),
    true,
    false,
    VaryingValue)
AAF_CLASS_END(VaryingValue)
AAF_CLASS_SEPARATOR()

// TaggedValue
//
AAF_CLASS(TaggedValue,
  AAF_LITERAL_AUID(0x064F0000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  InterchangeObject,
  true)
  AAF_PROPERTY(Name,
    AAF_LITERAL_AUID(0x064F0100,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x5001,
    AAF_TYPE(String),
    true,
    false,
    TaggedValue)
  AAF_PROPERTY(Value,
    AAF_LITERAL_AUID(0x064F0300,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x5003,
    AAF_TYPE(Indirect),
    true,
    false,
    TaggedValue)
AAF_CLASS_END(TaggedValue)
AAF_CLASS_SEPARATOR()

// KLVData
//
AAF_CLASS(KLVData,
  AAF_LITERAL_AUID(0x06500000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  InterchangeObject,
  true)
  AAF_PROPERTY(Value,
    AAF_LITERAL_AUID(0x06500100,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x5101,
    AAF_TYPE(Opaque),
    true,
    false,
    KLVData)
AAF_CLASS_END(KLVData)
AAF_CLASS_SEPARATOR()

// TypeDefinitionInteger
//
AAF_CLASS(TypeDefinitionInteger,
  AAF_LITERAL_AUID(0x06510000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  TypeDefinition,
  true)
  AAF_PROPERTY(Size,
    AAF_LITERAL_AUID(0x06510100,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x5201,
    AAF_TYPE(UInt8),
    true,
    false,
    TypeDefinitionInteger)
  AAF_PROPERTY(IsSigned,
    AAF_LITERAL_AUID(0x06510200,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x5202,
    AAF_TYPE(Boolean),
    true,
    false,
    TypeDefinitionInteger)
AAF_CLASS_END(TypeDefinitionInteger)
AAF_CLASS_SEPARATOR()

// TypeDefinitionStrongObjectReference
//
AAF_CLASS(TypeDefinitionStrongObjectReference,
  AAF_LITERAL_AUID(0x06520000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  TypeDefinition,
  true)
  AAF_PROPERTY(ReferencedType,
    AAF_LITERAL_AUID(0x06520100,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x5301,
    AAF_REFERENCE_TYPE(WeakReference, ClassDefinition),
    true,
    false,
    TypeDefinitionStrongObjectReference)
AAF_CLASS_END(TypeDefinitionStrongObjectReference)
AAF_CLASS_SEPARATOR()

// TypeDefinitionWeakObjectReference
//
AAF_CLASS(TypeDefinitionWeakObjectReference,
  AAF_LITERAL_AUID(0x06530000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  TypeDefinition,
  true)
  AAF_PROPERTY(ReferencedType,
    AAF_LITERAL_AUID(0x06530100,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x5401,
    AAF_REFERENCE_TYPE(WeakReference, ClassDefinition),
    true,
    false,
    TypeDefinitionWeakObjectReference)
AAF_CLASS_END(TypeDefinitionWeakObjectReference)
AAF_CLASS_SEPARATOR()

// TypeDefinitionEnumeration
//
AAF_CLASS(TypeDefinitionEnumeration,
  AAF_LITERAL_AUID(0x06540000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  TypeDefinition,
  true)
  AAF_PROPERTY(ElementType,
    AAF_LITERAL_AUID(0x06540100,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x5501,
    AAF_REFERENCE_TYPE(WeakReference, TypeDefinition),
    true,
    false,
    TypeDefinitionEnumeration)
  AAF_PROPERTY(ElementNames,
    AAF_LITERAL_AUID(0x06540200,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x5502,
    AAF_TYPE(StringArray),
    true,
    false,
    TypeDefinitionEnumeration)
  AAF_PROPERTY(ElementValues,
    AAF_LITERAL_AUID(0x06540300,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x5503,
    AAF_TYPE(Int64Array),
    true,
    false,
    TypeDefinitionEnumeration)
AAF_CLASS_END(TypeDefinitionEnumeration)
AAF_CLASS_SEPARATOR()

// TypeDefinitionFixedArray
//
AAF_CLASS(TypeDefinitionFixedArray,
  AAF_LITERAL_AUID(0x06550000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  TypeDefinition,
  true)
  AAF_PROPERTY(ElementType,
    AAF_LITERAL_AUID(0x06550100,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x5601,
    AAF_REFERENCE_TYPE(WeakReference, TypeDefinition),
    true,
    false,
    TypeDefinitionFixedArray)
  AAF_PROPERTY(ElementCount,
    AAF_LITERAL_AUID(0x06550200,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x5602,
    AAF_TYPE(UInt32),
    true,
    false,
    TypeDefinitionFixedArray)
AAF_CLASS_END(TypeDefinitionFixedArray)
AAF_CLASS_SEPARATOR()

// TypeDefinitionVariableArray
//
AAF_CLASS(TypeDefinitionVariableArray,
  AAF_LITERAL_AUID(0x06560000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  TypeDefinition,
  true)
  AAF_PROPERTY(ElementType,
    AAF_LITERAL_AUID(0x06560100,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x5701,
    AAF_REFERENCE_TYPE(WeakReference, TypeDefinition),
    true,
    false,
    TypeDefinitionVariableArray)
AAF_CLASS_END(TypeDefinitionVariableArray)
AAF_CLASS_SEPARATOR()

// TypeDefinitionSet
//
AAF_CLASS(TypeDefinitionSet,
  AAF_LITERAL_AUID(0x06570000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  TypeDefinition,
  true)
  AAF_PROPERTY(ElementType,
    AAF_LITERAL_AUID(0x06570100,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x5801,
    AAF_REFERENCE_TYPE(WeakReference, TypeDefinition),
    true,
    false,
    TypeDefinitionSet)
AAF_CLASS_END(TypeDefinitionSet)
AAF_CLASS_SEPARATOR()

// TypeDefinitionString
//
AAF_CLASS(TypeDefinitionString,
  AAF_LITERAL_AUID(0x06580000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  TypeDefinition,
  true)
  AAF_PROPERTY(ElementType,
    AAF_LITERAL_AUID(0x06580100,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x5901,
    AAF_REFERENCE_TYPE(WeakReference, TypeDefinition),
    true,
    false,
    TypeDefinitionString)
AAF_CLASS_END(TypeDefinitionString)
AAF_CLASS_SEPARATOR()

// TypeDefinitionStream
//
AAF_CLASS(TypeDefinitionStream,
  AAF_LITERAL_AUID(0x06590000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  TypeDefinition,
  true)
  AAF_PROPERTY(ElementType,
    AAF_LITERAL_AUID(0x06590100,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x5a01,
    AAF_REFERENCE_TYPE(WeakReference, TypeDefinition),
    true,
    false,
    TypeDefinitionStream)
AAF_CLASS_END(TypeDefinitionStream)
AAF_CLASS_SEPARATOR()

// TypeDefinitionRecord
//
AAF_CLASS(TypeDefinitionRecord,
  AAF_LITERAL_AUID(0x065A0000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  TypeDefinition,
  true)
  AAF_PROPERTY(MemberTypes,
    AAF_LITERAL_AUID(0x065A0100,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x5b01,
    AAF_REFERENCE_TYPE(WeakReferenceVector, TypeDefinition),
    true,
    false,
    TypeDefinitionRecord)
  AAF_PROPERTY(MemberNames,
    AAF_LITERAL_AUID(0x065A0200,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x5b02,
    AAF_TYPE(StringArray),
    true,
    false,
    TypeDefinitionRecord)
AAF_CLASS_END(TypeDefinitionRecord)
AAF_CLASS_SEPARATOR()

// TypeDefinitionRename
//
AAF_CLASS(TypeDefinitionRename,
  AAF_LITERAL_AUID(0x065B0000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  TypeDefinition,
  true)
  AAF_PROPERTY(RenamedType,
    AAF_LITERAL_AUID(0x065B0100,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x5c01,
    AAF_REFERENCE_TYPE(WeakReference, TypeDefinition),
    true,
    false,
    TypeDefinitionRename)
AAF_CLASS_END(TypeDefinitionRename)
AAF_CLASS_SEPARATOR()

// TypeDefinitionExtendibleEnumeration
//
AAF_CLASS(TypeDefinitionExtendibleEnumeration,
  AAF_LITERAL_AUID(0x065C0000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  TypeDefinition,
  true)
  AAF_PROPERTY(ElementNames,
    AAF_LITERAL_AUID(0x065C0100,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x5d01,
    AAF_TYPE(StringArray),
    true,
    false,
    TypeDefinitionExtendibleEnumeration)
  AAF_PROPERTY(ElementValues,
    AAF_LITERAL_AUID(0x065C0200,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x5d02,
    AAF_TYPE(AUIDArray),
    true,
    false,
    TypeDefinitionExtendibleEnumeration)
AAF_CLASS_END(TypeDefinitionExtendibleEnumeration)
AAF_CLASS_SEPARATOR()

// TypeDefinitionIndirect
//
AAF_CLASS(TypeDefinitionIndirect,
  AAF_LITERAL_AUID(0x065D0000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  TypeDefinition,
  true)
AAF_CLASS_END(TypeDefinitionIndirect)
AAF_CLASS_SEPARATOR()

// TypeDefinitionOpaque
//
AAF_CLASS(TypeDefinitionOpaque,
  AAF_LITERAL_AUID(0x065E0000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  TypeDefinitionIndirect,
  true)
AAF_CLASS_END(TypeDefinitionOpaque)
AAF_CLASS_SEPARATOR()

// TypeDefinitionCharacter
//
AAF_CLASS(TypeDefinitionCharacter,
  AAF_LITERAL_AUID(0x065F0000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  TypeDefinition,
  true)
AAF_CLASS_END(TypeDefinitionCharacter)
AAF_CLASS_SEPARATOR()

// MetaDefinition
//
AAF_CLASS(MetaDefinition,
  AAF_LITERAL_AUID(0x06600000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  InterchangeObject,
  false)
  AAF_PROPERTY(Identification,
    AAF_LITERAL_AUID(0x06600100,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x5e01,
    AAF_TYPE(AUID),
    true,
    true,
    MetaDefinition)
  AAF_PROPERTY(Name,
    AAF_LITERAL_AUID(0x06600200,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x5e02,
    AAF_TYPE(String),
    true,
    false,
    MetaDefinition)
  AAF_PROPERTY(Description,
    AAF_LITERAL_AUID(0x06600300,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x5e03,
    AAF_TYPE(String),
    false,
    false,
    MetaDefinition)
AAF_CLASS_END(MetaDefinition)
AAF_CLASS_SEPARATOR()

// MetaDictionary
//
AAF_CLASS(MetaDictionary,
  AAF_LITERAL_AUID(0x06610000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
  InterchangeObject,
  true)
  AAF_PROPERTY(ClassDefinitions,
    AAF_LITERAL_AUID(0x06610100,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x5f01,
    AAF_REFERENCE_TYPE(StrongReferenceSet, ClassDefinition),
    false,
    false,
    MetaDictionary)
  AAF_PROPERTY(TypeDefinitions,
    AAF_LITERAL_AUID(0x06610200,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x04),
    0x5f02,
    AAF_REFERENCE_TYPE(StrongReferenceSet, TypeDefinition),
    false,
    false,
    MetaDictionary)
AAF_CLASS_END(MetaDictionary)

AAF_TABLE_END()

// Types
//

AAF_TYPE_TABLE_BEGIN()

// UInt8
//
AAF_TYPE_DEFINITION_INTEGER(UInt8, 
  AAF_LITERAL_AUID(0x0C010000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C), 1, False)
AAF_TYPE_SEPARATOR()

// UInt16
//
AAF_TYPE_DEFINITION_INTEGER(UInt16, 
  AAF_LITERAL_AUID(0x0C010100,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C), 2, False)
AAF_TYPE_SEPARATOR()

// UInt32
//
AAF_TYPE_DEFINITION_INTEGER(UInt32, 
  AAF_LITERAL_AUID(0x0C010200,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C), 4, False)
AAF_TYPE_SEPARATOR()

// UInt64
//
AAF_TYPE_DEFINITION_INTEGER(UInt64, 
  AAF_LITERAL_AUID(0x0C010300,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C), 8, False)
AAF_TYPE_SEPARATOR()

// Int8
//
AAF_TYPE_DEFINITION_INTEGER(Int8, 
  AAF_LITERAL_AUID(0x0C010400,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C), 1, True)
AAF_TYPE_SEPARATOR()

// Int16
//
AAF_TYPE_DEFINITION_INTEGER(Int16, 
  AAF_LITERAL_AUID(0x0C010500,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C), 2, True)
AAF_TYPE_SEPARATOR()

// Int32
//
AAF_TYPE_DEFINITION_INTEGER(Int32, 
  AAF_LITERAL_AUID(0x0C010600,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C), 4, True)
AAF_TYPE_SEPARATOR()

// Int64
//
AAF_TYPE_DEFINITION_INTEGER(Int64, 
  AAF_LITERAL_AUID(0x0C010700,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C), 8, True)
AAF_TYPE_SEPARATOR()

// Boolean
//
AAF_TYPE_DEFINITION_ENUMERATION(Boolean, 
  AAF_LITERAL_AUID(0x0C020000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C), AAF_TYPE(UInt8))
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFFalse,
    0, Boolean)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFTrue,
    1, Boolean)
AAF_TYPE_DEFINITION_ENUMERATION_END(Boolean)
AAF_TYPE_SEPARATOR()

// ProductReleaseType
//
AAF_TYPE_DEFINITION_ENUMERATION(ProductReleaseType, 
  AAF_LITERAL_AUID(0x0C020100,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C), AAF_TYPE(UInt8))
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFVersionUnknown,
    0, ProductReleaseType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFVersionReleased,
    1, ProductReleaseType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFVersionDebug,
    2, ProductReleaseType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFVersionPatched,
    3, ProductReleaseType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFVersionBeta,
    4, ProductReleaseType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFVersionPrivateBuild,
    5, ProductReleaseType)
AAF_TYPE_DEFINITION_ENUMERATION_END(ProductReleaseType)
AAF_TYPE_SEPARATOR()

// TapeFormatType
//
AAF_TYPE_DEFINITION_ENUMERATION(TapeFormatType, 
  AAF_LITERAL_AUID(0x0C020200,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C), AAF_TYPE(UInt8))
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFTapeFormatNull,
    0, TapeFormatType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFBetacamFormat,
    1, TapeFormatType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFBetacamSPFormat,
    2, TapeFormatType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFVHSFormat,
    3, TapeFormatType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFSVHSFormat,
    4, TapeFormatType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAF8mmFormat,
    5, TapeFormatType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFHi8Format,
    6, TapeFormatType)
AAF_TYPE_DEFINITION_ENUMERATION_END(TapeFormatType)
AAF_TYPE_SEPARATOR()

// VideoSignalType
//
AAF_TYPE_DEFINITION_ENUMERATION(VideoSignalType, 
  AAF_LITERAL_AUID(0x0C020300,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C), AAF_TYPE(UInt8))
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFVideoSignalNull,
    0, VideoSignalType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFNTSCSignal,
    1, VideoSignalType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFPALSignal,
    2, VideoSignalType)
AAF_TYPE_DEFINITION_ENUMERATION_END(VideoSignalType)
AAF_TYPE_SEPARATOR()

// TapeCaseType
//
AAF_TYPE_DEFINITION_ENUMERATION(TapeCaseType, 
  AAF_LITERAL_AUID(0x0C020400,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C), AAF_TYPE(UInt8))
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFTapeCaseNull,
    0, TapeCaseType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFThreeFourthInchVideoTape,
    1, TapeCaseType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFVHSVideoTape,
    2, TapeCaseType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAF8mmVideoTape,
    3, TapeCaseType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFBetacamVideoTape,
    4, TapeCaseType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFCompactCassette,
    5, TapeCaseType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFDATCartridge,
    6, TapeCaseType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFNagraAudioTape,
    7, TapeCaseType)
AAF_TYPE_DEFINITION_ENUMERATION_END(TapeCaseType)
AAF_TYPE_SEPARATOR()

// ColorSitingType
//
AAF_TYPE_DEFINITION_ENUMERATION(ColorSitingType, 
  AAF_LITERAL_AUID(0x0C020500,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C), AAF_TYPE(UInt8))
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFCoSiting,
    0, ColorSitingType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFAveraging,
    1, ColorSitingType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFThreeTap,
    2, ColorSitingType)
AAF_TYPE_DEFINITION_ENUMERATION_END(ColorSitingType)
AAF_TYPE_SEPARATOR()

// EditHintType
//
AAF_TYPE_DEFINITION_ENUMERATION(EditHintType, 
  AAF_LITERAL_AUID(0x0C020600,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C), AAF_TYPE(UInt8))
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFNoEditHint,
    0, EditHintType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFProportional,
    1, EditHintType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFRelativeLeft,
    2, EditHintType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFRelativeRight,
    3, EditHintType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFRelativeFixed,
    4, EditHintType)
AAF_TYPE_DEFINITION_ENUMERATION_END(EditHintType)
AAF_TYPE_SEPARATOR()

// FadeType
//
AAF_TYPE_DEFINITION_ENUMERATION(FadeType, 
  AAF_LITERAL_AUID(0x0C020700,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C), AAF_TYPE(UInt8))
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFFadeNone,
    0, FadeType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFFadeLinearAmp,
    1, FadeType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFFadeLinearPower,
    2, FadeType)
AAF_TYPE_DEFINITION_ENUMERATION_END(FadeType)
AAF_TYPE_SEPARATOR()

// LayoutType
//
AAF_TYPE_DEFINITION_ENUMERATION(LayoutType, 
  AAF_LITERAL_AUID(0x0C020800,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C), AAF_TYPE(UInt8))
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFFullFrame,
    0, LayoutType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFSeparateFields,
    1, LayoutType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFOneField,
    2, LayoutType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFMixedFields,
    3, LayoutType)
AAF_TYPE_DEFINITION_ENUMERATION_END(LayoutType)
AAF_TYPE_SEPARATOR()

// TCSource
//
AAF_TYPE_DEFINITION_ENUMERATION(TCSource, 
  AAF_LITERAL_AUID(0x0C020900,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C), AAF_TYPE(UInt8))
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFTimecodeLTC,
    0, TCSource)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFTimecodeVITC,
    1, TCSource)
AAF_TYPE_DEFINITION_ENUMERATION_END(TCSource)
AAF_TYPE_SEPARATOR()

// PulldownDirectionType
//
AAF_TYPE_DEFINITION_ENUMERATION(PulldownDirectionType, 
  AAF_LITERAL_AUID(0x0C020A00,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C), AAF_TYPE(UInt8))
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFTapeToFilmSpeed,
    0, PulldownDirectionType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFFilmToTapeSpeed,
    1, PulldownDirectionType)
AAF_TYPE_DEFINITION_ENUMERATION_END(PulldownDirectionType)
AAF_TYPE_SEPARATOR()

// PulldownKindType
//
AAF_TYPE_DEFINITION_ENUMERATION(PulldownKindType, 
  AAF_LITERAL_AUID(0x0C021000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C), AAF_TYPE(UInt8))
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFTwoThreePD,
    0, PulldownKindType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFPALPD,
    1, PulldownKindType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFOneToOneNTSC,
    2, PulldownKindType)
AAF_TYPE_DEFINITION_ENUMERATION_END(PulldownKindType)
AAF_TYPE_SEPARATOR()

// EdgeType
//
AAF_TYPE_DEFINITION_ENUMERATION(EdgeType, 
  AAF_LITERAL_AUID(0x0C020B00,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C), AAF_TYPE(UInt8))
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFEtNull,
    0, EdgeType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFEtKeycode,
    1, EdgeType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFEtEdgenum4,
    2, EdgeType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFEtEdgenum5,
    3, EdgeType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFEtHeaderSize,
    8, EdgeType)
AAF_TYPE_DEFINITION_ENUMERATION_END(EdgeType)
AAF_TYPE_SEPARATOR()

// FilmType
//
AAF_TYPE_DEFINITION_ENUMERATION(FilmType, 
  AAF_LITERAL_AUID(0x0C020C00,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C), AAF_TYPE(UInt8))
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFFtNull,
    0, FilmType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFFt35MM,
    1, FilmType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFFt16MM,
    2, FilmType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFFt8MM,
    3, FilmType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFFt65MM,
    4, FilmType)
AAF_TYPE_DEFINITION_ENUMERATION_END(FilmType)
AAF_TYPE_SEPARATOR()

// RGBAComponentKind
//
AAF_TYPE_DEFINITION_ENUMERATION(RGBAComponentKind, 
  AAF_LITERAL_AUID(0x0C020D00,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C), AAF_TYPE(UInt8))
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFCompNone,
    0x30, RGBAComponentKind)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFCompAlpha,
    0x41, RGBAComponentKind)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFCompBlue,
    0x42, RGBAComponentKind)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFCompFill,
    0x46, RGBAComponentKind)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFCompGreen,
    0x47, RGBAComponentKind)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFCompPalette,
    0x50, RGBAComponentKind)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFCompRed,
    0x52, RGBAComponentKind)
AAF_TYPE_DEFINITION_ENUMERATION_END(RGBAComponentKind)
AAF_TYPE_SEPARATOR()

// ReferenceType
//
AAF_TYPE_DEFINITION_ENUMERATION(ReferenceType, 
  AAF_LITERAL_AUID(0x0C020E00,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C), AAF_TYPE(UInt8))
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFRefLimitMinimum,
    0, ReferenceType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFRefLimitMaximum,
    1, ReferenceType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFRefMinimum,
    2, ReferenceType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFRefMaximum,
    3, ReferenceType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFRefEnumvalue,
    4, ReferenceType)
AAF_TYPE_DEFINITION_ENUMERATION_END(ReferenceType)
AAF_TYPE_SEPARATOR()

// AlphaTransparency
//
AAF_TYPE_DEFINITION_ENUMERATION(AlphaTransparency, 
  AAF_LITERAL_AUID(0x0C020F00,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C), AAF_TYPE(UInt8))
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFMinValueTransparent,
    0, AlphaTransparency)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFMaxValueTransparent,
    1, AlphaTransparency)
AAF_TYPE_DEFINITION_ENUMERATION_END(AlphaTransparency)
AAF_TYPE_SEPARATOR()

// UInt8Array8
//
AAF_TYPE_DEFINITION_FIXED_ARRAY(UInt8Array8, 
  AAF_LITERAL_AUID(0x0C030000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(UInt8), 8)
AAF_TYPE_SEPARATOR()

// UInt8Array
//
AAF_TYPE_DEFINITION_VARYING_ARRAY(UInt8Array, 
  AAF_LITERAL_AUID(0x0C030100,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(UInt8))
AAF_TYPE_SEPARATOR()

// UInt8Array12
//
AAF_TYPE_DEFINITION_FIXED_ARRAY(UInt8Array12, 
  AAF_LITERAL_AUID(0x0C030200,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(UInt8), 12)
AAF_TYPE_SEPARATOR()

// Int32Array
//
AAF_TYPE_DEFINITION_VARYING_ARRAY(Int32Array, 
  AAF_LITERAL_AUID(0x0C040000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(Int32))
AAF_TYPE_SEPARATOR()

// Int64Array
//
AAF_TYPE_DEFINITION_VARYING_ARRAY(Int64Array, 
  AAF_LITERAL_AUID(0x0C040100,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(Int64))
AAF_TYPE_SEPARATOR()

// RGBALayout
//
AAF_TYPE_DEFINITION_FIXED_ARRAY(RGBALayout, 
  AAF_LITERAL_AUID(0x0C040300,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(RGBAComponent), 8)
AAF_TYPE_SEPARATOR()

// StringArray
//
AAF_TYPE_DEFINITION_VARYING_ARRAY(StringArray, 
  AAF_LITERAL_AUID(0x0C040500,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(String))
AAF_TYPE_SEPARATOR()

// AUIDArray
//
AAF_TYPE_DEFINITION_VARYING_ARRAY(AUIDArray, 
  AAF_LITERAL_AUID(0x0C040400,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(AUID))
AAF_TYPE_SEPARATOR()

// Rational
//
AAF_TYPE_DEFINITION_RECORD(Rational, 
  AAF_LITERAL_AUID(0x0C050000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C))
  AAF_TYPE_DEFINITION_RECORD_FIELD(Numerator, AAF_TYPE(Int32),
    Rational)
  AAF_TYPE_DEFINITION_RECORD_FIELD(Denominator, AAF_TYPE(Int32),
    Rational)
AAF_TYPE_DEFINITION_RECORD_END(Rational)
AAF_TYPE_SEPARATOR()

// AUID
//
AAF_TYPE_DEFINITION_RECORD(AUID, 
  AAF_LITERAL_AUID(0x0C050100,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C))
  AAF_TYPE_DEFINITION_RECORD_FIELD(Data1, AAF_TYPE(UInt32),
    AUID)
  AAF_TYPE_DEFINITION_RECORD_FIELD(Data2, AAF_TYPE(UInt16),
    AUID)
  AAF_TYPE_DEFINITION_RECORD_FIELD(Data3, AAF_TYPE(UInt16),
    AUID)
  AAF_TYPE_DEFINITION_RECORD_FIELD(Data4, AAF_TYPE(UInt8Array8),
    AUID)
AAF_TYPE_DEFINITION_RECORD_END(AUID)
AAF_TYPE_SEPARATOR()

// MobID
//
AAF_TYPE_DEFINITION_RECORD(MobID, 
  AAF_LITERAL_AUID(0x0C050200,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C))
  AAF_TYPE_DEFINITION_RECORD_FIELD(SMPTELabel, AAF_TYPE(UInt8Array12),
    MobID)
  AAF_TYPE_DEFINITION_RECORD_FIELD(length, AAF_TYPE(UInt8),
    MobID)
  AAF_TYPE_DEFINITION_RECORD_FIELD(instanceHigh, AAF_TYPE(UInt8),
    MobID)
  AAF_TYPE_DEFINITION_RECORD_FIELD(instanceMid, AAF_TYPE(UInt8),
    MobID)
  AAF_TYPE_DEFINITION_RECORD_FIELD(instanceLow, AAF_TYPE(UInt8),
    MobID)
  AAF_TYPE_DEFINITION_RECORD_FIELD(material, AAF_TYPE(AUID),
    MobID)
AAF_TYPE_DEFINITION_RECORD_END(MobID)
AAF_TYPE_SEPARATOR()

// ProductVersion
//
AAF_TYPE_DEFINITION_RECORD(ProductVersion, 
  AAF_LITERAL_AUID(0x0C050300,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C))
  AAF_TYPE_DEFINITION_RECORD_FIELD(major, AAF_TYPE(UInt16),
    ProductVersion)
  AAF_TYPE_DEFINITION_RECORD_FIELD(minor, AAF_TYPE(UInt16),
    ProductVersion)
  AAF_TYPE_DEFINITION_RECORD_FIELD(tertiary, AAF_TYPE(UInt16),
    ProductVersion)
  AAF_TYPE_DEFINITION_RECORD_FIELD(patchLevel, AAF_TYPE(UInt16),
    ProductVersion)
  AAF_TYPE_DEFINITION_RECORD_FIELD(type, AAF_TYPE(ProductReleaseType),
    ProductVersion)
AAF_TYPE_DEFINITION_RECORD_END(ProductVersion)
AAF_TYPE_SEPARATOR()

// VersionType
//
AAF_TYPE_DEFINITION_RECORD(VersionType, 
  AAF_LITERAL_AUID(0x0C050400,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C))
  AAF_TYPE_DEFINITION_RECORD_FIELD(major, AAF_TYPE(Int8),
    VersionType)
  AAF_TYPE_DEFINITION_RECORD_FIELD(minor, AAF_TYPE(Int8),
    VersionType)
AAF_TYPE_DEFINITION_RECORD_END(VersionType)
AAF_TYPE_SEPARATOR()

// Rectangle
//
AAF_TYPE_DEFINITION_RECORD(Rectangle, 
  AAF_LITERAL_AUID(0x0C050500,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C))
  AAF_TYPE_DEFINITION_RECORD_FIELD(upperLeftHorizontal, AAF_TYPE(Rational),
    Rectangle)
  AAF_TYPE_DEFINITION_RECORD_FIELD(upperLeftVertical, AAF_TYPE(Rational),
    Rectangle)
  AAF_TYPE_DEFINITION_RECORD_FIELD(lowerRightHorizontal, AAF_TYPE(Rational),
    Rectangle)
  AAF_TYPE_DEFINITION_RECORD_FIELD(lowerRightVertical, AAF_TYPE(Rational),
    Rectangle)
AAF_TYPE_DEFINITION_RECORD_END(Rectangle)
AAF_TYPE_SEPARATOR()

// RGBAComponent
//
AAF_TYPE_DEFINITION_RECORD(RGBAComponent, 
  AAF_LITERAL_AUID(0x0C050600,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C))
  AAF_TYPE_DEFINITION_RECORD_FIELD(Code, AAF_TYPE(RGBAComponentKind),
    RGBAComponent)
  AAF_TYPE_DEFINITION_RECORD_FIELD(Size, AAF_TYPE(UInt8),
    RGBAComponent)
AAF_TYPE_DEFINITION_RECORD_END(RGBAComponent)
AAF_TYPE_SEPARATOR()

// DateStruct
//
AAF_TYPE_DEFINITION_RECORD(DateStruct, 
  AAF_LITERAL_AUID(0x0C050700,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C))
  AAF_TYPE_DEFINITION_RECORD_FIELD(year, AAF_TYPE(Int16),
    DateStruct)
  AAF_TYPE_DEFINITION_RECORD_FIELD(month, AAF_TYPE(UInt8),
    DateStruct)
  AAF_TYPE_DEFINITION_RECORD_FIELD(day, AAF_TYPE(UInt8),
    DateStruct)
AAF_TYPE_DEFINITION_RECORD_END(DateStruct)
AAF_TYPE_SEPARATOR()

// TimeStruct
//
AAF_TYPE_DEFINITION_RECORD(TimeStruct, 
  AAF_LITERAL_AUID(0x0C050800,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C))
  AAF_TYPE_DEFINITION_RECORD_FIELD(hour, AAF_TYPE(UInt8),
    TimeStruct)
  AAF_TYPE_DEFINITION_RECORD_FIELD(minute, AAF_TYPE(UInt8),
    TimeStruct)
  AAF_TYPE_DEFINITION_RECORD_FIELD(second, AAF_TYPE(UInt8),
    TimeStruct)
  AAF_TYPE_DEFINITION_RECORD_FIELD(fraction, AAF_TYPE(UInt8),
    TimeStruct)
AAF_TYPE_DEFINITION_RECORD_END(TimeStruct)
AAF_TYPE_SEPARATOR()

// TimeStamp
//
AAF_TYPE_DEFINITION_RECORD(TimeStamp, 
  AAF_LITERAL_AUID(0x0C050900,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C))
  AAF_TYPE_DEFINITION_RECORD_FIELD(date, AAF_TYPE(DateStruct),
    TimeStamp)
  AAF_TYPE_DEFINITION_RECORD_FIELD(time, AAF_TYPE(TimeStruct),
    TimeStamp)
AAF_TYPE_DEFINITION_RECORD_END(TimeStamp)
AAF_TYPE_SEPARATOR()

// Position
//
AAF_TYPE_DEFINITION_RENAME(Position, 
  AAF_LITERAL_AUID(0x0C060000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C), Int64)
AAF_TYPE_SEPARATOR()

// Length
//
AAF_TYPE_DEFINITION_RENAME(Length, 
  AAF_LITERAL_AUID(0x0C060100,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C), Int64)
AAF_TYPE_SEPARATOR()

// JPEGTableIDType
//
AAF_TYPE_DEFINITION_RENAME(JPEGTableIDType, 
  AAF_LITERAL_AUID(0x0C060200,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C), Int32)
AAF_TYPE_SEPARATOR()

// PhaseFrameType
//
AAF_TYPE_DEFINITION_RENAME(PhaseFrameType, 
  AAF_LITERAL_AUID(0x0C060300,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C), Int32)
AAF_TYPE_SEPARATOR()

// String
//
AAF_TYPE_DEFINITION_STRING(String, 
  AAF_LITERAL_AUID(0x0C070000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C), Character)
AAF_TYPE_SEPARATOR()

// OperationCategory
//
AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION(OperationCategory, 
  AAF_LITERAL_AUID(0x0C080000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C))
  AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_MEMBER(Effect,
    AAF_LITERAL_AUID(0x0D000000,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
    OperationCategory)
AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_END(OperationCategory)
AAF_TYPE_SEPARATOR()

// Character
//
AAF_TYPE_DEFINITION_CHARACTER(Character, 
  AAF_LITERAL_AUID(0x0C090000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C))
AAF_TYPE_SEPARATOR()

// DataStream
//
AAF_TYPE_DEFINITION_STREAM(DataStream, 
  AAF_LITERAL_AUID(0x0C0A0000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C))
AAF_TYPE_SEPARATOR()

// GammaType
//
AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION(GammaType, 
  AAF_LITERAL_AUID(0x0C0B0000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C))
AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_END(GammaType)
AAF_TYPE_SEPARATOR()

// CategoryType
//
AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION(CategoryType, 
  AAF_LITERAL_AUID(0x0C0C0000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C))
  AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_MEMBER(kAAFPluginCatagoryEffect,
    AAF_LITERAL_AUID(0x0C0C0100,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
    CategoryType)
  AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_MEMBER(kAAFPluginCatagoryCodec,
    AAF_LITERAL_AUID(0x0C0C0200,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
    CategoryType)
  AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_MEMBER(kAAFPluginCatagoryInterpolation,
    AAF_LITERAL_AUID(0x0C0C0300,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
    CategoryType)
AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_END(CategoryType)
AAF_TYPE_SEPARATOR()

// DataValue
//
AAF_TYPE_DEFINITION_VARYING_ARRAY(DataValue, 
  AAF_LITERAL_AUID(0xFFFFFFFF,
    0xFFFF, 0xFFFF,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0xFF),
  AAF_TYPE(UInt8))
AAF_TYPE_SEPARATOR()

// PositionArray
//
AAF_TYPE_DEFINITION_VARYING_ARRAY(PositionArray, 
  AAF_LITERAL_AUID(0xFFFFFFFF,
    0xFFFF, 0xFFFE,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0xFF),
  AAF_TYPE(UInt8))
AAF_TYPE_SEPARATOR()

// Indirect
//
AAF_TYPE_DEFINITION_INDIRECT(Indirect, 
  AAF_LITERAL_AUID(0xFFFFFFFF,
    0xFFFF, 0xFFFD,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0xFF))
AAF_TYPE_SEPARATOR()

// Opaque
//
AAF_TYPE_DEFINITION_OPAQUE(Opaque, 
  AAF_LITERAL_AUID(0xFFFFFFFF,
    0xFFFF, 0xFFFC,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0xFF))
AAF_TYPE_SEPARATOR()

// StrongReference<ContentStorage>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, ContentStorage), 
  AAF_LITERAL_AUID(0x0C0D0000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(ContentStorage))
AAF_TYPE_SEPARATOR()

// StrongReference<Dictionary>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, Dictionary), 
  AAF_LITERAL_AUID(0x0C0D0001,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(Dictionary))
AAF_TYPE_SEPARATOR()

// StrongReference<EssenceDescriptor>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, EssenceDescriptor), 
  AAF_LITERAL_AUID(0x0C0D0002,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(EssenceDescriptor))
AAF_TYPE_SEPARATOR()

// StrongReference<NetworkLocator>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, NetworkLocator), 
  AAF_LITERAL_AUID(0x0C0D0003,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(NetworkLocator))
AAF_TYPE_SEPARATOR()

// StrongReference<OperationGroup>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, OperationGroup), 
  AAF_LITERAL_AUID(0x0C0D0004,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(OperationGroup))
AAF_TYPE_SEPARATOR()

// StrongReference<Segment>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, Segment), 
  AAF_LITERAL_AUID(0x0C0D0005,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(Segment))
AAF_TYPE_SEPARATOR()

// StrongReference<SourceClip>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, SourceClip), 
  AAF_LITERAL_AUID(0x0C0D0006,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(SourceClip))
AAF_TYPE_SEPARATOR()

// StrongReference<SourceReference>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, SourceReference), 
  AAF_LITERAL_AUID(0x0C0D0007,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(SourceReference))
AAF_TYPE_SEPARATOR()

// StrongReference<ClassDefinition>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, ClassDefinition), 
  AAF_LITERAL_AUID(0x0C0D0008,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(ClassDefinition))
AAF_TYPE_SEPARATOR()

// StrongReference<CodecDefinition>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, CodecDefinition), 
  AAF_LITERAL_AUID(0x0C0D0009,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(CodecDefinition))
AAF_TYPE_SEPARATOR()

// StrongReference<Component>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, Component), 
  AAF_LITERAL_AUID(0x0C0D000A,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(Component))
AAF_TYPE_SEPARATOR()

// StrongReference<ContainerDefinition>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, ContainerDefinition), 
  AAF_LITERAL_AUID(0x0C0D000B,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(ContainerDefinition))
AAF_TYPE_SEPARATOR()

// StrongReference<ControlPoint>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, ControlPoint), 
  AAF_LITERAL_AUID(0x0C0D000C,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(ControlPoint))
AAF_TYPE_SEPARATOR()

// StrongReference<DataDefinition>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, DataDefinition), 
  AAF_LITERAL_AUID(0x0C0D000D,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(DataDefinition))
AAF_TYPE_SEPARATOR()

// StrongReference<EssenceData>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, EssenceData), 
  AAF_LITERAL_AUID(0x0C0D000E,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(EssenceData))
AAF_TYPE_SEPARATOR()

// StrongReference<Identification>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, Identification), 
  AAF_LITERAL_AUID(0x0C0D000F,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(Identification))
AAF_TYPE_SEPARATOR()

// StrongReference<InterpolationDefinition>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, InterpolationDefinition), 
  AAF_LITERAL_AUID(0x0C0D0010,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(InterpolationDefinition))
AAF_TYPE_SEPARATOR()

// StrongReference<Locator>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, Locator), 
  AAF_LITERAL_AUID(0x0C0D0011,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(Locator))
AAF_TYPE_SEPARATOR()

// StrongReference<Mob>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, Mob), 
  AAF_LITERAL_AUID(0x0C0D0012,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(Mob))
AAF_TYPE_SEPARATOR()

// StrongReference<MobSlot>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, MobSlot), 
  AAF_LITERAL_AUID(0x0C0D0013,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(MobSlot))
AAF_TYPE_SEPARATOR()

// StrongReference<OperationDefinition>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, OperationDefinition), 
  AAF_LITERAL_AUID(0x0C0D0014,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(OperationDefinition))
AAF_TYPE_SEPARATOR()

// StrongReference<Parameter>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, Parameter), 
  AAF_LITERAL_AUID(0x0C0D0015,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(Parameter))
AAF_TYPE_SEPARATOR()

// StrongReference<ParameterDefinition>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, ParameterDefinition), 
  AAF_LITERAL_AUID(0x0C0D0016,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(ParameterDefinition))
AAF_TYPE_SEPARATOR()

// StrongReference<PluginDefinition>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, PluginDefinition), 
  AAF_LITERAL_AUID(0x0C0D0017,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(PluginDefinition))
AAF_TYPE_SEPARATOR()

// StrongReference<PropertyDefinition>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, PropertyDefinition), 
  AAF_LITERAL_AUID(0x0C0D0018,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(PropertyDefinition))
AAF_TYPE_SEPARATOR()

// StrongReference<TaggedValue>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, TaggedValue), 
  AAF_LITERAL_AUID(0x0C0D0019,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(TaggedValue))
AAF_TYPE_SEPARATOR()

// StrongReference<TypeDefinition>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, TypeDefinition), 
  AAF_LITERAL_AUID(0x0C0D001A,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(TypeDefinition))
AAF_TYPE_SEPARATOR()

// StrongReference<KLVData>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, KLVData), 
  AAF_LITERAL_AUID(0x0C0D001b,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(KLVData))
AAF_TYPE_SEPARATOR()

// StrongReferenceSet<ClassDefinition>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceSet, ClassDefinition), 
  AAF_LITERAL_AUID(0x0C0D0100,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(ClassDefinition))
AAF_TYPE_SEPARATOR()

// StrongReferenceSet<CodecDefinition>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceSet, CodecDefinition), 
  AAF_LITERAL_AUID(0x0C0D0101,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(CodecDefinition))
AAF_TYPE_SEPARATOR()

// StrongReferenceSet<ContainerDefinition>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceSet, ContainerDefinition), 
  AAF_LITERAL_AUID(0x0C0D0102,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(ContainerDefinition))
AAF_TYPE_SEPARATOR()

// StrongReferenceSet<DataDefinition>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceSet, DataDefinition), 
  AAF_LITERAL_AUID(0x0C0D0103,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(DataDefinition))
AAF_TYPE_SEPARATOR()

// StrongReferenceSet<EssenceData>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceSet, EssenceData), 
  AAF_LITERAL_AUID(0x0C0D0104,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(EssenceData))
AAF_TYPE_SEPARATOR()

// StrongReferenceSet<InterpolationDefinition>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceSet, InterpolationDefinition), 
  AAF_LITERAL_AUID(0x0C0D0105,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(InterpolationDefinition))
AAF_TYPE_SEPARATOR()

// StrongReferenceSet<Locator>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceSet, Locator), 
  AAF_LITERAL_AUID(0x0C0D0106,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(Locator))
AAF_TYPE_SEPARATOR()

// StrongReferenceSet<Mob>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceSet, Mob), 
  AAF_LITERAL_AUID(0x0C0D0107,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(Mob))
AAF_TYPE_SEPARATOR()

// StrongReferenceSet<OperationDefinition>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceSet, OperationDefinition), 
  AAF_LITERAL_AUID(0x0C0D0108,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(OperationDefinition))
AAF_TYPE_SEPARATOR()

// StrongReferenceSet<ParameterDefinition>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceSet, ParameterDefinition), 
  AAF_LITERAL_AUID(0x0C0D010A,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(ParameterDefinition))
AAF_TYPE_SEPARATOR()

// StrongReferenceSet<PluginDefinition>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceSet, PluginDefinition), 
  AAF_LITERAL_AUID(0x0C0D010B,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(PluginDefinition))
AAF_TYPE_SEPARATOR()

// StrongReferenceSet<PropertyDefinition>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceSet, PropertyDefinition), 
  AAF_LITERAL_AUID(0x0C0D010C,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(PropertyDefinition))
AAF_TYPE_SEPARATOR()

// StrongReferenceSet<Segment>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceSet, Segment), 
  AAF_LITERAL_AUID(0x0C0D010D,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(Segment))
AAF_TYPE_SEPARATOR()

// StrongReferenceSet<TaggedValue>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceSet, TaggedValue), 
  AAF_LITERAL_AUID(0x0C0D010E,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(TaggedValue))
AAF_TYPE_SEPARATOR()

// StrongReferenceSet<TypeDefinition>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceSet, TypeDefinition), 
  AAF_LITERAL_AUID(0x0C0D010F,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(TypeDefinition))
AAF_TYPE_SEPARATOR()

// StrongReferenceVector<Component>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceVector, Component), 
  AAF_LITERAL_AUID(0x0C0D0200,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(Component))
AAF_TYPE_SEPARATOR()

// StrongReferenceVector<ControlPoint>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceVector, ControlPoint), 
  AAF_LITERAL_AUID(0x0C0D0201,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(ControlPoint))
AAF_TYPE_SEPARATOR()

// StrongReferenceVector<Identification>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceVector, Identification), 
  AAF_LITERAL_AUID(0x0C0D0202,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(Identification))
AAF_TYPE_SEPARATOR()

// StrongReferenceVector<Locator>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceVector, Locator), 
  AAF_LITERAL_AUID(0x0C0D0203,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(Locator))
AAF_TYPE_SEPARATOR()

// StrongReferenceVector<MobSlot>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceVector, MobSlot), 
  AAF_LITERAL_AUID(0x0C0D0204,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(MobSlot))
AAF_TYPE_SEPARATOR()

// StrongReferenceVector<Segment>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceVector, Segment), 
  AAF_LITERAL_AUID(0x0C0D0205,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(Segment))
AAF_TYPE_SEPARATOR()

// StrongReferenceVector<SourceReference>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceVector, SourceReference), 
  AAF_LITERAL_AUID(0x0C0D0206,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(SourceReference))
AAF_TYPE_SEPARATOR()

// StrongReferenceVector<TaggedValue>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceVector, TaggedValue), 
  AAF_LITERAL_AUID(0x0C0D0207,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(TaggedValue))
AAF_TYPE_SEPARATOR()

// StrongReferenceVector<KLVData>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceVector, KLVData), 
  AAF_LITERAL_AUID(0x0C0D0208,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(KLVData))
AAF_TYPE_SEPARATOR()

// StrongReferenceVector<Parameter>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceVector, Parameter), 
  AAF_LITERAL_AUID(0x0C0D0209,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(Parameter))
AAF_TYPE_SEPARATOR()

// WeakReference<ClassDefinition>
//
AAF_TYPE_DEFINITION_WEAK_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(WeakReference, ClassDefinition), 
  AAF_LITERAL_AUID(0x0C0D0300,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(ClassDefinition))
AAF_TYPE_SEPARATOR()

// WeakReference<ContainerDefinition>
//
AAF_TYPE_DEFINITION_WEAK_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(WeakReference, ContainerDefinition), 
  AAF_LITERAL_AUID(0x0C0D0301,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(ContainerDefinition))
AAF_TYPE_SEPARATOR()

// WeakReference<DataDefinition>
//
AAF_TYPE_DEFINITION_WEAK_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(WeakReference, DataDefinition), 
  AAF_LITERAL_AUID(0x0C0D0302,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(DataDefinition))
AAF_TYPE_SEPARATOR()

// WeakReference<DefinitionObject>
//
AAF_TYPE_DEFINITION_WEAK_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(WeakReference, DefinitionObject), 
  AAF_LITERAL_AUID(0x0C0D0303,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(DefinitionObject))
AAF_TYPE_SEPARATOR()

// WeakReference<Identification>
//
AAF_TYPE_DEFINITION_WEAK_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(WeakReference, Identification), 
  AAF_LITERAL_AUID(0x0C0D0304,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(Identification))
AAF_TYPE_SEPARATOR()

// WeakReference<InterpolationDefinition>
//
AAF_TYPE_DEFINITION_WEAK_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(WeakReference, InterpolationDefinition), 
  AAF_LITERAL_AUID(0x0C0D0305,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(InterpolationDefinition))
AAF_TYPE_SEPARATOR()

// WeakReference<Mob>
//
AAF_TYPE_DEFINITION_WEAK_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(WeakReference, Mob), 
  AAF_LITERAL_AUID(0x0C0D0306,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(Mob))
AAF_TYPE_SEPARATOR()

// WeakReference<OperationDefinition>
//
AAF_TYPE_DEFINITION_WEAK_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(WeakReference, OperationDefinition), 
  AAF_LITERAL_AUID(0x0C0D0307,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(OperationDefinition))
AAF_TYPE_SEPARATOR()

// WeakReference<ParameterDefinition>
//
AAF_TYPE_DEFINITION_WEAK_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(WeakReference, ParameterDefinition), 
  AAF_LITERAL_AUID(0x0C0D0308,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(ParameterDefinition))
AAF_TYPE_SEPARATOR()

// WeakReference<TypeDefinition>
//
AAF_TYPE_DEFINITION_WEAK_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(WeakReference, TypeDefinition), 
  AAF_LITERAL_AUID(0x0C0D0309,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(TypeDefinition))
AAF_TYPE_SEPARATOR()

// WeakReference<PluginDefinition>
//
AAF_TYPE_DEFINITION_WEAK_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(WeakReference, PluginDefinition), 
  AAF_LITERAL_AUID(0x0C0D030A,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(PluginDefinition))
AAF_TYPE_SEPARATOR()

// WeakReference<CodecDefinition>
//
AAF_TYPE_DEFINITION_WEAK_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(WeakReference, CodecDefinition), 
  AAF_LITERAL_AUID(0x0C0D030B,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(CodecDefinition))
AAF_TYPE_SEPARATOR()

// WeakReferenceSet<DataDefinition>
//
AAF_TYPE_DEFINITION_WEAK_REFERENCE_SET(
  AAF_REFERENCE_TYPE_NAME(WeakReferenceSet, DataDefinition), 
  AAF_LITERAL_AUID(0x0C0D0400,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(DataDefinition))
AAF_TYPE_SEPARATOR()

// WeakReferenceSet<ParameterDefinition>
//
AAF_TYPE_DEFINITION_WEAK_REFERENCE_SET(
  AAF_REFERENCE_TYPE_NAME(WeakReferenceSet, ParameterDefinition), 
  AAF_LITERAL_AUID(0x0C0D0401,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(ParameterDefinition))
AAF_TYPE_SEPARATOR()

// WeakReferenceSet<PluginDefinition>
//
AAF_TYPE_DEFINITION_WEAK_REFERENCE_SET(
  AAF_REFERENCE_TYPE_NAME(WeakReferenceSet, PluginDefinition), 
  AAF_LITERAL_AUID(0x0C0D0402,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(PluginDefinition))
AAF_TYPE_SEPARATOR()

// WeakReferenceVector<OperationDefinition>
//
AAF_TYPE_DEFINITION_WEAK_REFERENCE_VECTOR(
  AAF_REFERENCE_TYPE_NAME(WeakReferenceVector, OperationDefinition), 
  AAF_LITERAL_AUID(0x0C0D0500,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(OperationDefinition))
AAF_TYPE_SEPARATOR()

// WeakReferenceVector<TypeDefinition>
//
AAF_TYPE_DEFINITION_WEAK_REFERENCE_VECTOR(
  AAF_REFERENCE_TYPE_NAME(WeakReferenceVector, TypeDefinition), 
  AAF_LITERAL_AUID(0x0C0D0501,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x0C),
  AAF_TYPE(TypeDefinition))

AAF_TYPE_TABLE_END()

// Undefine all macros
//
#undef AAF_TABLE_BEGIN

#undef AAF_TABLE_END

#undef AAF_CLASS

#undef AAF_CLASS_END

#undef AAF_CLASS_SEPARATOR

#undef AAF_PROPERTY

#undef AAF_TYPE

#undef AAF_REFERENCE_TYPE

#undef AAF_REFERENCE_TYPE_NAME

#undef AAF_LITERAL_AUID

#undef AAF_TYPE_TABLE_BEGIN

#undef AAF_TYPE_TABLE_END

#undef AAF_TYPE_DEFINITION_INTEGER

#undef AAF_TYPE_DEFINITION_ENUMERATION

#undef AAF_TYPE_DEFINITION_ENUMERATION_MEMBER

#undef AAF_TYPE_DEFINITION_ENUMERATION_END

#undef AAF_TYPE_DEFINITION_RECORD

#undef AAF_TYPE_DEFINITION_RECORD_FIELD

#undef AAF_TYPE_DEFINITION_RECORD_END

#undef AAF_TYPE_DEFINITION_VARYING_ARRAY

#undef AAF_TYPE_DEFINITION_FIXED_ARRAY

#undef AAF_TYPE_DEFINITION_RENAME

#undef AAF_TYPE_DEFINITION_STRING

#undef AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION

#undef AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_MEMBER

#undef AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_END

#undef AAF_TYPE_DEFINITION_CHARACTER

#undef AAF_TYPE_DEFINITION_INDIRECT

#undef AAF_TYPE_DEFINITION_OPAQUE

#undef AAF_TYPE_DEFINITION_SET

#undef AAF_TYPE_DEFINITION_STRONG_REFERENCE

#undef AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET

#undef AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR

#undef AAF_TYPE_DEFINITION_WEAK_REFERENCE

#undef AAF_TYPE_DEFINITION_WEAK_REFERENCE_SET

#undef AAF_TYPE_DEFINITION_WEAK_REFERENCE_VECTOR

#undef AAF_TYPE_DEFINITION_STREAM

#undef AAF_TYPE_SEPARATOR

//
// End of automatically generated file.

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
// This file was generated on Fri May 17 16:55:52 EDT 2002
// by user tjb on system tjb.avid.com.
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
// AAF_CLASS_END(name, id, parent, concrete)
//
//   End an AAF class definition.
//
//     name      = the name of the class
//     id        = the auid used to identify the class [*]
//     parent    = the immediate ancestor class
//     concrete  = true if the class is concrete
//                 false if the class is abstract
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
//     uid       = true if the property is the unique identifier
//                 for this class, false if the property is not
//                 the unique identifier for this class
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
// AAF_TYPE_DEFINITION_ENUMERATION_END(name, id, type)
//
//   End an AAF enumerated type definition.
//
//     name      = the name of the type
//     id        = the auid used to identify the type [*]
//     type      = the type of each enumeration element [*]
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
// AAF_TYPE_DEFINITION_RECORD_END(name, id)
//
//   End an AAF record type definition.
//
//     name      = the name of the type
//     id        = the auid used to identify the type [*]
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
// AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_END(name, id)
//
//   End an AAF extendible enumerated type definition.
//
//     name      = the name of the type
//     id        = the auid used to identify the type [*]
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
// AAF_TYPE_DEFINITION_STRONG_REFERENCE(name, id, type)
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
// AAF_TYPE_DEFINITION_WEAK_REFERENCE(name, id, type)
//
//   Define an AAF weak reference type.
//
//     name      = the name of the type
//     id        = the auid used to identify the type [*]
//     type      = the target type [*]
//
// AAF_TYPE_DEFINITION_WEAK_REFERENCE_MEMBER(name, parent, container)
//
//   Define a member of an AAF extendible enumerated type.
//
//     name      = the name of the type
//     parent    = the parent property for member [*]
//     container = the name of the containing weak reference type
//
// AAF_TYPE_DEFINITION_WEAK_REFERENCE_END(name, id, type)
//
//   End an AAF extendible enumerated type definition.
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
// AAF_TYPE_DEFINITION_STREAM(name, id)
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
// AAF_CLASS_ALIAS(name, alias)
//
//     name      = class name
//     alias     = another, usually shorter, name by which the
//                 class is also known
//
// AAF_LITERAL_AUID(l, w1, w2,  b1, b2, b3, b4, b5, b6, b7, b8)
//
//   Define an AUID.
//
//     l         = Data1
//     w1        = Data2
//     w2        = Data3
//     b1-b8     = Data4
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
#define AAF_CLASS_END(name, id, parent, concrete)
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
#define AAF_TYPE_DEFINITION_ENUMERATION_END(name, id, type)
#endif

#ifndef AAF_TYPE_DEFINITION_RECORD
#define AAF_TYPE_DEFINITION_RECORD(name, id)
#endif

#ifndef AAF_TYPE_DEFINITION_RECORD_FIELD
#define AAF_TYPE_DEFINITION_RECORD_FIELD(name, type, parent)
#endif

#ifndef AAF_TYPE_DEFINITION_RECORD_END
#define AAF_TYPE_DEFINITION_RECORD_END(name, id)
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
#define AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_END(name, id)
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

#ifndef AAF_TYPE_DEFINITION_WEAK_REFERENCE_MEMBER
#define AAF_TYPE_DEFINITION_WEAK_REFERENCE_MEMBER(name, parent, container)
#endif

#ifndef AAF_TYPE_DEFINITION_WEAK_REFERENCE_END
#define AAF_TYPE_DEFINITION_WEAK_REFERENCE_END(name, id, type)
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

#ifndef AAF_CLASS_ALIAS
#define AAF_CLASS_ALIAS(name, alias)
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
  // {0101010D-0101-0100-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x0100,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Root,
  false)
  AAF_PROPERTY(ObjClass,
    // {04010106-0101-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010104,
      0x0101, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0101,
    AAF_REFERENCE_TYPE(WeakReference, ClassDefinition),
    true,
    false,
    InterchangeObject)
  AAF_PROPERTY(Generation,
    // {01072005-0800-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x05200701,
      0x0800, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0102,
    AAF_TYPE(AUID),
    false,
    false,
    InterchangeObject)
AAF_CLASS_END(InterchangeObject,
  // {0101010D-0101-0100-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x0100,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Root,
  false)
AAF_CLASS_SEPARATOR()

// Component
//
AAF_CLASS(Component,
  // {0101010D-0101-0200-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x0200,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  InterchangeObject,
  false)
  AAF_PROPERTY(DataDefinition,
    // {00010704-0000-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x04070100,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0201,
    AAF_REFERENCE_TYPE(WeakReference, DataDefinition),
    true,
    false,
    Component)
  AAF_PROPERTY(Length,
    // {01020207-0103-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x07020201,
      0x0103, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0202,
    AAF_TYPE(Length),
    false,
    false,
    Component)
  AAF_PROPERTY(KLVData,
    // {10020103-0400-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x03010210,
      0x0400, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0203,
    AAF_REFERENCE_TYPE(StrongReferenceVector, KLVData),
    false,
    false,
    Component)
AAF_CLASS_END(Component,
  // {0101010D-0101-0200-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x0200,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  InterchangeObject,
  false)
AAF_CLASS_SEPARATOR()

// Segment
//
AAF_CLASS(Segment,
  // {0101010D-0101-0300-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x0300,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Component,
  false)
AAF_CLASS_END(Segment,
  // {0101010D-0101-0300-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x0300,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Component,
  false)
AAF_CLASS_SEPARATOR()

// EdgeCode
//
AAF_CLASS(EdgeCode,
  // {0101010D-0101-0400-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x0400,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Segment,
  true)
  AAF_PROPERTY(Start,
    // {01090401-0000-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x01040901,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0401,
    AAF_TYPE(Position),
    true,
    false,
    EdgeCode)
  AAF_PROPERTY(FilmKind,
    // {03011004-0109-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x04100103,
      0x0109, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0402,
    AAF_TYPE(FilmType),
    true,
    false,
    EdgeCode)
  AAF_PROPERTY(CodeFormat,
    // {03011004-0102-0000-060E-2B3401010101}
    AAF_LITERAL_AUID(0x04100103,
      0x0102, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
    0x0403,
    AAF_TYPE(EdgeType),
    true,
    false,
    EdgeCode)
  AAF_PROPERTY(Header,
    // {01020301-0200-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x01030201,
      0x0200, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0404,
    AAF_TYPE(DataValue),
    false,
    false,
    EdgeCode)
AAF_CLASS_END(EdgeCode,
  // {0101010D-0101-0400-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x0400,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Segment,
  true)
AAF_CLASS_SEPARATOR()

// EssenceGroup
//
AAF_CLASS(EssenceGroup,
  // {0101010D-0101-0500-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x0500,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Segment,
  true)
  AAF_PROPERTY(Choices,
    // {04010106-0601-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010104,
      0x0601, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0501,
    AAF_REFERENCE_TYPE(StrongReferenceVector, SourceReference),
    true,
    false,
    EssenceGroup)
  AAF_PROPERTY(StillFrame,
    // {04010106-0208-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010104,
      0x0208, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0502,
    AAF_REFERENCE_TYPE(StrongReference, SourceReference),
    false,
    false,
    EssenceGroup)
AAF_CLASS_END(EssenceGroup,
  // {0101010D-0101-0500-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x0500,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Segment,
  true)
AAF_CLASS_SEPARATOR()

// Event
//
AAF_CLASS(Event,
  // {0101010D-0101-0600-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x0600,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Segment,
  false)
  AAF_PROPERTY(Position,
    // {03010207-0303-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x07020103,
      0x0303, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0601,
    AAF_TYPE(Position),
    true,
    false,
    Event)
  AAF_PROPERTY(Comment,
    // {04043005-0100-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x05300404,
      0x0100, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0602,
    AAF_TYPE(String),
    false,
    false,
    Event)
AAF_CLASS_END(Event,
  // {0101010D-0101-0600-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x0600,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Segment,
  false)
AAF_CLASS_SEPARATOR()

// GPITrigger
//
AAF_CLASS(GPITrigger,
  // {0101010D-0101-0700-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x0700,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Event,
  true)
  AAF_PROPERTY(ActiveState,
    // {01043005-0000-0000-060E-2B3401010101}
    AAF_LITERAL_AUID(0x05300401,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
    0x0801,
    AAF_TYPE(Boolean),
    true,
    false,
    GPITrigger)
AAF_CLASS_END(GPITrigger,
  // {0101010D-0101-0700-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x0700,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Event,
  true)
AAF_CLASS_SEPARATOR()

// CommentMarker
//
AAF_CLASS(CommentMarker,
  // {0101010D-0101-0800-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x0800,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Event,
  true)
  AAF_PROPERTY(Annotation,
    // {04010106-020A-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010104,
      0x020A, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0901,
    AAF_REFERENCE_TYPE(StrongReference, SourceReference),
    false,
    false,
    CommentMarker)
AAF_CLASS_END(CommentMarker,
  // {0101010D-0101-0800-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x0800,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Event,
  true)
AAF_CLASS_SEPARATOR()

// Filler
//
AAF_CLASS(Filler,
  // {0101010D-0101-0900-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x0900,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Segment,
  true)
AAF_CLASS_END(Filler,
  // {0101010D-0101-0900-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x0900,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Segment,
  true)
AAF_CLASS_SEPARATOR()

// OperationGroup
//
AAF_CLASS(OperationGroup,
  // {0101010D-0101-0A00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x0A00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Segment,
  true)
  AAF_PROPERTY(OperationDefinition,
    // {06053005-0000-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x05300506,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0b01,
    AAF_REFERENCE_TYPE(WeakReference, OperationDefinition),
    true,
    false,
    OperationGroup)
  AAF_PROPERTY(InputSegments,
    // {04010106-0602-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010104,
      0x0602, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0b02,
    AAF_REFERENCE_TYPE(StrongReferenceVector, Segment),
    false,
    false,
    OperationGroup)
  AAF_PROPERTY(Parameters,
    // {04010106-060A-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010104,
      0x060A, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0b03,
    AAF_REFERENCE_TYPE(StrongReferenceVector, Parameter),
    false,
    false,
    OperationGroup)
  AAF_PROPERTY(BypassOverride,
    // {0C053005-0000-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x0530050C,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0b04,
    AAF_TYPE(UInt32),
    false,
    false,
    OperationGroup)
  AAF_PROPERTY(Rendering,
    // {04010106-0206-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010104,
      0x0206, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0b05,
    AAF_REFERENCE_TYPE(StrongReference, SourceReference),
    false,
    false,
    OperationGroup)
AAF_CLASS_END(OperationGroup,
  // {0101010D-0101-0A00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x0A00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Segment,
  true)
AAF_CLASS_SEPARATOR()

// NestedScope
//
AAF_CLASS(NestedScope,
  // {0101010D-0101-0B00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x0B00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Segment,
  true)
  AAF_PROPERTY(Slots,
    // {04010106-0607-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010104,
      0x0607, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0c01,
    AAF_REFERENCE_TYPE(StrongReferenceVector, Segment),
    true,
    false,
    NestedScope)
AAF_CLASS_END(NestedScope,
  // {0101010D-0101-0B00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x0B00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Segment,
  true)
AAF_CLASS_SEPARATOR()

// Pulldown
//
AAF_CLASS(Pulldown,
  // {0101010D-0101-0C00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x0C00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Segment,
  true)
  AAF_PROPERTY(InputSegment,
    // {04010106-0207-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010104,
      0x0207, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0d01,
    AAF_REFERENCE_TYPE(StrongReference, Segment),
    true,
    false,
    Pulldown)
  AAF_PROPERTY(PulldownKind,
    // {01104005-0200-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x05401001,
      0x0200, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0d02,
    AAF_TYPE(PulldownKindType),
    true,
    false,
    Pulldown)
  AAF_PROPERTY(PulldownDirection,
    // {01104005-0100-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x05401001,
      0x0100, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0d03,
    AAF_TYPE(PulldownDirectionType),
    true,
    false,
    Pulldown)
  AAF_PROPERTY(PhaseFrame,
    // {01104005-0300-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x05401001,
      0x0300, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0d04,
    AAF_TYPE(PhaseFrameType),
    true,
    false,
    Pulldown)
AAF_CLASS_END(Pulldown,
  // {0101010D-0101-0C00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x0C00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Segment,
  true)
AAF_CLASS_SEPARATOR()

// ScopeReference
//
AAF_CLASS(ScopeReference,
  // {0101010D-0101-0D00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x0D00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Segment,
  true)
  AAF_PROPERTY(RelativeScope,
    // {03010106-0300-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010103,
      0x0300, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0e01,
    AAF_TYPE(UInt32),
    true,
    false,
    ScopeReference)
  AAF_PROPERTY(RelativeSlot,
    // {03010106-0400-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010103,
      0x0400, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0e02,
    AAF_TYPE(UInt32),
    true,
    false,
    ScopeReference)
AAF_CLASS_END(ScopeReference,
  // {0101010D-0101-0D00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x0D00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Segment,
  true)
AAF_CLASS_SEPARATOR()

// Selector
//
AAF_CLASS(Selector,
  // {0101010D-0101-0E00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x0E00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Segment,
  true)
  AAF_PROPERTY(Selected,
    // {04010106-0209-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010104,
      0x0209, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0f01,
    AAF_REFERENCE_TYPE(StrongReference, Segment),
    true,
    false,
    Selector)
  AAF_PROPERTY(Alternates,
    // {04010106-0608-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010104,
      0x0608, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0f02,
    AAF_REFERENCE_TYPE(StrongReferenceVector, Segment),
    false,
    false,
    Selector)
AAF_CLASS_END(Selector,
  // {0101010D-0101-0E00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x0E00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Segment,
  true)
AAF_CLASS_SEPARATOR()

// Sequence
//
AAF_CLASS(Sequence,
  // {0101010D-0101-0F00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x0F00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Segment,
  true)
  AAF_PROPERTY(Components,
    // {04010106-0609-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010104,
      0x0609, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x1001,
    AAF_REFERENCE_TYPE(StrongReferenceVector, Component),
    true,
    false,
    Sequence)
AAF_CLASS_END(Sequence,
  // {0101010D-0101-0F00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x0F00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Segment,
  true)
AAF_CLASS_SEPARATOR()

// SourceReference
//
AAF_CLASS(SourceReference,
  // {0101010D-0101-1000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x1000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Segment,
  false)
  AAF_PROPERTY(SourceID,
    // {03010106-0100-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010103,
      0x0100, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x1101,
    AAF_TYPE(MobID),
    false,
    false,
    SourceReference)
  AAF_PROPERTY(SourceMobSlotID,
    // {03010106-0200-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010103,
      0x0200, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x1102,
    AAF_TYPE(UInt32),
    true,
    false,
    SourceReference)
AAF_CLASS_END(SourceReference,
  // {0101010D-0101-1000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x1000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Segment,
  false)
AAF_CLASS_SEPARATOR()

// SourceClip
//
AAF_CLASS(SourceClip,
  // {0101010D-0101-1100-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x1100,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  SourceReference,
  true)
  AAF_PROPERTY(StartTime,
    // {03010207-0104-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x07020103,
      0x0104, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x1201,
    AAF_TYPE(Position ),
    false,
    false,
    SourceClip)
  AAF_PROPERTY(FadeInLength,
    // {01020207-0105-0200-060E-2B3401010102}
    AAF_LITERAL_AUID(0x07020201,
      0x0105, 0x0200,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x1202,
    AAF_TYPE(Length),
    false,
    false,
    SourceClip)
  AAF_PROPERTY(FadeInType,
    // {01053005-0000-0000-060E-2B3401010101}
    AAF_LITERAL_AUID(0x05300501,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
    0x1203,
    AAF_TYPE(FadeType),
    false,
    false,
    SourceClip)
  AAF_PROPERTY(FadeOutLength,
    // {01020207-0105-0300-060E-2B3401010102}
    AAF_LITERAL_AUID(0x07020201,
      0x0105, 0x0300,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x1204,
    AAF_TYPE(Length),
    false,
    false,
    SourceClip)
  AAF_PROPERTY(FadeOutType,
    // {02053005-0000-0000-060E-2B3401010101}
    AAF_LITERAL_AUID(0x05300502,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
    0x1205,
    AAF_TYPE(FadeType),
    false,
    false,
    SourceClip)
AAF_CLASS_END(SourceClip,
  // {0101010D-0101-1100-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x1100,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  SourceReference,
  true)
AAF_CLASS_SEPARATOR()

// TextClip
//
AAF_CLASS(TextClip,
  // {0101010D-0101-1200-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x1200,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  SourceReference,
  false)
AAF_CLASS_END(TextClip,
  // {0101010D-0101-1200-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x1200,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  SourceReference,
  false)
AAF_CLASS_SEPARATOR()

// HTMLClip
//
AAF_CLASS(HTMLClip,
  // {0101010D-0101-1300-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x1300,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  TextClip,
  true)
  AAF_PROPERTY(BeginAnchor,
    // {01063005-0100-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x05300601,
      0x0100, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x1401,
    AAF_TYPE(String),
    false,
    false,
    HTMLClip)
  AAF_PROPERTY(EndAnchor,
    // {02063005-0100-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x05300602,
      0x0100, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x1402,
    AAF_TYPE(String),
    false,
    false,
    HTMLClip)
AAF_CLASS_END(HTMLClip,
  // {0101010D-0101-1300-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x1300,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  TextClip,
  true)
AAF_CLASS_SEPARATOR()

// Timecode
//
AAF_CLASS(Timecode,
  // {0101010D-0101-1400-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x1400,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Segment,
  true)
  AAF_PROPERTY(Start,
    // {03010207-0105-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x07020103,
      0x0105, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x1501,
    AAF_TYPE(Position),
    true,
    false,
    Timecode)
  AAF_PROPERTY(FPS,
    // {01010404-0206-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x04040101,
      0x0206, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x1502,
    AAF_TYPE(UInt16),
    true,
    false,
    Timecode)
  AAF_PROPERTY(Drop,
    // {01010404-0500-0000-060E-2B3401010101}
    AAF_LITERAL_AUID(0x04040101,
      0x0500, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
    0x1503,
    AAF_TYPE(Boolean),
    true,
    false,
    Timecode)
AAF_CLASS_END(Timecode,
  // {0101010D-0101-1400-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x1400,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Segment,
  true)
AAF_CLASS_SEPARATOR()

// TimecodeStream
//
AAF_CLASS(TimecodeStream,
  // {0101010D-0101-1500-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x1500,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Segment,
  false)
  AAF_PROPERTY(SampleRate,
    // {01010404-0201-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x04040101,
      0x0201, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x1601,
    AAF_TYPE(Rational),
    true,
    false,
    TimecodeStream)
  AAF_PROPERTY(Source,
    // {00030704-0000-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x04070300,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x1602,
    AAF_TYPE(Stream),
    true,
    false,
    TimecodeStream)
  AAF_PROPERTY(SourceType,
    // {01020404-0000-0000-060E-2B3401010101}
    AAF_LITERAL_AUID(0x04040201,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
    0x1603,
    AAF_TYPE(TCSource),
    true,
    false,
    TimecodeStream)
AAF_CLASS_END(TimecodeStream,
  // {0101010D-0101-1500-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x1500,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Segment,
  false)
AAF_CLASS_SEPARATOR()

// TimecodeStream12M
//
AAF_CLASS(TimecodeStream12M,
  // {0101010D-0101-1600-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x1600,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  TimecodeStream,
  true)
  AAF_PROPERTY(IncludeSync,
    // {01010404-0400-0000-060E-2B3401010101}
    AAF_LITERAL_AUID(0x04040101,
      0x0400, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
    0x1701,
    AAF_TYPE(Boolean),
    true,
    false,
    TimecodeStream12M)
AAF_CLASS_END(TimecodeStream12M,
  // {0101010D-0101-1600-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x1600,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  TimecodeStream,
  true)
AAF_CLASS_SEPARATOR()

// Transition
//
AAF_CLASS(Transition,
  // {0101010D-0101-1700-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x1700,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Component,
  true)
  AAF_PROPERTY(OperationGroup,
    // {04010106-0205-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010104,
      0x0205, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x1801,
    AAF_REFERENCE_TYPE(StrongReference, OperationGroup),
    true,
    false,
    Transition)
  AAF_PROPERTY(CutPoint,
    // {03010207-0106-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x07020103,
      0x0106, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x1802,
    AAF_TYPE(Position),
    true,
    false,
    Transition)
AAF_CLASS_END(Transition,
  // {0101010D-0101-1700-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x1700,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Component,
  true)
AAF_CLASS_SEPARATOR()

// ContentStorage
//
AAF_CLASS(ContentStorage,
  // {0101010D-0101-1800-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x1800,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  InterchangeObject,
  true)
  AAF_PROPERTY(Mobs,
    // {04010106-0501-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010104,
      0x0501, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x1901,
    AAF_REFERENCE_TYPE(StrongReferenceSet, Mob),
    true,
    false,
    ContentStorage)
  AAF_PROPERTY(EssenceData,
    // {04010106-0502-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010104,
      0x0502, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x1902,
    AAF_REFERENCE_TYPE(StrongReferenceSet, EssenceData),
    false,
    false,
    ContentStorage)
AAF_CLASS_END(ContentStorage,
  // {0101010D-0101-1800-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x1800,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  InterchangeObject,
  true)
AAF_CLASS_SEPARATOR()

// ControlPoint
//
AAF_CLASS(ControlPoint,
  // {0101010D-0101-1900-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x1900,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  InterchangeObject,
  true)
  AAF_PROPERTY(Value,
    // {0D053005-0000-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x0530050D,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x1a02,
    AAF_TYPE(Indirect),
    true,
    false,
    ControlPoint)
  AAF_PROPERTY(Time,
    // {03010207-1002-0100-060E-2B3401010102}
    AAF_LITERAL_AUID(0x07020103,
      0x1002, 0x0100,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x1a03,
    AAF_TYPE(Rational),
    true,
    false,
    ControlPoint)
  AAF_PROPERTY(EditHint,
    // {08053005-0000-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x05300508,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x1a04,
    AAF_TYPE(EditHintType),
    false,
    false,
    ControlPoint)
AAF_CLASS_END(ControlPoint,
  // {0101010D-0101-1900-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x1900,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  InterchangeObject,
  true)
AAF_CLASS_SEPARATOR()

// DefinitionObject
//
AAF_CLASS(DefinitionObject,
  // {0101010D-0101-1A00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x1A00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  InterchangeObject,
  false)
  AAF_PROPERTY(Identification,
    // {03150101-0000-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x01011503,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x1b01,
    AAF_TYPE(AUID),
    true,
    true,
    DefinitionObject)
  AAF_PROPERTY(Name,
    // {02010701-0301-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x01070102,
      0x0301, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x1b02,
    AAF_TYPE(String),
    true,
    false,
    DefinitionObject)
  AAF_PROPERTY(Description,
    // {01030203-0201-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x03020301,
      0x0201, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x1b03,
    AAF_TYPE(String),
    false,
    false,
    DefinitionObject)
AAF_CLASS_END(DefinitionObject,
  // {0101010D-0101-1A00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x1A00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  InterchangeObject,
  false)
AAF_CLASS_SEPARATOR()

// ClassDefinition
//
AAF_CLASS(ClassDefinition,
  // {0101010D-0201-0000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0201, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  MetaDefinition,
  true)
  AAF_PROPERTY(ParentClass,
    // {07010106-0100-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010107,
      0x0100, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0008,
    AAF_REFERENCE_TYPE(WeakReference, ClassDefinition),
    true,
    false,
    ClassDefinition)
  AAF_PROPERTY(Properties,
    // {07010106-0200-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010107,
      0x0200, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0009,
    AAF_REFERENCE_TYPE(StrongReferenceSet, PropertyDefinition),
    false,
    false,
    ClassDefinition)
  AAF_PROPERTY(IsConcrete,
    // {07010106-0300-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010107,
      0x0300, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x000A,
    AAF_TYPE(Boolean),
    true,
    false,
    ClassDefinition)
AAF_CLASS_END(ClassDefinition,
  // {0101010D-0201-0000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0201, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  MetaDefinition,
  true)
AAF_CLASS_SEPARATOR()

// DataDefinition
//
AAF_CLASS(DataDefinition,
  // {0101010D-0101-1B00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x1B00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  DefinitionObject,
  true)
AAF_CLASS_END(DataDefinition,
  // {0101010D-0101-1B00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x1B00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  DefinitionObject,
  true)
AAF_CLASS_SEPARATOR()

// OperationDefinition
//
AAF_CLASS(OperationDefinition,
  // {0101010D-0101-1C00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x1C00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  DefinitionObject,
  true)
  AAF_PROPERTY(DataDefinition,
    // {09053005-0000-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x05300509,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x1e01,
    AAF_REFERENCE_TYPE(WeakReference, DataDefinition),
    true,
    false,
    OperationDefinition)
  AAF_PROPERTY(IsTimeWarp,
    // {03053005-0000-0000-060E-2B3401010101}
    AAF_LITERAL_AUID(0x05300503,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
    0x1e02,
    AAF_TYPE(Boolean),
    false,
    false,
    OperationDefinition)
  AAF_PROPERTY(DegradeTo,
    // {04010106-0401-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010104,
      0x0401, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x1e03,
    AAF_REFERENCE_TYPE(WeakReferenceVector, OperationDefinition),
    false,
    false,
    OperationDefinition)
  AAF_PROPERTY(Category,
    // {0A053005-0000-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x0530050A,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x1e06,
    AAF_TYPE(AUID),
    false,
    false,
    OperationDefinition)
  AAF_PROPERTY(NumberInputs,
    // {04053005-0000-0000-060E-2B3401010101}
    AAF_LITERAL_AUID(0x05300504,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
    0x1e07,
    AAF_TYPE(Int32),
    true,
    false,
    OperationDefinition)
  AAF_PROPERTY(Bypass,
    // {05053005-0000-0000-060E-2B3401010101}
    AAF_LITERAL_AUID(0x05300505,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
    0x1e08,
    AAF_TYPE(UInt32),
    false,
    false,
    OperationDefinition)
  AAF_PROPERTY(ParametersDefined,
    // {04010106-0302-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010104,
      0x0302, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x1e09,
    AAF_REFERENCE_TYPE(WeakReferenceSet, ParameterDefinition),
    false,
    false,
    OperationDefinition)
AAF_CLASS_END(OperationDefinition,
  // {0101010D-0101-1C00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x1C00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  DefinitionObject,
  true)
AAF_CLASS_SEPARATOR()

// ParameterDefinition
//
AAF_CLASS(ParameterDefinition,
  // {0101010D-0101-1D00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x1D00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  DefinitionObject,
  true)
  AAF_PROPERTY(Type,
    // {04010106-0106-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010104,
      0x0106, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x1f01,
    AAF_REFERENCE_TYPE(WeakReference, TypeDefinition),
    true,
    false,
    ParameterDefinition)
  AAF_PROPERTY(DisplayUnits,
    // {0B053005-0100-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x0530050B,
      0x0100, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x1f03,
    AAF_TYPE(String),
    false,
    false,
    ParameterDefinition)
AAF_CLASS_END(ParameterDefinition,
  // {0101010D-0101-1D00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x1D00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  DefinitionObject,
  true)
AAF_CLASS_SEPARATOR()

// PropertyDefinition
//
AAF_CLASS(PropertyDefinition,
  // {0101010D-0202-0000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0202, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  MetaDefinition,
  true)
  AAF_PROPERTY(Type,
    // {07010106-0400-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010107,
      0x0400, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x000B,
    AAF_TYPE(AUID),
    true,
    false,
    PropertyDefinition)
  AAF_PROPERTY(IsOptional,
    // {02020103-0100-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x03010202,
      0x0100, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x000C,
    AAF_TYPE(Boolean),
    true,
    false,
    PropertyDefinition)
  AAF_PROPERTY(LocalIdentification,
    // {07010106-0500-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010107,
      0x0500, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x000D,
    AAF_TYPE(UInt16),
    true,
    false,
    PropertyDefinition)
  AAF_PROPERTY(IsUniqueIdentifier,
    // {07010106-0600-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010107,
      0x0600, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x000E,
    AAF_TYPE(Boolean),
    false,
    false,
    PropertyDefinition)
AAF_CLASS_END(PropertyDefinition,
  // {0101010D-0202-0000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0202, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  MetaDefinition,
  true)
AAF_CLASS_SEPARATOR()

// TypeDefinition
//
AAF_CLASS(TypeDefinition,
  // {0101010D-0203-0000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0203, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  MetaDefinition,
  false)
AAF_CLASS_END(TypeDefinition,
  // {0101010D-0203-0000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0203, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  MetaDefinition,
  false)
AAF_CLASS_SEPARATOR()

// PluginDefinition
//
AAF_CLASS(PluginDefinition,
  // {0101010D-0101-1E00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x1E00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  DefinitionObject,
  true)
  AAF_PROPERTY(CategoryClass,
    // {01092005-0000-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x05200901,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x2203,
    AAF_TYPE(AUID),
    true,
    false,
    PluginDefinition)
  AAF_PROPERTY(VersionNumber,
    // {01030303-0300-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x03030301,
      0x0300, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x2204,
    AAF_TYPE(VersionType),
    true,
    false,
    PluginDefinition)
  AAF_PROPERTY(VersionString,
    // {01030303-0201-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x03030301,
      0x0201, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x2205,
    AAF_TYPE(String),
    false,
    false,
    PluginDefinition)
  AAF_PROPERTY(Manufacturer,
    // {01010A01-0101-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x010A0101,
      0x0101, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x2206,
    AAF_TYPE(String),
    false,
    false,
    PluginDefinition)
  AAF_PROPERTY(ManufacturerInfo,
    // {04010106-020B-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010104,
      0x020B, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x2207,
    AAF_REFERENCE_TYPE(StrongReference, NetworkLocator),
    false,
    false,
    PluginDefinition)
  AAF_PROPERTY(ManufacturerID,
    // {01010A01-0300-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x010A0101,
      0x0300, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x2208,
    AAF_TYPE(AUID),
    false,
    false,
    PluginDefinition)
  AAF_PROPERTY(Platform,
    // {02092005-0000-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x05200902,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x2209,
    AAF_TYPE(AUID),
    false,
    false,
    PluginDefinition)
  AAF_PROPERTY(MinPlatformVersion,
    // {03092005-0000-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x05200903,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x220a,
    AAF_TYPE(VersionType),
    false,
    false,
    PluginDefinition)
  AAF_PROPERTY(MaxPlatformVersion,
    // {04092005-0000-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x05200904,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x220b,
    AAF_TYPE(VersionType),
    false,
    false,
    PluginDefinition)
  AAF_PROPERTY(Engine,
    // {05092005-0000-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x05200905,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x220c,
    AAF_TYPE(AUID),
    false,
    false,
    PluginDefinition)
  AAF_PROPERTY(MinEngineVersion,
    // {06092005-0000-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x05200906,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x220d,
    AAF_TYPE(VersionType),
    false,
    false,
    PluginDefinition)
  AAF_PROPERTY(MaxEngineVersion,
    // {07092005-0000-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x05200907,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x220e,
    AAF_TYPE(VersionType),
    false,
    false,
    PluginDefinition)
  AAF_PROPERTY(PluginAPI,
    // {08092005-0000-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x05200908,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x220f,
    AAF_TYPE(AUID),
    false,
    false,
    PluginDefinition)
  AAF_PROPERTY(MinPluginAPI,
    // {09092005-0000-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x05200909,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x2210,
    AAF_TYPE(VersionType),
    false,
    false,
    PluginDefinition)
  AAF_PROPERTY(MaxPluginAPI,
    // {0A092005-0000-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x0520090A,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x2211,
    AAF_TYPE(VersionType),
    false,
    false,
    PluginDefinition)
  AAF_PROPERTY(SoftwareOnly,
    // {0B092005-0000-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x0520090B,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x2212,
    AAF_TYPE(Boolean),
    false,
    false,
    PluginDefinition)
  AAF_PROPERTY(Accelerator,
    // {0C092005-0000-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x0520090C,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x2213,
    AAF_TYPE(Boolean),
    false,
    false,
    PluginDefinition)
  AAF_PROPERTY(Locators,
    // {0D092005-0000-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x0520090D,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x2214,
    AAF_REFERENCE_TYPE(StrongReferenceVector, Locator),
    false,
    false,
    PluginDefinition)
  AAF_PROPERTY(Authentication,
    // {0E092005-0000-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x0520090E,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x2215,
    AAF_TYPE(Boolean),
    false,
    false,
    PluginDefinition)
  AAF_PROPERTY(DefinitionObject,
    // {0F092005-0000-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x0520090F,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x2216,
    AAF_TYPE(AUID),
    false,
    false,
    PluginDefinition)
AAF_CLASS_END(PluginDefinition,
  // {0101010D-0101-1E00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x1E00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  DefinitionObject,
  true)
AAF_CLASS_SEPARATOR()

// CodecDefinition
//
AAF_CLASS(CodecDefinition,
  // {0101010D-0101-1F00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x1F00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  DefinitionObject,
  true)
  AAF_PROPERTY(FileDescriptorClass,
    // {04010106-0107-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010104,
      0x0107, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x2301,
    AAF_REFERENCE_TYPE(WeakReference, ClassDefinition),
    true,
    false,
    CodecDefinition)
  AAF_PROPERTY(DataDefinitions,
    // {04010106-0301-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010104,
      0x0301, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x2302,
    AAF_REFERENCE_TYPE(WeakReferenceVector, DataDefinition),
    true,
    false,
    CodecDefinition)
AAF_CLASS_END(CodecDefinition,
  // {0101010D-0101-1F00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x1F00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  DefinitionObject,
  true)
AAF_CLASS_SEPARATOR()

// ContainerDefinition
//
AAF_CLASS(ContainerDefinition,
  // {0101010D-0101-2000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x2000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  DefinitionObject,
  true)
  AAF_PROPERTY(EssenceIsIdentified,
    // {01020103-0300-0000-060E-2B3401010101}
    AAF_LITERAL_AUID(0x03010201,
      0x0300, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
    0x2401,
    AAF_TYPE(Boolean),
    false,
    false,
    ContainerDefinition)
AAF_CLASS_END(ContainerDefinition,
  // {0101010D-0101-2000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x2000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  DefinitionObject,
  true)
AAF_CLASS_SEPARATOR()

// InterpolationDefinition
//
AAF_CLASS(InterpolationDefinition,
  // {0101010D-0101-2100-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x2100,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  DefinitionObject,
  true)
AAF_CLASS_END(InterpolationDefinition,
  // {0101010D-0101-2100-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x2100,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  DefinitionObject,
  true)
AAF_CLASS_SEPARATOR()

// Dictionary
//
AAF_CLASS(Dictionary,
  // {0101010D-0101-2200-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x2200,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  InterchangeObject,
  true)
  AAF_PROPERTY(OperationDefinitions,
    // {04010106-0503-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010104,
      0x0503, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x2603,
    AAF_REFERENCE_TYPE(StrongReferenceSet, OperationDefinition),
    false,
    false,
    Dictionary)
  AAF_PROPERTY(ParameterDefinitions,
    // {04010106-0504-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010104,
      0x0504, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x2604,
    AAF_REFERENCE_TYPE(StrongReferenceSet, ParameterDefinition),
    false,
    false,
    Dictionary)
  AAF_PROPERTY(DataDefinitions,
    // {04010106-0505-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010104,
      0x0505, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x2605,
    AAF_REFERENCE_TYPE(StrongReferenceSet, DataDefinition),
    false,
    false,
    Dictionary)
  AAF_PROPERTY(PluginDefinitions,
    // {04010106-0506-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010104,
      0x0506, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x2606,
    AAF_REFERENCE_TYPE(StrongReferenceSet, PluginDefinition),
    false,
    false,
    Dictionary)
  AAF_PROPERTY(CodecDefinitions,
    // {04010106-0507-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010104,
      0x0507, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x2607,
    AAF_REFERENCE_TYPE(StrongReferenceSet, CodecDefinition),
    false,
    false,
    Dictionary)
  AAF_PROPERTY(ContainerDefinitions,
    // {04010106-0508-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010104,
      0x0508, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x2608,
    AAF_REFERENCE_TYPE(StrongReferenceSet, ContainerDefinition),
    false,
    false,
    Dictionary)
  AAF_PROPERTY(InterpolationDefinitions,
    // {04010106-0509-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010104,
      0x0509, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x2609,
    AAF_REFERENCE_TYPE(StrongReferenceSet, InterpolationDefinition),
    false,
    false,
    Dictionary)
AAF_CLASS_END(Dictionary,
  // {0101010D-0101-2200-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x2200,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  InterchangeObject,
  true)
AAF_CLASS_SEPARATOR()

// EssenceData
//
AAF_CLASS(EssenceData,
  // {0101010D-0101-2300-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x2300,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  InterchangeObject,
  true)
  AAF_PROPERTY(MobID,
    // {06010106-0100-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010106,
      0x0100, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x2701,
    AAF_TYPE(MobID),
    true,
    true,
    EssenceData)
  AAF_PROPERTY(Data,
    // {00020704-0000-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x04070200,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x2702,
    AAF_TYPE(Stream),
    true,
    false,
    EssenceData)
  AAF_PROPERTY(SampleIndex,
    // {02010106-0100-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010102,
      0x0100, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x2b01,
    AAF_TYPE(Stream),
    false,
    false,
    EssenceData)
AAF_CLASS_END(EssenceData,
  // {0101010D-0101-2300-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x2300,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  InterchangeObject,
  true)
AAF_CLASS_SEPARATOR()

// EssenceDescriptor
//
AAF_CLASS(EssenceDescriptor,
  // {0101010D-0101-2400-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x2400,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  InterchangeObject,
  false)
  AAF_PROPERTY(Locator,
    // {04010106-0603-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010104,
      0x0603, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x2f01,
    AAF_REFERENCE_TYPE(StrongReferenceVector, Locator),
    false,
    false,
    EssenceDescriptor)
AAF_CLASS_END(EssenceDescriptor,
  // {0101010D-0101-2400-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x2400,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  InterchangeObject,
  false)
AAF_CLASS_SEPARATOR()

// FileDescriptor
//
AAF_CLASS(FileDescriptor,
  // {0101010D-0101-2500-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x2500,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  EssenceDescriptor,
  false)
  AAF_PROPERTY(SampleRate,
    // {01010604-0000-0000-060E-2B3401010101}
    AAF_LITERAL_AUID(0x04060101,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
    0x3001,
    AAF_TYPE(Rational),
    true,
    false,
    FileDescriptor)
  AAF_PROPERTY(Length,
    // {02010604-0000-0000-060E-2B3401010101}
    AAF_LITERAL_AUID(0x04060102,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
    0x3002,
    AAF_TYPE(Length),
    true,
    false,
    FileDescriptor)
  AAF_PROPERTY(ContainerFormat,
    // {04010106-0102-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010104,
      0x0102, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3004,
    AAF_REFERENCE_TYPE(WeakReference, ContainerDefinition),
    false,
    false,
    FileDescriptor)
  AAF_PROPERTY(CodecDefinition,
    // {04010106-0103-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010104,
      0x0103, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3005,
    AAF_REFERENCE_TYPE(WeakReference, CodecDefinition),
    false,
    false,
    FileDescriptor)
AAF_CLASS_END(FileDescriptor,
  // {0101010D-0101-2500-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x2500,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  EssenceDescriptor,
  false)
AAF_CLASS_SEPARATOR()

// AIFCDescriptor
//
AAF_CLASS(AIFCDescriptor,
  // {0101010D-0101-2600-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x2600,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  FileDescriptor,
  true)
  AAF_PROPERTY(Summary,
    // {02030303-0200-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x03030302,
      0x0200, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3101,
    AAF_TYPE(DataValue),
    true,
    false,
    AIFCDescriptor)
AAF_CLASS_END(AIFCDescriptor,
  // {0101010D-0101-2600-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x2600,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  FileDescriptor,
  true)
AAF_CLASS_SEPARATOR()

// DigitalImageDescriptor
//
AAF_CLASS(DigitalImageDescriptor,
  // {0101010D-0101-2700-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x2700,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  FileDescriptor,
  false)
  AAF_PROPERTY(Compression,
    // {01060104-0000-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x04010601,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3201,
    AAF_TYPE(AUID),
    false,
    false,
    DigitalImageDescriptor)
  AAF_PROPERTY(StoredHeight,
    // {02050104-0100-0000-060E-2B3401010101}
    AAF_LITERAL_AUID(0x04010502,
      0x0100, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
    0x3202,
    AAF_TYPE(UInt32),
    true,
    false,
    DigitalImageDescriptor)
  AAF_PROPERTY(StoredWidth,
    // {02050104-0200-0000-060E-2B3401010101}
    AAF_LITERAL_AUID(0x04010502,
      0x0200, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
    0x3203,
    AAF_TYPE(UInt32),
    true,
    false,
    DigitalImageDescriptor)
  AAF_PROPERTY(SampledHeight,
    // {01050104-0700-0000-060E-2B3401010101}
    AAF_LITERAL_AUID(0x04010501,
      0x0700, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
    0x3204,
    AAF_TYPE(UInt32),
    false,
    false,
    DigitalImageDescriptor)
  AAF_PROPERTY(SampledWidth,
    // {01050104-0800-0000-060E-2B3401010101}
    AAF_LITERAL_AUID(0x04010501,
      0x0800, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
    0x3205,
    AAF_TYPE(UInt32),
    false,
    false,
    DigitalImageDescriptor)
  AAF_PROPERTY(SampledXOffset,
    // {01050104-0900-0000-060E-2B3401010101}
    AAF_LITERAL_AUID(0x04010501,
      0x0900, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
    0x3206,
    AAF_TYPE(Int32),
    false,
    false,
    DigitalImageDescriptor)
  AAF_PROPERTY(SampledYOffset,
    // {01050104-0A00-0000-060E-2B3401010101}
    AAF_LITERAL_AUID(0x04010501,
      0x0A00, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
    0x3207,
    AAF_TYPE(Int32),
    false,
    false,
    DigitalImageDescriptor)
  AAF_PROPERTY(DisplayHeight,
    // {01050104-0B00-0000-060E-2B3401010101}
    AAF_LITERAL_AUID(0x04010501,
      0x0B00, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
    0x3208,
    AAF_TYPE(UInt32),
    false,
    false,
    DigitalImageDescriptor)
  AAF_PROPERTY(DisplayWidth,
    // {01050104-0C00-0000-060E-2B3401010101}
    AAF_LITERAL_AUID(0x04010501,
      0x0C00, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
    0x3209,
    AAF_TYPE(UInt32),
    false,
    false,
    DigitalImageDescriptor)
  AAF_PROPERTY(DisplayXOffset,
    // {01050104-0D00-0000-060E-2B3401010101}
    AAF_LITERAL_AUID(0x04010501,
      0x0D00, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
    0x320a,
    AAF_TYPE(Int32),
    false,
    false,
    DigitalImageDescriptor)
  AAF_PROPERTY(DisplayYOffset,
    // {01050104-0E00-0000-060E-2B3401010101}
    AAF_LITERAL_AUID(0x04010501,
      0x0E00, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
    0x320b,
    AAF_TYPE(Int32),
    false,
    false,
    DigitalImageDescriptor)
  AAF_PROPERTY(FrameLayout,
    // {01030104-0400-0000-060E-2B3401010101}
    AAF_LITERAL_AUID(0x04010301,
      0x0400, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
    0x320c,
    AAF_TYPE(LayoutType),
    true,
    false,
    DigitalImageDescriptor)
  AAF_PROPERTY(VideoLineMap,
    // {02030104-0500-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x04010302,
      0x0500, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x320d,
    AAF_TYPE(Int32Array),
    true,
    false,
    DigitalImageDescriptor)
  AAF_PROPERTY(ImageAspectRatio,
    // {01010104-0100-0000-060E-2B3401010101}
    AAF_LITERAL_AUID(0x04010101,
      0x0100, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
    0x320e,
    AAF_TYPE(Rational),
    true,
    false,
    DigitalImageDescriptor)
  AAF_PROPERTY(AlphaTransparency,
    // {02012005-0000-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x05200102,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x320f,
    AAF_TYPE(AlphaTransparency),
    false,
    false,
    DigitalImageDescriptor)
  AAF_PROPERTY(Gamma,
    // {01020104-0101-0200-060E-2B3401010102}
    AAF_LITERAL_AUID(0x04010201,
      0x0101, 0x0200,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3210,
    AAF_TYPE(AUID),
    false,
    false,
    DigitalImageDescriptor)
  AAF_PROPERTY(ImageAlignmentFactor,
    // {01011804-0000-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x04180101,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3211,
    AAF_TYPE(UInt32),
    false,
    false,
    DigitalImageDescriptor)
  AAF_PROPERTY(FieldDominance,
    // {01030104-0600-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x04010301,
      0x0600, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3212,
    AAF_TYPE(FieldNumber),
    false,
    false,
    DigitalImageDescriptor)
  AAF_PROPERTY(FieldStartOffset,
    // {02011804-0000-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x04180102,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3213,
    AAF_TYPE(UInt32),
    false,
    false,
    DigitalImageDescriptor)
  AAF_PROPERTY(FieldEndOffset,
    // {03011804-0000-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x04180103,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3214,
    AAF_TYPE(UInt32),
    false,
    false,
    DigitalImageDescriptor)
AAF_CLASS_END(DigitalImageDescriptor,
  // {0101010D-0101-2700-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x2700,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  FileDescriptor,
  false)
AAF_CLASS_SEPARATOR()

// CDCIDescriptor
//
AAF_CLASS(CDCIDescriptor,
  // {0101010D-0101-2800-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x2800,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  DigitalImageDescriptor,
  true)
  AAF_PROPERTY(ComponentWidth,
    // {03050104-0A00-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x04010503,
      0x0A00, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3301,
    AAF_TYPE(UInt32),
    true,
    false,
    CDCIDescriptor)
  AAF_PROPERTY(HorizontalSubsampling,
    // {01050104-0500-0000-060E-2B3401010101}
    AAF_LITERAL_AUID(0x04010501,
      0x0500, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
    0x3302,
    AAF_TYPE(UInt32),
    true,
    false,
    CDCIDescriptor)
  AAF_PROPERTY(ColorSiting,
    // {01050104-0600-0000-060E-2B3401010101}
    AAF_LITERAL_AUID(0x04010501,
      0x0600, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
    0x3303,
    AAF_TYPE(ColorSitingType),
    false,
    false,
    CDCIDescriptor)
  AAF_PROPERTY(BlackReferenceLevel,
    // {03050104-0300-0000-060E-2B3401010101}
    AAF_LITERAL_AUID(0x04010503,
      0x0300, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
    0x3304,
    AAF_TYPE(UInt32),
    false,
    false,
    CDCIDescriptor)
  AAF_PROPERTY(WhiteReferenceLevel,
    // {03050104-0400-0000-060E-2B3401010101}
    AAF_LITERAL_AUID(0x04010503,
      0x0400, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
    0x3305,
    AAF_TYPE(UInt32),
    false,
    false,
    CDCIDescriptor)
  AAF_PROPERTY(ColorRange,
    // {03050104-0500-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x04010503,
      0x0500, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3306,
    AAF_TYPE(UInt32),
    false,
    false,
    CDCIDescriptor)
  AAF_PROPERTY(PaddingBits,
    // {04011804-0000-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x04180104,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3307,
    AAF_TYPE(Int16),
    false,
    false,
    CDCIDescriptor)
  AAF_PROPERTY(VerticalSubsampling,
    // {01050104-1000-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x04010501,
      0x1000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3308,
    AAF_TYPE(UInt32),
    false,
    false,
    CDCIDescriptor)
  AAF_PROPERTY(AlphaSamplingWidth,
    // {03050104-0700-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x04010503,
      0x0700, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3309,
    AAF_TYPE(UInt32),
    false,
    false,
    CDCIDescriptor)
AAF_CLASS_END(CDCIDescriptor,
  // {0101010D-0101-2800-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x2800,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  DigitalImageDescriptor,
  true)
AAF_CLASS_SEPARATOR()

// RGBADescriptor
//
AAF_CLASS(RGBADescriptor,
  // {0101010D-0101-2900-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x2900,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  DigitalImageDescriptor,
  true)
  AAF_PROPERTY(PixelLayout,
    // {03050104-0600-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x04010503,
      0x0600, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3401,
    AAF_TYPE(RGBALayout),
    true,
    false,
    RGBADescriptor)
  AAF_PROPERTY(Palette,
    // {03050104-0800-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x04010503,
      0x0800, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3403,
    AAF_TYPE(DataValue),
    false,
    false,
    RGBADescriptor)
  AAF_PROPERTY(PaletteLayout,
    // {03050104-0900-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x04010503,
      0x0900, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3404,
    AAF_TYPE(RGBALayout),
    false,
    false,
    RGBADescriptor)
AAF_CLASS_END(RGBADescriptor,
  // {0101010D-0101-2900-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x2900,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  DigitalImageDescriptor,
  true)
AAF_CLASS_SEPARATOR()

// HTMLDescriptor
//
AAF_CLASS(HTMLDescriptor,
  // {0101010D-0101-2A00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x2A00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  FileDescriptor,
  true)
AAF_CLASS_END(HTMLDescriptor,
  // {0101010D-0101-2A00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x2A00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  FileDescriptor,
  true)
AAF_CLASS_SEPARATOR()

// TIFFDescriptor
//
AAF_CLASS(TIFFDescriptor,
  // {0101010D-0101-2B00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x2B00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  FileDescriptor,
  true)
  AAF_PROPERTY(IsUniform,
    // {03010205-0101-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x05020103,
      0x0101, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3701,
    AAF_TYPE(Boolean),
    true,
    false,
    TIFFDescriptor)
  AAF_PROPERTY(IsContiguous,
    // {01020806-0000-0000-060E-2B3401010101}
    AAF_LITERAL_AUID(0x06080201,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
    0x3702,
    AAF_TYPE(Boolean),
    true,
    false,
    TIFFDescriptor)
  AAF_PROPERTY(LeadingLines,
    // {02030104-0300-0000-060E-2B3401010101}
    AAF_LITERAL_AUID(0x04010302,
      0x0300, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
    0x3703,
    AAF_TYPE(Int32),
    false,
    false,
    TIFFDescriptor)
  AAF_PROPERTY(TrailingLines,
    // {02030104-0400-0000-060E-2B3401010101}
    AAF_LITERAL_AUID(0x04010302,
      0x0400, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
    0x3704,
    AAF_TYPE(Int32),
    false,
    false,
    TIFFDescriptor)
  AAF_PROPERTY(JPEGTableID,
    // {03010205-0102-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x05020103,
      0x0102, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3705,
    AAF_TYPE(JPEGTableIDType),
    false,
    false,
    TIFFDescriptor)
  AAF_PROPERTY(Summary,
    // {02030303-0300-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x03030302,
      0x0300, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3706,
    AAF_TYPE(DataValue),
    true,
    false,
    TIFFDescriptor)
AAF_CLASS_END(TIFFDescriptor,
  // {0101010D-0101-2B00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x2B00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  FileDescriptor,
  true)
AAF_CLASS_SEPARATOR()

// WAVEDescriptor
//
AAF_CLASS(WAVEDescriptor,
  // {0101010D-0101-2C00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x2C00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  FileDescriptor,
  true)
  AAF_PROPERTY(Summary,
    // {02030303-0100-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x03030302,
      0x0100, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3801,
    AAF_TYPE(DataValue),
    true,
    false,
    WAVEDescriptor)
AAF_CLASS_END(WAVEDescriptor,
  // {0101010D-0101-2C00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x2C00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  FileDescriptor,
  true)
AAF_CLASS_SEPARATOR()

// FilmDescriptor
//
AAF_CLASS(FilmDescriptor,
  // {0101010D-0101-2D00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x2D00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  EssenceDescriptor,
  true)
  AAF_PROPERTY(FilmFormat,
    // {03011004-0108-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x04100103,
      0x0108, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3901,
    AAF_TYPE(FilmType),
    false,
    false,
    FilmDescriptor)
  AAF_PROPERTY(FrameRate,
    // {02080104-0300-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x04010802,
      0x0300, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3902,
    AAF_TYPE(UInt32),
    false,
    false,
    FilmDescriptor)
  AAF_PROPERTY(PerforationsPerFrame,
    // {03011004-0103-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x04100103,
      0x0103, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3903,
    AAF_TYPE(UInt8),
    false,
    false,
    FilmDescriptor)
  AAF_PROPERTY(FilmAspectRatio,
    // {03011004-0203-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x04100103,
      0x0203, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3904,
    AAF_TYPE(Rational),
    false,
    false,
    FilmDescriptor)
  AAF_PROPERTY(Manufacturer,
    // {03011004-0106-0100-060E-2B3401010102}
    AAF_LITERAL_AUID(0x04100103,
      0x0106, 0x0100,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3905,
    AAF_TYPE(String),
    false,
    false,
    FilmDescriptor)
  AAF_PROPERTY(Model,
    // {03011004-0105-0100-060E-2B3401010102}
    AAF_LITERAL_AUID(0x04100103,
      0x0105, 0x0100,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3906,
    AAF_TYPE(String),
    false,
    false,
    FilmDescriptor)
  AAF_PROPERTY(FilmGaugeFormat,
    // {03011004-0104-0100-060E-2B3401010102}
    AAF_LITERAL_AUID(0x04100103,
      0x0104, 0x0100,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3907,
    AAF_TYPE(String),
    false,
    false,
    FilmDescriptor)
  AAF_PROPERTY(FilmBatchNumber,
    // {03011004-0107-0100-060E-2B3401010102}
    AAF_LITERAL_AUID(0x04100103,
      0x0107, 0x0100,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3908,
    AAF_TYPE(String),
    false,
    false,
    FilmDescriptor)
AAF_CLASS_END(FilmDescriptor,
  // {0101010D-0101-2D00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x2D00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  EssenceDescriptor,
  true)
AAF_CLASS_SEPARATOR()

// TapeDescriptor
//
AAF_CLASS(TapeDescriptor,
  // {0101010D-0101-2E00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x2E00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  EssenceDescriptor,
  true)
  AAF_PROPERTY(FormFactor,
    // {01011004-0101-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x04100101,
      0x0101, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3a01,
    AAF_TYPE(TapeCaseType),
    false,
    false,
    TapeDescriptor)
  AAF_PROPERTY(VideoSignal,
    // {01040104-0100-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x04010401,
      0x0100, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3a02,
    AAF_TYPE(VideoSignalType),
    false,
    false,
    TapeDescriptor)
  AAF_PROPERTY(TapeFormat,
    // {0101010D-0101-0100-060E-2B3401010102}
    AAF_LITERAL_AUID(0x0D010101,
      0x0101, 0x0100,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3a03,
    AAF_TYPE(TapeFormatType),
    false,
    false,
    TapeDescriptor)
  AAF_PROPERTY(Length,
    // {01011004-0300-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x04100101,
      0x0300, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3a04,
    AAF_TYPE(UInt32),
    false,
    false,
    TapeDescriptor)
  AAF_PROPERTY(ManufacturerID,
    // {01011004-0401-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x04100101,
      0x0401, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3a05,
    AAF_TYPE(String),
    false,
    false,
    TapeDescriptor)
  AAF_PROPERTY(Model,
    // {01011004-0201-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x04100101,
      0x0201, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3a06,
    AAF_TYPE(String),
    false,
    false,
    TapeDescriptor)
  AAF_PROPERTY(TapeBatchNumber,
    // {01011004-0601-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x04100101,
      0x0601, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3a07,
    AAF_TYPE(String),
    false,
    false,
    TapeDescriptor)
  AAF_PROPERTY(TapeStock,
    // {01011004-0501-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x04100101,
      0x0501, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3a08,
    AAF_TYPE(String),
    false,
    false,
    TapeDescriptor)
AAF_CLASS_END(TapeDescriptor,
  // {0101010D-0101-2E00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x2E00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  EssenceDescriptor,
  true)
AAF_CLASS_SEPARATOR()

// Header
//
AAF_CLASS(Header,
  // {0101010D-0101-2F00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x2F00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  InterchangeObject,
  true)
  AAF_PROPERTY(ByteOrder,
    // {01020103-0200-0000-060E-2B3401010101}
    AAF_LITERAL_AUID(0x03010201,
      0x0200, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
    0x3b01,
    AAF_TYPE(Int16),
    true,
    false,
    Header)
  AAF_PROPERTY(LastModified,
    // {10010207-0204-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x07020110,
      0x0204, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3b02,
    AAF_TYPE(TimeStamp),
    true,
    false,
    Header)
  AAF_PROPERTY(Content,
    // {04010106-0201-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010104,
      0x0201, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3b03,
    AAF_REFERENCE_TYPE(StrongReference, ContentStorage),
    true,
    false,
    Header)
  AAF_PROPERTY(Dictionary,
    // {04010106-0202-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010104,
      0x0202, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3b04,
    AAF_REFERENCE_TYPE(StrongReference, Dictionary),
    true,
    false,
    Header)
  AAF_PROPERTY(Version,
    // {01020103-0500-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x03010201,
      0x0500, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3b05,
    AAF_TYPE(VersionType),
    true,
    false,
    Header)
  AAF_PROPERTY(IdentificationList,
    // {04010106-0604-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010104,
      0x0604, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3b06,
    AAF_REFERENCE_TYPE(StrongReferenceVector, Identification),
    true,
    false,
    Header)
  AAF_PROPERTY(ObjectModelVersion,
    // {01020103-0400-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x03010201,
      0x0400, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3b07,
    AAF_TYPE(UInt32),
    false,
    false,
    Header)
AAF_CLASS_END(Header,
  // {0101010D-0101-2F00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x2F00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  InterchangeObject,
  true)
AAF_CLASS_SEPARATOR()

// Identification
//
AAF_CLASS(Identification,
  // {0101010D-0101-3000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x3000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  InterchangeObject,
  true)
  AAF_PROPERTY(CompanyName,
    // {01072005-0201-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x05200701,
      0x0201, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3c01,
    AAF_TYPE(String),
    true,
    false,
    Identification)
  AAF_PROPERTY(ProductName,
    // {01072005-0301-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x05200701,
      0x0301, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3c02,
    AAF_TYPE(String),
    true,
    false,
    Identification)
  AAF_PROPERTY(ProductVersion,
    // {01072005-0400-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x05200701,
      0x0400, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3c03,
    AAF_TYPE(ProductVersion),
    false,
    false,
    Identification)
  AAF_PROPERTY(ProductVersionString,
    // {01072005-0501-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x05200701,
      0x0501, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3c04,
    AAF_TYPE(String),
    true,
    false,
    Identification)
  AAF_PROPERTY(ProductID,
    // {01072005-0700-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x05200701,
      0x0700, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3c05,
    AAF_TYPE(AUID),
    true,
    false,
    Identification)
  AAF_PROPERTY(Date,
    // {10010207-0203-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x07020110,
      0x0203, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3c06,
    AAF_TYPE(TimeStamp),
    true,
    false,
    Identification)
  AAF_PROPERTY(ToolkitVersion,
    // {01072005-0A00-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x05200701,
      0x0A00, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3c07,
    AAF_TYPE(ProductVersion),
    false,
    false,
    Identification)
  AAF_PROPERTY(Platform,
    // {01072005-0601-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x05200701,
      0x0601, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3c08,
    AAF_TYPE(String),
    false,
    false,
    Identification)
  AAF_PROPERTY(GenerationAUID,
    // {01072005-0100-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x05200701,
      0x0100, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x3c09,
    AAF_TYPE(AUID),
    true,
    false,
    Identification)
AAF_CLASS_END(Identification,
  // {0101010D-0101-3000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x3000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  InterchangeObject,
  true)
AAF_CLASS_SEPARATOR()

// Locator
//
AAF_CLASS(Locator,
  // {0101010D-0101-3100-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x3100,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  InterchangeObject,
  false)
AAF_CLASS_END(Locator,
  // {0101010D-0101-3100-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x3100,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  InterchangeObject,
  false)
AAF_CLASS_SEPARATOR()

// NetworkLocator
//
AAF_CLASS(NetworkLocator,
  // {0101010D-0101-3200-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x3200,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Locator,
  true)
  AAF_PROPERTY(URLString,
    // {01010201-0100-0000-060E-2B3401010101}
    AAF_LITERAL_AUID(0x01020101,
      0x0100, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
    0x4001,
    AAF_TYPE(String),
    true,
    false,
    NetworkLocator)
AAF_CLASS_END(NetworkLocator,
  // {0101010D-0101-3200-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x3200,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Locator,
  true)
AAF_CLASS_SEPARATOR()

// TextLocator
//
AAF_CLASS(TextLocator,
  // {0101010D-0101-3300-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x3300,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Locator,
  true)
  AAF_PROPERTY(Name,
    // {02010401-0100-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x01040102,
      0x0100, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x4101,
    AAF_TYPE(String),
    true,
    false,
    TextLocator)
AAF_CLASS_END(TextLocator,
  // {0101010D-0101-3300-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x3300,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Locator,
  true)
AAF_CLASS_SEPARATOR()

// Mob
//
AAF_CLASS(Mob,
  // {0101010D-0101-3400-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x3400,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  InterchangeObject,
  false)
  AAF_PROPERTY(MobID,
    // {10150101-0000-0000-060E-2B3401010101}
    AAF_LITERAL_AUID(0x01011510,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
    0x4401,
    AAF_TYPE(MobID),
    true,
    true,
    Mob)
  AAF_PROPERTY(Name,
    // {02030301-0100-0000-060E-2B3401010101}
    AAF_LITERAL_AUID(0x01030302,
      0x0100, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
    0x4402,
    AAF_TYPE(String),
    false,
    false,
    Mob)
  AAF_PROPERTY(Slots,
    // {04010106-0605-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010104,
      0x0605, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x4403,
    AAF_REFERENCE_TYPE(StrongReferenceVector, MobSlot),
    true,
    false,
    Mob)
  AAF_PROPERTY(LastModified,
    // {10010207-0205-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x07020110,
      0x0205, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x4404,
    AAF_TYPE(TimeStamp),
    true,
    false,
    Mob)
  AAF_PROPERTY(CreationTime,
    // {10010207-0103-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x07020110,
      0x0103, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x4405,
    AAF_TYPE(TimeStamp),
    true,
    false,
    Mob)
  AAF_PROPERTY(UserComments,
    // {02010203-0C00-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x03020102,
      0x0C00, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x4406,
    AAF_REFERENCE_TYPE(StrongReferenceVector, TaggedValue),
    false,
    false,
    Mob)
  AAF_PROPERTY(KLVData,
    // {10020103-0300-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x03010210,
      0x0300, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x4407,
    AAF_REFERENCE_TYPE(StrongReferenceVector, KLVData),
    false,
    false,
    Mob)
AAF_CLASS_END(Mob,
  // {0101010D-0101-3400-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x3400,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  InterchangeObject,
  false)
AAF_CLASS_SEPARATOR()

// CompositionMob
//
AAF_CLASS(CompositionMob,
  // {0101010D-0101-3500-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x3500,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Mob,
  true)
  AAF_PROPERTY(DefaultFadeLength,
    // {01020207-0105-0100-060E-2B3401010102}
    AAF_LITERAL_AUID(0x07020201,
      0x0105, 0x0100,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x4501,
    AAF_TYPE(Length),
    false,
    false,
    CompositionMob)
  AAF_PROPERTY(DefFadeType,
    // {01023005-0000-0000-060E-2B3401010101}
    AAF_LITERAL_AUID(0x05300201,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x01),
    0x4502,
    AAF_TYPE(FadeType),
    false,
    false,
    CompositionMob)
  AAF_PROPERTY(DefFadeEditUnit,
    // {03043005-0000-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x05300403,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x4503,
    AAF_TYPE(Rational),
    false,
    false,
    CompositionMob)
AAF_CLASS_END(CompositionMob,
  // {0101010D-0101-3500-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x3500,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Mob,
  true)
AAF_CLASS_SEPARATOR()

// MasterMob
//
AAF_CLASS(MasterMob,
  // {0101010D-0101-3600-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x3600,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Mob,
  true)
AAF_CLASS_END(MasterMob,
  // {0101010D-0101-3600-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x3600,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Mob,
  true)
AAF_CLASS_SEPARATOR()

// SourceMob
//
AAF_CLASS(SourceMob,
  // {0101010D-0101-3700-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x3700,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Mob,
  true)
  AAF_PROPERTY(EssenceDescription,
    // {04010106-0203-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010104,
      0x0203, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x4701,
    AAF_REFERENCE_TYPE(StrongReference, EssenceDescriptor),
    true,
    false,
    SourceMob)
AAF_CLASS_END(SourceMob,
  // {0101010D-0101-3700-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x3700,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Mob,
  true)
AAF_CLASS_SEPARATOR()

// MobSlot
//
AAF_CLASS(MobSlot,
  // {0101010D-0101-3800-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x3800,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  InterchangeObject,
  false)
  AAF_PROPERTY(SlotID,
    // {01010701-0000-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x01070101,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x4801,
    AAF_TYPE(Int32),
    true,
    false,
    MobSlot)
  AAF_PROPERTY(SlotName,
    // {02010701-0100-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x01070102,
      0x0100, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x4802,
    AAF_TYPE(String),
    false,
    false,
    MobSlot)
  AAF_PROPERTY(Segment,
    // {04010106-0204-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010104,
      0x0204, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x4803,
    AAF_REFERENCE_TYPE(StrongReference, Segment),
    true,
    false,
    MobSlot)
  AAF_PROPERTY(PhysicalTrackNumber,
    // {03010401-0000-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x01040103,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x4804,
    AAF_TYPE(UInt32),
    false,
    false,
    MobSlot)
AAF_CLASS_END(MobSlot,
  // {0101010D-0101-3800-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x3800,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  InterchangeObject,
  false)
AAF_CLASS_SEPARATOR()

// EventMobSlot
//
AAF_CLASS(EventMobSlot,
  // {0101010D-0101-3900-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x3900,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  MobSlot,
  true)
  AAF_PROPERTY(EditRate,
    // {02043005-0000-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x05300402,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x4901,
    AAF_TYPE(Rational),
    true,
    false,
    EventMobSlot)
AAF_CLASS_END(EventMobSlot,
  // {0101010D-0101-3900-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x3900,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  MobSlot,
  true)
AAF_CLASS_SEPARATOR()

// StaticMobSlot
//
AAF_CLASS(StaticMobSlot,
  // {0101010D-0101-3A00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x3A00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  MobSlot,
  true)
AAF_CLASS_END(StaticMobSlot,
  // {0101010D-0101-3A00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x3A00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  MobSlot,
  true)
AAF_CLASS_SEPARATOR()

// TimelineMobSlot
//
AAF_CLASS(TimelineMobSlot,
  // {0101010D-0101-3B00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x3B00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  MobSlot,
  true)
  AAF_PROPERTY(EditRate,
    // {05043005-0000-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x05300405,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x4b01,
    AAF_TYPE(Rational),
    true,
    false,
    TimelineMobSlot)
  AAF_PROPERTY(Origin,
    // {03010207-0103-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x07020103,
      0x0103, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x4b02,
    AAF_TYPE(Position),
    true,
    false,
    TimelineMobSlot)
AAF_CLASS_END(TimelineMobSlot,
  // {0101010D-0101-3B00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x3B00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  MobSlot,
  true)
AAF_CLASS_SEPARATOR()

// Parameter
//
AAF_CLASS(Parameter,
  // {0101010D-0101-3C00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x3C00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  InterchangeObject,
  false)
  AAF_PROPERTY(Definition,
    // {04010106-0104-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010104,
      0x0104, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x4c01,
    AAF_TYPE(AUID),
    true,
    false,
    Parameter)
AAF_CLASS_END(Parameter,
  // {0101010D-0101-3C00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x3C00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  InterchangeObject,
  false)
AAF_CLASS_SEPARATOR()

// ConstantValue
//
AAF_CLASS(ConstantValue,
  // {0101010D-0101-3D00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x3D00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Parameter,
  true)
  AAF_PROPERTY(Value,
    // {07053005-0000-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x05300507,
      0x0000, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x4d01,
    AAF_TYPE(Indirect),
    true,
    false,
    ConstantValue)
AAF_CLASS_END(ConstantValue,
  // {0101010D-0101-3D00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x3D00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Parameter,
  true)
AAF_CLASS_SEPARATOR()

// VaryingValue
//
AAF_CLASS(VaryingValue,
  // {0101010D-0101-3E00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x3E00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Parameter,
  true)
  AAF_PROPERTY(Interpolation,
    // {04010106-0105-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010104,
      0x0105, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x4e01,
    AAF_REFERENCE_TYPE(WeakReference, InterpolationDefinition),
    true,
    false,
    VaryingValue)
  AAF_PROPERTY(PointList,
    // {04010106-0606-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010104,
      0x0606, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x4e02,
    AAF_REFERENCE_TYPE(StrongReferenceVector, ControlPoint),
    true,
    false,
    VaryingValue)
AAF_CLASS_END(VaryingValue,
  // {0101010D-0101-3E00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x3E00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Parameter,
  true)
AAF_CLASS_SEPARATOR()

// TaggedValue
//
AAF_CLASS(TaggedValue,
  // {0101010D-0101-3F00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x3F00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  InterchangeObject,
  true)
  AAF_PROPERTY(Name,
    // {02010203-0901-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x03020102,
      0x0901, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x5001,
    AAF_TYPE(String),
    true,
    false,
    TaggedValue)
  AAF_PROPERTY(Value,
    // {02010203-0A01-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x03020102,
      0x0A01, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x5003,
    AAF_TYPE(Indirect),
    true,
    false,
    TaggedValue)
AAF_CLASS_END(TaggedValue,
  // {0101010D-0101-3F00-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x3F00,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  InterchangeObject,
  true)
AAF_CLASS_SEPARATOR()

// KLVData
//
AAF_CLASS(KLVData,
  // {0101010D-0101-4000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x4000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  InterchangeObject,
  true)
  AAF_PROPERTY(Value,
    // {10020103-0200-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x03010210,
      0x0200, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x5101,
    AAF_TYPE(Opaque),
    true,
    false,
    KLVData)
AAF_CLASS_END(KLVData,
  // {0101010D-0101-4000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0101, 0x4000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  InterchangeObject,
  true)
AAF_CLASS_SEPARATOR()

// TypeDefinitionInteger
//
AAF_CLASS(TypeDefinitionInteger,
  // {0101010D-0204-0000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0204, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  TypeDefinition,
  true)
  AAF_PROPERTY(Size,
    // {03020103-0100-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x03010203,
      0x0100, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x000F,
    AAF_TYPE(UInt8),
    true,
    false,
    TypeDefinitionInteger)
  AAF_PROPERTY(IsSigned,
    // {03020103-0200-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x03010203,
      0x0200, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0010,
    AAF_TYPE(Boolean),
    true,
    false,
    TypeDefinitionInteger)
AAF_CLASS_END(TypeDefinitionInteger,
  // {0101010D-0204-0000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0204, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  TypeDefinition,
  true)
AAF_CLASS_SEPARATOR()

// TypeDefinitionStrongObjectReference
//
AAF_CLASS(TypeDefinitionStrongObjectReference,
  // {0101010D-0205-0000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0205, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  TypeDefinition,
  true)
  AAF_PROPERTY(ReferencedType,
    // {07010106-0900-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010107,
      0x0900, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0011,
    AAF_REFERENCE_TYPE(WeakReference, ClassDefinition),
    true,
    false,
    TypeDefinitionStrongObjectReference)
AAF_CLASS_END(TypeDefinitionStrongObjectReference,
  // {0101010D-0205-0000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0205, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  TypeDefinition,
  true)
AAF_CLASS_SEPARATOR()

// TypeDefinitionWeakObjectReference
//
AAF_CLASS(TypeDefinitionWeakObjectReference,
  // {0101010D-0206-0000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0206, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  TypeDefinition,
  true)
  AAF_PROPERTY(ReferencedType,
    // {07010106-0A00-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010107,
      0x0A00, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0012,
    AAF_REFERENCE_TYPE(WeakReference, ClassDefinition),
    true,
    false,
    TypeDefinitionWeakObjectReference)
  AAF_PROPERTY(TargetSet,
    // {03020103-0B00-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x03010203,
      0x0B00, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0013,
    AAF_TYPE(AUIDArray),
    true,
    false,
    TypeDefinitionWeakObjectReference)
AAF_CLASS_END(TypeDefinitionWeakObjectReference,
  // {0101010D-0206-0000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0206, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  TypeDefinition,
  true)
AAF_CLASS_SEPARATOR()

// TypeDefinitionEnumeration
//
AAF_CLASS(TypeDefinitionEnumeration,
  // {0101010D-0207-0000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0207, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  TypeDefinition,
  true)
  AAF_PROPERTY(ElementType,
    // {07010106-0B00-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010107,
      0x0B00, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0014,
    AAF_REFERENCE_TYPE(WeakReference, TypeDefinition),
    true,
    false,
    TypeDefinitionEnumeration)
  AAF_PROPERTY(ElementNames,
    // {03020103-0400-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x03010203,
      0x0400, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0015,
    AAF_TYPE(StringArray),
    true,
    false,
    TypeDefinitionEnumeration)
  AAF_PROPERTY(ElementValues,
    // {03020103-0500-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x03010203,
      0x0500, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0016,
    AAF_TYPE(Int64Array),
    true,
    false,
    TypeDefinitionEnumeration)
AAF_CLASS_END(TypeDefinitionEnumeration,
  // {0101010D-0207-0000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0207, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  TypeDefinition,
  true)
AAF_CLASS_SEPARATOR()

// TypeDefinitionFixedArray
//
AAF_CLASS(TypeDefinitionFixedArray,
  // {0101010D-0208-0000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0208, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  TypeDefinition,
  true)
  AAF_PROPERTY(ElementType,
    // {07010106-0C00-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010107,
      0x0C00, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0017,
    AAF_REFERENCE_TYPE(WeakReference, TypeDefinition),
    true,
    false,
    TypeDefinitionFixedArray)
  AAF_PROPERTY(ElementCount,
    // {03020103-0300-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x03010203,
      0x0300, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0018,
    AAF_TYPE(UInt32),
    true,
    false,
    TypeDefinitionFixedArray)
AAF_CLASS_END(TypeDefinitionFixedArray,
  // {0101010D-0208-0000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0208, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  TypeDefinition,
  true)
AAF_CLASS_SEPARATOR()

// TypeDefinitionVariableArray
//
AAF_CLASS(TypeDefinitionVariableArray,
  // {0101010D-0209-0000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0209, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  TypeDefinition,
  true)
  AAF_PROPERTY(ElementType,
    // {07010106-0D00-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010107,
      0x0D00, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0019,
    AAF_REFERENCE_TYPE(WeakReference, TypeDefinition),
    true,
    false,
    TypeDefinitionVariableArray)
AAF_CLASS_END(TypeDefinitionVariableArray,
  // {0101010D-0209-0000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0209, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  TypeDefinition,
  true)
AAF_CLASS_SEPARATOR()

// TypeDefinitionSet
//
AAF_CLASS(TypeDefinitionSet,
  // {0101010D-020A-0000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x020A, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  TypeDefinition,
  true)
  AAF_PROPERTY(ElementType,
    // {07010106-0E00-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010107,
      0x0E00, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x001A,
    AAF_REFERENCE_TYPE(WeakReference, TypeDefinition),
    true,
    false,
    TypeDefinitionSet)
AAF_CLASS_END(TypeDefinitionSet,
  // {0101010D-020A-0000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x020A, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  TypeDefinition,
  true)
AAF_CLASS_SEPARATOR()

// TypeDefinitionString
//
AAF_CLASS(TypeDefinitionString,
  // {0101010D-020B-0000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x020B, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  TypeDefinition,
  true)
  AAF_PROPERTY(ElementType,
    // {07010106-0F00-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010107,
      0x0F00, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x001B,
    AAF_REFERENCE_TYPE(WeakReference, TypeDefinition),
    true,
    false,
    TypeDefinitionString)
AAF_CLASS_END(TypeDefinitionString,
  // {0101010D-020B-0000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x020B, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  TypeDefinition,
  true)
AAF_CLASS_SEPARATOR()

// TypeDefinitionStream
//
AAF_CLASS(TypeDefinitionStream,
  // {0101010D-020C-0000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x020C, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  TypeDefinition,
  true)
AAF_CLASS_END(TypeDefinitionStream,
  // {0101010D-020C-0000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x020C, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  TypeDefinition,
  true)
AAF_CLASS_SEPARATOR()

// TypeDefinitionRecord
//
AAF_CLASS(TypeDefinitionRecord,
  // {0101010D-020D-0000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x020D, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  TypeDefinition,
  true)
  AAF_PROPERTY(MemberTypes,
    // {07010106-1100-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010107,
      0x1100, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x001C,
    AAF_REFERENCE_TYPE(WeakReferenceVector, TypeDefinition),
    true,
    false,
    TypeDefinitionRecord)
  AAF_PROPERTY(MemberNames,
    // {03020103-0600-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x03010203,
      0x0600, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x001D,
    AAF_TYPE(StringArray),
    true,
    false,
    TypeDefinitionRecord)
AAF_CLASS_END(TypeDefinitionRecord,
  // {0101010D-020D-0000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x020D, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  TypeDefinition,
  true)
AAF_CLASS_SEPARATOR()

// TypeDefinitionRename
//
AAF_CLASS(TypeDefinitionRename,
  // {0101010D-020E-0000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x020E, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  TypeDefinition,
  true)
  AAF_PROPERTY(RenamedType,
    // {07010106-1200-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010107,
      0x1200, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x001E,
    AAF_REFERENCE_TYPE(WeakReference, TypeDefinition),
    true,
    false,
    TypeDefinitionRename)
AAF_CLASS_END(TypeDefinitionRename,
  // {0101010D-020E-0000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x020E, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  TypeDefinition,
  true)
AAF_CLASS_SEPARATOR()

// TypeDefinitionExtendibleEnumeration
//
AAF_CLASS(TypeDefinitionExtendibleEnumeration,
  // {0101010D-0220-0000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0220, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  TypeDefinition,
  true)
  AAF_PROPERTY(ElementNames,
    // {03020103-0700-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x03010203,
      0x0700, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x001F,
    AAF_TYPE(StringArray),
    true,
    false,
    TypeDefinitionExtendibleEnumeration)
  AAF_PROPERTY(ElementValues,
    // {03020103-0800-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x03010203,
      0x0800, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0020,
    AAF_TYPE(AUIDArray),
    true,
    false,
    TypeDefinitionExtendibleEnumeration)
AAF_CLASS_END(TypeDefinitionExtendibleEnumeration,
  // {0101010D-0220-0000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0220, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  TypeDefinition,
  true)
AAF_CLASS_SEPARATOR()

// TypeDefinitionIndirect
//
AAF_CLASS(TypeDefinitionIndirect,
  // {0101010D-0221-0000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0221, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  TypeDefinition,
  true)
AAF_CLASS_END(TypeDefinitionIndirect,
  // {0101010D-0221-0000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0221, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  TypeDefinition,
  true)
AAF_CLASS_SEPARATOR()

// TypeDefinitionOpaque
//
AAF_CLASS(TypeDefinitionOpaque,
  // {0101010D-0222-0000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0222, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  TypeDefinitionIndirect,
  true)
AAF_CLASS_END(TypeDefinitionOpaque,
  // {0101010D-0222-0000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0222, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  TypeDefinitionIndirect,
  true)
AAF_CLASS_SEPARATOR()

// TypeDefinitionCharacter
//
AAF_CLASS(TypeDefinitionCharacter,
  // {0101010D-0223-0000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0223, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  TypeDefinition,
  true)
AAF_CLASS_END(TypeDefinitionCharacter,
  // {0101010D-0223-0000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0223, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  TypeDefinition,
  true)
AAF_CLASS_SEPARATOR()

// MetaDefinition
//
AAF_CLASS(MetaDefinition,
  // {0101010D-0224-0000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0224, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Root,
  false)
  AAF_PROPERTY(Identification,
    // {07010106-1300-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010107,
      0x1300, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0005,
    AAF_TYPE(AUID),
    true,
    true,
    MetaDefinition)
  AAF_PROPERTY(Name,
    // {01040203-0201-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x03020401,
      0x0201, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0006,
    AAF_TYPE(String),
    true,
    false,
    MetaDefinition)
  AAF_PROPERTY(Description,
    // {07010106-1401-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010107,
      0x1401, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0007,
    AAF_TYPE(String),
    false,
    false,
    MetaDefinition)
AAF_CLASS_END(MetaDefinition,
  // {0101010D-0224-0000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0224, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Root,
  false)
AAF_CLASS_SEPARATOR()

// MetaDictionary
//
AAF_CLASS(MetaDictionary,
  // {0101010D-0225-0000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0225, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Root,
  true)
  AAF_PROPERTY(ClassDefinitions,
    // {07010106-0700-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010107,
      0x0700, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0003,
    AAF_REFERENCE_TYPE(StrongReferenceSet, ClassDefinition),
    false,
    false,
    MetaDictionary)
  AAF_PROPERTY(TypeDefinitions,
    // {07010106-0800-0000-060E-2B3401010102}
    AAF_LITERAL_AUID(0x06010107,
      0x0800, 0x0000,
      0x06, 0x0E, 0x2B, 0x34, 0x01, 0x01, 0x01, 0x02),
    0x0004,
    AAF_REFERENCE_TYPE(StrongReferenceSet, TypeDefinition),
    false,
    false,
    MetaDictionary)
AAF_CLASS_END(MetaDictionary,
  // {0101010D-0225-0000-060E-2B3402060101}
  AAF_LITERAL_AUID(0x0D010101,
    0x0225, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x02, 0x06, 0x01, 0x01),
  Root,
  true)

AAF_TABLE_END()

// Types
//

AAF_TYPE_TABLE_BEGIN()

// UInt8
//
AAF_TYPE_DEFINITION_INTEGER(UInt8, 
  // {00010101-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x01010100,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), 1, False)
AAF_TYPE_SEPARATOR()

// UInt16
//
AAF_TYPE_DEFINITION_INTEGER(UInt16, 
  // {00020101-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x01010200,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), 2, False)
AAF_TYPE_SEPARATOR()

// UInt32
//
AAF_TYPE_DEFINITION_INTEGER(UInt32, 
  // {00030101-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x01010300,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), 4, False)
AAF_TYPE_SEPARATOR()

// UInt64
//
AAF_TYPE_DEFINITION_INTEGER(UInt64, 
  // {00040101-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x01010400,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), 8, False)
AAF_TYPE_SEPARATOR()

// Int8
//
AAF_TYPE_DEFINITION_INTEGER(Int8, 
  // {00050101-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x01010500,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), 1, True)
AAF_TYPE_SEPARATOR()

// Int16
//
AAF_TYPE_DEFINITION_INTEGER(Int16, 
  // {00060101-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x01010600,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), 2, True)
AAF_TYPE_SEPARATOR()

// Int32
//
AAF_TYPE_DEFINITION_INTEGER(Int32, 
  // {00070101-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x01010700,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), 4, True)
AAF_TYPE_SEPARATOR()

// Int64
//
AAF_TYPE_DEFINITION_INTEGER(Int64, 
  // {00080101-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x01010800,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), 8, True)
AAF_TYPE_SEPARATOR()

// Boolean
//
AAF_TYPE_DEFINITION_ENUMERATION(Boolean, 
  // {00010401-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x01040100,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), AAF_TYPE(UInt8))
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFFalse,
    0, Boolean)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFTrue,
    1, Boolean)
AAF_TYPE_DEFINITION_ENUMERATION_END(Boolean, 
  // {00010401-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x01040100,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), AAF_TYPE(UInt8))
AAF_TYPE_SEPARATOR()

// ProductReleaseType
//
AAF_TYPE_DEFINITION_ENUMERATION(ProductReleaseType, 
  // {01010102-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x02010101,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), AAF_TYPE(UInt8))
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
AAF_TYPE_DEFINITION_ENUMERATION_END(ProductReleaseType, 
  // {01010102-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x02010101,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), AAF_TYPE(UInt8))
AAF_TYPE_SEPARATOR()

// TapeFormatType
//
AAF_TYPE_DEFINITION_ENUMERATION(TapeFormatType, 
  // {02010102-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x02010102,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), AAF_TYPE(UInt8))
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
AAF_TYPE_DEFINITION_ENUMERATION_END(TapeFormatType, 
  // {02010102-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x02010102,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), AAF_TYPE(UInt8))
AAF_TYPE_SEPARATOR()

// VideoSignalType
//
AAF_TYPE_DEFINITION_ENUMERATION(VideoSignalType, 
  // {03010102-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x02010103,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), AAF_TYPE(UInt8))
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFVideoSignalNull,
    0, VideoSignalType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFNTSCSignal,
    1, VideoSignalType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFPALSignal,
    2, VideoSignalType)
AAF_TYPE_DEFINITION_ENUMERATION_END(VideoSignalType, 
  // {03010102-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x02010103,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), AAF_TYPE(UInt8))
AAF_TYPE_SEPARATOR()

// TapeCaseType
//
AAF_TYPE_DEFINITION_ENUMERATION(TapeCaseType, 
  // {04010102-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x02010104,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), AAF_TYPE(UInt8))
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
AAF_TYPE_DEFINITION_ENUMERATION_END(TapeCaseType, 
  // {04010102-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x02010104,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), AAF_TYPE(UInt8))
AAF_TYPE_SEPARATOR()

// ColorSitingType
//
AAF_TYPE_DEFINITION_ENUMERATION(ColorSitingType, 
  // {05010102-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x02010105,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), AAF_TYPE(UInt8))
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFCoSiting,
    0, ColorSitingType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFAveraging,
    1, ColorSitingType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFThreeTap,
    2, ColorSitingType)
AAF_TYPE_DEFINITION_ENUMERATION_END(ColorSitingType, 
  // {05010102-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x02010105,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), AAF_TYPE(UInt8))
AAF_TYPE_SEPARATOR()

// EditHintType
//
AAF_TYPE_DEFINITION_ENUMERATION(EditHintType, 
  // {06010102-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x02010106,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), AAF_TYPE(UInt8))
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
AAF_TYPE_DEFINITION_ENUMERATION_END(EditHintType, 
  // {06010102-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x02010106,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), AAF_TYPE(UInt8))
AAF_TYPE_SEPARATOR()

// FadeType
//
AAF_TYPE_DEFINITION_ENUMERATION(FadeType, 
  // {07010102-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x02010107,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), AAF_TYPE(UInt8))
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFFadeNone,
    0, FadeType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFFadeLinearAmp,
    1, FadeType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFFadeLinearPower,
    2, FadeType)
AAF_TYPE_DEFINITION_ENUMERATION_END(FadeType, 
  // {07010102-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x02010107,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), AAF_TYPE(UInt8))
AAF_TYPE_SEPARATOR()

// LayoutType
//
AAF_TYPE_DEFINITION_ENUMERATION(LayoutType, 
  // {08010102-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x02010108,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), AAF_TYPE(UInt8))
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFFullFrame,
    0, LayoutType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFSeparateFields,
    1, LayoutType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFOneField,
    2, LayoutType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFMixedFields,
    3, LayoutType)
AAF_TYPE_DEFINITION_ENUMERATION_END(LayoutType, 
  // {08010102-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x02010108,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), AAF_TYPE(UInt8))
AAF_TYPE_SEPARATOR()

// TCSource
//
AAF_TYPE_DEFINITION_ENUMERATION(TCSource, 
  // {09010102-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x02010109,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), AAF_TYPE(UInt8))
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFTimecodeLTC,
    0, TCSource)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFTimecodeVITC,
    1, TCSource)
AAF_TYPE_DEFINITION_ENUMERATION_END(TCSource, 
  // {09010102-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x02010109,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), AAF_TYPE(UInt8))
AAF_TYPE_SEPARATOR()

// PulldownDirectionType
//
AAF_TYPE_DEFINITION_ENUMERATION(PulldownDirectionType, 
  // {0A010102-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x0201010A,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), AAF_TYPE(UInt8))
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFTapeToFilmSpeed,
    0, PulldownDirectionType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFFilmToTapeSpeed,
    1, PulldownDirectionType)
AAF_TYPE_DEFINITION_ENUMERATION_END(PulldownDirectionType, 
  // {0A010102-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x0201010A,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), AAF_TYPE(UInt8))
AAF_TYPE_SEPARATOR()

// PulldownKindType
//
AAF_TYPE_DEFINITION_ENUMERATION(PulldownKindType, 
  // {0B010102-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x0201010B,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), AAF_TYPE(UInt8))
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFTwoThreePD,
    0, PulldownKindType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFPALPD,
    1, PulldownKindType)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFOneToOneNTSC,
    2, PulldownKindType)
AAF_TYPE_DEFINITION_ENUMERATION_END(PulldownKindType, 
  // {0B010102-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x0201010B,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), AAF_TYPE(UInt8))
AAF_TYPE_SEPARATOR()

// EdgeType
//
AAF_TYPE_DEFINITION_ENUMERATION(EdgeType, 
  // {0C010102-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x0201010C,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), AAF_TYPE(UInt8))
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
AAF_TYPE_DEFINITION_ENUMERATION_END(EdgeType, 
  // {0C010102-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x0201010C,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), AAF_TYPE(UInt8))
AAF_TYPE_SEPARATOR()

// FilmType
//
AAF_TYPE_DEFINITION_ENUMERATION(FilmType, 
  // {0D010102-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x0201010D,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), AAF_TYPE(UInt8))
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
AAF_TYPE_DEFINITION_ENUMERATION_END(FilmType, 
  // {0D010102-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x0201010D,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), AAF_TYPE(UInt8))
AAF_TYPE_SEPARATOR()

// RGBAComponentKind
//
AAF_TYPE_DEFINITION_ENUMERATION(RGBAComponentKind, 
  // {0E010102-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x0201010E,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), AAF_TYPE(UInt8))
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
AAF_TYPE_DEFINITION_ENUMERATION_END(RGBAComponentKind, 
  // {0E010102-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x0201010E,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), AAF_TYPE(UInt8))
AAF_TYPE_SEPARATOR()

// ReferenceType
//
AAF_TYPE_DEFINITION_ENUMERATION(ReferenceType, 
  // {0F010102-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x0201010F,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), AAF_TYPE(UInt8))
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
AAF_TYPE_DEFINITION_ENUMERATION_END(ReferenceType, 
  // {0F010102-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x0201010F,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), AAF_TYPE(UInt8))
AAF_TYPE_SEPARATOR()

// AlphaTransparency
//
AAF_TYPE_DEFINITION_ENUMERATION(AlphaTransparency, 
  // {20010102-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x02010120,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), AAF_TYPE(UInt8))
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFMinValueTransparent,
    0, AlphaTransparency)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFMaxValueTransparent,
    1, AlphaTransparency)
AAF_TYPE_DEFINITION_ENUMERATION_END(AlphaTransparency, 
  // {20010102-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x02010120,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), AAF_TYPE(UInt8))
AAF_TYPE_SEPARATOR()

// FieldNumber
//
AAF_TYPE_DEFINITION_ENUMERATION(FieldNumber, 
  // {21010102-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x02010121,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), AAF_TYPE(UInt8))
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFUnspecifiedField,
    0, FieldNumber)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFFieldOne,
    1, FieldNumber)
  AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(kAAFFieldTwo,
    2, FieldNumber)
AAF_TYPE_DEFINITION_ENUMERATION_END(FieldNumber, 
  // {21010102-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x02010121,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), AAF_TYPE(UInt8))
AAF_TYPE_SEPARATOR()

// UInt8Array8
//
AAF_TYPE_DEFINITION_FIXED_ARRAY(UInt8Array8, 
  // {00080104-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x04010800,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(UInt8), 8)
AAF_TYPE_SEPARATOR()

// UInt8Array
//
AAF_TYPE_DEFINITION_VARYING_ARRAY(UInt8Array, 
  // {00010104-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x04010100,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(UInt8))
AAF_TYPE_SEPARATOR()

// UInt8Array12
//
AAF_TYPE_DEFINITION_FIXED_ARRAY(UInt8Array12, 
  // {00020104-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x04010200,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(UInt8), 12)
AAF_TYPE_SEPARATOR()

// Int32Array
//
AAF_TYPE_DEFINITION_VARYING_ARRAY(Int32Array, 
  // {00030104-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x04010300,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(Int32))
AAF_TYPE_SEPARATOR()

// Int64Array
//
AAF_TYPE_DEFINITION_VARYING_ARRAY(Int64Array, 
  // {00040104-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x04010400,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(Int64))
AAF_TYPE_SEPARATOR()

// RGBALayout
//
AAF_TYPE_DEFINITION_FIXED_ARRAY(RGBALayout, 
  // {00010204-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x04020100,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(RGBAComponent), 8)
AAF_TYPE_SEPARATOR()

// StringArray
//
AAF_TYPE_DEFINITION_VARYING_ARRAY(StringArray, 
  // {00050104-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x04010500,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(Character))
AAF_TYPE_SEPARATOR()

// AUIDArray
//
AAF_TYPE_DEFINITION_VARYING_ARRAY(AUIDArray, 
  // {00060104-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x04010600,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(AUID))
AAF_TYPE_SEPARATOR()

// Rational
//
AAF_TYPE_DEFINITION_RECORD(Rational, 
  // {00010103-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x03010100,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01))
  AAF_TYPE_DEFINITION_RECORD_FIELD(Numerator, AAF_TYPE(Int32),
    Rational)
  AAF_TYPE_DEFINITION_RECORD_FIELD(Denominator, AAF_TYPE(Int32),
    Rational)
AAF_TYPE_DEFINITION_RECORD_END(Rational, 
  // {00010103-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x03010100,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01))
AAF_TYPE_SEPARATOR()

// AUID
//
AAF_TYPE_DEFINITION_RECORD(AUID, 
  // {00010301-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x01030100,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01))
  AAF_TYPE_DEFINITION_RECORD_FIELD(Data1, AAF_TYPE(UInt32),
    AUID)
  AAF_TYPE_DEFINITION_RECORD_FIELD(Data2, AAF_TYPE(UInt16),
    AUID)
  AAF_TYPE_DEFINITION_RECORD_FIELD(Data3, AAF_TYPE(UInt16),
    AUID)
  AAF_TYPE_DEFINITION_RECORD_FIELD(Data4, AAF_TYPE(UInt8Array8),
    AUID)
AAF_TYPE_DEFINITION_RECORD_END(AUID, 
  // {00010301-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x01030100,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01))
AAF_TYPE_SEPARATOR()

// MobID
//
AAF_TYPE_DEFINITION_RECORD(MobID, 
  // {00020301-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x01030200,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01))
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
AAF_TYPE_DEFINITION_RECORD_END(MobID, 
  // {00020301-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x01030200,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01))
AAF_TYPE_SEPARATOR()

// ProductVersion
//
AAF_TYPE_DEFINITION_RECORD(ProductVersion, 
  // {00020103-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x03010200,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01))
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
AAF_TYPE_DEFINITION_RECORD_END(ProductVersion, 
  // {00020103-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x03010200,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01))
AAF_TYPE_SEPARATOR()

// VersionType
//
AAF_TYPE_DEFINITION_RECORD(VersionType, 
  // {00030103-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x03010300,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01))
  AAF_TYPE_DEFINITION_RECORD_FIELD(major, AAF_TYPE(Int8),
    VersionType)
  AAF_TYPE_DEFINITION_RECORD_FIELD(minor, AAF_TYPE(Int8),
    VersionType)
AAF_TYPE_DEFINITION_RECORD_END(VersionType, 
  // {00030103-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x03010300,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01))
AAF_TYPE_SEPARATOR()

// RGBAComponent
//
AAF_TYPE_DEFINITION_RECORD(RGBAComponent, 
  // {00040103-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x03010400,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01))
  AAF_TYPE_DEFINITION_RECORD_FIELD(Code, AAF_TYPE(RGBAComponentKind),
    RGBAComponent)
  AAF_TYPE_DEFINITION_RECORD_FIELD(Size, AAF_TYPE(UInt8),
    RGBAComponent)
AAF_TYPE_DEFINITION_RECORD_END(RGBAComponent, 
  // {00040103-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x03010400,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01))
AAF_TYPE_SEPARATOR()

// DateStruct
//
AAF_TYPE_DEFINITION_RECORD(DateStruct, 
  // {00050103-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x03010500,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01))
  AAF_TYPE_DEFINITION_RECORD_FIELD(year, AAF_TYPE(Int16),
    DateStruct)
  AAF_TYPE_DEFINITION_RECORD_FIELD(month, AAF_TYPE(UInt8),
    DateStruct)
  AAF_TYPE_DEFINITION_RECORD_FIELD(day, AAF_TYPE(UInt8),
    DateStruct)
AAF_TYPE_DEFINITION_RECORD_END(DateStruct, 
  // {00050103-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x03010500,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01))
AAF_TYPE_SEPARATOR()

// TimeStruct
//
AAF_TYPE_DEFINITION_RECORD(TimeStruct, 
  // {00060103-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x03010600,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01))
  AAF_TYPE_DEFINITION_RECORD_FIELD(hour, AAF_TYPE(UInt8),
    TimeStruct)
  AAF_TYPE_DEFINITION_RECORD_FIELD(minute, AAF_TYPE(UInt8),
    TimeStruct)
  AAF_TYPE_DEFINITION_RECORD_FIELD(second, AAF_TYPE(UInt8),
    TimeStruct)
  AAF_TYPE_DEFINITION_RECORD_FIELD(fraction, AAF_TYPE(UInt8),
    TimeStruct)
AAF_TYPE_DEFINITION_RECORD_END(TimeStruct, 
  // {00060103-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x03010600,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01))
AAF_TYPE_SEPARATOR()

// TimeStamp
//
AAF_TYPE_DEFINITION_RECORD(TimeStamp, 
  // {00070103-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x03010700,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01))
  AAF_TYPE_DEFINITION_RECORD_FIELD(date, AAF_TYPE(DateStruct),
    TimeStamp)
  AAF_TYPE_DEFINITION_RECORD_FIELD(time, AAF_TYPE(TimeStruct),
    TimeStamp)
AAF_TYPE_DEFINITION_RECORD_END(TimeStamp, 
  // {00070103-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x03010700,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01))
AAF_TYPE_SEPARATOR()

// Position
//
AAF_TYPE_DEFINITION_RENAME(Position, 
  // {01200101-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x01012001,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), Int64)
AAF_TYPE_SEPARATOR()

// Length
//
AAF_TYPE_DEFINITION_RENAME(Length, 
  // {02200101-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x01012002,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), Int64)
AAF_TYPE_SEPARATOR()

// JPEGTableIDType
//
AAF_TYPE_DEFINITION_RENAME(JPEGTableIDType, 
  // {03200101-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x01012003,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), Int32)
AAF_TYPE_SEPARATOR()

// PhaseFrameType
//
AAF_TYPE_DEFINITION_RENAME(PhaseFrameType, 
  // {00230101-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x01012300,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), Int32)
AAF_TYPE_SEPARATOR()

// String
//
AAF_TYPE_DEFINITION_STRING(String, 
  // {00021001-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x01100200,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01), Character)
AAF_TYPE_SEPARATOR()

// OperationCategory
//
AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION(OperationCategory, 
  // {01010202-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x02020101,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01))
  AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_MEMBER(kAAFOperationEffect,
    // {0201010D-0101-0100-060E-2B3404010101}
    AAF_LITERAL_AUID(0x0D010102,
      0x0101, 0x0100,
      0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
    OperationCategory)
AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_END(OperationCategory, 
  // {01010202-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x02020101,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01))
AAF_TYPE_SEPARATOR()

// Character
//
AAF_TYPE_DEFINITION_CHARACTER(Character, 
  // {00011001-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x01100100,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01))
AAF_TYPE_SEPARATOR()

// Stream
//
AAF_TYPE_DEFINITION_STREAM(Stream, 
  // {00021004-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x04100200,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01))
AAF_TYPE_SEPARATOR()

// GammaType
//
AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION(GammaType, 
  // {02010202-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x02020102,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01))
AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_END(GammaType, 
  // {02010202-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x02020102,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01))
AAF_TYPE_SEPARATOR()

// CategoryType
//
AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION(CategoryType, 
  // {03010202-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x02020103,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01))
  AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_MEMBER(kAAFPluginCatagoryEffect,
    // {0201010D-0101-0200-060E-2B3404010101}
    AAF_LITERAL_AUID(0x0D010102,
      0x0101, 0x0200,
      0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
    CategoryType)
  AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_MEMBER(kAAFPluginCatagoryCodec,
    // {0201010D-0101-0300-060E-2B3404010101}
    AAF_LITERAL_AUID(0x0D010102,
      0x0101, 0x0300,
      0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
    CategoryType)
  AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_MEMBER(kAAFPluginCatagoryInterpolation,
    // {0201010D-0101-0400-060E-2B3404010101}
    AAF_LITERAL_AUID(0x0D010102,
      0x0101, 0x0400,
      0x06, 0x0E, 0x2B, 0x34, 0x04, 0x01, 0x01, 0x01),
    CategoryType)
AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_END(CategoryType, 
  // {03010202-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x02020103,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01))
AAF_TYPE_SEPARATOR()

// DataValue
//
AAF_TYPE_DEFINITION_VARYING_ARRAY(DataValue, 
  // {00011004-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x04100100,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(UInt8))
AAF_TYPE_SEPARATOR()

// PositionArray
//
AAF_TYPE_DEFINITION_VARYING_ARRAY(PositionArray, 
  // {00070104-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x04010700,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(UInt8))
AAF_TYPE_SEPARATOR()

// Indirect
//
AAF_TYPE_DEFINITION_INDIRECT(Indirect, 
  // {00031004-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x04100300,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01))
AAF_TYPE_SEPARATOR()

// Opaque
//
AAF_TYPE_DEFINITION_OPAQUE(Opaque, 
  // {00041004-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x04100400,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01))
AAF_TYPE_SEPARATOR()

// StrongReference<ContentStorage>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, ContentStorage), 
  // {00010205-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05020100,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(ContentStorage))
AAF_TYPE_SEPARATOR()

// StrongReference<Dictionary>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, Dictionary), 
  // {00020205-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05020200,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(Dictionary))
AAF_TYPE_SEPARATOR()

// StrongReference<EssenceDescriptor>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, EssenceDescriptor), 
  // {00030205-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05020300,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(EssenceDescriptor))
AAF_TYPE_SEPARATOR()

// StrongReference<NetworkLocator>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, NetworkLocator), 
  // {00040205-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05020400,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(NetworkLocator))
AAF_TYPE_SEPARATOR()

// StrongReference<OperationGroup>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, OperationGroup), 
  // {00050205-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05020500,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(OperationGroup))
AAF_TYPE_SEPARATOR()

// StrongReference<Segment>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, Segment), 
  // {00060205-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05020600,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(Segment))
AAF_TYPE_SEPARATOR()

// StrongReference<SourceClip>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, SourceClip), 
  // {00070205-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05020700,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(SourceClip))
AAF_TYPE_SEPARATOR()

// StrongReference<SourceReference>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, SourceReference), 
  // {00080205-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05020800,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(SourceReference))
AAF_TYPE_SEPARATOR()

// StrongReference<ClassDefinition>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, ClassDefinition), 
  // {00090205-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05020900,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(ClassDefinition))
AAF_TYPE_SEPARATOR()

// StrongReference<CodecDefinition>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, CodecDefinition), 
  // {000A0205-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05020A00,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(CodecDefinition))
AAF_TYPE_SEPARATOR()

// StrongReference<Component>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, Component), 
  // {000B0205-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05020B00,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(Component))
AAF_TYPE_SEPARATOR()

// StrongReference<ContainerDefinition>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, ContainerDefinition), 
  // {000C0205-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05020C00,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(ContainerDefinition))
AAF_TYPE_SEPARATOR()

// StrongReference<ControlPoint>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, ControlPoint), 
  // {000D0205-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05020D00,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(ControlPoint))
AAF_TYPE_SEPARATOR()

// StrongReference<DataDefinition>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, DataDefinition), 
  // {000E0205-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05020E00,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(DataDefinition))
AAF_TYPE_SEPARATOR()

// StrongReference<EssenceData>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, EssenceData), 
  // {000F0205-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05020F00,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(EssenceData))
AAF_TYPE_SEPARATOR()

// StrongReference<Identification>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, Identification), 
  // {00100205-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05021000,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(Identification))
AAF_TYPE_SEPARATOR()

// StrongReference<InterpolationDefinition>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, InterpolationDefinition), 
  // {00110205-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05021100,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(InterpolationDefinition))
AAF_TYPE_SEPARATOR()

// StrongReference<Locator>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, Locator), 
  // {00120205-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05021200,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(Locator))
AAF_TYPE_SEPARATOR()

// StrongReference<Mob>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, Mob), 
  // {00130205-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05021300,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(Mob))
AAF_TYPE_SEPARATOR()

// StrongReference<MobSlot>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, MobSlot), 
  // {00140205-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05021400,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(MobSlot))
AAF_TYPE_SEPARATOR()

// StrongReference<OperationDefinition>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, OperationDefinition), 
  // {00150205-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05021500,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(OperationDefinition))
AAF_TYPE_SEPARATOR()

// StrongReference<Parameter>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, Parameter), 
  // {00160205-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05021600,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(Parameter))
AAF_TYPE_SEPARATOR()

// StrongReference<ParameterDefinition>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, ParameterDefinition), 
  // {00170205-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05021700,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(ParameterDefinition))
AAF_TYPE_SEPARATOR()

// StrongReference<PluginDefinition>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, PluginDefinition), 
  // {00180205-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05021800,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(PluginDefinition))
AAF_TYPE_SEPARATOR()

// StrongReference<PropertyDefinition>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, PropertyDefinition), 
  // {00190205-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05021900,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(PropertyDefinition))
AAF_TYPE_SEPARATOR()

// StrongReference<TaggedValue>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, TaggedValue), 
  // {001A0205-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05021A00,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(TaggedValue))
AAF_TYPE_SEPARATOR()

// StrongReference<TypeDefinition>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, TypeDefinition), 
  // {001B0205-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05021B00,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(TypeDefinition))
AAF_TYPE_SEPARATOR()

// StrongReference<KLVData>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(StrongReference, KLVData), 
  // {001C0205-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05021C00,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(KLVData))
AAF_TYPE_SEPARATOR()

// StrongReferenceSet<ClassDefinition>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceSet, ClassDefinition), 
  // {00010505-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05050100,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(ClassDefinition))
AAF_TYPE_SEPARATOR()

// StrongReferenceSet<CodecDefinition>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceSet, CodecDefinition), 
  // {00020505-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05050200,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(CodecDefinition))
AAF_TYPE_SEPARATOR()

// StrongReferenceSet<ContainerDefinition>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceSet, ContainerDefinition), 
  // {00030505-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05050300,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(ContainerDefinition))
AAF_TYPE_SEPARATOR()

// StrongReferenceSet<DataDefinition>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceSet, DataDefinition), 
  // {00040505-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05050400,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(DataDefinition))
AAF_TYPE_SEPARATOR()

// StrongReferenceSet<EssenceData>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceSet, EssenceData), 
  // {00050505-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05050500,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(EssenceData))
AAF_TYPE_SEPARATOR()

// StrongReferenceSet<InterpolationDefinition>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceSet, InterpolationDefinition), 
  // {00060505-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05050600,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(InterpolationDefinition))
AAF_TYPE_SEPARATOR()

// StrongReferenceSet<Mob>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceSet, Mob), 
  // {00070505-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05050700,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(Mob))
AAF_TYPE_SEPARATOR()

// StrongReferenceSet<OperationDefinition>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceSet, OperationDefinition), 
  // {00080505-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05050800,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(OperationDefinition))
AAF_TYPE_SEPARATOR()

// StrongReferenceSet<ParameterDefinition>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceSet, ParameterDefinition), 
  // {00090505-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05050900,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(ParameterDefinition))
AAF_TYPE_SEPARATOR()

// StrongReferenceSet<PluginDefinition>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceSet, PluginDefinition), 
  // {000A0505-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05050A00,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(PluginDefinition))
AAF_TYPE_SEPARATOR()

// StrongReferenceSet<PropertyDefinition>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceSet, PropertyDefinition), 
  // {000B0505-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05050B00,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(PropertyDefinition))
AAF_TYPE_SEPARATOR()

// StrongReferenceSet<TypeDefinition>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceSet, TypeDefinition), 
  // {000C0505-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05050C00,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(TypeDefinition))
AAF_TYPE_SEPARATOR()

// StrongReferenceVector<Component>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceVector, Component), 
  // {00010605-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05060100,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(Component))
AAF_TYPE_SEPARATOR()

// StrongReferenceVector<ControlPoint>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceVector, ControlPoint), 
  // {00020605-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05060200,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(ControlPoint))
AAF_TYPE_SEPARATOR()

// StrongReferenceVector<Identification>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceVector, Identification), 
  // {00030605-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05060300,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(Identification))
AAF_TYPE_SEPARATOR()

// StrongReferenceVector<Locator>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceVector, Locator), 
  // {00040605-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05060400,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(Locator))
AAF_TYPE_SEPARATOR()

// StrongReferenceVector<MobSlot>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceVector, MobSlot), 
  // {00050605-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05060500,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(MobSlot))
AAF_TYPE_SEPARATOR()

// StrongReferenceVector<Segment>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceVector, Segment), 
  // {00060605-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05060600,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(Segment))
AAF_TYPE_SEPARATOR()

// StrongReferenceVector<SourceReference>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceVector, SourceReference), 
  // {00070605-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05060700,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(SourceReference))
AAF_TYPE_SEPARATOR()

// StrongReferenceVector<TaggedValue>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceVector, TaggedValue), 
  // {00080605-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05060800,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(TaggedValue))
AAF_TYPE_SEPARATOR()

// StrongReferenceVector<KLVData>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceVector, KLVData), 
  // {00090605-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05060900,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(KLVData))
AAF_TYPE_SEPARATOR()

// StrongReferenceVector<Parameter>
//
AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR(
  AAF_REFERENCE_TYPE_NAME(StrongReferenceVector, Parameter), 
  // {000A0605-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05060A00,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(Parameter))
AAF_TYPE_SEPARATOR()

// WeakReference<ClassDefinition>
//
AAF_TYPE_DEFINITION_WEAK_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(WeakReference, ClassDefinition), 
  // {00010105-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05010100,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(ClassDefinition))
  AAF_TYPE_DEFINITION_WEAK_REFERENCE_MEMBER(MetaDictionary, Root,
    AAF_REFERENCE_TYPE_NAME(WeakReference, ClassDefinition))
  AAF_TYPE_DEFINITION_WEAK_REFERENCE_MEMBER(ClassDefinitions, MetaDictionary,
    AAF_REFERENCE_TYPE_NAME(WeakReference, ClassDefinition))
AAF_TYPE_DEFINITION_WEAK_REFERENCE_END(
  AAF_REFERENCE_TYPE_NAME(WeakReference, ClassDefinition), 
  // {00010105-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05010100,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(ClassDefinition))
AAF_TYPE_SEPARATOR()

// WeakReference<ContainerDefinition>
//
AAF_TYPE_DEFINITION_WEAK_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(WeakReference, ContainerDefinition), 
  // {00020105-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05010200,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(ContainerDefinition))
  AAF_TYPE_DEFINITION_WEAK_REFERENCE_MEMBER(Header, Root,
    AAF_REFERENCE_TYPE_NAME(WeakReference, ContainerDefinition))
  AAF_TYPE_DEFINITION_WEAK_REFERENCE_MEMBER(Dictionary, Header,
    AAF_REFERENCE_TYPE_NAME(WeakReference, ContainerDefinition))
  AAF_TYPE_DEFINITION_WEAK_REFERENCE_MEMBER(ContainerDefinitions, Dictionary,
    AAF_REFERENCE_TYPE_NAME(WeakReference, ContainerDefinition))
AAF_TYPE_DEFINITION_WEAK_REFERENCE_END(
  AAF_REFERENCE_TYPE_NAME(WeakReference, ContainerDefinition), 
  // {00020105-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05010200,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(ContainerDefinition))
AAF_TYPE_SEPARATOR()

// WeakReference<DataDefinition>
//
AAF_TYPE_DEFINITION_WEAK_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(WeakReference, DataDefinition), 
  // {00030105-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05010300,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(DataDefinition))
  AAF_TYPE_DEFINITION_WEAK_REFERENCE_MEMBER(Header, Root,
    AAF_REFERENCE_TYPE_NAME(WeakReference, DataDefinition))
  AAF_TYPE_DEFINITION_WEAK_REFERENCE_MEMBER(Dictionary, Header,
    AAF_REFERENCE_TYPE_NAME(WeakReference, DataDefinition))
  AAF_TYPE_DEFINITION_WEAK_REFERENCE_MEMBER(DataDefinitions, Dictionary,
    AAF_REFERENCE_TYPE_NAME(WeakReference, DataDefinition))
AAF_TYPE_DEFINITION_WEAK_REFERENCE_END(
  AAF_REFERENCE_TYPE_NAME(WeakReference, DataDefinition), 
  // {00030105-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05010300,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(DataDefinition))
AAF_TYPE_SEPARATOR()

// WeakReference<InterpolationDefinition>
//
AAF_TYPE_DEFINITION_WEAK_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(WeakReference, InterpolationDefinition), 
  // {00050105-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05010500,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(InterpolationDefinition))
  AAF_TYPE_DEFINITION_WEAK_REFERENCE_MEMBER(Header, Root,
    AAF_REFERENCE_TYPE_NAME(WeakReference, InterpolationDefinition))
  AAF_TYPE_DEFINITION_WEAK_REFERENCE_MEMBER(Dictionary, Header,
    AAF_REFERENCE_TYPE_NAME(WeakReference, InterpolationDefinition))
  AAF_TYPE_DEFINITION_WEAK_REFERENCE_MEMBER(InterpolationDefinitions, Dictionary,
    AAF_REFERENCE_TYPE_NAME(WeakReference, InterpolationDefinition))
AAF_TYPE_DEFINITION_WEAK_REFERENCE_END(
  AAF_REFERENCE_TYPE_NAME(WeakReference, InterpolationDefinition), 
  // {00050105-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05010500,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(InterpolationDefinition))
AAF_TYPE_SEPARATOR()

// WeakReference<Mob>
//
AAF_TYPE_DEFINITION_WEAK_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(WeakReference, Mob), 
  // {00060105-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05010600,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(Mob))
  AAF_TYPE_DEFINITION_WEAK_REFERENCE_MEMBER(Header, Root,
    AAF_REFERENCE_TYPE_NAME(WeakReference, Mob))
  AAF_TYPE_DEFINITION_WEAK_REFERENCE_MEMBER(Content, Header,
    AAF_REFERENCE_TYPE_NAME(WeakReference, Mob))
  AAF_TYPE_DEFINITION_WEAK_REFERENCE_MEMBER(Mobs, ContentStorage,
    AAF_REFERENCE_TYPE_NAME(WeakReference, Mob))
AAF_TYPE_DEFINITION_WEAK_REFERENCE_END(
  AAF_REFERENCE_TYPE_NAME(WeakReference, Mob), 
  // {00060105-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05010600,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(Mob))
AAF_TYPE_SEPARATOR()

// WeakReference<OperationDefinition>
//
AAF_TYPE_DEFINITION_WEAK_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(WeakReference, OperationDefinition), 
  // {00070105-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05010700,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(OperationDefinition))
  AAF_TYPE_DEFINITION_WEAK_REFERENCE_MEMBER(Header, Root,
    AAF_REFERENCE_TYPE_NAME(WeakReference, OperationDefinition))
  AAF_TYPE_DEFINITION_WEAK_REFERENCE_MEMBER(Dictionary, Header,
    AAF_REFERENCE_TYPE_NAME(WeakReference, OperationDefinition))
  AAF_TYPE_DEFINITION_WEAK_REFERENCE_MEMBER(OperationDefinitions, Dictionary,
    AAF_REFERENCE_TYPE_NAME(WeakReference, OperationDefinition))
AAF_TYPE_DEFINITION_WEAK_REFERENCE_END(
  AAF_REFERENCE_TYPE_NAME(WeakReference, OperationDefinition), 
  // {00070105-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05010700,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(OperationDefinition))
AAF_TYPE_SEPARATOR()

// WeakReference<ParameterDefinition>
//
AAF_TYPE_DEFINITION_WEAK_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(WeakReference, ParameterDefinition), 
  // {00080105-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05010800,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(ParameterDefinition))
  AAF_TYPE_DEFINITION_WEAK_REFERENCE_MEMBER(Header, Root,
    AAF_REFERENCE_TYPE_NAME(WeakReference, ParameterDefinition))
  AAF_TYPE_DEFINITION_WEAK_REFERENCE_MEMBER(Dictionary, Header,
    AAF_REFERENCE_TYPE_NAME(WeakReference, ParameterDefinition))
  AAF_TYPE_DEFINITION_WEAK_REFERENCE_MEMBER(ParameterDefinitions, Dictionary,
    AAF_REFERENCE_TYPE_NAME(WeakReference, ParameterDefinition))
AAF_TYPE_DEFINITION_WEAK_REFERENCE_END(
  AAF_REFERENCE_TYPE_NAME(WeakReference, ParameterDefinition), 
  // {00080105-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05010800,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(ParameterDefinition))
AAF_TYPE_SEPARATOR()

// WeakReference<TypeDefinition>
//
AAF_TYPE_DEFINITION_WEAK_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(WeakReference, TypeDefinition), 
  // {00090105-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05010900,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(TypeDefinition))
  AAF_TYPE_DEFINITION_WEAK_REFERENCE_MEMBER(MetaDictionary, Root,
    AAF_REFERENCE_TYPE_NAME(WeakReference, TypeDefinition))
  AAF_TYPE_DEFINITION_WEAK_REFERENCE_MEMBER(TypeDefinitions, MetaDictionary,
    AAF_REFERENCE_TYPE_NAME(WeakReference, TypeDefinition))
AAF_TYPE_DEFINITION_WEAK_REFERENCE_END(
  AAF_REFERENCE_TYPE_NAME(WeakReference, TypeDefinition), 
  // {00090105-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05010900,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(TypeDefinition))
AAF_TYPE_SEPARATOR()

// WeakReference<PluginDefinition>
//
AAF_TYPE_DEFINITION_WEAK_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(WeakReference, PluginDefinition), 
  // {000A0105-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05010A00,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(PluginDefinition))
  AAF_TYPE_DEFINITION_WEAK_REFERENCE_MEMBER(Header, Root,
    AAF_REFERENCE_TYPE_NAME(WeakReference, PluginDefinition))
  AAF_TYPE_DEFINITION_WEAK_REFERENCE_MEMBER(Dictionary, Header,
    AAF_REFERENCE_TYPE_NAME(WeakReference, PluginDefinition))
  AAF_TYPE_DEFINITION_WEAK_REFERENCE_MEMBER(PluginDefinitions, Dictionary,
    AAF_REFERENCE_TYPE_NAME(WeakReference, PluginDefinition))
AAF_TYPE_DEFINITION_WEAK_REFERENCE_END(
  AAF_REFERENCE_TYPE_NAME(WeakReference, PluginDefinition), 
  // {000A0105-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05010A00,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(PluginDefinition))
AAF_TYPE_SEPARATOR()

// WeakReference<CodecDefinition>
//
AAF_TYPE_DEFINITION_WEAK_REFERENCE(
  AAF_REFERENCE_TYPE_NAME(WeakReference, CodecDefinition), 
  // {000B0105-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05010B00,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(CodecDefinition))
  AAF_TYPE_DEFINITION_WEAK_REFERENCE_MEMBER(Header, Root,
    AAF_REFERENCE_TYPE_NAME(WeakReference, CodecDefinition))
  AAF_TYPE_DEFINITION_WEAK_REFERENCE_MEMBER(Dictionary, Header,
    AAF_REFERENCE_TYPE_NAME(WeakReference, CodecDefinition))
  AAF_TYPE_DEFINITION_WEAK_REFERENCE_MEMBER(CodecDefinitions, Dictionary,
    AAF_REFERENCE_TYPE_NAME(WeakReference, CodecDefinition))
AAF_TYPE_DEFINITION_WEAK_REFERENCE_END(
  AAF_REFERENCE_TYPE_NAME(WeakReference, CodecDefinition), 
  // {000B0105-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05010B00,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(CodecDefinition))
AAF_TYPE_SEPARATOR()

// WeakReferenceSet<DataDefinition>
//
AAF_TYPE_DEFINITION_WEAK_REFERENCE_SET(
  AAF_REFERENCE_TYPE_NAME(WeakReferenceSet, DataDefinition), 
  // {000D0305-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05030D00,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(DataDefinition))
AAF_TYPE_SEPARATOR()

// WeakReferenceSet<ParameterDefinition>
//
AAF_TYPE_DEFINITION_WEAK_REFERENCE_SET(
  AAF_REFERENCE_TYPE_NAME(WeakReferenceSet, ParameterDefinition), 
  // {000E0305-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05030E00,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(ParameterDefinition))
AAF_TYPE_SEPARATOR()

// WeakReferenceSet<PluginDefinition>
//
AAF_TYPE_DEFINITION_WEAK_REFERENCE_SET(
  AAF_REFERENCE_TYPE_NAME(WeakReferenceSet, PluginDefinition), 
  // {000F0305-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05030F00,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(PluginDefinition))
AAF_TYPE_SEPARATOR()

// WeakReferenceVector<OperationDefinition>
//
AAF_TYPE_DEFINITION_WEAK_REFERENCE_VECTOR(
  AAF_REFERENCE_TYPE_NAME(WeakReferenceVector, OperationDefinition), 
  // {00010405-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05040100,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(OperationDefinition))
AAF_TYPE_SEPARATOR()

// WeakReferenceVector<TypeDefinition>
//
AAF_TYPE_DEFINITION_WEAK_REFERENCE_VECTOR(
  AAF_REFERENCE_TYPE_NAME(WeakReferenceVector, TypeDefinition), 
  // {00020405-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05040200,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(TypeDefinition))
AAF_TYPE_SEPARATOR()

// WeakReferenceVector<DataDefinition>
//
AAF_TYPE_DEFINITION_WEAK_REFERENCE_VECTOR(
  AAF_REFERENCE_TYPE_NAME(WeakReferenceVector, DataDefinition), 
  // {00030405-0000-0000-060E-2B3401040101}
  AAF_LITERAL_AUID(0x05040300,
    0x0000, 0x0000,
    0x06, 0x0E, 0x2B, 0x34, 0x01, 0x04, 0x01, 0x01),
  AAF_TYPE(DataDefinition))

// Aliases
//
AAF_CLASS_ALIAS(CodecDefinition, CodecDef)
AAF_CLASS_ALIAS(DataDefinition, DataDef)
AAF_CLASS_ALIAS(DefinitionObject, DefObject)
AAF_CLASS_ALIAS(EdgeCode, Edgecode)
AAF_CLASS_ALIAS(OperationDefinition, OperationDef)
AAF_CLASS_ALIAS(InterchangeObject, Object)
AAF_CLASS_ALIAS(ParameterDefinition, ParameterDef)
AAF_CLASS_ALIAS(InterpolationDefinition, InterpolationDef)
AAF_CLASS_ALIAS(PropertyDefinition, PropertyDef)
AAF_CLASS_ALIAS(TypeDefinition, TypeDef)
AAF_CLASS_ALIAS(TypeDefinitionCharacter, TypeDefCharacter)
AAF_CLASS_ALIAS(TypeDefinitionEnumeration, TypeDefEnum)
AAF_CLASS_ALIAS(TypeDefinitionExtendibleEnumeration, TypeDefExtEnum)
AAF_CLASS_ALIAS(TypeDefinitionFixedArray, TypeDefFixedArray)
AAF_CLASS_ALIAS(TypeDefinitionInteger, TypeDefInt)
AAF_CLASS_ALIAS(TypeDefinitionRecord, TypeDefRecord)
AAF_CLASS_ALIAS(TypeDefinitionRename, TypeDefRename)
AAF_CLASS_ALIAS(TypeDefinitionSet, TypeDefSet)
AAF_CLASS_ALIAS(TypeDefinitionStream, TypeDefStream)
AAF_CLASS_ALIAS(TypeDefinitionString, TypeDefString)
AAF_CLASS_ALIAS(TypeDefinitionIndirect, TypeDefIndirect)
AAF_CLASS_ALIAS(TypeDefinitionOpaque, TypeDefOpaque)
AAF_CLASS_ALIAS(TypeDefinitionStrongObjectReference, TypeDefStrongObjRef)
AAF_CLASS_ALIAS(TypeDefinitionVariableArray, TypeDefVariableArray)
AAF_CLASS_ALIAS(TypeDefinitionWeakObjectReference, TypeDefWeakObjRef)
AAF_CLASS_ALIAS(ContainerDefinition, ContainerDef)
AAF_CLASS_ALIAS(PluginDefinition, PluginDef)

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

#undef AAF_TYPE_DEFINITION_WEAK_REFERENCE_MEMBER

#undef AAF_TYPE_DEFINITION_WEAK_REFERENCE_END

#undef AAF_TYPE_DEFINITION_WEAK_REFERENCE_SET

#undef AAF_TYPE_DEFINITION_WEAK_REFERENCE_VECTOR

#undef AAF_TYPE_DEFINITION_STREAM

#undef AAF_TYPE_SEPARATOR

#undef AAF_CLASS_ALIAS

#undef AAF_LITERAL_AUID

//
// End of automatically generated file.

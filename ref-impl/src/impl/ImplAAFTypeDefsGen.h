#ifndef __ImplAAFTypeDefsGen_h__
#define __ImplAAFTypeDefsGen_h__

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


#include "AAFTypes.h"


// Get guids for types we'll use
// #define TYPE_GUID_NAME(type) kAAFTypeID_##type
#include "AAFTypeDefUIDs.h"

//
// Pass 1:  Do stuff for integers.
//
#define AAF_TYPE_TABLE_BEGIN()  \
struct TypeInteger              \
{                               \
  wchar_t * typeName;           \
  aafUID_t  typeID;             \
  aafUInt8  size;               \
  aafBool   isSigned;           \
  int       isValid;            \
};                              \
                                \
static TypeInteger s_AAFAllTypeIntegers [] = {

#define AAF_TYPE_DEFINITION_INTEGER(name, id, size, signed) {L"aaf" L###name, id, size, kAAF##signed, 1},

#define AAF_TYPE_TABLE_END()  0 };

#include "AAFMetaDictionary.h"

//
// pass 2: For enumerations, define individual enumeration fields
//

//
// Define structs to describe each member of an enumeration typedef,
// and to describe the entire enumeration typedef.
//
#define AAF_TYPE_TABLE_BEGIN()             \
                                           \
struct TypeEnumerationMember               \
{                                          \
  wchar_t *        memberName;             \
  aafInt64         memberValue;            \
};                                         \
                                           \
struct TypeEnumeration                     \
{                                          \
  aafUID_t                 typeID;         \
  const wchar_t *          typeName;       \
  const aafUID_t *         pElementTypeId; \
  size_t                   size;           \
  TypeEnumerationMember ** members;        \
};

#define AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(name, value, parent) \
static const TypeEnumerationMember s_TypeEnumerationMember_##parent##_##name = \
{ L###name, \
  value \
};

#define AAF_TYPE(x) aaf##x##_t
#define AAF_REFERENCE_TYPE(type, target)      AAF_TYPE(target##type)
#define AAF_REFERENCE_TYPE_NAME(type, target) AAF_TYPE(target##type)
#include "AAFMetaDictionary.h"


//
// pass 3: For enumerations, create the (null-terminated) field list for
// each enumeration
//

#define AAF_TYPE_DEFINITION_ENUMERATION(name, id, type) \
static const TypeEnumerationMember *s_TypeEnumerationMembers_##name[] = {

#define AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(name, value, parent) \
	&s_TypeEnumerationMember_##parent##_##name,

#define AAF_TYPE_DEFINITION_ENUMERATION_END(name) \
  0 } ;

#define AAF_TYPE(x) _aaf##x##_t
#define AAF_REFERENCE_TYPE(type, target)      AAF_TYPE(target##type)
#define AAF_REFERENCE_TYPE_NAME(type, target) AAF_TYPE(target##type)
#include "AAFMetaDictionary.h"

//
// pass 4: For enumerations, create each enumeration definition
//

#define AAF_TYPE_DEFINITION_ENUMERATION(name, id, type) \
static const TypeEnumeration s_TypeEnumeration_##name = \
{ id, \
  L###name, \
	/*(aafUID_t *)*/& TYPE_GUID_NAME(type), \
  sizeof (aaf##name##_t), \
  (TypeEnumerationMember **) s_TypeEnumerationMembers_##name, \
 };

#define AAF_TYPE(x) x
// #define AAF_REFERENCE_TYPE(type, target)      AAF_TYPE(target##type)
// #define AAF_REFERENCE_TYPE_NAME(type, target) AAF_TYPE(target##type)
#include "AAFMetaDictionary.h"

//
// pass 5: For enumerations, create a master null-terminated list of all
// type def enumerations
//

#define AAF_TYPE_TABLE_BEGIN()   \
static TypeEnumeration * s_AAFAllTypeEnumerations [] = {

#define AAF_TYPE_DEFINITION_ENUMERATION(name, id, type) \
  (TypeEnumeration*)& s_TypeEnumeration_##name,

#define AAF_TYPE_TABLE_END()   \
0 };

#define AAF_TYPE(x) _aaf##x##_t
#define AAF_REFERENCE_TYPE(type, target)      AAF_TYPE(target##type)
#define AAF_REFERENCE_TYPE_NAME(type, target) AAF_TYPE(target##type)
#include "AAFMetaDictionary.h"


//
// Done with enumerations.
//

//
// pass 6: For records, define individual record fields
//

//
// Define structs to describe each member of a record typedef, and to
// describe the entire record typedef.
//
#define AAF_TYPE_TABLE_BEGIN()      \
                                    \
struct TypeRecordMember             \
{                                   \
  const aafUID_t *   pMemberTypeId; \
  wchar_t *          memberName;    \
  size_t             memberOffset;  \
  eAAFTypeCategory_t typeCat;       \
};                                  \
                                    \
struct TypeRecord                   \
{                                   \
  aafUID_t   typeID;                \
  wchar_t *  typeName;              \
  size_t     size;                  \
  TypeRecordMember ** members;      \
};

#define AAF_TYPE_RECORD_MEMBER(recordName, memberType, memberName) \

#define AAF_TYPE_DEFINITION_RECORD_FIELD(name, type, parent) \
static const TypeRecordMember s_TypeRecordMember_##parent##_##name = \
{ (aafUID_t *)& TYPE_GUID_NAME(type), \
  L###name, \
  offsetof (aaf##parent##_t, name), \
  kAAFTypeCatUnknown \
};

#define AAF_TYPE(x) x
#define AAF_REFERENCE_TYPE(type, target)      AAF_TYPE(target##type)
#define AAF_REFERENCE_TYPE_NAME(type, target) AAF_TYPE(target##type)
#include "AAFMetaDictionary.h"

//
// pass 7: For records, create the (null-terminated) field list for
// each record
//

#define AAF_TYPE_DEFINITION_RECORD(name, id) \
static const TypeRecordMember *s_TypeRecordMembers_##name[] = {

#define AAF_TYPE_DEFINITION_RECORD_FIELD(name, type, parent) \
	&s_TypeRecordMember_##parent##_##name,

#define AAF_TYPE_DEFINITION_RECORD_END(name) \
  0 } ;

#define AAF_TYPE(x) _aaf##x##_t
#define AAF_REFERENCE_TYPE(type, target)      AAF_TYPE(target##type)
#define AAF_REFERENCE_TYPE_NAME(type, target) AAF_TYPE(target##type)
#include "AAFMetaDictionary.h"

//
// pass 8: For records, create each record definition
//

#define AAF_TYPE_DEFINITION_RECORD(name, id) \
static const TypeRecord s_TypeRecord_##name = \
{ id, L###name, sizeof (AAF_TYPE(name)), (TypeRecordMember **) s_TypeRecordMembers_##name };

#define AAF_TYPE(x) aaf##x##_t
#define AAF_REFERENCE_TYPE(type, target)      AAF_TYPE(target##type)
#define AAF_REFERENCE_TYPE_NAME(type, target) AAF_TYPE(target##type)
#include "AAFMetaDictionary.h"

//
// pass 9: For records, create a master null-terminated list of all
// type def records
//

#define AAF_TYPE_TABLE_BEGIN()   \
static TypeRecord * s_AAFAllTypeRecords [] = {

#define AAF_TYPE_DEFINITION_RECORD(name, id) \
  (TypeRecord*)& s_TypeRecord_##name,

#define AAF_TYPE_TABLE_END()   \
0 };

#define AAF_TYPE(x) _aaf##x##_t
#define AAF_REFERENCE_TYPE(type, target)      AAF_TYPE(target##type)
#define AAF_REFERENCE_TYPE_NAME(type, target) AAF_TYPE(target##type)
#include "AAFMetaDictionary.h"


//
// Done with records.
//

//
// Pass 10:  Do stuff for varying arrays.
//
#define AAF_TYPE_TABLE_BEGIN()  \
struct TypeVaryingArray         \
{                               \
  const wchar_t *  typeName;    \
  aafUID_t         typeId;      \
  const aafUID_t * pBaseTypeId; \
  int              isValid;     \
};                              \
                                \
static TypeVaryingArray s_AAFAllTypeVaryingArrays [] = {

#define AAF_TYPE(x) kAAFTypeID_##x

#define AAF_TYPE_DEFINITION_VARYING_ARRAY(name, id, type) \
  {L"aaf" L###name, id, &type, 1},

#define AAF_TYPE_TABLE_END()  \
0 };

#include "AAFMetaDictionary.h"

//
// Pass 11:  Do stuff for fixed arrays.
//
#define AAF_TYPE_TABLE_BEGIN()  \
struct TypeFixedArray           \
{                               \
  const wchar_t *  typeName;    \
  aafUID_t         typeId;      \
  const aafUID_t * pBaseTypeId; \
  aafUInt32        count;       \
  int              isValid;     \
};                              \
                                \
static TypeFixedArray s_AAFAllTypeFixedArrays [] = {

#define AAF_TYPE(x) kAAFTypeID_##x

#define AAF_TYPE_DEFINITION_FIXED_ARRAY(name, id, type, count) \
  {L"aaf" L###name, id, &type, count, 1},

#define AAF_TYPE_TABLE_END()  \
0 };

#include "AAFMetaDictionary.h"

//
// Pass 12:  Do stuff for rename types.
//
#define AAF_TYPE_TABLE_BEGIN()  \
struct TypeRename               \
{                               \
  const wchar_t *  typeName;    \
  aafUID_t         typeId;      \
  const aafUID_t * pBaseTypeId; \
  int              isValid;     \
};                              \
                                \
static TypeRename s_AAFAllTypeRenames [] = {

#define AAF_TYPE(x) kAAFTypeID_##x

#define AAF_TYPE_DEFINITION_RENAME(name, id, type) \
  {L"aaf" L###name, id, (aafUID_t *)& AAF_TYPE(type), 1},

#define AAF_TYPE_TABLE_END()  \
0 };

#include "AAFMetaDictionary.h"

//
// Pass 13:  Do stuff for string types.
//
#define AAF_TYPE_TABLE_BEGIN()  \
struct TypeString               \
{                               \
  const wchar_t *  typeName;    \
  aafUID_t         typeId;      \
  const aafUID_t * pBaseTypeId; \
  int              isValid;     \
};                              \
                                \
static TypeString s_AAFAllTypeStrings [] = {

#define AAF_TYPE(x) kAAFTypeID_##x

#define AAF_TYPE_DEFINITION_STRING(name, id, type) \
  {L"aaf" L###name, id, (aafUID_t *)& AAF_TYPE(type), 1},

#define AAF_TYPE_TABLE_END()  \
0 };

#include "AAFMetaDictionary.h"

//
// Pass 13a:  Do stuff for character types.
//
#define AAF_TYPE_TABLE_BEGIN()  \
struct TypeCharacter            \
{                               \
  const wchar_t * typeName;     \
  aafUID_t        typeID;       \
  aafUInt8        size;         \
  int             isValid;      \
};                              \
                                \
static TypeCharacter s_AAFAllTypeCharacters [] = {

#define AAF_TYPE_DEFINITION_CHARACTER(name, id) \
  {L"aaf" L###name, id, 2, 1},

#define AAF_TYPE_TABLE_END()  \
0 };

#include "AAFMetaDictionary.h"

//
// Pass 13b:  Do stuff for indirect types. (there should be only one!)
//
#define AAF_TYPE_TABLE_BEGIN()  \
struct TypeIndirect             \
{                               \
  const wchar_t * typeName;     \
  aafUID_t        typeID;       \
  int             isValid;      \
};                              \
                                \
static TypeIndirect s_AAFAllTypeIndirects [] = {

#define AAF_TYPE_DEFINITION_INDIRECT(name, id) \
  {L"aaf" L###name, id, 1},

#define AAF_TYPE_TABLE_END()  \
0 };

#include "AAFMetaDictionary.h"

//
// Pass 13c:  Do stuff for opaque types. (there should be only one!)
//
#define AAF_TYPE_TABLE_BEGIN()  \
struct TypeOpaque             \
{                               \
  const wchar_t * typeName;     \
  aafUID_t        typeID;       \
  int             isValid;      \
};                              \
                                \
static TypeOpaque s_AAFAllTypeOpaques [] = {

#define AAF_TYPE_DEFINITION_OPAQUE(name, id) \
  {L"aaf" L###name, id, 1},

#define AAF_TYPE_TABLE_END()  \
0 };

#include "AAFMetaDictionary.h"

//
// Pass 14:  Do stuff for strong ref types.
//
#define AAF_TYPE_TABLE_BEGIN()  \
struct TypeStrongRef            \
{                               \
  const wchar_t *  typeName;    \
  aafUID_t         typeId;      \
  const aafUID_t * pRefdTypeId; \
  int              isValid;     \
};                              \
                                \
static TypeStrongRef s_AAFAllTypeStrongRefs [] = {

#define AAF_TYPE(x) AUID_AAF##x
#define AAF_REFERENCE_TYPE(type, target) kAAFTypeID_target##type
#define AAF_REFERENCE_TYPE_NAME(type, target)  L"kAAFTypeID_" L###target L###type

#define AAF_TYPE_DEFINITION_STRONG_REFERENCE(name, id, type) \
  {name, id, (aafUID_t *)& type, 1},

#define AAF_TYPE_TABLE_END()  \
0 };

#include "AAFMetaDictionary.h"

//
// Pass 15:  Do stuff for strong ref set types.
//
#define AAF_TYPE_TABLE_BEGIN()  \
struct TypeStrongRefSet         \
{                               \
  const wchar_t *  typeName;    \
  aafUID_t         typeId;      \
  const aafUID_t * pRefdTypeId; \
  int              isValid;     \
};                              \
                                \
static TypeStrongRefSet s_AAFAllTypeStrongRefSets [] = {

#define AAF_TYPE(x) kAAFTypeID_##x##StrongReference
#define AAF_REFERENCE_TYPE_NAME(type, target)  L"kAAFTypeID_" L###target L###type

#define AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(name, id, type) \
  {name, id, (aafUID_t *)& type, 1},

#define AAF_TYPE_TABLE_END()  \
0 };

#include "AAFMetaDictionary.h"

//
// Pass 16:  Do stuff for strong ref vector types.
//
#define AAF_TYPE_TABLE_BEGIN()  \
struct TypeStrongRefVector      \
{                               \
  const wchar_t *  typeName;    \
  aafUID_t         typeId;      \
  const aafUID_t * pRefdTypeId; \
  int              isValid;     \
};                              \
                                \
static TypeStrongRefVector s_AAFAllTypeStrongRefVectors [] = {

#define AAF_TYPE(x) kAAFTypeID_##x##StrongReference
#define AAF_REFERENCE_TYPE_NAME(type, target)  L"kAAFTypeID_" L###target L###type

#define AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR(name, id, type) \
  {name, id, (aafUID_t *)& type, 1},

#define AAF_TYPE_TABLE_END()  \
0 };

#include "AAFMetaDictionary.h"

//
// Pass 17:  Do stuff for weak ref types.
//
#define AAF_TYPE_TABLE_BEGIN()  \
struct TypeWeakRef              \
{                               \
  const wchar_t *  typeName;    \
  aafUID_t         typeId;      \
  const aafUID_t * pRefdTypeId; \
  int              isValid;     \
};                              \
                                \
static TypeWeakRef s_AAFAllTypeWeakRefs [] = {

#define AAF_TYPE(x) AUID_AAF##x
#define AAF_REFERENCE_TYPE(type, target) kAAFTypeID_target##type
#define AAF_REFERENCE_TYPE_NAME(type, target)  L"kAAFTypeID_" L###target L###type

#define AAF_TYPE_DEFINITION_WEAK_REFERENCE(name, id, type) \
  {name, id, (aafUID_t *)& type, 1},

#define AAF_TYPE_TABLE_END()  \
0 };

#include "AAFMetaDictionary.h"

//
// Pass 18:  Do stuff for weak ref set types.
//
#define AAF_TYPE_TABLE_BEGIN()  \
struct TypeWeakRefSet           \
{                               \
  const wchar_t *  typeName;    \
  aafUID_t         typeId;      \
  const aafUID_t * pRefdTypeId; \
  int              isValid;     \
};                              \
                                \
static TypeWeakRefSet s_AAFAllTypeWeakRefSets [] = {

#define AAF_TYPE(x) kAAFTypeID_##x##WeakReference
#define AAF_REFERENCE_TYPE_NAME(type, target)  L"kAAFTypeID_" L###target L###type

#define AAF_TYPE_DEFINITION_WEAK_REFERENCE_SET(name, id, type) \
  {name, id, (aafUID_t *)& type, 1},

#define AAF_TYPE_TABLE_END()  \
0 };

#include "AAFMetaDictionary.h"

//
// Pass 19:  Do stuff for weak ref vector types.
//
#define AAF_TYPE_TABLE_BEGIN()  \
struct TypeWeakRefVector        \
{                               \
  const wchar_t *  typeName;    \
  aafUID_t         typeId;      \
  const aafUID_t * pRefdTypeId; \
  int              isValid;     \
};                              \
                                \
static TypeWeakRefVector s_AAFAllTypeWeakRefVectors [] = {

#define AAF_TYPE(x) kAAFTypeID_##x##WeakReference
#define AAF_REFERENCE_TYPE_NAME(type, target)  L"kAAFTypeID_" L###target L###type

#define AAF_TYPE_DEFINITION_WEAK_REFERENCE_VECTOR(name, id, type) \
  {name, id, (aafUID_t *)& type, 1},

#define AAF_TYPE_TABLE_END()  \
0 };

#include "AAFMetaDictionary.h"

//
// pass 20: For extendible enumerations, define individual enumeration
// fields
//

//
// Define structs to describe each member of an extendible enumeration
// typedef, and to describe the entire extendible enumeration typedef.
//
#define AAF_TYPE_TABLE_BEGIN()             \
                                           \
struct TypeExtEnumerationMember            \
{                                          \
  wchar_t *        memberName;             \
  aafUID_t         memberValue;            \
};                                         \
                                           \
struct TypeExtEnumeration                  \
{                                          \
  aafUID_t                 typeID;         \
  const wchar_t *          typeName;       \
  TypeExtEnumerationMember ** members;     \
};

#define AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_MEMBER(name, auid, container) \
static const TypeExtEnumerationMember s_TypeExtEnumerationMember_##container##_##name = \
{ L###name, \
  auid \
};

#define AAF_TYPE(x) aaf##x##_t
#define AAF_REFERENCE_TYPE(type, target)      AAF_TYPE(target##type)
#define AAF_REFERENCE_TYPE_NAME(type, target) AAF_TYPE(target##type)
#include "AAFMetaDictionary.h"

//
// pass 21: For extendible enumerations, create the (null-terminated)
// field list for each extendible enumeration
//

#define AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION(name, id) \
static const TypeExtEnumerationMember *s_TypeExtEnumerationMembers_##name[] = {

#define AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_MEMBER(name, value, container) \
	&s_TypeExtEnumerationMember_##container##_##name,

#define AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_END(name) \
  0 } ;

#define AAF_TYPE(x) _aaf##x##_t
#define AAF_REFERENCE_TYPE(type, target)      AAF_TYPE(target##type)
#define AAF_REFERENCE_TYPE_NAME(type, target) AAF_TYPE(target##type)
#include "AAFMetaDictionary.h"

//
// pass 22: For extendible enumerations, create each extendible
// enumeration definition
//

#define AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION(name, id) \
static const TypeExtEnumeration s_TypeExtEnumeration_##name = \
{ id, \
  L###name, \
  (TypeExtEnumerationMember **) s_TypeExtEnumerationMembers_##name, \
 };

#define AAF_TYPE(x) x
// #define AAF_REFERENCE_TYPE(type, target)      AAF_TYPE(target##type)
// #define AAF_REFERENCE_TYPE_NAME(type, target) AAF_TYPE(target##type)
#include "AAFMetaDictionary.h"

//
// pass 23: For extendible enumerations, create a master
// null-terminated list of all type def extendible enumerations
//

#define AAF_TYPE_TABLE_BEGIN()   \
static TypeExtEnumeration * s_AAFAllTypeExtEnumerations [] = {

#define AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION(name, id) \
  (TypeExtEnumeration*)& s_TypeExtEnumeration_##name,

#define AAF_TYPE_TABLE_END()   \
0 };

#define AAF_TYPE(x) _aaf##x##_t
#define AAF_REFERENCE_TYPE(type, target)      AAF_TYPE(target##type)
#define AAF_REFERENCE_TYPE_NAME(type, target) AAF_TYPE(target##type)
#include "AAFMetaDictionary.h"

//
// Done with extendible enumerations.
//

//
// pass 24: For stream types (transdel:2000-JUN-13)
//
struct TypeStream
{
  TypeStream(aafUID_constptr id,
             const wchar_t * name) :
    typeID(id),
    typeName(name)
  {}

  
  aafUID_constptr typeID;
  const wchar_t * typeName;
};

#define MY_TYPE_ID(name) &kAAFTypeID_##name
#define MY_TYPE_NAME(name) L###name


#define AAF_TYPE_TABLE_BEGIN()   \
static TypeStream s_AAFAllTypeStreams [] = \
{
#define AAF_TYPE_DEFINITION_STREAM(name, id) \
  TypeStream(MY_TYPE_ID(name), MY_TYPE_NAME(name)),
#define AAF_TYPE_TABLE_END() \
};

#include "AAFMetaDictionary.h"

#undef MY_TYPE_ID
#undef MY_TYPE_NAME

#endif // ! __ImplAAFTypeDefsGen_h__







/***********************************************************************
 *
 *              Copyright (c) 1998-2000 Avid Technology, Inc.
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
#include "AAFObjectModel.h"


#include "AAFResult.h"
#include "AAFStoredObjectIDs.h" // declare AUID's
#include "AAFPropertyDefs.h"
#include "AAFTypeDefUIDs.h"

#if defined(macintosh) || defined(_MAC)
#include <wstring.h>
#include <string.h> // memcmp?
#endif

#include <assert.h>
#include <stdlib.h>
#include <stddef.h>


// Needed for the auid of the "Root" class.
const static aafUID_t NULL3_AUID = { 0 };


// The following defines were copied from ImplAAFBuiltinTypes.cpp.
// We need to fix the object model asap!
//
// Some types which haven't been synchronized yet
#define aafAUID_t          aafUID_t
#define aafRectangle_t     aafRect_t

// aafRational_t struct field hax 
#define Numerator   numerator
#define Denominator denominator

// aafRect_t struct field hax
#define upperLeftHorizontal  xOffset
#define lowerRightHorizontal xSize
#define upperLeftVertical    yOffset
#define lowerRightVertical   ySize

//
// Here are a few common macros:
//
#define MY_ARRAY_ELEMENT_COUNT(array) \
  sizeof(array) / sizeof(array[0])





//
// Create a static table of all predefined extendible enumeration ids.
// NOTE: This should really already be defined elsewhere!
//
#define MY_EXTENDIBLE_ID(name, container) \
  kAAFExtendibleEnum_##container##_##name

#define AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_MEMBER(name, auid, container) \
const aafUID_t MY_EXTENDIBLE_ID(name, container) = auid;

#include "AAFMetaDictionary.h"

//#undef AAF_CLASS_SEPARATOR


//
// Create a static table of all property definitions.
//
#define MY_TYPE_NAME(name) L###name
#define MY_TYPE_ID(name) kAAFTypeID_##name
#define MY_CONTAINER_ID(container) AUID_AAF##container
#define MY_PROPERTY_ID(name, container) kAAFPropID_##container##_##name


#define AAF_TYPE(type) type
#define AAF_REFERENCE_TYPE(type, target) target##type
    
#define AAF_TABLE_BEGIN() \
static PropertyDefinition sPropertyDefinitions[] = \
{
#define AAF_PROPERTY(name, id, tag, type, mandatory, uid, container) \
    PropertyDefinition( \
    MY_TYPE_NAME(name), \
    &MY_PROPERTY_ID(name, container), \
    tag, \
    &MY_TYPE_ID(type), \
    mandatory, \
    uid, \
    &MY_CONTAINER_ID(container), \
    true),
#define AAF_TABLE_END() \
};


#include "AAFMetaDictionary.h"


#undef MY_TYPE_NAME
#undef MY_TYPE_ID
#undef MY_CONTAINER_ID
#undef MY_PROPERTY_ID

//
// Create a static table of the properties for each class.
//
#define AAF_TABLE_BEGIN() \
static aafUInt32 sPropertyIndex = 0;
    
#define AAF_CLASS(name, id, parent, concrete) \
static PropertyDefinition * s##name##_PropertyDefinitions[] = \
{

#define AAF_PROPERTY(name, id, tag, type, mandatory, uid, container) \
  &sPropertyDefinitions[sPropertyIndex++],

#define AAF_CLASS_END(name) \
  0 \
};

const aafUInt32 kPropertyDefinitionCount =
  sizeof (sPropertyDefinitions) / sizeof (sPropertyDefinitions[0]);


//
// Create an array of property definition pointers sorted by id.
// This is initialized the the meta dictionary.
// 
static PropertyDefinition * 
  sPropertyDefinitionsById[kPropertyDefinitionCount] = {0};

//
// Create an array of property definition pointers sorted by pid.
// This is initialized the the meta dictionary.
// 
static PropertyDefinition * 
  sPropertyDefinitionsByPid[kPropertyDefinitionCount] = {0};


#include "AAFMetaDictionary.h"



//
// Create a static table of all class definitions.
//


// Special case: classes which descend from AAFRoot (actualy only
// AAFObject) should report NULL as their parent class.
#define AUID_AAFRoot NULL3_AUID

#define MY_TYPE_NAME(name) L###name
#define MY_CLASS_ID(name) AUID_AAF##name
#define MY_PARENT_ID(parent) MY_CLASS_ID(parent)


#define AAF_TABLE_BEGIN() \
static ClassDefinition sClassDefinitions[] = \
{
#define AAF_CLASS(name, id, parent, concrete) \
    ClassDefinition( \
    MY_TYPE_NAME(name), \
    &MY_CLASS_ID(name), \
    &MY_PARENT_ID(parent), \
    (bool)concrete, \
    (MY_ARRAY_ELEMENT_COUNT(s##name##_PropertyDefinitions)) - 1, \
    s##name##_PropertyDefinitions)
#define AAF_CLASS_SEPARATOR() ,
#define AAF_TABLE_END() };

#include "AAFMetaDictionary.h"

#undef MY_TYPE_NAME
#undef MY_CLASS_ID
#undef MY_PARENT_ID

const aafUInt32 kClassDefinitionCount =
  sizeof (sClassDefinitions) / sizeof (sClassDefinitions[0]);

//
// Create an array of class definition pointers sorted by id.
// This is initialized the the meta dictionary.
// 
static ClassDefinition * 
  sClassDefinitionsById[kClassDefinitionCount] = {0};



//
// We cannot create an array of TypeDefinitions since there are
// also subclasses. So we need to first build an array of 
// TypeDefinition pointers and then initialize this array
// from all of the subclass instances in the meta dictionary.
//
#define AAF_TYPE_TABLE_BEGIN() \
TypeDefinition* sTypeDefinitions [] = {
#define AAF_TYPE_DEFINITION_INTEGER(name, id, size, signed) \
  NULL
#define AAF_TYPE_DEFINITION_ENUMERATION(name, id, type) \
  NULL
#define AAF_TYPE_DEFINITION_FIXED_ARRAY(name, id, type, count) \
  NULL
#define AAF_TYPE_DEFINITION_VARYING_ARRAY(name, id, type) \
  NULL
#define AAF_TYPE_DEFINITION_RECORD(name, id) \
  NULL
#define AAF_TYPE_DEFINITION_RENAME(name, id, type) \
  NULL
#define AAF_TYPE_DEFINITION_STRING(name, id, type) \
  NULL
#define AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION(name, id) \
  NULL
#define AAF_TYPE_DEFINITION_CHARACTER(name, id) \
  NULL
#define AAF_TYPE_DEFINITION_SET(name, id, type) \
  NULL
#define AAF_TYPE_DEFINITION_STRONG_REFERENCE(name, id, type) \
  NULL
#define AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(name, id, type) \
  NULL
#define AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR(name, id, type) \
  NULL
#define AAF_TYPE_DEFINITION_WEAK_REFERENCE(name, id, type) \
  NULL
#define AAF_TYPE_DEFINITION_WEAK_REFERENCE_SET(name, id, type) \
  NULL
#define AAF_TYPE_DEFINITION_WEAK_REFERENCE_VECTOR(name, id, type) \
  NULL
#define AAF_TYPE_DEFINITION_STREAM(name, id) \
  NULL
#define AAF_TYPE_DEFINITION_INDIRECT(name, id) \
  NULL
#define AAF_TYPE_DEFINITION_OPAQUE(name, id) \
  NULL
#define AAF_TYPE_SEPARATOR() ,
#define AAF_TYPE_TABLE_END() };
#define AAF_TYPE(name) name

#include "AAFMetaDictionary.h"


const aafUInt32 kTypeDefinitionCount = MY_ARRAY_ELEMENT_COUNT(sTypeDefinitions);

// Allocate a secord array that well be sorted by type id.
static TypeDefinition * sTypeDefinitionsById[kTypeDefinitionCount];



//
// Initialize an array for each derived type definition. As
// each new type is constructed a reference to it is added
// sTypeDefinitions.
// 



//
// Create an array of all Integer types.
//

#define MY_TYPE_NAME(name) L###name
#define MY_TYPE_ID(name) kAAFTypeID_##name
#define MY_TYPE_SIGN(signed) kAAF##signed

#define AAF_TYPE_TABLE_BEGIN() \
TypeDefinitionInteger sTypeDefinitionIntegers [] = \
{
#define AAF_TYPE_DEFINITION_INTEGER(name, id, size, signed) \
    TypeDefinitionInteger( \
    MY_TYPE_NAME(name), \
    &MY_TYPE_ID(name), \
    size, \
    MY_TYPE_SIGN(signed)),
#define AAF_TYPE_TABLE_END() \
};

#include "AAFMetaDictionary.h"

#undef MY_TYPE_NAME
#undef MY_TYPE_ID
#undef MY_TYPE_SIGN

const aafUInt32 kTypeDefinitionIntegerCount = MY_ARRAY_ELEMENT_COUNT(sTypeDefinitionIntegers);


//
// Create an array of all Enumeration type members.
//

#define MY_TYPE_NAME(name) L###name
#define MY_CONTAINER_ID(container) kAAFTypeID_##container
#define AAF_TYPE(name) kAAFTypeID_##name

#define AAF_TYPE_TABLE_BEGIN() \
static DefinitionEnumerationMember sTypeDefinitionEnumerationMembers [] = \
{
#define AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(name, value, container) \
    DefinitionEnumerationMember( \
    MY_TYPE_NAME(name), \
    value, \
    &MY_CONTAINER_ID(container)),
#define AAF_TYPE_TABLE_END() \
};

#include "AAFMetaDictionary.h"

#undef MY_TYPE_NAME
#undef MY_CONTAINER_ID

const aafUInt32 kTypeDefinitionEnumerationMemberCount = 
  MY_ARRAY_ELEMENT_COUNT(sTypeDefinitionEnumerationMembers);

//
// Create a static table of the members for each enumeration.
//
#define MY_ENUM_MEMBER_ARRAY(name) \
  s##name##_Member

#define AAF_TYPE_TABLE_BEGIN() \
static aafUInt32 sEnumerationMemberIndex = 0;
    
#define AAF_TYPE_DEFINITION_ENUMERATION(name, id, type) \
static DefinitionEnumerationMember * MY_ENUM_MEMBER_ARRAY(name)[] = \
{
#define AAF_TYPE_DEFINITION_ENUMERATION_MEMBER(name, value, container) \
  &sTypeDefinitionEnumerationMembers[sEnumerationMemberIndex++],
#define AAF_TYPE_DEFINITION_ENUMERATION_END(name) \
  0 \
};


#include "AAFMetaDictionary.h"


//
// Create an array of all Enumeration types.
//

#define MY_TYPE_NAME(name) L###name
#define MY_TYPE_ID(name) kAAFTypeID_##name
#define AAF_TYPE(name) kAAFTypeID_##name

#define AAF_TYPE_TABLE_BEGIN() \
TypeDefinitionEnumeration sTypeDefinitionEnumerations [] = \
{
#define AAF_TYPE_DEFINITION_ENUMERATION(name, id, type) \
    TypeDefinitionEnumeration( \
    MY_TYPE_NAME(name), \
    &MY_TYPE_ID(name), \
    &type, \
    (MY_ARRAY_ELEMENT_COUNT(MY_ENUM_MEMBER_ARRAY(name))) - 1, \
    MY_ENUM_MEMBER_ARRAY(name)),
#define AAF_TYPE_TABLE_END() \
};

#include "AAFMetaDictionary.h"

#undef MY_TYPE_NAME
#undef MY_TYPE_ID
#undef MY_ENUM_MEMBER_ARRAY // defined above

const aafUInt32 kTypeDefinitionEnumerationCount = 
  MY_ARRAY_ELEMENT_COUNT(sTypeDefinitionEnumerations);


//
// Create an array of all Variable Array types.
//
#define MY_TYPE_NAME(name) L###name
#define MY_TYPE_ID(name) kAAFTypeID_##name
#define AAF_TYPE(name) kAAFTypeID_##name

#define AAF_TYPE_TABLE_BEGIN() \
static TypeDefinitionVariableArray sTypeDefinitionVariableArrays [] = \
{
#define AAF_TYPE_DEFINITION_VARYING_ARRAY(name, id, elementType) \
    TypeDefinitionVariableArray( \
    MY_TYPE_NAME(name), \
    &MY_TYPE_ID(name), \
    &elementType),
#define AAF_TYPE_TABLE_END() \
};

#include "AAFMetaDictionary.h"

#undef MY_TYPE_NAME
#undef MY_TYPE_ID

const aafUInt32 kTypeDefinitionVariableArrayCount = 
  MY_ARRAY_ELEMENT_COUNT(sTypeDefinitionVariableArrays);


//
// Create an array of all Fixed Array types.
//
#define MY_TYPE_NAME(name) L###name
#define MY_TYPE_ID(name) kAAFTypeID_##name
#define AAF_TYPE(type) MY_TYPE_ID(type)

#define AAF_TYPE_TABLE_BEGIN() \
static TypeDefinitionFixedArray sTypeDefinitionFixedArrays [] = \
{
#define AAF_TYPE_DEFINITION_FIXED_ARRAY(name, id, elementType, count) \
    TypeDefinitionFixedArray( \
    MY_TYPE_NAME(name), \
    &MY_TYPE_ID(name), \
    &elementType, \
    count),
#define AAF_TYPE_TABLE_END() \
};

#include "AAFMetaDictionary.h"

#undef MY_TYPE_NAME
#undef MY_TYPE_ID

const aafUInt32 kTypeDefinitionFixedArrayCount = 
  MY_ARRAY_ELEMENT_COUNT(sTypeDefinitionFixedArrays);


//
// Create an array of all Record type members.
//

#define MY_TYPE_NAME(name) L###name
#define MY_CONTAINER_ID(container) &kAAFTypeID_##container
#define MY_FIELD_OFFSET(name, container) \
  offsetof(aaf##container##_t, name)

#define AAF_TYPE(name) &kAAFTypeID_##name
// We don't need to support AAF_REFERENCE_TYPE because references are not
// allowed inside of a record.

#define AAF_TYPE_TABLE_BEGIN() \
static DefinitionRecordField sTypeDefinitionRecordFields [] = \
{
#define AAF_TYPE_DEFINITION_RECORD_FIELD(name, type, container) \
    DefinitionRecordField( \
    MY_TYPE_NAME(name), \
    type, \
    MY_CONTAINER_ID(container), \
    MY_FIELD_OFFSET(name, container)),
#define AAF_TYPE_TABLE_END() \
};

#include "AAFMetaDictionary.h"

#undef MY_TYPE_NAME
#undef MY_CONTAINER_ID
#undef MY_FIELD_OFFSET

const aafUInt32 kTypeDefinitionRecordFieldCount = 
  MY_ARRAY_ELEMENT_COUNT(sTypeDefinitionRecordFields);


//
// Create a static table of the fields for each record.
//
#define AAF_TYPE(name) \
  kAAFTypeID_##name

#define MY_RECORD_FIELD_ARRAY(name) \
  s##name##_Field

#define AAF_TYPE_TABLE_BEGIN() \
static aafUInt32 sRecordFieldIndex = 0;
    
#define AAF_TYPE_DEFINITION_RECORD(name, id) \
static DefinitionRecordField * \
  MY_RECORD_FIELD_ARRAY(name)[] = \
{

#define AAF_TYPE_DEFINITION_RECORD_FIELD(name, type, container) \
  &sTypeDefinitionRecordFields[sRecordFieldIndex++],

#define AAF_TYPE_DEFINITION_RECORD_END(name) \
  0 \
};

#include "AAFMetaDictionary.h"


//
// Create an array of all Record types.
//
#define MY_TYPE_NAME(name) L###name
#define MY_TYPE_ID(name) kAAFTypeID_##name
#define MY_RECORD_SIZE(name) \
  sizeof(aaf##name##_t)

#define AAF_TYPE_TABLE_BEGIN() \
static TypeDefinitionRecord sTypeDefinitionRecords [] = \
{
#define AAF_TYPE_DEFINITION_RECORD(name, id) \
    TypeDefinitionRecord( \
    MY_TYPE_NAME(name), \
    &MY_TYPE_ID(name), \
    MY_RECORD_SIZE(name), \
    (MY_ARRAY_ELEMENT_COUNT(MY_RECORD_FIELD_ARRAY(name))) - 1, \
    MY_RECORD_FIELD_ARRAY(name)),
#define AAF_TYPE_TABLE_END() \
};


#include "AAFMetaDictionary.h"

#undef MY_TYPE_NAME
#undef MY_TYPE_ID
#undef MY_RECORD_SIZE
#undef MY_RECORD_FIELD_ARRAY // defined above...

const aafUInt32 kTypeDefinitionRecordCount = 
  MY_ARRAY_ELEMENT_COUNT(sTypeDefinitionRecords);

#if 1
//
// Create an array of all Renamed types.
//
#define MY_TYPE_NAME(name) L###name
#define MY_TYPE_ID(name) kAAFTypeID_##name
#define AAF_TYPE(type) MY_TYPE_ID(type) /* NOT used for RENAME macro! */

#define AAF_TYPE_TABLE_BEGIN() \
static TypeDefinitionRename sTypeDefinitionRenames [] = \
{
#define AAF_TYPE_DEFINITION_RENAME(name, id, type) \
    TypeDefinitionRename( \
    MY_TYPE_NAME(name), \
    &MY_TYPE_ID(name), \
    &MY_TYPE_ID(type)),
#define AAF_TYPE_TABLE_END() \
};

#include "AAFMetaDictionary.h"

#undef MY_TYPE_NAME
#undef MY_TYPE_ID

const aafUInt32 kTypeDefinitionRenameCount = 
  MY_ARRAY_ELEMENT_COUNT(sTypeDefinitionRenames);
#endif

//
// Create an array of all string types.
//
#define MY_TYPE_NAME(name) L###name
#define MY_TYPE_ID(name) kAAFTypeID_##name
#define AAF_TYPE(type) MY_TYPE_ID(type) /* NOT used for STRING macro! */

#define AAF_TYPE_TABLE_BEGIN() \
static TypeDefinitionString sTypeDefinitionStrings [] = \
{
#define AAF_TYPE_DEFINITION_STRING(name, id, type) \
    TypeDefinitionString( \
    MY_TYPE_NAME(name), \
    &MY_TYPE_ID(name), \
    &MY_TYPE_ID(type)),
#define AAF_TYPE_TABLE_END() \
};

#include "AAFMetaDictionary.h"

#undef MY_TYPE_NAME
#undef MY_TYPE_ID

const aafUInt32 kTypeDefinitionStringCount = 
  MY_ARRAY_ELEMENT_COUNT(sTypeDefinitionStrings);



//
// Create an array of all Extendible Enumeration type members.
//
#define MY_TYPE_NAME(name) L###name
#define MY_CONTAINER_ID(container) kAAFTypeID_##container

#define AAF_TYPE_TABLE_BEGIN() \
static DefinitionExtendibleEnumerationMember sTypeDefinitionExtendibleEnumerationMembers [] = \
{
#define AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_MEMBER(name, auid, container) \
    DefinitionExtendibleEnumerationMember( \
    MY_TYPE_NAME(name), \
    &MY_EXTENDIBLE_ID(name, container), \
    &MY_CONTAINER_ID(container)),
#define AAF_TYPE_TABLE_END() \
};

#include "AAFMetaDictionary.h"

#undef MY_TYPE_NAME
#undef MY_CONTAINER_ID
#undef MY_EXTENDIBLE_ID // defined above...

const aafUInt32 kTypeDefinitionExtendibleEnumerationMemberCount = 
  MY_ARRAY_ELEMENT_COUNT(sTypeDefinitionExtendibleEnumerationMembers);


//
// Create a static table of the members for each enumeration.
//
#define MY_EXTENDIBLE_MEMBER_ARRAY(name) \
  sExtensible##name##_Member

#define AAF_TYPE_TABLE_BEGIN() \
static aafUInt32 sExtensibleEnumerationMemberIndex = 0;
    
#define AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION(name, id) \
static DefinitionExtendibleEnumerationMember * \
  MY_EXTENDIBLE_MEMBER_ARRAY(name)[] = \
{
#define AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_MEMBER(name, auid, container) \
  &sTypeDefinitionExtendibleEnumerationMembers[sExtensibleEnumerationMemberIndex++],
#define AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION_END(name) \
  0 \
};


#include "AAFMetaDictionary.h"

//
// Create an array of all string types.
//
#define MY_TYPE_NAME(name) L###name
#define MY_TYPE_ID(name) &kAAFTypeID_##name
#define MY_ARRAY_COUNT(name) \
  MY_ARRAY_ELEMENT_COUNT(MY_EXTENDIBLE_MEMBER_ARRAY(name))

#define AAF_TYPE_TABLE_BEGIN() \
static TypeDefinitionExtendibleEnumeration sTypeDefinitionExtendibleEnumerations [] = \
{
#define AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION(name, id) \
    TypeDefinitionExtendibleEnumeration( \
    MY_TYPE_NAME(name), \
    MY_TYPE_ID(name), \
    MY_ARRAY_COUNT(name), \
    MY_EXTENDIBLE_MEMBER_ARRAY(name)),
#define AAF_TYPE_TABLE_END() \
};

#include "AAFMetaDictionary.h"

#undef MY_TYPE_NAME
#undef MY_TYPE_ID
#undef MY_EXTENDIBLE_MEMBER_ARRAY
#undef MY_ARRAY_COUNT

const aafUInt32 kTypeDefinitionExtendibleEnumerationCount = 
  MY_ARRAY_ELEMENT_COUNT(sTypeDefinitionExtendibleEnumerations);


//
// Create an array of all Character types.
//
#define MY_TYPE_NAME(name) L###name
#define MY_TYPE_ID(name) &kAAFTypeID_##name

#define AAF_TYPE_TABLE_BEGIN() \
TypeDefinitionCharacter sTypeDefinitionCharacters [] = \
{
#define AAF_TYPE_DEFINITION_CHARACTER(name, id) \
    TypeDefinitionCharacter( \
    MY_TYPE_NAME(name), \
    MY_TYPE_ID(name)),
#define AAF_TYPE_TABLE_END() \
};

#include "AAFMetaDictionary.h"

#undef MY_TYPE_NAME
#undef MY_TYPE_ID

const aafUInt32 kTypeDefinitionCharacterCount = 
  MY_ARRAY_ELEMENT_COUNT(sTypeDefinitionCharacters);

#if 0 
// This is an abstract class. There should be NO instances in the
// Meta Dictionary or in an AAF file.
//
// Create an array of all set types.
//
#define MY_TYPE_NAME(name) L###name
#define MY_TYPE_ID(name) &kAAFTypeID_##name
#define AAF_TYPE(name)	MY_TYPE_ID(name)

#define AAF_TYPE_TABLE_BEGIN() \
static TypeDefinitionSet sTypeDefinitionSets [] = \
{
#define AAF_TYPE_DEFINITION_SET(name, id, type) \
    TypeDefinitionSet(MY_TYPE_NAME(name), \
    MY_TYPE_ID(name), \
    MY_TYPE_ID(type)),
#define AAF_TYPE_TABLE_END() \
};

#include "AAFMetaDictionary.h"

#undef MY_TYPE_NAME
#undef MY_TYPE_ID


const aafUInt32 kTypeDefinitionSetCount = 
  MY_ARRAY_ELEMENT_COUNT(sTypeDefinitionSets);
#endif


//
// Create an array of all strong reference types.
//

#define MY_TYPE_NAME(name) L###name
#define MY_TYPE_ID(name) kAAFTypeID_##name
#define MY_TARGET_ID(name) AUID_AAF##name

#define AAF_TYPE(name) name
#define AAF_REFERENCE_TYPE_NAME(type, target) target##type


#define AAF_TYPE_TABLE_BEGIN()  \
static TypeDefinitionStrongReference sTypeDefinitionStrongReferences [] = \
{
#define AAF_TYPE_DEFINITION_STRONG_REFERENCE(name, id, type) \
    TypeDefinitionStrongReference( \
    MY_TYPE_NAME(name), \
    (aafUID_constptr) &MY_TYPE_ID(name), \
    (aafUID_constptr) &MY_TARGET_ID(type)),
#define AAF_TYPE_TABLE_END() \
};

#include "AAFMetaDictionary.h"

#undef MY_TYPE_NAME
#undef MY_TYPE_ID
#undef MY_TARGET_ID


const aafUInt32 kTypeDefinitionStrongReferenceCount = 
  MY_ARRAY_ELEMENT_COUNT(sTypeDefinitionStrongReferences);


//
// Create an array of all strong reference set types.
//

#define MY_TYPE_NAME(name) L###name
#define MY_TYPE_ID(name) kAAFTypeID_##name
#define MY_TARGET_ID(type) MY_TYPE_ID(type)

#define AAF_TYPE(name) name##StrongReference
#define AAF_REFERENCE_TYPE_NAME(type, target) target##type


#define AAF_TYPE_TABLE_BEGIN()  \
static TypeDefinitionStrongReferenceSet sTypeDefinitionStrongReferenceSets [] = \
{
#define AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(name, id, type) \
    TypeDefinitionStrongReferenceSet( \
    MY_TYPE_NAME(name), \
    (aafUID_constptr) &MY_TYPE_ID(name), \
    (aafUID_constptr) &MY_TARGET_ID(type)),
#define AAF_TYPE_TABLE_END() \
};

#include "AAFMetaDictionary.h"

#undef MY_TYPE_NAME
#undef MY_TYPE_ID
#undef MY_TARGET_ID


const aafUInt32 kTypeDefinitionStrongReferenceSetCount = 
  MY_ARRAY_ELEMENT_COUNT(sTypeDefinitionStrongReferenceSets);


//
// Create an array of all strong reference vector types.
//

#define MY_TYPE_NAME(name) L###name
#define MY_TYPE_ID(name) kAAFTypeID_##name
//#define MY_TARGET_ID(name) AUID_AAF##name
#define MY_TARGET_ID(type) MY_TYPE_ID(type)

//#define AAF_TYPE(name) name
#define AAF_TYPE(name) name##StrongReference
#define AAF_REFERENCE_TYPE_NAME(type, target) target##type


#define AAF_TYPE_TABLE_BEGIN()  \
static TypeDefinitionStrongReferenceVector sTypeDefinitionStrongReferenceVectors [] = \
{
#define AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR(name, id, type) \
    TypeDefinitionStrongReferenceVector( \
    MY_TYPE_NAME(name), \
    (aafUID_constptr) &MY_TYPE_ID(name), \
    (aafUID_constptr) &MY_TARGET_ID(type)),
#define AAF_TYPE_TABLE_END() \
};

#include "AAFMetaDictionary.h"

#undef MY_TYPE_NAME
#undef MY_TYPE_ID
#undef MY_TARGET_ID


const aafUInt32 kTypeDefinitionStrongReferenceVectorCount = 
  MY_ARRAY_ELEMENT_COUNT(sTypeDefinitionStrongReferenceVectors);


//
// Create an array of all weak reference types.
//

#define MY_TYPE_NAME(name) L###name
#define MY_TYPE_ID(name) kAAFTypeID_##name
#define MY_TARGET_ID(name) AUID_AAF##name

#define AAF_TYPE(name) name
#define AAF_REFERENCE_TYPE_NAME(type, target) target##type


#define AAF_TYPE_TABLE_BEGIN()  \
static TypeDefinitionWeakReference sTypeDefinitionWeakReferences [] = \
{
#define AAF_TYPE_DEFINITION_WEAK_REFERENCE(name, id, type) \
    TypeDefinitionWeakReference( \
    MY_TYPE_NAME(name), \
    (aafUID_constptr) &MY_TYPE_ID(name), \
    (aafUID_constptr) &MY_TARGET_ID(type)),
#define AAF_TYPE_TABLE_END() \
};

#include "AAFMetaDictionary.h"

#undef MY_TYPE_NAME
#undef MY_TYPE_ID
#undef MY_TARGET_ID


const aafUInt32 kTypeDefinitionWeakReferenceCount = 
  MY_ARRAY_ELEMENT_COUNT(sTypeDefinitionWeakReferences);


//
// Create an array of all weak reference set types.
//

#define MY_TYPE_NAME(name) L###name
#define MY_TYPE_ID(name) kAAFTypeID_##name
//#define MY_TARGET_ID(name) AUID_AAF##name
#define MY_TARGET_ID(type) MY_TYPE_ID(type)

//#define AAF_TYPE(name) name
#define AAF_TYPE(name) name##WeakReference
#define AAF_REFERENCE_TYPE_NAME(type, target) target##type


#define AAF_TYPE_TABLE_BEGIN()  \
static TypeDefinitionWeakReferenceSet sTypeDefinitionWeakReferenceSets [] = \
{
#define AAF_TYPE_DEFINITION_WEAK_REFERENCE_SET(name, id, type) \
    TypeDefinitionWeakReferenceSet( \
    MY_TYPE_NAME(name), \
    (aafUID_constptr) &MY_TYPE_ID(name), \
    (aafUID_constptr) &MY_TARGET_ID(type)),
#define AAF_TYPE_TABLE_END() \
};

#include "AAFMetaDictionary.h"

#undef MY_TYPE_NAME
#undef MY_TYPE_ID
#undef MY_TARGET_ID


const aafUInt32 kTypeDefinitionWeakReferenceSetCount = 
  MY_ARRAY_ELEMENT_COUNT(sTypeDefinitionWeakReferenceSets);


//
// Create an array of all weak reference vector types.
//

#define MY_TYPE_NAME(name) L###name
#define MY_TYPE_ID(name) kAAFTypeID_##name
//#define MY_TARGET_ID(name) AUID_AAF##name
#define MY_TARGET_ID(type) MY_TYPE_ID(type)

//#define AAF_TYPE(name) name
#define AAF_TYPE(name) name##WeakReference
#define AAF_REFERENCE_TYPE_NAME(type, target) target##type


#define AAF_TYPE_TABLE_BEGIN()  \
static TypeDefinitionWeakReferenceVector sTypeDefinitionWeakReferenceVectors [] = \
{
#define AAF_TYPE_DEFINITION_WEAK_REFERENCE_VECTOR(name, id, type) \
    TypeDefinitionWeakReferenceVector( \
    MY_TYPE_NAME(name), \
    (aafUID_constptr) &MY_TYPE_ID(name), \
    (aafUID_constptr) &MY_TARGET_ID(type)),
#define AAF_TYPE_TABLE_END() \
};

#include "AAFMetaDictionary.h"

#undef MY_TYPE_NAME
#undef MY_TYPE_ID
#undef MY_TARGET_ID


const aafUInt32 kTypeDefinitionWeakReferenceVectorCount = 
  MY_ARRAY_ELEMENT_COUNT(sTypeDefinitionWeakReferenceVectors);


//
// Create an array of all stream types.
//

#define MY_TYPE_NAME(name) L###name
#define MY_TYPE_ID(name) kAAFTypeID_##name


#define AAF_TYPE_TABLE_BEGIN()  \
static TypeDefinitionStream sTypeDefinitionStreams [] = \
{
#define AAF_TYPE_DEFINITION_STREAM(name, id) \
    TypeDefinitionStream( \
    MY_TYPE_NAME(name), \
    (aafUID_constptr) &MY_TYPE_ID(name), \
    (aafUID_constptr) &kAAFTypeID_UInt8),
#define AAF_TYPE_TABLE_END() \
};

#include "AAFMetaDictionary.h"

#undef MY_TYPE_NAME
#undef MY_TYPE_ID


const aafUInt32 kTypeDefinitionStreamCount = 
  MY_ARRAY_ELEMENT_COUNT(sTypeDefinitionStreams);


//
// Create an array of all indirect types.
//

#define MY_TYPE_NAME(name) L###name
#define MY_TYPE_ID(name) kAAFTypeID_##name


#define AAF_TYPE_TABLE_BEGIN()  \
static TypeDefinitionIndirect sTypeDefinitionIndirects [] = \
{
#define AAF_TYPE_DEFINITION_INDIRECT(name, id) \
    TypeDefinitionIndirect( \
    MY_TYPE_NAME(name), \
    (aafUID_constptr) &MY_TYPE_ID(name)),
#define AAF_TYPE_TABLE_END() \
};

#include "AAFMetaDictionary.h"

#undef MY_TYPE_NAME
#undef MY_TYPE_ID


const aafUInt32 kTypeDefinitionIndirectCount = 
  MY_ARRAY_ELEMENT_COUNT(sTypeDefinitionIndirects);

//
// Create an array of all opaque types.
//

#define MY_TYPE_NAME(name) L###name
#define MY_TYPE_ID(name) kAAFTypeID_##name


#define AAF_TYPE_TABLE_BEGIN()  \
static TypeDefinitionOpaque sTypeDefinitionOpaques [] = \
{
#define AAF_TYPE_DEFINITION_OPAQUE(name, id) \
    TypeDefinitionOpaque( \
    MY_TYPE_NAME(name), \
    (aafUID_constptr) &MY_TYPE_ID(name)),
#define AAF_TYPE_TABLE_END() \
};

#include "AAFMetaDictionary.h"

#undef MY_TYPE_NAME
#undef MY_TYPE_ID


const aafUInt32 kTypeDefinitionOpaqueCount = 
  MY_ARRAY_ELEMENT_COUNT(sTypeDefinitionOpaques);



//
// Allocate the global array for all definitions
//
const aafUInt32 kDefinitionCount = kClassDefinitionCount
                                 + kPropertyDefinitionCount
                                 + kTypeDefinitionCount;

static Definition * sDefinitionsById[kDefinitionCount];


//
// class for all of the meta class definitions.
//
class DefinitionKey : public Definition
{
public:
  DefinitionKey(aafCharacter_constptr name, 
                  aafUID_constptr id, 
                  bool concrete) :
    Definition (name, id, concrete)
  {}

  virtual void Initialize (void);
  virtual const ClassDefinition *classDefinition(void) const;
  static const DefinitionKey * null(void); // return sentinel for this class.
  virtual bool isNil(void) const;
};


//
// Define all of the "null" definition sentinels.
//
static aafUID_t NilDefinitionID = 
  {0x11111111, 0x1111, 0x1111, {0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11}};
static aafUID_t NilClassDefinitionID = 
  {0x22222222, 0x2222, 0x2222, {0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22}};
static aafUID_t NilPropertyDefinitionID = 
  {0x33333333, 0x3333, 0x3333, {0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33}};
static aafUID_t NilTypeDefinitionID = 
  {0x44444444, 0x4444, 0x4444, {0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44}};

static const DefinitionKey sNilDefinition(
  L"*** Definition not found *** [ERROR]",
  &NilDefinitionID,
  false /*not concrete*/);

static const ClassDefinition sNilClassDefinition(
  L"*** ClassDefinition not found *** [ERROR]",
  &NilClassDefinitionID,
  &NilClassDefinitionID,
  false, /*not concrete*/
  0,
  NULL);

static const PropertyDefinition sNilPropertyDefinition(
  L"*** PropertyDefinition not found *** [ERROR]",
  &NilPropertyDefinitionID,
  0x0000,
  &NilTypeDefinitionID,
  false,
  false,
  &NilClassDefinitionID,
  false /*not concrete*/);

static const TypeDefinition sNilTypeDefinition(
  L"*** TypeDefinition not found *** [ERROR]",
  &NilTypeDefinitionID,
  false /*not concrete*/);



void DefinitionKey::Initialize()
{
}

const ClassDefinition *DefinitionKey::classDefinition(void) const
{
  return objectModel()->findClassDefinition(&AUID_AAFMetaDefinition);
}


const DefinitionKey *
  DefinitionKey::null(void) 
{
  return &sNilDefinition;
}

bool DefinitionKey::isNil(void) const
{
  return (this == null());
}


///////////////////////////////////////////////////////////////////////////////
// The last step:
// Construct and initialize the our private instance of
// the meta dictionary. 
///////////////////////////////////////////////////////////////////////////////
class AAFObjectModelConstructor
{
  AAFObjectModel _objectModel;

public:
  AAFObjectModelConstructor() {}
  const AAFObjectModel *objectModel(void) const {return &_objectModel;}
};
AAFObjectModelConstructor sObjectModelConstructor;


static int compareId(const void* d1, const void* d2)
{
  Definition* def1 = *((Definition **)(d1));
  Definition* def2 = *((Definition **)(d2));
  return memcmp(def1->id(), def2->id(), sizeof (aafUID_t));
}

static int comparePid(const void* d1, const void* d2)
{
  PropertyDefinition* def1 = *((PropertyDefinition **)(d1));
  PropertyDefinition* def2 = *((PropertyDefinition **)(d2));
  return (int)def1->pid() - (int)def2->pid(), sizeof (aafUID_t);
}


///////////////////////////////////////////////////////////////////////////////
// class AAFObjectModel
///////////////////////////////////////////////////////////////////////////////

void AAFObjectModel::SortClassDefinitions(void)
{
  aafUInt32 i;

  // Initialize the sorted array of class definitions by id
  for (i = 0; i < countClassDefinitions(); ++i)
    sClassDefinitionsById[i] = &sClassDefinitions[i];
  // Sort the array of pointers...
  qsort(sClassDefinitionsById,
      countClassDefinitions(),
      sizeof(sClassDefinitionsById[0]),
      compareId);

#ifndef NDEBUG
  //
  // Validate that all of the classId's are unique.
  //
  for (i = 0; i < countClassDefinitions(); ++i)
  {
    const ClassDefinition *currentClass = classDefinitionAt(i);
    assert (0 != currentClass);
    const ClassDefinition *foundClass = findClassDefinition(currentClass->id());
    assert (0 != foundClass);
    assert (foundClass == currentClass);
  }
#endif
}


void AAFObjectModel::InitializeClassDefinitions(void)
{
  // Initialize the sorted array of class definitions by id
  ClassDefinition *classDefinition;
  for (aafUInt32 i = 0; i < countClassDefinitions(); ++i)
  {
    classDefinition = &sClassDefinitions[i];
    classDefinition->Initialize();
  }
}


void AAFObjectModel::SortPropertyDefinitions(void)
{
  aafUInt32 i;

  // Initialize the sorted array of class definitions by id
  for (i = 0; i < countPropertyDefinitions(); ++i)
    sPropertyDefinitionsById[i] = &sPropertyDefinitions[i];
  // Sort the array of pointers...
  qsort(sPropertyDefinitionsById,
      countPropertyDefinitions(),
      sizeof(sPropertyDefinitionsById[0]),
      compareId);

  // Initialize the sorted array of class definitions by id
  for (i = 0; i < countPropertyDefinitions(); ++i)
    sPropertyDefinitionsByPid[i] = &sPropertyDefinitions[i];
  // Sort the array of pointers...
  qsort(sPropertyDefinitionsByPid,
      countPropertyDefinitions(),
      sizeof(sPropertyDefinitionsByPid[0]),
      comparePid);



#ifndef NDEBUG
  //
  // Validate that all of the property id's are unique.
  //
  for (i = 0; i < countPropertyDefinitions(); ++i)
  {
    const PropertyDefinition *currentProperty = propertyDefinitionAt(i);
    assert (0 != currentProperty);
    const PropertyDefinition *foundProperty = findPropertyDefinition(currentProperty->id());
    assert (0 != foundProperty);
    assert (foundProperty == currentProperty);
  }

  //
  // Validate that all of the property id's are unique.
  //
  aafUInt16 previousPid = 0;
  aafUInt16 currentPid = 0;
  for (i = 0; i < countPropertyDefinitions(); ++i)
  {
    currentPid = sPropertyDefinitionsByPid[i]->pid();
    assert (currentPid != previousPid);
    previousPid = currentPid;
  }
#endif

}


void AAFObjectModel::InitializePropertyDefinitions(void)
{
  // Initialize the sorted array of property definitions by id
  PropertyDefinition *propertyDefinition;
  for (aafUInt32 i = 0; i < countPropertyDefinitions(); ++i)
  {
    propertyDefinition = &sPropertyDefinitions[i];
    propertyDefinition->Initialize();
  }
}


void AAFObjectModel::SortTypeDefinitions(void)
{
  aafUInt32 i;
  // Before we can sort we fill in the sTypeDefinitions array
  // with pointers from all of the derived type arrays.
  aafUInt32 actualTypeCount = 0;

  // TypeDefinitionInteger
  for (i = 0; i < kTypeDefinitionIntegerCount; ++i)
  {
    sTypeDefinitions[actualTypeCount++] = &sTypeDefinitionIntegers[i];
  }

  // TypeDefinitionEnumeration
  for (i = 0; i < kTypeDefinitionEnumerationCount; ++i)
  {
    sTypeDefinitions[actualTypeCount++] = &sTypeDefinitionEnumerations[i];
  }

  // TypeDefinitionVariableArray
  for (i = 0; i < kTypeDefinitionVariableArrayCount; ++i)
  {
    sTypeDefinitions[actualTypeCount++] = &sTypeDefinitionVariableArrays[i];
  }

  // TypeDefinitionFixedArray
  for (i = 0; i < kTypeDefinitionFixedArrayCount; ++i)
  {
    sTypeDefinitions[actualTypeCount++] = &sTypeDefinitionFixedArrays[i];
  }

  // TypeDefinitionRecord
  for (i = 0; i < kTypeDefinitionRecordCount; ++i)
  {
    sTypeDefinitions[actualTypeCount++] = &sTypeDefinitionRecords[i];
  }

  // TypeDefinitionRename
  for (i = 0; i < kTypeDefinitionRenameCount; ++i)
  {
    sTypeDefinitions[actualTypeCount++] = &sTypeDefinitionRenames[i];
  }

  // TypeDefinitionString
  for (i = 0; i < kTypeDefinitionStringCount; ++i)
  {
    sTypeDefinitions[actualTypeCount++] = &sTypeDefinitionStrings[i];
  }

  // TypeDefinitionExtendibleEnumeration
  for (i = 0; i < kTypeDefinitionExtendibleEnumerationCount; ++i)
  {
    sTypeDefinitions[actualTypeCount++] = &sTypeDefinitionExtendibleEnumerations[i];
  }

  // TypeDefinitionCharacter
  for (i = 0; i < kTypeDefinitionCharacterCount; ++i)
  {
    sTypeDefinitions[actualTypeCount++] = &sTypeDefinitionCharacters[i];
  }

  // TypeDefinitionStrongReference
  for (i = 0; i < kTypeDefinitionStrongReferenceCount; ++i)
  {
    sTypeDefinitions[actualTypeCount++] = &sTypeDefinitionStrongReferences[i];
  }

  // TypeDefinitionStrongReferenceSet
  for (i = 0; i < kTypeDefinitionStrongReferenceSetCount; ++i)
  {
    sTypeDefinitions[actualTypeCount++] = &sTypeDefinitionStrongReferenceSets[i];
  }

  // TypeDefinitionStrongReferenceVector
  for (i = 0; i < kTypeDefinitionStrongReferenceVectorCount; ++i)
  {
    sTypeDefinitions[actualTypeCount++] = &sTypeDefinitionStrongReferenceVectors[i];
  }

  // TypeDefinitionWeakReference
  for (i = 0; i < kTypeDefinitionWeakReferenceCount; ++i)
  {
    sTypeDefinitions[actualTypeCount++] = &sTypeDefinitionWeakReferences[i];
  }

  // TypeDefinitionWeakReferenceSet
  for (i = 0; i < kTypeDefinitionWeakReferenceSetCount; ++i)
  {
    sTypeDefinitions[actualTypeCount++] = &sTypeDefinitionWeakReferenceSets[i];
  }

  // TypeDefinitionWeakReferenceVector
  for (i = 0; i < kTypeDefinitionWeakReferenceVectorCount; ++i)
  {
    sTypeDefinitions[actualTypeCount++] = &sTypeDefinitionWeakReferenceVectors[i];
  }

  // TypeDefinitionStream
  for (i = 0; i < kTypeDefinitionStreamCount; ++i)
  {
    sTypeDefinitions[actualTypeCount++] = &sTypeDefinitionStreams[i];
  }

  // TypeDefinitionIndirect
  for (i = 0; i < kTypeDefinitionIndirectCount; ++i)
  {
    sTypeDefinitions[actualTypeCount++] = &sTypeDefinitionIndirects[i];
  }

  // TypeDefinitionOpaque
  for (i = 0; i < kTypeDefinitionOpaqueCount; ++i)
  {
    sTypeDefinitions[actualTypeCount++] = &sTypeDefinitionOpaques[i];
  }

  
  //
  // Validate that we have created the correct number of type definitions.
  //
  assert (countTypeDefinitions() == actualTypeCount);


  // Initialize the sorted array of class definitions by id
  for (i = 0; i < countTypeDefinitions(); ++i)
    sTypeDefinitionsById[i] = sTypeDefinitions[i];
  // Sort the array of pointers...
  qsort(sTypeDefinitionsById,
      countTypeDefinitions(),
      sizeof(sTypeDefinitionsById[0]),
      compareId);



#ifndef NDEBUG
  //
  // Validate that all of the type id's are unique.
  //
  for (i = 0; i < countTypeDefinitions(); ++i)
  {
    const TypeDefinition *currentType = typeDefinitionAt(i);
    assert (0 != currentType);
    const TypeDefinition *foundType = findTypeDefinition(currentType->id());
    assert (0 != foundType);
    assert (foundType == currentType);
  }
#endif

}


void AAFObjectModel::SortDefinitions(void)
{
  SortClassDefinitions();
  SortPropertyDefinitions();
  SortTypeDefinitions();


  //
  // First initialize the elements of the definition array.
  //
  aafUInt32 actualDefinitionCount = 0;
  aafUInt32 i;
  for (i = 0; i < countClassDefinitions(); ++i)
    sDefinitionsById[actualDefinitionCount++] = const_cast<ClassDefinition *>(classDefinitionAt(i));

  for (i = 0; i < countPropertyDefinitions(); ++i)
    sDefinitionsById[actualDefinitionCount++] = const_cast<PropertyDefinition *>(propertyDefinitionAt(i));

  for (i = 0; i < countTypeDefinitions(); ++i)
    sDefinitionsById[actualDefinitionCount++] = const_cast<TypeDefinition *>(typeDefinitionAt(i));
  
  //
  // Validate that we have created the correct number of definitions.
  //
  assert(actualDefinitionCount == countDefinitions());

  // Sort the array of pointers...
  qsort(sDefinitionsById,
      countDefinitions(),
      sizeof(sDefinitionsById[0]),
      compareId);


#ifndef NDEBUG
  //
  // Validate that all of the classId's are unique.
  //
  for (i = 0; i < countDefinitions(); ++i)
  {
    const Definition *currentDefinition = sDefinitionsById[i];
    assert (0 != currentDefinition);
    const Definition *foundDefinition = findDefinition(currentDefinition->id());
    assert (0 != foundDefinition);
    assert (foundDefinition == currentDefinition);
  }
#endif

}


void AAFObjectModel::InitializeTypeDefinitions(void)
{
  // Initialize the sorted array of type definitions by id
  TypeDefinition *typeDefinition;
  for (aafUInt32 i = 0; i < countTypeDefinitions(); ++i)
  {
    typeDefinition = sTypeDefinitions[i];
    assert (typeDefinition);
    typeDefinition->Initialize();
  }
}

void AAFObjectModel::InitializeAxiomaticDefinitions(void)
{
  // There is only one meta dictionary and it must be internally
  // created.
  findClassDefinition(&AUID_AAFMetaDictionary)->makeAxiomatic();

  // We know that the class definition for all class definitions is
  // axiomatic. This should also make property definitions axiomatic
  // and all associated type definitions.
//  findClassDefinition(&AUID_AAFClassDefinition)->makeAxiomatic();
//  findClassDefinition(&AUID_AAFPropertyDefinition)->makeAxiomatic();


  // There really should only be 8 instances of TypeDefinitionInteger.
  // Forse all of them to be aximatic.
  findTypeDefinition(&kAAFTypeID_UInt8)->makeAxiomatic();
  findTypeDefinition(&kAAFTypeID_UInt16)->makeAxiomatic();
  findTypeDefinition(&kAAFTypeID_UInt32)->makeAxiomatic();
  findTypeDefinition(&kAAFTypeID_UInt64)->makeAxiomatic();
  findTypeDefinition(&kAAFTypeID_Int8)->makeAxiomatic();
  findTypeDefinition(&kAAFTypeID_Int16)->makeAxiomatic();
  findTypeDefinition(&kAAFTypeID_Int32)->makeAxiomatic();
  findTypeDefinition(&kAAFTypeID_Int64)->makeAxiomatic();

  // Other single instance types...
  findTypeDefinition(&kAAFTypeID_Character)->makeAxiomatic();
  findTypeDefinition(&kAAFTypeID_String)->makeAxiomatic();
  findTypeDefinition(&kAAFTypeID_Stream)->makeAxiomatic();
  findTypeDefinition(&kAAFTypeID_Indirect)->makeAxiomatic();
  findTypeDefinition(&kAAFTypeID_Opaque)->makeAxiomatic();

  // Add the following two classes since the we still have the 
  // type, class and property definitions stored in the AAF dictionary.
  // When we move type, class and property definitions to another
  // object "above" header we can remove the following two classes
  // then their dependent property, type and class definitions.
  // (transdel 2000-MAR-27)
  //findClassDefinition(&AUID_AAFHeader)->makeAxiomatic();
  //findClassDefinition(&AUID_AAFDictionary)->makeAxiomatic();
}


typedef struct _findCyclicDependenciesData
{
  const Definition *_startingDefinition; // The first definition to be
  aafUInt32 _maxCount; // Count at which we must stop. (detects loop within dependents).
  aafUInt32 _count; // (dependents + 1)
  aafUInt32 _selfReferences;  // number of times that the starting definition is its own dependent.
} findCyclicDependenciesUserData;

static bool findCyclicDependencies(const Definition *definition, 
                                   findCyclicDependenciesUserData *userData)
{
  // The number of times this function has been called.
  ++userData->_count;

  // Detect cyclic dependency count for the starting definition.
  if (userData->_startingDefinition == definition)
  {
    if (++userData->_selfReferences > 1)
      return false;
  }

  if (userData->_count > userData->_maxCount)
    return false;

  return true;
}

void AAFObjectModel::InitializeCyclicDefinitions(void)
{
  aafUInt32 index;


  // Set the maximum count to an "impossible" value. Just in case
  // there is a class that depends on every other definition including
  // itself.
  findCyclicDependenciesUserData userData;
  userData._maxCount = countClassDefinitions()
                     + countPropertyDefinitions()
                     + countTypeDefinitions()
                     + 1;

  const ClassDefinition *classDefinition;
  for (index = 0; index < countClassDefinitions(); ++index)
  {
    classDefinition = classDefinitionAt(index);

    userData._startingDefinition = classDefinition;
    userData._count = 0;
    userData._selfReferences = 0;

    classDefinition->visitPreOrder((VisitDefinitionProcType)findCyclicDependencies, (void *)&userData);
    if (userData._selfReferences > 1)
    {
      // has a cycle; it depends on itself
      const_cast<ClassDefinition *>(classDefinition)->setCyclic(true);
    }
    if (userData._count > userData._maxCount)
    {
      // has a cycle; at least one dependent depends on itself.
      const_cast<ClassDefinition *>(classDefinition)->setContainsCycles(true);
    }
  }
}


void AAFObjectModel::InitializeDefinitions(void)
{
  SortDefinitions();

  InitializeClassDefinitions();
  InitializeTypeDefinitions();
  InitializePropertyDefinitions();
  InitializeAxiomaticDefinitions();
  InitializeCyclicDefinitions();
}



//
// 
//
AAFObjectModel::AAFObjectModel()
{
  InitializeDefinitions();
}



const AAFObjectModel * AAFObjectModel::singleton(void)
{
  return (sObjectModelConstructor.objectModel()); // return a reference
}



aafUInt32 AAFObjectModel::countClassDefinitions(void) const
{
  return kClassDefinitionCount;
}

// Access the class definitions in the order that they are 
// initialized from MetaDictionary.h
const ClassDefinition * 
  AAFObjectModel::classDefinitionAt (aafUInt32 index) const
{
  assert (index < countClassDefinitions());
  return &sClassDefinitions[index];
}


aafUInt32 AAFObjectModel::countPropertyDefinitions(void) const
{
  return kPropertyDefinitionCount;
}

// Access the property definitions in the order that they are 
// initialized from MetaDictionary.h
const PropertyDefinition * 
  AAFObjectModel::propertyDefinitionAt (aafUInt32 index) const
{
  assert (index < countPropertyDefinitions());
  return &sPropertyDefinitions[index];
}

aafUInt32 AAFObjectModel::countTypeDefinitions(void) const
{
  return kTypeDefinitionCount;
}

// Access the property definitions in the order that they are 
// initialized from MetaDictionary.h
const TypeDefinition * 
  AAFObjectModel::typeDefinitionAt (aafUInt32 index) const
{
  assert (index < countTypeDefinitions());
  return sTypeDefinitions[index];
}



aafUInt32 AAFObjectModel::countDefinitions(void) const
{
  return kDefinitionCount;
}

aafUInt32 AAFObjectModel::countAxiomaticClassDefinitions(void) const
{
  static aafUInt32 axiomaticClassCount = 0;

  if (!axiomaticClassCount)
  {
    for (aafUInt32 i = 0; i < countClassDefinitions(); ++i)
    {
      if (classDefinitionAt(i)->axiomatic())
        ++axiomaticClassCount;
    }
  }

  return axiomaticClassCount;
}

aafUInt32 AAFObjectModel::countAxiomaticPropertyDefinitions(void) const
{
  static aafUInt32 axiomaticPropertyCount = 0;

  if (!axiomaticPropertyCount)
  {
    for (aafUInt32 i = 0; i < countPropertyDefinitions(); ++i)
    {
      if (propertyDefinitionAt(i)->axiomatic())
        ++axiomaticPropertyCount;
    }
  }

  return axiomaticPropertyCount;
}

aafUInt32 AAFObjectModel::countAxiomaticTypeDefinitions(void) const
{
  static aafUInt32 axiomaticTypeCount = 0;

  if (!axiomaticTypeCount)
  {
    for (aafUInt32 i = 0; i < countTypeDefinitions(); ++i)
    {
      if (typeDefinitionAt(i)->axiomatic())
        ++axiomaticTypeCount;
    }
  }

  return axiomaticTypeCount;
}


aafUInt32 AAFObjectModel::countAxiomaticDefinitions(void) const
{
  static aafUInt32 axiomaticCount = 0;

  if (!axiomaticCount)
  {
    axiomaticCount = countAxiomaticClassDefinitions()
                   + countAxiomaticPropertyDefinitions()
                   + countAxiomaticTypeDefinitions();
  }

  return axiomaticCount;
}


//
// AAFObjectModel search routines:
//


//
// Find the type definition by its guid.
//
const TypeDefinition * 
  AAFObjectModel::findTypeDefinition (aafUID_constptr id) const
{
  const TypeDefinition *result = NULL;
  TypeDefinition **ppResult = NULL;
  TypeDefinition key(L"KEY", id, false);
  TypeDefinition *pKey = &key;
  
  // Use standard library's binary search routine.
  ppResult = (TypeDefinition **)bsearch(
                &pKey, 
                sTypeDefinitionsById, 
                countTypeDefinitions(),
                sizeof(TypeDefinition *),
                compareId);

  // Return the corresponding class id.
  if (NULL != ppResult)
    result = *ppResult;
  else
    result = TypeDefinition::null();

  return (result);
}


//
// Find the class definition by its guid.
//
const ClassDefinition * 
  AAFObjectModel::findClassDefinition (aafUID_constptr id) const
{
  const ClassDefinition *result = NULL;
  ClassDefinition **ppResult = NULL;
  ClassDefinition key(L"KEY", id, NULL, false, 0, NULL);
  ClassDefinition *pKey = &key;
  
  // Use standard library's binary search routine.
  ppResult = (ClassDefinition **)bsearch(
                &pKey, 
                sClassDefinitionsById, 
                countClassDefinitions(),
                sizeof(ClassDefinition *),
                compareId);

  // Return the corresponding class id.
  if (NULL != ppResult)
    result = *ppResult;
  else
    result = ClassDefinition::null();

  return (result);
}


//
// Find the property definition by its guid.
//
const PropertyDefinition * 
  AAFObjectModel::findPropertyDefinition (aafUID_constptr id) const
{
  const PropertyDefinition *result = NULL;
  PropertyDefinition **ppResult = NULL;
  PropertyDefinition key(L"KEY", id, 0, NULL, false, false, NULL, false);
  PropertyDefinition *pKey = &key;
  
  // Use standard library's binary search routine.
  ppResult = (PropertyDefinition **)bsearch(
                &pKey, 
                sPropertyDefinitionsById, 
                countPropertyDefinitions(),
                sizeof(PropertyDefinition *),
                compareId);

  // Return the corresponding class id.
  if (NULL != ppResult)
    result = *ppResult;
  else
    result = PropertyDefinition::null();

  return (result);
}


//
// Find the property definition by its short, but unique, pid.
//
const PropertyDefinition * 
  AAFObjectModel::findPropertyDefinition (aafUInt16 pid) const
{
  const PropertyDefinition *result = NULL;
  PropertyDefinition **ppResult = NULL;
  PropertyDefinition key(L"KEY", NULL, pid, NULL, false, false, NULL, false);
  PropertyDefinition *pKey = &key;
  
  // Use standard library's binary search routine.
  ppResult = (PropertyDefinition **)bsearch(
                &pKey, 
                sPropertyDefinitionsByPid, 
                countPropertyDefinitions(),
                sizeof(PropertyDefinition *),
                comparePid);

  // Return the corresponding class id.
  if (NULL != ppResult)
    result = *ppResult;
  else
    result = PropertyDefinition::null();

  return (result);
}





//
// Find the class definition by its guid.
//
const Definition * 
  AAFObjectModel::findDefinition (aafUID_constptr id) const
{
  const Definition *result = NULL;
  Definition **ppResult = NULL;
  DefinitionKey key(L"KEY", id, false);
  Definition *pKey = &key;
  
  // Use standard library's binary search routine.
  ppResult = (Definition **)bsearch(
                &pKey, 
                sDefinitionsById, 
                countDefinitions(),
                sizeof(Definition *),
                compareId);

  // Return the corresponding class id.
  if (NULL != ppResult)
    result = *ppResult;
  else
    result = DefinitionKey::null();

  return (result);
}

///////////////////////////////////////////////////////////////////////////////
// class Definition
///////////////////////////////////////////////////////////////////////////////

//
// Abstract base class for all of the meta classes.
//
const AAFObjectModel *Definition::objectModel() const
{
  return AAFObjectModel::singleton(); 
}


void Definition::makeAxiomatic (void) const
{
  if (!axiomatic())
  {
    const_cast<Definition *>(this)->setAxiomatic(true);

    // Make this definition's class axiomatic
    classDefinition()->makeAxiomatic();
  }
}



// Call the create proc with the given userData and return the result.
bool Definition::createDefinition(void *userData) const
{
  // Call the create proc if one if available.
  if (_createDefinitionProc)
  {
    return (*_createDefinitionProc)(this, userData);
  }
  else
  {
    return false;
  }
}


// Call the initialize proc with the given userData and return the result.
bool Definition::initializeDefinition(void *userData) const
{
  // Call the initialization proc if one if available.
  if (_initializeDefinitionProc)
  {
    return (*_initializeDefinitionProc)(this, userData);
  }
  else
  {
    return false;
  }
}



// Walk the tree of definitions pre-order, call the given function for 
// the current definition before any dependent definitions.
bool Definition::visitPreOrder(VisitDefinitionProcType f, void *userData) const
{
  assert(NULL != f);

  // Call the visit proc for this type definition.
  // Assume there are no dependents. Derived classes that have dependents should
  // overrided this method.
  (void)(*f)(this, userData);
  return true;
}

// Walk the tree of definitions post-order, call the given function for 
// the current definition after any dependent definitions.
bool Definition::visitPostOrder(VisitDefinitionProcType f, void *userData) const
{
  assert(NULL != f);

  // Call the visit proc for this definition.
  // Assume there are no dependents. Derived classes that have dependents should
  // overrided this method.
  return (*f)(this, userData);
}


//
// ClassDefinition
//


// only the member data of this object
void ClassDefinition::Initialize (void)
{
  // PRECONDITION: we should only initialize once.
  assert (!_parentClass);

  // Lookup the parent class...
  _parentClass = objectModel()->findClassDefinition (parentId ());

  // POSTCONDITION
  assert (_parentClass || 
       (!_parentClass && 0 == memcmp(parentId(), &NULL3_AUID, sizeof(NULL3_AUID))));
}

void ClassDefinition::makeAxiomatic (void) const
{
  if (!axiomatic())
  {
    // Make this definition and its class definition axiomatic
    //Definition::makeAxiomatic();
    const_cast<ClassDefinition *>(this)->setAxiomatic(true);

    // Make parent class and its properties axiomatic first...
    if (!isRoot())
      parentClass()->makeAxiomatic(); //(RECURSIVE)
 
    // Make all of the property definitions axiomatic.
    for (aafUInt32 i = 0; i < propertyCount(); ++i)
      propertyDefinitionAt(i)->makeAxiomatic();
  }
}

void ClassDefinition::makePropertiesAxiomatic(void) const
{
  // Recurse until we reach the root.
  if (isNil())
    return;
  
  // Make parent class' properties axiomatic first...
  // Do not mark parent classes axiomatic.
  if (!isRoot())
    parentClass()->makePropertiesAxiomatic();
 
  // Make all of the property definitions axiomatic.
  for (aafUInt32 i = 0; i < propertyCount(); ++i)
    propertyDefinitionAt(i)->makeAxiomatic();
}


const ClassDefinition *
  ClassDefinition::classDefinition(void) const
{
  return objectModel()->findClassDefinition (&AUID_AAFClassDefinition);
}

const ClassDefinition *
  ClassDefinition::null(void) 
{
  return &sNilClassDefinition;
}

bool ClassDefinition::isNil(void) const
{
  return (this == null());
}

// Is this the root object (InterchangeObject)
bool ClassDefinition::isRoot() const
{
  // The root object does not have a valid "parent" class.
  assert (_parentClass);
  return parentClass()->isNil();
}


// Return true if this is the class definition for a uniquely identified object.
bool ClassDefinition::isUniquelyIdentified(void) const
{
  return (NULL != uniqueIdentifierProperty());
}

// Return the unique identifier property for this class definition (if any).
// Return NULL if this class 
const PropertyDefinition *ClassDefinition::uniqueIdentifierProperty(void) const
{
  const PropertyDefinition *result = NULL;

  // Find the first unique identifier property in this class 
  // definition.
  if (!isRoot())
    result = parentClass()->uniqueIdentifierProperty(); //(RECURSIVE)

  // If the unique identifier was not found in the parent class then
  // search the properties of this class.
  if (!result)
  {
    for (aafUInt32 i = 0; i < propertyCount(); ++i)
    {
      if (propertyDefinitionAt(i)->uid())
      {
        result = propertyDefinitionAt(i);
        break;
      }
    }
  }

  return result;
}



// Walk the tree of definitions pre-order, call the given function for 
// the current definition before any dependent definitions.
// NOTE: This method does NOT visit parent the parent class definition.
bool ClassDefinition::visitPreOrder(VisitDefinitionProcType f, void *userData) const
{
  assert(NULL != f);
  bool bContinue = false;

  // Call the visit proc for this definition.
  bContinue = (*f)(this, userData);
  if (bContinue)
  {
    // Call the visit proc for each property definition.
    for (aafUInt32 i = 0; (i < propertyCount()) /*&& bContinue */; ++i)
    {
      bContinue = propertyDefinitionAt(i)->visitPreOrder(f, userData);
    }
  }

  return true;
}

// Walk the tree of definitions post-order, call the given function for 
// the current definition after any dependent definitions.
// NOTE: This method does NOT visit parent the parent class definition.
bool ClassDefinition::visitPostOrder(VisitDefinitionProcType f, void *userData) const
{
  assert(NULL != f);
  bool bContinue = false;

  // Call the visit proc for each property definition.
  // NOTE: Should we acutally visit the properties in "reverse"
  // order for "real" post-order traversal?
  for (aafUInt32 i = (propertyCount() - 1); (i >= 0) && bContinue; --i)
  {
    bContinue = propertyDefinitionAt(i)->visitPostOrder(f, userData);
  }

  if (bContinue)
  { // Call the visit proc for this definition.
    bContinue = (*f)(this, userData);
  }

  return bContinue;
}




// Access the property definitions in the order that they are 
// initialized from MetaDictionary.h
const PropertyDefinition * 
ClassDefinition::propertyDefinitionAt (aafUInt32 index) const
{
  assert (index < propertyCount());
  assert (NULL != _propertyDefinitions[index]);
  return _propertyDefinitions[index];
}


const PropertyDefinition * 
  ClassDefinition::findPropertyDefinition (aafUID_constptr id) const
{
  const PropertyDefinition *result = NULL;
  const PropertyDefinition *propertyDefinition;
  for (aafUInt32 i = 0; i < propertyCount(); ++i)
  {
    propertyDefinition = propertyDefinitionAt(i);
    assert (NULL != propertyDefinition);
    if (id == propertyDefinition->id())
    {
      result = propertyDefinition;
      break;
    }
  }

  if (!result)
    result = PropertyDefinition::null();

  return (result);
}

const PropertyDefinition * 
  ClassDefinition::findPropertyDefinition (aafUInt16 pid) const
{
  const PropertyDefinition *result = NULL;
  const PropertyDefinition *propertyDefinition;
  for (aafUInt32 i = 0; i < propertyCount(); ++i)
  {
    propertyDefinition = propertyDefinitionAt(i);
    assert (NULL != propertyDefinition);
    if (pid == propertyDefinition->pid())
    {
      result = propertyDefinition;
      break;
    }
  }

  if (!result)
    result = PropertyDefinition::null();

  return (result);
}



//
// PropertyDefinition
//

// only the member data of this object
void PropertyDefinition::Initialize (void)
{
  // PRECONDITION: we should only initialize once.
  assert (!_container);

  // Lookup the container class...
  _container = objectModel()->findClassDefinition (classId ());

  // Lookup the type...
  _typeDefinition = objectModel()->findTypeDefinition (typeId ());

  // POSTCONDITION: if the class and type definitions do not exist then
  // the meta dictionary is invalid.
  assert (_typeDefinition);
  assert (_container);
}

void PropertyDefinition::makeAxiomatic (void) const
{
  if (!axiomatic())
  {
    // Make this definition and its class definition axiomatic
    Definition::makeAxiomatic();
    
    // Make the associated type definition axiomatic.
    typeDefinition()->makeAxiomatic();
  }
}


const ClassDefinition *
  PropertyDefinition::classDefinition(void) const
{
  return objectModel()->findClassDefinition (&AUID_AAFPropertyDefinition);
}

const PropertyDefinition *
  PropertyDefinition::null(void) 
{
  return &sNilPropertyDefinition;
}

bool PropertyDefinition::isNil(void) const
{
  return (this == null());
}


// Walk the tree of definitions pre-order, call the given function for 
// the current definition before any dependent definitions.
bool PropertyDefinition::visitPreOrder(VisitDefinitionProcType f, void *userData) const
{
  assert(NULL != f);
  bool bContinue = false;

  // Call the visit proc for this property definition.
  bContinue = (*f)(this, userData);
  if (bContinue)
  {
    // Call the visit proc for this property's type definition.
    bContinue = typeDefinition()->visitPreOrder(f, userData);
  }

  return true;
}

// Walk the tree of definitions post-order, call the given function for 
// the current definition after any dependent definitions.
bool PropertyDefinition::visitPostOrder(VisitDefinitionProcType f, void *userData) const
{
  assert(NULL != f);
  bool bContinue = false;

  // Call the visit proc for this property's type definition.
  bContinue = typeDefinition()->visitPostOrder(f, userData);
  if (bContinue)
  {
    // Call the visit proc for this property definition.
    bContinue = (*f)(this, userData);
  }

  return bContinue;
}


//
// TypeDefinition
//

// only the member data of this object
void TypeDefinition::Initialize (void)
{
}

const ClassDefinition *
  TypeDefinition::classDefinition(void) const
{
  return objectModel()->findClassDefinition (&AUID_AAFTypeDefinition);
}

const TypeDefinition *
  TypeDefinition::null(void) 
{
  return &sNilTypeDefinition;
}

bool TypeDefinition::isNil(void) const
{
  return (this == null());
}


//
// TypeDefinitionInteger
//

// only the member data of this object
void TypeDefinitionInteger::Initialize (void)
{
  // Just validate that the given size is supported.
  assert ((size() == 1) || 
          (size() == 2) ||
          (size() == 4) ||
          (size() == 8));
}

const ClassDefinition *
  TypeDefinitionInteger::classDefinition(void) const
{
  return objectModel()->findClassDefinition (&AUID_AAFTypeDefinitionInteger);
}


//
// TypeDefinitionEnumeration
//

// only the member data of this object
void TypeDefinitionEnumeration::Initialize (void)
{
  // PRECONDITION: we should only initialize once.
  assert (!_typeDefinition);

  // Lookup the type definition; it must be one of the integer types.
  const TypeDefinition *type;
  type = objectModel()->findTypeDefinition (typeId ());
  _typeDefinition = dynamic_cast<const TypeDefinitionInteger *>(type);

  // POSTCONDITION: if the type definition does not exist then either
  // the meta dictionary is invalid or the sort/search mechnism is 
  // broken.
  assert (_typeDefinition);
}

const ClassDefinition *
  TypeDefinitionEnumeration::classDefinition(void) const
{
  return objectModel()->findClassDefinition (&AUID_AAFTypeDefinitionEnumeration);
}


const DefinitionEnumerationMember * 
  TypeDefinitionEnumeration::memberAt(aafUInt32 index) const
{
  assert(index < memberCount());
  return _members[index];
}

void DefinitionEnumerationMember::Initialize()
{
}

void TypeDefinitionEnumeration::makeAxiomatic (void) const
{
  if (!axiomatic())
  {
    // Make this definition and its class definition axiomatic
    Definition::makeAxiomatic();
    
    // Make the associated type definition axiomatic.
    typeDefinition()->makeAxiomatic();
  }
}


// Walk the tree of definitions pre-order, call the given function for 
// the current definition before any dependent definitions.
bool TypeDefinitionEnumeration::visitPreOrder(VisitDefinitionProcType f, void *userData) const
{
  assert(NULL != f);
  bool bContinue = false;

  // Call the visit proc for this type definition.
  bContinue = (*f)(this, userData);
  if (bContinue)
  {
    // Call the visit proc for this type's element type definition.
    bContinue = typeDefinition()->visitPreOrder(f, userData);
  }

  return true;
}

// Walk the tree of definitions post-order, call the given function for 
// the current definition after any dependent definitions.
bool TypeDefinitionEnumeration::visitPostOrder(VisitDefinitionProcType f, void *userData) const
{
  assert(NULL != f);
  bool bContinue = false;

  // Call the visit proc for this type's element type definition.
  bContinue = typeDefinition()->visitPostOrder(f, userData);
  if (bContinue)
  {
    // Call the visit proc for this type definition.
    bContinue = (*f)(this, userData);
  }

  return bContinue;
}

void TypeDefinitionVariableArray::Initialize(void)
{
  // PRECONDITION: we should only initialize once.
  assert (!_elementType);

  // Lookup the type definition; it must be one of the integer types.
  _elementType = objectModel()->findTypeDefinition (elementTypeId ());

  // POSTCONDITION: if the type definition does not exist then either
  // the meta dictionary is invalid or the sort/search mechnism is 
  // broken.
  assert (_elementType);
}

void TypeDefinitionVariableArray::makeAxiomatic (void) const
{
  if (!axiomatic())
  {
    // Make this definition and its class definition axiomatic
    Definition::makeAxiomatic();
    
    // Make the associated type definition axiomatic.
    elementType()->makeAxiomatic();
  }
}

const ClassDefinition *
  TypeDefinitionVariableArray::classDefinition(void) const
{
  return objectModel()->findClassDefinition (&AUID_AAFTypeDefinitionVariableArray);
}



const ClassDefinition *
  TypeDefinitionFixedArray::classDefinition(void) const
{
  return objectModel()->findClassDefinition (&AUID_AAFTypeDefinitionFixedArray);
}


// Walk the tree of definitions pre-order, call the given function for 
// the current definition before any dependent definitions.
bool TypeDefinitionVariableArray::visitPreOrder(VisitDefinitionProcType f, void *userData) const
{
  assert(NULL != f);
  bool bContinue = false;

  // Call the visit proc for this type definition.
  bContinue = (*f)(this, userData);
  if (bContinue)
  {
    // Call the visit proc for this type's element type definition.
    bContinue = elementType()->visitPreOrder(f, userData);
  }

  return true;
}

// Walk the tree of definitions post-order, call the given function for 
// the current definition after any dependent definitions.
bool TypeDefinitionVariableArray::visitPostOrder(VisitDefinitionProcType f, void *userData) const
{
  assert(NULL != f);
  bool bContinue = false;

  // Call the visit proc for this type's element type definition.
  bContinue = elementType()->visitPostOrder(f, userData);
  if (bContinue)
  {
    // Call the visit proc for this type definition.
    bContinue = (*f)(this, userData);
  }

  return bContinue;
}


void TypeDefinitionRecord::Initialize(void)
{
  // Make all of the record field type definitions axiomatic.
  for (aafUInt32 i = 0; i < fieldCount(); ++i)
  { 
    assert(NULL != _fields[i]);
    _fields[i]->Initialize();
  }
}

void TypeDefinitionRecord::makeAxiomatic (void) const
{
  if (!axiomatic())
  {
    // Make this definition and its class definition axiomatic
    Definition::makeAxiomatic();

    // Make all of the record field type definitions axiomatic.
    for (aafUInt32 i = 0; i < fieldCount(); ++i)
      fieldAt(i)->typeDefinition()->makeAxiomatic();
  }
}


const ClassDefinition *
  TypeDefinitionRecord::classDefinition(void) const
{
  return objectModel()->findClassDefinition (&AUID_AAFTypeDefinitionRecord);
}


// Walk the tree of definitions pre-order, call the given function for 
// the current definition before any dependent definitions.
bool TypeDefinitionRecord::visitPreOrder(VisitDefinitionProcType f, void *userData) const
{
  assert(NULL != f);
  bool bContinue = false;

  // Call the visit proc for this type definition.
  bContinue = (*f)(this, userData);
  if (bContinue)
  {
    // Call the visit proc for this record's field type definitions.
    for (aafUInt32 i = 0; (i < fieldCount()) /*&& bContinue */; ++i)
    {
      bContinue = fieldAt(i)->typeDefinition()->visitPreOrder(f, userData);
    }
  }

  return true;
}

// Walk the tree of definitions post-order, call the given function for 
// the current definition after any dependent definitions.
bool TypeDefinitionRecord::visitPostOrder(VisitDefinitionProcType f, void *userData) const
{
  assert(NULL != f);
  bool bContinue = false;

  // Call the visit proc for this record's field type definitions.
  for (aafUInt32 i = (fieldCount() - 1); (i >= 0) && bContinue; ++i)
  {
    bContinue = fieldAt(i)->typeDefinition()->visitPostOrder(f, userData);
  }

  if (bContinue)
  { // Call the visit proc for this definition.
    bContinue = (*f)(this, userData);
  }

  return bContinue;
}


const DefinitionRecordField * 
  TypeDefinitionRecord::fieldAt(aafUInt32 index) const
{
  assert(index < fieldCount());
  return _fields[index];
}

void DefinitionRecordField::Initialize()
{
  // PRECONDITION: we should only initialize once.
  assert (!_typeDefinition);

  // Lookup the type definition; it must be one of the integer types.
  _typeDefinition = AAFObjectModel::singleton()->findTypeDefinition (typeId());

  // POSTCONDITION: if the type definition does not exist then either
  // the meta dictionary is invalid or the sort/search mechnism is 
  // broken.
  assert (_typeDefinition);
}

void TypeDefinitionRename::Initialize()
{
  // PRECONDITION: we should only initialize once.
  assert (!_renamedType);

  // Lookup the type definition; it must be one of the integer types.
  _renamedType = objectModel()->findTypeDefinition (renamedTypeId());

  // POSTCONDITION: if the type definition does not exist then
  // the meta dictionary is invalid
  assert (_renamedType);
}

void TypeDefinitionRename::makeAxiomatic (void) const
{
  if (!axiomatic())
  {
    // Make this definition and its class definition axiomatic
    Definition::makeAxiomatic();
    
    // Make the associated type definition axiomatic.
    renamedType()->makeAxiomatic();
  }
}

const ClassDefinition *
  TypeDefinitionRename::classDefinition(void) const
{
  return objectModel()->findClassDefinition (&AUID_AAFTypeDefinitionRename);
}


// Walk the tree of definitions pre-order, call the given function for 
// the current definition before any dependent definitions.
bool TypeDefinitionRename::visitPreOrder(VisitDefinitionProcType f, void *userData) const
{
  assert(NULL != f);
  bool bContinue = false;

  // Call the visit proc for this type definition.
  bContinue = (*f)(this, userData);
  if (bContinue)
  {
    // Call the visit proc for this type's element type definition.
    bContinue = renamedType()->visitPreOrder(f, userData);
  }

  return true;
}

// Walk the tree of definitions post-order, call the given function for 
// the current definition after any dependent definitions.
bool TypeDefinitionRename::visitPostOrder(VisitDefinitionProcType f, void *userData) const
{
  assert(NULL != f);
  bool bContinue = false;

  // Call the visit proc for this type's element type definition.
  bContinue = renamedType()->visitPostOrder(f, userData);
  if (bContinue)
  {
    // Call the visit proc for this type definition.
    bContinue = (*f)(this, userData);
  }

  return bContinue;
}


void TypeDefinitionString::Initialize()
{
  // PRECONDITION: we should only initialize once.
  assert (!_stringType);

  // Lookup the type definition; it must be one of the character types.
  _stringType = dynamic_cast<const TypeDefinitionCharacter *>
                          (objectModel()->findTypeDefinition (stringTypeId()));

  // POSTCONDITION: if the type definition does not exist then
  // the meta dictionary is invalid
  assert (_stringType);
}

void TypeDefinitionString::makeAxiomatic (void) const
{
  if (!axiomatic())
  {
    // Make this definition and its class definition axiomatic
    Definition::makeAxiomatic();
    
    // Make the associated type definition axiomatic.
    stringType()->makeAxiomatic();
  }
}

const ClassDefinition *
  TypeDefinitionString::classDefinition(void) const
{
  return objectModel()->findClassDefinition (&AUID_AAFTypeDefinitionString);
}


// Walk the tree of definitions pre-order, call the given function for 
// the current definition before any dependent definitions.
bool TypeDefinitionString::visitPreOrder(VisitDefinitionProcType f, void *userData) const
{
  assert(NULL != f);
  bool bContinue = false;

  // Call the visit proc for this type definition.
  bContinue = (*f)(this, userData);
  if (bContinue)
  {
    // Call the visit proc for this type's element type definition.
    bContinue = stringType()->visitPreOrder(f, userData);
  }

  return true;
}

// Walk the tree of definitions post-order, call the given function for 
// the current definition after any dependent definitions.
bool TypeDefinitionString::visitPostOrder(VisitDefinitionProcType f, void *userData) const
{
  assert(NULL != f);
  bool bContinue = false;

  // Call the visit proc for this type's element type definition.
  bContinue = stringType()->visitPostOrder(f, userData);
  if (bContinue)
  {
    // Call the visit proc for this type definition.
    bContinue = (*f)(this, userData);
  }

  return bContinue;
}


void TypeDefinitionExtendibleEnumeration::Initialize()
{
}

const ClassDefinition *
  TypeDefinitionExtendibleEnumeration::classDefinition(void) const
{
  return objectModel()->findClassDefinition (&AUID_AAFTypeDefinitionExtendibleEnumeration);
}

void DefinitionExtendibleEnumerationMember::Initialize()
{
}


void TypeDefinitionCharacter::Initialize()
{
  // We really only should persist 2 byte UNICODE...but for some
  // reason the we can specify and persist 1, and 4 byte characters.
  // (transdel 2000-MAR-17)
//  assert((size() == 1) ||
//         (size() == 2) ||
//         (size() == 4));
}

const ClassDefinition *
  TypeDefinitionCharacter::classDefinition(void) const
{
  return objectModel()->findClassDefinition (&AUID_AAFTypeDefinitionCharacter);
}


void TypeDefinitionSet::Initialize()
{
  // PRECONDITION: we should only initialize once.
  assert (!_elementType);

  // Lookup the type definition; it must be one of the object reference types.
  _elementType = dynamic_cast<const TypeDefinitionObjectReference *>
                          (objectModel()->findTypeDefinition (elementTypeId()));

  // POSTCONDITION: if the type definition does not exist then
  // the meta dictionary is invalid
  assert (_elementType);
}

void TypeDefinitionSet::makeAxiomatic (void) const
{
  if (!axiomatic())
  {
    // Make this definition and its class definition axiomatic
    Definition::makeAxiomatic();
    
    // Make the associated type definition axiomatic.
    elementType()->makeAxiomatic();
  }
}

const ClassDefinition *
  TypeDefinitionSet::classDefinition(void) const
{
  return objectModel()->findClassDefinition (&AUID_AAFTypeDefinitionSet);
}


// Walk the tree of definitions pre-order, call the given function for 
// the current definition before any dependent definitions.
bool TypeDefinitionSet::visitPreOrder(VisitDefinitionProcType f, void *userData) const
{
  assert(NULL != f);
  bool bContinue = false;

  // Call the visit proc for this type definition.
  bContinue = (*f)(this, userData);
  if (bContinue)
  {
    // Call the visit proc for this set's element type definition.
    bContinue = elementType()->visitPreOrder(f, userData);
  }

  return true;
}

// Walk the tree of definitions post-order, call the given function for 
// the current definition after any dependent definitions.
bool TypeDefinitionSet::visitPostOrder(VisitDefinitionProcType f, void *userData) const
{
  assert(NULL != f);
  bool bContinue = false;

  // Call the visit proc for this set's element type definition.
  bContinue = elementType()->visitPostOrder(f, userData);
  if (bContinue)
  {
    // Call the visit proc for this type definition.
    bContinue = (*f)(this, userData);
  }

  return bContinue;
}

void TypeDefinitionObjectReference::Initialize()
{
  // PRECONDITION: we should only initialize once.
  assert (!_target);

  // Lookup the class definition of the target object.
  _target = objectModel()->findClassDefinition (targetId());

  // POSTCONDITION: if the type definition does not exist then
  // the meta dictionary is invalid
  assert (_target);
}

void TypeDefinitionObjectReference::makeAxiomatic (void) const
{
  if (!axiomatic())
  {
    // Make this definition and its class definition axiomatic
    Definition::makeAxiomatic();
    
    // Make the associated type definition axiomatic.
    target()->makeAxiomatic();
  }
}


// Walk the tree of definitions pre-order, call the given function for 
// the current definition before any dependent definitions.
bool TypeDefinitionObjectReference::visitPreOrder(VisitDefinitionProcType f, void *userData) const
{
  assert(NULL != f);
  bool bContinue = false;

  // Call the visit proc for this type definition.
  bContinue = (*f)(this, userData);
  if (bContinue)
  {
    // Call the visit proc for this object reference's target definition.
    bContinue = target()->visitPreOrder(f, userData);
  }

  return true;
}

// Walk the tree of definitions post-order, call the given function for 
// the current definition after any dependent definitions.
bool TypeDefinitionObjectReference::visitPostOrder(VisitDefinitionProcType f, void *userData) const
{
  assert(NULL != f);
  bool bContinue = false;

  // Call the visit proc for this object reference's target definition.
  bContinue = target()->visitPostOrder(f, userData);
  if (bContinue)
  {
    // Call the visit proc for this type definition.
    bContinue = (*f)(this, userData);
  }

  return bContinue;
}


const ClassDefinition *
  TypeDefinitionStrongReference::classDefinition(void) const
{
  // error C2065: 'AUID_AAFTypeDefinitionStrongReference' : undeclared identifier
//  return objectModel()->findClassDefinition (&AUID_AAFTypeDefinitionStrongReference);
  return objectModel()->findClassDefinition (&AUID_AAFTypeDefinitionStrongObjectReference);
}

const ClassDefinition *
  TypeDefinitionStrongReferenceSet::classDefinition(void) const
{
  return objectModel()->findClassDefinition (&AUID_AAFTypeDefinitionSet);
}


void TypeDefinitionStrongReferenceVector::Initialize()
{
  TypeDefinitionVariableArray::Initialize();

  // Lookup the type definition; it must be a strong reference type.
  const TypeDefinitionStrongReference *type;
  type = dynamic_cast<const TypeDefinitionStrongReference *>
                          (elementType());

  // POSTCONDITION: if the type definition does not exist then
  // the meta dictionary is invalid
  assert (type);
}

const ClassDefinition *
  TypeDefinitionStrongReferenceVector::classDefinition(void) const
{
  return objectModel()->findClassDefinition (&AUID_AAFTypeDefinitionVariableArray);
}

const ClassDefinition *
  TypeDefinitionWeakReference::classDefinition(void) const
{
  // error C2065: 'AUID_AAFTypeDefinitionWeakReference' : undeclared identifier
//  return objectModel()->findClassDefinition (&AUID_AAFTypeDefinitionWeakReference);
  return objectModel()->findClassDefinition (&AUID_AAFTypeDefinitionWeakObjectReference);
}

const ClassDefinition *
  TypeDefinitionWeakReferenceSet::classDefinition(void) const
{
  return objectModel()->findClassDefinition (&AUID_AAFTypeDefinitionSet);
}


void TypeDefinitionWeakReferenceVector::Initialize()
{
  TypeDefinitionVariableArray::Initialize();

  // Lookup the type definition; it must be a weak reference type.
  const TypeDefinitionWeakReference *type;
  type = dynamic_cast<const TypeDefinitionWeakReference *>(elementType());

  // POSTCONDITION: if the type definition does not exist then
  // the meta dictionary is invalid
  assert (type);
}

const ClassDefinition *
  TypeDefinitionWeakReferenceVector::classDefinition(void) const
{
  return objectModel()->findClassDefinition (&AUID_AAFTypeDefinitionVariableArray);
}




void TypeDefinitionStream::Initialize()
{
  // PRECONDITION: we should only initialize once.
  assert (!_elementType);

  // Lookup the type definition; it must be one of the object reference types.
  _elementType = objectModel()->findTypeDefinition (elementTypeId());

  // POSTCONDITION: if the type definition does not exist then
  // the meta dictionary is invalid
  assert (_elementType);
}

void TypeDefinitionStream::makeAxiomatic (void) const
{
  if (!axiomatic())
  {
    // Make this definition and its class definition axiomatic
    Definition::makeAxiomatic();
    
    // Make the associated type definition axiomatic.
    elementType()->makeAxiomatic();
  }
}

const ClassDefinition *
  TypeDefinitionStream::classDefinition(void) const
{
  return objectModel()->findClassDefinition (&AUID_AAFTypeDefinitionStream);
}


// Walk the tree of definitions pre-order, call the given function for 
// the current definition before any dependent definitions.
bool TypeDefinitionStream::visitPreOrder(VisitDefinitionProcType f, void *userData) const
{
  assert(NULL != f);
  bool bContinue = false;

  // Call the visit proc for this type definition.
  bContinue = (*f)(this, userData);
  if (bContinue)
  {
    // Call the visit proc for this set's element type definition.
    bContinue = elementType()->visitPreOrder(f, userData);
  }

  return true;
}

// Walk the tree of definitions post-order, call the given function for 
// the current definition after any dependent definitions.
bool TypeDefinitionStream::visitPostOrder(VisitDefinitionProcType f, void *userData) const
{
  assert(NULL != f);
  bool bContinue = false;

  // Call the visit proc for this set's element type definition.
  bContinue = elementType()->visitPostOrder(f, userData);
  if (bContinue)
  {
    // Call the visit proc for this type definition.
    bContinue = (*f)(this, userData);
  }

  return bContinue;
}


void TypeDefinitionIndirect::Initialize()
{
}

const ClassDefinition *
  TypeDefinitionIndirect::classDefinition(void) const
{
  return objectModel()->findClassDefinition (&AUID_AAFTypeDefinitionIndirect);
}


const ClassDefinition *
  TypeDefinitionOpaque::classDefinition(void) const
{
  return objectModel()->findClassDefinition (&AUID_AAFTypeDefinitionOpaque);
}

#ifndef __AAFTypeDefUIDs_h__
#define __AAFTypeDefUIDs_h__
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
// GUIDs for built-in type definitions
//

#include "AAFTypes.h"

#define TYPE_GUID_NAME(type) kAAFTypeID_##type

//
// pass 0: define guids for types we'll use
//

#define AAF_TYPE_DEFINITION_INTEGER(name, id, size, signed) \
  const aafUID_t TYPE_GUID_NAME(name) = id;
#define AAF_TYPE_DEFINITION_ENUMERATION(name, id, type) \
  const aafUID_t TYPE_GUID_NAME(name) = id;
#define AAF_TYPE_DEFINITION_RECORD(name, id) \
  const aafUID_t TYPE_GUID_NAME(name) = id;
#define AAF_TYPE_DEFINITION_VARYING_ARRAY(name, id, type) \
  const aafUID_t TYPE_GUID_NAME(name) = id;
#define AAF_TYPE_DEFINITION_FIXED_ARRAY(name, id, type, count) \
  const aafUID_t TYPE_GUID_NAME(name) = id;
#define AAF_TYPE_DEFINITION_RENAME(name, id, type) \
  const aafUID_t TYPE_GUID_NAME(name) = id;
#define AAF_TYPE_DEFINITION_STRING(name, id, type) \
  const aafUID_t TYPE_GUID_NAME(name) = id;
#define AAF_TYPE_DEFINITION_INDIRECT(name, id) \
  const aafUID_t TYPE_GUID_NAME(name) = id;
#define AAF_TYPE_DEFINITION_OPAQUE(name, id) \
  const aafUID_t TYPE_GUID_NAME(name) = id;
#define AAF_TYPE_DEFINITION_EXTENDIBLE_ENUMERATION(name, id) \
  const aafUID_t TYPE_GUID_NAME(name) = id;
#define AAF_TYPE_DEFINITION_CHARACTER(name, id) \
  const aafUID_t TYPE_GUID_NAME(name) = id;
#define AAF_TYPE_DEFINITION_SET(name, id, type) \
  const aafUID_t TYPE_GUID_NAME(name) = id;
#define AAF_TYPE_DEFINITION_STRONG_REFERENCE(name, id, type) \
  const aafUID_t TYPE_GUID_NAME(name) = id;
#define AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(name, id, type) \
  const aafUID_t TYPE_GUID_NAME(name) = id;
#define AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR(name, id, type) \
  const aafUID_t TYPE_GUID_NAME(name) = id;
#define AAF_TYPE_DEFINITION_WEAK_REFERENCE(name, id, type) \
  const aafUID_t TYPE_GUID_NAME(name) = id;
#define AAF_TYPE_DEFINITION_WEAK_REFERENCE_SET(name, id, type) \
  const aafUID_t TYPE_GUID_NAME(name) = id;
#define AAF_TYPE_DEFINITION_WEAK_REFERENCE_VECTOR(name, id, type) \
  const aafUID_t TYPE_GUID_NAME(name) = id;
#define AAF_TYPE_DEFINITION_STRONG_REFERENCE(name, id, type) \
  const aafUID_t TYPE_GUID_NAME(name) = id;
#define AAF_TYPE_DEFINITION_STRONG_REFERENCE_SET(name, id, type) \
  const aafUID_t TYPE_GUID_NAME(name) = id;
#define AAF_TYPE_DEFINITION_STRONG_REFERENCE_VECTOR(name, id, type) \
  const aafUID_t TYPE_GUID_NAME(name) = id;
#define AAF_TYPE_DEFINITION_WEAK_REFERENCE(name, id, type) \
  const aafUID_t TYPE_GUID_NAME(name) = id;
#define AAF_TYPE_DEFINITION_WEAK_REFERENCE_SET(name, id, type) \
  const aafUID_t TYPE_GUID_NAME(name) = id;
#define AAF_TYPE_DEFINITION_WEAK_REFERENCE_VECTOR(name, id, type) \
  const aafUID_t TYPE_GUID_NAME(name) = id;
#define AAF_TYPE_DEFINITION_STREAM(name, id) \
  const aafUID_t TYPE_GUID_NAME(name) = id;

#define AAF_TYPE(name) name
#define AAF_REFERENCE_TYPE(type, target)      AAF_TYPE(target##type)
#define AAF_REFERENCE_TYPE_NAME(type, target) AAF_TYPE(target##type)

#include "AAFMetaDictionary.h"


#if 0

// {669349C6-EBC3-11d2-842C-00600832ACB8}
const aafUID_t kAAFTypeID_aafUInt8Array_t = 
{ 0x669349c6, 0xebc3, 0x11d2, { 0x84, 0x2c, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };


// {669349C4-EBC3-11d2-842C-00600832ACB8}
const aafUID_t kAAFTypeID_ObjRefArray = 
{ 0x669349c4, 0xebc3, 0x11d2, { 0x84, 0x2c, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };


// {669349C3-EBC3-11d2-842C-00600832ACB8}
const aafUID_t kAAFTypeID_ObjRef = 
{ 0x669349c3, 0xebc3, 0x11d2, { 0x84, 0x2c, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };


// {669349B6-EBC3-11d2-842C-00600832ACB8}
const aafUID_t kAAFTypeID_AUID = 
{ 0x669349b6, 0xebc3, 0x11d2, { 0x84, 0x2c, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

// {669349B3-EBC3-11d2-842C-00600832ACB8}
const aafUID_t kAAFTypeID_UInt8 = 
{ 0x669349b3, 0xebc3, 0x11d2, { 0x84, 0x2c, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

// {669349B4-EBC3-11d2-842C-00600832ACB8}
const aafUID_t kAAFTypeID_UInt16 = 
{ 0x669349b4, 0xebc3, 0x11d2, { 0x84, 0x2c, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

// {669349B8-EBC3-11d2-842C-00600832ACB8}
const aafUID_t kAAFTypeID_Int16 = 
{ 0x669349b8, 0xebc3, 0x11d2, { 0x84, 0x2c, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

// {669349B5-EBC3-11d2-842C-00600832ACB8}
const aafUID_t kAAFTypeID_UInt32 = 
{ 0x669349b5, 0xebc3, 0x11d2, { 0x84, 0x2c, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

// {669349B9-EBC3-11d2-842C-00600832ACB8}
const aafUID_t kAAFTypeID_Int32 = 
{ 0x669349b9, 0xebc3, 0x11d2, { 0x84, 0x2c, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

// {669349B7-EBC3-11d2-842C-00600832ACB8}
const aafUID_t kAAFTypeID_Int8 = 
{ 0x669349b7, 0xebc3, 0x11d2, { 0x84, 0x2c, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

// {669349BA-EBC3-11d2-842C-00600832ACB8}
const aafUID_t kAAFTypeID_Int64 = 
{ 0x669349ba, 0xebc3, 0x11d2, { 0x84, 0x2c, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

// {669349C3-EBC3-11d2-842C-00600832ACB8}
const aafUID_t kAAFTypeID_ObjRef = 
{ 0x669349c3, 0xebc3, 0x11d2, { 0x84, 0x2c, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

// {669349C4-EBC3-11d2-842C-00600832ACB8}
const aafUID_t kAAFTypeID_ObjRefArray = 
{ 0x669349c4, 0xebc3, 0x11d2, { 0x84, 0x2c, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

// {669349C5-EBC3-11d2-842C-00600832ACB8}
const aafUID_t kAAFTypeID_AUIDArray = 
{ 0x669349c5, 0xebc3, 0x11d2, { 0x84, 0x2c, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

// {669349C6-EBC3-11d2-842C-00600832ACB8}
const aafUID_t kAAFTypeID_UInt8Array = 
{ 0x669349c6, 0xebc3, 0x11d2, { 0x84, 0x2c, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

// {451B3AD1-FE59-11d2-842C-00600832ACB8}
const aafUID_t kAAFTypeID_UInt8Array8 = 
{ 0x451b3ad1, 0xfe59, 0x11d2, { 0x84, 0x2c, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

// {F998ED81-0220-11d3-842D-00600832ACB8}
const aafUID_t kAAFTypeID_WCharString = 
{ 0xf998ed81, 0x220, 0x11d3, { 0x84, 0x2d, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

// {EEE39BC1-183C-11d3-842F-00600832ACB8}
const aafUID_t kAAFTypeID_FadeType = 
{ 0xeee39bc1, 0x183c, 0x11d3, { 0x84, 0x2f, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

#endif

#endif // ! __AAFTypeDefUIDs_h__ */

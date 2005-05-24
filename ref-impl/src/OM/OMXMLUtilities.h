//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2005, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is British Broadcasting Corporation.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef __OMXMLUTILITIES_H__
#define __OMXMLUTILITIES_H__

#include "OMDataTypes.h"
#include "OMByteArray.h"


#define XML_MAX_AUID_URI_SIZE                   46
#define XML_MAX_MOBID_URI_SIZE                  79
// XML_MAX_URI_SIZE = max(XML_MAX_AUID_URI_SIZE, XML_MAX_MOBID_URI_SIZE)
#define XML_MAX_URI_SIZE                        79 
#define XML_MAX_INTEGER_STRING_SIZE             22
#define XML_MAX_BYTE_ORDER_STRING_SIZE          13
#define XML_MAX_TIMESTAMP_STRING_SIZE           26
#define XML_MAX_TIMESTRUCT_STRING_SIZE          13
#define XML_MAX_DATESTRUCT_STRING_SIZE          14
#define XML_MAX_BOOL_STRING_SIZE                6
#define XML_MAX_RATIONAL_STRING_SIZE            23
#define XML_MAX_VERSIONTYPE_STRING_SIZE         10


#if defined (_MSC_VER)
int std_swprintf(wchar_t* buffer, size_t count, const wchar_t* format, ...);
#else
#define std_swprintf swprintf
#endif


int utf8CodeLen(const char* u8Code);
int utf16CodeLen(const wchar_t* u16Code);
int utf16CodeLen(const char* u8Code);
int utf8CodeLen(const wchar_t* u16Code);
int utf8CodeToUTF16(wchar_t* u16Code, const char* u8Code, int* u8Len, int* u16Len);
int utf16CodeToUTF8(char* u8Code, const wchar_t* u16Code, int* u16Len, int* u8Len);
long utf16StrLen(const char* u8str);
long utf8StrLen(const wchar_t* u16str);
wchar_t* utf8ToUTF16(const char* u8str);
void utf8ToUTF16(wchar_t* u16str, const char* u8str, size_t u16Size);
char* utf16ToUTF8(const wchar_t* u16str);
void utf16ToUTF8(char* u8str, const wchar_t* u16str, size_t u8Size);
bool isValidCodePoint(OMUInt32 code);
void codePointToUTF16(OMUInt32 code, wchar_t** u16Str);
OMUInt32 codePoint(const char* u8Code);
OMUInt32 codePoint(const wchar_t* u16Code);


void auidToURI(OMUniqueObjectIdentification id, wchar_t* uri);
void mobIdToURI(OMMaterialIdentification mobId, wchar_t* uri);

bool isURI(const wchar_t* uri);
void uriToAUID(const wchar_t* uri, OMUniqueObjectIdentification* id);
void uriToMobId(const wchar_t* uri, OMMaterialIdentification* mobId);

void integerToString(const OMByte* value, OMUInt8 size, bool isSigned, wchar_t* str);
void byteOrderToString(OMByteOrder byteOrder, wchar_t* str);
void headerByteOrderToString(const OMByte* internalBytes, wchar_t* str);
void timeStampToString(const OMByte* internalBytes, wchar_t* str);
void timeStructToString(const OMByte* internalBytes, wchar_t* str);
void dateStructToString(const OMByte* internalBytes, wchar_t* str);
void boolToString(bool value, wchar_t* str);
void rationalToString(const OMByte* internalBytes, wchar_t* str);
void versionTypeToString(const OMByte* internalBytes, wchar_t* str);

void integerFromString(OMByteArray& bytes, const wchar_t* str, OMUInt8 size, bool isSigned);
void mobIdFromString(OMByteArray& bytes, const wchar_t* str);
void auidFromString(OMByteArray& bytes, const wchar_t* str);
void timeStructFromString(OMByteArray& bytes, const wchar_t* str);
void dateStructFromString(OMByteArray& bytes, const wchar_t* str);
void timeStampFromString(OMByteArray& bytes, const wchar_t* str);
void byteOrderFromString(OMByteArray& bytes, const wchar_t* str);
void byteArrayFromString(OMByteArray& bytes, const wchar_t* str);
void rationalFromString(OMByteArray& bytes, const wchar_t* str);
void versionTypeFromString(OMByteArray& bytes, const wchar_t* str);

void byteOrderFromString(const wchar_t* str, OMByteOrder* byteOrder);
void headerByteOrderFromString(OMByteArray& bytes, const wchar_t* str);
void boolFromString(const wchar_t* str, bool& value);
void uint16FromString(const wchar_t* str, OMUInt16& value);
void int64FromString(const wchar_t* str, OMInt64& value);
void uint32FromString(const wchar_t* str, OMUInt32& value);
void uint8FromString(const wchar_t* str, OMUInt8& value);


bool stringRequiresEscaping(const wchar_t* str);
bool characterRequiresEscaping(const wchar_t* c);
bool characterRequiresEscaping(OMUInt32 code);
wchar_t* escapeString(const wchar_t* str);
wchar_t* escapeCharacter(const wchar_t* c);
wchar_t* unescapeString(const wchar_t* str);
wchar_t* unescapeCharacter(const wchar_t* cstr);

wchar_t* wideCharacterStringDup(const wchar_t* str);

#endif 

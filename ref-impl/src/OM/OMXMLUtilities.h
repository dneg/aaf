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



wchar_t* convertToWideString(const char* str);
void convertToWideString(wchar_t* dest, const char* src, size_t n);
char* convertFromWideString(const wchar_t* str);
void convertFromWideString(char* dest, const wchar_t* src, size_t n);


void auidToURI(OMUniqueObjectIdentification id, wchar_t* uri);
void mobIdToURI(OMMaterialIdentification mobId, wchar_t* uri);

bool isURI(const wchar_t* uri);
void uriToAUID(const wchar_t* uri, OMUniqueObjectIdentification* id);
void uriToAUID(const char* uri, OMUniqueObjectIdentification* id);
void uriToMobId(const wchar_t* uri, OMMaterialIdentification* mobId);
void uriToMobId(const char* uri, OMMaterialIdentification* mobId);

void integerToString(const OMByte* value, OMUInt8 size, bool isSigned, wchar_t* str);
void byteOrderToString(OMByteOrder byteOrder, wchar_t* str);
void headerByteOrderToString(const OMByte* internalBytes, wchar_t* str);
void timeStampToString(const OMByte* internalBytes, wchar_t* str);
void timeStructToString(const OMByte* internalBytes, wchar_t* str);
void dateStructToString(const OMByte* internalBytes, wchar_t* str);
void boolToString(bool value, wchar_t* str);
void rationalToString(const OMByte* internalBytes, wchar_t* str);
void versionTypeToString(const OMByte* internalBytes, wchar_t* str);

void integerFromString(OMByteArray& bytes, const char* str, OMUInt8 size, bool isSigned);
void mobIdFromString(OMByteArray& bytes, const char* str);
void auidFromString(OMByteArray& bytes, const char* str);
void timeStructFromString(OMByteArray& bytes, const char* str);
void dateStructFromString(OMByteArray& bytes, const char* str);
void timeStampFromString(OMByteArray& bytes, const char* str);
void byteOrderFromString(OMByteArray& bytes, const wchar_t* str);
void byteArrayFromString(OMByteArray& bytes, const char* str);
void rationalFromString(OMByteArray& bytes, const char* str);
void versionTypeFromString(OMByteArray& bytes, const char* str);

void byteOrderFromString(const wchar_t* str, OMByteOrder* byteOrder);
void headerByteOrderFromString(OMByteArray& bytes, const char* str);
void boolFromString(const char* str, bool& value);
void uint16FromString(const char* str, OMUInt16& value);
void int64FromString(const char* str, OMInt64& value);
void uint32FromString(const char* str, OMUInt32& value);
void uint8FromString(const char* str, OMUInt8& value);


bool stringRequiresEscaping(const wchar_t* str);
bool characterRequiresEscaping(wchar_t c);
wchar_t* escapeString(const wchar_t* str);
wchar_t* escapeCharacter(wchar_t c);
wchar_t* unescapeString(const wchar_t* str);
wchar_t unescapeCharacter(const wchar_t* cstr);


#endif 

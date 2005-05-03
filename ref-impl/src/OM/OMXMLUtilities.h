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


#define XML_MAX_OID_URI_SIZE                    46
#define XML_MAX_UMID_URI_SIZE                   79
// XML_MAX_URI_SIZE = max(XML_MAX_OID_URI_SIZE, XML_MAX_UMID_URI_SIZE)
#define XML_MAX_URI_SIZE                        79 
#define XML_MAX_INTEGER_STRING_SIZE             22
#define XML_MAX_BYTE_ORDER_STRING_SIZE          13
#define XML_MAX_TIMESTAMP_STRING_SIZE           26
#define XML_MAX_TIMESTRUCT_STRING_SIZE          13
#define XML_MAX_DATESTRUCT_STRING_SIZE          14
#define XML_MAX_BOOL_STRING_SIZE                6


wchar_t* convertToWideString(const char* str);
void convertToWideString(wchar_t* dest, const char* src, size_t n);
char* convertFromWideString(const wchar_t* str);


void oidToURI(OMUniqueObjectIdentification oid, wchar_t* uri);
void umidToURI(OMMaterialIdentification umid, wchar_t* uri);

bool isURI(const wchar_t* uri);
void uriToOID(const wchar_t* uri, OMUniqueObjectIdentification* oid);
void uriToAUID(const char* uri, OMUniqueObjectIdentification* auid);
void uriToUMID(const wchar_t* uri, OMMaterialIdentification* umid);
void uriToMobId(const char* uri, OMMaterialIdentification* mobId);

void integerToString(const OMByte* value, OMUInt8 size, bool isSigned, wchar_t* str);
void byteOrderToString(OMByteOrder byteOrder, wchar_t* str);
void timeStampToString(const OMByte* internalBytes, wchar_t* str);
void timeStructToString(const OMByte* internalBytes, wchar_t* str);
void dateStructToString(const OMByte* internalBytes, wchar_t* str);
void boolToString(bool value, wchar_t* str);

void integerFromString(OMByteArray& bytes, const char* str, OMUInt8 size, bool isSigned);
void mobIdFromString(OMByteArray& bytes, const char* str);
void auidFromString(OMByteArray& bytes, const char* str);
void timeStructFromString(OMByteArray& bytes, const char* str);
void dateStructFromString(OMByteArray& bytes, const char* str);
void timeStampFromString(OMByteArray& bytes, const char* str);
void byteOrderFromString(OMByteArray& bytes, const wchar_t* str);
void byteArrayFromString(OMByteArray& bytes, const char* str);

void byteOrderFromString(const wchar_t* str, OMByteOrder* byteOrder);
void boolFromString(const char* str, bool& value);
void uint16FromString(const char* str, OMUInt16& value);
void int64FromString(const char* str, OMInt64& value);
void uint32FromString(const char* str, OMUInt32& value);
void uint8FromString(const char* str, OMUInt8& value);


#endif 

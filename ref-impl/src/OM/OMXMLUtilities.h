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

#define XML_MAX_OID_URI_SIZE  46
#define XML_MAX_UMID_URI_SIZE 79
#define XML_MAX_URI_SIZE 79 // max(XML_MAX_OID_URI_SIZE, XML_MAX_UMID_URI_SIZE)
#define XML_MAX_INTEGER_STRING_SIZE 22
#define XML_MAX_BYTE_ORDER_STRING_SIZE 13
#define XML_MAX_TIMESTAMP_STRING_SIZE 26
#define XML_MAX_TIMESTRUCT_STRING_SIZE 13
#define XML_MAX_DATESTRUCT_STRING_SIZE 14


void oidToURI(OMUniqueObjectIdentification oid, wchar_t* uri);
void umidToURI(OMMaterialIdentification umid, wchar_t* uri);

bool isURI(const wchar_t* uri);
void uriToOID(const wchar_t* uri, OMUniqueObjectIdentification* oid);
void uriToUMID(const wchar_t* uri, OMMaterialIdentification* umid);

void integerToString(const OMByte* value, OMUInt8 size, bool isSigned, wchar_t* str);

void byteOrderToString(OMByteOrder byteOrder, wchar_t* str);

void timeStampToString(const OMByte* internalBytes, wchar_t* str);
void timeStructToString(const OMByte* internalBytes, wchar_t* str);
void dateStructToString(const OMByte* internalBytes, wchar_t* str);

#endif 

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

#include "OMXMLUtilities.h"
#include "OMUtilities.h"
#include "OMXMLException.h"
#include "utf8.h"
#include "OMAssertions.h"



int 
utf8CodeLen(const char* u8Code)
{
    TRACE("::utf8CodeLen(char*)");

    if ((unsigned char)u8Code[0] < 0x80)
    {
        return 1;
    }
    else if (((unsigned char)u8Code[0] & 0xE0) == 0xC0)
    {
        if ((u8Code[1] & 0xC0) == 0x80)
        {
            return 2;
        }
        else
        {
            return -1;
        }
    }
    else if (((unsigned char)u8Code[0] & 0xF0) == 0xE0)
    {
        if (((unsigned char)u8Code[1] & 0xC0) == 0x80 && 
            ((unsigned char)u8Code[2] & 0xC0) == 0x80)
        {
            return 3;
        }
        else
        {
            return -1;
        }
    }
    else if (((unsigned char)u8Code[0] & 0xF8) == 0xF0)
    {
        if (((unsigned char)u8Code[1] & 0xC0) == 0x80 && 
            ((unsigned char)u8Code[2] & 0xC0) == 0x80 && 
            ((unsigned char)u8Code[3] & 0xC0) == 0x80)
        {
            return 4;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        return -1;
    }
}

int 
utf16CodeLen(const wchar_t* u16Code)
{
    TRACE("::utf16CodeLen(wchar_t*)");

    if (u16Code[0] < 0xD800 || u16Code[0] > 0xDFFF)
    {
        return 1;
    }
    else if (((u16Code[0] & 0xFC00) == 0xD800) && ((u16Code[1] & 0xFC00) == 0xDC00))
    {
        return 2;
    }
    else
    {
        return -1;
    }
}

int 
utf8CodeLen(const wchar_t* u16Code)
{
    TRACE("::utf8CodeLen(wchar_t*)");

    if (u16Code[0] < 0x80)
    {
        return 1;
    }
    else if (u16Code[0] < 0x800)
    {
        return 2;
    }
    else if (u16Code[0] < 0xD800 || u16Code[0] > 0xDFFF)
    {
        return 3;
    }
    else if (((u16Code[0] & 0xFC00) == 0xD800) && ((u16Code[1] & 0xFC00) == 0xDC00))
    {
        return 4;
    }
    else
    {
        return -1;
    }
}

int 
utf16CodeLen(const char* u8Code)
{
    TRACE("::utf16CodeLen(char*)");

    int u8CodeLen = utf8CodeLen(u8Code);
    if (u8CodeLen == 1 || u8CodeLen == 2 || u8CodeLen == 3)
    {
        return 1;
    }
    else if (u8CodeLen == 4)
    {
        return 2;
    }
    else
    {
        return -1;
    }
}

int 
utf8CodeToUTF16(wchar_t* u16Code, const char* u8Code, int* u8Len, int* u16Len)
{
    TRACE("::utf8CodeToUTF16");

    int len16 = utf16CodeLen(u8Code);
    int len8 = utf8CodeLen(u8Code);
    if (len16 == -1 || len8 == -1)
    {
        return -1;
    }
    *u16Len = len16;
    *u8Len = len8;
    
    if (len8 == 1)
    {
        u16Code[0] = (wchar_t)u8Code[0];
    }
    else if (len8 == 2)
    {
        wchar_t c = (((wchar_t)u8Code[0]) & 0x1F) << 6;
        c |= (((wchar_t)u8Code[1]) & 0x3F);
        u16Code[0] = c;
    }
    else if (len8 == 3)
    {
        wchar_t c = (((wchar_t)u8Code[0]) & 0x0F) << 12;
        c |= (((wchar_t)u8Code[1]) & 0x3F) << 6;
        c |= (((wchar_t)u8Code[2]) & 0x3F);
        u16Code[0] = c;
    }
    else
    {
        OMUInt32 c = (((wchar_t)u8Code[0]) & 0x07) << 18;
        c |= (((wchar_t)u8Code[1]) & 0x3F) << 12;
        c |= (((wchar_t)u8Code[2]) & 0x3F) << 6;
        c |= (((wchar_t)u8Code[3]) & 0x3F);
        c -= 0x10000;
        u16Code[0] = (wchar_t)(0xD800 | ((c >> 10) & 0x03FF));
        u16Code[1] = (wchar_t)(0xDC00 | (c & 0x03FF));
    }

    return *u16Len;
}

int 
utf16CodeToUTF8(char* u8Code, const wchar_t* u16Code, int* u16Len, int* u8Len)
{
    TRACE("::utf16CodeToUTF8");

    int len8 = utf8CodeLen(u16Code);
    int len16 = utf16CodeLen(u16Code);
    if (len8 == -1 || len16 == -1)
    {
        return -1;
    }
    *u8Len = len8;
    *u16Len = len16;
    
    if (len8 == 1)
    {
        u8Code[0] = (char)(u16Code[0]);
    }
    else if (len8 == 2)
    {
        u8Code[0] = (char)(0xC0 | (u16Code[0] >> 6));
        u8Code[1] = (char)(0x80 | (u16Code[0] & 0x3F));
    }
    else if (len8 == 3)
    {
        u8Code[0] = (char)(0xE0 | (u16Code[0] >> 12));
        u8Code[1] = (char)(0x80 | ((u16Code[0] >> 6) & 0x3F));
        u8Code[2] = (char)(0x80 | (u16Code[0] & 0x3F));
    }
    else
    {
        OMUInt32 c = (u16Code[0] & 0x03FF) << 10;
        c |= (u16Code[1] & 0x03FF);
        c += 0x10000;
        u8Code[0] = (char)(0xF0 | ((c >> 18) & 0x07));
        u8Code[1] = (char)(0x80 | ((c >> 12) & 0x3F));
        u8Code[2] = (char)(0x80 | ((c >> 6) & 0x3F));
        u8Code[3] = (char)(0x80 | (c & 0x3F));
    }
    
    return *u8Len;
}

long 
utf16StrLen(const char* u8str)
{
    TRACE("::utf16StrLen");

    long len = 0;
    const char* u8strPtr = u8str;
    while (*u8strPtr != '\0')
    {
        int u8CodeLen = utf8CodeLen(u8strPtr);
        int u16CodeLen = utf16CodeLen(u8strPtr);
        if (u8CodeLen == -1 || u16CodeLen == -1)
        {
            len = -1;
            break;
        }
        u8strPtr += u8CodeLen;
        len += u16CodeLen;
    }
    
    return len;
}

long 
utf8StrLen(const wchar_t* u16str)
{
    TRACE("::utf8StrLen");

    long len = 0;
    const wchar_t* u16strPtr = u16str;
    while (*u16strPtr != L'\0')
    {
        int u8CodeLen = utf8CodeLen(u16strPtr);
        int u16CodeLen = utf16CodeLen(u16strPtr);
        if (u8CodeLen == -1 || u16CodeLen == -1)
        {
            len = -1;
            break;
        }
        u16strPtr += u16CodeLen;
        len += u8CodeLen;
    }
    
    return len;
}

wchar_t* 
utf8ToUTF16(const char* u8str)
{
    TRACE("::utf8ToUTF16");

    long u16Len = utf16StrLen(u8str);
    if (u16Len == -1)
    {
        return 0;
    }
    
    wchar_t* u16str = new wchar_t[u16Len + 1];
    
    wchar_t* u16strPtr = u16str;
    const char* u8strPtr = u8str;
    while (*u8strPtr != '\0')
    {
        int u8CodeLen;
        int u16CodeLen;
        utf8CodeToUTF16(u16strPtr, u8strPtr, &u8CodeLen, &u16CodeLen);
        if (u8CodeLen == -1 || u16CodeLen == -1)
        {
            delete [] u16str;
            return 0;
        }
        u8strPtr += u8CodeLen;
        u16strPtr += u16CodeLen;
    }
    *u16strPtr = L'\0';
    
    return u16str;    
}

void 
utf8ToUTF16(wchar_t* u16str, const char* u8str, size_t u16Size)
{
    TRACE("::utf8ToUTF16");

    if (u16Size == 0)
    {
        return;
    }
    long u16Len = utf16StrLen(u8str);
    if (u16Len == -1)
    {
        return;
    }

    size_t count = 0;    
    wchar_t* u16strPtr = u16str;
    const char* u8strPtr = u8str;
    while (*u8strPtr != '\0' && count < u16Size)
    {
        int u16CodeLen = utf16CodeLen(u8strPtr);
        if (u16CodeLen == -1)
        {
            return;
        }
        count += u16CodeLen;
        if (count < u16Size)
        {
            int u8CodeLen;
            utf8CodeToUTF16(u16strPtr, u8strPtr, &u8CodeLen, &u16CodeLen);
            if (u8CodeLen == -1 || u16CodeLen == -1)
            {
                return;
            }
            u8strPtr += u8CodeLen;
            u16strPtr += u16CodeLen;
        }
    }
    while (count < u16Size)
    {
        *u16strPtr = L'\0';
        u16strPtr++;
        count++;
    }
}

char* 
utf16ToUTF8(const wchar_t* u16str)
{
    TRACE("::utf16ToUTF8");

    long u8Len = utf8StrLen(u16str);
    if (u8Len == -1)
    {
        return 0;
    }
    
    char* u8str = new char[u8Len + 1];
    
    const wchar_t* u16strPtr = u16str;
    char* u8strPtr = u8str;
    while (*u16strPtr != L'\0')
    {
        int u8CodeLen;
        int u16CodeLen;
        utf16CodeToUTF8(u8strPtr, u16strPtr, &u16CodeLen, &u8CodeLen);
        if (u16CodeLen == -1 || u8CodeLen == -1)
        {
            delete [] u8str;
            return 0;
        }
        u8strPtr += u8CodeLen;
        u16strPtr += u16CodeLen;
    }
    *u8strPtr = '\0';

    return u8str;
}

void 
utf16ToUTF8(char* u8str, const wchar_t* u16str, size_t u8Size)
{
    TRACE("::utf16ToUTF8");

    if (u8Size == 0)
    {
        return;
    }
    long u8Len = utf8StrLen(u16str);
    if (u8Len == -1)
    {
        return;
    }

    size_t count = 0;    
    char* u8strPtr = u8str;
    const wchar_t* u16strPtr = u16str;
    while (*u16strPtr != L'\0' && count < u8Size)
    {
        int u8CodeLen = utf8CodeLen(u16strPtr);
        if (u8CodeLen == -1)
        {
            return;
        }
        count += u8CodeLen;
        if (count < u8Size)
        {
            int u16CodeLen;
            utf16CodeToUTF8(u8strPtr, u16strPtr, &u16CodeLen, &u8CodeLen);
            if (u16CodeLen == -1 || u8CodeLen == -1)
            {
                return;
            }
            u16strPtr += u16CodeLen;
            u8strPtr += u8CodeLen;
        }
    }
    while (count < u8Size)
    {
        *u8strPtr = '\0';
        u8strPtr++;
        count++;
    }
}

bool
isValidCodePoint(OMUInt32 code)
{
    TRACE("::isValidCodePoint");

    if ((code >= 0xD800 && code <= 0xDFFF) || // used for UTF-16 surrogates
        (code >= 0xFFFE && code <= 0xFFFF) || // process internal, not for interchange
        (code >= 0x110000)) // outside code range
    {
        return false;
    }
    else
    {
        return true;
    }
}

void
codePointToUTF16(OMUInt32 code, wchar_t** u16Str)
{
    TRACE("::codePointToUTF16");
    ASSERT("Valid code point", isValidCodePoint(code));
    
    if (code > 0xFFFF)
    {
        OMUInt32 c = code - 0x10000;
        **u16Str = (wchar_t)(0xD800 | ((c >> 10) & 0x03FF));
        (*u16Str)++;
        **u16Str = (wchar_t)(0xDC00 | (c & 0x03FF));
    }
    else
    {
        **u16Str = (wchar_t)code;
    }
}

OMUInt32
codePoint(const char* u8Code)
{
    TRACE("::codePoint(char*)");

    int u8Len = utf8CodeLen(u8Code);
    if (u8Len == -1)
    {
        return 0xD800; // indicates an invalid code point
    }
    
    OMUInt32 c;
    if (u8Len == 1)
    {
        c = (OMUInt32)u8Code[0];
    }
    else if (u8Len == 2)
    {
        c = (((OMUInt32)u8Code[0]) & 0x1F) << 6;
        c |= (((OMUInt32)u8Code[1]) & 0x3F);
    }
    else if (u8Len == 3)
    {
        c = (((OMUInt32)u8Code[0]) & 0x0F) << 12;
        c |= (((OMUInt32)u8Code[1]) & 0x3F) << 6;
        c |= (((OMUInt32)u8Code[2]) & 0x3F);
    }
    else
    {
        c = (((OMUInt32)u8Code[0]) & 0x07) << 18;
        c |= (((OMUInt32)u8Code[1]) & 0x3F) << 12;
        c |= (((OMUInt32)u8Code[2]) & 0x3F) << 6;
        c |= (((OMUInt32)u8Code[3]) & 0x3F);
    }

    return c;
}

OMUInt32
codePoint(const wchar_t* u16Code)
{
    TRACE("::codePoint(wchar_t*)");

    int u16Len = utf16CodeLen(u16Code);
    if (u16Len == -1)
    {
        return 0xD800; // indicates an invalid code point
    }

    OMUInt32 c;
    if (u16Len == 1)
    {
        c = (OMUInt32)u16Code[0];
    }
    else
    {
        c = (u16Code[0] & 0x03FF) << 10;
        c |= (u16Code[1] & 0x03FF);
        c += 0x10000;
    }
    
    return c;
}


void 
auidToURI(OMUniqueObjectIdentification id, wchar_t* uri)
{
    TRACE("::auidToURI");

    if ((id.Data4[0] & 0x80) != 0 || // bit 65 of a UUID is 1 for non-NCS UUIDs
        id == nullOMUniqueObjectIdentification)
    {
        char uuidStr[XML_MAX_AUID_URI_SIZE];
        
        sprintf(uuidStr,
            "urn:uuid:%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
            id.Data1, id.Data2, id.Data3,
            id.Data4[0], id.Data4[1], id.Data4[2], id.Data4[3],
            id.Data4[4], id.Data4[5], id.Data4[6], id.Data4[7]);
        
        utf8ToUTF16(uri, uuidStr, XML_MAX_AUID_URI_SIZE);
    }
    else
    {
        char ulStr[XML_MAX_AUID_URI_SIZE];
    
        sprintf(ulStr,
            "urn:x-ul:%02x%02x%02x%02x.%02x%02x.%02x%02x.%08x.%04x%04x",
            id.Data4[0], id.Data4[1], id.Data4[2], id.Data4[3],
            id.Data4[4], id.Data4[5], id.Data4[6], id.Data4[7],
            id.Data1, id.Data2, id.Data3);
        
        utf8ToUTF16(uri, ulStr, XML_MAX_AUID_URI_SIZE);
    }
}

void 
mobIdToURI(OMMaterialIdentification mobId, wchar_t* uri)
{
    TRACE("::mobIdToURI");

    char mobIdStr[XML_MAX_MOBID_URI_SIZE];
    
    sprintf(mobIdStr,
        "urn:x-umid:%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x-"
        "%02x-"
        "%02x%02x%02x-"
        "%08x%04x%04x%02x%02x%02x%02x%02x%02x%02x%02x",
        mobId.SMPTELabel[0], mobId.SMPTELabel[1], mobId.SMPTELabel[2], mobId.SMPTELabel[3], 
        mobId.SMPTELabel[4], mobId.SMPTELabel[5], mobId.SMPTELabel[6], mobId.SMPTELabel[7], 
        mobId.SMPTELabel[8], mobId.SMPTELabel[9], mobId.SMPTELabel[10], mobId.SMPTELabel[11], 
        mobId.length,
        mobId.instanceHigh, mobId.instanceMid, mobId.instanceLow,
        mobId.material.Data1, mobId.material.Data2, mobId.material.Data3,
        mobId.material.Data4[0], mobId.material.Data4[1], mobId.material.Data4[2], mobId.material.Data4[3], 
        mobId.material.Data4[4], mobId.material.Data4[5], mobId.material.Data4[6], mobId.material.Data4[7]);
    
    utf8ToUTF16(uri, mobIdStr, XML_MAX_MOBID_URI_SIZE);
}

bool 
isURI(const wchar_t* uri)
{
    TRACE("::isURI");

    if (compareWideString(uri, L"urn:uuid", 8) == 0 ||
        compareWideString(uri, L"urn:x-ul", 8) == 0 ||
        compareWideString(uri, L"urn:x-umid", 10) == 0)
    {
        return true;
    }
    return false;
}

void 
uriToAUID(const wchar_t* uri, OMUniqueObjectIdentification* id)
{
    TRACE("::uriToAUID(wchar");

    char* uri8 = utf16ToUTF8(uri);
    uriToAUID(uri8, id);     
    
    delete [] uri8;
}

void 
uriToAUID(const char* uri, OMUniqueObjectIdentification* id)
{
    TRACE("::uriToAUID(char");

    unsigned int bytes[16];
    
    if (strncmp(uri, "urn:uuid", 8) == 0)
    {
        int ret = sscanf(uri,
		    "urn:uuid:%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		    &bytes[0], &bytes[1], &bytes[2], &bytes[3],
		    &bytes[4], &bytes[5], &bytes[6], &bytes[7],
		    &bytes[8], &bytes[9], &bytes[10], &bytes[11],
		    &bytes[12], &bytes[13], &bytes[14], &bytes[15]);

        if (ret != 16) 
        {
            throw OMXMLException(L"Invalid AUID");
        }
    }
    else
    {
        int ret = sscanf(uri,
            "urn:x-ul:%02x%02x%02x%02x.%02x%02x.%02x%02x.%02x%02x%02x%02x.%02x%02x%02x%02x",
		    &bytes[8], &bytes[9], &bytes[10], &bytes[11],
		    &bytes[12], &bytes[13], &bytes[14], &bytes[15],
		    &bytes[0], &bytes[1], &bytes[2], &bytes[3],
            &bytes[4], &bytes[5],
            &bytes[6], &bytes[7]);

        if (ret != 16) 
        {
            throw OMXMLException(L"Invalid AUID");
        }
    }
    
    id->Data1 = ((OMUInt32)bytes[0] << 24) + ((OMUInt32)bytes[1] << 16) +
        ((OMUInt32)bytes[2] << 8) + (OMUInt32)(bytes[3]);
    id->Data2 = ((OMUInt16)bytes[4] << 8) + (OMUInt16)(bytes[5]);
    id->Data3 = ((OMUInt16)bytes[6] << 8) + (OMUInt16)(bytes[7]);
    for (unsigned int i=0; i<8; i++)
    {
        id->Data4[i] = (OMUInt8)bytes[i + 8];
    }
}

void 
uriToMobId(const wchar_t* uri, OMMaterialIdentification* mobId)
{
    TRACE("::uriToMobId(wchar");

    char* uri8 = utf16ToUTF8(uri);
    uriToMobId(uri8, mobId);     

    delete [] uri8;
}

void 
uriToMobId(const char* uri, OMMaterialIdentification* mobId)
{
    TRACE("::uriToMobId(char");

    unsigned int bytes[32];
    
    int ret = sscanf(uri,
        "urn:x-umid:%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x-"
        "%02x-"
        "%02x%02x%02x-"
        "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
        &bytes[0], &bytes[1], &bytes[2], &bytes[3], 
        &bytes[4], &bytes[5], &bytes[6], &bytes[7], 
        &bytes[8], &bytes[9], &bytes[10], &bytes[11], 
        &bytes[12],
        &bytes[13], &bytes[14], &bytes[15],
        &bytes[16], &bytes[17], &bytes[18], &bytes[19], 
        &bytes[20], &bytes[21], 
        &bytes[22], &bytes[23], 
        &bytes[24], &bytes[25], &bytes[26], &bytes[27], 
        &bytes[28], &bytes[29], &bytes[30], &bytes[31]);
    
    if (ret != 32) 
    {
        throw OMXMLException(L"Invalid MobId");
    }
    
    unsigned int i;
    for (i=0; i<12; i++)
    {
        mobId->SMPTELabel[i] = (OMUInt8)bytes[i];
    }
    mobId->length = (OMUInt8)bytes[12];
    mobId->instanceHigh = (OMUInt8)bytes[13]; 
    mobId->instanceMid = (OMUInt8)bytes[14]; 
    mobId->instanceLow = (OMUInt8)bytes[15];
    mobId->material.Data1 = ((OMUInt32)bytes[16] << 24) + ((OMUInt32)bytes[17] << 16) +
        ((OMUInt32)bytes[18] << 8) + (OMUInt32)(bytes[19]);
    mobId->material.Data2 = ((OMUInt16)bytes[20] << 8) + (OMUInt16)(bytes[21]);
    mobId->material.Data3 = ((OMUInt16)bytes[22] << 8) + (OMUInt16)(bytes[23]);
    for (i=0; i<8; i++)
    {
        mobId->material.Data4[i] = (OMUInt8)bytes[i + 24];
    }
}

void 
integerToString(const OMByte* value, OMUInt8 size, bool isSigned, wchar_t* str)
{
    TRACE("::integerToString");

    char buffer[22];    
    switch (size)
    {
        case 1:
            {
                if (isSigned)
                {
                    sprintf(buffer, "%d", *((OMInt8*)value));
                }
                else
                {
                    sprintf(buffer, "%u", *((OMUInt8*)value));
                }
            }
            break;        
        case 2:
            {
                if (isSigned)
                {
                    sprintf(buffer, "%d", *((OMInt16*)value));
                }
                else
                {
                    sprintf(buffer, "%u", *((OMUInt16*)value));
                }
            }
            break;        
        case 4:
            {
                if (isSigned)
                {
                    sprintf(buffer, "%d", *((OMInt32*)value));
                }
                else
                {
                    sprintf(buffer, "%u", *((OMUInt32*)value));
                }
            }
            break;        
        case 8:
            {
                if (isSigned)
                {
                    sprintf(buffer, "%"OMFMT64"d", *((OMInt64*)value));
                }
                else
                {
                    sprintf(buffer, "%"OMFMT64"u", *((OMUInt64*)value));
                }
            }
            break;        
        default:
            ASSERT("Valid integer size", false);
            break;
    }
    
    utf8ToUTF16(str, buffer, 22);
}

void 
byteOrderToString(OMByteOrder byteOrder, wchar_t* str)
{
    if (byteOrder == littleEndian)
    {
        copyWideString(str, L"LittleEndian", 13);
    }
    else if (byteOrder == bigEndian)
    {
        copyWideString(str, L"BigEndian", 10);
    }
    else
    {
        copyWideString(str, L"Unknown", 8);
    }
}

void 
headerByteOrderToString(const OMByte* internalBytes, wchar_t* str)
{
    OMInt16 byteOrder = *(reinterpret_cast<const OMInt16*>(internalBytes));
    if (byteOrder == 0x4949)
    {
        copyWideString(str, L"LittleEndian", 13);
    }
    else if (byteOrder == 0x4D4D)
    {
        copyWideString(str, L"BigEndian", 10);
    }
    else
    {
        copyWideString(str, L"Unknown", 8);
    }
}


// must match type in AAFTypes.h
struct DateStruct
{
    OMInt16 year;
    OMUInt8 month;
    OMUInt8 day;
};

void 
dateStructToString(const OMByte* internalBytes, wchar_t* str)
{
    const DateStruct* dateStruct = reinterpret_cast<const DateStruct*>(internalBytes);
    
    char dateStr[XML_MAX_DATESTRUCT_STRING_SIZE];
    sprintf(dateStr, "%04d-%02u-%02uZ", dateStruct->year, dateStruct->month, 
        dateStruct->day);
        
    utf8ToUTF16(str, dateStr, XML_MAX_DATESTRUCT_STRING_SIZE);        
}

// must match type in AAFTypes.h
struct TimeStruct
{
    OMUInt8 hour;
    OMUInt8 minute;
    OMUInt8 second;
    OMUInt8 fraction;
};

void 
timeStructToString(const OMByte* internalBytes, wchar_t* str)
{
    const TimeStruct* timeStruct = reinterpret_cast<const TimeStruct*>(internalBytes);
    
    char timeStr[XML_MAX_TIMESTRUCT_STRING_SIZE];
    sprintf(timeStr, "%02u:%02u:%02u.%02uZ", timeStruct->hour, timeStruct->minute, 
        timeStruct->second, timeStruct->fraction);
        
    utf8ToUTF16(str, timeStr, XML_MAX_TIMESTRUCT_STRING_SIZE);        
}

// must match type in AAFTypes.h
struct TimeStamp
{
    DateStruct date;
    TimeStruct time;
};

void 
timeStampToString(const OMByte* internalBytes, wchar_t* str)
{
    const TimeStamp* timeStamp = reinterpret_cast<const TimeStamp*>(internalBytes);
    
    char timeStampStr[XML_MAX_TIMESTAMP_STRING_SIZE];
    sprintf(timeStampStr, "%04d-%02u-%02uT%02u:%02u:%02u.%02uZ", 
        timeStamp->date.year, timeStamp->date.month, timeStamp->date.day, 
        timeStamp->time.hour, timeStamp->time.minute, 
        timeStamp->time.second, timeStamp->time.fraction);
    
    utf8ToUTF16(str, timeStampStr, XML_MAX_TIMESTAMP_STRING_SIZE);        
}

void
boolToString(bool value, wchar_t* str)
{
    char boolStr[XML_MAX_BOOL_STRING_SIZE];
    
    if (value)
    {
        strcpy(boolStr, "true");
    }
    else
    {
        strcpy(boolStr, "false");
    }

    utf8ToUTF16(str, boolStr, XML_MAX_BOOL_STRING_SIZE);        
}

struct Rational
{
    OMInt32 numerator;
    OMInt32 denominator;
};

void 
rationalToString(const OMByte* internalBytes, wchar_t* str)
{
    const Rational* rational = reinterpret_cast<const Rational*>(internalBytes);
    
    char rationalStr[XML_MAX_RATIONAL_STRING_SIZE];
    sprintf(rationalStr, "%d/%d", rational->numerator, rational->denominator);
    
    utf8ToUTF16(str, rationalStr, XML_MAX_RATIONAL_STRING_SIZE); 
}

struct VersionType
{
    OMInt8 major;
    OMInt8 minor;
};

void 
versionTypeToString(const OMByte* internalBytes, wchar_t* str)
{
    const VersionType* version = reinterpret_cast<const VersionType*>(internalBytes);
    
    char versionStr[XML_MAX_VERSIONTYPE_STRING_SIZE];
    sprintf(versionStr, "%d.%d", version->major, version->minor);
    
    utf8ToUTF16(str, versionStr, XML_MAX_VERSIONTYPE_STRING_SIZE); 
}


void
integerFromString(OMByteArray& bytes, const char* str, OMUInt8 size, bool isSigned)
{
    TRACE("::integerFromString");

    const char* strPtr = str;
    bool hex = false;
    if (strncmp(str, "0x", 2) == 0)
    {
        hex = true;
        strPtr += 2;
    }
    
    int result = 0;
    switch (size) 
    {
        case 1:
            if (isSigned) 
            {
                int tmp;
                if (hex)
                {
                    result = sscanf(strPtr, "%x", &tmp);
                }
                else
                {
                    result = sscanf(strPtr, "%d", &tmp);
                }
                OMInt8 value = (OMInt8)tmp;
                bytes.append(reinterpret_cast<OMByte*>(&value), size);
            }
            else 
            {
                unsigned int tmp;
                if (hex)
                {
                    result = sscanf(strPtr, "%x", &tmp);
                }
                else
                {
                    result = sscanf(strPtr, "%u", &tmp);
                }
                OMUInt8 value = (OMUInt8)tmp;
                bytes.append(reinterpret_cast<OMByte*>(&value), size);
            }
            break;
            
        case 2:
            if (isSigned) 
            {
                int tmp;
                if (hex)
                {
                    result = sscanf(strPtr, "%x", &tmp);
                }
                else
                {
                    result = sscanf(strPtr, "%d", &tmp);
                }
                OMInt16 value = (OMInt16)tmp;
                bytes.append(reinterpret_cast<OMByte*>(&value), size);
            }
            else 
            {
                unsigned int tmp;
                if (hex)
                {
                    result = sscanf(strPtr, "%x", &tmp);
                }
                else
                {
                    result = sscanf(strPtr, "%u", &tmp);
                }
                OMUInt16 value = (OMUInt16)tmp;
                bytes.append(reinterpret_cast<OMByte*>(&value), size);
            }
            break;
            
        case 4:
            if (isSigned) 
            {
                OMInt32 value;
                if (hex)
                {
                    result = sscanf(strPtr, "%x", &value);
                }
                else
                {
                    result = sscanf(strPtr, "%d", &value);
                }
                bytes.append(reinterpret_cast<OMByte*>(&value), size);
            }
            else 
            {
                OMUInt32 value;
                if (hex)
                {
                    result = sscanf(strPtr, "%x", &value);
                }
                else
                {
                    result = sscanf(strPtr, "%u", &value);
                }
                bytes.append(reinterpret_cast<OMByte*>(&value), size);
            }
            break;
            
        case 8:
            if (isSigned) 
            {
                OMInt64 value;
                if (hex)
                {
                    result = sscanf(strPtr, "%"OMFMT64"x", &value);
                }
                else
                {
                    result = sscanf(strPtr, "%"OMFMT64"d", &value);
                }
                bytes.append(reinterpret_cast<OMByte*>(&value), size);
            }
            else 
            {
                OMUInt64 value;
                if (hex)
                {
                    result = sscanf(strPtr, "%"OMFMT64"x", &value);
                }
                else
                {
                    result = sscanf(strPtr, "%"OMFMT64"u", &value);
                }
                bytes.append(reinterpret_cast<OMByte*>(&value), size);
            }
            break;
            
        default:
            ASSERT("Valid integer size", false);
            break;
    }

    if (result != 1)
    {
        throw OMXMLException(L"Invalid integer value");
    }

}

void
mobIdFromString(OMByteArray& bytes, const char* str)
{
    TRACE("::mobIdFromString");

    OMMaterialIdentification mobId;
    uriToMobId(str, &mobId);
    
    bytes.append(reinterpret_cast<OMByte*>(&mobId), sizeof(OMMaterialIdentification));
}

void
auidFromString(OMByteArray& bytes, const char* str)
{
    TRACE("::auidFromString");

    OMUniqueObjectIdentification id;
    uriToAUID(str, &id);
    
    bytes.append(reinterpret_cast<OMByte*>(&id), sizeof(OMUniqueObjectIdentification));
}

void
timeStructFromString(OMByteArray& bytes, const char* str)
{
    TRACE("::timeStructFromString");

    unsigned int hour;
    unsigned int minute;
    unsigned int second;
    unsigned int fraction;
    
    int result = sscanf(str, "%02u:%02u:%02u.%02u", &hour, &minute, &second, &fraction);
    if (result != 4)
    {
        throw OMXMLException(L"Invalid TimeStruct value");
    }

    TimeStruct timeStruct;
    timeStruct.hour = (OMUInt8)hour;
    timeStruct.minute = (OMUInt8)minute;
    timeStruct.second = (OMUInt8)second;
    timeStruct.fraction = (OMUInt8)fraction;

    bytes.append(reinterpret_cast<OMByte*>(&timeStruct), sizeof(TimeStruct));
}

void
dateStructFromString(OMByteArray& bytes, const char* str)
{
    TRACE("::dateStructFromString");

    int year;
    unsigned int month;
    unsigned int day;
    
    int result = sscanf(str, "%04d-%02u-%02u", &year, &month, &day);
    if (result != 3)
    {
        throw OMXMLException(L"Invalid DateStruct value");
    }

    DateStruct dateStruct;
    dateStruct.year = (OMInt16)year;
    dateStruct.month = (OMUInt8)month;
    dateStruct.day = (OMUInt8)day;

    bytes.append(reinterpret_cast<OMByte*>(&dateStruct), sizeof(DateStruct));
}

void
timeStampFromString(OMByteArray& bytes, const char* str)
{
    TRACE("::timeStampFromString");
    
    unsigned int hour;
    unsigned int minute;
    unsigned int second;
    unsigned int fraction;
    int year;
    unsigned int month;
    unsigned int day;
    
    int result = sscanf(str, "%04d-%02u-%02uT%02u:%02u:%02u.%02u", 
        &year, &month, &day,
        &hour, &minute, &second, &fraction);
    if (result != 7)
    {
        throw OMXMLException(L"Invalid TimeStamp value");
    }

    TimeStamp timeStamp;
    
    timeStamp.time.hour = (OMUInt8)hour;
    timeStamp.time.minute = (OMUInt8)minute;
    timeStamp.time.second = (OMUInt8)second;
    timeStamp.time.fraction = (OMUInt8)fraction;
    timeStamp.date.year = (OMInt16)year;
    timeStamp.date.month = (OMUInt8)month;
    timeStamp.date.day = (OMUInt8)day;

    bytes.append(reinterpret_cast<OMByte*>(&timeStamp), sizeof(TimeStamp));
}

void 
byteOrderFromString(OMByteArray& bytes, const wchar_t* str)
{
    TRACE("::byteOrderFromString(bytes, str)");

    OMByteOrder byteOrder;
    byteOrderFromString(str, &byteOrder);
    
    bytes.append(reinterpret_cast<OMByte*>(&byteOrder), sizeof(OMByteOrder));
}

void 
byteArrayFromString(OMByteArray& bytes, const char* str)
{
    TRACE("::byteArrayFromString");

    size_t len = strlen(str);
    const char* valuePtr = str;
    for (size_t i = 0; i< len; i++, valuePtr++)
    {
        if ((*valuePtr >= 'a' && *valuePtr <= 'f') ||
            (*valuePtr >= 'A' && *valuePtr <= 'F') ||
            (*valuePtr >= '0' && *valuePtr <= '9'))
        {
            if ((i + 1) == len)
            {
                throw OMXMLException(L"Invalid hex byte array value");
            }
            unsigned int tmp;
            sscanf(valuePtr, "%02x", &tmp);
            OMByte byte = (OMByte)tmp;
            bytes.append(&byte, 1);
            
            i++;
            valuePtr++;
        }
    }
}

void 
rationalFromString(OMByteArray& bytes, const char* str)
{
    TRACE("::rationalFromString");
    
    int numerator;
    int denominator;

    int result = sscanf(str, "%d/%d", &numerator, &denominator);
    if (result != 2)
    {
        throw OMXMLException(L"Invalid Rational value");
    }

    Rational rational;
    
    rational.numerator = (OMInt32)numerator;
    rational.denominator = (OMInt32)denominator;

    bytes.append(reinterpret_cast<OMByte*>(&rational), sizeof(Rational));
}

void 
versionTypeFromString(OMByteArray& bytes, const char* str)
{
    TRACE("::versionTypeFromString");
    
    int major;
    int minor;

    int result = sscanf(str, "%d.%d", &major, &minor);
    if (result != 2)
    {
        throw OMXMLException(L"Invalid VersionType value");
    }

    VersionType version;
    
    version.major = (OMInt8)major;
    version.minor = (OMInt8)minor;

    bytes.append(reinterpret_cast<OMByte*>(&version), sizeof(VersionType));
}

void 
byteOrderFromString(const wchar_t* str, OMByteOrder* byteOrder)
{
    TRACE("::byteOrderFromString(str, byteOrder)");

    if (compareWideString(str, L"LittleEndian") == 0)
    {
        *byteOrder = littleEndian;
    }
    else if (compareWideString(str, L"BigEndian") == 0)
    {
        *byteOrder = bigEndian;
    }
    else if (compareWideString(str, L"Unknown") == 0)
    {
        *byteOrder = unspecified;
    }
    else
    {
        throw OMXMLException(L"Invalid byte order value");
    }
    
}

void 
headerByteOrderFromString(OMByteArray& bytes, const char* str)
{
    TRACE("::headerByteOrderFromString");

    OMInt16 byteOrder; 
    if (strcmp(str, "LittleEndian") == 0)
    {
        byteOrder = 0x4949;
    }
    else if (strcmp(str, "BigEndian") == 0)
    {
        byteOrder = 0x4D4D;
    }
    else if (strcmp(str, "Unknown") == 0)
    {
        if (hostByteOrder() == littleEndian)
        {
            byteOrder = 0x4949;
        }
        else
        {
            byteOrder = 0x4D4D;
        }
    }
    else
    {
        throw OMXMLException(L"Invalid header byte order value");
    }
    
    bytes.append(reinterpret_cast<OMByte*>(&byteOrder), sizeof(OMInt16));
}

void 
boolFromString(const char* str, bool& value)
{
    TRACE("::boolFromString");

    if (strcmp(str, "true") == 0)
    {
        value = true;
    }
    else if (strcmp(str, "false") == 0)
    {
        value = false;
    }
    else if (strcmp(str, "1") == 0)
    {
        value = true;
    }
    else if (strcmp(str, "0") == 0)
    {
        value = false;
    }
    else
    {
        throw OMXMLException(L"Invalid boolean value");
    }
}

void 
uint16FromString(const char* str, OMUInt16& value)
{
    TRACE("::uint16FromString");

    unsigned int tmp;
    int result = sscanf(str, "%u", &tmp);
    if (result != 1)
    {
        throw OMXMLException(L"Invalid UInt16 integer value");
    }
    value = (OMUInt16)tmp;
}

void 
int64FromString(const char* str, OMInt64& value)
{
    TRACE("::int64FromString");

    OMInt64 tmp;
    int result = sscanf(str, "%"OMFMT64"d", &tmp);
    if (result != 1)
    {
        throw OMXMLException(L"Invalid Int64 integer value");
    }
    value = tmp;
}

void 
uint32FromString(const char* str, OMUInt32& value)
{
    TRACE("::uint32FromString");

    unsigned int tmp;
    int result = sscanf(str, "%u", &tmp);
    if (result != 1)
    {
        throw OMXMLException(L"Invalid UInt32 integer value");
    }
    value = (OMUInt32)tmp;
}

void 
uint8FromString(const char* str, OMUInt8& value)
{
    TRACE("::uint8FromString");

    unsigned int tmp;
    int result = sscanf(str, "%u", &tmp);
    if (result != 1)
    {
        throw OMXMLException(L"Invalid UInt8 integer value");
    }
    value = (OMUInt8)tmp;
}


bool 
characterRequiresEscaping(OMUInt32 code)
{
    TRACE("::characterRequiresEscaping(OMUInt32)");

    // characters not accepted in XML version 1.0
    if ((code >= 0x0000 && code <= 0x0009) ||
        (code >= 0x000B && code <= 0x000C) ||
        (code >= 0x000E && code <= 0x001F) ||
        (code >= 0xD800 && code <= 0xDFFF) || // invalid Unicode character - used for UTF-16 surrogates
        (code >= 0xFFFE && code <= 0xFFFF) || // invalid Unicode character - process internal, not for interchange
        (code >= 0x110000)) // invalid Unicode character - outside code range
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool 
stringRequiresEscaping(const wchar_t* str)
{
    TRACE("::stringRequiresEscaping");

    bool escapeRequired = false;
    const wchar_t* strPtr = str;
    while (!escapeRequired && *strPtr != L'\0')
    {
        int codeLen = utf16CodeLen(strPtr);
        if (codeLen == -1)
        {
            escapeRequired = true;
        }
        else
        {
            OMUInt32 code = codePoint(strPtr);
            if (characterRequiresEscaping(code))
            {
                escapeRequired = true;
            }
            else
            {
                strPtr += codeLen;
            }
        }
    }
    
    return escapeRequired;
}

bool
characterRequiresEscaping(const wchar_t* c)
{
    TRACE("::characterRequiresEscaping(wchar_t*)");
    
    bool escapeRequired = false;
    int codeLen = utf16CodeLen(c);
    if (codeLen == -1)
    {
        escapeRequired = true;
    }
    else
    {
        OMUInt32 code = codePoint(c);
        if (characterRequiresEscaping(code))
        {
            escapeRequired = true;
        }
    }
    
    return escapeRequired;
}

wchar_t* 
escapeString(const wchar_t* str)
{
    TRACE("::escapeString");

    OMByteArray buffer;
    buffer.grow((lengthOfWideString(str) + 1) * sizeof(wchar_t));
    const wchar_t* strPtr = str;
    while (*strPtr != 0)
    {
        if (characterRequiresEscaping(strPtr))
        {
            OMUInt32 code;
            int codeLen = utf16CodeLen(strPtr);
            if (codeLen == -1)
            {
                code = (OMUInt32)*strPtr;
                strPtr++;
            }
            else
            {
                code = codePoint(strPtr);
                strPtr += codeLen;
            }

            char codeStr[13];
            sprintf(codeStr, "$#x%x;", code);
            wchar_t* tmp = utf8ToUTF16(codeStr);
            buffer.append(reinterpret_cast<OMByte*>(tmp), 
                lengthOfWideString(tmp) * sizeof(wchar_t));
            delete [] tmp;

        }
        else if (*strPtr == L'$')
        {
            const wchar_t* escapedDollar = L"$#x24;";
            buffer.append(reinterpret_cast<const OMByte*>(escapedDollar), 
                lengthOfWideString(escapedDollar) * sizeof(wchar_t));
            strPtr++;
        }
        else
        {
            buffer.append(reinterpret_cast<const OMByte*>(strPtr), sizeof(wchar_t));
            int codeLen = utf16CodeLen(strPtr);
            ASSERT("Valid code length", codeLen >= 1);
            strPtr += codeLen;
        }
    }
    const wchar_t* nullTerm = L'\0';
    buffer.append(reinterpret_cast<OMByte*>(&nullTerm), sizeof(wchar_t));
    wchar_t* result = new wchar_t[buffer.size() / sizeof(wchar_t)];
    memcpy(result, buffer.bytes(), buffer.size());
    return result;    
}

wchar_t* 
escapeCharacter(const wchar_t* c)
{
    TRACE("::escapeCharacter");

    OMByteArray buffer;
    if (characterRequiresEscaping(c))
    {
        OMUInt32 code;
        int codeLen = utf16CodeLen(c);
        if (codeLen == -1)
        {
            code = (OMUInt32)*c;
        }
        else
        {
            code = codePoint(c);
        }
        char codeStr[13];
        sprintf(codeStr, "$#x%x;", code);
        wchar_t* tmp = utf8ToUTF16(codeStr);
        buffer.append(reinterpret_cast<OMByte*>(tmp), 
            (lengthOfWideString(tmp) + 1) * sizeof(wchar_t));
        delete [] tmp;
    }
    else if (*c == L'$')
    {
        const wchar_t* escapedDollar = L"$#x24;";
        buffer.append(reinterpret_cast<const OMByte*>(escapedDollar), 
            (lengthOfWideString(escapedDollar) + 1) * sizeof(wchar_t));
    }
    else
    {
        int codeLen = utf16CodeLen(c);
        buffer.append(reinterpret_cast<const OMByte*>(c), sizeof(wchar_t) * codeLen);
        const wchar_t* nullTerm = L'\0';
        buffer.append(reinterpret_cast<OMByte*>(&nullTerm), sizeof(wchar_t));
    }
    wchar_t* result = new wchar_t[buffer.size() / sizeof(wchar_t)];
    memcpy(result, buffer.bytes(), buffer.size());
    return result;    
}

wchar_t* 
unescapeString(const wchar_t* str)
{
    TRACE("::unescapeString");

    wchar_t* result = new wchar_t[lengthOfWideString(str) + 1];
    wchar_t* resultPtr = result;
    const wchar_t* strPtr = str;
    while (*strPtr != L'\0')
    {
        if (*strPtr == L'$')
        {
            char cstr[14];
            utf16ToUTF8(cstr, strPtr, 14);
            OMUInt32 code;
            int ret;
            if (compareWideString(strPtr, L"$#x", 3) == 0)
            {
                ret = sscanf(cstr, "$#x%x;", &code);
            }
            else if (compareWideString(strPtr, L"$#", 2) == 0)
            {
                ret = sscanf(cstr, "$#%u;", &code);
            }
            else
            {
                throw OMXMLException(L"Invalid escaped string");
            }

            if (ret != 1)
            {
                throw OMXMLException(L"Invalid escaped string - could not read number");
            }
            if (isValidCodePoint(code))
            {
                codePointToUTF16(code, &resultPtr);
            }
            else
            {
                if (code > 0xFFFF)
                {
                    *resultPtr = (wchar_t)(code >> 16);
                    resultPtr++;
                    *resultPtr = (wchar_t)(code & 0xFFFF);
                }
                else
                {
                    *resultPtr = (wchar_t)(code);
                }
            }
            
            while (*strPtr != 0 && *strPtr != L';')
            {
                strPtr++;
            }
            if (*strPtr != L';')
            {
                throw OMXMLException(L"Invalid escaped string - missing ';'");
            }
        }
        else
        {
            int u16Len = utf16CodeLen(strPtr);
            ASSERT("Valid code length", u16Len == 1 || u16Len == 2);
            *resultPtr = *strPtr;
            if (u16Len == 2)
            {
                resultPtr++;
                strPtr++;
                *resultPtr = *strPtr;
            }
        }
        resultPtr++;
        strPtr++;            
    }
    *resultPtr = L'\0';

    return result;    
}

wchar_t*
unescapeCharacter(const wchar_t* cstr)
{
    TRACE("::unescapeCharacter");

    wchar_t* unescapeStr = unescapeString(cstr);

    return unescapeStr;
}


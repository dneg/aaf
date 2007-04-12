//=---------------------------------------------------------------------=
//
// $Id$
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
#include "OMExceptions.h"
#include "OMAssertions.h"
#include "wchar.h"

// includes for function wmkdir()
#if defined(_WIN32)
#include <direct.h>
#else
#include <sys/stat.h>
#endif



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
utf8ToUTF16(wchar_t* u16str, const char* u8str, OMUInt32 u16Size)
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

    OMUInt32 count = 0;    
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
utf16ToUTF8(char* u8str, const wchar_t* u16str, OMUInt32 u8Size)
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

    OMUInt32 count = 0;    
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
        std_swprintf(uri, XML_MAX_AUID_URI_SIZE,
            L"urn:uuid:%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
            id.Data1, id.Data2, id.Data3,
            id.Data4[0], id.Data4[1], id.Data4[2], id.Data4[3],
            id.Data4[4], id.Data4[5], id.Data4[6], id.Data4[7]);
    }
    else
    {
        std_swprintf(uri, XML_MAX_AUID_URI_SIZE,
            L"urn:x-ul:%02x%02x%02x%02x.%02x%02x.%02x%02x.%08x.%04x%04x",
            id.Data4[0], id.Data4[1], id.Data4[2], id.Data4[3],
            id.Data4[4], id.Data4[5], id.Data4[6], id.Data4[7],
            id.Data1, id.Data2, id.Data3);
    }
}

void 
mobIdToURI(OMMaterialIdentification mobId, wchar_t* uri)
{
    TRACE("::mobIdToURI");

    // handle special case of OMF UMIDs where the material number if half swapped
    if (mobId.SMPTELabel[11] == 0x00 && // "not defined" material generation method
        mobId.material.Data4[0] == 0x06 &&
        mobId.material.Data4[1] == 0x0E &&
        mobId.material.Data4[2] == 0x2B &&
        mobId.material.Data4[3] == 0x34 &&
        mobId.material.Data4[4] == 0x7F &&
        mobId.material.Data4[5] == 0x7F)
    {
        std_swprintf(uri, XML_MAX_MOBID_URI_SIZE, 
            L"urn:x-umid:%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x-"
            L"%02x-"
            L"%02x%02x%02x-"
            L"%02x%02x%02x%02x%02x%02x%02x%02x%08x%04x%04x",
            mobId.SMPTELabel[0], mobId.SMPTELabel[1], mobId.SMPTELabel[2], mobId.SMPTELabel[3], 
            mobId.SMPTELabel[4], mobId.SMPTELabel[5], mobId.SMPTELabel[6], mobId.SMPTELabel[7], 
            mobId.SMPTELabel[8], mobId.SMPTELabel[9], mobId.SMPTELabel[10], mobId.SMPTELabel[11], 
            mobId.length,
            mobId.instanceHigh, mobId.instanceMid, mobId.instanceLow,
            mobId.material.Data4[0], mobId.material.Data4[1], mobId.material.Data4[2], mobId.material.Data4[3], 
            mobId.material.Data4[4], mobId.material.Data4[5], mobId.material.Data4[6], mobId.material.Data4[7],
            mobId.material.Data1, mobId.material.Data2, mobId.material.Data3);
    }
    else
    {
        std_swprintf(uri, XML_MAX_MOBID_URI_SIZE, 
            L"urn:x-umid:%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x-"
            L"%02x-"
            L"%02x%02x%02x-"
            L"%08x%04x%04x%02x%02x%02x%02x%02x%02x%02x%02x",
            mobId.SMPTELabel[0], mobId.SMPTELabel[1], mobId.SMPTELabel[2], mobId.SMPTELabel[3], 
            mobId.SMPTELabel[4], mobId.SMPTELabel[5], mobId.SMPTELabel[6], mobId.SMPTELabel[7], 
            mobId.SMPTELabel[8], mobId.SMPTELabel[9], mobId.SMPTELabel[10], mobId.SMPTELabel[11], 
            mobId.length,
            mobId.instanceHigh, mobId.instanceMid, mobId.instanceLow,
            mobId.material.Data1, mobId.material.Data2, mobId.material.Data3,
            mobId.material.Data4[0], mobId.material.Data4[1], mobId.material.Data4[2], mobId.material.Data4[3], 
            mobId.material.Data4[4], mobId.material.Data4[5], mobId.material.Data4[6], mobId.material.Data4[7]);
    }
}

bool 
isQSymbol(const wchar_t* str)
{
    TRACE("::isQSymbol");

    // qSymbol == "something without spaces (a uri)" + L' ' +  
    // "something without spaces (a symbol)"
    
    if (str == 0 || *str == L'\0' || *str == L' ')
    {
        return false;
    }
    const wchar_t* spc1 = wmemchr(str, L' ', wcslen(str));
    if (spc1 != 0)
    {
        if (*(spc1 + 1) != L'\0' && wmemchr(spc1 + 1, L' ', wcslen(spc1 + 1)) == 0)
        {
            return true;
        }
    }
    return false;
}

bool 
isAUIDURI(const wchar_t* str)
{
    TRACE("::isAUIDURI");

    if ((wcsncmp(str, L"urn:uuid", 8) == 0 || wcsncmp(str, L"urn:x-ul", 8) == 0) &&
        (wmemchr(str, L' ', wcslen(str)) == 0))
    
    {
        return true;
    }
    return false;
}

bool 
isUMIDURI(const wchar_t* str)
{
    TRACE("::isUMIDURI");

    if (wcsncmp(str, L"urn:x-umid", 10) == 0)
    {
        return true;
    }
    return false;
}

void 
uriToAUID(const wchar_t* uri, OMUniqueObjectIdentification* id)
{
    TRACE("::uriToAUID");

    unsigned int bytes[16];
    
    if (wcsncmp(uri, L"urn:uuid", 8) == 0)
    {
        int ret = swscanf(uri,
		    L"urn:uuid:%02x%02x%02x%02x-%02x%02x-%02x%02x-%02x%02x-%02x%02x%02x%02x%02x%02x",
		    &bytes[0], &bytes[1], &bytes[2], &bytes[3],
		    &bytes[4], &bytes[5], &bytes[6], &bytes[7],
		    &bytes[8], &bytes[9], &bytes[10], &bytes[11],
		    &bytes[12], &bytes[13], &bytes[14], &bytes[15]);

        if (ret != 16) 
        {
            throw OMException("Invalid AUID");
        }
    }
    else
    {
        int ret = swscanf(uri,
            L"urn:x-ul:%02x%02x%02x%02x.%02x%02x.%02x%02x.%02x%02x%02x%02x.%02x%02x%02x%02x",
		    &bytes[8], &bytes[9], &bytes[10], &bytes[11],
		    &bytes[12], &bytes[13], &bytes[14], &bytes[15],
		    &bytes[0], &bytes[1], &bytes[2], &bytes[3],
            &bytes[4], &bytes[5],
            &bytes[6], &bytes[7]);

        if (ret != 16) 
        {
            throw OMException("Invalid AUID");
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
    TRACE("::uriToMobId");

    unsigned int bytes[32];
    
    int ret = swscanf(uri,
        L"urn:x-umid:%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x-"
        L"%02x-"
        L"%02x%02x%02x-"
        L"%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
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
        throw OMException("Invalid MobId");
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
    
    // handle special case of OMF UMIDs where the material number if half swapped
    if (bytes[11] == 0x00 && 
        bytes[16] == 0x06 && bytes[17] == 0x0E && bytes[18] == 0x2B &&
        bytes[19] == 0x34 && bytes[20] == 0x7F && bytes[21] == 0x7F)
    {
        mobId->material.Data1 = ((OMUInt32)bytes[24] << 24) + ((OMUInt32)bytes[25] << 16) +
            ((OMUInt32)bytes[26] << 8) + (OMUInt32)(bytes[27]);
        mobId->material.Data2 = ((OMUInt16)bytes[28] << 8) + (OMUInt16)(bytes[29]);
        mobId->material.Data3 = ((OMUInt16)bytes[30] << 8) + (OMUInt16)(bytes[31]);
        for (i=0; i<8; i++)
        {
            mobId->material.Data4[i] = (OMUInt8)bytes[i + 16];
        }
    }
    else
    {
        mobId->material.Data1 = ((OMUInt32)bytes[16] << 24) + ((OMUInt32)bytes[17] << 16) +
            ((OMUInt32)bytes[18] << 8) + (OMUInt32)(bytes[19]);
        mobId->material.Data2 = ((OMUInt16)bytes[20] << 8) + (OMUInt16)(bytes[21]);
        mobId->material.Data3 = ((OMUInt16)bytes[22] << 8) + (OMUInt16)(bytes[23]);
        for (i=0; i<8; i++)
        {
            mobId->material.Data4[i] = (OMUInt8)bytes[i + 24];
        }
    }
}

void 
integerToString(const OMByte* value, OMUInt8 size, bool isSigned, wchar_t* str, bool hex)
{
    TRACE("::integerToString");

    switch (size)
    {
        case 1:
            {
                if (hex)
                {
                    std_swprintf(str, 5, L"0x%x", *((OMUInt8*)value));
                }
                else if (isSigned)
                {
                    std_swprintf(str, 5, L"%d", *((OMInt8*)value));
                }
                else
                {
                    std_swprintf(str, 4, L"%u", *((OMUInt8*)value));
                }
            }
            break;        
        case 2:
            {
                if (hex)
                {
                    std_swprintf(str, 9, L"0x%x", *((OMUInt16*)value));
                }
                else if (isSigned)
                {
                    std_swprintf(str, 7, L"%d", *((OMInt16*)value));
                }
                else
                {
                    std_swprintf(str, 6, L"%u", *((OMUInt16*)value));
                }
            }
            break;        
        case 4:
            {
                if (hex)
                {
                    std_swprintf(str, 11, L"0x%x", *((OMUInt32*)value));
                }
                else if (isSigned)
                {
                    std_swprintf(str, 12, L"%d", *((OMInt32*)value));
                }
                else
                {
                    std_swprintf(str, 11, L"%u", *((OMUInt32*)value));
                }
            }
            break;        
        case 8:
            {
                if (hex)
                {
                    std_swprintf(str, 19, L"0x%"OMWFMT64 L"x", *((OMUInt64*)value));
                }
                else if (isSigned)
                {
                    std_swprintf(str, 22, L"%"OMWFMT64 L"d", *((OMInt64*)value));
                }
                else
                {
                    std_swprintf(str, 21, L"%"OMWFMT64 L"u", *((OMUInt64*)value));
                }
            }
            break;        
        default:
            ASSERT("Valid integer size", false);
            break;
    }
}

void 
byteOrderToString(OMByteOrder byteOrder, wchar_t* str)
{
    if (byteOrder == littleEndian)
    {
        wcscpy(str, L"LittleEndian");
    }
    else if (byteOrder == bigEndian)
    {
        wcscpy(str, L"BigEndian");
    }
    else
    {
        wcscpy(str, L"Unknown");
    }
}

void 
headerByteOrderToString(const OMByte* internalBytes, wchar_t* str)
{
    OMInt16 byteOrder = *(reinterpret_cast<const OMInt16*>(internalBytes));
    if (byteOrder == 0x4949)
    {
        wcscpy(str, L"LittleEndian");
    }
    else if (byteOrder == 0x4D4D)
    {
        wcscpy(str, L"BigEndian");
    }
    else
    {
        wcscpy(str, L"Unknown");
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
    
    std_swprintf(str, XML_MAX_DATESTRUCT_STRING_SIZE, L"%04d-%02u-%02uZ", dateStruct->year, dateStruct->month, 
        dateStruct->day);
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
    
    std_swprintf(str, XML_MAX_TIMESTRUCT_STRING_SIZE, L"%02u:%02u:%02u.%02uZ", timeStruct->hour, timeStruct->minute, 
        timeStruct->second, timeStruct->fraction);
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
    
    std_swprintf(str, XML_MAX_TIMESTAMP_STRING_SIZE, L"%04d-%02u-%02uT%02u:%02u:%02u.%02uZ", 
        timeStamp->date.year, timeStamp->date.month, timeStamp->date.day, 
        timeStamp->time.hour, timeStamp->time.minute, 
        timeStamp->time.second, timeStamp->time.fraction);
}

void
boolToString(bool value, wchar_t* str)
{
    if (value)
    {
        wcscpy(str, L"true");
    }
    else
    {
        wcscpy(str, L"false");
    }
}

// must match type in AAFTypes.h
struct Rational
{
    OMInt32 numerator;
    OMInt32 denominator;
};

void 
rationalToString(const OMByte* internalBytes, wchar_t* str)
{
    const Rational* rational = reinterpret_cast<const Rational*>(internalBytes);
    
    // TODO: SHOULD BE LONG!!
    std_swprintf(str, XML_MAX_RATIONAL_STRING_SIZE, L"%d/%d", rational->numerator, rational->denominator);
}

// must match type in AAFTypes.h
struct VersionType
{
    OMInt8 major;
    OMInt8 minor;
};

void 
versionTypeToString(const OMByte* internalBytes, wchar_t* str)
{
    const VersionType* version = reinterpret_cast<const VersionType*>(internalBytes);
    
    std_swprintf(str, XML_MAX_VERSIONTYPE_STRING_SIZE, L"%d.%d", version->major, version->minor);
}


void
integerFromString(OMByteArray& bytes, const wchar_t* str, OMUInt8 size, bool isSigned)
{
    TRACE("::integerFromString");

    const wchar_t* strPtr = str;
    bool hex = false;
    if (wcsncmp(str, L"0x", 2) == 0)
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
                    result = swscanf(strPtr, L"%x", &tmp);
                }
                else
                {
                    result = swscanf(strPtr, L"%d", &tmp);
                }
                OMInt8 value = (OMInt8)tmp;
                bytes.append(reinterpret_cast<OMByte*>(&value), size);
            }
            else 
            {
                unsigned int tmp;
                if (hex)
                {
                    result = swscanf(strPtr, L"%x", &tmp);
                }
                else
                {
                    result = swscanf(strPtr, L"%u", &tmp);
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
                    result = swscanf(strPtr, L"%x", &tmp);
                }
                else
                {
                    result = swscanf(strPtr, L"%d", &tmp);
                }
                OMInt16 value = (OMInt16)tmp;
                bytes.append(reinterpret_cast<OMByte*>(&value), size);
            }
            else 
            {
                unsigned int tmp;
                if (hex)
                {
                    result = swscanf(strPtr, L"%x", &tmp);
                }
                else
                {
                    result = swscanf(strPtr, L"%u", &tmp);
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
                    result = swscanf(strPtr, L"%x", &value);
                }
                else
                {
                    result = swscanf(strPtr, L"%d", &value);
                }
                bytes.append(reinterpret_cast<OMByte*>(&value), size);
            }
            else 
            {
                OMUInt32 value;
                if (hex)
                {
                    result = swscanf(strPtr, L"%x", &value);
                }
                else
                {
                    result = swscanf(strPtr, L"%u", &value);
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
                    result = swscanf(strPtr, L"%"OMWFMT64 L"x", &value);
                }
                else
                {
                    result = swscanf(strPtr, L"%"OMWFMT64 L"d", &value);
                }
                bytes.append(reinterpret_cast<OMByte*>(&value), size);
            }
            else 
            {
                OMUInt64 value;
                if (hex)
                {
                    result = swscanf(strPtr, L"%"OMWFMT64 L"x", &value);
                }
                else
                {
                    result = swscanf(strPtr, L"%"OMWFMT64 L"u", &value);
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
        throw OMException("Invalid integer value");
    }

}

void
mobIdFromString(OMByteArray& bytes, const wchar_t* str)
{
    TRACE("::mobIdFromString");

    OMMaterialIdentification mobId;
    uriToMobId(str, &mobId);
    
    bytes.append(reinterpret_cast<OMByte*>(&mobId), sizeof(OMMaterialIdentification));
}

void
auidFromString(OMByteArray& bytes, const wchar_t* str)
{
    TRACE("::auidFromString");

    OMUniqueObjectIdentification id;
    uriToAUID(str, &id);
    
    bytes.append(reinterpret_cast<OMByte*>(&id), sizeof(OMUniqueObjectIdentification));
}

void
timeStructFromString(OMByteArray& bytes, const wchar_t* str)
{
    TRACE("::timeStructFromString");

    unsigned int hour;
    unsigned int minute;
    unsigned int second;
    unsigned int fraction;
    
    int result = swscanf(str, L"%02u:%02u:%02u.%02u", &hour, &minute, &second, &fraction);
    if (result != 4)
    {
        throw OMException("Invalid TimeStruct value");
    }

    TimeStruct timeStruct;
    timeStruct.hour = (OMUInt8)hour;
    timeStruct.minute = (OMUInt8)minute;
    timeStruct.second = (OMUInt8)second;
    timeStruct.fraction = (OMUInt8)fraction;

    bytes.append(reinterpret_cast<OMByte*>(&timeStruct), sizeof(TimeStruct));
}

void
dateStructFromString(OMByteArray& bytes, const wchar_t* str)
{
    TRACE("::dateStructFromString");

    int year;
    unsigned int month;
    unsigned int day;
    
    int result = swscanf(str, L"%04d-%02u-%02u", &year, &month, &day);
    if (result != 3)
    {
        throw OMException("Invalid DateStruct value");
    }

    DateStruct dateStruct;
    dateStruct.year = (OMInt16)year;
    dateStruct.month = (OMUInt8)month;
    dateStruct.day = (OMUInt8)day;

    bytes.append(reinterpret_cast<OMByte*>(&dateStruct), sizeof(DateStruct));
}

void
timeStampFromString(OMByteArray& bytes, const wchar_t* str)
{
    TRACE("::timeStampFromString");
    
    unsigned int hour;
    unsigned int minute;
    unsigned int second;
    unsigned int fraction;
    int year;
    unsigned int month;
    unsigned int day;
    
    int result = swscanf(str, L"%04d-%02u-%02uT%02u:%02u:%02u.%02u", 
        &year, &month, &day,
        &hour, &minute, &second, &fraction);
    if (result != 7)
    {
        throw OMException("Invalid TimeStamp value");
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
byteArrayFromString(OMByteArray& bytes, const wchar_t* str)
{
    TRACE("::byteArrayFromString");

    size_t len = wcslen(str);
    const wchar_t* valuePtr = str;
    for (size_t i = 0; i< len; i++, valuePtr++)
    {
        if ((*valuePtr >= L'a' && *valuePtr <= L'f') ||
            (*valuePtr >= L'A' && *valuePtr <= L'F') ||
            (*valuePtr >= L'0' && *valuePtr <= L'9'))
        {
            if ((i + 1) == len)
            {
                throw OMException("Invalid hex byte array value");
            }
            unsigned int tmp;
            swscanf(valuePtr, L"%02x", &tmp);
            OMByte byte = (OMByte)tmp;
            bytes.append(&byte, 1);
            
            i++;
            valuePtr++;
        }
    }
}

void 
rationalFromString(OMByteArray& bytes, const wchar_t* str)
{
    TRACE("::rationalFromString");
    // TODO: SHOULD BE LONG!!
    int numerator;
    int denominator;

    int result = swscanf(str, L"%d/%d", &numerator, &denominator);
    if (result != 2)
    {
        throw OMException("Invalid Rational value");
    }

    Rational rational;
    
    rational.numerator = (OMInt32)numerator;
    rational.denominator = (OMInt32)denominator;

    bytes.append(reinterpret_cast<OMByte*>(&rational), sizeof(Rational));
}

void 
versionTypeFromString(OMByteArray& bytes, const wchar_t* str)
{
    TRACE("::versionTypeFromString");
    
    int major;
    int minor;

    int result = swscanf(str, L"%d.%d", &major, &minor);
    if (result != 2)
    {
        throw OMException("Invalid VersionType value");
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

    if (wcscmp(str, L"LittleEndian") == 0)
    {
        *byteOrder = littleEndian;
    }
    else if (wcscmp(str, L"BigEndian") == 0)
    {
        *byteOrder = bigEndian;
    }
    else if (wcscmp(str, L"Unknown") == 0)
    {
        *byteOrder = unspecified;
    }
    else
    {
        throw OMException("Invalid byte order value");
    }
    
}

void 
headerByteOrderFromString(OMByteArray& bytes, const wchar_t* str)
{
    TRACE("::headerByteOrderFromString");

    OMInt16 byteOrder; 
    if (wcscmp(str, L"LittleEndian") == 0)
    {
        byteOrder = 0x4949;
    }
    else if (wcscmp(str, L"BigEndian") == 0)
    {
        byteOrder = 0x4D4D;
    }
    else if (wcscmp(str, L"Unknown") == 0)
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
        throw OMException("Invalid header byte order value");
    }
    
    bytes.append(reinterpret_cast<OMByte*>(&byteOrder), sizeof(OMInt16));
}

void 
boolFromString(const wchar_t* str, bool& value)
{
    TRACE("::boolFromString");

    if (wcscmp(str, L"true") == 0)
    {
        value = true;
    }
    else if (wcscmp(str, L"false") == 0)
    {
        value = false;
    }
    else if (wcscmp(str, L"1") == 0)
    {
        value = true;
    }
    else if (wcscmp(str, L"0") == 0)
    {
        value = false;
    }
    else
    {
        throw OMException("Invalid boolean value");
    }
}

void 
uint16FromString(const wchar_t* str, OMUInt16& value)
{
    TRACE("::uint16FromString");

    unsigned int tmp;
    int result = 0;
    if (wcsncmp(str, L"0x", 2) == 0)
    {
        result = swscanf(str, L"0x%x", &tmp);
    }
    else
    {
        result = swscanf(str, L"%u", &tmp);
    }
    if (result != 1)
    {
        throw OMException("Invalid UInt16 integer value");
    }
    value = (OMUInt16)tmp;
}

void 
int64FromString(const wchar_t* str, OMInt64& value)
{
    TRACE("::int64FromString");

    OMInt64 tmp;
    int result = 0;
    if (wcsncmp(str, L"0x", 2) == 0)
    {
        result = swscanf(str, L"0x%"OMWFMT64 L"x", &tmp);
    }
    else
    {
        result = swscanf(str, L"%"OMWFMT64 L"d", &tmp);
    }
    if (result != 1)
    {
        throw OMException("Invalid Int64 integer value");
    }
    value = tmp;
}

void 
uint32FromString(const wchar_t* str, OMUInt32& value)
{
    TRACE("::uint32FromString");

    unsigned int tmp;
    int result = 0;
    if (wcsncmp(str, L"0x", 2) == 0)
    {
        result = swscanf(str, L"0x%x", &tmp);
    }
    else
    {
        result = swscanf(str, L"%u", &tmp);
    }
    if (result != 1)
    {
        throw OMException("Invalid UInt32 integer value");
    }
    value = (OMUInt32)tmp;
}

void 
uint8FromString(const wchar_t* str, OMUInt8& value)
{
    TRACE("::uint8FromString");

    unsigned int tmp;
    int result = 0;
    if (wcsncmp(str, L"0x", 2) == 0)
    {
        result = swscanf(str, L"0x%x", &tmp);
    }
    else
    {
        result = swscanf(str, L"%u", &tmp);
    }
    if (result != 1)
    {
        throw OMException("Invalid UInt8 integer value");
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

wchar_t* 
escapeString(const wchar_t* str)
{
    TRACE("::escapeString");

    OMByteArray buffer;
    buffer.grow((wcslen(str) + 1) * sizeof(wchar_t));
    const wchar_t* strPtr = str;
    while (*strPtr != 0)
    {
        bool escapeRequired = false;
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
        }

        if (escapeRequired)
        {
            OMUInt32 code;
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

            wchar_t codeStr[13];
            std_swprintf(codeStr, 13, L"$#x%x;", code);
            buffer.append(reinterpret_cast<OMByte*>(codeStr), 
                wcslen(codeStr) * sizeof(wchar_t));
        }
        else if (*strPtr == L'$')
        {
            const wchar_t* escapedDollar = L"$#x24;";
            buffer.append(reinterpret_cast<const OMByte*>(escapedDollar), 
                wcslen(escapedDollar) * sizeof(wchar_t));
            strPtr++;
        }
        else
        {
            buffer.append(reinterpret_cast<const OMByte*>(strPtr), sizeof(wchar_t));
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
escapeCharacter(const wchar_t c)
{
    TRACE("::escapeCharacter");

    wchar_t* cstr = 0;
    if (characterRequiresEscaping(c))
    {
        cstr = new wchar_t[13];
        std_swprintf(cstr, 13, L"$#x%x;", c);
    }
    else if (c == L'$')
    {
        const wchar_t* escapedDollar = L"$#x24;";
        cstr = new wchar_t[wcslen(escapedDollar) + 1];
        wcscpy(cstr, escapedDollar);
    }
    else
    {
        cstr = new wchar_t[2];
        cstr[0] = c;
        cstr[1] = L'\0';
    }
    return cstr;    
}

wchar_t* 
unescapeString(const wchar_t* str)
{
    TRACE("::unescapeString");

    wchar_t* result = new wchar_t[wcslen(str) + 1];
    wchar_t* resultPtr = result;
    const wchar_t* strPtr = str;
    while (*strPtr != L'\0')
    {
        if (*strPtr == L'$')
        {
            OMUInt32 code;
            int ret;
            if (wcsncmp(strPtr, L"$#x", 3) == 0)
            {
                ret = swscanf(strPtr, L"$#x%x;", &code);
            }
            else if (wcsncmp(strPtr, L"$#", 2) == 0)
            {
                ret = swscanf(strPtr, L"$#%u;", &code);
            }
            else
            {
                throw OMException("Invalid escaped string");
            }

            if (ret != 1)
            {
                throw OMException("Invalid escaped string - could not read number");
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
                throw OMException("Invalid escaped string - missing ';'");
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

wchar_t
unescapeCharacter(const wchar_t* cstr)
{
    TRACE("::unescapeCharacter");

    wchar_t* unescapeStr = unescapeString(cstr);

    if (unescapeStr[0] != 0 && unescapeStr[1] != 0)
    {
        throw OMException("Invalid escaped character value");
    }
    
    wchar_t c = *unescapeStr;
    delete [] unescapeStr;
    
    return c;
}


wchar_t* 
wideCharacterStringDup(const wchar_t* str)
{
    TRACE("::wideCharacterStringDup");
    ASSERT("Valid string", str != 0);

    wchar_t* result = new wchar_t[wcslen(str) + 1];
    wcscpy(result, str);
    
    return result;
}


bool 
fileExists(const wchar_t* fileName)
{
    TRACE("::wideCharacterStringDup");
    
    if (fileName == 0)
    {
        return false;
    }
    
    FILE* file = wfopen(fileName, L"r");
    if (file == 0)
    {
        return false;
    }
    
    fclose(file);
    return true;
}

int 
wmkdir(const wchar_t* dirpath)
{
    char* u8Dirpath = utf16ToUTF8(dirpath);
#if defined(_WIN32)
    int status = _mkdir(u8Dirpath);
#else
    int status = mkdir(u8Dirpath, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
#endif
    delete [] u8Dirpath;
    
    return status;
}

bool 
isRelativePath(const wchar_t* filepath)
{
#ifdef _WIN32
    // check if the path starts with a drive letter, i.e. x:
    const wchar_t* pathPtr = filepath;
    while (*pathPtr != 0 && *pathPtr != L'\\' && *pathPtr != L'/' && *pathPtr != L':')
    {
        pathPtr++;
    }
    if (*pathPtr != L':')
    {
        return true;
    }
    return false;
#else
    // check if path starts with root
    if (*filepath != L'/')
    {
        return true;
    }
    return false;
#endif
}

bool 
isFileURL(const wchar_t* uri)
{
    char* u8URI = utf16ToUTF8(uri);
    bool result = strncasecmp(u8URI, "file://", strlen("file://")) == 0;
    delete [] u8URI;
 
    return result;
}

bool
isRelativeURI(const wchar_t* uri)
{
    const wchar_t* uriPtr = uri;

    if (*uriPtr == L'/')
    {
        return false;
    }
    while (*uriPtr != 0 && *uriPtr != L'/' && *uriPtr != L':')
    {
        uriPtr++;
    }
    if (*uriPtr == L':')
    {
        return false;
    }

    return true;
}

wchar_t* 
getBaseFilePath(const wchar_t* filepath)
{
    TRACE("::getBaseFilePath");

    const wchar_t* endPtr = filepath;
    const wchar_t* ptr = filepath;
    while (*ptr != L'\0')
    {
#ifdef _WIN32
        if (*ptr == L'/' || *ptr == L'\\' || *ptr == L':')
#else
        if (*ptr == L'/')
#endif
        {
            endPtr = ptr + 1;
        }
        ptr++;
    }
#ifdef _WIN32
    ASSERT("Valid end pointer", endPtr == filepath || *(endPtr - 1) == L'\\' ||
        *(endPtr - 1) == L'/' || *(endPtr - 1) == L':');
#else
    ASSERT("Valid end pointer", endPtr == filepath || *(endPtr - 1) == L'/');
#endif
    
    size_t len = endPtr - filepath;
    wchar_t* result = new wchar_t[len + 1];
    wcsncpy(result, filepath, len);
    result[len] = L'\0';
    
    return result;
}




// Code below was copied from ref-impl/src/impl/AAFUtils.cpp
// The code was modified to support relative URIs in addition to File URLs
// This code is a candidate for inclusion in the shared library


/**************** Start of copied functions ************************/

#include "utf8.h"

/*
 *	Routines to support the URL and URI utility functions.
 */

// These routines rely upon URLs as defined in RFC 1738:
//
// <scheme>:<scheme-specific-part>
// ; the scheme is in lower case; interpreters should use case-ignore
//	scheme		= 1*[ lowalpha | digit | "+" | "-" | "." ]
//
// For file scheme:
//	fileurl		= "file://" [ host | "localhost" ] "/" fpath
//
// NB. ':' is acceptable unescaped in the fpath component


static bool acceptable_pchar(unsigned char c)
{
	static const unsigned char isAcceptable[96] =
	/*	0x0 0x1 0x2 0x3 0x4 0x5 0x6 0x7 0x8 0x9 0xA 0xB 0xC 0xD 0xE 0xF */
	{
	    0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xF,0xE,0x0,0xF,0xF,0xC, /* 2x  !"#$%&'()*+,-./   */
	    0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0x8,0x0,0x0,0x0,0x0,0x0, /* 3x 0123456789:;<=>?   */
	    0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF, /* 4x @ABCDEFGHIJKLMNO   */
	    0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0x0,0x0,0x0,0x0,0xF, /* 5x PQRSTUVWXYZ[\]^_   */
	    0x0,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF, /* 6x `abcdefghijklmno   */
	    0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0xF,0x0,0x0,0x0,0x0,0x0  /* 7x pqrstuvwxyz{\}~DEL */
	};

	return (c >= 32 && c < 128 && isAcceptable[c - 32]);
}

static void escapeURI(const char *str, char *result)
{
	const char	*p, hex[] = "0123456789ABCDEF";
    char		*q;

    if (!str || !result)
		return;

    for (q = result, p = str; *p; p++)
	{
    	unsigned char a = *p;
		if (!acceptable_pchar(a))
		{
		    *q++ = '%';
		    *q++ = hex[a >> 4];
		    *q++ = hex[a & 15];
		}
		else
			*q++ = *p;
    }
    *q++ = '\0';
}

static char asciiHexToChar (char c)
{
    return  c >= '0' && c <= '9' ?  c - '0' 
    	    : c >= 'A' && c <= 'F'? c - 'A' + 10
    	    : c - 'a' + 10;	/* accept lowercase letters too */
}

static void unescapeURI(char *str)
{
    char *p = str;
    char *q = str;

    while (*p)
	{
        if (*p == '%')		// URI hex escape char
		{
			p++;
			if (*p)
				*q = asciiHexToChar(*p++) * 16;
			if (*p)
				*q = *q + asciiHexToChar(*p);
			p++;
			q++;
		}
		else
		    *q++ = *p++; 
    }
    *q++ = 0;
}

/************************
 * Function: wcsconvertURItoFilepath
 *
 *	Converts a file scheme URL into an absolute filepath or a relative URI
 *  into a relative filepath. An invalid URI is returned unmodified.
 *
 * Argument Notes:
 *		filepath must have space allocated by the caller
 *
 * ReturnValue:
 *		void
 *
 * Possible Errors:
 *		none
 */
void wcsconvertURItoFilepath(const wchar_t *uri, wchar_t *filepath)
{
	// Convert to char* for ease of processing.
	// (wcsncasecmp and similiar are not available everywhere)
	//unsigned tlen = wcslen(uri);
	unsigned tlen =wcsu8slen(uri);
	char *tmp = new char[tlen+1];		// +1 includes terminating '\0'
	wcstou8s(tmp, uri, tlen+1);

	// If no file scheme is found, assume a relative URI.
	// Note that other schemes such as http and ftp are not supported.
	if (strncasecmp(tmp, "file://", strlen("file://")) != 0)
	{
    	unescapeURI(tmp);
    	u8stowcs(filepath, tmp, strlen(tmp)+1);		// convert back to wcs
	    delete [] tmp;
        return;
	}

	// Skip over the file://[host]/ to point to the fpath.
	char *fpath = tmp + strlen("file://");
	while (*fpath && *fpath != '/')
		fpath++;

#ifdef _WIN32
	// WIN32 filepaths must start with a drive letter, so remove the
	// initial '/' from the URL.
	if (*fpath == '/')
		fpath++;
#endif

	unescapeURI(fpath);

	u8stowcs(filepath, fpath, strlen(fpath)+1);		// convert back to wcs
	delete [] tmp;
}

/************************
 * Function: wcsconvertFilepathtoURI
 *
 *	Converts an absolute filepath into a file scheme URL or a relative filepath
 *  into a relative URI.
 *
 * Argument Notes:
 *		uri must have space allocated by the caller
 *
 * ReturnValue:
 *		void
 *
 * Possible Errors:
 *		none
 */
void wcsconvertFilepathtoURI(const wchar_t *filepath, wchar_t *uri)
{
    // convert to char* for ease of processing
    int tlen = wcsu8slen(filepath);
    char *tmp = new char[tlen+1];		// +1 includes terminating '\0'
    wcstou8s(tmp, filepath, tlen+1);
    
#ifdef _WIN32
    // On WIN32 backslash is the directory separator, not a regular filename
    // character like under Unix or in a URL.  So convert them to the URL
    // directory separator, forward slash '/', to preserve the hierarchy.
    char *p = tmp;
    while (*p)
    {
        if (*p == '\\')
            *p = '/';
        p++;
    }
#endif
    // worst case: every char must be hex-escaped - therefore multiply by 3
    char *escaped = new char[tlen*3+1];
    escapeURI(tmp, escaped);
    
    
    if (isRelativePath(filepath))
    {
        // straight copy for relative URIs
        u8stowcs(uri, escaped, strlen(escaped)+1);		// convert back to wcs
    }
    else
    {
        // prepare the file scheme URL (+1 for '/', +1 for '\0')
        char *mb_uri = new char[strlen(escaped) + strlen("file://") +1 +1];
        strcpy(mb_uri, "file://");
        if (*escaped != '/')		// ensure a leading path slash is present
            strcat(mb_uri, "/");
        strcat(mb_uri, escaped);
    
        u8stowcs(uri, mb_uri, strlen(mb_uri)+1);		// convert back to wcs
        delete [] mb_uri;
    }

    delete [] escaped;
    delete [] tmp;
}


/**************** End of copied functions ************************/




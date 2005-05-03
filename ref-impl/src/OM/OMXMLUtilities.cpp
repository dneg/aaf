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


wchar_t* 
convertToWideString(const char* str)
{
    TRACE("::convertToWideString(str)");

    size_t length = strlen(str);
    wchar_t* result = new wchar_t[length + 1];
    u8stowcs(result, str, length + 1);
    
    return result;
}

void 
convertToWideString(wchar_t* dest, const char* src, size_t n)
{
    TRACE("::convertToWideString(dest, src, n)");

    u8stowcs(dest, src, n);
}

char* 
convertFromWideString(const wchar_t* str)
{
    TRACE("::convertFromWideString");

    size_t length = lengthOfWideString(str);
    char* result = new char[length + 1];
    wcstou8s(result, str, length + 1);

    return result;
}

void 
oidToURI(OMUniqueObjectIdentification oid, wchar_t* uri)
{
    TRACE("::oidToURI");

    if ((oid.Data4[0] & 0x80) != 0 || // bit 65 of a UUID is 1 for non-NCS UUIDs
        oid == nullOMUniqueObjectIdentification)
    {
        char uuidStr[XML_MAX_OID_URI_SIZE];
        
        sprintf(uuidStr,
            "urn:uuid:%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
            oid.Data1, oid.Data2, oid.Data3,
            oid.Data4[0], oid.Data4[1], oid.Data4[2], oid.Data4[3],
            oid.Data4[4], oid.Data4[5], oid.Data4[6], oid.Data4[7]);
        
        convertToWideString(uri, uuidStr, XML_MAX_OID_URI_SIZE);
    }
    else
    {
        char ulStr[XML_MAX_OID_URI_SIZE];
    
        sprintf(ulStr,
            "urn:x-ul:%02x%02x%02x%02x.%02x%02x.%02x%02x.%08x.%04x%04x",
            oid.Data4[0], oid.Data4[1], oid.Data4[2], oid.Data4[3],
            oid.Data4[4], oid.Data4[5], oid.Data4[6], oid.Data4[7],
            oid.Data1, oid.Data2, oid.Data3);
        
        convertToWideString(uri, ulStr, XML_MAX_OID_URI_SIZE);
    }
}

void 
umidToURI(OMMaterialIdentification umid, wchar_t* uri)
{
    TRACE("::umidToURI");

    char umidStr[XML_MAX_UMID_URI_SIZE];
    
    sprintf(umidStr,
        "urn:x-umid:%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x-"
        "%02x-"
        "%02x%02x%02x-"
        "%08x%04x%04x%02x%02x%02x%02x%02x%02x%02x%02x",
        umid.SMPTELabel[0], umid.SMPTELabel[1], umid.SMPTELabel[2], umid.SMPTELabel[3], 
        umid.SMPTELabel[4], umid.SMPTELabel[5], umid.SMPTELabel[6], umid.SMPTELabel[7], 
        umid.SMPTELabel[8], umid.SMPTELabel[9], umid.SMPTELabel[10], umid.SMPTELabel[11], 
        umid.length,
        umid.instanceHigh, umid.instanceMid, umid.instanceLow,
        umid.material.Data1, umid.material.Data2, umid.material.Data3,
        umid.material.Data4[0], umid.material.Data4[1], umid.material.Data4[2], umid.material.Data4[3], 
        umid.material.Data4[4], umid.material.Data4[5], umid.material.Data4[6], umid.material.Data4[7]);
    
    convertToWideString(uri, umidStr, XML_MAX_UMID_URI_SIZE);
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
uriToOID(const wchar_t* uri, OMUniqueObjectIdentification* oid)
{
    TRACE("::uriToOID");

    char* uri8 = convertFromWideString(uri);
    uriToAUID(uri8, oid);     
    
    delete [] uri8;
}

void 
uriToAUID(const char* uri, OMUniqueObjectIdentification* auid)
{
    TRACE("::uriToAUID");

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
    
    auid->Data1 = ((OMUInt32)bytes[0] << 24) + ((OMUInt32)bytes[1] << 16) +
        ((OMUInt32)bytes[2] << 8) + (OMUInt32)(bytes[3]);
    auid->Data2 = ((OMUInt16)bytes[4] << 8) + (OMUInt16)(bytes[5]);
    auid->Data3 = ((OMUInt16)bytes[6] << 8) + (OMUInt16)(bytes[7]);
    for (unsigned int i=0; i<8; i++)
    {
        auid->Data4[i] = (OMUInt8)bytes[i + 8];
    }
}

void 
uriToUMID(const wchar_t* uri, OMMaterialIdentification* umid)
{
    TRACE("::uriToUMID");

    char* uri8 = convertFromWideString(uri);
    uriToMobId(uri8, umid);     

    delete [] uri8;
}

void 
uriToMobId(const char* uri, OMMaterialIdentification* mobId)
{
    TRACE("::uriToMobId");

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
                    sprintf(buffer, "%u", *((OMInt8*)value));
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
                    sprintf(buffer, "%u", *((OMInt16*)value));
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
                    sprintf(buffer, "%u", *((OMInt32*)value));
                }
            }
            break;        
        case 8:
            {
                // TODO: need to have access to AAFFMT64 from AAFPlatform.h
                if (isSigned)
                {
                    sprintf(buffer, "%lld", *((OMInt64*)value));
                    // sprintf(buffer, "%"AAFFMT64"d", *((OMInt64*)value));
                }
                else
                {
                    sprintf(buffer, "%llu", *((OMInt64*)value));
                    // sprintf(buffer, "%"AAFFMT64"u", *((OMInt64*)value));
                }
            }
            break;        
        default:
            ASSERT("Valid integer size", false);
            break;
    }
    
    convertToWideString(str, buffer, 22);
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
        
    convertToWideString(str, dateStr, XML_MAX_DATESTRUCT_STRING_SIZE);        
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
        
    convertToWideString(str, timeStr, XML_MAX_TIMESTRUCT_STRING_SIZE);        
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
    
    convertToWideString(str, timeStampStr, XML_MAX_TIMESTAMP_STRING_SIZE);        
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

    convertToWideString(str, boolStr, XML_MAX_BOOL_STRING_SIZE);        
}


void
integerFromString(OMByteArray& bytes, const char* str, OMUInt8 size, bool isSigned)
{
    TRACE("::integerFromString");

    int result = 0;
    switch (size) 
    {
        case 1:
            if (isSigned) 
            {
                int tmp;
                result = sscanf(str, "%d", &tmp);
                OMInt8 value = (OMInt8)tmp;
                bytes.append(reinterpret_cast<OMByte*>(&value), size);
            }
            else 
            {
                unsigned int tmp;
                result = sscanf(str, "%u", &tmp);
                OMUInt8 value = (OMUInt8)tmp;
                bytes.append(reinterpret_cast<OMByte*>(&value), size);
            }
            break;
            
        case 2:
            if (isSigned) 
            {
                int tmp;
                result = sscanf(str, "%d", &tmp);
                OMInt16 value = (OMInt16)tmp;
                bytes.append(reinterpret_cast<OMByte*>(&value), size);
            }
            else 
            {
                unsigned int tmp;
                result = sscanf(str, "%u", &tmp);
                OMUInt16 value = (OMUInt16)tmp;
                bytes.append(reinterpret_cast<OMByte*>(&value), size);
            }
            break;
            
        case 4:
            if (isSigned) 
            {
                OMInt32 value;
                result = sscanf(str, "%d", &value);
                bytes.append(reinterpret_cast<OMByte*>(&value), size);
            }
            else 
            {
                OMUInt32 value;
                result = sscanf(str, "%u", &value);
                bytes.append(reinterpret_cast<OMByte*>(&value), size);
            }
            break;
            
        case 8:
            if (isSigned) 
            {
                OMInt64 value;
                result = sscanf(str, "%lld", &value);
    //            result = sscanf(str, "%"AAFFMT64"d", &tmp);
                bytes.append(reinterpret_cast<OMByte*>(&value), size);
            }
            else 
            {
                OMUInt64 value;
                result = sscanf(str, "%llu", &value);
    //            result = sscanf(str, "%"AAFFMT64"u", &tmp);
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

    OMUniqueObjectIdentification auid;
    uriToAUID(str, &auid);
    
    bytes.append(reinterpret_cast<OMByte*>(&auid), sizeof(OMUniqueObjectIdentification));
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
    int result = sscanf(str, "%lld", &tmp);
    //            int result = sscanf(str, "%"AAFFMT64"d", &tmp);
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




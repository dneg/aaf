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
#include "OMAssertions.h"


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
        
        convertStringToWideString(uri, uuidStr, XML_MAX_OID_URI_SIZE);
    }
    else
    {
        char ulStr[XML_MAX_OID_URI_SIZE];
    
        sprintf(ulStr,
            "urn:x-ul:%02x%02x%02x%02x.%02x%02x.%02x%02x.%08x.%04x%04x",
            oid.Data4[0], oid.Data4[1], oid.Data4[2], oid.Data4[3],
            oid.Data4[4], oid.Data4[5], oid.Data4[6], oid.Data4[7],
            oid.Data1, oid.Data2, oid.Data3);
        
        convertStringToWideString(uri, ulStr, XML_MAX_OID_URI_SIZE);
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
    
    convertStringToWideString(uri, umidStr, XML_MAX_UMID_URI_SIZE);
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

    unsigned int bytes[16];
    
    if (compareWideString(uri, L"urn:uuid", 8) == 0)
    {
        char* uri8 = convertWideString(uri);
        
        int ret = sscanf(uri8,
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
        char* uri8 = convertWideString(uri);
        
        int ret = sscanf(uri8,
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
    
    oid->Data1 = ((OMUInt32)bytes[0] << 24) + ((OMUInt32)bytes[1] << 16) +
        ((OMUInt32)bytes[2] << 8) + (OMUInt32)(bytes[3]);
    oid->Data2 = ((OMUInt16)bytes[4] << 8) + (OMUInt16)(bytes[5]);
    oid->Data3 = ((OMUInt16)bytes[6] << 8) + (OMUInt16)(bytes[7]);
    for (unsigned int i=0; i<8; i++)
    {
        oid->Data4[i] = (OMUInt8)bytes[i + 8];
    }
    
}

void 
uriToUMID(const wchar_t* uri, OMMaterialIdentification* umid)
{
    TRACE("::uriToUMID");

    char* uri8 = convertWideString(uri);
    
    unsigned int bytes[32];
    
    int ret = sscanf(uri8,
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
        throw OMXMLException(L"Invalid UMID");
    }
    
    unsigned int i;
    for (i=0; i<12; i++)
    {
        umid->SMPTELabel[i] = (OMUInt8)bytes[i];
    }
    umid->length = (OMUInt8)bytes[12];
    umid->instanceHigh = (OMUInt8)bytes[13]; 
    umid->instanceMid = (OMUInt8)bytes[14]; 
    umid->instanceLow = (OMUInt8)bytes[15];
    umid->material.Data1 = ((OMUInt32)bytes[16] << 24) + ((OMUInt32)bytes[17] << 16) +
        ((OMUInt32)bytes[18] << 8) + (OMUInt32)(bytes[19]);
    umid->material.Data2 = ((OMUInt16)bytes[20] << 8) + (OMUInt16)(bytes[21]);
    umid->material.Data3 = ((OMUInt16)bytes[22] << 8) + (OMUInt16)(bytes[23]);
    for (i=0; i<8; i++)
    {
        umid->material.Data4[i] = (OMUInt8)bytes[i + 24];
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
    
    convertStringToWideString(str, buffer, 22);
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
        
    convertStringToWideString(str, dateStr, XML_MAX_DATESTRUCT_STRING_SIZE);        
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
        
    convertStringToWideString(str, timeStr, XML_MAX_TIMESTRUCT_STRING_SIZE);        
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
    
    convertStringToWideString(str, timeStampStr, XML_MAX_TIMESTAMP_STRING_SIZE);        
}




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


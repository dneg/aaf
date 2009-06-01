//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// British Broadcasting Corporation.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef __OMWSTRING_H__
#define __OMWSTRING_H__

#include "wchar.h"

class OMWString
{
public:
    OMWString();
    OMWString(const OMWString& ws);
    OMWString(const wchar_t* ws);

    ~OMWString();

    int compare(const OMWString& rhs) const;
    int compare(const wchar_t* rhs) const;
    size_t length() const;
    const wchar_t* c_str() const;
    
    OMWString& operator=(const OMWString& rhs);
    OMWString& operator=(const wchar_t* rhs);

    bool operator==(const OMWString& rhs) const;
    bool operator==(const wchar_t* rhs) const;
    bool operator!=(const OMWString& rhs) const;
    bool operator!=(const wchar_t* rhs) const;
    bool operator<(const OMWString& rhs) const;
    bool operator<(const wchar_t* rhs) const;

private:
    wchar_t* _ws;
};

#endif //__OMWSTRING_H__


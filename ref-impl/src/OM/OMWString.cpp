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

#include <OMWString.h>
#include <OMUtilities.h>
#include <OMAssertions.h>


OMWString::OMWString()
: _ws(0)
{
    TRACE("OMWString::OMWString");
}


OMWString::OMWString(const OMWString& ws)
: _ws(0)
{
    TRACE("OMWString::OMWString");

    if (ws.length() != 0)
    {
        _ws = new wchar_t[ws.length() + 1];
        copyWideString(_ws, ws.c_str());
    }
}


OMWString::OMWString(const wchar_t* ws)
: _ws(0)
{
    TRACE("OMWString::OMWString");

    if (ws != 0)
    {
        _ws = new wchar_t[lengthOfWideString(ws) + 1];
        copyWideString(_ws, ws);
    }
}


OMWString::~OMWString()
{
    TRACE("OMWString::~OMWString");
    
    if (_ws != 0) 
    {
        delete [] _ws;
    }
}


int OMWString::compare(const OMWString& rhs) const
{
    TRACE("OMWString::compare");
    PRECONDITION("Valid strings", _ws != 0 && rhs.c_str() != 0);

    return compare(rhs.c_str());
}


int OMWString::compare(const wchar_t* rhs) const
{
    TRACE("OMWString::compare");
    PRECONDITION("Valid strings", _ws != 0 && rhs != 0);

    return compareWideString(_ws, rhs);
}


size_t OMWString::length() const
{
    TRACE("OMWString::length");

    if (_ws != 0)
    {
        return lengthOfWideString(_ws);
    }
    else
    {
        return 0;
    }
}


OMWString& OMWString::operator=(const OMWString& rhs)
{
    TRACE("OMWString::operator=");

    return operator=(rhs.c_str());
}


OMWString& OMWString::operator=(const wchar_t* rhs)
{
    TRACE("OMWString::operator=");

    if (_ws != 0)
    {
        delete [] _ws;
    }
    
    if (rhs != 0) 
    {
        _ws = new wchar_t[lengthOfWideString(rhs) + 1];
        copyWideString(_ws, rhs);
    }
    else
    {
        _ws = 0;
    }
    
    return *this;
}


bool OMWString::operator==(const OMWString& rhs) const
{
    TRACE("OMWString::operator==");
    PRECONDITION("Valid strings", _ws != 0 && rhs.c_str() != 0);

    return operator==(rhs.c_str());
}


bool OMWString::operator==(const wchar_t* rhs) const
{
    TRACE("OMWString::operator==");
    PRECONDITION("Valid strings", _ws != 0 && rhs != 0);

    return compareWideString(_ws, rhs) == 0;
}


bool OMWString::operator!=(const OMWString& rhs) const
{
    TRACE("OMWString::operator!=");
    PRECONDITION("Valid strings", _ws != 0 && rhs.c_str() != 0);

    return operator!=(rhs.c_str());
}


bool OMWString::operator!=(const wchar_t* rhs) const
{
    TRACE("OMWString::operator!=");
    PRECONDITION("Valid strings", _ws != 0 && rhs != 0);

    return compareWideString(_ws, rhs) != 0;
}


bool OMWString::operator<(const OMWString& rhs) const
{
    TRACE("OMWString::operator<");
    PRECONDITION("Valid strings", _ws != 0 && rhs.c_str() != 0);

    return operator<(rhs.c_str());
}


bool OMWString::operator<(const wchar_t* rhs) const
{
    TRACE("OMWString::operator<");
    PRECONDITION("Valid strings", _ws != 0 && rhs != 0);

    return compareWideString(_ws, rhs) < 0;
}


const wchar_t* OMWString::c_str() const
{
    TRACE("OMWString::c_str");

    return _ws;
}


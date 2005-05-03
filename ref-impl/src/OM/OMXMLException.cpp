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

#include "OMXMLException.h"
#include "OMUtilities.h"


OMXMLException::OMXMLException()
: _message(0), _emptyMessage(L"")
{
}

OMXMLException::OMXMLException(const wchar_t* message)
: _message(0), _emptyMessage(L"")
{
    if (message != 0)
    {
        _message = new wchar_t[lengthOfWideString(message) + 1];
        copyWideString(_message, message);
    }
}

OMXMLException::OMXMLException(const wchar_t* message, const wchar_t* secondaryMessage)
: _message(0), _emptyMessage(L"")
{
    if (message != 0)
    {
        if (secondaryMessage != 0)
        {
            _message = new wchar_t[lengthOfWideString(message) + 
                lengthOfWideString(secondaryMessage) + 3];
            _message[0] = L'\0';
            concatenateWideString(_message, message);
            concatenateWideString(_message, L":\n");
            concatenateWideString(_message, secondaryMessage);
        }
        else
        {
            _message = new wchar_t[lengthOfWideString(message) + 1];
            copyWideString(_message, message);
        }
    }
}

OMXMLException::OMXMLException(const OMXMLException& ex)
: _message(0), _emptyMessage(L"")
{
    if (ex._message != 0)
    {
        _message = new wchar_t[lengthOfWideString(ex._message) + 1];
        copyWideString(_message, ex._message);
    }
}

OMXMLException::~OMXMLException()
{
    if (_message != 0)
    {
        delete [] _message;
    }
}
    
const wchar_t* 
OMXMLException::getMessage()
{
    if (_message != 0)
    {
        return _message;
    }
    return _emptyMessage;
}


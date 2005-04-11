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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is British Broadcasting Corporation.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#ifndef __OMXMLEXCEPTION_H__
#define __OMXMLEXCEPTION_H__

#include <wchar.h>


class OMXMLException
{
public:
    OMXMLException();
    OMXMLException(const wchar_t* message);
    OMXMLException(const wchar_t* message, const wchar_t* secondaryMessage);
    OMXMLException(const OMXMLException& ex);
    ~OMXMLException();
    
    const wchar_t* getMessage();
    
private:
    wchar_t* _message;
    const wchar_t* _emptyMessage;
};

#endif

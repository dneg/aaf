//=---------------------------------------------------------------------=
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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMEXTERNAL
#include "OMExceptions.h"

OMException::OMException(void)
: _name("Unknown")
{
}

OMException::OMException(const char* name)
: _name(name)
{
}

const char* OMException::name(void) const
{
  return _name;
}

OMException::~OMException(void)
{
}

OMWindowsException::OMWindowsException(OMWindowsResult result)
: _result(result)
{
}

OMWindowsException::OMWindowsException(
  const char* name,
  OMWindowsResult result)
: OMException(name),
  _result(result)
{
}

OMWindowsException::~OMWindowsException(void)
{
}

OMWindowsResult OMWindowsException::result(void)
{
  return _result;
}

OMWindowsResult OMExceptionToResult(
  OMException& exception,
  OMWindowsResult fallback)
{
  OMWindowsResult result;

  OMWindowsException* e = dynamic_cast<OMWindowsException*>(&exception);
  if (e != 0) {
    result = e->result();
  } else {
    result = fallback;
  }
  return result;
}

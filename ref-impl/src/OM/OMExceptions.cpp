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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMException
#include "OMExceptions.h"

#include "OMAssertions.h"

OMException::OMException(void)
: _name("Unknown"),
  _hasResult(false),
  _result(0)
{
}

OMException::OMException(OMResult result)
: _name("Unknown"),
  _hasResult(true),
  _result(result)
{
}

OMException::OMException(const char* name)
: _name(name),
  _hasResult(false),
  _result(0)
{
}

OMException::OMException(const char* name, OMResult result)
: _name(name),
  _hasResult(true),
  _result(result)
{
}

OMException::~OMException(void)
{
}

const char* OMException::name(void) const
{
  return _name;
}

bool OMException::hasResult(void)
{
  return _hasResult;
}

OMResult OMException::result(void)
{
  TRACE("OMException::result");
  PRECONDITION("Result available", hasResult());

  return _result;
}

OMResult OMExceptionToResult(
  OMException& exception,
  OMResult fallback)
{
  OMResult result;

  if (exception.hasResult()) {
    result = exception.result();
  } else {
    result = fallback;
  }
  return result;
}

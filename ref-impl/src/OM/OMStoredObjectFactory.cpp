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
// The Original Code of this file is Copyright 1998-2002, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMINTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMStoredObjectFactory
#include "OMStoredObjectFactory.h"

#include "OMAssertions.h"
#include "OMDataTypes.h"
#include "OMUtilities.h"

  // @mfunc Constructor.
OMStoredObjectFactory::OMStoredObjectFactory(
                                        const OMStoredObjectEncoding& encoding,
                                        const wchar_t* name,
                                        const wchar_t* description)
: _encoding(encoding),
  _name(saveWideString(name)),
  _description(saveWideString(description))
{
  TRACE("OMStoredObjectFactory::OMStoredObjectFactory");

  PRECONDITION("Valid name", validWideString(name));
  PRECONDITION("Valid name", validWideString(description));
  PRECONDITION("Valid encoding", encoding != nullOMStoredObjectEncoding);
}

  // @mfunc Destructor.
OMStoredObjectFactory::~OMStoredObjectFactory(void)
{
  TRACE("OMStoredObjectFactory::~OMStoredObjectFactory");

  delete [] _name;
  _name = 0;
}

  // @mfunc The key used to identify the encoding of
  //        <c OMStoredObject>s created by this <c OMStoredObjectFactory>.
  //   @rdesc The encoding key.
OMStoredObjectEncoding OMStoredObjectFactory::encoding(void) const
{
  TRACE("OMStoredObjectFactory::encoding");
  return _encoding;
}

  // @mfunc The name used to identify the encoding of
  //        <c OMStoredObject>s created by this <c OMStoredObjectFactory>.
  //   @rdesc The name.
const wchar_t* OMStoredObjectFactory::name(void) const
{
  TRACE("OMStoredObjectFactory::name");
  return _name;
}

  // @mfunc The description of the encoding of
  //        <c OMStoredObject>s created by this <c OMStoredObjectFactory>.
  //   @rdesc The description.
const wchar_t* OMStoredObjectFactory::description(void) const
{
  TRACE("OMStoredObjectFactory::description");
  return _description;
}

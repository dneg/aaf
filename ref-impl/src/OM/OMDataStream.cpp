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
// The Original Code of this file is Copyright 1998-2003, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

// @doc OMEXTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMDataStream

#include "OMDataStream.h"

#include "OMAssertions.h"

  // @mfunc Constructor.
OMDataStream::OMDataStream(const OMPropertyId propertyId,
                           const wchar_t* name)
: OMProperty(propertyId, SF_DATA_STREAM, name)
{
  TRACE("OMDataStream::OMDataStream");
}

  // @mfunc Destructor.
OMDataStream::~OMDataStream(void)
{
  TRACE("OMDataStream::~OMDataStream");
}


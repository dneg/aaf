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

// @doc OMINTERNAL
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMStreamAccess
#include "OMDataStreamAccess.h"

#include "OMAssertions.h"

  // @mfunc Constructor.
  //   @parm A client supplied context.
OMDataStreamAccess::OMDataStreamAccess(void* context)
: _context(context)
{
  TRACE("OMDataStreamAccess::OMDataStreamAccess");
}

  // @mfunc Save data to the given <c OMDataStream> object.
  //   @parm The <c OMDataStream> on which the data is to be saved.
void OMDataStreamAccess::save(OMDataStream& stream)
{
  TRACE("OMDataStreamAccess::save");

  // Call the client back.
  save(stream, _context);
}

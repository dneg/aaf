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

// @doc OMINTERNAL
#ifndef OMOSTREAM_H
#define OMOSTREAM_H

#include "OMDataTypes.h"

  // @class Simple, platform independent, text output stream
  //        for diagnostic (debug only) use by the Object Manager.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMOStream {
public:
  // @access Public members.

    // @cmember Insert a character string.
  OMOStream& operator << (const char* string);

    // @cmember Insert an OMUInt32 in decimal.
  OMOStream& operator << (OMUInt32 i);

    // @cmember Insert a pointer in hex.
  OMOStream& operator << (void* p);

    // @cmember Insert a new line.
  OMOStream& endLine(void);

    // @cmember Insert (call) a manipulator.
  OMOStream& operator << (OMOStream& (*manipulator)(OMOStream&));

protected:
  // @access Protected members.

    // @cmember Put a character string.
  OMOStream& put(const char* string);

    // @cmember Put an OMUInt32 in decimal.
  OMOStream& put(OMUInt32 i);

    // @cmember Put a pointer in hex.
  OMOStream& put(void* p);

    // @cmember Put a new line.
  OMOStream& putLine(void);

};

 // @func <c OMOStream> end of line manipulator.
 //   @parm The <c OMOStream> in which to inset the new line.
 //   @rdesc The modified <c OMOStream>.
OMOStream& endl(OMOStream& s);

 // @globalv Global <c OMOStream> for Object Manager logging.
 //          Debug use only.
extern OMOStream omlog;

#endif

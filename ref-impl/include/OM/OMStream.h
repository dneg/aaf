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
// @author Tim Bingham | tjb | Avid Technology, Inc. | OMStream
#ifndef OMSTREAM_H
#define OMSTREAM_H

#include <stdio.h>
#include "OMFile.h"
#include "OMDataTypes.h"

  // @class Wrappers for ISO FILE*s.
  //        Additionally supporting wchar_t file names and 64-bit positions.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMStream {
public:

    // @cmember Create an <c OMStream> object by opening an existing
    //          file for read-only access, the file is named <p fileName>.
    //          The file must already exist.
  static OMStream* openExistingRead(const wchar_t* fileName);

    // @cmember Create an <c OMStream> object by opening an existing
    //          file for modify access, the file is named <p fileName>.
    //          The file must already exist.
  static OMStream* openExistingModify(const wchar_t* fileName);

    // @cmember Create an <c OMStream> object by creating a new
    //          file for modify access, the file is named <p fileName>.
    //          The file must not already exist.
  static OMStream* openNewModify(const wchar_t* fileName);

    // @cmember Create an <c OMStream> object by creating a new
    //          file temporary for modify access.
  static OMStream* openNewModify(void);

  void read(OMByte* bytes,
            OMUInt32 byteCount,
            OMUInt32& bytesRead) const;

  void write(const OMByte* bytes,
             OMUInt32 byteCount,
             OMUInt32& bytesWritten);

  OMUInt64 size(void) const;

  void setSize(OMUInt64 newSize);

  OMUInt64 position(void) const;

  void setPosition(OMUInt64 newPosition);

  void synchronize(void);

  bool isWritable(void) const;

  virtual ~OMStream(void);

private:

  OMStream(FILE* file, bool isWritable);

  FILE* _file;
  bool _isWritable;
};

#endif

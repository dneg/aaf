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
#ifndef OMEXCEPTIONS_H
#define OMEXCEPTIONS_H

#include "OMDataTypes.h"

  // @class Exceptions thrown by the Object Manager.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMException {
public:
  // @access Public members.

    // @cmember Constructor.
  OMException(void);

    // @cmember Constructor.
  OMException(const char* name);

    // @cmember Destructor.
  virtual ~OMException(void);

    // @cmember The name of this <c OMException>.
  virtual const char* name(void) const;

private:
  // @access Private members.
  const char* _name;
};

  // Equivalent to an HRESULT
typedef OMUInt32 OMWindowsResult;

  // @class Windows specific descendant of <c OMException> thrown
  //        by the Object Manager.
  //   @cauthor Tim Bingham | tjb | Avid Technology, Inc.
class OMWindowsException : public OMException {
public:
  // @access Public members.

    // @cmember Constructor.
  OMWindowsException(OMWindowsResult result);

    // @cmember Constructor.
  OMWindowsException(const char* name, OMWindowsResult result);

    // @cmember Destructor.
  virtual ~OMWindowsException(void);

    // @cmember The result associated with this <c OMWindowsException>.
  virtual OMWindowsResult result(void);

private:
  // @access Private members.
  OMWindowsResult _result;
};

  // @func If <p exception> has an associated result code then return
  //       it, otherwise return <p fallback>.
  //   @parm An exception, which may have a result code.
  //   @parm The fall back result code, this result code is used
  //         if the exception does not have one.
  //   @rdesc The result code from the exception, if it has one, otherwise
  //          the fall back result code.
OMWindowsResult OMExceptionToResult(
  OMException& exception,
  OMWindowsResult fallback);

#endif

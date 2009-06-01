#ifndef __ImplAAFGetFileBits_h__
#define __ImplAAFGetFileBits_h__
//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif

#include "OMRawStorage.h"

class ImplAAFGetFileBits : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFGetFileBits ();

protected:
  virtual ~ImplAAFGetFileBits ();

public:

  virtual void
    Initialize
        (OMRawStorage * prs);

  //****************
  // ReadAt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    ReadAt
        (// @parm [out, size_is(bufSize)] Buffer into which data is read
         aafMemPtr_t  buf,

         // @parm [in] Size of buf in bytes
         aafUInt32  bufSize,

         // @parm [in] The position in bytes at which to read
         aafUInt64  position);


  //****************
  // GetSize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSize
        // @parm [out] The size of this file in bytes
        (aafUInt64 *  pSize);

private:
  OMRawStorage * _rep;
};

#endif // ! __ImplAAFGetFileBits_h__

#ifndef __ImplAAFSetFileBits_h__
#define __ImplAAFSetFileBits_h__
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

#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif

#include "OMRawStorage.h"

class ImplAAFSetFileBits : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFSetFileBits ();

protected:
  virtual ~ImplAAFSetFileBits ();

public:

  virtual void
    Initialize
        (OMRawStorage * prs);

  //****************
  // WriteAt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    WriteAt
        (// @parm [in, size_is(bufSize)] Buffer from which data is written
         aafMemPtr_t  buf,

         // @parm [in] Number of bytes
         aafUInt32  bufSize,

         // @parm [in] The position in bytes at which to read
         aafUInt64  position);


  //****************
  // SetSize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetSize
        // @parm [in] The requested file size
        (aafUInt64  size);

private:
  OMRawStorage * _rep;
};

#endif // ! __ImplAAFSetFileBits_h__

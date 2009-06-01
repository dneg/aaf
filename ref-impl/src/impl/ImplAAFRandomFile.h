#ifndef __ImplAAFRandomFile_h__
#define __ImplAAFRandomFile_h__
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



class ImplAAFGetFileBits;

class ImplAAFSetFileBits;





#ifndef __ImplAAFFile_h__
#include "ImplAAFFile.h"
#endif


class ImplAAFRandomFile : public ImplAAFFile
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFRandomFile ();

protected:
  virtual ~ImplAAFRandomFile ();

public:

  //****************
  // GetFileBits()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetFileBits
        // @parm [out] The interface to the GetFileBits object
        (ImplAAFGetFileBits ** ppGetFileBits);


  //****************
  // SaveAsFile()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SaveAsFile
        // @parm [in] The destination file into which the contents of this file are to
    // be written.
        (ImplAAFFile * pDestFile);


  //****************
  // SetFileBits()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetFileBits
        // @parm [out] The interface to the SetFileBits object
        (ImplAAFSetFileBits ** ppSetFileBits);


public:
  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFRandomFileTest.cpp.
  static AAFRESULT test();
};

#endif // ! __ImplAAFRandomFile_h__



#ifndef __ImplAAFRandomFile_h__
#define __ImplAAFRandomFile_h__
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



#ifndef __ImplAAFRandomFile_h__
#define __ImplAAFRandomFile_h__
/***********************************************************************
*
*              Copyright (c) 1998-2000 Avid Technology, Inc.
*
* Permission to use, copy and modify this software and accompanying
* documentation, and to distribute and sublicense application software
* incorporating this software for any purpose is hereby granted,
* provided that (i) the above copyright notice and this permission
* notice appear in all copies of the software and related documentation,
* and (ii) the name Avid Technology, Inc. may not be used in any
* advertising or publicity relating to the software without the specific,
* prior written permission of Avid Technology, Inc.
*
* THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
* EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
* WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
* IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
* SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
* OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
* OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
* ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
* RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
* ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
* LIABILITY.
*
************************************************************************/



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
  // Revert()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Revert ();


  //****************
  // SaveAs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SaveAs
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



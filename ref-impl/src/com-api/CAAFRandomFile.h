//@doc
//@class    AAFRandomFile | Implementation class for AAFRandomFile
#ifndef __CAAFRandomFile_h__
#define __CAAFRandomFile_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
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



#ifndef __AAF_h__
#include "AAF.h"
#endif







#ifndef __CAAFFile_h__
#include "CAAFFile.h"
#endif


class CAAFRandomFile
  : public IAAFRandomFile,
    public CAAFFile
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFRandomFile (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFRandomFile ();

public:


  //***********************************************************
  //
  // GetFileBits()
  //
  // Since the purpose of this method is to obtain file data after a
  // file has been created, it will only succeed if this file has
  // already been Open()ed and then Close()d, or if it was never
  // opened with write access.
  // 
  // Succeeds if:
  // - The ppGetFileBits argument is valid
  // - This file has been opened for write or modify, and has been
  //   subsequently closed.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppGetFileBits arg is NULL.
  //
  // AAFRESULT_BADOPEN
  // - This file has never been opened
  //
  // AAFRESULT_NOT_WRITEABLE
  // - This file has never been opened for write or modify
  //
  // AAFRESULT_BADCLOSE
  // - This file has not been closed after being opened.
  //
  STDMETHOD (GetFileBits) (
    // The interface to the GetFileBits object 
    /*[out]*/ IAAFGetFileBits ** ppGetFileBits);


  //***********************************************************
  //
  // Revert()
  //
  // Reverts this file to the previously-saved state.
    // NOTE! Stub only.   Implementation not yet added.
  //

  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (Revert)
     ();



  //***********************************************************
  //
  // SaveAsFile()
  //
  // Associates this file with the storage specified in pDestFile.
  // Any objects contained in pDestFile will be lost.
  // 
  // Behaves in a manner similar to SaveCopyAs(), except that
  // SaveAs() will change this object to be associated with the new
  // file.
  // 
  // Special case: if f->SaveAs(f) is called, passing this as the
  // save-to argument, the operation will save everything including
  // things which have not changed.
    // NOTE! Stub only.   Implementation not yet added.
  //

  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SaveAsFile) (
    // The destination file into which the contents of this file are to
    // be written. 
    /*[in]*/ IAAFFile * pDestFile);


  //***********************************************************
  //
  // SetFileBits()
  //
  // Since the purpose of this method is to specify file data with
  // which a file is to be created, it will only succeed if this file
  // has not yet been Open()ed.
  //
  // Succeeds if:
  // - The ppSetFileBits argument is valid
  // - This file has not yet been opened.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppSetFileBits arg is NULL.
  //
  // AAFRESULT_BADOPEN
  // - This file has has already been opened
  //
  // AAFRESULT_NOT_READABLE
  // - This read or modify has not been specified for this file
  //
  STDMETHOD (SetFileBits) (
    // The interface to the SetFileBits object 
    /*[out]*/ IAAFSetFileBits ** ppSetFileBits);

protected:
  // 
  // Declare the QI that implements for the interfaces
  // for this module. This will be called by CAAFUnknown::QueryInterface().
  // 
  STDMETHOD(InternalQueryInterface)(REFIID riid, void **ppvObjOut);


public:
  //
  // This class as concrete. All AAF objects can be constructed from
  // a CLSID. This will allow subclassing all "base-classes" by
  // aggreggation.
  // 
  AAF_DECLARE_CONCRETE();
  //
  //********
};

#endif // ! __CAAFRandomFile_h__



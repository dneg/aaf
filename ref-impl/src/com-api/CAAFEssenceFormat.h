//@doc
//@class    AAFEssenceFormat | Implementation class for AAFEssenceFormat
#ifndef __CAAFEssenceFormat_h__
#define __CAAFEssenceFormat_h__

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





#ifndef __CAAFRoot_h__
#include "CAAFRoot.h"
#endif


class CAAFEssenceFormat
  : public IAAFEssenceFormat,
    public CAAFRoot
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFEssenceFormat (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFEssenceFormat ();

public:


  //***********************************************************
  //
  // AddFormatSpecifier()
  //
  // Appends a format specifier to the AAFEssenceFormat.  If an
  // essenceFormatCode has already been added, then this call
  // replaces the value, otherwise the value is appended.
  //
  // The value data is passed in as a void * through the "value"
  // argument.  The size of the value must be passed through the
  // valueSize argument.
  //
  STDMETHOD (AddFormatSpecifier) (
    // From aaddefuids.h
    /*[in]*/ aafUID_constref  essenceFormatCode,

    // Size of preallocated buffer
    /*[in]*/ aafInt32  valueSize,

    // Value data
    /*[in, size_is(valueSize)]*/ aafDataBuffer_t  value);


  //***********************************************************
  //
  // GetFormatSpecifier()
  //
  // The data value is returned in a preallocated buffer of size
  // valueSize.
  //
  // The actual number of bytes read is returned in bytesRead.  If the
  // buffer is not big enough to return the entire  value, an error
  // is returned.
  //
  STDMETHOD (GetFormatSpecifier) (
    // From aaddefuids.h
    /*[in]*/ aafUID_constref  essenceFormatCode,

    // Size of preallocated buffer
    /*[in]*/ aafInt32  valueSize,

    // Preallocated buffer to hold value
    /*[out, size_is(valueSize),length_is(*bytesRead)]*/ aafDataBuffer_t  value,

    // Number of actual bytes read
    /*[out]*/ aafInt32*  bytesRead);


  //***********************************************************
  //
  // NumFormatSpecifiers()
  //
  // The data value is returned in a preallocated buffer of size
  // valueSize.
  //
  STDMETHOD (NumFormatSpecifiers) (
    // The number of specifiers present. 
    /*[out]*/ aafInt32*  numSpecifiers);


  //***********************************************************
  //
  // GetIndexedFormatSpecifier()
  //
  // The data value is returned in a preallocated buffer of size
  // valueSize.
  //
  STDMETHOD (GetIndexedFormatSpecifier) (
    // 0-based index
    /*[in]*/ aafInt32  index,

    // From aaddefuids.h
    /*[out]*/ aafUID_t*  essenceFormatCode,

    // Size of preallocated buffer
    /*[in]*/ aafInt32  valueSize,

    // Preallocated buffer to hold value
    /*[out, size_is(valueSize),length_is(*bytesRead)]*/ aafDataBuffer_t  value,

    // Number of actual bytes read
    /*[out]*/ aafInt32*  bytesRead);

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

#endif // ! __CAAFEssenceFormat_h__



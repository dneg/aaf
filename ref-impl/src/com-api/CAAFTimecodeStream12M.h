//@doc
//@class    AAFTimecodeStream12M | Implementation class for AAFTimecodeStream12M
#ifndef __CAAFTimecodeStream12M_h__
#define __CAAFTimecodeStream12M_h__

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

#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif





#ifndef __CAAFTimecodeStream_h__
#include "CAAFTimecodeStream.h"
#endif


class CAAFTimecodeStream12M
  : public IAAFTimecodeStream12M,
    public CAAFTimecodeStream
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFTimecodeStream12M (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFTimecodeStream12M ();

public:






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

#endif // ! __CAAFTimecodeStream12M_h__



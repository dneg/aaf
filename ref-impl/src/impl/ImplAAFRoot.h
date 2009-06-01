//@doc
//@class    AAFRoot | Implementation class for AAFRoot
#ifndef __ImplAAFRoot_h__
#define __ImplAAFRoot_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
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







//
// Forward declaration
//
struct IAAFRoot;
class AAFRoot;

#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif

#ifndef __AAFResult_h_
#include "AAFResult.h"
#endif

class ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFRoot ();
  virtual ~ImplAAFRoot ();

public:
  // Reference Counting interface
  virtual aafUInt32 AcquireReference() const;
  virtual aafUInt32 ReleaseReference();
  virtual aafUInt32 ReferenceCount() const;


public:
  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFRootTest.cpp.
  static AAFRESULT test();

  void InitContainer (void * pContainer);

  void * GetContainer ();

private:

  void * _pContainer;
  
  bool   _isInitialized;


protected:
  bool isInitialized (void) const;
  void setInitialized (void);
};

#endif // ! __ImplAAFRoot_h__



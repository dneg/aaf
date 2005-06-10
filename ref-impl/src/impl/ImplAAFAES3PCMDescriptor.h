#ifndef __ImplAAFAES3PCMDescriptor_h__
#define __ImplAAFAES3PCMDescriptor_h__

//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2005, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#include "ImplAAFPCMDescriptor.h"



class ImplAAFAES3PCMDescriptor : public ImplAAFPCMDescriptor
{
public:
  //****************
  // Constructor
  //
  ImplAAFAES3PCMDescriptor ();

protected:
  //****************
  // Desctructor
  //
  virtual ~ImplAAFAES3PCMDescriptor ();

public:
  //********************************
  // AAF SDK exported methods start
  //********************************

  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize ();

  //********************************
  // AAF SDK exported methods start
  //********************************
};

#endif // ! __ImplAAFAES3PCMDescriptor_h__

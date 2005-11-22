#ifndef __AxDescriptiveFramework_h__
#define __AxDescriptiveFramework_h__

//=---------------------------------------------------------------------=
//
// $Id$
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
//=---------------------------------------------------------------------=

#include "AxSmartPointer.h"
#include "AxObject.h"


class AxDescriptiveFramework : public AxObject {
 public:
  AxDescriptiveFramework( IAAFDescriptiveFrameworkSP spIaafDescriptiveFramework );
  virtual ~AxDescriptiveFramework();
 
  inline operator IAAFDescriptiveFrameworkSP ()
    { return _spIaafDescriptiveFramework; }

 private:
  AxDescriptiveFramework();
  AxDescriptiveFramework( const AxDescriptiveFramework& );
  AxDescriptiveFramework& operator=( const AxDescriptiveFramework& );

  IAAFDescriptiveFrameworkSP _spIaafDescriptiveFramework;
};

//=---------------------------------------------------------------------=

#endif

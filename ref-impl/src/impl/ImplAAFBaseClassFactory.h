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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#ifndef __ImplAAFBaseClassFactory_h__
#define __ImplAAFBaseClassFactory_h__ 1

//!This file should be merged into other files over time

#include "AAFTypes.h"

// Forward declarations.
class ImplAAFObject;


class ImplAAFBaseClassFactory
{
public:
	ImplAAFBaseClassFactory(void);
	~ImplAAFBaseClassFactory(void);

  
  // Utility to find the code class id for the corresponding stored
  // object id.
  static const aafClassID_t* 
    LookupClassID(const aafUID_t & auid);
};


#endif /* __ImplAAFBaseClassFactory_h__	*/

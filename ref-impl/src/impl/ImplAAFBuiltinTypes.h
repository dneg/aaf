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

#ifndef __ImplAAFBuiltinTypes_h__
#define __ImplAAFBuiltinTypes_h__

//
// Support for built-in type definitions.
//


#include "AAFTypes.h"

#ifndef __ImplAAFSearchableStack_h__
#include "ImplAAFSearchableStack.h"
#endif

#ifndef __ImplAAFUID_h__
#include "ImplAAFUID.h"
#endif

class ImplAAFDictionary;
class ImplAAFTypeDef;
class ImplAAFTypeDefEnum;
class ImplAAFTypeDefRecord;


class ImplAAFBuiltinTypes
{
public:
  ImplAAFBuiltinTypes (ImplAAFDictionary* dictionary);

  //
  // Creates the requested type def object, registers it in the
  // dictionary, and initializes the OM properties therein.
  //
  AAFRESULT ImportBuiltinTypeDef (const aafUID_t & rTypeID,
								  ImplAAFTypeDef ** ppResult);


  //
  // Creates the requested type def object and initializes the OM
  // properties therein.
  //
  AAFRESULT NewBuiltinTypeDef (const aafUID_t & rTypeID,
							   ImplAAFTypeDef ** ppResult);

  //
  // If the given type defs are built-in, will register their
  // offsets/sizes.
  //
  static void RegisterExistingType (ImplAAFTypeDefEnum * ptde);
  static void RegisterExistingType (ImplAAFTypeDefRecord * ptdr);

private:

  ImplAAFDictionary* _dictionary; // pointer back to associated dictionary (temp)

  ImplAAFSearchableStack<ImplAAFUID> _lookupStack;
};

#endif // ! __ImplAAFBuiltinTypes_h__

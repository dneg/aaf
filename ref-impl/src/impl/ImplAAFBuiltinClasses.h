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

#ifndef __ImplAAFBuiltinClasses_h__
#define __ImplAAFBuiltinClasses_h__

//
// Support for built-in class definitions.
//

#include "AAFTypes.h"
#include "AAFUtils.h"

#ifndef __ImplAAFSearchableStack_h__
#include "ImplAAFSearchableStack.h"
#endif

#ifndef __ImplAAFPropertyDef_h__
#include "ImplAAFPropertyDef.h"
#endif

#ifndef __ImplAAFClassDef_h__
#include "ImplAAFClassDef.h"
#endif

#ifndef __ImplAAFUID_h__
#include "ImplAAFUID.h"
#endif

class ImplAAFClassDef;
class ImplAAFDictionary;
class ClassDefinition;

class ImplAAFBuiltinClasses
{
public:
  ImplAAFBuiltinClasses (ImplAAFDictionary* dictionary);
  ~ImplAAFBuiltinClasses ();

  //
  // Creates the requested class object, registers it in the
  // dictionary, and initializes the OM properties therein.
  //
  AAFRESULT NewBuiltinClassDef (const aafUID_t & rClassID,
								ImplAAFClassDef ** pcd);


  // Looks up the given property definition AUID.  If found in the
  // table of built-ins, places the corresponding OM pid in outPid
  // and returns AAFRESULT_SUCCESS.  If not found, does nothing with
  // outPid and returns E_FAIL.
  AAFRESULT LookupOmPid (const aafUID_t & rAuid,
						 OMPropertyId & outPid) const;

  // Returns the axiomatic class definition corresponding to the given
  // class ID.  If not found, returns null.
  ImplAAFClassDef * LookupAxiomaticClass (const aafUID_t & classId);

  // Returns true if the class corresponding to the named classId is
  // axiomatic; returns false otherwise.
  bool IsAxiomaticClass (const aafUID_t & classId) const;

private:

  // Identifies all properties defined as built-in for the given
  // (built-in) class definition, and registers them in the class
  // def.  Will not work for client-extended properties of built-in
  // classes, or for client-extended classes.
  void RegisterBuiltinProperties
    (const ClassDefinition * classDefinition, 
     ImplAAFClassDef * pClassDef) const;


  //
  // Initializes this object and populates it with property definitions.
  //
  AAFRESULT InitBuiltinClassDef (const aafUID_t & rClassID,
								 const ClassDefinition * classDefinition,
								 ImplAAFClassDef * pClass);



  ImplAAFDictionary* _dictionary; // pointer back to associated dictionary (temp)

  ImplAAFSearchableStack<ImplAAFUID> _createStack;
  ImplAAFSearchableStack<ImplAAFUID> _initStack;
};

#endif // ! __ImplAAFBuiltinClasses_h__

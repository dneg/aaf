/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/

#ifndef __ImplAAFBuiltinClasses_h__
#define __ImplAAFBuiltinClasses_h__

//
// Support for built-in class definitions.
//

#include "AAFTypes.h"
#include "OMProperty.h"

class ImplAAFClassDef;
class ImplAAFDictionary;

class ImplAAFBuiltinClasses
{
public:
  ImplAAFBuiltinClasses (ImplAAFDictionary* dictionary);
  ~ImplAAFBuiltinClasses ();

  //
  // Creates the requested class object, registers it in the
  // dictionary, and initializes the OM properties therein.
  //
  AAFRESULT ImportBuiltinClassDef (const aafUID_t & classID,
								   ImplAAFClassDef ** ppResult);

private:

  //
  // Will create a new class definition object and return it to the
  // caller.  No attempt is made to see if this class def has already
  // been created.  This class will not have any properties defined
  // yet.
  //
  AAFRESULT NewBuiltinClassDef (const aafUID_t & rClassID,
								ImplAAFClassDef ** pcd);


  struct ClassTblEntry
  {
    const aafUID_t     * pThisId;
	const aafCharacter * pName;
	const aafUID_t     * pParentId;
  };

  static const ClassTblEntry sBuiltinClassTable[];

  ImplAAFDictionary* _dictionary; // pointer back to associated dictionary (temp)
};

#endif // ! __ImplAAFBuiltinClasses_h__

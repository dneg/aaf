/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/

#ifndef __ImplAAFBuiltinTypes_h__
#define __ImplAAFBuiltinTypes_h__

//
// Support for built-in type definitions.
//

#include "AAFTypes.h"
#include "OMProperty.h"

class ImplAAFTypeDef;
class ImplAAFTypeDefEnum;
class ImplAAFTypeDefInt;
class ImplAAFTypeDefFixedArray;
class ImplAAFTypeDefRecord;
class ImplAAFTypeDefVariableArray;
class ImplAAFTypeDefStrongObjRef;
class ImplAAFTypeDefString;
class ImplAAFDictionary;

class ImplAAFBuiltinTypes
{
public:
  ImplAAFBuiltinTypes (ImplAAFDictionary* dictionary);
  ~ImplAAFBuiltinTypes ();


  //
  // Creates the requested type def object, registers it in the
  // dictionary, and initializes the OM properties therein.
  //
  AAFRESULT ImportBuiltinTypeDef (const aafUID_t & rTypeID,
								  ImplAAFTypeDef ** ppResult);

private:

  //
  // Will create a new type definition object and return it to the
  // caller.  No attempt is made to see if this type def has already
  // been created.
  //
  AAFRESULT NewBuiltinTypeDef (const aafUID_t & rTypeID,
							   ImplAAFTypeDef ** ppResult);


  AAFRESULT TypeDefAUID (ImplAAFTypeDef ** ptd);
  AAFRESULT TypeDefAUIDArray (ImplAAFTypeDef ** ptd);
  AAFRESULT TypeDefUInt8Array (ImplAAFTypeDef ** ptd);
  AAFRESULT TypeDefUInt8Array8 (ImplAAFTypeDef ** ptd);
  AAFRESULT TypeDefUInt8 (ImplAAFTypeDef ** ptd);
  AAFRESULT TypeDefUInt16 (ImplAAFTypeDef ** ptd);
  AAFRESULT TypeDefInt16 (ImplAAFTypeDef ** ptd);
  AAFRESULT TypeDefUInt32 (ImplAAFTypeDef ** ptd);
  AAFRESULT TypeDefInt32 (ImplAAFTypeDef ** ptd);
  AAFRESULT TypeDefInt64 (ImplAAFTypeDef ** ptd);
  AAFRESULT TypeDefObjRef (ImplAAFTypeDef ** ptd);
  AAFRESULT TypeDefObjRefArray (ImplAAFTypeDef ** ptd);
  AAFRESULT TypeDefWCharString (ImplAAFTypeDef ** ptd);
  AAFRESULT TypeDefFadeType (ImplAAFTypeDef ** ptd);

  ImplAAFDictionary* _dictionary; // pointer back to associated dictionary (temp)
  ImplAAFTypeDefRecord *        _TD_AUID;
  ImplAAFTypeDefVariableArray * _TD_AUIDArray;
  ImplAAFTypeDefVariableArray * _TD_UInt8Array;
  ImplAAFTypeDefFixedArray *    _TD_UInt8Array8;
  ImplAAFTypeDefInt *           _TD_UInt8;
  ImplAAFTypeDefInt *           _TD_UInt16;
  ImplAAFTypeDefInt *           _TD_Int16;
  ImplAAFTypeDefInt *           _TD_UInt32;
  ImplAAFTypeDefInt *           _TD_Int32;
  ImplAAFTypeDefInt *           _TD_Int64;
  ImplAAFTypeDefStrongObjRef *  _TD_ObjRef;
  ImplAAFTypeDefVariableArray * _TD_ObjRefArray;
  ImplAAFTypeDefString *        _TD_WCharString;
  ImplAAFTypeDefEnum *          _TD_FadeType;

  typedef AAFRESULT (ImplAAFBuiltinTypes::*pTypeFunc_t)(ImplAAFTypeDef**);

  struct TypeTblEntry
  {
	const aafUID_t * pID;
	pTypeFunc_t      pTypeFunc;
  };

  static /*const*/ TypeTblEntry sBuiltinTypeTable[];

};

#endif // ! __ImplAAFBuiltinTypes_h__

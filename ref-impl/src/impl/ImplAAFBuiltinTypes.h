/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 *  prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/

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

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

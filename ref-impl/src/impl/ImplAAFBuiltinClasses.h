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

  ImplAAFClassDef *             _CD_Object;
};

#endif // ! __ImplAAFBuiltinClasses_h__

//@doc
//@class    AAFTypeDefOpaque | Implementation class for AAFTypeDefOpaque
#ifndef __ImplAAFTypeDefOpaque_h__
#define __ImplAAFTypeDefOpaque_h__


/***********************************************************************
*
*              Copyright (c) 1998-2000 Avid Technology, Inc.
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
* THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
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

#ifndef __ImplAAFTypeDefIndirect_h__
#include "ImplAAFTypeDefIndirect.h"
#endif

class ImplAAFPropertyValue;


class ImplAAFTypeDefOpaque : public ImplAAFTypeDefIndirect
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTypeDefOpaque ();

protected:
  virtual ~ImplAAFTypeDefOpaque ();

public:

  //****************
  // GetActualTypeID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetActualTypeID
        (// @parm [in] indirect property value to read
         ImplAAFPropertyValue * pOpaquePropertyValue,

         // @parm [out] the type id of the actual data
         aafUID_t *  pActualTypeID);

  // Override from AAFTypeDef
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTypeCategory (/*[out]*/ eAAFTypeCategory_t *  pTid);

public:

  //****************
  // pvtInitialize() 
  //   Called when we initialize as one of the "builtin" types.
  //
  AAFRESULT
    pvtInitialize
        (// @parm [in, ref] auid to be used to identify this type
         aafUID_constref  id,

         // @parm [in, ref, string] friendly name of this type definition
         aafCharacter_constptr  pTypeName);


protected:
  // Find the actual type definition from the dictionary.
	// This override of TypeDefIndirect will also look in the
	// dictionary for opaque types.
  virtual AAFRESULT LookupActualType (aafUID_constref typeID, 
                                      ImplAAFTypeDef ** ppActualType) const;

	
};

#endif // ! __ImplAAFTypeDefOpaque_h__



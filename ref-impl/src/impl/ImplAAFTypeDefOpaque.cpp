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

#ifndef __ImplAAFTypeDefOpaque_h__
#include "ImplAAFTypeDefOpaque.h"
#endif

#ifndef __ImplAAFPropertyValue_h__
#include "ImplAAFPropertyValue.h"
#endif

#ifndef __ImplAAFDictionary_h__
#include "ImplAAFDictionary.h"
#endif



#include "AAFStoredObjectIDs.h"


#include <assert.h>
#include <string.h>


ImplAAFTypeDefOpaque::ImplAAFTypeDefOpaque ()
{}


ImplAAFTypeDefOpaque::~ImplAAFTypeDefOpaque ()
{}

//   Called when we initialize as one of the "builtin" types.
//
AAFRESULT
  ImplAAFTypeDefOpaque::pvtInitialize
      (// @parm [in, ref] auid to be used to identify this type
       aafUID_constref  id,

       // @parm [in, ref, string] friendly name of this type definition
       aafCharacter_constptr  pTypeName)
{
  return ImplAAFTypeDefIndirect::pvtInitialize(id, pTypeName);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefOpaque::GetActualTypeID (
      ImplAAFPropertyValue * pOpaquePropertyValue,
      aafUID_t *  pActualTypeID)
{
  //
	// Validate the indirect value and extract common information...
	//
	aafUInt32  indirectValueSize = 0;
  aafUInt32 actualValueDataSize = 0;
	aafMemPtr_t pIndirectValueDataBits = NULL;
  return (GetIndirectValueInfo (pOpaquePropertyValue, indirectValueSize, pIndirectValueDataBits,
                                NULL, NULL, pActualTypeID));
}


// Find the actual type definition from the dictionary.
// This override of TypeDefIndirect will also look in the
// dictionary for opaque types.
AAFRESULT ImplAAFTypeDefOpaque::LookupActualType (
	aafUID_constref typeID, 
	ImplAAFTypeDef ** ppActualType) const
{
	if (!ppActualType)
		return (AAFRESULT_NULL_PARAM);

	// First see if this is a known type:
	AAFRESULT result = ImplAAFTypeDefIndirect::LookupActualType(typeID, ppActualType);
	if (AAFRESULT_FAILED(result))
	{
		// See if this is an opaque type.
		result = _dictionary->LookupOpaqueTypeDef(typeID, ppActualType);
	}

	return (result);
}



// Override from AAFTypeDef
AAFRESULT STDMETHODCALLTYPE
  ImplAAFTypeDefOpaque::GetTypeCategory (/*[out]*/ eAAFTypeCategory_t *  pTid)
{
  if (!pTid)
    return AAFRESULT_NULL_PARAM;
  *pTid = kAAFTypeCatOpaque;
  return AAFRESULT_SUCCESS;
}





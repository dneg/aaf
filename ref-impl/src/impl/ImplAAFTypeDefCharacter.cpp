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



#ifndef __ImplAAFPropertyValue_h__
#include "ImplAAFPropertyValue.h"
#endif

#include "AAFStoredObjectIDs.h"

#ifndef __ImplAAFTypeDefCharacter_h__
#include "ImplAAFTypeDefCharacter.h"
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"
#include "ImplAAFObjectCreation.h"
#include "AAFClassIDs.h"

#ifndef __ImplAAFPropValData_h__
#include "ImplAAFPropValData.h"
#endif

#ifndef __AAFTypeDefUIDs_h__
#include "AAFTypeDefUIDs.h"
#endif

#include <assert.h>
#include <string.h>

// We only support two byte unicode characters.
const aafUInt32 kExternalCharacterSize = 2;


//some macros
#define check_hr(expr)\
{\
	HRESULT  the_hresult = (expr);\
	if (FAILED(the_hresult))\
	   return the_hresult;\
}


ImplAAFTypeDefCharacter::ImplAAFTypeDefCharacter ()
{}

ImplAAFTypeDefCharacter::~ImplAAFTypeDefCharacter ()
{}


AAFRESULT ImplAAFTypeDefCharacter::pvtInitialize (
      aafUID_constref  id,
      aafCharacter_constptr  pTypeName)
{
  // The description string needs to be initialized elsewhere...
  return ImplAAFMetaDefinition::Initialize(id, pTypeName, L"Two-byte Unicode Character Type Definition");
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefCharacter::CreateValueFromCharacter (
      aafCharacter  character,
      ImplAAFPropertyValue ** ppCharacterValue)
{
    if (! ppCharacterValue)
		return AAFRESULT_NULL_PARAM;
	
	aafUInt32 cbChar = NativeSize();
	
	// Create a temporary pointer to copy to the smartptr
	ImplAAFPropValData * tmp = (ImplAAFPropValData *)CreateImpl(CLSID_AAFPropValData);
	if (NULL == tmp)
		return AAFRESULT_NOMEMORY;
	ImplAAFPropValDataSP pv;
	pv = tmp;
	
	tmp->ReleaseReference(); // we don't need this reference anymore.
	tmp = 0;
	
	//Initialize
	check_hr ( pv->Initialize(this) );
	
	//Allocate appropriate bits
	aafMemPtr_t pBits = NULL;
	check_hr ( pv->AllocateBits (cbChar, &pBits) );
	
	//Set the bits to incoming character
	assert (pBits);
	memcpy (pBits, &character, cbChar);
	
	*ppCharacterValue = pv;
	(*ppCharacterValue)->AcquireReference ();
	return AAFRESULT_SUCCESS;
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefCharacter::SetCharacter (
      ImplAAFPropertyValue * pCharacterValue,
      aafCharacter  character)
{
	if (! pCharacterValue)
		return AAFRESULT_NULL_PARAM;
	
	//get a pointer to the Val Data
	ImplAAFPropValDataSP pvd;
	pvd = dynamic_cast<ImplAAFPropValData*>(pCharacterValue);
	if (!pvd) return AAFRESULT_BAD_TYPE;
	
	// get the property value's embedded type
	ImplAAFTypeDefSP pPropType;
	check_hr ( pvd->GetType (&pPropType) );
	assert (pPropType);
	//Make sure the TD of the pv passed in, matches that of the ImplAAFTypeDefCharacter
	if ((ImplAAFTypeDef *)pPropType != this) // call operator ImplAAFTypeDef *
		return AAFRESULT_BAD_TYPE;
	
	//check to make sure that the size in the val data matches that of the native size
	aafUInt32 cbChar = 0;
	check_hr ( pvd->GetBitsSize(&cbChar) );
	
	if (cbChar != NativeSize())
	{
		return AAFRESULT_BAD_SIZE;
	}
	
	//ok all set with initial conditions
	//now set the value to the incoming character
	
	aafMemPtr_t pBits = NULL;
	check_hr ( pvd->GetBits (&pBits)  );
	assert (pBits);
	
	memcpy (pBits, &character, cbChar);
	
	return AAFRESULT_SUCCESS;
	
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefCharacter::GetCharacter (
      ImplAAFPropertyValue * pCharacterValue,
      aafCharacter *  pCharacter)
{
	if (! pCharacterValue)
		return AAFRESULT_NULL_PARAM;
	
	if (! pCharacter)
		return AAFRESULT_NULL_PARAM;

	//get a pointer to the Val Data
	ImplAAFPropValDataSP pvd;
	pvd = dynamic_cast<ImplAAFPropValData*>(pCharacterValue);
	if (!pvd) return AAFRESULT_BAD_TYPE;
	
	// get the property value's embedded type
	ImplAAFTypeDefSP pPropType;
	check_hr ( pvd->GetType (&pPropType) );
	assert (pPropType);
	//Make sure the TD of the pv passed in, matches that of the ImplAAFTypeDefCharacter
	if ((ImplAAFTypeDef *)pPropType != this) // call operator ImplAAFTypeDef *
		return AAFRESULT_BAD_TYPE;
	
	//check to make sure that the size in the val data matches that of the native size
	aafUInt32 cbChar = 0;
	check_hr (  pvd->GetBitsSize(&cbChar) );

	if (cbChar != NativeSize())
	{
		return AAFRESULT_BAD_SIZE;
	}

	//Now set the character from that contained in the prop val data

	aafMemPtr_t pBits = NULL;
	check_hr ( pvd->GetBits (&pBits) );
	assert (pBits);
	
	memcpy (pCharacter, pBits, cbChar);
	
	return AAFRESULT_SUCCESS;
}




// Override from AAFTypeDef
AAFRESULT STDMETHODCALLTYPE
  ImplAAFTypeDefCharacter::GetTypeCategory (/*[out]*/ eAAFTypeCategory_t * pTid)
{
  if (!pTid)
    return AAFRESULT_NULL_PARAM;
  *pTid = kAAFTypeCatCharacter;
  return AAFRESULT_SUCCESS;
}



// class ImplAAFTypeDefCharacter

void ImplAAFTypeDefCharacter::reorder(OMByte* externalBytes,
                             size_t externalBytesSize) const
{
  TRACE("ImplAAFTypeDefCharacter::reorder");
  PRECONDITION("Valid external bytes", externalBytes != 0);
  PRECONDITION("Valid external bytes size", externalBytesSize == kExternalCharacterSize );

  reorderInteger(externalBytes, kExternalCharacterSize);
}

size_t ImplAAFTypeDefCharacter::externalSize(OMByte* internalBytes,
                                    size_t internalBytesSize) const
{
  TRACE("WideStringType::externalSize");

  PRECONDITION("Valid internal bytes", internalBytes != 0);
  PRECONDITION("Valid internal bytes size", internalBytesSize >= kExternalCharacterSize);
  
  return kExternalCharacterSize;
}

void ImplAAFTypeDefCharacter::externalize(OMByte* internalBytes,
                                 size_t internalBytesSize,
                                 OMByte* externalBytes,
                                 size_t externalBytesSize,
                                 OMByteOrder byteOrder) const
{
  TRACE("WideStringType::externalize");
  PRECONDITION("Valid internal bytes", internalBytes != 0);
  PRECONDITION("Valid internal bytes size",
          internalBytesSize >= internalSize(externalBytes, externalBytesSize));
  PRECONDITION("Valid external bytes", externalBytes != 0);
  PRECONDITION("Valid external bytes size",
          externalBytesSize >= externalSize(internalBytes, internalBytesSize));

  if (sizeof(aafCharacter) == kExternalCharacterSize)
    copy(internalBytes, externalBytes, externalBytesSize);
  else
    contract(internalBytes, sizeof(aafCharacter), externalBytes, kExternalCharacterSize, byteOrder);
}

size_t ImplAAFTypeDefCharacter::internalSize(OMByte* externalBytes,
                                    size_t externalBytesSize) const
{
  TRACE("ImplAAFTypeDefCharacter::internalSize");

  PRECONDITION("Valid external bytes", externalBytes != 0);
  PRECONDITION("Valid external bytes size", externalBytesSize == kExternalCharacterSize);

  return (sizeof(aafCharacter));
}

void ImplAAFTypeDefCharacter::internalize(OMByte* externalBytes,
                                 size_t externalBytesSize,
                                 OMByte* internalBytes,
                                 size_t internalBytesSize,
                                 OMByteOrder byteOrder) const
{
  TRACE("ImplAAFTypeDefCharacter::internalize");
  PRECONDITION("Valid internal bytes", internalBytes != 0);
  PRECONDITION("Valid internal bytes size",
          internalBytesSize >= internalSize(externalBytes, externalBytesSize));
  PRECONDITION("Valid external bytes", externalBytes != 0);
  PRECONDITION("Valid external bytes size",
          externalBytesSize >= externalSize(internalBytes, internalBytesSize));

  if (sizeof(aafCharacter) == kExternalCharacterSize)
    copy(externalBytes, internalBytes, internalBytesSize);
  else
    expand(externalBytes, kExternalCharacterSize, internalBytes, sizeof(aafCharacter), byteOrder);
}








//
// Overrides of ImplAAFTypeDef
//
//
// Override from AAFTypeDef

aafBool ImplAAFTypeDefCharacter::IsFixedSize (void) const
{
  return kAAFTrue;
}


size_t ImplAAFTypeDefCharacter::PropValSize (void) const
{
  return kExternalCharacterSize; // We only support 2 byte unicode characters.
}


aafBool ImplAAFTypeDefCharacter::IsRegistered (void) const
{
  // Only depends on registration of basic type.
  return kAAFTrue;
}


size_t ImplAAFTypeDefCharacter::NativeSize (void) const
{
  assert( (2 == sizeof(aafCharacter)) || (4 == sizeof(aafCharacter)));
  return (sizeof(aafCharacter));
}


OMProperty * ImplAAFTypeDefCharacter::pvtCreateOMProperty
  (OMPropertyId pid,
   const wchar_t * name) const
{
  assert (name);

  OMProperty * result = 0;

	result = new OMFixedSizeProperty<aafCharacter> (pid, name);

  assert (result); // need better error handling!
  return result;
}



bool ImplAAFTypeDefCharacter::IsAggregatable () const
{ return true; }

bool ImplAAFTypeDefCharacter::IsStreamable () const
{ return true; }

bool ImplAAFTypeDefCharacter::IsFixedArrayable () const
{ return true; }

bool ImplAAFTypeDefCharacter::IsVariableArrayable () const
{ return true; }

bool ImplAAFTypeDefCharacter::IsStringable () const
{ return true; }






// override from OMStorable.
const OMClassId& ImplAAFTypeDefCharacter::classId(void) const
{
  return (*reinterpret_cast<const OMClassId *>(&AUID_AAFTypeDefCharacter));
}

// Override callbacks from OMStorable
void ImplAAFTypeDefCharacter::onSave(void* clientContext) const
{
  ImplAAFTypeDef::onSave(clientContext);
}

void ImplAAFTypeDefCharacter::onRestore(void* clientContext) const
{
  ImplAAFTypeDef::onRestore(clientContext);
}

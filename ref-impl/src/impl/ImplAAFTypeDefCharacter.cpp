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
  TRACE("ImplAAFTypeDefCharacter::CreateValueFromCharacter");
  
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
	ASSERT("Valid bits", pBits != 0);
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
  TRACE("ImplAAFTypeDefCharacter::SetCharacter");
  
	if (! pCharacterValue)
		return AAFRESULT_NULL_PARAM;
	
	//get a pointer to the Val Data
	ImplAAFPropValDataSP pvd;
	pvd = dynamic_cast<ImplAAFPropValData*>(pCharacterValue);
	if (!pvd) return AAFRESULT_BAD_TYPE;
	
	// get the property value's embedded type
	ImplAAFTypeDefSP pPropType;
	check_hr ( pvd->GetType (&pPropType) );
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
	ASSERT("Valid bits", pBits != 0);
	
	memcpy (pBits, &character, cbChar);
	
	return AAFRESULT_SUCCESS;
	
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFTypeDefCharacter::GetCharacter (
      ImplAAFPropertyValue * pCharacterValue,
      aafCharacter *  pCharacter)
{
  TRACE("ImplAAFTypeDefCharacter::GetCharacter");
  
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
	ASSERT("Valid bits", pBits != 0);
	
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
                             size_t ANAME(externalBytesSize)) const
{
  TRACE("ImplAAFTypeDefCharacter::reorder");
  PRECONDITION("Valid external bytes", externalBytes != 0);
  PRECONDITION("Valid external bytes size", externalBytesSize == kExternalCharacterSize );

  reorderInteger(externalBytes, kExternalCharacterSize);
}

size_t ImplAAFTypeDefCharacter::externalSize(OMByte* ANAME(internalBytes),
                                    size_t ANAME(internalBytesSize)) const
{
  TRACE("ImplAAFTypeDefCharacter::externalSize");

  PRECONDITION("Valid internal bytes", internalBytes != 0);
  PRECONDITION("Valid internal bytes size", internalBytesSize >= kExternalCharacterSize);
  
  return kExternalCharacterSize;
}

void ImplAAFTypeDefCharacter::externalize(OMByte* internalBytes,
                                 size_t ANAME(internalBytesSize),
                                 OMByte* externalBytes,
                                 size_t externalBytesSize,
                                 OMByteOrder byteOrder) const
{
  TRACE("ImplAAFTypeDefCharacter::externalize");
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

size_t ImplAAFTypeDefCharacter::internalSize(OMByte* ANAME(externalBytes),
                                    size_t ANAME(externalBytesSize)) const
{
  TRACE("ImplAAFTypeDefCharacter::internalSize");

  PRECONDITION("Valid external bytes", externalBytes != 0);
  PRECONDITION("Valid external bytes size", externalBytesSize == kExternalCharacterSize);

  return (sizeof(aafCharacter));
}

void ImplAAFTypeDefCharacter::internalize(OMByte* externalBytes,
                                 size_t ANAME(externalBytesSize),
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
  TRACE("ImplAAFTypeDefCharacter::NativeSize");
  ASSERT("Valid character size", (2 == sizeof(aafCharacter)) || (4 == sizeof(aafCharacter)));
  return (sizeof(aafCharacter));
}


OMProperty * ImplAAFTypeDefCharacter::pvtCreateOMProperty
  (OMPropertyId pid,
   const wchar_t * name) const
{
  TRACE("ImplAAFTypeDefCharacter::pvtCreateOMProperty");
  PRECONDITION("Valid name", name != 0);

  OMProperty * result = 0;

	result = new OMFixedSizeProperty<aafCharacter> (pid, name);

  POSTCONDITION("Valid property allocated", result != 0); // need better error handling!
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

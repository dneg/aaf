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

#include "extensionUtils.h"
#include "AAF.h"
#include <assert.h>
#include "AAFStoredObjectIDs.h"
#include "AAFTypeDefUIDs.h"

#include "AAFResult.h"

#include "CAAFBuiltinDefs.h"

#include <iostream.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>

// utility function to set String property value
static void SetStringProperty (IAAFObject * pObj,
						aafUID_constref propertyID,
						aafCharacter_constptr strValue)
{
  assert (pObj);
  assert (strValue);

  IAAFClassDef *cd=NULL;
  IAAFPropertyDef *pd=NULL;
  IAAFTypeDef *td=NULL;
  IAAFTypeDefString *tds=NULL;
  IAAFPropertyValue *pv=NULL;


  try
  {
    check (pObj->GetDefinition (&cd));

    check (cd->LookupPropertyDef (propertyID,
								  &pd));

    check (pd->GetTypeDef (&td));

    check (td->QueryInterface (IID_IAAFTypeDefString, (void **)&tds));

    check (tds->CreateValueFromCString ( (aafMemPtr_t) strValue,
										   wcslen (strValue) * sizeof (aafCharacter), 
										   &pv));

    check (pObj->SetPropertyValue (pd, pv));

    pv->Release();
    pv=NULL;
    tds->Release();
    tds=NULL;
    td->Release();
    td=NULL;
    pd->Release();
    pd=NULL;
    cd->Release();
    cd=NULL;
  }
  catch (...)
  {
    // cleanup after error...
    if (pv)
      pv->Release();
    if (tds)
      tds->Release();
    if (td)
      td->Release();
    if (pd)
      pd->Release();
    if (cd)
      cd->Release();
    throw;
  }

}

static void GetStringBufLen (IAAFObject * pObj,
							 const aafUID_t propertyID,
							 aafUInt32 *bufLen)
{
  assert (pObj);
  assert (bufLen);
		
  IAAFClassDef *cd=NULL;
  IAAFPropertyDef *pd=NULL;
  IAAFPropertyValue *pv=NULL;
  IAAFTypeDef *td=NULL;
  IAAFTypeDefString *tds=NULL;

  
  try
  {
    check (pObj->GetDefinition (&cd));

    check (cd->LookupPropertyDef (propertyID,
								  &pd));

    check (pObj->GetPropertyValue (pd, &pv));
  
    check (pd->GetTypeDef (&td));

    check (td->QueryInterface (IID_IAAFTypeDefString, (void **)&tds));

    aafUInt32 numChars = 0;
    check (tds->GetCount (pv, &numChars));


    // Add 1 for null terminator and 
    // convert from count of aafCharacter to bytes
    *bufLen= (numChars + 1) * sizeof (aafCharacter);
    //*bufLen = (numChars +1)*2;

    tds->Release();
    tds=NULL;
    td->Release();
    td=NULL;
    pv->Release();
    pv=NULL;
    pd->Release();
    pd=NULL;
    cd->Release();
    cd=NULL;
  }
  catch (...)
  {
    // cleanup after error...
    if (tds)
      tds->Release();
    if (td)
      td->Release();
    if (pv)
      pv->Release();
    if (pd)
      pd->Release();
    if (cd)
      cd->Release();
    throw;
  }

}

void GetStringValue (IAAFObject * pObj,
							 const aafUID_t propertyID,
							 aafCharacter * buf,
							 aafUInt32 buflen)
{
  assert (pObj);
  assert (buf);

  IAAFClassDef *cd=NULL;
  IAAFPropertyDef *pd=NULL;
  IAAFPropertyValue *pv=NULL;
  IAAFTypeDef *td=NULL;
  IAAFTypeDefString *tds=NULL;
  aafUInt32 stringBufLen,numCharsWNull;


  try
  {
    GetStringBufLen (pObj, propertyID,
					  &stringBufLen);
    assert (buflen >= stringBufLen);
    numCharsWNull = stringBufLen/sizeof (aafCharacter);

    check (pObj->GetDefinition (&cd));

    check (cd->LookupPropertyDef (propertyID,
								  &pd));

    check (pObj->GetPropertyValue (pd, &pv));
  
    check (pd->GetTypeDef (&td));

    check (td->QueryInterface (IID_IAAFTypeDefString, (void **)&tds));

    check (tds->GetElements (pv, (aafMemPtr_t) buf, buflen));

    // Make sure we're null-terminated.
    buf[numCharsWNull-1] = '\0';

    tds->Release();
    tds=NULL;
    td->Release();
    td=NULL;
    pv->Release();
    pv=NULL;
    pd->Release();
    pd=NULL;
    cd->Release();
    cd=NULL;
  }
  catch (...)
  {
    // cleanup after error...
    if (tds)
      tds->Release();
    if (td)
      td->Release();
    if (pv)
      pv->Release();
    if (pd)
      pd->Release();
    if (cd)
      cd->Release();
    throw;
  }
}


// Checks if pClassDefQuery is or is a subclass of class defined for the specified AUID
bool classDefinitionIsA ( IAAFClassDef *pClassDefQuery,
							   aafUID_constref targetAUID)
{
	IAAFMetaDefinition *pMetaDefinition = NULL;
	IAAFClassDef *pParentClass=NULL;
	bool classMatch=false;


  try
  {
	  check(pClassDefQuery->QueryInterface(IID_IAAFMetaDefinition,(void **)&pMetaDefinition));

    aafUID_t testAUID;
	  check(pMetaDefinition->GetAUID(&testAUID));
	  pMetaDefinition->Release();
	  pMetaDefinition=NULL;


    if (!memcmp (&testAUID, &targetAUID, sizeof (aafUID_t)))
	  {
		  // AUIDs match
		  return true;
	  } else if (!memcmp(&testAUID, (aafUID_t *) &AUID_AAFInterchangeObject, sizeof(aafUID_t)))
	  {
		  return false;
	  }

	  // Get parent class
	  check(pClassDefQuery->GetParent(&pParentClass));

	  classMatch= classDefinitionIsA( pParentClass, targetAUID);
	  pParentClass->Release();
	  pParentClass=NULL;
  }
  catch (...)
  {
    // cleanup after error...
    if (pParentClass)
      pParentClass->Release();
    if (pMetaDefinition)
      pMetaDefinition->Release();

    throw;
  }

	return classMatch;
}

bool IsOptionalPropertyPresent (IAAFObject * pObj,
								const aafUID_t propertyID)
{
  aafBool r;
  assert (pObj);

  IAAFClassDef *cd=NULL;
  IAAFPropertyDef *pd=NULL;


  try
  {
    check (pObj->GetDefinition (&cd));

    check (cd->LookupPropertyDef (propertyID,
								  &pd));

    check (pObj->IsPropertyPresent (pd, &r));
    pd->Release();
    pd=NULL;
    cd->Release();
    cd=NULL;
  }
  catch (...)
  {
    // cleanup after error...
    if (pd)
      pd->Release();
    if (cd)
      cd->Release();

    throw;
  }

  return (kAAFTrue == r) ? true : false;
}

// Main functions for PersonnelResource
void PersonnelResourceSetFamilyName(IAAFObject *pResource,
							   aafCharacter_constptr familyName)
{
	assert (pResource);
	assert (familyName);
    SetStringProperty (pResource, kPropID_PersonnelResource_FamilyName,
						familyName);
}

void PersonnelResourceSetGivenName(IAAFObject *pResource,
							   aafCharacter_constptr givenName)
{
	assert (pResource);
	assert(givenName);
    SetStringProperty (pResource, kPropID_PersonnelResource_GivenName,
						givenName);
}


void PersonnelResourceSetPosition (IAAFObject * pResource,
							 const ePosition & position)
{
  assert (pResource);

  IAAFClassDef *cd=NULL;
  IAAFPropertyDef *pd=NULL;
  IAAFPropertyValue *pv=NULL;
  IAAFTypeDef *td=NULL;
  IAAFTypeDefExtEnum *tde=NULL;

  try
  {
    check (pResource->GetDefinition (&cd));

    check (cd->LookupPropertyDef (kPropID_PersonnelResource_Position,
								  &pd));

    check (pResource->GetPropertyValue (pd, &pv));
  
    check (pd->GetTypeDef (&td));

    check (td->QueryInterface (IID_IAAFTypeDefExtEnum, (void **)&tde));

    check (tde->SetAUIDValue (pv, position));
    check (pResource->SetPropertyValue (pd, pv));

    tde->Release();
    tde=NULL;
    td->Release();
    td=NULL;
    pv->Release();
    pv=NULL;
    pd->Release();
    pd=NULL;
    cd->Release();
    cd=NULL;
  }
  catch (...)
  {
    // cleanup after error...
    if (tde)
      tde->Release();
    if (td)
      td->Release();
    if (pv)
      pv->Release();
    if (pd)
      pd->Release();
    if (cd)
      cd->Release();

    throw;
  }
}
void PersonnelResourceSetActorRole (IAAFObject *pResource,
								  aafCharacter_constptr role)
{
	assert (pResource);
	assert (role);
	SetStringProperty (pResource, kPropID_PersonnelResource_Role, role);
}

void PersonnelResourceSetContractID (IAAFObject * pObj,
								   contractID_t cid)
{
  assert (pObj);

  IAAFClassDef *cd=NULL;
  IAAFPropertyDef *pd=NULL;
  IAAFTypeDef *td=NULL;
  IAAFTypeDefInt* tdi=NULL;
  IAAFPropertyValue *pv=NULL;


  try
  {
    check (pObj->GetDefinition (&cd));

    check (cd->LookupPropertyDef (kPropID_PersonnelResource_ContractID,
								  &pd));

    check (pd->GetTypeDef (&td));

    check (td->QueryInterface (IID_IAAFTypeDefInt, (void **)&tdi));

    check (tdi->CreateValue ((aafMemPtr_t) &cid, sizeof (cid), &pv));

    check (pObj->SetPropertyValue (pd, pv));

    pv->Release();
    pv=NULL;
    tdi->Release();
    tdi=NULL;
    td->Release();
    td=NULL;
    pd->Release();
    pd=NULL;
    cd->Release();
    cd=NULL;
  }
  catch (...)
  {
    // cleanup after error...
    if (pv)
      pv->Release();
    if (tdi)
      tdi->Release();
    if (td)
      td->Release();
    if (pd)
      pd->Release();
    if (cd)
      cd->Release();
    throw;
  }
}

void PersonnelResourceInitialize(IAAFObject *pResource,
							   aafCharacter_constptr familyName,
							   aafCharacter_constptr givenName,
							   const ePosition & position)
{
    PersonnelResourceSetFamilyName (pResource, familyName);
    PersonnelResourceSetGivenName (pResource, givenName);
	PersonnelResourceSetPosition (pResource,position);
}

void PersonnelResourceGetGivenNameBufLen (IAAFObject * pObj, aafUInt32 *bufLen)
{
  assert (pObj);
  GetStringBufLen (pObj, kPropID_PersonnelResource_GivenName,
					bufLen);
}


void PersonnelResourceGetGivenName (IAAFObject * pObj,
							 aafCharacter * buf,
							 aafUInt32 buflen)
{
  assert (pObj);
  assert (buf);
  GetStringValue (pObj, kPropID_PersonnelResource_GivenName,
		          buf,buflen);
}

void PersonnelResourceGetFamilyNameBufLen (IAAFObject *pObj,
										   aafUInt32 *bufLen)
{
  assert (pObj);
  GetStringBufLen (pObj, kPropID_PersonnelResource_FamilyName,
					bufLen);
}


void PersonnelResourceGetFamilyName (IAAFObject * pObj,
							 aafCharacter * buf,
							 aafUInt32 buflen)
{
  assert (pObj);
  assert (buf);
  GetStringValue (pObj, kPropID_PersonnelResource_FamilyName,
		          buf,buflen);
}

void PersonnelResourceGetPosition (IAAFObject * pResource,
							 ePosition *position)
{
  assert (pResource);

  IAAFClassDef *cd=NULL;
  IAAFPropertyDef *pd=NULL;
  IAAFPropertyValue *pv=NULL;
  IAAFTypeDef *td=NULL;
  IAAFTypeDefExtEnum *tde=NULL;


  try
  {  
    check (pResource->GetDefinition (&cd));

    check (cd->LookupPropertyDef (kPropID_PersonnelResource_Position,
								  &pd));

    check (pResource->GetPropertyValue (pd, &pv));
  
    check (pd->GetTypeDef (&td));

    check (td->QueryInterface (IID_IAAFTypeDefExtEnum, (void **)&tde));

    check (tde->GetAUIDValue (pv, position));

    tde->Release();
    tde=NULL;
    td->Release();
    td=NULL;
    pv->Release();
    pv=NULL;
    pd->Release();
    pd=NULL;   
    cd->Release();
    cd=NULL;
  }
  catch (...)
  {
    // cleanup after error...
    if (tde)
      tde->Release();
    if (td)
      td->Release();
    if (pv)
      pv->Release();
    if (pd)
      pd->Release();
    if (cd)
      cd->Release();
    throw;
  }

}

HRESULT PersonnelResourceGetContractID (IAAFObject * pObj,
								   contractID_t *cid)
{
  assert (pObj);
  assert (cid);
  if (IsOptionalPropertyPresent (pObj,
	                             kPropID_PersonnelResource_ContractID))
  {
    IAAFClassDef *cd=NULL;
    IAAFPropertyDef *pd=NULL;
    IAAFPropertyValue *pv=NULL;
    IAAFTypeDef *td=NULL;
    IAAFTypeDefInt *tdi=NULL;
    
    try
    {
      check (pObj->GetDefinition (&cd));

      check (cd->LookupPropertyDef (kPropID_PersonnelResource_ContractID,
								  &pd));

      check (pObj->GetPropertyValue (pd, &pv));
  
      check (pd->GetTypeDef (&td));

      check (td->QueryInterface (IID_IAAFTypeDefInt, (void **)&tdi));

      check (tdi->GetInteger (pv, (aafMemPtr_t) cid, sizeof (*cid)));

      tdi->Release();
      tdi=NULL;
      td->Release();
      td=NULL;
      pv->Release();
      pv=NULL;
      pd->Release();
      pd=NULL;
      cd->Release();
      cd=NULL;
    }
    catch (...)
    {
      // cleanup after error...
      if (tdi)
        tdi->Release();
      if (td)
        td->Release();
      if (pv)
        pv->Release();
      if (pd)
        pd->Release();
      if (cd)
        cd->Release();

      throw;
    }

	  return AAFRESULT_SUCCESS;
  } else return AAFRESULT_PROP_NOT_PRESENT;
}


HRESULT PersonnelResourceGetActorNameBufLen (IAAFObject *pObj,
										   aafUInt32 *bufLen)
{
  assert (pObj);
  if (IsOptionalPropertyPresent (pObj,
	                             kPropID_PersonnelResource_Role))
  {
	GetStringBufLen (pObj, kPropID_PersonnelResource_Role,
					bufLen);
	return AAFRESULT_SUCCESS;
  } else return AAFRESULT_PROP_NOT_PRESENT;
}

HRESULT PersonnelResourceGetActorName (IAAFObject * pObj,
							 aafCharacter * buf,
							 aafUInt32 buflen)
{
  assert (pObj);
  assert (buf);
  if (IsOptionalPropertyPresent (pObj,
	                             kPropID_PersonnelResource_Role))
  {
	GetStringValue (pObj, kPropID_PersonnelResource_Role,
		          buf,buflen);
	return AAFRESULT_SUCCESS;
  } else return AAFRESULT_PROP_NOT_PRESENT;
}

// On Irix CC, this operator conflicts with ostream::<<(const wchar_t*)
#if !defined( OS_IRIX )
//
// Prints the given string.
//
ostream& operator<< (ostream& s,
					 aafCharacter_constptr wstring)
{
  assert (wstring);

  const aafUInt32 numChars = wcslen (wstring);

  char * cstring = new char [numChars+1];
  assert (cstring);

  size_t status = wcstombs(cstring, wstring, numChars);

  assert (status != (size_t)-1);
  cstring[numChars] = '\0';

  s << cstring;

  delete[] cstring;

  return s;
}
#endif

//
// Returns true if the positions match; returns false otherwise.
//
bool ArePositionsEqual (const ePosition & positionA, const ePosition & positionB)
{
  return (memcmp (&positionA, &positionB, sizeof (ePosition)) == 0 ?
		  true : false);
}


//
// Prints the given ePosition.
//
void PrintPosition (IAAFDictionary * pDict,
				const ePosition & position)
{
  assert (pDict);

  IAAFTypeDef *ptd=NULL;
  IAAFTypeDefExtEnum *ptde=NULL;
  aafCharacter *buf = NULL;

  
  try
  {
    check (pDict->LookupTypeDef (kTypeID_ePosition, &ptd));
    check (ptd->QueryInterface (IID_IAAFTypeDefExtEnum, (void **)&ptde));

    aafUInt32 nameLen = 0;
    check (ptde->GetNameBufLenFromAUID (position, &nameLen));
    assert (nameLen);

    buf = new aafCharacter[nameLen];
    assert (buf);

    check (ptde->GetNameFromAUID (position, buf, nameLen));

    cout << buf;

    delete[] buf;
    buf = NULL;
    ptde->Release();
    ptde=NULL;
    ptd->Release();
    ptd=NULL;
  }
  catch (...)
  {
    // cleanup after error...
    if (buf)
      delete [] buf;
    if (ptde)
      ptde->Release();
    if (ptd)
      ptd->Release();

    throw;
  }
}


void AdminMobAppendResource (IAAFDictionary *pDict,
									IAAFMob *pAdminMob,
							        IAAFObject *pResource)
{
  assert (pAdminMob);
  assert (pResource);

  IAAFTypeDef *td=NULL;
  IAAFTypeDefObjectRef *tdo=NULL;
  IAAFPropertyValue *pv=NULL;
  IAAFObject *pMobObj=NULL;
  IAAFClassDef *cd=NULL;
  IAAFPropertyDef *pd=NULL;
  IAAFPropertyValue *pva=NULL;
  IAAFTypeDef *tdpv=NULL;
  IAAFTypeDefVariableArray *tda=NULL;


  try
  {
    // Create a property value corresponding to new object reference.
    // We'll have to do that through the type def for that kind of obj
    // ref.

    // Look up the type def for StrongReference-to-PersonnelResource
    // objects.
    check (pDict->LookupTypeDef (kTypeID_PersonnelResourceStrongReference,
							     &td));
  
    // Create the StrongReference-to-PersonnelResource property value
    // through the type def.  This must be done through the
    // IAAFTypeDefObjectRef interface, so QI for that first.
    check (td->QueryInterface (IID_IAAFTypeDefObjectRef,
							   (void **)&tdo));

    // Create the value.  The CreateValue() method takes as argument the
    // object to which this value will refer.
    check (tdo->CreateValue (pResource, &pv));

    // Append the new property value (which refers to the new
    // PersonnelRecord object) to the existing array of PersonnelRecord
    // object references.

    // First get the existing array-of-objects property value from the
    // AdminMob object.  To do so, we'll need:
    // - The IAAFObject interface from the AdminMob object.
    // - Using the IAAFObject interface, we obtain the ClassDefinition
    //   describing this AdminMob object.
    // - From that class definition, we look up the PropertyDef for the
    //   Personnel property on AdminMob.
    // - We use PropertyDef to specify which property value we're
    //   getting, and get the property.

    // QI for the AdminMob's IAAFObject interface
    check (pAdminMob->QueryInterface (IID_IAAFObject,
							     (void **)&pMobObj));

    // Use the IAAFObject interface to obtain the class definition
    // describing the AdminMob.
    check (pMobObj->GetDefinition (&cd));

    // Use the ClassDefinition to look up the PropertyDefinition for the
    // Personnel property on the Personnel mob.
    check (cd->LookupPropertyDef (kPropID_AdminMob_Personnel,
								  &pd));

    // Use the PropertyDefinition to specify the property whose value we
    // wish to obtain.
    check (pMobObj->GetPropertyValue (pd, &pva));

    // Now get a type def for the array of personnel, and use that type
    // def to append the new personnel prop val to it.
    check (pva->GetType (&tdpv));
    check (tdpv->QueryInterface (IID_IAAFTypeDefVariableArray,
							     (void **)&tda));
    check (tda->AppendElement (pva, pv));

    // Now that the property value has had the new element appended to
    // it, set the property in the AdminMob to its new value.
    check (pMobObj->SetPropertyValue (pd, pva));

    tda->Release();
    tda=NULL;
    tdpv->Release();
    tdpv=NULL;
    pva->Release();
    pva=NULL;
    pd->Release();
    pd=NULL;
    cd->Release();
    cd=NULL;
    pMobObj->Release();
    pMobObj=NULL;
    pv->Release();
    pv=NULL;
    tdo->Release();
    tdo=NULL;
    td->Release();
    td=NULL;
  }
  catch (...)
  {
    // cleanup after error...
    if (tda)
      tda->Release();
    if (tdpv)
      tdpv->Release();
    if (pva)
      pva->Release();
    if (pd)
      pd->Release();
    if (cd)
      cd->Release();
    if (pMobObj)
      pMobObj->Release();
    if (pv)
      pv->Release();
    if (tdo)
      tdo->Release();
    if (td)
      td->Release();

    throw;
  }
}


void AdminMobGetNumPersonnel (IAAFDictionary * pDict,
							  IAAFMob * pMob,
							  aafUInt32 *numPersonnel)
{
  IAAFObject *pMobObj=NULL;
  IAAFClassDef *cd=NULL;
  IAAFPropertyDef *pd=NULL;
  IAAFPropertyValue *pva=NULL;
  IAAFTypeDef *td=NULL;
  IAAFTypeDefVariableArray *tda=NULL;


  try
  {
    // Get an IAAFObject for the mob, then use it to get the class
    // definition describing the AdminMob
    check (pMob->QueryInterface (IID_IAAFObject, (void **)&pMobObj));
    check (pMobObj->GetDefinition (&cd));

    // Use the class definition to get the definition for the Personnel
    // property.
    check (cd->LookupPropertyDef (kPropID_AdminMob_Personnel,
								  &pd));

    // Get the property value for the array of personnel objects
    check (pMobObj->GetPropertyValue (pd, &pva));

    // Get the type def for arrays
    check (pva->GetType (&td));
    check (td->QueryInterface (IID_IAAFTypeDefVariableArray, (void **)&tda));

    // Use the type def for arrays to get the number of personnel
    // objects in this property value.
    check (tda->GetCount (pva, numPersonnel));

    tda->Release();
    tda=NULL;
    td->Release();
    td=NULL;
    pva->Release();
    pva=NULL;
    pd->Release();
    pd=NULL;
    cd->Release();
    cd=NULL;
    pMobObj->Release();
    pMobObj=NULL;
  }
  catch (...)
  {
    // cleanup after error...
    if (tda)
      tda->Release();
    if (td)
      td->Release();
    if (pva)
      pva->Release();
    if (pd)
      pd->Release();
    if (cd)
      cd->Release();
    if (pMobObj)
      pMobObj->Release();

    throw;
  }
}

void AdminMobGetNthPersonnel (IAAFDictionary * pDict,
							  IAAFMob * pMob,
							  aafUInt32 index,
							  IAAFObject **personnelResource)
{
  IAAFObject *pMobObj=NULL;
  IAAFClassDef *cd=NULL;
  IAAFPropertyDef *pd=NULL;
  IAAFPropertyValue *pva=NULL;
  IAAFTypeDef *td=NULL;
  IAAFTypeDefVariableArray *tda=NULL;
  IAAFTypeDef *basetd=NULL;
  IAAFTypeDefObjectRef *tdo=NULL;
  IAAFPropertyValue *pvr=NULL;


  try
  {
    // Get an IAAFObject for the mob, then use it to get the class
    // definition describing the AdminMob
    check (pMob->QueryInterface (IID_IAAFObject, (void **)&pMobObj));
    check (pMobObj->GetDefinition (&cd));

    // Use the class definition to get the definition for the Personnel
    // property.
    check (cd->LookupPropertyDef (kPropID_AdminMob_Personnel,
								  &pd));

    // Get the property value for the array of personnel objects
    check (pMobObj->GetPropertyValue (pd, &pva));

    // Get the type def for arrays
    check (pva->GetType (&td));
    check (td->QueryInterface (IID_IAAFTypeDefVariableArray, (void **)&tda));
    // Get the type def describing elements of the array
    check (tda->GetType (&basetd));
    check (basetd->QueryInterface (IID_IAAFTypeDefObjectRef, (void **)&tdo));

    // Get the property value for the indexed record
    check (tda->GetElementValue (pva, index, &pvr));

    // Get the object contained in that property value
    check (tdo->GetObject (pvr, IID_IAAFObject, (IUnknown **)personnelResource));

    pvr->Release();
    pvr=NULL;
    tdo->Release();
    tdo=NULL;
    basetd->Release();
    basetd=NULL;
    tda->Release();
    tda=NULL;
    td->Release();
    td=NULL;
    pva->Release();
    pva=NULL;
    pd->Release();
    pd=NULL;
    cd->Release();
    cd=NULL;
    pMobObj->Release();
    pMobObj=NULL;
  }
  catch (...)
  {
    // cleanup after error...
    if (pvr)
      pvr->Release();
    if (tdo)
      tdo->Release();
    if (basetd)
      basetd->Release();
    if (tda)
      tda->Release();
    if (td)
      td->Release();
    if (pva)
      pva->Release();
    if (pd)
      pd->Release();
    if (cd)
      cd->Release();
    if (pMobObj)
      pMobObj->Release();

    throw;
  }
}


//
// Creates a type definition to describe ePosition enumerations, and
// registers it in the dictionary.
//
void CreateAndRegisterPositionEnum (IAAFDictionary * pDict)
{
  assert (pDict);

  IAAFTypeDef *ptd = NULL;
  IAAFTypeDefExtEnum *ptde = NULL;

  CAAFBuiltinDefs defs (pDict);

  // Check to see if we are have already been registered.
  if (SUCCEEDED(pDict->LookupTypeDef (kTypeID_ePosition, &ptd)))
  {
    ptd->Release();
    ptd = NULL;
    return;
  }

  try
  {
    // Instantiate a type definition object which will describe ePosition
    // extensible enumerations.
    check (pDict->CreateMetaInstance (AUID_AAFTypeDefExtEnum, 
                                      IID_IAAFTypeDefExtEnum,
                                      (IUnknown**) &ptde));

    // Initialize the type definition object with the given name, and to
    // be represented by the given AUID.  We've already generated an
    // auid (kTypeID_ePosition) to represent this type defintion.
    check (ptde->Initialize (kTypeID_ePosition,
						     L"PersonnelPosition"));

    // Pre-register a few element values, along with their names.  We've
    // already generated AUIDs to represent the values (kPosition_XXX).
    check (ptde->AppendElement (kPosition_Producer,     L"Producer"));
    check (ptde->AppendElement (kPosition_Editor,       L"Editor"));
    check (ptde->AppendElement (kPosition_FloorManager, L"FloorManager"));
    check (ptde->AppendElement (kPosition_Actor, L"Actor"));

    // Register this type definition in the dictionary.  The
    // dictionary::RegisterTypeDef() method expects an IAAFTypeDef
    // pointer, so we'll QI for that first.
    check (ptde->QueryInterface (IID_IAAFTypeDef, (void **)&ptd));
    check (pDict->RegisterTypeDef (ptd));
    ptd->Release();
    ptd=NULL;
    ptde->Release();
    ptde=NULL;
  }
  catch (...)
  {
    // cleanup after error...
    if (ptd)
      ptd->Release();
    if (ptde)
      ptde->Release();

    throw;
  }
}


//
// Creates a class definition to describe PersonnelResource objects,
// and registers it in the dictionary.
//
void CreateAndRegisterPersonnelResource (IAAFDictionary * pDict)
{
  assert (pDict);

  IAAFClassDef *pcd=NULL;
  IAAFClassDef *pcd_Object=NULL;
  IAAFPropertyDef *pd_unused=NULL;
  IAAFTypeDef *ptd_String=NULL;
  IAAFTypeDef *ptd_Position=NULL;
  IAAFTypeDef *ptd_ui32=NULL;

  CAAFBuiltinDefs defs (pDict);

  // Check to see if we are have already been registered.
  if (SUCCEEDED(pDict->LookupClassDef (kClassID_PersonnelResource,
									   &pcd)))
  {
    pcd->Release();
    pcd = NULL;
    return;
  }

  try
  {
    // Instantiate a class definition object which will describe
    // PersonnelResource objects.
    check (pDict->CreateMetaInstance (AUID_AAFClassDef,
                                      IID_IAAFClassDef,
                                      (IUnknown**) &pcd));

    // We'll have to specify the PersonnelResource class definition's
    // parent class, so look it up here.  Since PersonnelResource
    // inherits directly from AAFObject, look up the AAFObject class
    // definition.
    check (pDict->LookupClassDef (AUID_AAFObject, &pcd_Object));

    // Now initialize our new PersonnelResource class definition.  We'll
    // specify an AUID with which it will be identified
    // (kClassID_PersonnelResource, which we've already generated); the
    // parent class (specified by the class definition for AAFObject,
    // which we've just looked up), and a name for this class.
    check (pcd->Initialize (kClassID_PersonnelResource,
						    pcd_Object,
						    L"PersonnelResource",
							kAAFTrue));

    //
    // Now we add definitions for the properties that this class will
    // contain.  Note that each property is described by a type, which
    // must be specified at the time the property def is appended.  Note
    // also that we'll be specifying some mandatory properties; it is
    // required that these be specified *before* the class is registered
    // (using the AppendNewPropertyDef() method).
    //

    // Appending property defs returns the newly-created property
    // definition; declare a variable where the Append method can put
    // the new prop def.  However we won't use the prop def in this
    // function.

    // Add the FamilyName and GivenName properties to the PersonnelRecord class defintion.
    // The type needed to define these is String, so we'll need to specify
    // the type definition describing String when we append the prop
    // def.  First, look up the type definition describing String.
    check (pDict->LookupTypeDef (kAAFTypeID_String,
							     &ptd_String));

    // Now request that a new property def be appended to this class
    // def.  We're specifying an AUID by which this property will be
    // known (kPropID_PersonnelResource_FamilyName, which we've already
    // generated), a name, the type of the property, and that the
    // property is mandatory.  The returned property def is ignored
    // since we won't use it in this function.
    check (pcd->RegisterNewPropertyDef (kPropID_PersonnelResource_GivenName,
									   L"GivenName",
									   ptd_String,
									   kAAFFalse,	// mandatory
									   kAAFFalse,	// not a unique identifier
									   &pd_unused));
    pd_unused->Release();
    pd_unused=NULL;
    check (pcd->RegisterNewPropertyDef (kPropID_PersonnelResource_FamilyName,
									    L"FamilyName",
									    ptd_String,
									    kAAFFalse,	// mandatory
									    kAAFFalse,	// not a unique identifier
									    &pd_unused));
    pd_unused->Release();
    pd_unused=NULL;

    // Add the Position property to the PersonnelRecord class defintion.
    // The type of the Position property is ePosition (an extensible enumeration
    // we've just registered), so we'll need to specify the type
    // definition describing ePosition when we append the prop def.  First,
    // look up the type definition describing ePosition.

    // Now request that a new property def be appended to this class
    // def.  We're specifying an AUID by which this property will be
    // known (kPropID_PersonnelResource_Position, which we've already
    // generated), a name, the type of the property, and that the
    // property is mandatory.  The returned property def is ignored
    // since we won't use it in this function.
    check (pDict->LookupTypeDef (kTypeID_ePosition, &ptd_Position));
    check (pcd->RegisterNewPropertyDef (kPropID_PersonnelResource_Position,
									    L"Position",
									    ptd_Position,
									    kAAFFalse,   // mandatory
									    kAAFFalse,	// not a unique identifier
									    &pd_unused));
    pd_unused->Release();
    pd_unused=NULL;

    // Add the Contract ID property to the PersonnelRecord class
    // defintion.  The type of the Contract ID property is UInt32, so
    // we'll need to specify the type definition describing UInt32 when
    // we append the prop def.  First, look up the type definition
    // describing UInt32.
    check (pDict->LookupTypeDef (kAAFTypeID_UInt32, &ptd_ui32));

    // Now request that a new property def be appended to this class
    // def.  We're specifying an AUID by which this property will be
    // known (kPropID_PersonnelResource_ContractID, which we've already
    // generated), a name, the type of the property, and that the
    // property is optional.  The returned property def is ignored
    // since we won't use it in this function.
    check (pcd->RegisterNewPropertyDef (kPropID_PersonnelResource_ContractID,
									    L"ContractID",
									    ptd_ui32,
									    kAAFTrue,    // optional
									    kAAFFalse,	// not a unique identifier
									    &pd_unused));
    pd_unused->Release();
    pd_unused=NULL;

    // Now request that a new property def be appended to this class
    // def.  We're specifying an AUID by which this property will be
    // known (kPropID_PersonnelResource_Role, which we've already
    // generated), a name, the type of the property, and that the
    // property is optional.  The returned property def is ignored
    // since we won't use it in this function.
    check (pcd->RegisterNewPropertyDef (kPropID_PersonnelResource_Role,
									    L"Role",
									    ptd_String,
									    kAAFTrue,    // optional
									    kAAFFalse,	// not a unique identifier
									    &pd_unused));
    pd_unused->Release();
    pd_unused=NULL;

    // Now that the class definition describing PersonnelResource is
    // initialized and its properties have been specified, we can
    // register that class definition.
    check (pDict->RegisterClassDef (pcd));

    ptd_ui32->Release();
    ptd_ui32=NULL;
    ptd_Position->Release();
    ptd_Position=NULL;
    ptd_String->Release();
    ptd_String=NULL;
    pcd_Object->Release();
    pcd_Object=NULL;
    pcd->Release();
    pcd=NULL;
  }
  catch (...)
  {
    // cleanup after error...
    if (pd_unused)
      pd_unused->Release();
    if (ptd_ui32)
      ptd_ui32->Release();
    if (ptd_Position)
      ptd_Position->Release();
    if (ptd_String)
      ptd_String->Release();
    if (pcd_Object)
      pcd_Object->Release();
    if (pcd)
      pcd->Release();

    throw;
  }

}


//
// Create and register a type definition describing a reference to a
// PersonnelResource object.
//
void
CreateAndRegisterPersonnelResourceReference
(IAAFDictionary * pDict)
{
  assert (pDict);

  IAAFTypeDef *ptd=NULL;
  IAAFClassDef *pcd=NULL;
  IAAFTypeDefStrongObjRef *ptdsr=NULL;

  CAAFBuiltinDefs defs (pDict);

  // Check to see if we are have already been registered.
  if (SUCCEEDED(pDict->LookupTypeDef
				(kTypeID_PersonnelResourceStrongReference, &ptd)))
  {
    ptd->Release();
    return;
  }

  try
  {
    // When we create the type definition, we'll need to describe which
    // objects it is permissible to reference using this type.  In this
    // case, we want to specify that we're referencing objects of class
    // PersonnelResource.  Therefore we'll have to look up the class
    // describing PersonnelResource objects for use below.
    check (pDict->LookupClassDef (kClassID_PersonnelResource,
								  &pcd));

    // Instantiate a TypeDefinition for use as a Reference to a
    // PersonnelResource object.  We'll instantiate a
    // TypeDefinitionStrongObjectReference.
    check (pDict->CreateMetaInstance (AUID_AAFTypeDefStrongObjRef,
                                      IID_IAAFTypeDefStrongObjRef,
                                      (IUnknown**) &ptdsr));

    // Initialize our new type def, identifying the given AUID by which
    // this type will be known
    // (kTypeID_PersonnelResourceStrongReference, which we've already
    // generated), the class definition describing the types of objects
    // to which we'll refer (the ClassDef descriging PersonnelResource
    // objects), and a name for this type.
    check (ptdsr->Initialize (kTypeID_PersonnelResourceStrongReference,
							  pcd,
							  L"PersonnelResourceStrongReference"));

    // Register this type definition in the dictionary.  The
    // dictionary::RegisterTypeDef() method expects an IAAFTypeDef
    // pointer, so we'll QI for that first.
    check (ptdsr->QueryInterface (IID_IAAFTypeDef, (void **)&ptd));
    check (pDict->RegisterTypeDef (ptd));

    ptdsr->Release();
    ptdsr=NULL;
    ptd->Release();
    ptd=NULL;
    pcd->Release();
    pcd=NULL;
  }
  catch (...)
  {
    // cleanup after error...
    if (ptdsr)
      ptdsr->Release();
    if (ptd)
      ptd->Release();
    if (pcd)
      pcd->Release();

    throw;
  }
}


//
// Create and register a type definition describing a vector of
// references to PersonnelResource objects.
//
void
CreateAndRegisterPersonnelResourceReferenceVector
(IAAFDictionary * pDict)
{
  assert (pDict);

  IAAFTypeDef *ptd=NULL;
  IAAFTypeDefVariableArray *ptdv=NULL;
  IAAFTypeDef *ptdr=NULL;

  CAAFBuiltinDefs defs (pDict);
  
  // Check to see if we are have already been registered.
  if (SUCCEEDED(pDict->LookupTypeDef
				(kTypeID_PersonnelResourceStrongReferenceVector,
				 &ptd)))
  {
    ptd->Release();
    ptd = NULL;
    return;
  }

  try
  {
    // Instantiate a TypeDefinition for use as a Vector of References to
    // PersonnelResource objects.  We'll instantiate a
    // TypeDefinitionVariableArray.
    check (pDict->CreateMetaInstance (AUID_AAFTypeDefVariableArray,
                                      IID_IAAFTypeDefVariableArray, 
                                      (IUnknown**) &ptdv));

    // We'll need to describe the type of each element in this vector.
    // In this case, we want to specify that elements are
    // StrongReferenceToPersonnelRecord objects.  Therefore we'll have
    // to look up the type describing StrongReferenceToPersonnelRecord
    // for use below.
    check (pDict->LookupTypeDef (kTypeID_PersonnelResourceStrongReference,
							     &ptdr));

    // Initialize our new type def, identifying the given AUID by which
    // this type will be known
    // (kTypeID_PersonnelResourceStrongReferenceVector, which we've
    // already generated), the type definition describing the type of
    // elements in this vector (the TypeDef descriging
    // StrongReferenceToPersonnelResource objects), and a name for this
    // type.
    check (ptdv->Initialize (kTypeID_PersonnelResourceStrongReferenceVector,
						     ptdr,
						     L"PersonnelResourceStrongReferenceVector"));

    // Register this type definition in the dictionary.  The
    // dictionary::RegisterTypeDef() method expects an IAAFTypeDef
    // pointer, so we'll QI for that first.
    check (ptdv->QueryInterface (IID_IAAFTypeDef, (void **)&ptd));
    check (pDict->RegisterTypeDef (ptd));

    ptd->Release();
    ptd=NULL;
    ptdr->Release();
    ptdr=NULL;
    ptdv->Release();
    ptdv=NULL;
  }
  catch (...)
  {
    // cleanup after error...
    if (ptd)
      ptd->Release();
    if (ptdr)
      ptdr->Release();
    if (ptdv)
      ptdv->Release();

    throw;
  }

}


//
// Creates a class definition to describe AdminMob objects, and
// registers it in the dictionary.
//
void CreateAndRegisterAdminMob (IAAFDictionary * pDict)
{
  assert (pDict);

  IAAFClassDef *pcd=NULL;
  IAAFClassDef *pcd_Mob=NULL;
  IAAFPropertyDef *pd_unused=NULL;
  IAAFTypeDef *ptd_PersonnelVector=NULL;

  CAAFBuiltinDefs defs (pDict);

  // Check to see if we are have already been registered.
  if (SUCCEEDED(pDict->LookupClassDef (kClassID_AdminMob, &pcd)))
  {
    pcd->Release();
    pcd = NULL;
    return;
  }

  try
  {
    // Instantiate a class definition object which will describe
    // AdminMob objects.
    check (pDict->CreateMetaInstance (AUID_AAFClassDef,
                                      IID_IAAFClassDef,
                                      (IUnknown**) &pcd));

    // We'll have to specify the AdminMob class definition's parent
    // class, so look it up here.  Since PersonnelResource inherits from
    // AAFMob, look up the AAFMob class definition.
    check (pDict->LookupClassDef (AUID_AAFMob, &pcd_Mob));

    // Now initialize our new AdminMob class definition.  We'll
    // specify an AUID with which it will be identified
    // (kClassID_AdminMob, which we've already generated); the
    // parent class (specified by the class definition for AAFMob, which
    // we've just looked up), and a name for this class.
    check (pcd->Initialize (kClassID_AdminMob,
						    pcd_Mob,
						    L"AdminMob",
							kAAFTrue));

    //
    // Now we add a definition for the property that this class will
    // contain.  Note that each property is described by a type, which
    // must be specified at the time the property def is appended.  Note
    // also that this will be a mandatory property; it is required that
    // this be specified *before* the class is registered (using the
    // RegisterNewPropertyDef() method).
    //

    // Appending property defs returns the newly-created property
    // definition; declare a variable where the Append method can put
    // the new prop def.  However we won't use the prop def in this
    // function.

    // Add the Personnel property to the AdminMob class defintion.
    // The type of the Personnel property is
    // Vector-of-strong-reference-to-PesonnelRecord, so we'll need to
    // specify the type definition describing that kind of vector when
    // we append the prop def.  First, look up that type definition.
    check (pDict->LookupTypeDef (kTypeID_PersonnelResourceStrongReferenceVector,
							     &ptd_PersonnelVector));

    // Now request that a new property def be appended to this class
    // def.  We're specifying an AUID by which this property will be
    // known (kPropID_AdminMob_Personnel, which we've already
    // generated), a name, the type of the property, and that the
    // property is mandatory.  The returned property def is ignored
    // since we won't use it in this function.
    check (pcd->RegisterNewPropertyDef (kPropID_AdminMob_Personnel,
									    L"Personnel",
									    ptd_PersonnelVector,
									    kAAFFalse,
									    kAAFFalse,	// not a unique identifier
									    &pd_unused));
    pd_unused->Release();
    pd_unused=NULL;

    // Now that the class definition describing AdminMob is
    // initialized and its properties have been specified, we can
    // register that class definition.
    check (pDict->RegisterClassDef (pcd));

    ptd_PersonnelVector->Release();
    ptd_PersonnelVector=NULL;
    pcd_Mob->Release();
    pcd_Mob=NULL;
    pcd->Release();
    pcd=NULL;
  }
  catch (...)
  {
    // cleanup after error...
    if (pd_unused)
      pd_unused->Release();
    if (ptd_PersonnelVector)
      ptd_PersonnelVector->Release();
    if (pcd_Mob)
      pcd_Mob->Release();
    if (pcd)
      pcd->Release();

    throw;
  }

}


//
// Creates a PersonnelResource object through the given dictionary,
// and initializes it with the given info.  It is then appended to the
// given AdminMob, in its Personnel property (which is an array of
// object references to PersonnelResource objects).
//
// These are the steps:
// - Create the PersonnelResource object.
// - Set the values in the newly-created PersonnelResource object.
// - Create a object reference PropertyValue to refer to the new
//   PersonnelResource object.
// - Get the PropertyValue for the current value of the Personnel
//   array from the AdminMob.
// - Append the new PropertyValue (which references the new
//   PersonnelResource object) to the existing PropertyValue of the
//   Personnel vector from the AdminMob.
// - Set the value of the Personnel resource in the AdminMob to
//   the newly-appended PropertyValue.
//

//
// Creates a PersonnelResource object through the given dictionary,
// and initializes it with the given info.  It is then appended to the
// given AdminMob, in its Personnel property (which is an array of
// object references to PersonnelResource objects).
//
// These are the steps:
// - Create the PersonnelResource object.
// - Set the values in the newly-created PersonnelResource object.
// - Create a object reference PropertyValue to refer to the new
//   PersonnelResource object.
// - Get the PropertyValue for the current value of the Personnel
//   array from the AdminMob.
// - Append the new PropertyValue (which references the new
//   PersonnelResource object) to the existing PropertyValue of the
//   Personnel vector from the AdminMob.
// - Set the value of the Personnel resource in the AdminMob to
//   the newly-appended PropertyValue.
//

void DefineResourceClassExtensions(IAAFDictionary *pDict)
{
  // Need to check if already defined.
  assert(pDict);
  // Register the extensible enumeration describing Position in the
  // dictionary.
  CreateAndRegisterPositionEnum (pDict);

  // Create a class definition describing PesonnelResource objects and
  // register it in the dictionary.
  CreateAndRegisterPersonnelResource (pDict);

  // Create a type definition describing references to
  // PersonnelResource objects, and register it.
  CreateAndRegisterPersonnelResourceReference (pDict);

  // Create a type definition describing vectors of references to
  // PersonnelResource objects, and register it.
  CreateAndRegisterPersonnelResourceReferenceVector (pDict);

  // Create a class definition describing AdminMob objects, and
  // register it.
  CreateAndRegisterAdminMob (pDict);


}


//
// Verifies that all class and type defintions have been correctly
// registered to describe AdminMob and PersonnelResource objects.
//
void VerifyResourceClassExtensions(IAAFDictionary *pDict)
{
  IAAFTypeDef *ptd=NULL;
  IAAFClassDef *pcd=NULL;

  try
  {
    cout << "Verifying position enum type has been registered." << endl;
    check (pDict->LookupTypeDef (kTypeID_ePosition, &ptd));
    ptd->Release();
    ptd=NULL;

    cout << "Verifying PersonnelResource class has been registered." << endl;
    check (pDict->LookupClassDef (kClassID_PersonnelResource, &pcd));
    pcd->Release();
    pcd=NULL;

    cout << "Verifying AdminMob class has been registered." << endl;
    check (pDict->LookupClassDef (kClassID_AdminMob, &pcd));
    pcd->Release();
    pcd=NULL;

    cout << "Verifying PersonnelResourceReference type has been"
	     << " registered." << endl;
    check (pDict->LookupTypeDef (kTypeID_PersonnelResourceStrongReference,
							  &ptd));
    ptd->Release();
    ptd=NULL;

    cout << "Verifying PersonnelResourceReferenceVector type has been"
	     << " registered." << endl; 
    check (pDict->LookupTypeDef (kTypeID_PersonnelResourceStrongReferenceVector,
							  &ptd));
    ptd->Release();
    ptd=NULL;
  }
  catch (...)
  {
    // Cleanup
    if (pcd)
      pcd->Release();
    if (ptd)
      ptd->Release();

    throw;
  }
}

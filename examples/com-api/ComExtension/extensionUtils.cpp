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

#include "extensionUtils.h"
#include "AAF.h"
#include <assert.h>
#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"

#if defined(_MAC) || defined(macintosh)
#include <wstring.h>
#endif

#include <iostream.h>

// utility function to set String property value
static void SetStringProperty (IAAFObject * pObj,
						const aafUID_t propertyID,
						const aafCharacter * strValue)
{
  assert (pObj);
  assert (strValue);

  IAAFClassDef *cd=NULL;
  check (pObj->GetDefinition (&cd));

  IAAFPropertyDef *pd=NULL;
  check (cd->LookupPropertyDef (propertyID,
								&pd));

  IAAFTypeDef *td=NULL;
  check (pd->GetTypeDef (&td));

  IAAFTypeDefString *tds=NULL;
  check (td->QueryInterface (IID_IAAFTypeDefString, (void **)&tds));

  IAAFPropertyValue *pv=NULL;
  check (tds->CreateValueFromCString ( (aafMemPtr_t) strValue,
										 wcslen (strValue) * sizeof (aafCharacter), 
										 &pv));

  check (pObj->SetPropertyValue (pd, pv));
  cd->Release();
  cd=NULL;
  pd->Release();
  pd=NULL;
  pv->Release();
  pv=NULL;
  td->Release();
  td=NULL;
  tds->Release();
  tds=NULL;
}

static void GetStringBufLen (IAAFObject * pObj,
							 const aafUID_t propertyID,
							 aafUInt32 *bufLen)
{
  assert (pObj);
  assert (bufLen);
		
  IAAFClassDef *cd=NULL;
  check (pObj->GetDefinition (&cd));

  IAAFPropertyDef *pd=NULL;
  check (cd->LookupPropertyDef (propertyID,
								&pd));

  IAAFPropertyValue *pv=NULL;
  check (pObj->GetPropertyValue (pd, &pv));
  
  IAAFTypeDef *td=NULL;
  check (pd->GetTypeDef (&td));

  IAAFTypeDefString *tds=NULL;
  check (td->QueryInterface (IID_IAAFTypeDefString, (void **)&tds));

  aafUInt32 numChars = 0;
  check (tds->GetCount (pv, &numChars));


  // Add 1 for null terminator and 
  // convert from count of aafCharacter to bytes
//  *bufLen= (numChars + 1)*sizeof (aafCharacter);
  *bufLen = (numChars +1)*2;

  cd->Release();
  cd=NULL;
  pd->Release();
  pd=NULL;
  pv->Release();
  pv=NULL;
  td->Release();
  td=NULL;
  tds->Release();
  tds=NULL;
}

void GetStringValue (IAAFObject * pObj,
							 const aafUID_t propertyID,
							 aafCharacter * buf,
							 aafUInt32 buflen)
{
  assert (pObj);
  assert (buf);
  aafUInt32 stringBufLen,numCharsWNull;
  GetStringBufLen (pObj, propertyID,
					&stringBufLen);
  assert (buflen >= stringBufLen);
  numCharsWNull = stringBufLen/sizeof (aafCharacter);

  IAAFClassDef *cd=NULL;
  check (pObj->GetDefinition (&cd));

  IAAFPropertyDef *pd=NULL;
  check (cd->LookupPropertyDef (propertyID,
								&pd));

  IAAFPropertyValue *pv=NULL;
  check (pObj->GetPropertyValue (pd, &pv));
  
  IAAFTypeDef *td=NULL;
  check (pd->GetTypeDef (&td));

  IAAFTypeDefString *tds=NULL;
  check (td->QueryInterface (IID_IAAFTypeDefString, (void **)&tds));

  check (tds->GetElements (pv, (aafMemPtr_t) buf, buflen));

  // Make sure we're null-terminated.
  buf[numCharsWNull-1] = '\0';
  cd->Release();
  cd=NULL;
  pd->Release();
  pd=NULL;
  pv->Release();
  pv=NULL;
  td->Release();
  td=NULL;
  tds->Release();
  tds=NULL;
}


// Checks if pClassDefQuery is or is a subclass of class defined for the specified AUID
bool classDefinitionIsA ( IAAFClassDef *pClassDefQuery,
							   const aafUID_t targetAUID)
{
	IAAFDefObject *pDefObject;
	check(pClassDefQuery->QueryInterface(IID_IAAFDefObject,(void **)&pDefObject));
	aafUID_t testAUID;
	check(pDefObject->GetAUID(&testAUID));
	pDefObject->Release();
	pDefObject=NULL;
    if (!memcmp (&testAUID, &targetAUID, sizeof (aafUID_t)))
	{
		// AUIDs match
		return true;
	} else if (!memcmp(&testAUID, (aafUID_t *) &AUID_AAFInterchangeObject, sizeof(aafUID_t)))
	{
		return false;
	}

	// Get parent class
	IAAFClassDef *pParentClass=NULL;
	check(pClassDefQuery->GetParent(&pParentClass));
	bool classMatch=false;
	classMatch= classDefinitionIsA( pParentClass, targetAUID);
	pParentClass->Release();
	pParentClass=NULL;
	return classMatch;
}

bool IsOptionalPropertyPresent (IAAFObject * pObj,
								const aafUID_t propertyID)
{
  aafBool r;
  assert (pObj);

  IAAFClassDef *cd=NULL;
  check (pObj->GetDefinition (&cd));

  IAAFPropertyDef *pd=NULL;
  check (cd->LookupPropertyDef (propertyID,
								&pd));

  check (pObj->IsPropertyPresent (pd, &r));
  cd->Release();
  cd=NULL;
  pd->Release();
  pd=NULL;
  return r ? true : false;
}

// Main functions for PersonnelResource
void PersonnelResourceSetFamilyName(IAAFObject *pResource,
							   aafCharacter *familyName)
{
	assert (pResource);
	assert (familyName);
    SetStringProperty (pResource, kPropID_PersonnelResource_FamilyName,
						familyName);
}

void PersonnelResourceSetGivenName(IAAFObject *pResource,
							   aafCharacter *givenName)
{
	assert (pResource);
	assert(givenName);
    SetStringProperty (pResource, kPropID_PersonnelResource_GivenName,
						givenName);
}


void PersonnelResourceSetPosition (IAAFObject * pResource,
							 ePosition position)
{
  assert (pResource);

  IAAFClassDef *cd=NULL;
  check (pResource->GetDefinition (&cd));

  IAAFPropertyDef *pd=NULL;
  check (cd->LookupPropertyDef (kPropID_PersonnelResource_Position,
								&pd));

  IAAFPropertyValue *pv=NULL;
  check (pResource->GetPropertyValue (pd, &pv));
  
  IAAFTypeDef *td=NULL;
  check (pd->GetTypeDef (&td));

  IAAFTypeDefExtEnum *tde=NULL;
  check (td->QueryInterface (IID_IAAFTypeDefExtEnum, (void **)&tde));

  check (tde->SetAUIDValue (pv, position));
  check (pResource->SetPropertyValue (pd, pv));
  cd->Release();
  cd=NULL;
  pd->Release();
  pd=NULL;
  pv->Release();
  pv=NULL;
  td->Release();
  td=NULL;
  tde->Release();
  tde=NULL;
}
void PersonnelResourceSetActorRole (IAAFObject *pResource,
								  aafCharacter *role)
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
  check (pObj->GetDefinition (&cd));

  IAAFPropertyDef *pd=NULL;
  check (cd->LookupPropertyDef (kPropID_PersonnelResource_ContractID,
								&pd));

  IAAFTypeDef *td=NULL;
  check (pd->GetTypeDef (&td));

  IAAFTypeDefInt* tdi=NULL;
  check (td->QueryInterface (IID_IAAFTypeDefInt, (void **)&tdi));

  IAAFPropertyValue *pv=NULL;
  check (tdi->CreateValue ((aafMemPtr_t) &cid, sizeof (cid), &pv));

  check (pObj->SetPropertyValue (pd, pv));
  cd->Release();
  cd=NULL;
  pd->Release();
  pd=NULL;
  td->Release();
  td=NULL;
  tdi->Release();
  tdi=NULL;
  pv->Release();
  pv=NULL;
}

void PersonnelResourceInitialize(IAAFObject *pResource,
							   aafCharacter *familyName,
							   aafCharacter *givenName,
							   ePosition position)
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
  check (pResource->GetDefinition (&cd));

  IAAFPropertyDef *pd=NULL;
  check (cd->LookupPropertyDef (kPropID_PersonnelResource_Position,
								&pd));

  IAAFPropertyValue *pv=NULL;
  check (pResource->GetPropertyValue (pd, &pv));
  
  IAAFTypeDef *td=NULL;
  check (pd->GetTypeDef (&td));

  IAAFTypeDefExtEnum *tde=NULL;
  check (td->QueryInterface (IID_IAAFTypeDefExtEnum, (void **)&tde));

  check (tde->GetAUIDValue (pv, position));
  cd->Release();
  cd=NULL;
  pd->Release();
  pd=NULL;
  pv->Release();
  pv=NULL;
  td->Release();
  td=NULL;
  tde->Release();
  tde=NULL;
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
    check (pObj->GetDefinition (&cd));

    IAAFPropertyDef *pd=NULL;
    check (cd->LookupPropertyDef (kPropID_PersonnelResource_ContractID,
								&pd));

    IAAFPropertyValue *pv=NULL;
    check (pObj->GetPropertyValue (pd, &pv));
  
    IAAFTypeDef *td=NULL;
    check (pd->GetTypeDef (&td));

    IAAFTypeDefInt *tdi=NULL;
    check (td->QueryInterface (IID_IAAFTypeDefInt, (void **)&tdi));

    check (tdi->GetInteger (pv, (aafMemPtr_t) cid, sizeof (*cid)));
    cd->Release();
    cd=NULL;
    pd->Release();
    pd=NULL;
    pv->Release();
    pv=NULL;
    td->Release();
    td=NULL;
    tdi->Release();
    tdi=NULL;
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



//
// Prints the given string.
//
ostream& operator<< (ostream& s,
					 const aafCharacter * wstring)
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
				const ePosition position)
{
  assert (pDict);
  IAAFTypeDef *ptd=NULL;
  check (pDict->LookupType (kTypeID_ePosition, &ptd));

  IAAFTypeDefExtEnum *ptde=NULL;
  check (ptd->QueryInterface (IID_IAAFTypeDefExtEnum, (void **)&ptde));

  aafUInt32 nameLen = 0;
  check (ptde->GetNameBufLenFromAUID (position, &nameLen));
  assert (nameLen);

  aafCharacter *buf = 0;
  buf = new aafCharacter[nameLen];
  assert (buf);

  check (ptde->GetNameFromAUID (position, buf, nameLen));

  cout << buf;

  delete[] buf;
  ptd->Release();
  ptd=NULL;
  ptde->Release();
  ptde=NULL;
}


void AdminMobAppendResource (IAAFDictionary *pDict,
									IAAFMob *pAdminMob,
							        IAAFObject *pResource)
{
  assert (pAdminMob);
  assert (pResource);



  // Create a property value corresponding to new object reference.
  // We'll have to do that through the type def for that kind of obj
  // ref.

  // Look up the type def for StrongReference-to-PersonnelResource
  // objects.
  IAAFTypeDef *td=NULL;
  check (pDict->LookupType (kTypeID_PersonnelResourceStrongReference,
							&td));
  
  // Create the StrongReference-to-PersonnelResource property value
  // through the type def.  This must be done through the
  // IAAFTypeDefObjectRef interface, so QI for that first.
  IAAFTypeDefObjectRef *tdo=NULL;
  check (td->QueryInterface (IID_IAAFTypeDefObjectRef,
							 (void **)&tdo));

  // Create the value.  The CreateValue() method takes as argument the
  // object to which this value will refer.
  IAAFPropertyValue *pv=NULL;
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
  IAAFObject *pMobObj=NULL;
  check (pAdminMob->QueryInterface (IID_IAAFObject,
							   (void **)&pMobObj));

  // Use the IAAFObject interface to obtain the class definition
  // describing the AdminMob.
  IAAFClassDef *cd=NULL;
  check (pMobObj->GetDefinition (&cd));

  // Use the ClassDefinition to look up the PropertyDefinition for the
  // Personnel property on the Personnel mob.
  IAAFPropertyDef *pd=NULL;
  check (cd->LookupPropertyDef (kPropID_AdminMob_Personnel,
								&pd));

  // Use the PropertyDefinition to specify the property whose value we
  // wish to obtain.
  IAAFPropertyValue *pva=NULL;
  check (pMobObj->GetPropertyValue (pd, &pva));

  // Now get a type def for the array of personnel, and use that type
  // def to append the new personnel prop val to it.
  IAAFTypeDef *tdpv=NULL;
  check (pva->GetType (&tdpv));
  IAAFTypeDefVariableArray *tda=NULL;
  check (tdpv->QueryInterface (IID_IAAFTypeDefVariableArray,
							   (void **)&tda));
  check (tda->AppendElement (pva, pv));

  // Now that the property value has had the new element appended to
  // it, set the property in the AdminMob to its new value.
  check (pMobObj->SetPropertyValue (pd, pva));
  td->Release();
  td=NULL;
  tdo->Release();
  tdo=NULL;
  pv->Release();
  pv=NULL;
  pMobObj->Release();
  pMobObj=NULL;
  cd->Release();
  cd=NULL;
  pd->Release();
  pd=NULL;
  pva->Release();
  pva=NULL;
  tdpv->Release();
  tdpv=NULL;
  tda->Release();
  tda=NULL;

}


void AdminMobGetNumPersonnel (IAAFDictionary * pDict,
							  IAAFMob * pMob,
							  aafUInt32 *numPersonnel)
{
  // Get an IAAFObject for the mob, then use it to get the class
  // definition describing the AdminMob
  IAAFObject *pMobObj=NULL;
  check (pMob->QueryInterface (IID_IAAFObject, (void **)&pMobObj));
  IAAFClassDef *cd=NULL;
  check (pMobObj->GetDefinition (&cd));

  // Use the class definition to get the definition for the Personnel
  // property.
  IAAFPropertyDef *pd=NULL;
  check (cd->LookupPropertyDef (kPropID_AdminMob_Personnel,
								&pd));

  // Get the property value for the array of personnel objects
  IAAFPropertyValue *pva=NULL;
  check (pMobObj->GetPropertyValue (pd, &pva));

  // Get the type def for arrays
  IAAFTypeDef *td=NULL;
  check (pva->GetType (&td));
  IAAFTypeDefVariableArray *tda=NULL;
  check (td->QueryInterface (IID_IAAFTypeDefVariableArray, (void **)&tda));

  // Use the type def for arrays to get the number of personnel
  // objects in this property value.
  check (tda->GetCount (pva, numPersonnel));
  pMobObj->Release();
  pMobObj=NULL;
  cd->Release();
  cd=NULL;
  pd->Release();
  pd=NULL;
  pva->Release();
  pva=NULL;
  td->Release();
  td=NULL;
  tda->Release();
  tda=NULL;
}

void AdminMobGetNthPersonnel (IAAFDictionary * pDict,
							  IAAFMob * pMob,
							  aafUInt32 index,
							  IAAFObject **personnelResource)
{
  // Get an IAAFObject for the mob, then use it to get the class
  // definition describing the AdminMob
  IAAFObject *pMobObj=NULL;
  check (pMob->QueryInterface (IID_IAAFObject, (void **)&pMobObj));
  IAAFClassDef *cd=NULL;
  check (pMobObj->GetDefinition (&cd));

  // Use the class definition to get the definition for the Personnel
  // property.
  IAAFPropertyDef *pd=NULL;
  check (cd->LookupPropertyDef (kPropID_AdminMob_Personnel,
								&pd));

  // Get the property value for the array of personnel objects
  IAAFPropertyValue *pva=NULL;
  check (pMobObj->GetPropertyValue (pd, &pva));

  // Get the type def for arrays
  IAAFTypeDef *td=NULL;
  check (pva->GetType (&td));
  IAAFTypeDefVariableArray *tda=NULL;
  check (td->QueryInterface (IID_IAAFTypeDefVariableArray, (void **)&tda));
  // Get the type def describing elements of the array
  IAAFTypeDef *basetd=NULL;
  check (tda->GetType (&basetd));
  IAAFTypeDefObjectRef *tdo=NULL;
  check (basetd->QueryInterface (IID_IAAFTypeDefObjectRef, (void **)&tdo));

  // Get the property value for the indexed record
  IAAFPropertyValue *pvr=NULL;
  check (tda->GetElementValue (pva, index, &pvr));

  // Get the object contained in that property value
  check (tdo->GetObject (pvr, personnelResource));

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



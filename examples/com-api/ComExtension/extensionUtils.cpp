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

#if defined(_MAC) || defined(macintosh)
#include <wstring.h>
#endif

#include <iostream.h>

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

//
// Temporarily (?) only used to print out the role enum.
//
static void sPrintAuid (const aafUID_t auid)
{
  cout << hex
	   << "{ 0x" << auid.Data1
	   << ", 0x" << auid.Data2
	   << ", 0x" << auid.Data3
	   << ", {";
  aafUInt32 i;
  for (i = 0; i < 8; i++)
	{
	  if (i)
		cout << ", ";
	  cout << "0x" << (int) auid.Data4[i];
	}
  cout << "} }";
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
// Returns true if the roles match; returns false otherwise.
//
bool AreRolesEqual (const eRole & roleA, const eRole & roleB)
{
  return (memcmp (&roleA, &roleB, sizeof (eRole)) == 0 ?
		  true : false);
}


//
// Prints the given eRole.
//
void PrintRole (IAAFDictionary * pDict,
				const eRole role)
{
  assert (pDict);
  IAAFTypeDef *ptd=NULL;
  check (pDict->LookupType (kTypeID_eRole, &ptd));

  IAAFTypeDefExtEnum *ptde=NULL;
  check (ptd->QueryInterface (IID_IAAFTypeDefExtEnum, (void **)&ptde));

  aafUInt32 nameLen = 0;
  check (ptde->GetNameBufLenFromAUID (role, &nameLen));
  assert (nameLen);

  aafCharacter *buf = 0;
  buf = new aafCharacter[nameLen];
  assert (buf);

  check (ptde->GetNameFromAUID (role, buf, nameLen));

  cout << buf;

  delete[] buf;
  ptd->Release();
  ptd=NULL;
  ptde->Release();
  ptde=NULL;
}


//
// Prints the given PersonnelResource.
//
void PrintPersonnelResource (IAAFDictionary * pDict,
							 const PersonnelResource & resource)
{
  assert (pDict);
  assert (resource.name);

  cout << "Name:        " << resource.name << endl;
  cout << "Role:        "; PrintRole (pDict, resource.role); cout << endl;
  if (resource.cid_present)
	cout << "Contract ID: " << dec << resource.cid << endl;
}


//
// Prints all personnel resources in the given PersonnelMob.
//
void PrintPersonnelResources (IAAFDictionary * pDict,
							  IAAFMob * pMob)
{
  // Get an IAAFObject for the mob, then use it to get the class
  // definition describing the PersonnelMob
  IAAFObject *pMobObj=NULL;
  check (pMob->QueryInterface (IID_IAAFObject, (void **)&pMobObj));
  IAAFClassDef *cd=NULL;
  check (pMobObj->GetDefinition (&cd));

  // Use the class definition to get the definition for the Personnel
  // property.
  IAAFPropertyDef *pd=NULL;
  check (cd->LookupPropertyDef (kPropID_PersonnelMob_Personnel,
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
  aafUInt32 numPersonnel = 0;
  check (tda->GetCount (pva, &numPersonnel));

  // Get the type def describing elements of the array
  IAAFTypeDef *basetd=NULL;
  check (tda->GetType (&basetd));
  IAAFTypeDefObjectRef *tdo=NULL;
  check (basetd->QueryInterface (IID_IAAFTypeDefObjectRef, (void **)&tdo));

  cout << "There are " << numPersonnel
	   << " personnel record objects." << endl;

  // Print each element in the array.
  aafUInt32 i;
  for (i = 0; i < numPersonnel; i++)
	{
	  // put a newline between each
	  if (i)
		cout << endl;

	  // Get the property value for the indexed record
	  IAAFPropertyValue *pvr=NULL;
	  check (tda->GetElementValue (pva, i, &pvr));

	  // Get the object contained in that property value
	  IAAFObject *personObj=NULL;
	  check (tdo->GetObject (pvr, &personObj));

	  // Get the resource info from that object
	  PersonnelResource r =
		PersonnelRecordGetInfo (personObj);

	  // print the resource info
	  PrintPersonnelResource (pDict, r);
	  pvr->Release();
	  pvr=NULL;
	  personObj->Release();
	  personObj=NULL;
	}
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
  basetd->Release();
  basetd=NULL;
  tdo->Release();
  tdo=NULL;
}


PersonnelResource FormatResource (aafCharacter * name,
								  const eRole &  role)
{
  PersonnelResource r;
  assert (name);
  wcscpy (r.name, name);
  r.role = role;
  r.cid_present = false;
  r.cid = 0;

  return r;
}


PersonnelResource FormatResource (aafCharacter * name,
								  const eRole &  role,
								  contractID_t   cid)
{
  PersonnelResource r =
	FormatResource (name, role);
  r.cid_present = true;
  r.cid = cid;
  return r;
}


void PersonnelRecordSetName (IAAFObject * pObj,
							 const aafCharacter * name)
{
  assert (pObj);
  assert (name);

  IAAFClassDef *cd=NULL;
  check (pObj->GetDefinition (&cd));

  IAAFPropertyDef *pd=NULL;
  check (cd->LookupPropertyDef (kPropID_PersonnelResource_Name,
								&pd));

  IAAFPropertyValue *pv=NULL;
  check (pObj->GetPropertyValue (pd, &pv));
  
  IAAFTypeDef *td=NULL;
  check (pd->GetTypeDef (&td));

  IAAFTypeDefString *tds=NULL;
  check (td->QueryInterface (IID_IAAFTypeDefString, (void **)&tds));

  check (tds->SetCString (pv,
						  (aafMemPtr_t) name,
						  wcslen (name) * sizeof (aafCharacter)));

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


aafUInt32 PersonnelRecordGetNameBufLen (IAAFObject * pObj)
{
  assert (pObj);

  IAAFClassDef *cd=NULL;
  check (pObj->GetDefinition (&cd));

  IAAFPropertyDef *pd=NULL;
  check (cd->LookupPropertyDef (kPropID_PersonnelResource_Name,
								&pd));

  IAAFPropertyValue *pv=NULL;
  check (pObj->GetPropertyValue (pd, &pv));
  
  IAAFTypeDef *td=NULL;
  check (pd->GetTypeDef (&td));

  IAAFTypeDefString *tds=NULL;
  check (td->QueryInterface (IID_IAAFTypeDefString, (void **)&tds));

  aafUInt32 numChars = 0;
  check (tds->GetCount (pv, &numChars));

  // Add 1 for null terminator.
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
  return numChars + 1;
}


void PersonnelRecordGetName (IAAFObject * pObj,
							 aafCharacter * buf,
							 aafUInt32 buflen)
{
  assert (pObj);
  assert (buf);
  aafUInt32 nameLen = PersonnelRecordGetNameBufLen (pObj);
  assert (buflen >= nameLen);

  IAAFClassDef *cd=NULL;
  check (pObj->GetDefinition (&cd));

  IAAFPropertyDef *pd=NULL;
  check (cd->LookupPropertyDef (kPropID_PersonnelResource_Name,
								&pd));

  IAAFPropertyValue *pv=NULL;
  check (pObj->GetPropertyValue (pd, &pv));
  
  IAAFTypeDef *td=NULL;
  check (pd->GetTypeDef (&td));

  IAAFTypeDefString *tds=NULL;
  check (td->QueryInterface (IID_IAAFTypeDefString, (void **)&tds));

  check (tds->GetElements (pv, (aafMemPtr_t) buf, buflen));

  // Make sure we're null-terminated.
  buf[nameLen-1] = '\0';
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


void PersonnelRecordSetRole (IAAFObject * pObj,
							 eRole role)
{
  assert (pObj);

  IAAFClassDef *cd=NULL;
  check (pObj->GetDefinition (&cd));

  IAAFPropertyDef *pd=NULL;
  check (cd->LookupPropertyDef (kPropID_PersonnelResource_Role,
								&pd));

  IAAFPropertyValue *pv=NULL;
  check (pObj->GetPropertyValue (pd, &pv));
  
  IAAFTypeDef *td=NULL;
  check (pd->GetTypeDef (&td));

  IAAFTypeDefExtEnum *tde=NULL;
  check (td->QueryInterface (IID_IAAFTypeDefExtEnum, (void **)&tde));

  check (tde->SetAUIDValue (pv, role));
  check (pObj->SetPropertyValue (pd, pv));
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


eRole PersonnelRecordGetRole (IAAFObject * pObj)
{
  eRole r;
  assert (pObj);

  IAAFClassDef *cd=NULL;
  check (pObj->GetDefinition (&cd));

  IAAFPropertyDef *pd=NULL;
  check (cd->LookupPropertyDef (kPropID_PersonnelResource_Role,
								&pd));

  IAAFPropertyValue *pv=NULL;
  check (pObj->GetPropertyValue (pd, &pv));
  
  IAAFTypeDef *td=NULL;
  check (pd->GetTypeDef (&td));

  IAAFTypeDefExtEnum *tde=NULL;
  check (td->QueryInterface (IID_IAAFTypeDefExtEnum, (void **)&tde));

  check (tde->GetAUIDValue (pv, &r));
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
  return r;
}


void PersonnelRecordSetContractID (IAAFObject * pObj,
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


bool PersonnelRecordContractIDIsPresent (IAAFObject * pObj)
{
  aafBool r;
  assert (pObj);

  IAAFClassDef *cd=NULL;
  check (pObj->GetDefinition (&cd));

  IAAFPropertyDef *pd=NULL;
  check (cd->LookupPropertyDef (kPropID_PersonnelResource_ContractID,
								&pd));

  check (pObj->IsPropertyPresent (pd, &r));
  cd->Release();
  cd=NULL;
  pd->Release();
  pd=NULL;
  return r ? true : false;
}


contractID_t PersonnelRecordGetContractID (IAAFObject * pObj)
{
  assert (pObj);
  assert (PersonnelRecordContractIDIsPresent (pObj));

  contractID_t r;

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

  check (tdi->GetInteger (pv, (aafMemPtr_t) &r, sizeof (r)));
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
  return r;
}


void PersonnelRecordSetInfo (IAAFObject * pObj,
							 const PersonnelResource & info)
{
  assert (info.name);
  PersonnelRecordSetName (pObj, info.name);
  PersonnelRecordSetRole (pObj, info.role);
  if (info.cid_present)
	PersonnelRecordSetContractID (pObj, info.cid);
}


PersonnelResource PersonnelRecordGetInfo (IAAFObject * pObj)
{
  PersonnelResource r;

  PersonnelRecordGetName (pObj, r.name, sizeof (r.name));
  r.role = PersonnelRecordGetRole (pObj);
  if (PersonnelRecordContractIDIsPresent (pObj))
	{
	  r.cid = PersonnelRecordGetContractID (pObj);
	  r.cid_present = true;
	}
  else
	{
	  r.cid_present = false;
	}
  return r;
}

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

#include "AAF.h"
#include "AAFStoredObjectIDs.h"
#include "AAFTypeDefUIDs.h"

#include "extensionUtils.h"
#include "extensionWrite.h"

#include <assert.h>
#include <stdio.h>

//
// This example code is intended to show how AAF may be extended to
// store personnel information relating to the staff which worked on a
// project.
//
// As a side benefit, it will also show how to define subclasses of
// existing classes, and add collections of them to AAF files.
//
// Specifically, this is what we'll do:
//
// - The eRole enumeration will be an extensible enumeration describing
//   that person's role in the project:
//
//  extensibleEnum eRole 
//  { 
//    Producer, 
//  	Editor, 
//  	FloorManager, 
//  	...
//  };
//
//
// - Create a new object (called PersonnelResource) which is used to
//   store the information corresponding to one person who's worked on
//   this project.  In pseudo-C++, it will look like this:
//
//   PersonnelResource : subclass of InterchangeObject 
//   { 
//     // mandatory properties 
//     String       name; 
//     eRole        role; 
//
//     // contract ID is optional
//     bool         cid_present;
//     contractID_t cid;
//   };
//
//
// - We will extend Mob to contain a collection of these
//   PersonnelResource objects:
//
//   PersonnelMob : subclass of Mob 
//   { 
//     // mandatory property 
//     StrongRefVector<Person> personnel;
//   };
//


static void convert(char* cName, size_t length, const wchar_t* name)
{
  assert (name);
  assert (*name);
  assert (cName);
  assert (length > 0);

  size_t status = wcstombs(cName, name, length);
  if (status == (size_t)-1) {
    cerr << "\nError : Conversion failed.\n" << endl;
    exit(1);  
  }
}


//
// Creates a type definition to describe eRole enumerations, and
// registers it in the dictionary.
//
static void CreateAndRegisterRoleEnum (IAAFDictionary * pDict)
{
  assert (pDict);

  IAAFTypeDefExtEnumSP ptde;

  check (pDict->CreateInstance (&AUID_AAFTypeDefExtEnum,
								IID_IAAFTypeDefExtEnum,
								(IUnknown**) &ptde));

  check (ptde->Initialize ((aafUID_t*) &kTypeID_eRole,
						  L"PersonnelRole"));

  check (ptde->AppendElement ((aafUID_t*) &kRole_Producer,     L"Producer"));
  check (ptde->AppendElement ((aafUID_t*) &kRole_Editor,       L"Editor"));
  check (ptde->AppendElement ((aafUID_t*) &kRole_FloorManager, L"FloorManager"));

  IAAFTypeDefSP ptd;
  check (ptde->QueryInterface (IID_IAAFTypeDef, (void **)&ptd));
  check (pDict->RegisterType (ptd));
}


//
// Creates a class definition to describe PersonnelResource objects,
// and registers it in the dictionary.
//
static void CreateAndRegisterPersonnelResource (IAAFDictionary * pDict)
{
  assert (pDict);

  IAAFClassDefSP pcd;

  check (pDict->CreateInstance (&AUID_AAFClassDef,
								IID_IAAFClassDef,
								(IUnknown**) &pcd));

  IAAFClassDefSP pcd_Object;
  check (pDict->LookupClass (&AUID_AAFObject, &pcd_Object));

  check (pcd->Initialize ((aafUID_t*) &kClassID_PersonnelResource,
						  pcd_Object,
						  L"PersonnelResource"));

  IAAFPropertyDefSP pd_unused;

  IAAFTypeDefSP ptd_String;
  check (pDict->LookupType ((aafUID_t*) &kAAFTypeID_String, &ptd_String));
  check (pcd->AppendNewPropertyDef ((aafUID_t*) &kPropID_PersonnelResource_Name,
									L"Name",
									ptd_String,
									AAFFalse,
									&pd_unused));

  IAAFTypeDefSP ptd_Role;
  check (pDict->LookupType ((aafUID_t*) &kTypeID_eRole, &ptd_Role));
  check (pcd->AppendNewPropertyDef ((aafUID_t*) &kPropID_PersonnelResource_Role,
									L"Role",
									ptd_Role,
									AAFFalse,
									&pd_unused));

  IAAFTypeDefSP ptd_ui32;
  check (pDict->LookupType ((aafUID_t*) &kAAFTypeID_UInt32, &ptd_ui32));
  check (pcd->AppendNewPropertyDef ((aafUID_t*) &kPropID_PersonnelResource_ContractID,
									L"ContractID",
									ptd_ui32,
									AAFTrue,
									&pd_unused));

  check (pDict->RegisterClass (pcd));
}



static void
CreateAndRegisterPersonnelResourceReference
(IAAFDictionary * pDict)
{
  assert (pDict);

  IAAFClassDefSP pcd;
  check (pDict->LookupClass ((aafUID_t*) &kClassID_PersonnelResource,
							 &pcd));

  IAAFTypeDefObjectRefSP ptdr;
  check (pDict->CreateInstance (&AUID_AAFTypeDefStrongObjRef,
								IID_IAAFTypeDefObjectRef,
								(IUnknown**) &ptdr));

  check (ptdr->Initialize ((aafUID_t*) &kTypeID_PersonnelResourceStrongReference,
						   pcd,
						   L"PersonnelResourceStrongReference"));

  IAAFTypeDefSP ptd;
  check (ptdr->QueryInterface (IID_IAAFTypeDef, (void **)&ptd));

  check (pDict->RegisterType (ptd));
}


static void
CreateAndRegisterPersonnelResourceReferenceVector
(IAAFDictionary * pDict)
{
  assert (pDict);

  IAAFTypeDefVariableArraySP ptdv;
  check (pDict->CreateInstance (&AUID_AAFTypeDefVariableArray,
								IID_IAAFTypeDefVariableArray,
								(IUnknown**) &ptdv));

  IAAFTypeDefSP ptdr;
  check (pDict->LookupType ((aafUID_t*) &kTypeID_PersonnelResourceStrongReference,
							&ptdr));

  check (ptdv->Initialize ((aafUID_t*) &kTypeID_PersonnelResourceStrongReferenceVector,
						   ptdr,
						   L"PersonnelResourceStrongReferenceVector"));

  IAAFTypeDefSP ptd;
  check (ptdv->QueryInterface (IID_IAAFTypeDef, (void **)&ptd));

  check (pDict->RegisterType (ptd));
}


//
// Creates a class definition to describe PersonnelMob objects, and
// registers it in the dictionary.
//
static void CreateAndRegisterPersonnelMob (IAAFDictionary * pDict)
{
  assert (pDict);

  IAAFClassDefSP pcd;

  check (pDict->CreateInstance (&AUID_AAFClassDef,
								IID_IAAFClassDef,
								(IUnknown**) &pcd));

  IAAFClassDefSP pcd_Mob;
  check (pDict->LookupClass (&AUID_AAFMob, &pcd_Mob));

  check (pcd->Initialize ((aafUID_t*) &kClassID_PersonnelMob,
						  pcd_Mob,
						  L"PersonnelMob"));

  IAAFPropertyDefSP pd_unused;

  IAAFTypeDefSP ptd_PersonnelVector;
  check (pDict->LookupType ((aafUID_t*) &kTypeID_PersonnelResourceStrongReferenceVector,
							&ptd_PersonnelVector));
  check (pcd->AppendNewPropertyDef ((aafUID_t*) &kPropID_PersonnelMob_Personnel,
									L"Personnel",
									ptd_PersonnelVector,
									AAFFalse,
									&pd_unused));

  check (pDict->RegisterClass (pcd));
}


static void CreatePersonnelMob (IAAFDictionary * pDict,
								IAAFMob ** ppMob)
{
  assert (pDict);
  assert (ppMob);

  IAAFMob * pMob = 0;
  check (pDict->CreateInstance ((aafUID_t*) &kClassID_PersonnelMob,
								IID_IAAFMob,
								(IUnknown**) &pMob));
  assert (pMob);
  check (pMob->SetMobID ((aafUID_t*) &kMobID_Personnel));
  check (pMob->SetName (L"Personnel"));
  
  assert (ppMob);
  *ppMob = pMob;
}


static void AppendResource (IAAFDictionary * pDict,
							IAAFMob *pMob,
							const PersonnelResource & info)
{
  assert (pMob);
  assert (info.name);

  IAAFObjectSP pObj;
  check (pDict->CreateInstance ((aafUID_t*) &kClassID_PersonnelResource,
								IID_IAAFObject,
								(IUnknown**) &pObj));

  PersonnelRecordSetInfo (pObj, info);

  // Create a property value corresponding to new object reference;
  // first get a type def for that kind of obj ref, then create the
  // prop val through the type def.
  IAAFTypeDefSP td;
  check (pDict->LookupType ((aafUID_t*) &kTypeID_PersonnelResourceStrongReference,
							&td));
  
  IAAFTypeDefObjectRefSP tdo;
  check (td->QueryInterface (IID_IAAFTypeDefObjectRef,
							 (void **)&tdo));
  IAAFPropertyValueSP pv;
  check (tdo->CreateValue (pObj, &pv));

  // Append record to mob.
  //
  // First get the existing array-of-objects property value
  IAAFObjectSP pMobObj;
  check (pMob->QueryInterface (IID_IAAFObject,
							   (void **)&pMobObj));
  IAAFClassDefSP cd;
  check (pMobObj->GetDefinition (&cd));
  IAAFPropertyDefSP pd;
  check (cd->LookupPropertyDef ((aafUID_t*) &kPropID_PersonnelMob_Personnel,
								&pd));
  IAAFPropertyValueSP pva;
  check (pMobObj->GetPropertyValue (pd, &pva));

  // Now get a type def for the array of personnel, and append the new
  // personnel prop val to it.
  IAAFTypeDefSP tdpv;
  check (pva->GetType (&tdpv));
  IAAFTypeDefVariableArraySP tda;
  check (tdpv->QueryInterface (IID_IAAFTypeDefVariableArray,
							   (void **)&tda));
  check (tda->AppendElement (pva, pv));
  // There. It's appended.  Now set the property value.
  check (pMobObj->SetPropertyValue (pd, pva));
}


void extensionWrite (const aafCharacter * filename)
{
  cout << "***Creating file " << filename << "***" << endl;

  aafProductIdentification_t  ProductInfo;
  
  // delete any previous test file before continuing...
  char chFileName[1000];
  convert(chFileName, sizeof(chFileName), filename);
  remove(chFileName);

  // Create a new file...
  static const aafUID_t NULL_UID = { 0 };
  ProductInfo.companyName = L"AAF Developers Desk";
  ProductInfo.productName = L"AAF extension example";
  ProductInfo.productVersion.major = 1;
  ProductInfo.productVersion.minor = 0;
  ProductInfo.productVersion.tertiary = 0;
  ProductInfo.productVersion.patchLevel = 0;
  ProductInfo.productVersion.type = kVersionUnknown;
  ProductInfo.productVersionString = 0;
  ProductInfo.productID = NULL_UID;
  ProductInfo.platform = 0;
  
  IAAFFileSP pFile;
  check (AAFFileOpenNewModify ((aafCharacter*) filename,
							   0,
							   &ProductInfo,
							   &pFile));
  
  IAAFHeaderSP pHead;
  check (pFile->GetHeader(&pHead));

  IAAFDictionarySP pDict;
  check (pHead->GetDictionary(&pDict));

  CreateAndRegisterRoleEnum (pDict);
  CreateAndRegisterPersonnelResource (pDict);
  CreateAndRegisterPersonnelResourceReference (pDict);
  CreateAndRegisterPersonnelResourceReferenceVector (pDict);
  CreateAndRegisterPersonnelMob (pDict);

  IAAFMobSP pMob;
  CreatePersonnelMob (pDict, &pMob);
  check (pHead->AppendMob (pMob));

  AppendResource (pDict, pMob,
				  FormatResource (L"Peter Vechtor", kRole_Producer,    42));
  AppendResource (pDict, pMob,
				  FormatResource (L"Oliver Morgan", kRole_FloorManager, 6 ));
  AppendResource (pDict, pMob,
				  FormatResource (L"Tom Ohanian",   kRole_Editor));

  // save and exit.
  check (pFile->Save());
  check (pFile->Close());
}

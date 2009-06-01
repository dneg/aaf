//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include "AAF.h"
#include "AAFStoredObjectIDs.h"
#include "AAFTypeDefUIDs.h"
#include "AAFFileKinds.h"

#include "extensionUtils.h"
#include "extensionWrite.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

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
// - The ePosition enumeration will be an extensible enumeration describing
//   that person's position in the project:
//
//  extensibleEnum ePosition 
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
//     String       givenName; 
//	   String		familyName;
//     ePosition        position; 
//
//     // contract ID is optional
//     bool         cid_present;
//     contractID_t cid;
//	   String		part; optional for actors
//   };
//
//
// - We will extend Mob to contain a collection of these
//   PersonnelResource objects:
//
//   AdminMob : subclass of Mob 
//   { 
//     // mandatory property 
//     StrongRefVector<Person> personnel;
//   };
//


//
// Utility to convert wide  strings to single-byte-character
// strings.
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

static aafCharacter companyName[] = L"AMW Association";
static aafCharacter productName[] = L"extensionWrite";


//
// - Creates an AAF file with the given name.
// - Registers extensions that we'll use in the file's dictionary.
// - Creates a AdminMob and adds it to the file's header.
// - Creates several PersonnelRecord objects and appends them to the
//   AdminMob.
// - Saves the file.
//
void extensionWrite (const aafCharacter * filename)
{
  IAAFFile *pFile=NULL;
  IAAFHeader *pHead=NULL;
  IAAFDictionary *pDict=NULL;
  IAAFMob *pAdminMob=NULL;
  IAAFObject *pPersResource=NULL;
  IAAFClassDef *pcd = 0;
  

  try
  {
    cout << "***Creating file " << filename << "***" << endl;

    aafProductIdentification_t  ProductInfo;
  
    // delete any previous test file before continuing...
    char chFileName[1000];
    convert(chFileName, sizeof(chFileName), filename);
    remove(chFileName);

    // Create a new file...
    static const aafUID_t NULL_UID = { 0 };
	aafProductVersion_t v;
	v.major = 1;
	v.minor = 0;
	v.tertiary = 0;
	v.patchLevel = 0;
	v.type = kAAFVersionUnknown;
	ProductInfo.companyName = companyName;
    ProductInfo.productName = productName;
    ProductInfo.productVersion = &v;
    ProductInfo.productVersionString = 0;
    ProductInfo.productID = NULL_UID;
    ProductInfo.platform = 0;
  
    check (AAFFileOpenNewModifyEx ((aafCharacter*) filename,
							     &kAAFFileKind_Aaf4KBinary,
							     0,
							     &ProductInfo,
							     &pFile));
  
    check (pFile->GetHeader(&pHead));
    check (pHead->GetDictionary(&pDict));

    DefineResourceClassExtensions(pDict);

 
    // Instantiate a AdministrativeMob object.
	check (pDict->LookupClassDef (kClassID_AdminMob, &pcd));
    check (pcd->CreateInstance (IID_IAAFMob,
								(IUnknown**) &pAdminMob));
	pcd->Release();
	pcd = 0;
    check (pAdminMob->SetName (L"Administrative Information"));
 

    // Add the new AdministrativeMob object to the file's header.
    check (pHead->AddMob (pAdminMob));

    // Add several PersonnelResource objects to the AdminMob.
    // Instantiate the PersonnelResource object.
	check (pDict->LookupClassDef (kClassID_PersonnelResource, &pcd));
    check (pcd->CreateInstance (IID_IAAFObject,
								(IUnknown**) &pPersResource));
	pcd->Release();
	pcd = 0;

    PersonnelResourceInitialize (pPersResource,
							     L"Morgan",
							     L"Oliver",
							     kPosition_FloorManager);
    AdminMobAppendResource (pDict,
						    pAdminMob,
						    pPersResource);
    pPersResource->Release();
    pPersResource=NULL;

  // Instantiate the PersonnelResource object.
	check (pDict->LookupClassDef (kClassID_PersonnelResource, &pcd));
    check (pcd->CreateInstance (IID_IAAFObject,
								(IUnknown**) &pPersResource));
	pcd->Release ();
	pcd = 0;

    PersonnelResourceInitialize (pPersResource,
							     L"Ohanian",
							     L"Tom",
							     kPosition_Editor);
    PersonnelResourceSetContractID(pPersResource, 299);

    AdminMobAppendResource (pDict,
						    pAdminMob,
						    pPersResource);
    pPersResource->Release();
    pPersResource=NULL;
  // Instantiate the PersonnelResource object.
	check (pDict->LookupClassDef (kClassID_PersonnelResource, &pcd))
    check (pcd->CreateInstance (IID_IAAFObject,
								(IUnknown**) &pPersResource));
	pcd->Release();
	pcd = 0;

    PersonnelResourceInitialize (pPersResource,
							     L"Oldman",
							     L"Arianna",
							     kPosition_Actor);
    PersonnelResourceSetActorRole(pPersResource, L"Lucy");
    PersonnelResourceSetContractID(pPersResource, 735);
    AdminMobAppendResource (pDict,
						    pAdminMob,
						    pPersResource);
    pPersResource->Release();
    pPersResource=NULL;
    pAdminMob->Release();
    pAdminMob=NULL;
    pDict->Release();
    pDict=NULL;
    pHead->Release();
    pHead=NULL;
    // Save the file and close it.
    check (pFile->Save());
    check (pFile->Close());
    pFile->Release();
    pFile=NULL;
  }
  catch (...)
  {
    // cleanup on error...
    if (pPersResource)
      pPersResource->Release();
    if (pAdminMob)
      pAdminMob->Release();
    if (pDict)
      pDict->Release();
    if (pHead)
      pHead->Release();
	if (pcd)
	  {
		pcd->Release();
		pcd = 0;
	  }

    if (pFile)
    {
      pFile->Close();
      pFile->Release();
    }

    throw;
  }
}

//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include "AAFPersonnelExtension.h"
#include "AAFPlugin.h"
#include "AAFStoredObjectIDs.h"
#include "AAFTypeDefUIDs.h"
#include "AAFFileKinds.h"

#include "extensionUtils.h"
#include "extensionWritePlugin.h"
#include "AAFResult.h"
#include "AAFTypes.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#if defined( OS_WINDOWS )
#define AAFPERSONNELEXTENSION_DLLNAME L"AAFPersonnelExtension.dll"
#elif defined( OS_DARWIN )
#define AAFPERSONNELEXTENSION_DLLNAME L"libAAFPersonnelExtension.dylib"
#elif defined( OS_UNIX )
#define AAFPERSONNELEXTENSION_DLLNAME L"libAAFPersonnelExtension.so"
#else
#error Unknown operating system
#endif

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



//
// - Creates an AAF file with the given name.
// - Registers extensions that we'll use in the file's dictionary.
// - Creates a AdminMob and adds it to the file's header.
// - Creates several PersonnelRecord objects and appends them to the
//   AdminMob.
// - Saves the file.
//
HRESULT extensionWritePlugin (const aafCharacter * filename)
{
  HRESULT rc = S_OK;
  IAAFPluginManager *pPluginManager = NULL;
  IAAFClassExtension *pClassExtension = NULL;
  IAAFFile *pFile=NULL;
  IAAFHeader *pHead=NULL;
  IAAFDictionary *pDict=NULL;
  IAAFAdminMob *pAdminMob=NULL;
  IAAFMob *pMob=NULL;
  IAAFPersonnelResource *pPersResource=NULL;
  IAAFClassDef * pcd = 0;


  cout << "***Creating file " << filename << " with plugin interface***" << endl;


  try
  {
    // Load our plugin.
    check(AAFGetPluginManager(&pPluginManager));
    rc = pPluginManager->RegisterPluginFile(AAFPERSONNELEXTENSION_DLLNAME);
    if (AAFRESULT_PLUGIN_ALREADY_REGISTERED == rc)
      rc = S_OK;
    check (rc);
    
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
    ProductInfo.companyName = L"AAF Developers Desk";
    ProductInfo.productName = L"AAF extension example";
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

    // Ask the plugins to register and initialize all necessary defintions.
    check (pPluginManager->CreateInstance(CLSID_AAFAdminMob, NULL, IID_IAAFClassExtension, (void **)&pClassExtension));
    check (pClassExtension->RegisterDefinitions(pDict));
    pClassExtension->Release();
    pClassExtension = NULL;
      
    pPluginManager->Release();
    pPluginManager = NULL;

 
    // Instantiate a AdministrativeMob object.
	check (pDict->LookupClassDef (kClassID_AdminMob, &pcd))
    check (pcd->CreateInstance (IID_IAAFMob,
								(IUnknown**) &pMob));
	pcd->Release ();
	pcd = 0;

    check (pMob->SetName (L"Administrative Information"));
    check (pMob->QueryInterface(IID_IAAFAdminMob, (void **)&pAdminMob));

    // Add the new AdministrativeMob object to the file's header.
    check (pHead->AddMob (pMob));

    // Add several PersonnelResource objects to the AdminMob.
    // Instantiate the PersonnelResource object.
	check (pDict->LookupClassDef (kClassID_PersonnelResource, &pcd));
    check (pcd->CreateInstance (IID_IAAFPersonnelResource,
								(IUnknown**) &pPersResource));
	pcd->Release ();
	pcd = 0;

    check (pPersResource->Initialize(
							     L"Morgan",
                   L"Oliver",
							     kPosition_FloorManager));
    check (pAdminMob->AppendPersonnelResource(pPersResource));
    pPersResource->Release();
    pPersResource=NULL;

  // Instantiate the PersonnelResource object.
	check (pDict->LookupClassDef (kClassID_PersonnelResource, &pcd));
    check (pcd->CreateInstance (IID_IAAFPersonnelResource,
								(IUnknown**) &pPersResource));
	pcd->Release ();
	pcd = 0;

    check (pPersResource->Initialize(
							     L"Ohanian",
							     L"Tom",
							     kPosition_Editor));
    check (pPersResource->SetContractID(299));

    check (pAdminMob->AppendPersonnelResource(pPersResource));
    pPersResource->Release();
    pPersResource=NULL;
  // Instantiate the PersonnelResource object.
    check (pDict->LookupClassDef (kClassID_PersonnelResource, &pcd));
    check (pcd->CreateInstance (IID_IAAFPersonnelResource,
								(IUnknown**) &pPersResource));
	pcd->Release ();
	pcd = 0;

    check (pPersResource->Initialize(
							     L"Oldman",
							     L"Arianna",
							     kPosition_Actor));
    check (pPersResource->SetPart(L"Lucy"));
    check (pPersResource->SetContractID(735));
    check (pAdminMob->AppendPersonnelResource(pPersResource));
  }
  catch (HRESULT& rhr)
  {
    rc = rhr;
  }
  catch (...)
  {
    // We CANNOT throw an exception out of a COM interface method!
    // Return a reasonable exception code.
  	rc =  AAFRESULT_UNEXPECTED_EXCEPTION;
  }


  if (pPersResource)
    pPersResource->Release();
  if (pAdminMob)
    pAdminMob->Release();
  if (pMob)
    pMob->Release();
  if (pClassExtension)
    pClassExtension->Release();
  if (pDict)
    pDict->Release();
  if (pHead)
    pHead->Release();
  // Save the file and close it.
  if (pcd)
	{
	  pcd->Release ();
	  pcd = 0;
	}

  if (pFile)
  {
  	try
	{
    	check (pFile->Save());
   	    check (pFile->Close());
        pFile->Release();
	}
    catch (HRESULT& rhr)
    {
    rc = rhr;
    }
    catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
    	rc =  AAFRESULT_UNEXPECTED_EXCEPTION;
    }
  }

  if (pPluginManager)
    pPluginManager->Release();

  return rc;
}

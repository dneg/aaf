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
 * prior written permission of Avid Technology, Inc.
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

#include "AAFPersonnelExtension.h"
#include "AAFPlugin.h"
#include "AAFStoredObjectIDs.h"
#include "AAFTypeDefUIDs.h"

#include "extensionUtils.h"
#include "extensionWritePlugin.h"
#include "AAFResult.h"

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
  IAAFPlugin *pPlugin = NULL;
  IAAFFile *pFile=NULL;
  IAAFHeader *pHead=NULL;
  IAAFDictionary *pDict=NULL;
  IAAFDefObject *pDefObject = NULL;
  IAAFAdminMob *pAdminMob=NULL;
  IAAFMob *pMob=NULL;
  IAAFPersonnelResource *pPersResource=NULL;
  IAAFClassDef * pcd = 0;


  cout << "***Creating file " << filename << " with plugin interface***" << endl;


  try
  {
    // Load our plugin.
    check(AAFGetPluginManager(&pPluginManager));
    rc = pPluginManager->RegisterPluginFile(L"AAFPersonnelExtension.dll");
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
  
    check (AAFFileOpenNewModify ((aafCharacter*) filename,
							     0,
							     &ProductInfo,
							     &pFile));
  
    check (pFile->GetHeader(&pHead));
    check (pHead->GetDictionary(&pDict));

    // Ask the plugins to register and initialize all necessary defintions.
    check (pPluginManager->CreateInstance(CLSID_AAFAdminMob, NULL, IID_IAAFPlugin, (void **)&pPlugin));

    aafInt32 defCount = 0;
    check (pPlugin->GetNumDefinitions(&defCount));
    for (aafInt32 i = 0; i < defCount; ++i)
    {
      check (pPlugin->GetIndexedDefinitionObject(i, pDict, &pDefObject));
      pDefObject->Release();
      pDefObject = NULL;
    }

    pPlugin->Release();
    pPlugin = NULL;
      
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


  if (pPersResource)
    pPersResource->Release();
  if (pAdminMob)
    pAdminMob->Release();
  if (pMob)
    pMob->Release();
  if (pPlugin)
    pPlugin->Release();
  if (pDefObject)
    pDefObject->Release();
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
    check (pFile->Save());
    check (pFile->Close());
    pFile->Release();
  }
  if (pPluginManager)
    pPluginManager->Release();

  return rc;
}

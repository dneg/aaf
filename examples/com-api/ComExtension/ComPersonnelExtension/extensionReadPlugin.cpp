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

#include "AAFPersonnelExtension.h"


#include "extensionReadPlugin.h"
#include "extensionUtils.h"
#include "AAFTypes.h"
#include "AAFResult.h"


#include <iostream.h>
#include <iomanip.h>
#include <assert.h>


#if defined( OS_WINDOWS ) || defined( OS_MACOS )
#define AAFPERSONNELEXTENSION_DLLNAME L"AAFPersonnelExtension.dll"
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
// See comments in extensionWrite.cpp for a discussion of what we're
// trying to do and how we're doing it.
//

HRESULT extensionReadPlugin (const aafCharacter * filename)
{
  assert (filename);

  HRESULT rc = S_OK;
  IAAFPluginManager *pPluginManager = NULL;
  IAAFFile *pFile=NULL;
  IAAFHeader *pHead=NULL;
  IAAFDictionary *pDict=NULL;
  IAAFTypeDef *ptd=NULL;
  IAAFClassDef *pcd=NULL;
  IEnumAAFMobs*	pMobIter = NULL;
  IAAFMob *pMob=NULL;
	IAAFAdminMob *pAdminMob = NULL;
	IAAFPersonnelResource *pPersonnelResource = NULL;


  try
  {

    // Load our plugin.
    check(AAFGetPluginManager(&pPluginManager));
    rc = pPluginManager->RegisterPluginFile(AAFPERSONNELEXTENSION_DLLNAME);
    if (AAFRESULT_PLUGIN_ALREADY_REGISTERED == rc)
      rc = S_OK;
    check (rc);
    pPluginManager->Release();
    pPluginManager = NULL;



    cout << "***Reading file " << filename << " with external plugin interface***" << endl;

    check (AAFFileOpenExistingRead ((aafCharacter*) filename,
								    0,
								    &pFile));
    check (pFile->GetHeader(&pHead));
    check (pHead->GetDictionary(&pDict));

    VerifyResourceClassExtensions(pDict);


    cout << "Verifying AdminMob instance has been created and added" 
	     << " to header." << endl;
    // Need to find AdminMob, temp iterate through Mobs to find one.
    bool foundAdmin=false;
    check(pHead->GetMobs(NULL, &pMobIter));
    while(!foundAdmin && (AAFRESULT_SUCCESS == pMobIter->NextOne(&pMob)))
    {
	    //Check if Mob is a Admin Mob
	    if (SUCCEEDED(pMob->QueryInterface(IID_IAAFAdminMob,(void **)&pAdminMob)))
	    {
        foundAdmin = true;
      } 

      // We always need to release the mob reference.
		  pMob->Release();
		  pMob=NULL;
    }


    if (foundAdmin) 
    {
	    cout << "Printing contents of Admin Mob." << endl;
	    aafUInt32 numPersonnel;

      check(pAdminMob->CountPersonnelResources(&numPersonnel));
      cout << "There are " << numPersonnel
	     << " personnel record objects." << endl;

      // Print each element in the array.
      aafUInt32 i;
      for (i = 0; i < numPersonnel; i++)
	    {
        check(pAdminMob->GetPersonnelResourceAt(i,&pPersonnelResource)); 
		    aafUInt32 bufferLen,numChars;
		    check(pPersonnelResource->GetGivenNameBufLen (&bufferLen));
		    numChars = bufferLen/sizeof(aafCharacter);


		    aafCharacter *givenName = new aafCharacter [numChars+1];
		    check(pPersonnelResource->GetGivenName (givenName, bufferLen));
            cout << "Given Name:     " << givenName << endl;
		    delete[] givenName;
        givenName = NULL;

		    check(pPersonnelResource->GetFamilyNameBufLen (&bufferLen));
		    numChars = bufferLen/sizeof(aafCharacter);


		    aafCharacter *familyName = new aafCharacter [numChars];
		    check(pPersonnelResource->GetFamilyName (familyName, bufferLen));
            cout << "Family Name:    " << familyName << endl;
		    delete[] familyName;
        familyName = NULL;

		    ePosition position;
		    check(pPersonnelResource->GetPosition (&position));
		    cout << "Position:       ";
		    PrintPosition (pDict, position);
		    cout << endl;

		    HRESULT retStatus;
		    contractID_t cid;
		    retStatus = pPersonnelResource->GetContractID (&cid);
		    if (retStatus == AAFRESULT_PROP_NOT_PRESENT) 
		    {
			    cout << "No ContractID defined" << endl;
		    } 
        else if (SUCCEEDED(retStatus))
		    {
			    cout << "ContractID:     " << dec << cid << endl;
		    } 
        else 
          check (retStatus);

		    retStatus = pPersonnelResource->GetPartBufLen (&bufferLen);
		    if (retStatus == AAFRESULT_PROP_NOT_PRESENT) 
		    {
			    cout << "No ActorRole defined" << endl;
		    } 
        else if (SUCCEEDED(retStatus))
		    {
			  numChars = bufferLen/sizeof(aafCharacter);
			  aafCharacter *actorRole = new aafCharacter [numChars];
		      check(pPersonnelResource->GetPart (actorRole, bufferLen));
              cout << "ActorRole:      " << actorRole << endl;
		      delete[] actorRole;
          actorRole = NULL;
		    }
		    cout << endl;
		    pPersonnelResource->Release();
		    pPersonnelResource=NULL;
	    }

	    pAdminMob->Release();
	    pAdminMob=NULL;
    } 
    else
    {
	    cout << "Did not find Admin Mob" << endl;
    }

  }
  catch (HRESULT& rhr)
  {
    rc = rhr;
  }


  // done.
  if (pPersonnelResource)
    pPersonnelResource->Release();
  if (pMob)
    pMob->Release();
  if (pAdminMob)
    pAdminMob->Release();
  if (pMobIter)
    pMobIter->Release();
  if (ptd)
    ptd->Release();
  if (pcd)
    pcd->Release();
  if (pDict)
    pDict->Release();
  if (pHead)
    pHead->Release();
  if (pFile)
  {
    pFile->Close ();
    pFile->Release();
  }

  return rc;
}

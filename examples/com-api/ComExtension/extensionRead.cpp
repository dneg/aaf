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


#include "extensionRead.h"
#include "extensionUtils.h"
#include "AAFResult.h"

#include <iostream.h>
#include <iomanip.h>
#include <assert.h>


//
// This example code is intended to show how AAF may be extended to
// store personnel information relating to the staff which worked on a
// project.
//
// See comments in extensionWrite.cpp for a discussion of what we're
// trying to do and how we're doing it.
//

bool extensionRead (const aafCharacter * filename)
{
  assert (filename);

  IAAFFile *pFile=NULL;
  IAAFHeader *pHead=NULL;
  IAAFDictionary *pDict=NULL;
  IEnumAAFMobs*	pMobIter = NULL;
  IAAFMob *pMob=NULL;
	IAAFObject *pObject=NULL;
	IAAFClassDef *pMobCD=NULL;
	IAAFObject *personnelResource=NULL;
  aafCharacter *givenName=NULL;
  aafCharacter *familyName=NULL;
  aafCharacter *actorRole=NULL;


  try 
  {
    cout << "***Reading file " << filename << "***" << endl;

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
	    check(pMob->QueryInterface(IID_IAAFObject,(void **)&pObject));
	    check (pObject->GetDefinition(&pMobCD));
	    pObject->Release();
	    pObject=NULL;
	    if (classDefinitionIsA(pMobCD, kClassID_AdminMob))
	    {
		    foundAdmin = true;
	    } else
	    {
		    // will need to release pMob when it is not the right mob
		    pMob->Release();
		    pMob=NULL;
	    }
 	    pMobCD->Release();
	    pMobCD=NULL;
    }


    if (foundAdmin) 
    {
	    cout << "Printing contents of Admin Mob." << endl;
	    aafUInt32 numPersonnel;
	    AdminMobGetNumPersonnel(pDict, pMob, &numPersonnel);
      cout << "There are " << numPersonnel
           << " personnel record objects." << endl;

      // Print each element in the array.
      aafUInt32 i;
      for (i = 0; i < numPersonnel; i++)
	    {
		    AdminMobGetNthPersonnel(pDict, pMob, i,
							     &personnelResource);
		    aafUInt32 bufferLen,numChars;
		    PersonnelResourceGetGivenNameBufLen (personnelResource, &bufferLen);
		    numChars = bufferLen/sizeof(aafCharacter);

		    givenName = new aafCharacter [numChars+1];
		    PersonnelResourceGetGivenName (personnelResource,
										    givenName,
										    bufferLen+2);
        cout << "Given Name:     " << givenName << endl;
		    delete[] givenName;
        givenName = NULL;

		    PersonnelResourceGetFamilyNameBufLen (personnelResource, &bufferLen);
		    numChars = bufferLen/sizeof(aafCharacter);


		    familyName = new aafCharacter [numChars];
		    PersonnelResourceGetFamilyName (personnelResource,
										    familyName,
										    bufferLen);
        cout << "Family Name:    " << familyName << endl;
		    delete[] familyName;
        familyName = NULL;

		    ePosition position;
		    PersonnelResourceGetPosition (personnelResource,
							   &position);
		    cout << "Position:       ";
		    PrintPosition (pDict, position);
		    cout << endl;

		    HRESULT retStatus;
		    contractID_t cid;
		    retStatus = PersonnelResourceGetContractID (personnelResource,
								     &cid);
		    if (retStatus == AAFRESULT_PROP_NOT_PRESENT) 
		    {
			    cout << "No ContractID defined" << endl;
		    } else if (SUCCEEDED(retStatus))
		    {
			    cout << "ContractID:     " << dec << cid << endl;
		    } else check (retStatus);

		    retStatus = PersonnelResourceGetActorNameBufLen (personnelResource,
										     &bufferLen);
		    if (retStatus == AAFRESULT_PROP_NOT_PRESENT) 
		    {
			    cout << "No ActorRole defined" << endl;
		    } else if (SUCCEEDED(retStatus))
		    {
			    numChars = bufferLen/sizeof(aafCharacter);
			    actorRole = new aafCharacter [numChars];
		      check(PersonnelResourceGetActorName (personnelResource,
										    actorRole,
										    bufferLen));
          cout << "ActorRole:      " << actorRole << endl;
		      delete[] actorRole;
          actorRole = NULL;
		    }
		    cout << endl;
		    personnelResource->Release();
		    personnelResource=NULL;
	    }

	    pMob->Release();
	    pMob=NULL;
    } else
    {
	    cout << "Did not find Admin Mob" << endl;
    }
    // done.
    pMobIter->Release();
    pMobIter=NULL;
    pDict->Release();
    pDict=NULL;
    pHead->Release();
    pHead=NULL;
    pFile->Close ();
    pFile->Release();
    pFile=NULL;
  }
  catch (...)
  {
    // cleanup on error...
    if (actorRole)
      delete [] actorRole;
    if (familyName)
      delete [] givenName;
    if (givenName)
      delete [] givenName;
    if (personnelResource)
      personnelResource->Release();
    if (pMobCD)
      pMobCD->Release();
    if (pObject)
      pObject->Release();
    if (pMob)
      pMob->Release();
    if (pMobIter)
      pMobIter->Release();
    if (pDict)
      pDict->Release();
    if (pHead)
      pHead->Release();
    if (pFile)
    {
      pFile->Close();
      pFile->Release();
    }

    throw;
  }

  return true;
}

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

#include "AAFPersonnelExtension.h"


#include "extensionReadPlugin.h"
#include "extensionUtils.h"
#include "AAFResult.h"


#include <iostream.h>
#include <assert.h>


#ifndef __sgi
#define AAFPERSONNELEXTENSION_DLLNAME L"AAFPersonnelExtension.dll"
#else
#define AAFPERSONNELEXTENSION_DLLNAME L"libAAFPersonnelExtension.so"
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

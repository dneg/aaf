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


#include "extensionRead.h"
#include "extensionUtils.h"

#include <iostream.h>
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

  cout << "***Reading file " << filename << "***" << endl;

  IAAFFile *pFile=NULL;
  check (AAFFileOpenExistingRead ((aafCharacter*) filename,
								  0,
								  &pFile));
  
  IAAFHeader *pHead=NULL;
  check (pFile->GetHeader(&pHead));

  IAAFDictionary *pDict=NULL;
  check (pHead->GetDictionary(&pDict));

  IAAFTypeDef *ptd=NULL;
  IAAFClassDef *pcd=NULL;

  cout << "Verifying role enum type has been registered." << endl;
  check (pDict->LookupType ((aafUID_t*) &kTypeID_eRole, &ptd));
  ptd->Release();
  ptd=NULL;

  cout << "Verifying PersonnelResource class has been registered." << endl;
  check (pDict->LookupClass ((aafUID_t*) &kClassID_PersonnelResource, &pcd));
  pcd->Release();
  pcd=NULL;

  cout << "Verifying PersonnelMob class has been registered." << endl;
  check (pDict->LookupClass ((aafUID_t*) &kClassID_PersonnelMob, &pcd));
  pcd->Release();
  pcd=NULL;

  cout << "Verifying PersonnelResourceReference type has been"
	   << " registered." << endl;
  check (pDict->LookupType ((aafUID_t*) &kTypeID_PersonnelResourceStrongReference,
							&ptd));
  ptd->Release();
  ptd=NULL;

  cout << "Verifying PersonnelResourceReferenceVector type has been"
	   << " registered." << endl; 
  check (pDict->LookupType ((aafUID_t*) &kTypeID_PersonnelResourceStrongReferenceVector,
							&ptd));
  ptd->Release();
  ptd=NULL;

  cout << "Verifying PersonnelMob instance has been created and added" 
	   << " to header." << endl;
  IAAFMob *pMob=NULL;
  check (pHead->LookupMob ((aafUID_t*) &kMobID_Personnel,
						   &pMob));

  cout << "Printing contents of PersonnelMob." << endl;
  PrintPersonnelResources (pDict, pMob);

  // done.
  pFile->Close ();
  pFile->Release();
  pFile=NULL;
  pHead->Release();
  pHead=NULL;
  pDict->Release();
  pDict=NULL;
  pMob->Release();
  pMob=NULL;
  return true;
}

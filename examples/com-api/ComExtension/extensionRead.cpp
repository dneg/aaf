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
// See comments in extension.cpp for a discussion of what we're trying
// to do and how we're doing it.
//

bool extensionRead (const aafCharacter * filename)
{
  assert (filename);

  cout << "***Reading file " << filename << "***" << endl;

  IAAFFileSP pFile;
  check (AAFFileOpenExistingRead ((aafCharacter*) filename,
								  0,
								  &pFile));
  
  IAAFHeaderSP pHead;
  check (pFile->GetHeader(&pHead));

  IAAFDictionarySP pDict;
  check (pHead->GetDictionary(&pDict));

  IAAFTypeDefSP ptd;
  IAAFClassDefSP pcd;

  cout << "Verifying role enum type has been registered." << endl;
  check (pDict->LookupType ((aafUID_t*) &kTypeID_eRole, &ptd));

  cout << "Verifying PersonnelResource class has been registered." << endl;
  check (pDict->LookupClass ((aafUID_t*) &kClassID_PersonnelResource, &pcd));

  cout << "Verifying PersonnelMob class has been registered." << endl;
  check (pDict->LookupClass ((aafUID_t*) &kClassID_PersonnelMob, &pcd));

  cout << "Verifying PersonnelResourceReference type has been"
	   << " registered." << endl;
  check (pDict->LookupType ((aafUID_t*) &kTypeID_PersonnelResourceStrongReference,
							&ptd));

  cout << "Verifying PersonnelResourceReferenceVector type has been"
	   << " registered." << endl; 
  check (pDict->LookupType ((aafUID_t*) &kTypeID_PersonnelResourceStrongReferenceVector,
							&ptd));

  cout << "Verifying PersonnelMob has been created and added to"
	   << " header." << endl;
  IAAFMobSP pMob;
  check (pHead->LookupMob ((aafUID_t*) &kMobID_Personnel,
						   &pMob));

  /*
  cout << "Printing contents of PersonnelMob." << endl;

  aafUInt32 numPersonnel = 
	PersonnelMobGetArraySize (pMob);
  cout << "There are " << numPersonnel << " personnel registered:"
	   << endl;

  IAAFObject ** personnelArray = 0;
  personnelArray = new IAAFObject*[numPersonnel];
  assert (personnelArray);
  aafUInt32 i;
  for (i = 0; i < numPersonnel; i++)
	personnelArray[i] = 0;

  if (numPersonnel)
	PersonnelMobGetArray (pMob, personnelArray, numPersonnel);

  for (i = 0; i < numPersonnel; i++)
	{
	  assert (personnelArray[i]);
	  cout << endl;
	  PersonnelResource r =
		PersonnelRecordGetInfo (personnelArray[i]);
	  PrintPersonnelResource (pDict, r);
	}
  */

  pFile->Close ();

  return true;
}

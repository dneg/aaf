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

#include <stdlib.h>

#include "AAF.h"

#include "extensionUtils.h"
#include "extensionRead.h"

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
//     String    name; 
//     eRole     role; 
//     aafUInt32 contractID;
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

#include "extensionWrite.h"

int main ()
{
  const aafWChar * filename = L"extension.aaf";

  extensionWrite (filename);
  bool status = extensionRead (filename);
  if (! status)
	{
	  cout << "Error dumping "
		   << filename
		   << "." << endl;

	  exit (EXIT_FAILURE);
	}
  else
	{
	  cout << "Succeeded." << endl;
	  exit (EXIT_SUCCESS);
	}


  return EXIT_SUCCESS;
}

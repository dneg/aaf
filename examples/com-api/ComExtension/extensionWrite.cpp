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
// Creates a type definition to describe ePosition enumerations, and
// registers it in the dictionary.
//
static void CreateAndRegisterPositionEnum (IAAFDictionary * pDict)
{
  assert (pDict);

  // Instantiate a type definition object which will describe ePosition
  // extensible enumerations.
  IAAFTypeDefExtEnum *ptde;
  check (pDict->CreateInstance (AUID_AAFTypeDefExtEnum,
								IID_IAAFTypeDefExtEnum,
								(IUnknown**) &ptde));

  // Initialize the type definition object with the given name, and to
  // be represented by the given AUID.  We've already generated an
  // auid (kTypeID_ePosition) to represent this type defintion.
  check (ptde->Initialize (kTypeID_ePosition,
						   L"PersonnelPosition"));

  // Pre-register a few element values, along with their names.  We've
  // already generated AUIDs to represent the values (kPosition_XXX).
  check (ptde->AppendElement (kPosition_Producer,     L"Producer"));
  check (ptde->AppendElement (kPosition_Editor,       L"Editor"));
  check (ptde->AppendElement (kPosition_FloorManager, L"FloorManager"));
  check (ptde->AppendElement (kPosition_Actor, L"Actor"));

  // Register this type definition in the dictionary.  The
  // dictionary::RegisterType() method expects an IAAFTypeDef pointer,
  // so we'll QI for that first.
  IAAFTypeDef *ptd;
  check (ptde->QueryInterface (IID_IAAFTypeDef, (void **)&ptd));
  check (pDict->RegisterType (ptd));
  ptde->Release();
  ptde=NULL;
  ptd->Release();
  ptd=NULL;
}


//
// Creates a class definition to describe PersonnelResource objects,
// and registers it in the dictionary.
//
static void CreateAndRegisterPersonnelResource (IAAFDictionary * pDict)
{
  assert (pDict);

  // Instantiate a class definition object which will describe
  // PersonnelResource objects.
  IAAFClassDef *pcd=NULL;
  check (pDict->CreateInstance (AUID_AAFClassDef,
								IID_IAAFClassDef,
								(IUnknown**) &pcd));

  // We'll have to specify the PersonnelResource class definition's
  // parent class, so look it up here.  Since PersonnelResource
  // inherits directly from AAFObject, look up the AAFObject class
  // definition.
  IAAFClassDef *pcd_Object=NULL;
  check (pDict->LookupClass (AUID_AAFObject, &pcd_Object));

  // Now initialize our new PersonnelResource class definition.  We'll
  // specify an AUID with which it will be identified
  // (kClassID_PersonnelResource, which we've already generated); the
  // parent class (specified by the class definition for AAFObject,
  // which we've just looked up), and a name for this class.
  check (pcd->Initialize (kClassID_PersonnelResource,
						  pcd_Object,
						  L"PersonnelResource"));

  //
  // Now we add definitions for the properties that this class will
  // contain.  Note that each property is described by a type, which
  // must be specified at the time the property def is appended.  Note
  // also that we'll be specifying some mandatory properties; it is
  // required that these be specified *before* the class is registered
  // (using the AppendNewPropertyDef() method).
  //

  // Appending property defs returns the newly-created property
  // definition; declare a variable where the Append method can put
  // the new prop def.  However we won't use the prop def in this
  // function.
  IAAFPropertyDef *pd_unused=NULL;

  // Add the FamilyName and GivenName properties to the PersonnelRecord class defintion.
  // The type needed to define these is String, so we'll need to specify
  // the type definition describing String when we append the prop
  // def.  First, look up the type definition describing String.
  IAAFTypeDef *ptd_String=NULL;
  check (pDict->LookupType (kAAFTypeID_String,
							&ptd_String));

  // Now request that a new property def be appended to this class
  // def.  We're specifying an AUID by which this property will be
  // known (kPropID_PersonnelResource_FamilyName, which we've already
  // generated), a name, the type of the property, and that the
  // property is mandatory.  The returned property def is ignored
  // since we won't use it in this function.
 check (pcd->AppendNewPropertyDef (kPropID_PersonnelResource_GivenName,
									L"GivenName",
									ptd_String,
									AAFFalse,	// mandatory
									&pd_unused));
  pd_unused->Release();
  pd_unused=NULL;
  check (pcd->AppendNewPropertyDef (kPropID_PersonnelResource_FamilyName,
									L"FamilyName",
									ptd_String,
									AAFFalse,	// mandatory
									&pd_unused));
  pd_unused->Release();
  pd_unused=NULL;

  // Add the Position property to the PersonnelRecord class defintion.
  // The type of the Position property is ePosition (an extensible enumeration
  // we've just registered), so we'll need to specify the type
  // definition describing ePosition when we append the prop def.  First,
  // look up the type definition describing ePosition.
  IAAFTypeDef *ptd_Position=NULL;

  // Now request that a new property def be appended to this class
  // def.  We're specifying an AUID by which this property will be
  // known (kPropID_PersonnelResource_Position, which we've already
  // generated), a name, the type of the property, and that the
  // property is mandatory.  The returned property def is ignored
  // since we won't use it in this function.
  check (pDict->LookupType (kTypeID_ePosition, &ptd_Position));
  check (pcd->AppendNewPropertyDef (kPropID_PersonnelResource_Position,
									L"Position",
									ptd_Position,
									AAFFalse,   // mandatory
									&pd_unused));
  pd_unused->Release();
  pd_unused=NULL;

  // Add the Contract ID property to the PersonnelRecord class
  // defintion.  The type of the Contract ID property is UInt32, so
  // we'll need to specify the type definition describing UInt32 when
  // we append the prop def.  First, look up the type definition
  // describing UInt32.
  IAAFTypeDef *ptd_ui32=NULL;
  check (pDict->LookupType (kAAFTypeID_UInt32, &ptd_ui32));

  // Now request that a new property def be appended to this class
  // def.  We're specifying an AUID by which this property will be
  // known (kPropID_PersonnelResource_ContractID, which we've already
  // generated), a name, the type of the property, and that the
  // property is optional.  The returned property def is ignored
  // since we won't use it in this function.
  check (pcd->AppendNewPropertyDef (kPropID_PersonnelResource_ContractID,
									L"ContractID",
									ptd_ui32,
									AAFTrue,    // optional
									&pd_unused));
  pd_unused->Release();
  pd_unused=NULL;

  // Now request that a new property def be appended to this class
  // def.  We're specifying an AUID by which this property will be
  // known (kPropID_PersonnelResource_Role, which we've already
  // generated), a name, the type of the property, and that the
  // property is optional.  The returned property def is ignored
  // since we won't use it in this function.
  check (pcd->AppendNewPropertyDef (kPropID_PersonnelResource_Role,
									L"Role",
									ptd_String,
									AAFTrue,    // optional
									&pd_unused));
  pd_unused->Release();
  pd_unused=NULL;

  // Now that the class definition describing PersonnelResource is
  // initialized and its properties have been specified, we can
  // register that class definition.
  check (pDict->RegisterClass (pcd));
  pcd->Release();
  pcd=NULL;
  pcd_Object->Release();
  pcd_Object=NULL;
  ptd_String->Release();
  ptd_String=NULL;
  ptd_Position->Release();
  ptd_Position=NULL;
  ptd_ui32->Release();
  ptd_ui32=NULL;

}


//
// Create and register a type definition describing a reference to a
// PersonnelResource object.
//
static void
CreateAndRegisterPersonnelResourceReference
(IAAFDictionary * pDict)
{
  assert (pDict);

  // When we create the type definition, we'll need to describe which
  // objects it is permissible to reference using this type.  In this
  // case, we want to specify that we're referencing objects of class
  // PersonnelResource.  Therefore we'll have to look up the class
  // describing PersonnelResource objects for use below.
  IAAFClassDef *pcd=NULL;
  check (pDict->LookupClass (kClassID_PersonnelResource,
							 &pcd));

  // Instantiate a TypeDefinition for use as a Reference to a
  // PersonnelResource object.  We'll instantiate a
  // TypeDefinitionStrongObjectReference.
  IAAFTypeDefObjectRef *ptdr=NULL;
  check (pDict->CreateInstance (AUID_AAFTypeDefStrongObjRef,
								IID_IAAFTypeDefObjectRef,
								(IUnknown**) &ptdr));

  // Initialize our new type def, identifying the given AUID by which
  // this type will be known
  // (kTypeID_PersonnelResourceStrongReference, which we've already
  // generated), the class definition describing the types of objects
  // to which we'll refer (the ClassDef descriging PersonnelResource
  // objects), and a name for this type.
  check (ptdr->Initialize (kTypeID_PersonnelResourceStrongReference,
						   pcd,
						   L"PersonnelResourceStrongReference"));

  // Register this type definition in the dictionary.  The
  // dictionary::RegisterType() method expects an IAAFTypeDef pointer,
  // so we'll QI for that first.
  IAAFTypeDef *ptd=NULL;
  check (ptdr->QueryInterface (IID_IAAFTypeDef, (void **)&ptd));
  check (pDict->RegisterType (ptd));
  pcd->Release();
  pcd=NULL;
  ptdr->Release();
  ptdr=NULL;
  ptd->Release();
  ptd=NULL;
}


//
// Create and register a type definition describing a vector of
// references to PersonnelResource objects.
//
static void
CreateAndRegisterPersonnelResourceReferenceVector
(IAAFDictionary * pDict)
{
  assert (pDict);

  // Instantiate a TypeDefinition for use as a Vector of References to
  // PersonnelResource objects.  We'll instantiate a
  // TypeDefinitionVariableArray.
  IAAFTypeDefVariableArray *ptdv=NULL;
  check (pDict->CreateInstance (AUID_AAFTypeDefVariableArray,
								IID_IAAFTypeDefVariableArray,
								(IUnknown**) &ptdv));

  // We'll need to describe the type of each element in this vector.
  // In this case, we want to specify that elements are
  // StrongReferenceToPersonnelRecord objects.  Therefore we'll have
  // to look up the type describing StrongReferenceToPersonnelRecord
  // for use below.
  IAAFTypeDef *ptdr=NULL;
  check (pDict->LookupType (kTypeID_PersonnelResourceStrongReference,
							&ptdr));

  // Initialize our new type def, identifying the given AUID by which
  // this type will be known
  // (kTypeID_PersonnelResourceStrongReferenceVector, which we've
  // already generated), the type definition describing the type of
  // elements in this vector (the TypeDef descriging
  // StrongReferenceToPersonnelResource objects), and a name for this
  // type.
  check (ptdv->Initialize (kTypeID_PersonnelResourceStrongReferenceVector,
						   ptdr,
						   L"PersonnelResourceStrongReferenceVector"));

  // Register this type definition in the dictionary.  The
  // dictionary::RegisterType() method expects an IAAFTypeDef pointer,
  // so we'll QI for that first.
  IAAFTypeDef *ptd=NULL;
  check (ptdv->QueryInterface (IID_IAAFTypeDef, (void **)&ptd));
  check (pDict->RegisterType (ptd));
  ptd->Release();
  ptd=NULL;
  ptdr->Release();
  ptdr=NULL;
  ptdv->Release();
  ptdv=NULL;

}


//
// Creates a class definition to describe AdminMob objects, and
// registers it in the dictionary.
//
static void CreateAndRegisterAdminMob (IAAFDictionary * pDict)
{
  assert (pDict);

  // Instantiate a class definition object which will describe
  // AdminMob objects.
  IAAFClassDef *pcd=NULL;
  check (pDict->CreateInstance (AUID_AAFClassDef,
								IID_IAAFClassDef,
								(IUnknown**) &pcd));

  // We'll have to specify the AdminMob class definition's parent
  // class, so look it up here.  Since PersonnelResource inherits from
  // AAFMob, look up the AAFMob class definition.
  IAAFClassDef *pcd_Mob=NULL;
  check (pDict->LookupClass (AUID_AAFMob, &pcd_Mob));

  // Now initialize our new AdminMob class definition.  We'll
  // specify an AUID with which it will be identified
  // (kClassID_AdminMob, which we've already generated); the
  // parent class (specified by the class definition for AAFMob, which
  // we've just looked up), and a name for this class.
  check (pcd->Initialize (kClassID_AdminMob,
						  pcd_Mob,
						  L"AdminMob"));

  //
  // Now we add a definition for the property that this class will
  // contain.  Note that each property is described by a type, which
  // must be specified at the time the property def is appended.  Note
  // also that this will be a mandatory property; it is required that
  // this be specified *before* the class is registered (using the
  // AppendNewPropertyDef() method).
  //

  // Appending property defs returns the newly-created property
  // definition; declare a variable where the Append method can put
  // the new prop def.  However we won't use the prop def in this
  // function.
  IAAFPropertyDef *pd_unused=NULL;

  // Add the Personnel property to the AdminMob class defintion.
  // The type of the Personnel property is
  // Vector-of-strong-reference-to-PesonnelRecord, so we'll need to
  // specify the type definition describing that kind of vector when
  // we append the prop def.  First, look up that type definition.
  IAAFTypeDef *ptd_PersonnelVector=NULL;
  check (pDict->LookupType (kTypeID_PersonnelResourceStrongReferenceVector,
							&ptd_PersonnelVector));

  // Now request that a new property def be appended to this class
  // def.  We're specifying an AUID by which this property will be
  // known (kPropID_AdminMob_Personnel, which we've already
  // generated), a name, the type of the property, and that the
  // property is mandatory.  The returned property def is ignored
  // since we won't use it in this function.
  check (pcd->AppendNewPropertyDef (kPropID_AdminMob_Personnel,
									L"Personnel",
									ptd_PersonnelVector,
									AAFFalse,
									&pd_unused));
  pd_unused->Release();
  pd_unused=NULL;

  // Now that the class definition describing AdminMob is
  // initialized and its properties have been specified, we can
  // register that class definition.
  check (pDict->RegisterClass (pcd));
  pcd->Release();
  pcd=NULL;
  pcd_Mob->Release();
  pcd_Mob=NULL;
  ptd_PersonnelVector->Release();
  ptd_PersonnelVector=NULL;

}


//
// Creates a PersonnelResource object through the given dictionary,
// and initializes it with the given info.  It is then appended to the
// given AdminMob, in its Personnel property (which is an array of
// object references to PersonnelResource objects).
//
// These are the steps:
// - Create the PersonnelResource object.
// - Set the values in the newly-created PersonnelResource object.
// - Create a object reference PropertyValue to refer to the new
//   PersonnelResource object.
// - Get the PropertyValue for the current value of the Personnel
//   array from the AdminMob.
// - Append the new PropertyValue (which references the new
//   PersonnelResource object) to the existing PropertyValue of the
//   Personnel vector from the AdminMob.
// - Set the value of the Personnel resource in the AdminMob to
//   the newly-appended PropertyValue.
//

//
// Creates a PersonnelResource object through the given dictionary,
// and initializes it with the given info.  It is then appended to the
// given AdminMob, in its Personnel property (which is an array of
// object references to PersonnelResource objects).
//
// These are the steps:
// - Create the PersonnelResource object.
// - Set the values in the newly-created PersonnelResource object.
// - Create a object reference PropertyValue to refer to the new
//   PersonnelResource object.
// - Get the PropertyValue for the current value of the Personnel
//   array from the AdminMob.
// - Append the new PropertyValue (which references the new
//   PersonnelResource object) to the existing PropertyValue of the
//   Personnel vector from the AdminMob.
// - Set the value of the Personnel resource in the AdminMob to
//   the newly-appended PropertyValue.
//

static void DefineResourceClassExtensions(IAAFDictionary *pDict)
{
  // Need to check if already defined.
  assert(pDict);
  // Register the extensible enumeration describing Position in the
  // dictionary.
  CreateAndRegisterPositionEnum (pDict);

  // Create a class definition describing PesonnelResource objects and
  // register it in the dictionary.
  CreateAndRegisterPersonnelResource (pDict);

  // Create a type definition describing references to
  // PersonnelResource objects, and register it.
  CreateAndRegisterPersonnelResourceReference (pDict);

  // Create a type definition describing vectors of references to
  // PersonnelResource objects, and register it.
  CreateAndRegisterPersonnelResourceReferenceVector (pDict);

  // Create a class definition describing AdminMob objects, and
  // register it.
  CreateAndRegisterAdminMob (pDict);


}






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
  
  IAAFFile *pFile=NULL;
  check (AAFFileOpenNewModify ((aafCharacter*) filename,
							   0,
							   &ProductInfo,
							   &pFile));
  
  IAAFHeader *pHead=NULL;
  check (pFile->GetHeader(&pHead));

  IAAFDictionary *pDict=NULL;
  check (pHead->GetDictionary(&pDict));

  DefineResourceClassExtensions(pDict);

 
  // Instantiate a AdministrativeMob object.
  IAAFMob *pAdminMob=NULL;
  check (pDict->CreateInstance (kClassID_AdminMob,
								IID_IAAFMob,
								(IUnknown**) &pAdminMob));
  check (pAdminMob->SetName (L"Administrative Information"));
 

  // Add the new AdministrativeMob object to the file's header.
  check (pHead->AppendMob (pAdminMob));

  // Add several PersonnelResource objects to the AdminMob.
  // Instantiate the PersonnelResource object.
  IAAFObject *pPersResource=NULL;
  check (pDict->CreateInstance (kClassID_PersonnelResource,
								IID_IAAFObject,
								(IUnknown**) &pPersResource));

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
  check (pDict->CreateInstance (kClassID_PersonnelResource,
								IID_IAAFObject,
								(IUnknown**) &pPersResource));

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
  check (pDict->CreateInstance (kClassID_PersonnelResource,
								IID_IAAFObject,
								(IUnknown**) &pPersResource));

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

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
//     String       name; 
//     eRole        role; 
//
//     // contract ID is optional
//     bool         cid_present;
//     contractID_t cid;
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
// Creates a type definition to describe eRole enumerations, and
// registers it in the dictionary.
//
static void CreateAndRegisterRoleEnum (IAAFDictionary * pDict)
{
  assert (pDict);

  // Instantiate a type definition object which will describe eRole
  // extensible enumerations.
  IAAFTypeDefExtEnum *ptde;
  check (pDict->CreateInstance (AUID_AAFTypeDefExtEnum,
								IID_IAAFTypeDefExtEnum,
								(IUnknown**) &ptde));

  // Initialize the type definition object with the given name, and to
  // be represented by the given AUID.  We've already generated an
  // auid (kTypeID_eRole) to represent this type defintion.
  check (ptde->Initialize (kTypeID_eRole,
						   L"PersonnelRole"));

  // Pre-register a few element values, along with their names.  We've
  // already generated AUIDs to represent the values (kRole_XXX).
  check (ptde->AppendElement (kRole_Producer,     L"Producer"));
  check (ptde->AppendElement (kRole_Editor,       L"Editor"));
  check (ptde->AppendElement (kRole_FloorManager, L"FloorManager"));
  check (ptde->AppendElement (kRole_Actor, L"Actor"));

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

  // Add the Name property to the PersonnelRecord class defintion.
  // The type of the Name property is String, so we'll need to specify
  // the type definition describing String when we append the prop
  // def.  First, look up the type definition describing String.
  IAAFTypeDef *ptd_String=NULL;
  check (pDict->LookupType (kAAFTypeID_String,
							&ptd_String));

  // Now request that a new property def be appended to this class
  // def.  We're specifying an AUID by which this property will be
  // known (kPropID_PersonnelResource_Name, which we've already
  // generated), a name, the type of the property, and that the
  // property is mandatory.  The returned property def is ignored
  // since we won't use it in this function.
  check (pcd->AppendNewPropertyDef (kPropID_PersonnelResource_Name,
									L"Name",
									ptd_String,
									AAFFalse,	// mandatory
									&pd_unused));
  pd_unused->Release();
  pd_unused=NULL;

  // Add the Role property to the PersonnelRecord class defintion.
  // The type of the Role property is eRole (an extensible enumeration
  // we've just registered), so we'll need to specify the type
  // definition describing eRole when we append the prop def.  First,
  // look up the type definition describing eRole.
  IAAFTypeDef *ptd_Role=NULL;

  // Now request that a new property def be appended to this class
  // def.  We're specifying an AUID by which this property will be
  // known (kPropID_PersonnelResource_Role, which we've already
  // generated), a name, the type of the property, and that the
  // property is mandatory.  The returned property def is ignored
  // since we won't use it in this function.
  check (pDict->LookupType (kTypeID_eRole, &ptd_Role));
  check (pcd->AppendNewPropertyDef (kPropID_PersonnelResource_Role,
									L"Role",
									ptd_Role,
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
  ptd_Role->Release();
  ptd_Role=NULL;
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

static void ResourceInitialize(IAAFObject *pResource,
							   aafCharacter *familyName,
							   aafCharacter *givenName,
							   eJobFunction jobFunction)
{
	assert (pResource);
}

static void ResourceSetActorRole (IAAFObject *pResource,
								  aafCharacter *role)
{
	assert (pResource);
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
static void AdminMobAppendResource (IAAFDictionary *pDict,
									IAAFMob *pAdminMob,
							        IAAFObject *pResource)
{
  assert (pAdminMob);
  assert (pResource);



  // Create a property value corresponding to new object reference.
  // We'll have to do that through the type def for that kind of obj
  // ref.

  // Look up the type def for StrongReference-to-PersonnelResource
  // objects.
  IAAFTypeDef *td=NULL;
  check (pDict->LookupType (kTypeID_PersonnelResourceStrongReference,
							&td));
  
  // Create the StrongReference-to-PersonnelResource property value
  // through the type def.  This must be done through the
  // IAAFTypeDefObjectRef interface, so QI for that first.
  IAAFTypeDefObjectRef *tdo=NULL;
  check (td->QueryInterface (IID_IAAFTypeDefObjectRef,
							 (void **)&tdo));

  // Create the value.  The CreateValue() method takes as argument the
  // object to which this value will refer.
  IAAFPropertyValue *pv=NULL;
  check (tdo->CreateValue (pResource, &pv));

  // Append the new property value (which refers to the new
  // PersonnelRecord object) to the existing array of PersonnelRecord
  // object references.

  // First get the existing array-of-objects property value from the
  // AdminMob object.  To do so, we'll need:
  // - The IAAFObject interface from the AdminMob object.
  // - Using the IAAFObject interface, we obtain the ClassDefinition
  //   describing this AdminMob object.
  // - From that class definition, we look up the PropertyDef for the
  //   Personnel property on AdminMob.
  // - We use PropertyDef to specify which property value we're
  //   getting, and get the property.

  // QI for the AdminMob's IAAFObject interface
  IAAFObject *pMobObj=NULL;
  check (pAdminMob->QueryInterface (IID_IAAFObject,
							   (void **)&pMobObj));

  // Use the IAAFObject interface to obtain the class definition
  // describing the AdminMob.
  IAAFClassDef *cd=NULL;
  check (pMobObj->GetDefinition (&cd));

  // Use the ClassDefinition to look up the PropertyDefinition for the
  // Personnel property on the Personnel mob.
  IAAFPropertyDef *pd=NULL;
  check (cd->LookupPropertyDef (kPropID_AdminMob_Personnel,
								&pd));

  // Use the PropertyDefinition to specify the property whose value we
  // wish to obtain.
  IAAFPropertyValue *pva=NULL;
  check (pMobObj->GetPropertyValue (pd, &pva));

  // Now get a type def for the array of personnel, and use that type
  // def to append the new personnel prop val to it.
  IAAFTypeDef *tdpv=NULL;
  check (pva->GetType (&tdpv));
  IAAFTypeDefVariableArray *tda=NULL;
  check (tdpv->QueryInterface (IID_IAAFTypeDefVariableArray,
							   (void **)&tda));
  check (tda->AppendElement (pva, pv));

  // Now that the property value has had the new element appended to
  // it, set the property in the AdminMob to its new value.
  check (pMobObj->SetPropertyValue (pd, pva));
  td->Release();
  td=NULL;
  tdo->Release();
  tdo=NULL;
  pv->Release();
  pv=NULL;
  pMobObj->Release();
  pMobObj=NULL;
  cd->Release();
  cd=NULL;
  pd->Release();
  pd=NULL;
  pva->Release();
  pva=NULL;
  tdpv->Release();
  tdpv=NULL;
  tda->Release();
  tda=NULL;

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

static void DefineResourceClassExtensions(IAAFDictionary *pDict)
{
  // Need to check if already defined.
  assert(pDict);
  // Register the extensible enumeration describing Role in the
  // dictionary.
  CreateAndRegisterRoleEnum (pDict);

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

static void InitializePersonnelResource (IAAFObject *pPersResource,
										 aafCharacter *familyName,
										 aafCharacter *givenName,
										 eJobFunction jobFunction)
{
	assert(pPersResource);
	PersonnelRecordSetName (pPersResource,familyName);
	PersonnelRecordSetJobFunction(pPersResource,jobFunction);

}

static void SetRole(IAAFObject *pPersResource,
					aafCharacter *role)
{
	assert(pPersResource);
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

  IAAFClassDef *persResourceCD=NULL;
  check (pDict->LookupClass (kClassID_PersonnelResource, &persResourceCD));
  persResourceCD->Release();
  persResourceCD=NULL;


 
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

  InitializePersonnelResource (pPersResource,
							   L"Morgan",
							   L"Oliver",
							   kRole_FloorManager);
  AdminMobAppendResource (pDict,
						  pAdminMob,
						  pPersResource);
  pPersResource->Release();
  pPersResource=NULL;

// Instantiate the PersonnelResource object.
  check (pDict->CreateInstance (kClassID_PersonnelResource,
								IID_IAAFObject,
								(IUnknown**) &pPersResource));

  InitializePersonnelResource (pPersResource,
							   L"Ohanian",
							   L"Tom",
							   kRole_Editor);
  AdminMobAppendResource (pDict,
						  pAdminMob,
						  pPersResource);
  pPersResource->Release();
  pPersResource=NULL;
// Instantiate the PersonnelResource object.
  check (pDict->CreateInstance (kClassID_PersonnelResource,
								IID_IAAFObject,
								(IUnknown**) &pPersResource));

  InitializePersonnelResource (pPersResource,
							   L"Oldman",
							   L"Arianna",
							   kRole_Actor);
  SetRole(pPersResource, L"Lucy");
  AdminMobAppendResource (pDict,
						  pAdminMob,
						  pPersResource);
  pPersResource->Release();
  pPersResource=NULL;
  // Save the file and close it.
  check (pFile->Save());
  check (pFile->Close());
  pFile->Release();
  pFile=NULL;
  pHead->Release();
  pHead=NULL;
  pDict->Release();
  pDict=NULL;
  pAdminMob->Release();
  pAdminMob=NULL;

}

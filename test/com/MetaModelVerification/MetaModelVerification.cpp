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
// The Original Code of this file is Copyright 2006, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Matt Beard / Metaglue Corporation.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#include <AAF.h>
#include "AAFTypeDefUIDs.h"
#include <AAFStoredObjectIDs.h>
#include <AAFClassDefUIDs.h>
#include "AAFPropertyDefs.h"
#include "AAFResult.h"
#include "ResultCodes.h"

#include <string>
#include <list>

#include <stdio.h>
#include <stdlib.h>


// Include our header file
#include "MetaModelVerification.h"


//! Product identification structure
aafProductIdentification_t TestIdent =
{ 
	L"freeMXF.org", L"TypeDef Tests", L"2.71828182818",
	{  0x87ba485e, 0x51db, 0x8ce5,	{0x8b, 0xd9, 0x26, 0xa3, 0xfc, 0xfe, 0x3d, 0xe1}},	0, 0
};


/* Definitions for test description set */
/****************************************/

//! Property ID of header property which is a strong reference to the test description object
const aafUID_t TestReferencePropID =  { 0x87ba465e, 0x25db, 0x8ce5, {0x8b, 0xd9, 0x26, 0xa3, 0xfc, 0xfe, 0x4d, 0xdf}};

//! Name of the header property which is a strong reference to the test description object
const aafString_t TestReferencePropName = L"Strong Reference to TypeDef Test Description Object";

//! Class ID for the test description class
const aafUID_t TestDescriptionClassID = { 0x87ba465e, 0x25db, 0x8ce5, {0x8b, 0xd9, 0x26, 0xa3, 0xfc, 0xfe, 0x4d, 0xe0}};

//! Name of the test description class
const aafString_t TestDescriptionClassName = L"TypeDef Test Property Description Class";

//! Property ID of test version number
const aafUID_t TestVersionPropID =  { 0x87ba465e, 0x25db, 0x8ce5, {0x8b, 0xd9, 0x26, 0xa3, 0xfc, 0xfe, 0x4d, 0xe1}};

//! Name of the test version number
const aafString_t TestVersionPropName = L"TypeDef Test Description Class Version Number";

//! Property ID of array of test property IDs
const aafUID_t TestPropsPropID =  { 0x87ba465e, 0x25db, 0x8ce5, {0x8b, 0xd9, 0x26, 0xa3, 0xfc, 0xfe, 0x4d, 0xe2}};

//! Name of the array of test property IDs
const aafString_t TestPropsPropName = L"List of IDs of properties added";

//! Property ID of test descriptions list string, each entry separated by TestStringSeparator
/*! /note The string starts with a TestStringSeparator character to show the reader what separation character is in use
 */
const aafUID_t TestNamesPropID =  { 0x87ba465e, 0x25db, 0x8ce5, {0x8b, 0xd9, 0x26, 0xa3, 0xfc, 0xfe, 0x4d, 0xe3}};

//! Name of the test descriptions list string
const aafString_t TestNamesPropName = L"List of names of properties added";


//! Our test version number
/*! This number is incremented every time a non-backward compatible change is made to the test values.
 *  The number is NOT changed when a test file from a new test app can be read and validated by an 
 *  older one with the same version number.
 */
const aafUInt32 TestVersion = 1;

//! The character we use to separate string in the test description class
const aafCharacter TestStringSeparator = '|';

//! The character we replace any occurances of TestStringSeparator with in a string added to the test description class
const aafCharacter TestStringSeparatorSubstitute = ':';

/*
//! An element in our list of properties added to the file
struct TestDescription_t
{
	aafUID_t PropertyID;						//!< The property ID of this test property
	std::string Name;							//!< A descriptive name for the test property
};

//! Our list of properties added to the file - this will be written to the test description class later
std::list<TestDescription_t> TestDescriptionList;
*/


//! Info for all type def classes
const ClassInfo ClassInfo::Array[] =
{
	ClassInfo("TypeDefCharacter", AUID_AAFTypeDefCharacter, IID_IAAFTypeDefCharacter, false, kAAFTypeID_Character),
	ClassInfo("TypeDefEnum", AUID_AAFTypeDefEnum, IID_IAAFTypeDefEnum, true, NULL_AUID),
	ClassInfo("TypeDefExtEnum", AUID_AAFTypeDefExtEnum, IID_IAAFTypeDefExtEnum, true, NULL_AUID),
	ClassInfo("TypeDefFixedArray", AUID_AAFTypeDefFixedArray, IID_IAAFTypeDefFixedArray, true, NULL_AUID),
	ClassInfo("TypeDefIndirect", AUID_AAFTypeDefIndirect, IID_IAAFTypeDefIndirect, false, kAAFTypeID_Indirect),
	ClassInfo("TypeDefInt", AUID_AAFTypeDefInt, IID_IAAFTypeDefInt, false, kAAFTypeID_Int32),
	ClassInfo("TypeDefOpaque", AUID_AAFTypeDefOpaque, IID_IAAFTypeDefOpaque, true, NULL_AUID),
	ClassInfo("TypeDefRecord", AUID_AAFTypeDefRecord, IID_IAAFTypeDefRecord, true, NULL_AUID),
	ClassInfo("TypeDefRename", AUID_AAFTypeDefRename, IID_IAAFTypeDefRename, true, NULL_AUID),
	ClassInfo("TypeDefSet", AUID_AAFTypeDefSet, IID_IAAFTypeDefSet, true, NULL_AUID),
	ClassInfo("TypeDefStream", AUID_AAFTypeDefStream, IID_IAAFTypeDefStream, false, kAAFTypeID_Stream),
	ClassInfo("TypeDefString", AUID_AAFTypeDefString, IID_IAAFTypeDefString, true, NULL_AUID),
	ClassInfo("TypeDefStrongObjRef", AUID_AAFTypeDefStrongObjRef, IID_IAAFTypeDefStrongObjRef, true, NULL_AUID),
	ClassInfo("TypeDefVariableArray", AUID_AAFTypeDefVariableArray, IID_IAAFTypeDefVariableArray, true, NULL_AUID),
	ClassInfo("TypeDefWeakObjRef", AUID_AAFTypeDefWeakObjRef, IID_IAAFTypeDefWeakObjRef, true, NULL_AUID),
};

//! Number of entries in Array
const int ClassInfo::ArrayCount = sizeof(ClassInfo::Array) / sizeof(ClassInfo::Array[0]);


//! List of class tests to perform
/*! The first item in each ClassTest is the class to test, the following items are the classes that can
 *  validly be used in the initialization of that class - for example AUID_AAFTypeDefEnum can only be
 *  an enumeration with AUID_AAFTypeDefInt typed values.
 *  Those entries with only a class to test and no following sub-classes will be used to determine the list 
 *  of possible classes. This list will be used to check that invalid class combinations will not be allowed, 
 *  for example attempting to build an AUID_AAFTypeDefEnum enumeration of AUID_AAFTypeDefStrongObjRef objects 
 *  should fail.
 */
const ClassTest ClassTests[] =
{
	ClassTest( AUID_AAFTypeDefCharacter ),
	ClassTest( AUID_AAFTypeDefEnum, AUID_AAFTypeDefInt ),
	ClassTest( AUID_AAFTypeDefExtEnum ),
	ClassTest( AUID_AAFTypeDefFixedArray, AUID_AAFTypeDefCharacter, AUID_AAFTypeDefEnum, AUID_AAFTypeDefExtEnum, 
										  AUID_AAFTypeDefFixedArray, AUID_AAFTypeDefInt, AUID_AAFTypeDefRecord ),
	ClassTest( AUID_AAFTypeDefIndirect ),
	ClassTest( AUID_AAFTypeDefInt ),
	ClassTest( AUID_AAFTypeDefOpaque ),
	ClassTest( AUID_AAFTypeDefRecord, AUID_AAFTypeDefCharacter, AUID_AAFTypeDefEnum, AUID_AAFTypeDefExtEnum, 
									  AUID_AAFTypeDefFixedArray, AUID_AAFTypeDefInt, AUID_AAFTypeDefRecord ),
	ClassTest( AUID_AAFTypeDefRename, AUID_AAFTypeDefCharacter, AUID_AAFTypeDefEnum, AUID_AAFTypeDefExtEnum, 
									  AUID_AAFTypeDefFixedArray, AUID_AAFTypeDefIndirect, AUID_AAFTypeDefInt, 
									  AUID_AAFTypeDefRecord, AUID_AAFTypeDefOpaque, AUID_AAFTypeDefRecord,
									  AUID_AAFTypeDefRename, AUID_AAFTypeDefSet, AUID_AAFTypeDefStream, 
									  AUID_AAFTypeDefString, AUID_AAFTypeDefStrongObjRef, AUID_AAFTypeDefVariableArray, 
									  AUID_AAFTypeDefWeakObjRef ),
	ClassTest( AUID_AAFTypeDefSet, AUID_AAFTypeDefStrongObjRef, AUID_AAFTypeDefWeakObjRef ),
	ClassTest( AUID_AAFTypeDefStream ),
	ClassTest( AUID_AAFTypeDefString, AUID_AAFTypeDefCharacter, AUID_AAFTypeDefEnum, AUID_AAFTypeDefExtEnum, 
									  AUID_AAFTypeDefInt ),
	ClassTest( AUID_AAFTypeDefStrongObjRef ),
	ClassTest( AUID_AAFTypeDefVariableArray, AUID_AAFTypeDefCharacter, AUID_AAFTypeDefEnum, AUID_AAFTypeDefExtEnum, 
											 AUID_AAFTypeDefFixedArray, AUID_AAFTypeDefInt, AUID_AAFTypeDefRecord, 
											 AUID_AAFTypeDefStrongObjRef, AUID_AAFTypeDefWeakObjRef ),
	ClassTest( AUID_AAFTypeDefWeakObjRef )
};

//! Number of entries in ClassInfo
const int ClassTestsCount = sizeof(ClassTests) / sizeof(ClassTests[0]);


//! Main Meta-Model Verification application
int main(int argc, char *argv[])
{
	// Set the default filename
	const std::string DefaultFilename = "MMVerify.aaf";
	std::string Filename = DefaultFilename;

	// Set options
	bool WriteTest = false;
	bool ReadTest = false;

	/* Parse command-line options */
	int i;
	for(i=1; i<argc; i++)
	{
		// If not a switch, set the filename
		if(argv[i][0] != '-')
		{
			Filename = argv[i];
		}
		else
		{
			if((argv[i][1] == 'w') || (argv[i][1] == 'W'))
			{
				WriteTest = true;
			}
			else if((argv[i][1] == 'r') || (argv[i][1] == 'R'))
			{
				ReadTest = true;
			}
			else if((argv[i][1] == 'v') || (argv[i][1] == 'V'))
			{
				SetVerbose(true);
			}
			else
			{
				printf("MetaModelVerification - AAF Test application\n\n");
				
				printf("Usage :  %s [options] [filename]\n\n", argv[0]);

				printf("Options : -w Perform write test only\n");
				printf("          -r Perform read test only\n");
				printf("          -v Set verbose mode\n\n");
				printf("Notes : By default both test will run.\n");
				printf("        Default filename is %s\n", DefaultFilename.c_str());
				
				return 1;
			}
		}
	}

	// Set default if neither test specified
	if((!WriteTest) && (!ReadTest))
	{
		WriteTest = true;
		ReadTest = true;
	}


	try
	{
		// Initialize the AAF library
		CAAFInitialize aafInit;
	
		IAAFDictionary* Dictionary = NULL;
		IAAFFile *File = NULL;
		IAAFHeader *Header = NULL;

		HRESULT hr = S_OK;
		if(WriteTest)
		{
			hr = CreateTestFile(Filename, &Dictionary, &File, &Header);
			if(FAILED(hr)) throw HRError(hr, "Failed to open test file");

			hr = RegisterRequiredItems(Dictionary);
			if(FAILED(hr)) throw HRError(hr, "Failed to register required internal items for test");

			printf("** Test Valid Combinations **\n\n");
			hr = TestValidClasses(Dictionary, ClassTests, ClassTestsCount);

			printf("\n\n** Test Invalid Combinations **\n\n");
			HRESULT hr2 = TestInvalidClasses(Dictionary, ClassTests, ClassTestsCount);

			// Preserve result from initial tests if not S_OK, else use result from second tests
			if(!FAILED(hr)) hr = hr2;

			if(!TypeList.empty())
			{
				IAAFObject *Object = NULL;

				try
				{
					hr2 = Header->QueryInterface(IID_IAAFObject, reinterpret_cast<void **>(&Object));

					if(!FAILED(hr2))
					{	
						printf("\n\n** Test Properties of New Types **\n\n");
						hr2 = TestTypeDefProperties(Dictionary, Object);
					}

					// Add the test description class
					AddDescription(Header);
				}
				catch(...)
				{
					hr2 = E_FAIL;

					ReleaseRefs(Header);
					ReleaseRefs(Object);
					ReleaseRefs(Dictionary);
					ReleaseRefs(File);

					throw;
				}

				File->Save();
				File->Close();

				ReleaseRefs(Header);
				ReleaseRefs(Object);
				ReleaseRefs(Dictionary);
				ReleaseRefs(File);
			}

			// Preserve result from initial tests if not S_OK, else use result from third tests
			if(!FAILED(hr)) hr = hr2;

			if(!FAILED(hr)) printf("All write tests passed\n\n");
			else printf("Write test failed with error code 0x%x %s\n\n", (int)hr, HRESULTtoString(hr).c_str());
		}

		/* Read Tests */

		if(ReadTest)
		{
			IAAFObject *Object = NULL;
			try
			{
				//! Open the test file
				hr = OpenTestFile(Filename, &Dictionary, &File, &Header);

				if(!FAILED(hr)) hr = Header->QueryInterface(IID_IAAFObject, reinterpret_cast<void **>(&Object));

				// Read the test description object from the file
				if(!FAILED(hr)) hr = ReadDescription(Header);

				if(!FAILED(hr))
				{	
					printf("\n\n** Verify the Read Properties **\n\n");
					hr = VerifyTypeDefProperties(Dictionary, Object);
				}

				File->Close();
			}
			catch(...)
			{
				hr = E_FAIL;

				ReleaseRefs(Header);
				ReleaseRefs(Object);
				ReleaseRefs(Dictionary);
				ReleaseRefs(File);

				throw;
			}

			if(!FAILED(hr)) printf("\nAll read tests passed\n\n");
			else printf("\nRead test failed with error code 0x%x %s\n\n", (int)hr, HRESULTtoString(hr).c_str());

			ReleaseRefs(Header);
			ReleaseRefs(Object);
			ReleaseRefs(Dictionary);
			ReleaseRefs(File);
		}
	}

	
	/* Catch any errors and clean up */
	/*********************************/

	catch(HRESULT hr)
	{
		fprintf(stderr, "Error 0x%x %s\n", static_cast<int>(hr), HRESULTtoString(hr).c_str());
		return static_cast<int>(hr);
	}
	catch(const std::string &str)
	{
		fprintf(stderr, "Error \"%s\"\n", str.c_str());
		return 1;
	}
	catch(const char *str)
	{
		fprintf(stderr, "Error \"%s\"\n", str);
		return 1;
	}
	catch(HRError Err)
	{
		fprintf(stderr, "Error \"%s\"\n", Err.Text.c_str());
		fprintf(stderr, "Error code 0x%x %s\n", static_cast<int>(Err.hr), HRESULTtoString(Err.hr).c_str());
		return Err.hr;
	}
	catch(...)
	{
		fprintf(stderr, "Unhandled Exception!\n");
		throw;
		return 1;
	}

	// Clean exit
	return 0;
}


//! Try adding all valid combinations of type defs to a dictionary
HRESULT TestValidClasses(IAAFDictionary *Dict, const ClassTest *TestList, int TestCount)
{
	HRESULT hr = S_OK;

	//! A generic ID to use for the test type to add
	const aafUID_t kTestBase = { 0x3fbd2b9b, 0x24ed, 0x3b12, {0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} };

	//! A specific ID for this test type
	aafUID_t kTest;

	int Test;
	for(Test=0; Test<TestCount; Test++)
	{
		if(!TestList[Test].Tests.empty())
		{
			printf("Testing %s:\n", GetName(TestList[Test].Class).c_str());

			std::list<aafUID_t>::const_iterator it = TestList[Test].Tests.begin();
			while(it !=  TestList[Test].Tests.end())
			{
				printf("...with %s : ", GetName(*it).c_str());

				//! Make the ID unique
				kTest = RandomID(kTestBase);

				hr = RegisterType(Dict, kTest, TestList[Test].Class, *it);
				if(FAILED(hr))
				{
					printf("Failed: %s\n", HRESULTtoString(hr).c_str());
					hr = E_FAIL;
				}
				else
				{
					printf("OK\n");

					// Add this type to created types list
					TypeInfo Info;
					Info.Name = GetName(TestList[Test].Class) + " of " + GetName(*it);
					Info.ID = kTest;
					TypeList.push_back(Info);
				}

				it++;
			}

			printf("\n");
		}
	}

	return hr;
}


//! Try adding all invalid combinations of type defs to a dictionary
HRESULT TestInvalidClasses(IAAFDictionary *Dict, const ClassTest *TestList, int TestCount)
{
	HRESULT hr = S_OK;

	//! A generic ID to use for the test type to add
	const aafUID_t kTestBase = { 0x3fbd2b9b, 0x24ed, 0x3b13, {0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00} };

	//! A specific ID for this test type
	aafUID_t kTest;

	int Test;
	for(Test=0; Test<TestCount; Test++)
	{
		// Note that if there are no valid sub-classes there is no point trying as there will be no "Initialize" method
		if(!TestList[Test].Tests.empty())
		{
			printf("Testing %s:\n", GetName(TestList[Test].Class).c_str());

			// Use the list of test types as the classes to try as sub-classes
			int SubType;
			for(SubType=0; SubType<TestCount; SubType++)
			{
				// Check if this would be a valid combination
				bool ValidSub = false;

				// You can always find a valid rename type, so don't test it
				if(TestList[SubType].Class == AUID_AAFTypeDefRename) ValidSub = true;

				// Check if this is a valid sub (unles we have already figured out that it is)
				if(!ValidSub)
				{
					std::list<aafUID_t>::const_iterator it = TestList[Test].Tests.begin();
					while(it !=  TestList[Test].Tests.end())
					{
						// If we found this sub-class in the sub-types list then it is a valid combination
						if((*it) == TestList[SubType].Class)
						{
							ValidSub = true;
							break;
						}
						it++;
					}
				}

				// We are looking for invalid combinations, so only test if invalid
				if(!ValidSub)
				{
					printf("...with %s : ", GetName(TestList[SubType].Class).c_str());

					//! Make the ID unique
					kTest = RandomID(kTestBase);

					hr = RegisterType(Dict, kTest, TestList[Test].Class, TestList[SubType].Class);
					if(FAILED(hr))
					{
						printf("Correctly Failed\n");
					}
					else
					{
						printf("Incorrectly Succeded\n");
						hr = E_FAIL;
					}
				}
			}

			printf("\n");
		}
	}

	return hr;
}


//! Try using all new type defs by adding properties of each type to a given object
HRESULT TestTypeDefProperties(IAAFDictionary *Dict, IAAFObject *Object)
{
	// Overall "it worked" / "it failed" value
	HRESULT Ret = S_OK;

	// Current operation result
	HRESULT hr = S_OK;

	/* First we must add a property of each type to the object's class */

	// Get the class ID of the object
	aafUID_t ClassID;

	IAAFMetaDefinition *md_Object = NULL;
	IAAFClassDef *cd_Object = NULL;
	try
	{
		hr = Object->GetDefinition(&cd_Object);
		if(!FAILED(hr))	hr = cd_Object->QueryInterface(IID_IAAFMetaDefinition, reinterpret_cast<void **>(&md_Object));
		if(!FAILED(hr)) hr = md_Object->GetAUID(&ClassID);

		// Throw the error code into our catch clause if we got one
		if(FAILED(hr)) throw HRError(hr, "Failed");
	}
	catch(...)
	{
		ReleaseRefs(cd_Object);
		ReleaseRefs(md_Object);

		throw HRError("Failed to get ClassID");
	}
	ReleaseRefs(cd_Object);
	ReleaseRefs(md_Object);


	// Start with an empty property list
	PropertyList.clear();

	std::list<TypeInfo>::iterator it = TypeList.begin();
	while(it != TypeList.end())
	{
		printf("%s:\n", (*it).Name.c_str());

		DebugMessage("Adding a new property of type %s\n", (*it).Name.c_str());

		aafUID_t PropertyID = RandomID((*it).ID);

		try
		{
			hr = RegisterProperty(Dict, ClassID, (*it).ID, PropertyID, StringToAAFString((*it).Name));

			if(FAILED(hr))
			{
				// Propagate last error (but not last success)
				Ret = hr;
			}
			else
			{
				printf("    Registered OK\n");
			}
		}
		catch(HRError HR)
		{
			hr = HR.hr;
			printf("Failed to register property of type %s\n", (*it).Name.c_str());
			printf("Reason: \"%s\"\n", HR.Text.c_str());
			printf("Error:  0x%x %s\n", static_cast<int>(HR.hr), HRESULTtoString(HR.hr).c_str());
		}
		catch(...)
		{
			hr = E_FAIL;
			printf("Failed to register property of type %s\n", (*it).Name.c_str());
			throw;
		}

		if(!FAILED(hr))
		{
			try
			{
				// Try adding the new property
				hr = AddProperty(Dict, Object, PropertyID);
				if(FAILED(hr))
				{
					printf("Failed to set value for property of type %s\n", (*it).Name.c_str());
					printf("Error:  0x%x %s\n", static_cast<int>(hr), HRESULTtoString(hr).c_str());
				}
				else
				{
					printf("    Property Value Added OK\n");
				
					// Add this property to created properties list
					PropertyInfo Info;
					Info.Name = (*it).Name;
					Info.ID = PropertyID;
					PropertyList.push_back(Info);
				}
			}
			catch(HRError HR)
			{
				Ret = HR.hr;
				printf("Failed to set value for property of type %s\n", (*it).Name.c_str());
				printf("Reason: \"%s\"\n", HR.Text.c_str());
				printf("Error:  0x%x %s\n", static_cast<int>(HR.hr), HRESULTtoString(HR.hr).c_str());
			}
			catch(...)
			{
				Ret = E_FAIL;
				printf("Failed to set value for property of type %s\n", (*it).Name.c_str());
				throw;
			}
		}

		printf("\n");

		it++;
	}

	// Add in the most recent error
	if(!FAILED(Ret)) Ret = hr;

	return Ret;
}


//! Try reading all new type def properties from the given object, and check thier values
HRESULT VerifyTypeDefProperties(IAAFDictionary *Dict, IAAFObject *Object)
{
	// Overall "it worked" / "it failed" value
	HRESULT Ret = S_OK;

	// Current operation result
	HRESULT hr = S_OK;

	IAAFClassDef *cd_Object = NULL;

	// Do some basic set-up
	try
	{
		hr = Object->GetDefinition(&cd_Object);
		if(FAILED(hr)) throw HRError(hr, "Failed to find class definition for object under test");
	}
	catch(...)
	{
		ReleaseRefs(cd_Object);
		throw;
	}

	std::list<PropertyInfo>::iterator it = PropertyList.begin();
	while(it != PropertyList.end())
	{
		DebugMessage("Checking: %s\n", (*it).Name.c_str());

		IAAFPropertyDef *pd_Property = NULL;
		IAAFPropertyValue *pv_Value = NULL;

		try
		{
			aafBoolean_t IsPresent = false;
			hr = cd_Object->LookupPropertyDef((*it).ID, &pd_Property);
			if(!FAILED(hr)) hr = Object->IsPropertyPresent(pd_Property, &IsPresent);
			if(FAILED(hr)) throw HRError(hr, "Failed attempt to look up property " + (*it).Name);
			if(!IsPresent) throw HRError(AAFRESULT_PROP_NOT_PRESENT, "Property \"" + (*it).Name + "\" is missing");

			hr = Object->GetPropertyValue(pd_Property, &pv_Value);
			if(FAILED(hr)) throw HRError(hr, "Failed to read property " + (*it).Name);

			// Verify the property
			hr = VerifyProperty(pd_Property, pv_Value);

			if(FAILED(hr))
			{
				// Propagate last error (but not last success)
				Ret = hr;

				printf("Property \"%s\": FAILED: %s\n", (*it).Name.c_str(), HRESULTtoString(hr).c_str());
			}
			else
			{
				printf("Property \"%s\": Verified\n", (*it).Name.c_str());
			}

			ReleaseRefs(pd_Property);
			ReleaseRefs(pv_Value);
		}
		catch(HRError HR)
		{
			Ret = HR.hr;
			printf("Property \"%s\": FAILED: %s (%s)\n", (*it).Name.c_str(), HR.Text.c_str(), HRESULTtoString(HR.hr).c_str());

			ReleaseRefs(pd_Property);
			ReleaseRefs(pv_Value);
		}
		catch(...)
		{
			Ret = E_FAIL;
			printf("Property \"%s\": FAILED\n", (*it).Name.c_str());

			ReleaseRefs(pd_Property);
			ReleaseRefs(pv_Value);

			throw;
		}

		it++;
	}

	// Add in the most recent error
	if(!FAILED(Ret)) Ret = hr;

	return Ret;
}


//! Build a new random ID based on a template ID
/*! Randomizes the last 63 bits
 */
aafUID_t RandomID(const aafUID_t &Template /*=NULL_AUID*/ )
{
	// Start with a copy of the template
	aafUID_t Ret = Template;

	// If we don't have a valid template then generate an all random ID
	if(Ret.Data1 == 0)
	{
		Ret.Data1 = static_cast<aafUInt32>(rand());
		Ret.Data2 = static_cast<aafUInt16>(rand());
		Ret.Data3 = static_cast<aafUInt16>(rand());
	}

	// Randomize the last 8 bits
	int i;
	for(i=0; i<8; i++) Ret.Data4[i] = static_cast<aafUInt8>(rand());

	// Set bit 65
	Ret.Data4[0] |= 0x80;

	return Ret;
}


//! Register a new TypeDef of the specified type, with a given sub-type
/*! Registers a type definition
 *  \param Dict The meta-dictionary in which to define the new type
 *  \param kNewTypeID The Type ID of the new type to define
 *  \param kTypeToDefine The ID of the TypeDef to use for this new type
 *  \param kSubType The Type ID of the sub-type to define
 *  \note All failures will result in an exception <b>except</b> if the Initialize() call fails. 
 *        This is because this application is testing the functionality of that function with various combiniations of types.
 */
HRESULT RegisterType(IAAFDictionary *Dict, const aafUID_t &kNewTypeID, const aafUID_t &kTypeToDefine, const aafUID_t &kSubType)
{
	DebugMessage("RegisterType(Dict, kNewTypeID=%s, kTypeToDefine=%s, kSubType=%s)\n", GetName(kNewTypeID).c_str(), GetName(kTypeToDefine).c_str(), GetName(kSubType).c_str());

	HRESULT hr = S_OK;

	IAAFTypeDef *tdNewType = NULL;
	IAAFTypeDef *tdSubType = NULL;
	IUnknown *Interface = NULL;

	/***************** TEMPORARY KLUDGE *****************/
	/* Filter out known assertion causing combinations! */
	if((kTypeToDefine == AUID_AAFTypeDefFixedArray) && ((kSubType == AUID_AAFTypeDefEnum) || (kSubType == AUID_AAFTypeDefRecord)))
	{
		printf("Abandoning this test to avoid assertion in AAF DLL\n");
		return E_FAIL;
	}

	if((kTypeToDefine == AUID_AAFTypeDefString) && ((kSubType == AUID_AAFTypeDefEnum) || (kSubType == AUID_AAFTypeDefExtEnum)))
	{
		printf("Abandoning this test to avoid assertion in AAF DLL\n");
		return E_FAIL;
	}
	/***************** TEMPORARY KLUDGE *****************/

	try
	{
		// Make an ID for the new sub type
		aafUID_t SubTypeID = RandomID(kNewTypeID);

		aafUID_t InstanceID = GetInstanceID(kSubType);
		if(InstanceID == NULL_AUID)
		{
			DebugMessage("  We will need to register an instance of kSubType\n");

			// Make a new type of the specified sub-type
			hr = RegisterType(Dict, SubTypeID, kSubType, &tdSubType);
			if(FAILED(hr)) throw HRError(hr, "Cannot register sub-type " + GetName(kSubType));
		}
		else
		{
			DebugMessage("  We will use the existing instance of kSubType, %s\n", GetName(InstanceID).c_str());

			hr = Dict->LookupTypeDef(InstanceID, &tdSubType);
			if(FAILED(hr)) throw HRError(hr, "Cannot locate sub-type " + GetName(InstanceID));
		}

		DebugMessage("  Registering an instance of %s with the ID %s\n", GetName(kTypeToDefine).c_str(), GetName(kNewTypeID).c_str());

		hr = RegisterTypeInternal(Dict, kTypeToDefine, &Interface);
		if(FAILED(hr)) throw HRError(hr, "Failed to register type " + GetName(kTypeToDefine));

		if(kTypeToDefine == AUID_AAFTypeDefCharacter)
		{
			// Nothing to do to initialize a character
		}
		else if(kTypeToDefine == AUID_AAFTypeDefEnum)
		{
			aafInt64 Vals[3] = { 1, 2, 42 };
			aafString_t Names[3] = { L"One", L"Two", L"Valid" };
			hr = reinterpret_cast<IAAFTypeDefEnum*>(Interface)->Initialize(kNewTypeID, tdSubType, Vals, Names, 3, L"Test for TypeDefEnum");
		}
		else if(kTypeToDefine == AUID_AAFTypeDefFixedArray)
		{
			hr = reinterpret_cast<IAAFTypeDefFixedArray*>(Interface)->Initialize(kNewTypeID, tdSubType, 1, L"Test for TypeDefFixedArray");
		}
		else if(kTypeToDefine == AUID_AAFTypeDefRecord)
		{
			IAAFTypeDef *Types[1];
			Types[0] = tdSubType;
			aafString_t Names[1] = { L"Test" };

			hr = reinterpret_cast<IAAFTypeDefRecord*>(Interface)->Initialize(kNewTypeID, Types, Names, 1, L"Test for TypeDefRecord");
		}
		else if(kTypeToDefine == AUID_AAFTypeDefRename)
		{
			hr = reinterpret_cast<IAAFTypeDefRename*>(Interface)->Initialize(kNewTypeID, tdSubType, L"Test for TypeDefRename");
		}
		else if(kTypeToDefine == AUID_AAFTypeDefSet)
		{
			hr = reinterpret_cast<IAAFTypeDefSet*>(Interface)->Initialize(kNewTypeID, tdSubType, L"Test for TypeDefSet");
		}
		else if(kTypeToDefine == AUID_AAFTypeDefString)
		{
			hr = reinterpret_cast<IAAFTypeDefString*>(Interface)->Initialize(kNewTypeID, tdSubType, L"Test for TypeDefString");
		}
		else if(kTypeToDefine == AUID_AAFTypeDefVariableArray)
		{
			hr = reinterpret_cast<IAAFTypeDefVariableArray*>(Interface)->Initialize(kNewTypeID, tdSubType, L"Test for TypeDefVariableArray");
		}
		else
		{
			throw(0, "Don't know how to initialize type " + AUIDtoString(kTypeToDefine));
		}

		// Register this new type
		if(!FAILED(hr)) hr = Interface->QueryInterface(IID_IAAFTypeDef, reinterpret_cast<void **>(&tdNewType));
		if(!FAILED(hr)) hr = Dict->RegisterTypeDef(tdNewType);
	}
	catch(...)
	{
		ReleaseRefs(tdNewType);
		ReleaseRefs(tdSubType);
		ReleaseRefs(Interface);

		throw;
	}

	ReleaseRefs(tdNewType);
	ReleaseRefs(tdSubType);
	ReleaseRefs(Interface);

	return hr;
}


//! Create a new TypeDef of the specified type - but don't actually initialize or register. Used by other RegisterType() funcs.
/*! Registers a type definition and returns the main interface to allow the caller to Initialize the type
 *  \param Dict The meta-dictionary in which to define the new type
 *  \param kTypeToDefine The ID of the TypeDef to use for this new type
 *  \param NewInterface A pointer to a pointer to the main interface of the type def - cast into IUnknown for passing
 *  \note All failures will result in an exception.
 *        The HRESULT return is simply for compatibility!
 */
HRESULT RegisterTypeInternal(IAAFDictionary *Dict, const aafUID_t &kTypeToDefine, IUnknown **NewInterface)
{
	DebugMessage("  RegisterTypeInternal(Dict, kTypeToDefine=%s, NewInterface)\n", GetName(kTypeToDefine).c_str());

	HRESULT hr = S_OK;

	IUnknown *Unknown = NULL;
	*NewInterface = NULL;

	try
	{
		const IID kInterfaceID = GetInterfaceID(kTypeToDefine);
		DebugMessage("    Creating a new MetaInstance of %s with interface %s\n", GetName(kTypeToDefine).c_str(), IIDtoString(kInterfaceID).c_str());

		hr = Dict->CreateMetaInstance(kTypeToDefine, kInterfaceID, &Unknown);
		if(FAILED(hr)) throw HRError(hr, "Cannot create meta-instance of " + GetName(kTypeToDefine));

		DebugMessage("    Querying the %s interface\n", IIDtoString(kInterfaceID).c_str());

		hr = Unknown->QueryInterface(kInterfaceID, reinterpret_cast<void **>(NewInterface));
		if(FAILED(hr)) throw HRError(hr, "Failed to query interface " + IIDtoString(kInterfaceID) + " from type " + GetName(kTypeToDefine));
	}
	catch(...)
	{
		ReleaseRefs(Unknown);
		ReleaseRefs(*NewInterface);

		throw;
	}

	ReleaseRefs(Unknown);

	return hr;
}


//! Register a new TypeDef of the specified type
/*! \param Dict The meta-dictionary in which to define the new type
 *  \param kNewTypeID The Type ID of the new type to define
 *  \param kTypeToDefine The ID of the TypeDef to use for this new type
 *  \param tdNewType A pointer to a pointer to the TypeDef interface
 *  \note All failures will result in an exception <b>except</b> if the Initialize() call fails. 
 *        This is because this application is testing the functionality of that function with various combiniations of types.
 */
HRESULT RegisterType(IAAFDictionary *Dict, const aafUID_t &kNewTypeID, const aafUID_t &kTypeToDefine, IAAFTypeDef **tdNewType)
{
	HRESULT hr = S_OK;

	IUnknown *Interface = NULL;
	*tdNewType = NULL;
	IAAFTypeDef *tdSubType = NULL;
	IAAFClassDef *cdSubClass = NULL;

	try
	{
		/* Register the type, without initializint it */
		/* Note that when registering an Opaque we register the actual type we are wanting to register (in this case a rename) */
		if(kTypeToDefine == AUID_AAFTypeDefOpaque)
			hr = RegisterTypeInternal(Dict, AUID_AAFTypeDefRename, &Interface);
		else
			hr = RegisterTypeInternal(Dict, kTypeToDefine, &Interface);

		if(FAILED(hr)) throw HRError(hr, "Failed to register type " + GetName(kTypeToDefine));

		if(kTypeToDefine == AUID_AAFTypeDefCharacter)
		{
			// Nothing to do to initialize a character
		}
		else if(kTypeToDefine == AUID_AAFTypeDefEnum)
		{
			// Make this an enum of Int64s
			hr = Dict->LookupTypeDef(kAAFTypeID_Int64, &tdSubType);
			if(FAILED(hr)) throw HRError(hr, "Cannot locate kAAFTypeID_Int64 " + AUIDtoString(kAAFTypeID_Int64));
			
			aafInt64 Vals[3] = { 1, 2, 42 };
			aafString_t Names[3] = { L"One", L"Two", L"Valid" };
			hr = reinterpret_cast<IAAFTypeDefEnum*>(Interface)->Initialize(kNewTypeID, tdSubType, Vals, Names, 3, L"Test for TypeDefEnum");
		}
		else if(kTypeToDefine == AUID_AAFTypeDefExtEnum)
		{
			hr = reinterpret_cast<IAAFTypeDefExtEnum*>(Interface)->Initialize(kNewTypeID, L"Test for TypeDefExtEnum");
			aafUID_t Value = RandomID();
			std::string Name = AUIDtoString(Value);
			aafCharacter *aafName = new aafCharacter[Name.length() + 1];
			size_t i;
			for(i=0; i<Name.length(); i++)
			{
				aafName[i] = Name.at(i);
			}
			aafName[Name.length()] = 0;

			if(!FAILED(hr)) reinterpret_cast<IAAFTypeDefExtEnum*>(Interface)->AppendElement(Value, aafName);

			delete[] aafName;
		}
		else if(kTypeToDefine == AUID_AAFTypeDefFixedArray)
		{
			// Make this an array of an Int32s
			hr = Dict->LookupTypeDef(kAAFTypeID_Int32, &tdSubType);
			if(FAILED(hr)) throw HRError(hr, "Cannot locate kAAFTypeID_Int32 " + AUIDtoString(kAAFTypeID_Int32));
			hr = reinterpret_cast<IAAFTypeDefFixedArray*>(Interface)->Initialize(kNewTypeID, tdSubType, 1, L"Test for TypeDefFixedArray");
		}
		else if(kTypeToDefine == AUID_AAFTypeDefOpaque)
		{
			// Make this an opaque rename of an Int32
			hr = Dict->LookupTypeDef(kAAFTypeID_Int32, &tdSubType);
			if(FAILED(hr)) throw HRError(hr, "Cannot locate kAAFTypeID_Int32 " + AUIDtoString(kAAFTypeID_Int32));
			hr = reinterpret_cast<IAAFTypeDefRename*>(Interface)->Initialize(kNewTypeID, tdSubType, L"Test for Opaque TypeDefRename of Int32");
			if(FAILED(hr)) throw HRError(hr, "Failed to initialize rename of Int32 for TypeDefOpaque");

			IAAFTypeDef *td = NULL;
			try
			{
                hr = reinterpret_cast<IAAFTypeDefRename*>(Interface)->QueryInterface(IID_IAAFTypeDef,(void**)&td);
				if(!FAILED(hr)) hr = Dict->RegisterOpaqueTypeDef(td);
			}
			catch(...)
			{
				ReleaseRefs(td);
				throw;
			}
			ReleaseRefs(td);

			if(FAILED(hr)) throw HRError(hr, "Failed to register opaque type");
		}
		else if(kTypeToDefine == AUID_AAFTypeDefRecord)
		{
			// Make this a record with a single Int32
			hr = Dict->LookupTypeDef(kAAFTypeID_Int32, &tdSubType);
			if(FAILED(hr)) throw HRError(hr, "Cannot locate kAAFTypeID_Int32 " + AUIDtoString(kAAFTypeID_Int32));

			IAAFTypeDef *Types[1];
			Types[0] = tdSubType;
			aafString_t Names[1] = { L"Integer" };

			hr = reinterpret_cast<IAAFTypeDefRecord*>(Interface)->Initialize(kNewTypeID, Types, Names, 1, L"Test for TypeDefRecord");

			// Register the struct offsets to allow us to access memebers
			aafUInt32 Offsets[1] = { 0 };
			if(!FAILED(hr)) reinterpret_cast<IAAFTypeDefRecord*>(Interface)->RegisterMembers(Offsets, 1, sizeof(aafInt32));
		}
		else if(kTypeToDefine == AUID_AAFTypeDefRename)
		{
			// Make this a rename of an Int32
			hr = Dict->LookupTypeDef(kAAFTypeID_Int32, &tdSubType);
			if(FAILED(hr)) throw HRError(hr, "Cannot locate kAAFTypeID_Int32 " + AUIDtoString(kAAFTypeID_Int32));
			hr = reinterpret_cast<IAAFTypeDefRename*>(Interface)->Initialize(kNewTypeID, tdSubType, L"Test for TypeDefRename of Int32");
		}
		else if(kTypeToDefine == AUID_AAFTypeDefSet)
		{
			// Make an ID for the new sub type
			aafUID_t SubTypeID = RandomID(kNewTypeID);

			// Make this a Strong Ref set
			hr = RegisterType(Dict, SubTypeID, AUID_AAFTypeDefStrongObjRef, &tdSubType);
			if(FAILED(hr)) throw HRError(hr, "Cannot register a TypeDefStrongObjRef " + GetName(AUID_AAFTypeDefStrongObjRef));

			hr = reinterpret_cast<IAAFTypeDefSet*>(Interface)->Initialize(kNewTypeID, tdSubType, L"Test for TypeDefSet");
		}
		else if(kTypeToDefine == AUID_AAFTypeDefStream)
		{
			// Nothing to do to initialize a stream
		}
		else if(kTypeToDefine == AUID_AAFTypeDefString)
		{
			// Make this a string of a characters
			hr = Dict->LookupTypeDef(kAAFTypeID_Character, &tdSubType);
			if(FAILED(hr)) throw HRError(hr, "Cannot locate kAAFTypeID_Character " + AUIDtoString(kAAFTypeID_Character));
			hr = reinterpret_cast<IAAFTypeDefString*>(Interface)->Initialize(kNewTypeID, tdSubType, L"Test for TypeDefString");
		}
		else if(kTypeToDefine == AUID_AAFTypeDefStrongObjRef)
		{
			// Make this a strong ref to a TestDef
			hr = Dict->LookupClassDef(kClassAUID_TestDef, &cdSubClass);
			if(FAILED(hr)) throw HRError(hr, "Cannot locate kClassAUID_TestDef " + AUIDtoString(kClassAUID_TestDef));
			hr = reinterpret_cast<IAAFTypeDefStrongObjRef*>(Interface)->Initialize(kNewTypeID, cdSubClass, L"Test for TypeDefStrongObjRef");
		}
		else if(kTypeToDefine == AUID_AAFTypeDefVariableArray)
		{
			// Make this an array of an Int32s
			hr = Dict->LookupTypeDef(kAAFTypeID_Int32, &tdSubType);
			if(FAILED(hr)) throw HRError(hr, "Cannot locate kAAFTypeID_Int32 " + AUIDtoString(kAAFTypeID_Int32));
			hr = reinterpret_cast<IAAFTypeDefVariableArray*>(Interface)->Initialize(kNewTypeID, tdSubType, L"Test for TypeDefVariableArray");
		}
		else if(kTypeToDefine == AUID_AAFTypeDefWeakObjRef)
		{
			// Make this a weak ref to a type definition
			hr = Dict->LookupClassDef(kAAFClassID_TypeDef, &cdSubClass);
			if(FAILED(hr)) throw HRError(hr, "Cannot locate kAAFClassID_TypeDef " + AUIDtoString(kAAFClassID_TypeDef));

			// Make the target a type definition
			const aafUID_t TargetSet[] = { kAAFPropID_Root_MetaDictionary, kAAFPropID_MetaDictionary_TypeDefinitions };
			hr = reinterpret_cast<IAAFTypeDefWeakObjRef*>(Interface)->Initialize(kNewTypeID, cdSubClass, L"Test for TypeDefWeakObjRef", sizeof(TargetSet)/sizeof(TargetSet[0]), TargetSet);
		}
		else
		{
			throw(0, "Don't know how to initialize type " + AUIDtoString(kTypeToDefine));
		}

		// Query the generic TypeDef interface
		if(!FAILED(hr)) hr = Interface->QueryInterface(IID_IAAFTypeDef, reinterpret_cast<void **>(tdNewType));

		// Register this new type (but not if it is an opaque type!)
		if(kTypeToDefine != AUID_AAFTypeDefOpaque)
		{
			if(!FAILED(hr)) hr = Dict->RegisterTypeDef(*tdNewType);
		}
	}
	catch(...)
	{
		ReleaseRefs(Interface);
		ReleaseRefs(*tdNewType);
		ReleaseRefs(tdSubType);
		ReleaseRefs(cdSubClass);

		throw;
	}

	ReleaseRefs(Interface);
	ReleaseRefs(cdSubClass);

	return hr;
}


//! Register a property of the given type in a given class
/*! \param Dict The meta-dictionary in which to define the new property
 *  \param kClassID The class ID of class to receive the new property
 *  \param kTypeID The type ID defining the type of property to add
 *  \param kPropertyID Property ID for the new property
 *  \param kPropertyName Descriptive name for this property
 *  \note Most errors will cause exceptions to be thrown
 */
HRESULT RegisterProperty(IAAFDictionary *Dict, const aafUID_t &kClassID, const aafUID_t &kTypeID, const aafUID_t &kPropertyID, const aafString_t &kPropertyName)
{
	HRESULT hr = S_OK;

	IAAFTypeDef *td_Type = NULL;
	IAAFClassDef *cd_Class = NULL;

	try
	{
		hr = Dict->LookupTypeDef(kTypeID, &td_Type);
		if(FAILED(hr)) throw HRError(hr, "Failed to locate newly defined type " + GetName(kTypeID));

		hr = Dict->LookupClassDef(kClassID, &cd_Class);
		if(FAILED(hr)) throw HRError(hr, "Failed to locate class " + GetName(kClassID));

		hr = cd_Class->RegisterOptionalPropertyDef(kPropertyID, kPropertyName, td_Type, NULL);
		if(FAILED(hr)) throw HRError(hr, "Failed to add optional property to class " + GetName(kClassID));
	}
	catch(...)
	{
		ReleaseRefs(cd_Class);
		ReleaseRefs(td_Type);

		throw;
	}

	ReleaseRefs(cd_Class);
	ReleaseRefs(td_Type);

	return hr;
}


//! Set a value for a given property of an object
HRESULT AddProperty(IAAFDictionary *Dict, IAAFObject *Object, const aafUID_t kPropertyID)
{
	HRESULT hr = S_OK;

	IAAFClassDef *cd_Object = NULL;
	IAAFPropertyDef *pd_Property = NULL;
	IAAFTypeDef *td_Property = NULL;
	IAAFTypeDef *td_Base = NULL;
	IAAFPropertyValue *pv_Value = NULL;
	IAAFPropertyValue *pv_BaseValue = NULL;

	eAAFTypeCategory_t TypeCat;
	eAAFTypeCategory_t BaseTypeCat;

	try
	{
		// Get the property definition via the class definition
		hr = Object->GetDefinition(&cd_Object);
		if(!FAILED(hr)) hr = cd_Object->LookupPropertyDef(kPropertyID, &pd_Property);
		if(FAILED(hr)) throw HRError(hr, "Failed to locate property definition for " + GetName(kPropertyID));

		// Get the type definition for this property
		hr = pd_Property->GetTypeDef(&td_Property);
		if(FAILED(hr)) throw HRError(hr, "Failed to locate type definition for property " + GetName(kPropertyID));

		// Get the type category of this property
		hr = td_Property->GetTypeCategory(&TypeCat);
		if(FAILED(hr)) throw HRError(hr, "Failed to identify the type category for property " + GetName(kPropertyID));

		/* Get the base type def */
		switch(TypeCat)
		{
		case kAAFTypeCatEnum:
			{
				IAAFTypeDefEnum *td = NULL;

				try
				{
					hr = td_Property->QueryInterface(IID_IAAFTypeDefEnum, (void **)&td);
					if(!FAILED(hr)) hr = td->GetElementType(&td_Base);

					if(FAILED(hr)) throw HRError(hr, "Failed to locate base type for TypeDefEnum");
				}
				catch(...)
				{
					ReleaseRefs(td);
					throw;
				}
				ReleaseRefs(td);
			
				break;
			}

		case kAAFTypeCatFixedArray:
			{
				IAAFTypeDefFixedArray *td = NULL;

				try
				{
					hr = td_Property->QueryInterface(IID_IAAFTypeDefFixedArray, (void **)&td);
					if(!FAILED(hr)) hr = td->GetType(&td_Base);

					if(FAILED(hr)) throw HRError(hr, "Failed to locate base type for TypeDefFixedArray");
				}
				catch(...)
				{
					ReleaseRefs(td);
					throw;
				}
				ReleaseRefs(td);
			
				break;
			}

		case kAAFTypeCatRecord:
			{
				IAAFTypeDefRecord *td = NULL;

				try
				{
					hr = td_Property->QueryInterface(IID_IAAFTypeDefRecord, (void **)&td);
					if(!FAILED(hr)) hr = td->GetMemberType(0, &td_Base);

					if(FAILED(hr)) throw HRError(hr, "Failed to locate base type for TypeDefRecord");
				}
				catch(...)
				{
					ReleaseRefs(td);
					throw;
				}
				ReleaseRefs(td);
			
				break;
			}

		case kAAFTypeCatRename:
			{
				IAAFTypeDefRename *td = NULL;

				try
				{
					hr = td_Property->QueryInterface(IID_IAAFTypeDefRename, (void **)&td);
					if(!FAILED(hr)) hr = td->GetBaseType(&td_Base);

					if(FAILED(hr)) throw HRError(hr, "Failed to locate base type for TypeDefRename");
				}
				catch(...)
				{
					ReleaseRefs(td);
					throw;
				}
				ReleaseRefs(td);
			
				break;
			}

		case kAAFTypeCatSet:
			{
				IAAFTypeDefSet *td = NULL;

				try
				{
					hr = td_Property->QueryInterface(IID_IAAFTypeDefSet, (void **)&td);
					if(!FAILED(hr)) hr = td->GetElementType(&td_Base);

					if(FAILED(hr)) throw HRError(hr, "Failed to locate base type for TypeDefSet");
				}
				catch(...)
				{
					ReleaseRefs(td);
					throw;
				}
				ReleaseRefs(td);
			
				break;
			}

		case kAAFTypeCatString:
			{
				IAAFTypeDefString *td = NULL;

				try
				{
					hr = td_Property->QueryInterface(IID_IAAFTypeDefString, (void **)&td);
					if(!FAILED(hr)) hr = td->GetType(&td_Base);

					if(FAILED(hr)) throw HRError(hr, "Failed to locate base type for TypeDefString");
				}
				catch(...)
				{
					ReleaseRefs(td);
					throw;
				}
				ReleaseRefs(td);
			
				break;
			}

		case kAAFTypeCatVariableArray:
			{
				IAAFTypeDefVariableArray *td = NULL;

				try
				{
					hr = td_Property->QueryInterface(IID_IAAFTypeDefVariableArray, (void **)&td);
					if(!FAILED(hr)) hr = td->GetType(&td_Base);

					if(FAILED(hr)) throw HRError(hr, "Failed to locate base type for TypeDefVariableArray");
				}
				catch(...)
				{
					ReleaseRefs(td);
					throw;
				}
				ReleaseRefs(td);
			
				break;
			}

		default:
			{
				char Buffer[128];
				sprintf(Buffer, "Unknown outer type category %d found", static_cast<int>(TypeCat));
				throw HRError(E_FAIL, Buffer);
			}
		}

		// Get the type category of the base type
		hr = td_Base->GetTypeCategory(&BaseTypeCat);
		if(FAILED(hr)) throw HRError(hr, "Failed to identify the type category for base type of " + GetName(kPropertyID));


		/* Set a value of the base type */
		switch(BaseTypeCat)
		{
		case kAAFTypeCatInt:
			{
				IAAFTypeDefInt *td = NULL;

				try
				{
					aafInt32 Value = 42;

					hr = td_Base->QueryInterface(IID_IAAFTypeDefInt, (void **)&td);
					if(!FAILED(hr)) hr = td->CreateValue((aafMemPtr_t) &Value, sizeof(Value), &pv_BaseValue);

					if(FAILED(hr)) throw HRError(hr, "Failed to create property value for TypeDefInt");
				}
				catch(...)
				{
					ReleaseRefs(td);
					throw;
				}

				ReleaseRefs(td);

				break;
			}

		case kAAFTypeCatCharacter:
			{
				IAAFTypeDefCharacter *td = NULL;

				try
				{
					aafCharacter Value = '*';

					hr = td_Base->QueryInterface(IID_IAAFTypeDefCharacter, (void **)&td);
					if(!FAILED(hr)) hr = td->CreateValueFromCharacter(Value, &pv_BaseValue);

					if(FAILED(hr)) throw HRError(hr, "Failed to create property value for TypeDefCharacter");
				}
				catch(...)
				{
					ReleaseRefs(td);
					throw;
				}

				ReleaseRefs(td);

				break;
			}

		case kAAFTypeCatEnum:
			{
				IAAFTypeDefEnum *td = NULL;

				try
				{
					const aafUInt32 BufferSize = 128;
					aafCharacter Buffer[BufferSize];

					hr = td_Base->QueryInterface(IID_IAAFTypeDefEnum, (void **)&td);
					if(FAILED(hr)) throw HRError(hr, "Failed to query AAFTypeDefEnum interface for TypeDefEnum");

					hr = td->GetElementName(0, Buffer, BufferSize);
					if(!FAILED(hr)) hr = td->CreateValueFromName(Buffer, &pv_BaseValue);

					if(FAILED(hr)) throw HRError(hr, "Failed to create property value for TypeDefEnum");
				}
				catch(...)
				{
					ReleaseRefs(td);
					throw;
				}

				ReleaseRefs(td);

				break;
			}

		case kAAFTypeCatExtEnum:
			{
				IAAFTypeDefExtEnum *td = NULL;

				try
				{
					const aafUInt32 BufferSize = 128;
					aafCharacter Buffer[BufferSize];

					hr = td_Base->QueryInterface(IID_IAAFTypeDefExtEnum, (void **)&td);
					if(FAILED(hr)) throw HRError(hr, "Failed to query AAFTypeDefExtEnum interface for TypeDefExtEnum");

					hr = td->GetElementName(0, Buffer, BufferSize);
					if(!FAILED(hr)) hr = td->CreateValueFromName(Buffer, &pv_BaseValue);

					if(FAILED(hr)) throw HRError(hr, "Failed to create property value for TypeDefExtEnum");
				}
				catch(...)
				{
					ReleaseRefs(td);
					throw;
				}

				ReleaseRefs(td);

				break;
			}

		case kAAFTypeCatRename:
			{
				/* NOTE: Our test rename is of an Int32 */

				IAAFTypeDefRename *tdRename = NULL;
				IAAFTypeDef *tdActual = NULL;
				IAAFTypeDefInt *td = NULL;

				try
				{
					aafInt32 Value = 42;

					hr = td_Base->QueryInterface(IID_IAAFTypeDefRename, (void **)&tdRename);
                    if(!FAILED(hr)) hr = tdRename->GetBaseType(&tdActual);

					if(FAILED(hr)) throw HRError(hr, "Failed to get base type of TypeDefRename");

					hr = tdActual->QueryInterface(IID_IAAFTypeDefInt, (void **)&td);
                    if(!FAILED(hr)) hr = td->CreateValue((aafMemPtr_t) &Value, sizeof(Value), &pv_BaseValue);

					if(FAILED(hr)) throw HRError(hr, "Failed to create integer property value for TypeDefRename");
				}
				catch(...)
				{
					ReleaseRefs(tdRename);
					ReleaseRefs(tdActual);
					ReleaseRefs(td);
					throw;
				}

				ReleaseRefs(tdRename);
				ReleaseRefs(tdActual);
				ReleaseRefs(td);

				break;
			}

		case kAAFTypeCatFixedArray:
			{
				IAAFTypeDefFixedArray *td = NULL;

				try
				{
					aafInt32 Value = 42;

					hr = td_Base->QueryInterface(IID_IAAFTypeDefFixedArray, (void **)&td);
					if(FAILED(hr)) throw HRError(hr, "Failed to query AAFTypeDefFixedArray interface for TypeDefFixedArray");

					hr = td->CreateValueFromCArray((aafMemPtr_t) &Value, sizeof(Value), &pv_BaseValue);

					if(FAILED(hr)) throw HRError(hr, "Failed to create property value for TypeDefFixedArray");
				}
				catch(...)
				{
					ReleaseRefs(td);
					throw;
				}

				ReleaseRefs(td);

				break;
			}

		case kAAFTypeCatVariableArray:
			{
				IAAFTypeDefVariableArray *td = NULL;

				try
				{
					aafInt32 Value = 42;

					hr = td_Base->QueryInterface(IID_IAAFTypeDefVariableArray, (void **)&td);
					if(FAILED(hr)) throw HRError(hr, "Failed to query AAFTypeDefVariableArray interface for TypeDefVariableArray");

					hr = td->CreateValueFromCArray((aafMemPtr_t) &Value, sizeof(Value), &pv_BaseValue);

					if(FAILED(hr)) throw HRError(hr, "Failed to create property value for TypeDefVariableArray");
				}
				catch(...)
				{
					ReleaseRefs(td);
					throw;
				}

				ReleaseRefs(td);

				break;
			}

		case kAAFTypeCatRecord:
			{
				IAAFTypeDefRecord *td = NULL;

				try
				{
					aafInt32 Value = 42;

					hr = td_Base->QueryInterface(IID_IAAFTypeDefRecord, (void **)&td);
					if(FAILED(hr)) throw HRError(hr, "Failed to query AAFTypeDefRecord interface for TypeDefRecord");

					hr = td->CreateValueFromStruct((aafMemPtr_t) &Value, sizeof(Value), &pv_BaseValue);

					if(FAILED(hr)) throw HRError(hr, "Failed to create property value for TypeDefRecord");
				}
				catch(...)
				{
					ReleaseRefs(td);
					throw;
				}

				ReleaseRefs(td);

				break;
			}

		case kAAFTypeCatSet:
			{
				IAAFTypeDef *td = NULL;
				IAAFTypeDefSet *tdSet = NULL;
				IAAFTypeDefObjectRef *tdObjRef = NULL;
				IAAFClassDef *cdTarget = NULL;
				IAAFDefObject *pTarget = NULL;
				IUnknown *pUnknown = NULL;
				IAAFPropertyValue *pvElement = NULL;

				try
				{
					/* Build a target set to strong reference */
					hr = Dict->LookupClassDef(kClassAUID_TestDef, &cdTarget);
					if(FAILED(hr)) throw HRError(hr, "Failed to find test class " + AUIDtoString(kClassAUID_TestDef));

					hr = cdTarget->CreateInstance(IID_IAAFDefObject, (IUnknown**)&pTarget);
					if(!FAILED(hr)) hr = pTarget->Initialize(RandomID(kPropertyID), L"Target for TypeDefSet strong ref");
					if(!FAILED(hr)) hr = pTarget->QueryInterface(IID_IUnknown, (void **)&pUnknown);
					if(FAILED(hr)) throw HRError(hr, "Failed to instantiate test class " + AUIDtoString(kClassAUID_TestDef));

					/* Build a property value which is a strong reference to the target set */
					hr = td_Base->QueryInterface(IID_IAAFTypeDefSet, (void **)&tdSet);
					if(FAILED(hr)) throw HRError(hr, "Failed to query AAFTypeDefSet interface for TypeDefSet");

					hr = tdSet->GetElementType(&td);
					if(FAILED(hr)) throw HRError(hr, "Failed to get TypeDefSet element type ");
					
					hr = td->QueryInterface(IID_IAAFTypeDefObjectRef, (void **)&tdObjRef);
					if(FAILED(hr)) throw HRError(hr, "Failed to query object ref interface for TypeDefSet element");

					hr = tdObjRef->CreateValue(pUnknown, &pvElement);
					if(FAILED(hr)) throw HRError(hr, "Failed to create set element property value for TypeDefSet");

					// We need to build this value in the property!
					hr = Object->GetPropertyValue( pd_Property, &pv_BaseValue);
					if(hr == AAFRESULT_PROP_NOT_PRESENT)
					{
						hr = Object->CreateOptionalPropertyValue( pd_Property, &pv_BaseValue);
					}
					if(FAILED(hr)) throw HRError(hr, "Failed to locate or create property value");

					hr = tdSet->AddElement(pv_BaseValue, pvElement);
					if(FAILED(hr)) throw HRError(hr, "Failed to create property value for TypeDefSet");
				}
				catch(...)
				{
					ReleaseRefs(td);
					ReleaseRefs(tdSet);
					ReleaseRefs(tdObjRef);
					ReleaseRefs(cdTarget);
					ReleaseRefs(pTarget);
					ReleaseRefs(pUnknown);
					ReleaseRefs(pvElement);
					throw;
				}

				ReleaseRefs(td);
				ReleaseRefs(tdSet);
				ReleaseRefs(tdObjRef);
				ReleaseRefs(cdTarget);
				ReleaseRefs(pTarget);
				ReleaseRefs(pUnknown);
				ReleaseRefs(pvElement);

				break;
			}

		case kAAFTypeCatString:
			{
				IAAFTypeDefString *td = NULL;

				try
				{
					aafCharacter Value = 42;

					hr = td_Base->QueryInterface(IID_IAAFTypeDefString, (void **)&td);
					if(FAILED(hr)) throw HRError(hr, "Failed to query AAFTypeDefString interface for TypeDefString");

					hr = td->CreateValueFromCString((aafMemPtr_t) &Value, sizeof(Value), &pv_BaseValue);

					if(FAILED(hr)) throw HRError(hr, "Failed to create property value for TypeDefString");
				}
				catch(...)
				{
					ReleaseRefs(td);
					throw;
				}

				ReleaseRefs(td);

				break;
			}

		case kAAFTypeCatIndirect:
			{
				IAAFTypeDefIndirect *td = NULL;
				IAAFTypeDef *tdSubType = NULL;

				try
				{
					aafInt32 Value = 42;

					hr = td_Base->QueryInterface(IID_IAAFTypeDefIndirect, (void **)&td);
					if(FAILED(hr)) throw HRError(hr, "Failed to query AAFTypeDefIndirect interface for TypeDefIndirect");

					hr = Dict->LookupTypeDef(kAAFTypeID_Int32, &tdSubType);
					if(FAILED(hr)) throw HRError(hr, "Cannot locate kAAFTypeID_Int32 " + AUIDtoString(kAAFTypeID_Int32));

					hr = td->CreateValueFromActualData(tdSubType, (aafMemPtr_t) &Value, sizeof(Value), &pv_BaseValue);

					if(FAILED(hr)) throw HRError(hr, "Failed to create property value for TypeDefIndirect");
				}
				catch(...)
				{
					ReleaseRefs(td);
					ReleaseRefs(tdSubType);
					throw;
				}

				ReleaseRefs(td);
				ReleaseRefs(tdSubType);

				break;
			}

		case kAAFTypeCatOpaque:
			{
				IAAFTypeDefOpaque *td = NULL;
				IAAFTypeDefIndirect *tdIndirect = NULL;

				try
				{
					aafInt32 Value = 42;

					hr = td_Base->QueryInterface(IID_IAAFTypeDefOpaque, (void **)&td);
					if(FAILED(hr)) throw HRError(hr, "Failed to query AAFTypeDefOpaque interface for TypeDefOpaque");

					hr = td->QueryInterface(IID_IAAFTypeDefIndirect, (void **)&tdIndirect);
					if(FAILED(hr)) throw HRError(hr, "Failed to query AAFTypeDefIndirect interface for TypeDefOpaque");

					hr = tdIndirect->CreateValueFromActualData(td_Base, (aafMemPtr_t) &Value, sizeof(Value), &pv_BaseValue);

					if(FAILED(hr)) throw HRError(hr, "Failed to create property value for TypeDefOpaque");
				}
				catch(...)
				{
					ReleaseRefs(td);
					ReleaseRefs(tdIndirect);
					throw;
				}

				ReleaseRefs(td);
				ReleaseRefs(tdIndirect);

				break;
			}

		case kAAFTypeCatStream:
			{
				IAAFTypeDefStream *td = NULL;

				try
				{
					aafInt32 Value = 42;

					hr = td_Base->QueryInterface(IID_IAAFTypeDefStream, (void **)&td);
					if(FAILED(hr)) throw HRError(hr, "Failed to query AAFTypeDefStream interface for TypeDefStream");

					// We need to build this value in the property!
					hr = Object->GetPropertyValue( pd_Property, &pv_BaseValue);
					if(hr == AAFRESULT_PROP_NOT_PRESENT)
					{
						hr = Object->CreateOptionalPropertyValue( pd_Property, &pv_BaseValue);
					}
					if(FAILED(hr)) throw HRError(hr, "Failed to locate or create property value");

					hr = td->Write(pv_BaseValue, sizeof(Value), (aafMemPtr_t) &Value);

					if(FAILED(hr)) throw HRError(hr, "Failed to create property value for TypeDefStream");
				}
				catch(...)
				{
					ReleaseRefs(td);
					throw;
				}

				ReleaseRefs(td);

				break;
			}

		case kAAFTypeCatStrongObjRef:
			{
				// Note: As we are generating a strong reference, and there can only be one strong 
				//       reference to any object, we must create a new object to reference

				IAAFTypeDefObjectRef *td = NULL;

				IAAFClassDef *cd_Target = NULL;
				IAAFObject *pTarget = NULL;
				IAAFDefObject *pDefObj = NULL;

				try
				{
					hr = td_Base->QueryInterface(IID_IAAFTypeDefObjectRef, (void **)&td);
					if(FAILED(hr)) throw HRError(hr, "Failed to aquire IAAFTypeDefObjectRef interface");

					hr = td->GetObjectType(&cd_Target);
					if(!FAILED(hr)) hr = cd_Target->CreateInstance(IID_IAAFObject, (IUnknown **)&pTarget);
					if(FAILED(hr)) throw HRError(hr, "Failed to create target object for TypeDefStrongObjRef");

					hr = pTarget->QueryInterface(IID_IAAFDefObject, (void **)&pDefObj);
					if(FAILED(hr)) throw HRError(hr, "Strong reference target is not a DefObject");

					hr = pDefObj->Initialize(RandomID(), L"New Test Target");
					if(FAILED(hr)) throw HRError(hr, "Failed to initialize strong reference target");

					hr = td->CreateValue((IUnknown *)pTarget, &pv_BaseValue);
					if(FAILED(hr)) throw HRError(hr, "Failed to create property value for TypeDefStrongObjRef");
				}
				catch(...)
				{
					ReleaseRefs(td);
					ReleaseRefs(cd_Target);
					ReleaseRefs(pTarget);
					ReleaseRefs(pDefObj);
					throw;
				}

				ReleaseRefs(td);
				ReleaseRefs(cd_Target);
				ReleaseRefs(pTarget);
				ReleaseRefs(pDefObj);

				break;
			}

		case kAAFTypeCatWeakObjRef:
			{
				IAAFTypeDefObjectRef *td = NULL;
				IUnknown *pUnk = NULL;

				try
				{
					hr = td_Base->QueryInterface(IID_IAAFTypeDefObjectRef, (void **)&td);
					if(FAILED(hr)) throw HRError(hr, "Failed to aquire IAAFTypeDefObjectRef interface");

					hr = td_Base->QueryInterface(IID_IUnknown, (void **)&pUnk);
					if(FAILED(hr)) throw HRError(hr, "Failed to aquire IUnknown interface");

					// Make this a weak reference to its own type definition
					hr = td->CreateValue(pUnk, &pv_BaseValue);
					if(FAILED(hr)) throw HRError(hr, "Failed to create property value for TypeDefWeakObjRef");
				}
				catch(...)
				{
					ReleaseRefs(td);
					ReleaseRefs(pUnk);
					throw;
				}

				ReleaseRefs(td);
				ReleaseRefs(pUnk);

				break;
			}

		default:
			{
				char Buffer[128];
				sprintf(Buffer, "Unknown type category %d found", static_cast<int>(BaseTypeCat));
				throw HRError(E_FAIL, Buffer);
			}
		}

		// We need to ensure that there is an existing property value, or create a new one
		hr = Object->GetPropertyValue( pd_Property, &pv_Value);
		if(hr == AAFRESULT_PROP_NOT_PRESENT)
		{
			/***************** TEMPORARY KLUDGE *****************/
			/* Filter out known assertion causing combinations! */
			
			if((TypeCat == kAAFTypeCatString) && ((BaseTypeCat == kAAFTypeCatEnum) || (BaseTypeCat == kAAFTypeCatExtEnum)) )
			{
				throw HRError(E_FAIL, "Abandoning this test to avoid assertion in AAF DLL");
			}

			if((TypeCat == kAAFTypeCatRename) && (BaseTypeCat == kAAFTypeCatVariableArray) )
			{
				throw HRError(E_FAIL, "Abandoning this test to avoid assertion in AAF DLL");
			}
			/***************** TEMPORARY KLUDGE *****************/

			hr = Object->CreateOptionalPropertyValue( pd_Property, &pv_Value);
		}
		if(FAILED(hr)) throw HRError(hr, "Failed to locate or create property value");


		/* Build the outer value from the base value */
		switch(TypeCat)
		{
		case kAAFTypeCatEnum:
			{
				if(BaseTypeCat != kAAFTypeCatInt)
				{
					// Note that we simply transfer the base value over to the outer value for an enum
					// Note also that we clear the base value pointer to ensure the reference counting stays correct
					ReleaseRefs(pv_Value);
					pv_Value = pv_BaseValue;
					pv_BaseValue = NULL;
				}
				else
				{
					IAAFTypeDefEnum *td = NULL;
					IAAFTypeDefInt *td_Integer = NULL;

					aafInt64 Value = 0;

					try
					{
						hr = td_Property->QueryInterface(IID_IAAFTypeDefEnum, (void **)&td);
						if(!FAILED(hr)) hr = td_Base->QueryInterface(IID_IAAFTypeDefInt, (void **)&td_Integer);
						if(!FAILED(hr)) hr = td_Integer->GetInteger(pv_BaseValue, (aafMemPtr_t) &Value, sizeof(Value));
						if(!FAILED(hr)) hr = td->SetIntegerValue(pv_Value, Value);

						if(FAILED(hr)) throw HRError(hr, "Failed to set outer value for TypeDefEnum");
					}
					catch(...)
					{
						ReleaseRefs(td);
						throw;
					}
					ReleaseRefs(td);
				}
				break;
			}

		case kAAFTypeCatFixedArray:
			{
				IAAFTypeDefFixedArray *td = NULL;

				try
				{
					ReleaseRefs(pv_Value);

					hr = td_Property->QueryInterface(IID_IAAFTypeDefFixedArray, (void **)&td);

					// Note that we use a pointer to the base value to turn it into an array of one item
					if(!FAILED(hr)) hr = td->CreateValueFromValues(&pv_BaseValue, 1, &pv_Value);

					if(FAILED(hr)) throw HRError(hr, "Failed to set outer value for TypeDefFixedArray");
				}
				catch(...)
				{
					ReleaseRefs(td);
					throw;
				}
				ReleaseRefs(td);

				break;
			}

		case kAAFTypeCatRecord:
			{
				IAAFTypeDefRecord *td = NULL;

				try
				{
					ReleaseRefs(pv_Value);

					hr = td_Property->QueryInterface(IID_IAAFTypeDefRecord, (void **)&td);

					// Note that we use a pointer to the base value to turn it into an array of one item
					if(!FAILED(hr)) hr = td->CreateValueFromValues(&pv_BaseValue, 1, &pv_Value);

					if(FAILED(hr)) throw HRError(hr, "Failed to set outer value for TypeDefRecord");
				}
				catch(...)
				{
					ReleaseRefs(td);
					throw;
				}
				ReleaseRefs(td);

				break;
			}

		case kAAFTypeCatRename:
			{
				if((BaseTypeCat == kAAFTypeCatStrongObjRef) || (BaseTypeCat == kAAFTypeCatWeakObjRef))
				{
					// Note that we simply transfer the base value over to the outer value for a rename
					// Note also that we clear the base value pointer to ensure the reference counting stays correct
					ReleaseRefs(pv_Value);
					pv_Value = pv_BaseValue;
					pv_BaseValue = NULL;
				}
				else
				{
					IAAFTypeDefRename *td = NULL;

					try
					{
						ReleaseRefs(pv_Value);

						hr = td_Property->QueryInterface(IID_IAAFTypeDefRename, (void **)&td);
						if(!FAILED(hr)) hr = td->CreateValue(pv_BaseValue, &pv_Value);

						if(FAILED(hr)) throw HRError(hr, "Failed to set outer value for TypeDefRename");
					}
					catch(...)
					{
						ReleaseRefs(td);
						throw;
					}
					ReleaseRefs(td);
				}

				break;
			}

		case kAAFTypeCatSet:
			{
				IAAFTypeDefSet *td = NULL;

				try
				{
					hr = td_Property->QueryInterface(IID_IAAFTypeDefSet, (void **)&td);
					if(!FAILED(hr)) hr = td->AddElement(pv_Value, pv_BaseValue);

					if(FAILED(hr)) throw HRError(hr, "Failed to set outer value for TypeDefSet");
				}
				catch(...)
				{
					ReleaseRefs(td);
					throw;
				}
				ReleaseRefs(td);

				break;
			}

		case kAAFTypeCatString:
			{
				IAAFTypeDefString *td = NULL;

				try
				{
					throw HRError(E_FAIL, "No applicable IAAFTypeDefString creation function");
				}
				catch(...)
				{
					ReleaseRefs(td);
					throw;
				}
				ReleaseRefs(td);
			
				break;
			}

		case kAAFTypeCatVariableArray:
			{
				IAAFTypeDefVariableArray *td = NULL;

				try
				{
					ReleaseRefs(pv_Value);

					hr = td_Property->QueryInterface(IID_IAAFTypeDefVariableArray, (void **)&td);
					if(!FAILED(hr)) hr = td->CreateEmptyValue(&pv_Value);
					if(!FAILED(hr)) hr = td->AppendElement(pv_Value, pv_BaseValue);

					if(FAILED(hr)) throw HRError(hr, "Failed to set outer value for TypeDefVariableArray");
				}
				catch(...)
				{
					ReleaseRefs(td);
					throw;
				}
				ReleaseRefs(td);
			
				break;
			}

		default:
			{
				char Buffer[128];
				sprintf(Buffer, "Unknown outer type category %d found", static_cast<int>(TypeCat));
				throw HRError(E_FAIL, Buffer);
			}
		}

		// Now we have built the value, we must set it
		hr = Object->SetPropertyValue( pd_Property, pv_Value);
	}
	catch(...)
	{
		ReleaseRefs(cd_Object);
		ReleaseRefs(pd_Property);
		ReleaseRefs(td_Property);
		ReleaseRefs(pv_BaseValue);
		ReleaseRefs(pv_Value);

		throw;
	}

	ReleaseRefs(cd_Object);
	ReleaseRefs(pd_Property);
	ReleaseRefs(td_Property);
	ReleaseRefs(pv_BaseValue);
	ReleaseRefs(pv_Value);

	return hr;
}


//! Verify the value of a given property
HRESULT VerifyProperty(IAAFPropertyDef *pd_Property, IAAFPropertyValue *pv_Value)
{
	HRESULT hr = S_OK;

	IAAFTypeDef *td_Property = NULL;
	IAAFTypeDef *td_Base = NULL;
	IAAFPropertyValue *pv_BaseValue = NULL;

	eAAFTypeCategory_t TypeCat;
	eAAFTypeCategory_t BaseTypeCat;

	try
	{
		// Get the type definition for this property
		hr = pd_Property->GetTypeDef(&td_Property);
		if(FAILED(hr)) throw HRError(hr, "Failed to locate type definition for property");

		// Get the type category of this property
		hr = td_Property->GetTypeCategory(&TypeCat);
		if(FAILED(hr)) throw HRError(hr, "Failed to identify the type category for property");

		/* Get the base type def */
		switch(TypeCat)
		{
		case kAAFTypeCatEnum:
			{
				IAAFTypeDefEnum *td = NULL;

				try
				{
					hr = td_Property->QueryInterface(IID_IAAFTypeDefEnum, (void **)&td);
					if(!FAILED(hr)) hr = td->GetElementType(&td_Base);

					if(FAILED(hr)) throw HRError(hr, "Failed to locate base type for TypeDefEnum");
				}
				catch(...)
				{
					ReleaseRefs(td);
					throw;
				}
				ReleaseRefs(td);
			
				break;
			}

		case kAAFTypeCatFixedArray:
			{
				IAAFTypeDefFixedArray *td = NULL;

				try
				{
					hr = td_Property->QueryInterface(IID_IAAFTypeDefFixedArray, (void **)&td);
					if(!FAILED(hr)) hr = td->GetType(&td_Base);

					if(FAILED(hr)) throw HRError(hr, "Failed to locate base type for TypeDefFixedArray");
				}
				catch(...)
				{
					ReleaseRefs(td);
					throw;
				}
				ReleaseRefs(td);
			
				break;
			}

		case kAAFTypeCatRecord:
			{
				IAAFTypeDefRecord *td = NULL;

				try
				{
					hr = td_Property->QueryInterface(IID_IAAFTypeDefRecord, (void **)&td);
					if(!FAILED(hr)) hr = td->GetMemberType(0, &td_Base);

					if(FAILED(hr)) throw HRError(hr, "Failed to locate base type for TypeDefRecord");
				}
				catch(...)
				{
					ReleaseRefs(td);
					throw;
				}
				ReleaseRefs(td);
			
				break;
			}

		case kAAFTypeCatRename:
			{
				IAAFTypeDefRename *td = NULL;

				try
				{
					hr = td_Property->QueryInterface(IID_IAAFTypeDefRename, (void **)&td);
					if(!FAILED(hr)) hr = td->GetBaseType(&td_Base);

					if(FAILED(hr)) throw HRError(hr, "Failed to locate base type for TypeDefRename");
				}
				catch(...)
				{
					ReleaseRefs(td);
					throw;
				}
				ReleaseRefs(td);
			
				break;
			}

		case kAAFTypeCatSet:
			{
				IAAFTypeDefSet *td = NULL;

				try
				{
					hr = td_Property->QueryInterface(IID_IAAFTypeDefSet, (void **)&td);
					if(!FAILED(hr)) hr = td->GetElementType(&td_Base);

					if(FAILED(hr)) throw HRError(hr, "Failed to locate base type for TypeDefSet");
				}
				catch(...)
				{
					ReleaseRefs(td);
					throw;
				}
				ReleaseRefs(td);
			
				break;
			}

		case kAAFTypeCatString:
			{
				IAAFTypeDefString *td = NULL;

				try
				{
					hr = td_Property->QueryInterface(IID_IAAFTypeDefString, (void **)&td);
					if(!FAILED(hr)) hr = td->GetType(&td_Base);

					if(FAILED(hr)) throw HRError(hr, "Failed to locate base type for TypeDefString");
				}
				catch(...)
				{
					ReleaseRefs(td);
					throw;
				}
				ReleaseRefs(td);
			
				break;
			}

		case kAAFTypeCatVariableArray:
			{
				IAAFTypeDefVariableArray *td = NULL;

				try
				{
					hr = td_Property->QueryInterface(IID_IAAFTypeDefVariableArray, (void **)&td);
					if(!FAILED(hr)) hr = td->GetType(&td_Base);

					if(FAILED(hr)) throw HRError(hr, "Failed to locate base type for TypeDefVariableArray");
				}
				catch(...)
				{
					ReleaseRefs(td);
					throw;
				}
				ReleaseRefs(td);
			
				break;
			}

		default:
			{
				char Buffer[128];
				sprintf(Buffer, "Unknown outer type category %d found", static_cast<int>(TypeCat));
				throw HRError(E_FAIL, Buffer);
			}
		}

		/* Get the base property value from the outer value */
		switch(TypeCat)
		{
		case kAAFTypeCatEnum:
			{
				// Note that we simply transfer the value over from the outer value for an enum
				// Note also that we bump the reference count as we have taken another reference
				pv_BaseValue = pv_Value;
				pv_BaseValue->AddRef();

				break;
			}

		case kAAFTypeCatFixedArray:
			{
				IAAFTypeDefFixedArray *td = NULL;

				try
				{
					hr = td_Property->QueryInterface(IID_IAAFTypeDefFixedArray, (void **)&td);
					if(!FAILED(hr)) hr = td->GetElementValue(pv_Value, 0, &pv_BaseValue);

					if(FAILED(hr)) throw HRError(hr, "Failed to get inner value for TypeDefFixedArray");
				}
				catch(...)
				{
					ReleaseRefs(td);
					throw;
				}
				ReleaseRefs(td);

				break;
			}

		case kAAFTypeCatRecord:
			{
				IAAFTypeDefRecord *td = NULL;

				try
				{
					hr = td_Property->QueryInterface(IID_IAAFTypeDefRecord, (void **)&td);
					if(!FAILED(hr)) hr = td->GetValue(pv_Value, 0, &pv_BaseValue);

					if(FAILED(hr)) throw HRError(hr, "Failed to get inner value for TypeDefRecord");
				}
				catch(...)
				{
					ReleaseRefs(td);
					throw;
				}
				ReleaseRefs(td);

				break;
			}

		case kAAFTypeCatRename:
			{
				IAAFTypeDefRename *td = NULL;

				try
				{
					hr = td_Property->QueryInterface(IID_IAAFTypeDefRename, (void **)&td);
					if(!FAILED(hr)) hr = td->GetBaseValue(pv_Value, &pv_BaseValue);

					if(FAILED(hr)) throw HRError(hr, "Failed to get inner value for TypeDefRename");
				}
				catch(...)
				{
					ReleaseRefs(td);
					throw;
				}
				ReleaseRefs(td);

				break;
			}
		case kAAFTypeCatSet:
			{
				IAAFTypeDefSet *td = NULL;
				IEnumAAFPropertyValues *Iter = NULL;

				try
				{
					aafUInt32 Count = 0;
					hr = td_Property->QueryInterface(IID_IAAFTypeDefSet, (void **)&td);
					if(!FAILED(hr)) hr = td->GetCount(pv_Value, &Count);
					if((!FAILED(hr)) && Count != 1) throw HRError(E_FAIL, "Wrong number of elements in TypeDefSet");
					if(!FAILED(hr)) hr = td->GetElements(pv_Value, &Iter);
					if(!FAILED(hr)) hr = Iter->NextOne(&pv_BaseValue);

					if(FAILED(hr)) throw HRError(hr, "Failed to get inner value for TypeDefSet");
				}
				catch(...)
				{
					ReleaseRefs(td);
					ReleaseRefs(Iter);
					throw;
				}
				ReleaseRefs(td);
				ReleaseRefs(Iter);

				break;
			}

		case kAAFTypeCatString:
			{
				IAAFTypeDefString *td = NULL;

				try
				{
					throw HRError(E_FAIL, "No applicable IAAFTypeDefString reading function");
				}
				catch(...)
				{
					ReleaseRefs(td);
					throw;
				}
				ReleaseRefs(td);
			
				break;
			}

		case kAAFTypeCatVariableArray:
			{
				IAAFTypeDefVariableArray *td = NULL;

				try
				{
					hr = td_Property->QueryInterface(IID_IAAFTypeDefVariableArray, (void **)&td);
					if(!FAILED(hr)) hr = td->GetElementValue(pv_Value, 0, &pv_BaseValue);

					if(FAILED(hr)) throw HRError(hr, "Failed to get inner value for TypeDefVariableArray");
				}
				catch(...)
				{
					ReleaseRefs(td);
					throw;
				}
				ReleaseRefs(td);
			
				break;
			}

		default:
			{
				char Buffer[128];
				sprintf(Buffer, "Unknown outer type category %d found", static_cast<int>(TypeCat));
				throw HRError(E_FAIL, Buffer);
			}
		}

		// Get the type category of the base type
		hr = td_Base->GetTypeCategory(&BaseTypeCat);
		if(FAILED(hr)) throw HRError(hr, "Failed to identify the type category for base type");


		/* Verify the base value */
		switch(BaseTypeCat)
		{
		case kAAFTypeCatInt:
			{
				IAAFTypeDefInt *td = NULL;

				try
				{
					aafInt32 Value = 0;

					hr = td_Base->QueryInterface(IID_IAAFTypeDefInt, (void **)&td);
					if(!FAILED(hr)) hr = td->GetInteger(pv_BaseValue, (aafMemPtr_t) &Value, sizeof(Value));
					if(FAILED(hr)) throw HRError(hr, "Failed to read property value for TypeDefInt");

					if(Value != 42) throw HRError(hr, "Value not correct");
				}
				catch(...)
				{
					ReleaseRefs(td);
					throw;
				}

				ReleaseRefs(td);

				break;
			}

		case kAAFTypeCatCharacter:
			{
				IAAFTypeDefCharacter *td = NULL;

				try
				{
					aafCharacter Value = 0;

					hr = td_Base->QueryInterface(IID_IAAFTypeDefCharacter, (void **)&td);
					if(!FAILED(hr)) hr = td->GetCharacter(pv_BaseValue, &Value);
					if(FAILED(hr)) throw HRError(hr, "Failed to read property value for TypeDefCharacter");

					if(Value != '*') throw HRError(hr, "Value not correct");
				}
				catch(...)
				{
					ReleaseRefs(td);
					throw;
				}

				ReleaseRefs(td);

				break;
			}

		case kAAFTypeCatEnum:
			{
				IAAFTypeDefEnum *td = NULL;

				try
				{
					aafInt64 Value;

					hr = td_Base->QueryInterface(IID_IAAFTypeDefEnum, (void **)&td);
					if(FAILED(hr)) throw HRError(hr, "Failed to query AAFTypeDefEnum interface for TypeDefEnum");

					hr = td->GetIntegerValue(pv_BaseValue, &Value);
					if(FAILED(hr)) throw HRError(hr, "Failed to read property value for TypeDefEnum");

					if(Value != 1) throw HRError(hr, "Value not correct");
				}
				catch(...)
				{
					ReleaseRefs(td);
					throw;
				}

				ReleaseRefs(td);

				break;
			}

		case kAAFTypeCatExtEnum:
			{
				IAAFTypeDefExtEnum *td = NULL;

				try
				{
					const aafUInt32 BufferSize = 1280;
					aafCharacter Buffer[BufferSize];

					hr = td_Base->QueryInterface(IID_IAAFTypeDefExtEnum, (void **)&td);
					if(FAILED(hr)) throw HRError(hr, "Failed to query AAFTypeDefExtEnum interface for TypeDefExtEnum");

					hr = td->GetNameFromValue(pv_BaseValue, Buffer, BufferSize);
					if(FAILED(hr)) throw HRError(hr, "Failed to read property value for TypeDefExtEnum");

					/* ASSUME IT IS OK IF WE READ IT!! */
				}
				catch(...)
				{
					ReleaseRefs(td);
					throw;
				}

				ReleaseRefs(td);

				break;
			}

		case kAAFTypeCatRename:
			{
				/* NOTE: Our test rename is of an Int32 */

				IAAFTypeDefRename *tdRename = NULL;
				IAAFTypeDef *tdActual = NULL;
				IAAFTypeDefInt *td = NULL;
				IAAFPropertyValue *pv_Integer = NULL;

				try
				{
					aafInt32 Value = 0;

					hr = td_Base->QueryInterface(IID_IAAFTypeDefRename, (void **)&tdRename);
                    if(!FAILED(hr)) hr = tdRename->GetBaseType(&tdActual);

					if(FAILED(hr)) throw HRError(hr, "Failed to get base type of TypeDefRename");

					hr = tdActual->QueryInterface(IID_IAAFTypeDefInt, (void **)&td);
					if(!FAILED(hr)) hr = tdRename->GetBaseValue(pv_BaseValue, &pv_Integer);
					if(!FAILED(hr)) hr = td->GetInteger(pv_Integer, (aafMemPtr_t) &Value, sizeof(Value));
					if(FAILED(hr)) throw HRError(hr, "Failed to read integer property value for TypeDefRename");

					if(Value != 42) throw HRError(hr, "Value not correct");
				}
				catch(...)
				{
					ReleaseRefs(tdRename);
					ReleaseRefs(tdActual);
					ReleaseRefs(td);
					ReleaseRefs(pv_Integer);
					throw;
				}

				ReleaseRefs(tdRename);
				ReleaseRefs(tdActual);
				ReleaseRefs(td);
				ReleaseRefs(pv_Integer);

				break;
			}

		case kAAFTypeCatFixedArray:
			{
				IAAFTypeDefFixedArray *td = NULL;
				IAAFPropertyValue *pv_Integer = NULL;
				IAAFTypeDef *td_BaseBase = NULL;
				IAAFTypeDefInt *td_Integer = NULL;

				try
				{
					aafInt32 Value = 0;

					hr = td_Base->QueryInterface(IID_IAAFTypeDefFixedArray, (void **)&td);
					if(FAILED(hr)) throw HRError(hr, "Failed to query AAFTypeDefFixedArray interface for TypeDefFixedArray");

					hr = td->GetElementValue(pv_BaseValue, 0, &pv_Integer);
					if(!FAILED(hr)) hr = td->GetType(&td_BaseBase);
					if(!FAILED(hr)) hr = td_BaseBase->QueryInterface(IID_IAAFTypeDefInt, (void **)&td_Integer);
					if(!FAILED(hr)) hr = td_Integer->GetInteger(pv_Integer, (aafMemPtr_t) &Value, sizeof(Value));
					if(FAILED(hr)) throw HRError(hr, "Failed to read integer property value for TypeDefFixedArray");

					if(Value != 42) throw HRError(hr, "Value not correct");
				}
				catch(...)
				{
					ReleaseRefs(td);
					ReleaseRefs(pv_Integer);
					ReleaseRefs(td_BaseBase);
					ReleaseRefs(td_Integer);
					throw;
				}

				ReleaseRefs(td);
				ReleaseRefs(pv_Integer);
				ReleaseRefs(td_BaseBase);
				ReleaseRefs(td_Integer);

				break;
			}

		case kAAFTypeCatVariableArray:
			{
				IAAFTypeDefVariableArray *td = NULL;
				IAAFPropertyValue *pv_Integer = NULL;
				IAAFTypeDef *td_BaseBase = NULL;
				IAAFTypeDefInt *td_Integer = NULL;

				try
				{
					aafInt32 Value = 0;

					hr = td_Base->QueryInterface(IID_IAAFTypeDefVariableArray, (void **)&td);
					if(FAILED(hr)) throw HRError(hr, "Failed to query AAFTypeDefVariableArray interface for TypeDefVariableArray");

					hr = td->GetElementValue(pv_BaseValue, 0, &pv_Integer);
					if(!FAILED(hr)) hr = td->GetType(&td_BaseBase);
					if(!FAILED(hr)) hr = td_BaseBase->QueryInterface(IID_IAAFTypeDefInt, (void **)&td_Integer);
					if(!FAILED(hr)) hr = td_Integer->GetInteger(pv_Integer, (aafMemPtr_t) &Value, sizeof(Value));
					if(FAILED(hr)) throw HRError(hr, "Failed to read integer property value for TypeDefVariableArray");

					if(Value != 42) throw HRError(hr, "Value not correct");
				}
				catch(...)
				{
					ReleaseRefs(td);
					ReleaseRefs(pv_Integer);
					ReleaseRefs(td_BaseBase);
					ReleaseRefs(td_Integer);
					throw;
				}

				ReleaseRefs(td);
				ReleaseRefs(pv_Integer);
				ReleaseRefs(td_BaseBase);
				ReleaseRefs(td_Integer);

				break;
			}

		case kAAFTypeCatRecord:
			{
				IAAFTypeDefRecord *td = NULL;
				IAAFPropertyValue *pv_Integer = NULL;
				IAAFTypeDef *td_BaseBase = NULL;
				IAAFTypeDefInt *td_Integer = NULL;

				try
				{
					aafInt32 Value = 0;

					hr = td_Base->QueryInterface(IID_IAAFTypeDefRecord, (void **)&td);
					if(FAILED(hr)) throw HRError(hr, "Failed to query AAFTypeDefRecord interface for TypeDefRecord");

					hr = td->GetValue(pv_BaseValue, 0, &pv_Integer);
					if(!FAILED(hr)) hr = td->GetMemberType(0, &td_BaseBase);
					if(!FAILED(hr)) hr = td_BaseBase->QueryInterface(IID_IAAFTypeDefInt, (void **)&td_Integer);
					if(!FAILED(hr)) hr = td_Integer->GetInteger(pv_Integer, (aafMemPtr_t) &Value, sizeof(Value));
					if(FAILED(hr)) throw HRError(hr, "Failed to read integer property value for TypeDefRecord");

					if(Value != 42) throw HRError(hr, "Value not correct");
				}
				catch(...)
				{
					ReleaseRefs(td);
					ReleaseRefs(pv_Integer);
					ReleaseRefs(td_BaseBase);
					ReleaseRefs(td_Integer);
					throw;
				}

				ReleaseRefs(td);
				ReleaseRefs(pv_Integer);
				ReleaseRefs(td_BaseBase);
				ReleaseRefs(td_Integer);

				break;
			}

		case kAAFTypeCatSet:
			{
				IAAFTypeDefSet *td = NULL;
				IAAFPropertyValue *pv_Element = NULL;
				IAAFTypeDef *td_Element = NULL;
				IAAFTypeDefObjectRef *td_Ref = NULL;
				IEnumAAFPropertyValues *Iter = NULL;
				IAAFObject *pObject = NULL;

				try
				{
					hr = td_Base->QueryInterface(IID_IAAFTypeDefSet, (void **)&td);
					if(FAILED(hr)) throw HRError(hr, "Failed to query AAFTypeDefSet interface for TypeDefSet");

					hr = td->GetElementType(&td_Element);
					if(FAILED(hr)) throw HRError(hr, "Failed to get element type for TypeDefSet");

					hr = td_Element->QueryInterface(IID_IAAFTypeDefObjectRef, (void **)&td_Ref);
					if(FAILED(hr)) throw HRError(hr, "Failed to query AAFTypeDefObjectRef interface of element type for TypeDefSet");

					hr = td->GetElements(pv_BaseValue, &Iter);
					if(!FAILED(hr)) hr = Iter->NextOne(&pv_Element);
					if(!FAILED(hr)) hr = td_Ref->GetObject(pv_Element, IID_IAAFObject, (IUnknown**)&pObject);
					if(FAILED(hr)) throw HRError(hr, "Failed to read strong reference property value for TypeDefSet");
				
					/* ASSUME ALL IS OK BY HERE */
				}
				catch(...)
				{
					ReleaseRefs(td);
					ReleaseRefs(pv_Element);
					ReleaseRefs(td_Element);
					ReleaseRefs(td_Ref);
					ReleaseRefs(Iter);
					ReleaseRefs(pObject);
					throw;
				}

				ReleaseRefs(td);
				ReleaseRefs(pv_Element);
				ReleaseRefs(td_Element);
				ReleaseRefs(td_Ref);
				ReleaseRefs(Iter);
				ReleaseRefs(pObject);

				break;
			}

		case kAAFTypeCatString:
			{
				IAAFTypeDefString *td = NULL;

				try
				{
					aafCharacter Value[2];

					hr = td_Base->QueryInterface(IID_IAAFTypeDefString, (void **)&td);
					if(FAILED(hr)) throw HRError(hr, "Failed to query AAFTypeDefString interface for TypeDefString");

					hr = td->GetElements(pv_BaseValue, (aafMemPtr_t) &Value, sizeof(Value));
					if(FAILED(hr)) throw HRError(hr, "Failed to read property value for TypeDefString");

					if(Value[0] != 42) throw HRError(hr, "Value not correct");
				}
				catch(...)
				{
					ReleaseRefs(td);
					throw;
				}

				ReleaseRefs(td);

				break;
			}

		case kAAFTypeCatIndirect:
			{
				IAAFTypeDefIndirect *td = NULL;
				IAAFTypeDefInt *td_Integer = NULL;
				IAAFTypeDef *tdSubType = NULL;
				IAAFPropertyValue *pv_Actual = NULL;

				try
				{
					aafInt32 Value = 0;

					hr = td_Base->QueryInterface(IID_IAAFTypeDefIndirect, (void **)&td);
					if(FAILED(hr)) throw HRError(hr, "Failed to query AAFTypeDefIndirect interface for TypeDefIndirect");

					hr = td->GetActualType(pv_BaseValue, &tdSubType);
					if(FAILED(hr)) throw HRError(hr, "Failed to get actual type of TypeDefIndirect");

					hr = tdSubType->QueryInterface(IID_IAAFTypeDefInt, (void **)&td_Integer);
					if(FAILED(hr)) throw HRError(hr, "Failed to get AAFTypeDefInt interface for actual type of TypeDefIndirect");

					hr = td->GetActualValue(pv_BaseValue, &pv_Actual);
					if(FAILED(hr)) throw HRError(hr, "Failed to get actual value of TypeDefIndirect");

					hr = td_Integer->GetInteger(pv_Actual, (aafMemPtr_t) &Value, sizeof(Value));
					if(FAILED(hr)) throw HRError(hr, "Failed to read integer property value for TypeDefIndirect");

					if(Value != 42) throw HRError(hr, "Value not correct");
				}
				catch(...)
				{
					ReleaseRefs(td);
					ReleaseRefs(tdSubType);
					ReleaseRefs(pv_Value);
					ReleaseRefs(td_Integer);
					throw;
				}

				ReleaseRefs(td);
				ReleaseRefs(tdSubType);
				ReleaseRefs(pv_Value);
				ReleaseRefs(td_Integer);

				break;
			}
#if 0
		case kAAFTypeCatOpaque:
			{
				IAAFTypeDefOpaque *td = NULL;
				IAAFTypeDefIndirect *tdIndirect = NULL;

				try
				{
					aafInt32 Value = 42;

					hr = td_Base->QueryInterface(IID_IAAFTypeDefOpaque, (void **)&td);
					if(FAILED(hr)) throw HRError(hr, "Failed to query AAFTypeDefOpaque interface for TypeDefOpaque");

					hr = td->QueryInterface(IID_IAAFTypeDefIndirect, (void **)&tdIndirect);
					if(FAILED(hr)) throw HRError(hr, "Failed to query AAFTypeDefIndirect interface for TypeDefOpaque");

					hr = tdIndirect->CreateValueFromActualData(td_Base, (aafMemPtr_t) &Value, sizeof(Value), &pv_BaseValue);

					if(FAILED(hr)) throw HRError(hr, "Failed to create property value for TypeDefOpaque");
				}
				catch(...)
				{
					ReleaseRefs(td);
					ReleaseRefs(tdIndirect);
					throw;
				}

				ReleaseRefs(td);
				ReleaseRefs(tdIndirect);

				break;
			}

		case kAAFTypeCatStream:
			{
				IAAFTypeDefStream *td = NULL;

				try
				{
					aafInt32 Value = 42;

					hr = td_Base->QueryInterface(IID_IAAFTypeDefStream, (void **)&td);
					if(FAILED(hr)) throw HRError(hr, "Failed to query AAFTypeDefStream interface for TypeDefStream");

					// We need to build this value in the property!
					hr = Object->GetPropertyValue( pd_Property, &pv_BaseValue);
					if(hr == AAFRESULT_PROP_NOT_PRESENT)
					{
						hr = Object->CreateOptionalPropertyValue( pd_Property, &pv_BaseValue);
					}
					if(FAILED(hr)) throw HRError(hr, "Failed to locate or create property value");

					hr = td->Write(pv_BaseValue, sizeof(Value), (aafMemPtr_t) &Value);

					if(FAILED(hr)) throw HRError(hr, "Failed to create property value for TypeDefStream");
				}
				catch(...)
				{
					ReleaseRefs(td);
					throw;
				}

				ReleaseRefs(td);

				break;
			}
#endif // 0
		case kAAFTypeCatStrongObjRef:
			{
				IAAFTypeDefObjectRef *td = NULL;
				IAAFObject *pObject = NULL;

				try
				{
					hr = td_Base->QueryInterface(IID_IAAFTypeDefObjectRef, (void **)&td);
					if(FAILED(hr)) throw HRError(hr, "Failed to query AAFTypeDefObjectRef interface for TypeDefStrongObjRef");

					hr = td->GetObject(pv_BaseValue, IID_IAAFObject, (IUnknown**)&pObject);
					if(FAILED(hr)) throw HRError(hr, "Failed to read strong reference property value for TypeDefStrongObjRef");

					/* ASSUME ALL IS OK BY HERE */
				}
				catch(...)
				{
					ReleaseRefs(td);
					ReleaseRefs(pObject);
					throw;
				}

				ReleaseRefs(td);
				ReleaseRefs(pObject);

				break;
			}

		case kAAFTypeCatWeakObjRef:
			{
				IAAFTypeDefObjectRef *td = NULL;
				IAAFObject *pObject = NULL;

				try
				{
					hr = td_Base->QueryInterface(IID_IAAFTypeDefObjectRef, (void **)&td);
					if(FAILED(hr)) throw HRError(hr, "Failed to query AAFTypeDefObjectRef interface for TypeDefWeakObjRef");

					hr = td->GetObject(pv_BaseValue, IID_IAAFObject, (IUnknown**)&pObject);
					if(FAILED(hr)) throw HRError(hr, "Failed to read weak reference property value for TypeDefWeakObjRef");

					/* ASSUME ALL IS OK BY HERE */
				}
				catch(...)
				{
					ReleaseRefs(td);
					ReleaseRefs(pObject);
					throw;
				}

				ReleaseRefs(td);
				ReleaseRefs(pObject);

				break;
			}

		default:
			{
				char Buffer[128];
				sprintf(Buffer, "Unknown type category %d found", static_cast<int>(BaseTypeCat));
				throw HRError(E_FAIL, Buffer);
			}
		}

	}
	catch(...)
	{
		ReleaseRefs(td_Property);
		ReleaseRefs(pv_BaseValue);

		throw;
	}

	ReleaseRefs(td_Property);
	ReleaseRefs(pv_BaseValue);

	return hr;
}


//! Register items required for our internal use
/*! Registers TestDef which is a concrete sub-class of DefinitionObject
 */
HRESULT RegisterRequiredItems(IAAFDictionary *Dict)
{
	// Ensure one-time only initialization
	static Inited = false;
	if(Inited) return S_OK;
	Inited = true;

	IAAFClassDef *cd_DefObj = NULL;
	IAAFClassDef *cd_TestDef = NULL;

	HRESULT hr = S_OK;

	
	/* Register a test definition class for use as a target of test strong references */

	try
	{
		hr = Dict->LookupClassDef(AUID_AAFDefinitionObject, &cd_DefObj);
		if(!FAILED(hr)) hr = Dict->CreateMetaInstance(AUID_AAFClassDef, IID_IAAFClassDef, (IUnknown **)&cd_TestDef);
		if(!FAILED(hr)) hr = cd_TestDef->Initialize(kClassAUID_TestDef, cd_DefObj, kClassName_TestDef, kAAFTrue);
		if(FAILED(hr)) throw HRError(hr, "Failed to sub-class DefinitionObject to make TestDef");

		hr = Dict->RegisterClassDef(cd_TestDef);
		if(FAILED(hr)) throw HRError(hr, "Failed to register TestDef class");
	}
	catch(HRError HR)
	{
		hr = HR.hr;
	}
	catch(...)
	{
		hr = E_FAIL;
	}

	ReleaseRefs(cd_DefObj);
	ReleaseRefs(cd_TestDef);

	if(FAILED(hr)) return hr;


	/* Register the variable array of AUIDs type required to hold the list of property IDs */

	IAAFTypeDefVariableArray *td_VariableArray = NULL;
	IAAFTypeDef *td_PropertyArray = NULL;
	IAAFTypeDef *td_AUID = NULL;

	try
	{
		hr = Dict->LookupTypeDef(kAAFTypeID_AUID, &td_AUID);
		if(FAILED(hr)) throw HRError(hr, "Failed to locate TypeDef for AUID type");

		hr = RegisterTypeInternal(Dict, AUID_AAFTypeDefVariableArray, reinterpret_cast<IUnknown **>(&td_VariableArray));
		if(!FAILED(hr)) hr = td_VariableArray->Initialize(RandomID(), td_AUID, L"Variable Array of AUIDs");
		if(FAILED(hr)) throw HRError(hr, "Failed to register or initialize Variable Array TypeDef required for internal use");
	
		// Register this new type
		hr = td_VariableArray->QueryInterface(IID_IAAFTypeDef, reinterpret_cast<void **>(&td_PropertyArray));
		if(!FAILED(hr)) hr = Dict->RegisterTypeDef(td_PropertyArray);
		if(FAILED(hr)) throw HRError(hr, "Failed to register new TypeDef of VariableArray of AUIDs");
	}
	catch(...)
	{
		ReleaseRefs(td_AUID);
		ReleaseRefs(td_VariableArray);
		ReleaseRefs(td_PropertyArray);

		throw;
	}

	// Note: We no longer need the specific array interface
	ReleaseRefs(td_AUID);
	ReleaseRefs(td_VariableArray);

	if(FAILED(hr)) return hr;


	/* Register our test description class which holds a list of all test properties added to the file */

	IAAFPropertyDef *pd_NewProp = NULL;
	IAAFTypeDef *td_PropDef = NULL;
	IAAFTypeDef *td_TypeDef = NULL;
	IAAFTypeDefStrongObjRef *td_StrongRef = NULL;
	IAAFClassDef *cd_Header = NULL;

	try
	{
		hr = Dict->LookupClassDef(AUID_AAFDefinitionObject, &cd_DefObj);
		if(!FAILED(hr)) hr = Dict->CreateMetaInstance(AUID_AAFClassDef, IID_IAAFClassDef, (IUnknown **)&cd_TestDef);
		if(!FAILED(hr)) hr = cd_TestDef->Initialize(TestDescriptionClassID, cd_DefObj, TestDescriptionClassName, kAAFTrue);
		if(FAILED(hr)) throw HRError(hr, "Failed to sub-class DefinitionObject to make TestDescriptionClass");

		// Register the test version number
		hr = Dict->LookupTypeDef(kAAFTypeID_Int32, &td_PropDef);
		if(!FAILED(hr)) hr = cd_TestDef->RegisterNewPropertyDef(TestVersionPropID, TestVersionPropName, td_PropDef, false, false, &pd_NewProp);
		if(FAILED(hr)) throw HRError(hr, "Failed to register version number property in test description class");
		ReleaseRefs(td_PropDef);
		ReleaseRefs(pd_NewProp);
 
		// Register the list of property IDs
		hr = cd_TestDef->RegisterNewPropertyDef(TestPropsPropID, TestPropsPropName, td_PropertyArray, false, false, &pd_NewProp);
		if(FAILED(hr)) throw HRError(hr, "Failed to register property ID list property in test description class");
		ReleaseRefs(pd_NewProp);

		// Register the list of property names
		hr = Dict->LookupTypeDef(kAAFTypeID_String, &td_PropDef);
		if(!FAILED(hr)) hr = cd_TestDef->RegisterNewPropertyDef(TestNamesPropID, TestNamesPropName, td_PropDef, false, false, &pd_NewProp);
		if(FAILED(hr)) throw HRError(hr, "Failed to register property names property in test description class");
		ReleaseRefs(td_PropDef);
		ReleaseRefs(pd_NewProp);

		// Register the new class
		hr = Dict->RegisterClassDef(cd_TestDef);
		if(FAILED(hr)) throw HRError(hr, "Failed to register TestDescriptionClass class");

		// Add a new TypeDef of a strong reference to the new class
		hr = RegisterTypeInternal(Dict, AUID_AAFTypeDefStrongObjRef, reinterpret_cast<IUnknown **>(&td_StrongRef));
		if(!FAILED(hr)) hr = td_StrongRef->Initialize(RandomID(), cd_TestDef, L"Strong Reference to Description Object");
		if(FAILED(hr)) throw HRError(hr, "Failed to register or initialize TypeDef of strong ref to Description Object");

		// Register this new type
		hr = td_StrongRef->QueryInterface(IID_IAAFTypeDef, reinterpret_cast<void **>(&td_TypeDef));
		if(!FAILED(hr)) hr = Dict->RegisterTypeDef(td_TypeDef);
		if(FAILED(hr)) throw HRError(hr, "Failed to register TypeDef of strong ref to Description Object");

		// Add a strong reference to the description class from the header
		hr = Dict->LookupClassDef(kAAFClassID_Header, &cd_Header);
		if(!FAILED(hr)) hr = cd_Header->RegisterOptionalPropertyDef(TestReferencePropID, TestReferencePropName, td_TypeDef, &pd_NewProp);
	}
	catch(...)
	{
		ReleaseRefs(cd_DefObj);
		ReleaseRefs(cd_TestDef);
		ReleaseRefs(td_PropDef);
		ReleaseRefs(pd_NewProp);
		ReleaseRefs(cd_Header);
		ReleaseRefs(td_TypeDef);
		ReleaseRefs(td_StrongRef);
		ReleaseRefs(td_PropertyArray);

		throw;
	}

	ReleaseRefs(cd_DefObj);
	ReleaseRefs(cd_TestDef);
	ReleaseRefs(td_PropDef);
	ReleaseRefs(pd_NewProp);
	ReleaseRefs(cd_Header);
	ReleaseRefs(td_TypeDef);
	ReleaseRefs(td_StrongRef);
	ReleaseRefs(td_PropertyArray);

	return hr;
}


//! Add the test description object to the file
HRESULT AddDescription(IAAFHeader *Header)
{
	HRESULT hr = S_OK;
	
	IAAFObject *pHeader = NULL;
	IAAFClassDef *cd_Header = NULL;
	IAAFClassDef *cd_DescObj = NULL;
	IAAFDefObject *pDefObj = NULL;
	IAAFObject *pDescObj = NULL;
	IUnknown *iu_DescObj = NULL;
	IAAFDictionary *Dict = NULL;
	IAAFPropertyDef *pd_PropDef = NULL;
	IAAFTypeDef *td_TypeDef = NULL;
	IAAFPropertyValue *pv_PropValue = NULL;

	try
	{
		// Get the dictionary
		hr = Header->GetDictionary(&Dict);
		if(FAILED(hr)) throw HRError(hr, "Failed to get dictionary from header");

		// Get the generic IAAFObject intefrace to the header
		hr = Header->QueryInterface(IID_IAAFObject, reinterpret_cast<void **>(&pHeader));
		if(FAILED(hr)) throw HRError(hr, "Failed to get IAAFObject interface for header");

		// Build a new test description object
		hr = Dict->LookupClassDef(TestDescriptionClassID, &cd_DescObj);
		if(!FAILED(hr)) hr = cd_DescObj->CreateInstance(IID_IAAFDefObject, reinterpret_cast<IUnknown **>(&pDefObj));
		if(FAILED(hr)) throw HRError(hr, "Failed to create a test description object");

		hr = pDefObj->Initialize(RandomID(), L"Test Description Object");
		if(FAILED(hr)) throw HRError(hr, "Failed to initialize a test description object");

		// Get an IUnknown interface for use later
		hr = pDefObj->QueryInterface(IID_IUnknown, reinterpret_cast<void **>(&iu_DescObj));
		if(FAILED(hr)) throw HRError(hr, "Failed to query IUnknown interface of test description object");

		// Get an AAFObject interface
		hr = pDefObj->QueryInterface(IID_IAAFObject, reinterpret_cast<void **>(&pDescObj));
		if(FAILED(hr)) throw HRError(hr, "Failed to query AAFObject interface of test description object");

		/* Add our test version number property */
		{
			IAAFTypeDefInt *td_Integer = NULL;
			
			try
			{
				hr = cd_DescObj->LookupPropertyDef(TestVersionPropID, &pd_PropDef);
				if(!FAILED(hr)) hr = pd_PropDef->GetTypeDef(&td_TypeDef);
				if(!FAILED(hr)) hr = td_TypeDef->QueryInterface(IID_IAAFTypeDefInt, reinterpret_cast<void **>(&td_Integer));
				if(!FAILED(hr)) hr = td_Integer->CreateValue((aafMemPtr_t)&TestVersion, sizeof(TestVersion), &pv_PropValue);
				if(!FAILED(hr)) hr = pDescObj->SetPropertyValue(pd_PropDef, pv_PropValue);
			}
			catch(...)
			{
				ReleaseRefs(td_Integer);
				throw;
			}
			ReleaseRefs(td_TypeDef);
			ReleaseRefs(td_Integer);
			ReleaseRefs(pv_PropValue);
			ReleaseRefs(pd_PropDef);
		}
		if(FAILED(hr)) throw HRError(hr, "Failed to add test version number property to description object");

		/* Add the list of property IDs */
		{
			IAAFTypeDefVariableArray *td_VariableArray = NULL;
			IAAFTypeDefRecord *td_Element = NULL;
			IAAFPropertyValue *pv_Element = NULL;

			try
			{
				hr = cd_DescObj->LookupPropertyDef(TestPropsPropID, &pd_PropDef);
				if(!FAILED(hr)) hr = pd_PropDef->GetTypeDef(&td_TypeDef);
				if(!FAILED(hr)) hr = td_TypeDef->QueryInterface(IID_IAAFTypeDefVariableArray, reinterpret_cast<void **>(&td_VariableArray));
				ReleaseRefs(td_TypeDef);
				if(!FAILED(hr)) hr = td_VariableArray->GetType(&td_TypeDef);
				if(!FAILED(hr)) hr = td_TypeDef->QueryInterface(IID_IAAFTypeDefRecord, reinterpret_cast<void **>(&td_Element));
				if(!FAILED(hr)) hr = td_VariableArray->CreateEmptyValue(&pv_PropValue);

				if(!FAILED(hr))
				{
					std::list<PropertyInfo>::iterator it = PropertyList.begin();
					while(it != PropertyList.end())
					{
						hr = td_Element->CreateValueFromStruct((aafMemPtr_t) &((*it).ID), sizeof(aafUID_t), &pv_Element);
						if(!FAILED(hr)) hr = td_VariableArray->AppendElement(pv_PropValue, pv_Element);
						if(FAILED(hr)) throw HRError(hr, "Failed to set element in Property ID list in test description object");

						ReleaseRefs(pv_Element);

						it++;
					}
				}
				if(!FAILED(hr)) hr = pDescObj->SetPropertyValue(pd_PropDef, pv_PropValue);
			}
			catch(...)
			{
				ReleaseRefs(td_VariableArray);
				ReleaseRefs(td_Element);
				ReleaseRefs(pv_Element);
				throw;
			}
			ReleaseRefs(td_TypeDef);
			ReleaseRefs(td_VariableArray);
			ReleaseRefs(td_Element);
			ReleaseRefs(pv_PropValue);
			ReleaseRefs(pv_Element);
			ReleaseRefs(pd_PropDef);
		}
		if(FAILED(hr)) throw HRError(hr, "Failed to add Property ID list to test description object");

		/* Add the list of property names */
		{
			IAAFTypeDefString *td_String = NULL;
			
			try
			{
				hr = cd_DescObj->LookupPropertyDef(TestNamesPropID, &pd_PropDef);
				if(!FAILED(hr)) hr = pd_PropDef->GetTypeDef(&td_TypeDef);
				if(!FAILED(hr)) hr = td_TypeDef->QueryInterface(IID_IAAFTypeDefString, reinterpret_cast<void **>(&td_String));

				if(!FAILED(hr))
				{
					// Get the concatinated list of names
					std::string NameList = ProcessNameList();

					// Convert to an aafString_t
					aafCharacter *Buffer = new aafCharacter[NameList.length() + 1];
					aafCharacter *p = Buffer;
					std::string::iterator it = NameList.begin();
					while(it != NameList.end())
					{
						*(p++) = *(it++);
					}
					*p = 0;

					try
					{
						hr = td_String->CreateValueFromCString((aafMemPtr_t)Buffer, (NameList.length() + 1) * sizeof(aafCharacter), &pv_PropValue);
					}
					catch(...)
					{
						delete[] Buffer;
						throw;
					}

					delete[] Buffer;
				}
				if(!FAILED(hr)) hr = pDescObj->SetPropertyValue(pd_PropDef, pv_PropValue);
			}
			catch(...)
			{
				ReleaseRefs(td_String);
				throw;
			}
			ReleaseRefs(td_TypeDef);
			ReleaseRefs(td_String);
			ReleaseRefs(pv_PropValue);
			ReleaseRefs(pd_PropDef);
		}
		if(FAILED(hr)) throw HRError(hr, "Failed to add Property names list to test description object");


		/* Build a strong reference value to link this test description object to the header */
		{
			IAAFTypeDefObjectRef *td_Ref = NULL;
		
			try
			{
				hr = pHeader->GetDefinition(&cd_Header);
				if(!FAILED(hr)) hr = cd_Header->LookupPropertyDef(TestReferencePropID, &pd_PropDef);
				if(!FAILED(hr)) hr = pd_PropDef->GetTypeDef(&td_TypeDef);
				if(!FAILED(hr)) hr = td_TypeDef->QueryInterface(IID_IAAFTypeDefObjectRef, reinterpret_cast<void **>(&td_Ref));
				if(!FAILED(hr)) hr = td_Ref->CreateValue(iu_DescObj, &pv_PropValue);
				if(FAILED(hr)) throw HRError(hr, "Failed to create strong reference property to link to description object");

				// Set the value into the header
				hr = pHeader->SetPropertyValue(pd_PropDef, pv_PropValue);
				if(FAILED(hr)) throw HRError(hr, "Failed to set strong reference property to link to description object");
			}
			catch(...)
			{
				ReleaseRefs(td_Ref);
				throw;
			}
			ReleaseRefs(td_Ref);
		}
	}
	catch(...)
	{
		ReleaseRefs(pHeader);
		ReleaseRefs(cd_Header);
		ReleaseRefs(cd_DescObj);
		ReleaseRefs(pDefObj);
		ReleaseRefs(iu_DescObj);
		ReleaseRefs(pDescObj);
		ReleaseRefs(Dict);
		ReleaseRefs(pd_PropDef);
		ReleaseRefs(td_TypeDef);
		ReleaseRefs(pv_PropValue);

		throw;
	}

	ReleaseRefs(pHeader);
	ReleaseRefs(cd_Header);
	ReleaseRefs(cd_DescObj);
	ReleaseRefs(pDefObj);
	ReleaseRefs(iu_DescObj);
	ReleaseRefs(pDescObj);
	ReleaseRefs(Dict);
	ReleaseRefs(pd_PropDef);
	ReleaseRefs(td_TypeDef);
	ReleaseRefs(pv_PropValue);

	return hr;
}


//! Read the test description object from the file
HRESULT ReadDescription(IAAFHeader *Header)
{
	HRESULT hr = S_OK;
	
	PropertyList.clear();

	IAAFObject *pHeader = NULL;
	IAAFClassDef *cd_Header = NULL;
	IAAFClassDef *cd_DescObj = NULL;
	IAAFObject *pDescObj = NULL;
	IAAFPropertyDef *pd_PropDef = NULL;
	IAAFTypeDef *td_TypeDef = NULL;
	IAAFPropertyValue *pv_PropValue = NULL;

	try
	{
		// Get the generic IAAFObject intefrace to the header
		hr = Header->QueryInterface(IID_IAAFObject, reinterpret_cast<void **>(&pHeader));
		if(FAILED(hr)) throw HRError(hr, "Failed to get IAAFObject interface for header");

		hr = pHeader->GetDefinition(&cd_Header);
		if(FAILED(hr)) throw HRError(hr, "Failed to get ClassDef of header");

		/* Locate the strong reference to the test description object from the header */
		{
			IAAFTypeDefObjectRef *td_Ref = NULL;

			try
			{
				hr = cd_Header->LookupPropertyDef(TestReferencePropID, &pd_PropDef);
				if(!FAILED(hr)) hr = pHeader->GetPropertyValue(pd_PropDef, &pv_PropValue);
				if(hr == AAFRESULT_PROP_NOT_PRESENT) throw HRError(hr, "No Test Description Object - probably not a MetaModelVerification file");

				if(!FAILED(hr)) hr = pv_PropValue->GetType(&td_TypeDef);
				if(!FAILED(hr)) hr = td_TypeDef->QueryInterface(IID_IAAFTypeDefObjectRef, reinterpret_cast<void **>(&td_Ref));
				if(!FAILED(hr)) hr = td_Ref->GetObject(pv_PropValue, IID_IAAFObject, reinterpret_cast<IUnknown **>(&pDescObj));
			}
			catch(...)
			{
				ReleaseRefs(td_Ref);
				throw;
			}
			ReleaseRefs(td_Ref);
			ReleaseRefs(pd_PropDef);
			ReleaseRefs(pv_PropValue);
			ReleaseRefs(td_TypeDef);
		}
		if(FAILED(hr)) throw HRError(hr, "Failed to follow strong reference property to link to description object");

		hr = pDescObj->GetDefinition(&cd_DescObj);
		if(FAILED(hr)) throw HRError(hr, "Failed to get ClassDef of test description object");

		/* Check the test version number property */
		{
			IAAFTypeDefInt *td_Integer = NULL;
			aafUInt32 Version = 0;

			try
			{
				hr = cd_DescObj->LookupPropertyDef(TestVersionPropID, &pd_PropDef);
				if(!FAILED(hr)) hr = pDescObj->GetPropertyValue(pd_PropDef, &pv_PropValue);
				if(!FAILED(hr)) hr = pv_PropValue->GetType(&td_TypeDef);
				if(!FAILED(hr)) hr = td_TypeDef->QueryInterface(IID_IAAFTypeDefInt, reinterpret_cast<void **>(&td_Integer));
				if(!FAILED(hr)) hr = td_Integer->GetInteger(pv_PropValue, (aafMemPtr_t)&Version, sizeof(Version));

				if(!FAILED(hr))
				{
					if(Version > TestVersion)
					{ 
						char Buffer[256];
						std::string Error;
						sprintf(Buffer, "Our version number: %d, test file version number: %d", (int)TestVersion, (int)Version);
						throw HRError(E_FAIL, std::string("Test file is from a newer test - ") + std::string(Buffer));
					}
				}
			}
			catch(...)
			{
				ReleaseRefs(td_Integer);
				throw;
			}
			ReleaseRefs(td_Integer);
			ReleaseRefs(pd_PropDef);
			ReleaseRefs(pv_PropValue);
			ReleaseRefs(td_TypeDef);
		}
		if(FAILED(hr)) throw HRError(hr, "Failed checking test version number in description object");


		/* Read the list of property IDs */
		{
			IAAFTypeDefVariableArray *td_VariableArray = NULL;
			IAAFTypeDefRecord *td_Element = NULL;
			IAAFPropertyValue *pv_Element = NULL;
			IEnumAAFPropertyValues *Iter = NULL;

			try
			{
				hr = cd_DescObj->LookupPropertyDef(TestPropsPropID, &pd_PropDef);
				if(!FAILED(hr)) hr = pDescObj->GetPropertyValue(pd_PropDef, &pv_PropValue);
				if(!FAILED(hr)) hr = pv_PropValue->GetType(&td_TypeDef);
				if(!FAILED(hr)) hr = td_TypeDef->QueryInterface(IID_IAAFTypeDefVariableArray, reinterpret_cast<void **>(&td_VariableArray));
				ReleaseRefs(td_TypeDef);
				if(!FAILED(hr)) hr = td_VariableArray->GetType(&td_TypeDef);
				if(!FAILED(hr)) hr = td_TypeDef->QueryInterface(IID_IAAFTypeDefRecord, reinterpret_cast<void **>(&td_Element));
				if(!FAILED(hr)) hr = td_VariableArray->GetElements(pv_PropValue, &Iter);

				if(!FAILED(hr))
				{
					for(;;)
					{
						hr = Iter->NextOne(&pv_Element);
						if(hr == AAFRESULT_NO_MORE_OBJECTS)
						{
							hr = S_OK;
							break;
						}

						if(!FAILED(hr))
						{
							PropertyInfo Info;

							hr = td_Element->GetStruct(pv_Element, (aafMemPtr_t)&Info.ID, sizeof(aafUID_t));
							if(!FAILED(hr)) PropertyList.push_back(Info);
						}
					}
				}
			}
			catch(...)
			{
				ReleaseRefs(td_VariableArray);
				ReleaseRefs(td_Element);
				ReleaseRefs(pv_Element);
				ReleaseRefs(Iter);
				throw;
			}
			ReleaseRefs(td_VariableArray);
			ReleaseRefs(td_Element);
			ReleaseRefs(pv_Element);
			ReleaseRefs(pd_PropDef);
			ReleaseRefs(pv_PropValue);
			ReleaseRefs(Iter);
			ReleaseRefs(td_TypeDef);
		}
		if(FAILED(hr)) throw HRError(hr, "Failed reading Property ID list from test description object");


		/* Read the list of property names */
		{
			std::list<PropertyInfo>::iterator it = PropertyList.begin();

			IAAFTypeDefString *td_String = NULL;
			aafUInt32 Count = 0;

			try
			{
				hr = cd_DescObj->LookupPropertyDef(TestNamesPropID, &pd_PropDef);
				if(!FAILED(hr)) hr = pDescObj->GetPropertyValue(pd_PropDef, &pv_PropValue);
				if(!FAILED(hr)) hr = pv_PropValue->GetType(&td_TypeDef);
				if(!FAILED(hr)) hr = td_TypeDef->QueryInterface(IID_IAAFTypeDefString, reinterpret_cast<void **>(&td_String));
				if(!FAILED(hr)) hr = td_String->GetCount(pv_PropValue, &Count);

				if(!FAILED(hr))
				{
					aafCharacter *Buffer = new aafCharacter[Count + 1];

					try
					{
						hr = td_String->GetElements(pv_PropValue, (aafMemPtr_t)Buffer, (Count + 1) * sizeof(aafCharacter));
						if(FAILED(hr)) throw HRError(hr, "Failed reading property names list from test description object");
						if(!(*Buffer)) throw HRError(E_FAIL, "Empty property names list in test description object");

						aafCharacter *p = Buffer;
						std::string Item;
						aafCharacter Separator = (*p++);
						for(;;)
						{
							// End of a property name
							if((!(*p)) || (*p == Separator))
							{
								if(it == PropertyList.end()) throw HRError(E_FAIL, "More property names than property IDs in test description object");
							
								// Store this name
								(*it).Name = Item;
								
								// End of all names
								if(!(*p)) break;

								// Get ready for next iteration
								Item = "";
								it++;
								p++;

								// Get next name
								continue;
							}

							Item += (char)(*p);
							p++;
						}
					}
					catch(...)
					{
						delete[] Buffer;
						throw;
					}
					delete[] Buffer;
				}
			}
			catch(...)
			{
				ReleaseRefs(td_String);
				throw;
			}
			ReleaseRefs(td_TypeDef);
			ReleaseRefs(td_String);
			ReleaseRefs(pd_PropDef);
			ReleaseRefs(pv_PropValue);
		}
		if(FAILED(hr)) throw HRError(hr, "Failed reading Property names list from test description object");
	}
	catch(...)
	{
		ReleaseRefs(pHeader);
		ReleaseRefs(pDescObj);
		ReleaseRefs(pd_PropDef);
		ReleaseRefs(td_TypeDef);
		ReleaseRefs(pv_PropValue);

		throw;
	}

	ReleaseRefs(pHeader);
	ReleaseRefs(pDescObj);
	ReleaseRefs(pd_PropDef);
	ReleaseRefs(td_TypeDef);
	ReleaseRefs(pv_PropValue);

	return hr;
}


/***************************************/
/* Constants and general useful values */
/***************************************/

//! A NULL AUID for using as a special token
const aafUID_t NULL_AUID = { 0, 0, 0, {0, 0, 0, 0, 0, 0, 0, 0} };

//! Flag to enable extra debug output
bool Verbose = false;



/*******************/
/* General Globals */
/*******************/

//! A list of all new types added
std::list<TypeInfo> TypeList;

//! A list of all new properties added
std::list<PropertyInfo> PropertyList;


/*******************/
/* Our definitions */
/*******************/

//! Class ID for TestDef class
const aafUID_t kClassAUID_TestDef = { 0x848735e1, 0x1d3b, 0x8b23, {0x8e, 0xe7, 0x32, 0x45, 0xfe, 0x34, 0x29, 0x1e}};

//! Class name for TestDef class
const aafString_t kClassName_TestDef = L"Simple definition type for TypeDef tests";


/******************/
/* Test Functions */
/******************/

//! Create the test file and return pointers to it and its header and dictionary
HRESULT CreateTestFile(std::string Filename, IAAFDictionary **Dict, IAAFFile **File, IAAFHeader **Header)
{
	HRESULT hr = S_OK;

	try
	{
		*Dict = NULL;
		*File = NULL;

		// Create a new file
		unlink( Filename.c_str() );
		hr = AAFFileOpenNewModify(StringToAAFString(Filename), 0, &TestIdent, File );
		if(FAILED(hr)) throw HRError(hr, std::string("Failed to open ") + Filename);

		// The dictionary is required to create a new objects
		(*File)->GetHeader( Header );
		(*Header)->GetDictionary( Dict );
	}
	catch(...)
	{
		/* Release everything */
		ReleaseRefs(*File);
		ReleaseRefs(*Header);
		ReleaseRefs(*Dict);

		throw;
	}

	return hr;
}


//! Open the test file and return pointers to it and its header and dictionary
HRESULT OpenTestFile(std::string Filename, IAAFDictionary **Dict, IAAFFile **File, IAAFHeader **Header)
{
	HRESULT hr = S_OK;

	try
	{
		*Dict = NULL;
		*File = NULL;

		// Open existing file
		hr = AAFFileOpenExistingRead(StringToAAFString(Filename), 0, File);
		if(FAILED(hr)) throw HRError(hr, std::string("Failed to open ") + Filename);

		(*File)->GetHeader( Header );
		(*Header)->GetDictionary( Dict );
	}
	catch(...)
	{
		/* Release everything */
		ReleaseRefs(*File);
		ReleaseRefs(*Header);
		ReleaseRefs(*Dict);

		throw;
	}

	return hr;
}


//! Process the property info array to produce a single string containing all the property names
std::string ProcessNameList(void)
{
	std::string Ret;

	std::list<PropertyInfo>::iterator it = PropertyList.begin();
	while(it != PropertyList.end())
	{
		// Perform any substitutions
		for(;;)
		{
			size_t pos = (*it).Name.find_first_of(TestStringSeparator);
			if(pos == std::string::npos) break;
			(*it).Name.substr(pos, 1) = TestStringSeparatorSubstitute;
		}

		Ret += TestStringSeparator;
		Ret += (*it).Name;

		it++;
	}

	return Ret;
}


/**********************************/
/* Class static member properties */
/**********************************/

// Define the string for holding the last error text
std::string HRError::LastError = "Unknown error";

// Define the variable for holding the last error code
HRESULT HRError::LastHR = S_OK;



/**************************/
/* Class member functions */
/**************************/

//! Locate the interface ID for a given class ID
const IID ClassInfo::GetInterfaceID(const aafUID_t &kTypeID)
{
	int i;
	for(i=0; i<ArrayCount; i++)
	{
		if(Array[i].ClassID == kTypeID) return Array[i].InterfaceID;
	}

	throw(0, std::string("No known interface for type ") + AUIDtoString(kTypeID));
}


//! Locate the type ID for an instance of a given class ID
const aafUID_t ClassInfo::GetInstanceID(const aafUID_t &kTypeID)
{
	int i;
	for(i=0; i<ArrayCount; i++)
	{
		if(Array[i].ClassID == kTypeID) return Array[i].InstanceID;
	}

	throw(0, std::string("No known instance of type ") + AUIDtoString(kTypeID));
}


//! Locate the type name for a given class ID
std::string ClassInfo::GetName(const aafUID_t &kTypeID)
{
	int i;
	for(i=0; i<ArrayCount; i++)
	{
		if(Array[i].ClassID == kTypeID) return std::string(Array[i].Name);
	}

	return AUIDtoString(kTypeID);
}


//! Construct a ClassTest
ClassTest::ClassTest(aafUID_t ThisClass, aafUID_t Class0, aafUID_t Class1, aafUID_t Class2, 
										 aafUID_t Class3, aafUID_t Class4, aafUID_t Class5, 
										 aafUID_t Class6, aafUID_t Class7 , aafUID_t Class8, 
										 aafUID_t Class9, aafUID_t Class10 , aafUID_t Class11, 
										 aafUID_t Class12, aafUID_t Class13 , aafUID_t Class14,
										 aafUID_t Class15, aafUID_t Class16, aafUID_t Class17 )
{
	aafUID_t *Params[18] = { &Class0, &Class1, &Class2, &Class3, &Class4, &Class5, &Class6, &Class7, &Class8, 
							 &Class9, &Class10, &Class11, &Class12, &Class13, &Class14, &Class15, &Class16, &Class17 };
	
	Class = ThisClass;

	int i;
	for(i=0; i<18; i++)
	{
		if(*Params[i] == NULL_AUID) break;
		Tests.push_back(*Params[i]);
	}
};


/****************************/
/* General useful functions */
/****************************/

//! Make a string representation of an AUID
std::string AUIDtoString(const aafUID_t &AUID)
{
	char Buffer[128];

	sprintf(Buffer, "{ 0x%08x, 0x%04x, 0x%04x, {0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x} }",
		    AUID.Data1, AUID.Data2, AUID.Data3, AUID.Data4[0], AUID.Data4[1], AUID.Data4[2],
			AUID.Data4[3], AUID.Data4[4], AUID.Data4[5], AUID.Data4[6], AUID.Data4[7]);

	return std::string(Buffer);
}


//! Make a string representation of an IID
std::string IIDtoString(const IID &ID)
{
	char Buffer[128];

	sprintf(Buffer, "{ 0x%08x, 0x%04x, 0x%04x, {0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x, 0x%02x} }",
		    ID.Data1, ID.Data2, ID.Data3, ID.Data4[0], ID.Data4[1], ID.Data4[2],
			ID.Data4[3], ID.Data4[4], ID.Data4[5], ID.Data4[6], ID.Data4[7]);

	return std::string(Buffer);
}



//! AUID comparison - Why is this not supplied by the library?
bool operator==(const aafUID_t &LHS, const aafUID_t RHS) 
{
	if(LHS.Data1 != RHS.Data1) return false;
	if(LHS.Data2 != RHS.Data2) return false;
	if(LHS.Data3 != RHS.Data3) return false;
	return (memcmp(LHS.Data4, RHS.Data4, 8) == 0);
}

// Translate an error code to a string
std::string HRESULTtoString(const HRESULT hr)
{
	int i;
	for(i=0; i<ResultCodesCount; i++)
	{
		if(hr == ResultCodes[i].hr)
		{
			return std::string(ResultCodes[i].Text);
		}
	}

	return std::string("Unknown Error Code");
}


//! Build an aafString_t from a std::string
aafString_t StringToAAFString(std::string Str)
{
	static aafCharacter Buffer[1024];

	if(Str.length() > 1023) throw "String too long for StringToAAFString()";

	aafCharacter *p = Buffer;
	std::string::iterator it = Str.begin();
	while(it != Str.end())
	{
		*(p++) = (*it);
		it++;
	}

	*p = 0;

	return reinterpret_cast<aafString_t>(Buffer);
}


//! Display a debug message - if verbose mode set
void DebugMessage(const char *Format, ...)
{
	if(!VerboseMode()) return;

	va_list Args;
	va_start(Args, Format);

	vprintf(Format, Args);

	va_end(Args);
}


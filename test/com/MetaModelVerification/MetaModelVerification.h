//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 2006-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Matt Beard / Metaglue Corporation.
// All rights reserved.
//
//=---------------------------------------------------------------------=


/***************************/
/* Declare general globals */
/***************************/

//! Flag to enable extra debug output
extern bool Verbose;

//! A NULL AUID for using as a special token
extern const aafUID_t NULL_AUID;


/******************/
/* Define classes */
/******************/

//! Simple class for throwing an HRESULT with an error string
/*! The error text is stored so that is an error is caught and handled a new throw can recover the last error text
 */
class HRError
{
public:
	HRESULT hr;										//!< The error result
	std::string Text;								//!< Text relating to the error

	static std::string LastError;					//!< The last error generated
	static HRESULT LastHR;							//!< The last error code generated

	//! Build a full error to throw
	HRError(HRESULT hr, std::string Text) : hr(hr), Text(Text) { LastError = Text; LastHR = hr; }

	//! Build an error from just the error number
	/*! Recalls the most recent error text
	 *  If a function catches an exception, handles it, then returns an HRESULT this may cause the
	 *  calling function to fail in a way that requires it to throw an exception - in which case
	 *  it can throw an HRError built from an HRESULT and picking up the text from the previous error.
	 */
	HRError(HRESULT hr) : hr(hr) { Text = LastError; LastHR = hr; }

	//! Build an error from just the error text
	/*! Recalls the most recent error number
	 *  If a function catches an HRError thrown from a lower function it may override the message
	 */
	HRError(std::string Text) : Text(Text) { LastError = Text; hr = LastHR; }

	//! Clear any thrown error text
	/*! Use if a caught HRError has been fully handled and is no longer a problem.
	 *  Don't use if the error will be propogated up with a non S_OK returned HRESULT.
	 */
	void Clear(void) 
	{ 
		LastError = "Unknown error - previous exception handled fully"; 
		LastHR = S_OK;
	}
};


//! Simple helper class to initialize and cleanup AAF library.
struct CAAFInitialize
{
	//! Initialize the library
	CAAFInitialize(const char *dllname = NULL)
	{
		HRESULT hr = AAFLoad(dllname);
		if (!AAFRESULT_SUCCEEDED(hr)) 
		{
			throw HRError(hr, "Failed to load the AAF library. Check PATH under Windows, or LD_LIBRARY_PATH under Unix/Linux");
		}
	}

	//! Clean up
	~CAAFInitialize()
	{
		AAFUnload();
	}
};


//! Structure for entries in ClassInfo
class ClassInfo
{
protected:
	const char *Name;				//!< The name of this ID
	aafUID_t ClassID;				//!< The AUID for this type def class
	IID InterfaceID;				//!< The interface ID for the primary interface of this type def class
	bool CanInstantiate;			//!< Can we make our own instance of this type?
	aafUID_t InstanceID;			//!< The AUID for an instance of this type def, or NULL if none known

protected:
	//! Info for all type def classes
	static const ClassInfo Array[];

	//! Number of entries in Array
	static const int ArrayCount;

public:
	ClassInfo(const char *Name, const aafUID_t &ClassID, const IID &InterfaceID, bool CanInstantiate, const aafUID_t InstanceID)
		: Name(Name), ClassID(ClassID), InterfaceID(InterfaceID), CanInstantiate(CanInstantiate), InstanceID(InstanceID)
	{ }

	//! Locate the interface ID for a given class ID
	static const IID GetInterfaceID(const aafUID_t &kTypeID);
	
	//! Locate the type ID for an instance of a given class ID
	static const aafUID_t GetInstanceID(const aafUID_t &kTypeID);

	//! Locate the type name for a given class ID
	static std::string GetName(const aafUID_t &kTypeID);
};

//! Helper function that removes the need to class specify GetInterfaceID()
inline const IID GetInterfaceID(const aafUID_t &kTypeID) { return ClassInfo::GetInterfaceID(kTypeID); }

//! Helper function that removes the need to class specify GetInstanceID()
inline const aafUID_t GetInstanceID(const aafUID_t &kTypeID) { return ClassInfo::GetInstanceID(kTypeID); }

//! Helper function that removes the need to class specify GetName()
inline std::string GetName(const aafUID_t &kTypeID) { return ClassInfo::GetName(kTypeID); }


//! Class to contain the tests required for a type def
class ClassTest
{
public:
	aafUID_t Class;					//!< The class to test
	std::list<aafUID_t> Tests;		//!< A list of other classes that must be tested for inclusion in this class

public:
	ClassTest(aafUID_t ThisClass, aafUID_t Class0 = NULL_AUID, aafUID_t Class1 = NULL_AUID, aafUID_t Class2 = NULL_AUID, 
								  aafUID_t Class3 = NULL_AUID, aafUID_t Class4 = NULL_AUID, aafUID_t Class5 = NULL_AUID, 
								  aafUID_t Class6 = NULL_AUID, aafUID_t Class7 = NULL_AUID, aafUID_t Class8 = NULL_AUID, 
								  aafUID_t Class9 = NULL_AUID, aafUID_t Class10 = NULL_AUID, aafUID_t Class11 = NULL_AUID, 
								  aafUID_t Class12 = NULL_AUID, aafUID_t Class13 = NULL_AUID, aafUID_t Class14 = NULL_AUID,
								  aafUID_t Class15 = NULL_AUID, aafUID_t Class16 = NULL_AUID, aafUID_t Class17 = NULL_AUID );
};


//! Structure to hold info about new types added
struct TypeInfo
{
	std::string Name;
	aafUID_t ID;
};


//! Structure to hold info about new properties added
struct PropertyInfo
{
	std::string Name;
	aafUID_t ID;
};


/**********************************************************/
/* Declare functions defined in MetaModelVerification.cpp */
/**********************************************************/

//! Create the test file and return pointers to it and its header and dictionary
HRESULT CreateTestFile(std::string Filename, IAAFDictionary **Dict, IAAFFile **File, IAAFHeader **Header);

//! Open the test file and return pointers to it and its header and dictionary
HRESULT OpenTestFile(std::string Filename, IAAFDictionary **Dict, IAAFFile **File, IAAFHeader **Header);

//! Register a new TypeDef of the specified type - but don't initialize. Used by other RegisterType() funcs.
HRESULT RegisterTypeInternal(IAAFDictionary *Dict, const aafUID_t &kTypeToDefine, IUnknown **NewInterface);

//! Register a new TypeDef of the specified type, with a given sub-type
HRESULT RegisterType(IAAFDictionary *Dict, const aafUID_t &kNewTypeID, const aafUID_t &kTypeToDefine, const aafUID_t &kSubType);

//! Register a new TypeDef of the specified type
HRESULT RegisterType(IAAFDictionary *Dict, const aafUID_t &kNewTypeID, const aafUID_t &kTypeToDefine, IAAFTypeDef **tdNewType);

//! Register a property of the given type in a given class
HRESULT RegisterProperty(IAAFDictionary *Dict, const aafUID_t &kClassID, const aafUID_t &kTypeID, const aafUID_t &kPropertyID, const aafString_t &kPropertyName);

//! Verify the value of a given property
HRESULT VerifyProperty(IAAFPropertyDef *pd_Property, IAAFPropertyValue *pv_Value);

//! Set a  value for a given property of an object
HRESULT AddProperty(IAAFDictionary *Dict, IAAFObject *Object, const aafUID_t kPropertyID);

//! Try adding all valid combinations of type defs to a dictionary
HRESULT TestValidClasses(IAAFDictionary *Dict, const ClassTest *TestList, int TestCount);

//! Try adding all invalid combinations of type defs to a dictionary
HRESULT TestInvalidClasses(IAAFDictionary *Dict, const ClassTest *TestList, int TestCount);

//! Try using all new type defs by adding properties of each type to a given object
HRESULT TestTypeDefProperties(IAAFDictionary *Dict, IAAFObject *Object);

//! Try reading all new type def properties from the given object, and check thier values
HRESULT VerifyTypeDefProperties(IAAFDictionary *Dict, IAAFObject *Object);

//! Register items required for our internal use
HRESULT RegisterRequiredItems(IAAFDictionary *Dict);

//! Add the test description object to the file
HRESULT AddDescription(IAAFHeader *Header);

//! Read the test description object from the file
HRESULT ReadDescription(IAAFHeader *Header);

//! Process the property info array to produce a single string containing all the property names
std::string ProcessNameList(void);

//! Make a string representation of an AUID
std::string AUIDtoString(const aafUID_t &AUID);

//! Make a string representation of an IID
std::string IIDtoString(const IID &ID);

//! Build a new random ID based on a template ID
aafUID_t RandomID(const aafUID_t &Template = NULL_AUID);

//! AUID comparison - Why is this not supplied by the library?
bool operator==(const aafUID_t &LHS, const aafUID_t RHS);

//! AUID comparison - Why is this not supplied by the library?
inline bool operator!=(const aafUID_t &LHS, const aafUID_t RHS) 
{
	return !operator==(LHS,RHS);
}

// Translate an error code to a string
std::string HRESULTtoString(const HRESULT hr);

//! Build an aafString_t from a std::string
aafString_t StringToAAFString(std::string Str);

//! Display a debug message - if verbose mode set
void DebugMessage(const char *Format, ...);


/***************************/
/* Declare our definitions */
/***************************/

//! Class ID for TestDef class
extern const aafUID_t kClassAUID_TestDef;

//! Class name for TestDef class
extern const aafWChar * const kClassName_TestDef;


/*******************************/
/* Declare application-globals */
/*******************************/

//! Product identification structure
extern aafProductIdentification_t TestIdent;

//! A list of all new types added
extern std::list<TypeInfo> TypeList;

//! A list of all new properties added
extern std::list<PropertyInfo> PropertyList;


/***************************/
/* Define inline functions */
/***************************/

//! Release any COM interface, providing it is not NULL
#define ReleaseRefs(p)  if(p) { (p)->Release(); p = NULL; } 

/*inline void Release(IUnknown **p) 
{ 
	if(*p) 
	{
		(*p)->Release();
		*p = NULL;
	}
}*/

//! Read the debug flag
inline bool VerboseMode(void) { return Verbose; }

//! Set the debug flag
inline void SetVerbose(bool Value) { Verbose = Value; }






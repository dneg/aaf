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

#if defined(macintosh)
// Make sure we have defined IID_IUnknown and IID_IClassFactory.
#include <initguid.h>
#include <coguid.h>	
#endif



#include "AAF.h"
#include "AAFTypes.h"
#include "AAFResult.h"
#include "AAFStoredObjectIDs.h"
#include "AAFTypeDefUIDs.h"
#include "AAFPropertyDefs.h"
#include <assert.h>
#include <iostream.h>
#include <iomanip.h>
#include <fstream.h>
#include <stdio.h> // for sprintf()
#include <stdlib.h>
#include <string.h>

#ifndef __AAFSmartPointer_h__
#include "AAFSmartPointer.h"
#endif

#if defined( OS_MACOS )
#include "DataInput.h"
#endif

#if defined( COMPILER_MWERKS )
#if defined(__MSL_CPP__) && (__MSL_CPP__ >= 0x5300)
#define IOS_FMT_FLAGS ios_base::fmtflags
#else
#define IOS_FMT_FLAGS long int
#endif
#elif defined(__GNUC__) && (__GNUC__ >= 3)
#define IOS_FMT_FLAGS std::ios_base::fmtflags
#else
#define IOS_FMT_FLAGS long int
#endif

// structure for dump flags
typedef struct _dumpFlags 
{
  bool showDict;
  bool showMetaDict;
  bool showAllHeaderProps;
  bool showTypes;
  bool showSummary;
  bool showDump;
  bool showSMPTE;
  bool showEssence;
  bool identifybyname;
  unsigned long maxCount;
  char *showOnlyClasses;
} dumpFlags_t;

// handy smart pointer typedefs
typedef IAAFSmartPointer<IAAFObject>               IAAFObjectSP;
typedef IAAFSmartPointer<IAAFPropertyValue>        IAAFPropertyValueSP;
typedef IAAFSmartPointer<IAAFProperty>             IAAFPropertySP;
typedef IAAFSmartPointer<IAAFPropertyDef>          IAAFPropertyDefSP;
typedef IAAFSmartPointer<IAAFTypeDef>              IAAFTypeDefSP;
typedef IAAFSmartPointer<IAAFMetaDefinition>       IAAFMetaDefinitionSP;
typedef IAAFSmartPointer<IAAFDictionary>           IAAFDictionarySP;
typedef IAAFSmartPointer<IAAFTypeDefCharacter>     IAAFTypeDefCharacterSP;
typedef IAAFSmartPointer<IAAFTypeDefIndirect>      IAAFTypeDefIndirectSP;
typedef IAAFSmartPointer<IAAFTypeDefOpaque>        IAAFTypeDefOpaqueSP;
typedef IAAFSmartPointer<IAAFTypeDefInt>           IAAFTypeDefIntSP;
typedef IAAFSmartPointer<IAAFTypeDefObjectRef>     IAAFTypeDefObjectRefSP;
typedef IAAFSmartPointer<IAAFClassDef>             IAAFClassDefSP;
typedef IAAFSmartPointer<IAAFTypeDefRename>        IAAFTypeDefRenameSP;
typedef IAAFSmartPointer<IAAFTypeDefEnum>          IAAFTypeDefEnumSP;
typedef IAAFSmartPointer<IAAFTypeDefExtEnum>       IAAFTypeDefExtEnumSP;
typedef IAAFSmartPointer<IAAFTypeDefFixedArray>    IAAFTypeDefFixedArraySP;
typedef IAAFSmartPointer<IAAFTypeDefVariableArray> IAAFTypeDefVariableArraySP;
typedef IAAFSmartPointer<IAAFTypeDefString>        IAAFTypeDefStringSP;
typedef IAAFSmartPointer<IAAFTypeDefRecord>        IAAFTypeDefRecordSP;
typedef IAAFSmartPointer<IAAFTypeDefStream>        IAAFTypeDefStreamSP;
typedef IAAFSmartPointer<IAAFTypeDefSet>           IAAFTypeDefSetSP;
typedef IAAFSmartPointer<IAAFTypeDefStrongObjRef>  IAAFTypeDefStrongObjRefSP;
typedef IAAFSmartPointer<IAAFTypeDefWeakObjRef> IAAFTypeDefWeakObjRefSP;
typedef IAAFSmartPointer<IAAFFile>                 IAAFFileSP;
typedef IAAFSmartPointer<IAAFHeader>               IAAFHeaderSP;
typedef IAAFSmartPointer<IEnumAAFProperties>       IEnumAAFPropertiesSP;
typedef IAAFSmartPointer<IEnumAAFPropertyDefs> IEnumAAFPropertyDefsSP;
typedef IAAFSmartPointer<IAAFTypeDefCharacter>       IAAFTypeDefCharacterSP;
typedef IAAFSmartPointer<IAAFComponent> IAAFComponentSP;
typedef IAAFSmartPointer<IAAFDataDef> IAAFDataDefSP;
typedef IAAFSmartPointer<IAAFFileDescriptor> IAAFFileDescriptorSP;
typedef IAAFSmartPointer<IAAFDefObject> IAAFDefObjectSP;
typedef IAAFSmartPointer<IAAFContentStorage> IAAFContentStorageSP;
typedef IAAFSmartPointer<IEnumAAFMobs> IEnumAAFMobsSP;
typedef IAAFSmartPointer<IAAFMob> IAAFMobSP;
typedef IAAFSmartPointer<IEnumAAFTypeDefs> IEnumAAFTypeDefsSP;
typedef IAAFSmartPointer<IEnumAAFCodecDefs> IEnumAAFCodecDefsSP;
typedef IAAFSmartPointer<IAAFCodecDef> IAAFCodecDefSP;
typedef IAAFSmartPointer<IEnumAAFDataDefs> IEnumAAFDataDefsSP;
typedef IAAFSmartPointer<IAAFDataDef> IAAFDataDefSP;
typedef IAAFSmartPointer<IEnumAAFPropertyValues>   IEnumAAFPropertyValuesSP;


// convenient error handlers.
inline void checkResult(HRESULT r)
{
	if (FAILED(r))
		throw r;
}
inline void checkExpression(bool expression, HRESULT r)
{
	if (!expression)
		throw r;
}


static void printIndent (int indent, ostream & os)
{
	while (indent--) os << " ";
}



static void convert(aafCharacter* wcName, size_t length, const char* name)
{
	assert(name );
	// assert(*name); this can handle 0-length
	assert(wcName != 0);
	assert(length > 0);
	
	// if there is an error handling only terminating, for !*name and set *wcName=0;
	size_t status = mbstowcs(wcName, name, length);
	if (status == (size_t)-1) {
		cerr << "Error : Failed to convert'"
			<< name
			<< "' to a wide character string.\n\n";
		exit(1);  
	}
}


static void convert(char* cName, size_t length, const char* name)
{
	assert(name );
	// assert(*name); can this handle 0-length
	// if there is an error handling only terminating, for !*name and set *cName=0;
	assert(cName != 0);
	assert(length > 0);
	
	size_t sourceLength = strlen(name);
	if (sourceLength < length - 1) {
		strncpy(cName, name, length);
	} else {
		cerr << "Error : Failed to copy '" << name << "'.\n\n";
		exit(1);  
	}
}

static void convert(aafCharacter* wName, size_t length, const aafCharacter* name)
{
	assert(name );
	// assert(*name); can this handle 0-length
	// if there is an error handling only terminating, for !*name and set *wName=0;
	assert(wName != 0);
	assert(length > 0);
	
	size_t sourceLength = 0;
	while (*name)
		++sourceLength;
	if (sourceLength < length - 1) {
		// Copy the string if there is enough room in the destinition buffer.
		while (0 != (*wName++ = *name++))
			;
	} else {
		cerr << "Error : Failed to copy '" << name << "'.\n\n";
		exit(1);  
	}
}

static void convert(char* cName, size_t length, const aafCharacter* name)
{
	assert(name );
	// assert(*name); can this handle 0-length
	// if there is an error handling only terminating, for !*name and set *cName=0;
	assert(cName != 0);
	assert(length > 0);
	
	size_t status = wcstombs(cName, name, length);
	if (status == (size_t)-1) {
		cerr << ": Error : Conversion failed.\n\n";
		exit(1); 
	}
}

static HRESULT printAAFString( 
			      aafCharacter* aafCharString, 
			      ostream &os
			      )
{
  HRESULT returnHr=AAFRESULT_SUCCESS;
  try
    {
      int count=0;
      int numChars=0;
      char *convertedChar= new char[MB_CUR_MAX+1];
      assert(aafCharString);
      assert(convertedChar);

      for (count=0; aafCharString[count] !=0 ; count++ )
	{
	  if ((numChars=wctomb(convertedChar,aafCharString[count])) !=-1)
	    {
	      // legal character in locale
	      convertedChar[numChars]=0;  // zero terminator
	      os << convertedChar;
	    }
	  else
	    {
	      // could not convert to multibyte
	      os << "\\u" << hex << aafCharString[count];
	    }
	}
      delete[]convertedChar;
    }
  catch (HRESULT &caught)
    {
      os << endl << "***Caught hresult 0x" << hex << caught << "***" << endl;
      returnHr = caught;
    }
	
  return returnHr;
}



static void printTimeStamp (const aafTimeStamp_t & ts,
							ostream & os)
{
	static const char * const monthNames[] =
	{
		"Jan", "Feb", "Mar",
			"Apr", "May", "Jun",
			"Jul", "Aug", "Sep",
			"Oct", "Nov", "Dec"
	};
	
	char monthNameBuf [10];
	if ((ts.date.month < 1) ||
		(ts.date.month > 12))
	{
		sprintf (monthNameBuf, "Month %d", ts.date.month);
	}
	else
	{
                assert (ts.date.month > 0 );
		aafUInt8 month_index = ts.date.month-1;
		assert (month_index < (sizeof (monthNames) / sizeof (monthNames[0])));
		strcpy (monthNameBuf, monthNames[month_index]);
	}
	
	os << dec
		<< monthNameBuf << " "
		<< (int) ts.date.day << ", "
		<< (int) ts.date.year <<	" ";
	char buf [12];
	sprintf (buf, "%d:%02d:%02d.%02d",
		ts.time.hour,
		ts.time.minute,
		ts.time.second,
		ts.time.fraction);
	os << buf;
	os << " GMT";
}

static void printAUID (const aafUID_t & auidVal,
		       ostream & os)
{
  char buf[100];
  int i=0;
  sprintf (buf, "{ 0x%08x, 0x%04x, 0x%04x, ",
	   auidVal.Data1, auidVal.Data2, auidVal.Data3);
					
  os	<< buf << "{ ";
  for (i = 0; i < 8; i++)
    {
      sprintf (buf, "0x%x", auidVal.Data4[i]);
      os << buf;
      if (i != 7)
	os << ", ";
    }
  os << " } }" ;
}

static void printMobID (const aafMobID_t &mobIDVal,
			ostream &os)
{
  char buf[100];
  int i=0;
    os << "{ {";
  for (i = 0; i < 12; i++)
    {
      sprintf (buf, "0x%x",mobIDVal.SMPTELabel[i]);
      os << buf;
      if (i !=11)
	os << ",";
    }
  os << "}";
  sprintf (buf, "0x%x", mobIDVal.length);
  os << buf << ", ";
  sprintf (buf, "0x%x", mobIDVal.instanceHigh);
  os << buf << ", ";
  sprintf (buf, "0x%x", mobIDVal.instanceMid);
  os << buf << ", ";
  sprintf (buf, "0x%x", mobIDVal.instanceLow);
  os << buf << ", ";
  printAUID(mobIDVal.material,os);
  os << " }" ;
}

static HRESULT dumpSummary
(
 IAAFHeaderSP pHeader, // object to be dumped
 IAAFDictionary * pDict,  // dictionary for this file
 dumpFlags_t dumpFlags,
 int indent,
 ostream & os
 );

static HRESULT dumpMetaDictionary
(
 IAAFHeaderSP pHeader, // object to be dumped
 IAAFDictionary * pDict,  // dictionary for this file
 dumpFlags_t dumpFlags,
 int indent,
 ostream & os
 );

static HRESULT dumpHeaderObject
(
 IAAFHeaderSP pHeader, // object to be dumped
 IAAFDictionary * pDict,  // dictionary for this file
 dumpFlags_t dumpFlags,
 int indent,
 ostream & os
 );
static HRESULT dumpMobsAndCS
(
 IAAFHeaderSP pHeader, // object to be dumped
 IAAFDictionary * pDict,  // dictionary for this file
 dumpFlags_t dumpFlags,
 int indent,
 ostream & os
 );
static HRESULT dumpEssence
(
 IAAFHeaderSP pHeader, // object to be dumped
 IAAFDictionary * pDict,  // dictionary for this file
 dumpFlags_t dumpFlags,
 int indent,
 ostream & os
 );

static HRESULT dumpMetaDefCommon
(
 IAAFMetaDefinitionSP pMetaDef, // object to be dumped
 IAAFDictionary * pDict,  // dictionary for this file
 dumpFlags_t dumpFlags,
 int indent,
 ostream & os
);

static HRESULT dumpPropertyDef
(
 IAAFPropertyDefSP pContainer, // object to be dumped
 IAAFDictionary * pDict,  // dictionary for this file
 dumpFlags_t dumpFlags,
 int indent,
 ostream & os
 );
static HRESULT dumpClassDef
(
 IAAFClassDef* pClassDef, // object to be dumped
 IAAFDictionary * pDict,  // dictionary for this file
 dumpFlags_t dumpFlags,
 int indent,
 ostream & os
 );
static HRESULT dumpTypeDef
(
 IAAFTypeDefSP pTypeDef, // object to be dumped
 IAAFDictionary * pDict,  // dictionary for this file
 dumpFlags_t dumpFlags,
 int indent,
 ostream & os
 );

static HRESULT dumpObject
(
 IAAFObjectSP pContainer, // object to be dumped
 IAAFDictionary * pDict,  // dictionary for this file
 dumpFlags_t dumpFlags,
 int indent,
 ostream & os
 );

static HRESULT dumpPropertyValue
(
 IAAFPropertyValueSP pPVal,
 IAAFDictionary * pDict,  // dictionary for this file
 dumpFlags_t dumpFlags,
 bool showProps,
 int indent,
 ostream & os
 );

static HRESULT dumpRawStreamPropertyValue
(
 IAAFPropertyValueSP pPVal,
 IAAFTypeDef *pTD,
 IAAFDictionary * pDict,  // dictionary for this file
 dumpFlags_t dumpFlags,
 int indent,
 ostream & os
 );

static HRESULT dumpPropertyDef(
			       IAAFPropertyDefSP pPropertyDef, // object to be dumped
			       IAAFDictionary * pDict,  // dictionary for this file
			       dumpFlags_t dumpFlags,
			       int indent,
			       ostream & os
			       )
{
  HRESULT returnHr = AAFRESULT_SUCCESS;
  
  try
    {
      aafCharacter* aafStringBuf=NULL;
      aafUInt32 bufSize=0;
      IAAFMetaDefinitionSP pMetaDef;
      printIndent (indent, os);
      os << "Property Definition for: ";

      // Get MetaDef pointer
      checkResult(pPropertyDef->QueryInterface(IID_IAAFMetaDefinition,
					    (void**) &pMetaDef));
      checkResult(dumpMetaDefCommon(pMetaDef, pDict,
				    dumpFlags, indent, os));

      // print optional/required
      aafBoolean_t isOptional;
      checkResult(pPropertyDef->GetIsOptional(&isOptional));
      if (isOptional)
	os << "Property is optional" << endl;
      else
	os << "Property is required" << endl;
      printIndent(indent, os);

      // print if unique identifier
      aafBoolean_t isUnique;
      checkResult(pPropertyDef->GetIsUniqueIdentifier(&isUnique));
      if (isUnique)
	{
	  os << "Property is unique identifier" << endl;
	  printIndent(indent,os);
	}
      IAAFTypeDef *pTypeDef=NULL;
      IAAFMetaDefinition *pTypeDefMD=NULL;
      checkResult(pPropertyDef->GetTypeDef(&pTypeDef));
      checkResult(pTypeDef->QueryInterface(IID_IAAFMetaDefinition,
					   (void**) &pTypeDefMD));
	  
      os << "Type is: ";
      if (dumpFlags.identifybyname)
	{
	  checkResult(pTypeDefMD->GetNameBufLen(&bufSize));
	  aafStringBuf = (aafCharacter*) new aafUInt8[bufSize];
	  checkResult(pTypeDefMD->GetName(aafStringBuf, bufSize));
	  checkResult(printAAFString(aafStringBuf,  os));
	  if (aafStringBuf)
	    delete[]aafStringBuf;
	  aafStringBuf=NULL;
	}
      else
	{
	  aafUID_t typeID;
	  checkResult(pTypeDefMD->GetAUID(&typeID));
	  printAUID(typeID,os);
	}
      os << endl;
      printIndent(indent,os);
      if (pTypeDefMD)
	pTypeDefMD->Release();
      pTypeDefMD=NULL;
      if (pTypeDef)
	pTypeDef->Release();
      pTypeDef=NULL;

    }
  catch (HRESULT &caught)
    {
      os << endl << "***Caught hresult 0x" << hex << caught << "***" << endl;
      returnHr = caught;
    }
	
  return returnHr;
}

				 
static HRESULT dumpMetaDefCommon
(
 IAAFMetaDefinitionSP pMetaDef, // object to be dumped
 IAAFDictionary * /*pDict*/,  // dictionary for this file
 dumpFlags_t dumpFlags,
 int indent,
 ostream & os
 )
{
  // Prints MetaDef name on current line
  // then prints AUID and Description
  HRESULT returnHr = AAFRESULT_SUCCESS;
	
  try
    {
      aafCharacter* aafStringBuf=NULL;
      aafUInt32 bufSize=0;
      checkResult(pMetaDef->GetNameBufLen(&bufSize));
      aafStringBuf = (aafCharacter*) new aafUInt8[bufSize];
      checkResult(pMetaDef->GetName(aafStringBuf, bufSize));
      // print the name
      checkResult(printAAFString(aafStringBuf, os));
      if (aafStringBuf)
	delete[]aafStringBuf;
      aafStringBuf=NULL;
      os << endl;

      // Print AUID
      aafUID_t classID;
      checkResult(pMetaDef->GetAUID(&classID));
      printIndent (indent, os);
      os << "Identification: ";
      if (dumpFlags.showTypes)
	os << "(AUID) ";
      printAUID(classID, os);
      os << endl;
      printIndent (indent, os);

      // print description
      checkResult(pMetaDef->GetDescriptionBufLen(&bufSize));
      if (bufSize)
	{
	  aafStringBuf = (aafCharacter*) new aafUInt8[bufSize];
	  checkResult(pMetaDef->GetDescription(aafStringBuf, bufSize));
	  // print the name
	  printIndent (indent, os);
	  os << "Description: ";
	  if (dumpFlags.showTypes)
	    os << "(aafString_t) ";
	  os << "\"";
	  checkResult(printAAFString(aafStringBuf,  os));
	  os << "\"" << endl;
	  if (aafStringBuf)
	    delete[]aafStringBuf;
	  aafStringBuf=NULL;
	}
    }
  catch (HRESULT &caught)
    {
      os << endl << "***Caught hresult 0x" << hex << caught << "***" << endl;
      returnHr = caught;
    }
	
  return returnHr;
}

static HRESULT dumpClassDef(
			    IAAFClassDef* pClassDef, // object to be dumped
			    IAAFDictionary * pDict,  // dictionary for this file
			    dumpFlags_t dumpFlags,
			    int indent,
			    ostream & os
			    )
{
  HRESULT returnHr = AAFRESULT_SUCCESS;
	
  try
    {
      aafUInt32 countPropDefs=0;
      IAAFMetaDefinitionSP pMetaDef;
      aafCharacter* aafStringBuf=NULL;
      aafUInt32 bufSize=0;
      os << "Class Definition for: ";
      // Get MetaDef pointer
      checkResult(pClassDef->QueryInterface(IID_IAAFMetaDefinition,
					    (void**) &pMetaDef));
      checkResult(dumpMetaDefCommon(pMetaDef, pDict,
				    dumpFlags, indent, os));
      // print root or parent class
      aafBoolean_t isRoot;
      checkResult(pClassDef->IsRoot(&isRoot));
      if (isRoot)
	{
	  os << "Is a root class" << endl;
	} 
      else
	{
	  IAAFClassDef* pParentClassDef=NULL;
	  checkResult(pClassDef->GetParent(&pParentClassDef));
	  os << "Parent class is: ";
	  if (dumpFlags.identifybyname)
	    {
	      checkResult(pParentClassDef->GetNameBufLen(&bufSize));
	      aafStringBuf = (aafCharacter*) new aafUInt8[bufSize];
	      checkResult(pParentClassDef->GetName(aafStringBuf, bufSize));
	      checkResult(printAAFString(aafStringBuf,  os));
	      if (aafStringBuf)
		delete[]aafStringBuf;
	    }
	  else
	    {
	      IAAFMetaDefinition* pParentClassDefMD=NULL;
	      checkResult(pParentClassDef->QueryInterface(IID_IAAFMetaDefinition,
							  (void**) &pParentClassDefMD));
	      aafUID_t parentClassID;
	      checkResult(pParentClassDefMD->GetAUID(&parentClassID));
	      printAUID(parentClassID, os);
	      if (pParentClassDefMD)
		pParentClassDefMD->Release();
	      pParentClassDefMD=NULL;
	    }
	  os << endl;
	  if (pParentClassDef)
	    pParentClassDef->Release();
	  pParentClassDef=NULL;
	}
      printIndent (indent, os);
		    
      // print abstract/concrete
      aafBoolean_t isConcrete;
      checkResult(pClassDef->IsConcrete(&isConcrete));
      if (isConcrete)
	os << "Class is concrete" << endl;
      else
	os << "Class is abstract" << endl;
      printIndent (indent, os);

      // print uniquely identified information
      aafBoolean_t isUniquelyIDed;
      checkResult(pClassDef->IsUniquelyIdentified(&isUniquelyIDed));
      if (isUniquelyIDed)
	{
	  IAAFPropertyDefSP pUIDPropDef;
	  checkResult(pClassDef->GetUniqueIdentifier(&pUIDPropDef));
	  IAAFMetaDefinition *pUIDPropDefMD=NULL;
	  checkResult(pUIDPropDef->QueryInterface(IID_IAAFMetaDefinition,
						  (void**) &pUIDPropDefMD));
	  os << "Uniquely identified by property: " ;
	  printIndent(indent, os);
	  if (dumpFlags.identifybyname)
	    {
	      checkResult(pUIDPropDefMD->GetNameBufLen(&bufSize));
	      aafStringBuf = (aafCharacter*) new aafUInt8[bufSize];
	      checkResult(pUIDPropDefMD->GetName(aafStringBuf, bufSize));
	      checkResult(printAAFString(aafStringBuf,  os));
	      if (aafStringBuf)
		delete[]aafStringBuf;
	      aafStringBuf=NULL;
	    }
	  else
	    {
	      aafUID_t uniqueProp;
	      checkResult(pUIDPropDefMD->GetAUID(&uniqueProp));
	      printAUID(uniqueProp, os);
	    }
	  os << endl;
	  printIndent (indent, os);
	  if (pUIDPropDefMD)
	    pUIDPropDefMD->Release();
	  pUIDPropDefMD=NULL;
	}

      // Dump the number of properties and each property def
      checkResult(pClassDef->CountPropertyDefs(&countPropDefs));
      os << "Class defines " << countPropDefs << " property definitions" << endl;
		    
      if (countPropDefs)
	{
	  IEnumAAFPropertyDefsSP pPropDefEnum;
	  checkResult(pClassDef->GetPropertyDefs(&pPropDefEnum));
	  IAAFPropertyDefSP pPropDef;
	  while (AAFRESULT_SUCCEEDED(pPropDefEnum->NextOne(&pPropDef)))
	    {
	      os << endl;
	      checkResult(dumpPropertyDef(pPropDef, pDict, dumpFlags, indent+1, os));
	    }
	}
		
    }
  catch (HRESULT &caught)
    {
      os << endl << "***Caught hresult 0x" << hex << caught << "***" << endl;
      returnHr = caught;
    }
	
  return returnHr;
}



static HRESULT printAAFName(
		IAAFMetaDefinition  *pMetaDef,
		ostream		    &os )
{
    HRESULT	    status = AAFRESULT_SUCCESS;
    aafCharacter*   aafStringBuf = NULL;
    aafUInt32	    bufSize = 0;

    checkResult( pMetaDef->GetNameBufLen( &bufSize ) );
    aafStringBuf = (aafCharacter*) new aafUInt8[ bufSize ];
    checkResult( pMetaDef->GetName(aafStringBuf, bufSize) );
    // print the name
    checkResult(printAAFString(aafStringBuf,  os));
    if (aafStringBuf)
	delete[]aafStringBuf;
    aafStringBuf=NULL;


    return status;
}
static HRESULT printAAFName(
		IAAFDefObject  *pDefObj,
		ostream		    &os )
{
    HRESULT	    status = AAFRESULT_SUCCESS;
    aafCharacter*   aafStringBuf = NULL;
    aafUInt32	    bufSize = 0;

    checkResult( pDefObj->GetNameBufLen( &bufSize ) );
    aafStringBuf = (aafCharacter*) new aafUInt8[ bufSize ];
    checkResult( pDefObj->GetName(aafStringBuf, bufSize) );
    // print the name
    checkResult(printAAFString(aafStringBuf,  os));
    if (aafStringBuf)
	delete[]aafStringBuf;
    aafStringBuf=NULL;


    return status;
}



static HRESULT printAAFName(
		IAAFTypeDef	*pTypeDef,
		ostream		&os )
{
    IAAFMetaDefinitionSP    pMetaDef;


    checkResult( pTypeDef->QueryInterface( IID_IAAFMetaDefinition,
			    (void**)&pMetaDef ) );

    return printAAFName( pMetaDef, os );
}



static HRESULT printAAFName(
		IAAFClassDef	*pClassDef,
		ostream		&os )
{
    IAAFMetaDefinitionSP    pMetaDef;


    checkResult( pClassDef->QueryInterface( IID_IAAFMetaDefinition,
			    (void**)&pMetaDef ) );

    return printAAFName( pMetaDef, os );
}



static HRESULT dumpTypeDef(
			   IAAFTypeDefSP pTypeDef, // object to be dumped
			   IAAFDictionary * pDict,  // dictionary for this file
			   dumpFlags_t dumpFlags,
			   int indent,
			   ostream & os
			   )
{
  HRESULT returnHr = AAFRESULT_SUCCESS;
	
  try
    {
      IAAFMetaDefinitionSP pMetaDef;
      // Get MetaDef pointer
      checkResult(pTypeDef->QueryInterface(IID_IAAFMetaDefinition,
					   (void**) &pMetaDef));
      IAAFTypeDefIntSP		    pTypeDefInt;
      IAAFTypeDefCharacterSP	    pTypeDefCharacter;
      IAAFTypeDefEnumSP		    pTypeDefEnum;
      IAAFTypeDefExtEnumSP	    pTypeDefExtEnum;
      IAAFTypeDefFixedArraySP	    pTypeDefFixedArray;
      IAAFTypeDefIndirectSP	    pTypeDefIndirect;
      IAAFTypeDefObjectRefSP	    pTypeDefObjectRef;
      IAAFTypeDefOpaqueSP	    pTypeDefOpaque;
      IAAFTypeDefRecordSP	    pTypeDefRecord;
      IAAFTypeDefRenameSP	    pTypeDefRename;
      IAAFTypeDefSetSP		    pTypeDefSet;
      IAAFTypeDefStreamSP	    pTypeDefStream;
      IAAFTypeDefStringSP	    pTypeDefString;
      IAAFTypeDefVariableArraySP    pTypeDefVariableArray;


      if (SUCCEEDED(pTypeDef->QueryInterface(IID_IAAFTypeDefInt,
					(void**) &pTypeDefInt)))
	{
	  os << "TypeDefinitionInteger for: ";
	  checkResult(dumpMetaDefCommon(pMetaDef, pDict,
					dumpFlags, indent, os));
	  aafUInt32 size=0;
	  checkResult(pTypeDefInt->GetSize(&size));
	  os << "Size: 0x" << size << endl;
	  aafBoolean_t isSigned;
	  checkResult(pTypeDefInt->IsSigned(&isSigned));
	  os << "IsSigned: ";
	  if (isSigned)
	    os << "true" << endl;
	  else
	    os << "false" << endl;
	}
      else
	//
	//  IAAFTypeDefCharacter
	//
	if (SUCCEEDED(pTypeDef->QueryInterface(IID_IAAFTypeDefCharacter,
					(void**) &pTypeDefCharacter)))
	{
	    os << "TypeDefinitionCharacter for: ";
	    checkResult( dumpMetaDefCommon( pMetaDef, pDict, 
		dumpFlags, indent, os ));
	}
	else
	//
	//  IAAFTypeDefEnum 
	//
	if (SUCCEEDED(pTypeDef->QueryInterface(IID_IAAFTypeDefEnum,
					(void**) &pTypeDefEnum)))
	{
	    IAAFTypeDefSP	    pElementTypeDef;
	    aafUInt32		    count;
	    aafUInt32		    i;
	    aafInt64		    elemValue;
	    aafCharacter	    *aafStringBuf = NULL;
	    aafUInt32		    bufSize;


	    os << "TypeDefinitionEnumeration for: ";
	    checkResult( dumpMetaDefCommon( pMetaDef, pDict, 
		dumpFlags, indent, os ));

	    checkResult( pTypeDefEnum->GetElementType( &pElementTypeDef ) );

	    // Print the name
	    os << "ElementType: ";
	    checkResult( printAAFName( pElementTypeDef, os ) );
	    os << endl;

	    // Get the number of enumeration elements 
	    checkResult( pTypeDefEnum->CountElements( &count ) );

	    os << "ElementNames:";
	    for( i=0; i<count; i++ )
	    {
		// Get enum element value
		checkResult( pTypeDefEnum->GetElementValue( i, &elemValue ) );

		// Get enum element name and print it out
		checkResult( pTypeDefEnum->GetNameBufLenFromInteger ( elemValue, &bufSize ) );
		aafStringBuf = (aafCharacter*)new aafUInt8[ bufSize ];
		checkResult( pTypeDefEnum->GetNameFromInteger( elemValue, aafStringBuf, bufSize ) );
		os << ' ';
		checkResult( printAAFString(aafStringBuf, os) );
		if( aafStringBuf )
		    delete[] aafStringBuf;
		aafStringBuf = NULL;
	    }
	    os << endl;

	    os << "ElementValues:";
	    for( i=0; i<count; i++ )
	    {
		checkResult( pTypeDefEnum->GetElementValue( i, &elemValue ) );
		os << ' ' << dec << (long)elemValue;
	    }
	    os << endl;
	}
	else
	//
	//  IAAFTypeDefExtEnum
	//
	if (SUCCEEDED(pTypeDef->QueryInterface(IID_IAAFTypeDefExtEnum,
					(void**) &pTypeDefExtEnum)))
	{
	    IAAFTypeDefSP	    pElementTypeDef;
	    aafUInt32		    count;
	    aafUInt32		    i;
	    aafUID_t		    elemValue;
	    aafCharacter	    *aafStringBuf = NULL;
	    aafUInt32		    bufSize;


	    os << "TypeDefinitionExtendibleEnumeration for: ";
	    checkResult( dumpMetaDefCommon( pMetaDef, pDict, 
		dumpFlags, indent, os ));

	    // Get the number of enumeration elements 
	    checkResult( pTypeDefExtEnum->CountElements( &count ) );

	    os << "ElementNames:";
	    for( i=0; i<count; i++ )
	    {
		// Get enum element value
		checkResult( pTypeDefExtEnum->GetElementValue( i, &elemValue ) );

		// Get enum element name and print it out
		checkResult( pTypeDefExtEnum->GetNameBufLenFromAUID( elemValue, &bufSize ) );
		aafStringBuf = (aafCharacter*)new aafUInt8[ bufSize ];
		checkResult( pTypeDefExtEnum->GetNameFromAUID( elemValue, aafStringBuf, bufSize ) );
		os << ' ';
		checkResult( printAAFString(aafStringBuf,  os) );
		if( aafStringBuf )
		    delete[] aafStringBuf;
		aafStringBuf = NULL;
	    }
	    os << endl;

	    os << "ElementValues:";
	    for( i=0; i<count; i++ )
	    {
		checkResult( pTypeDefExtEnum->GetElementValue( i, &elemValue ) );
		printAUID( elemValue, os );
	    }
	    os << endl;
	}
	else
	//
	//  IAAFTypeDefFixedArray
	//
	if (SUCCEEDED(pTypeDef->QueryInterface(IID_IAAFTypeDefFixedArray,
					(void**) &pTypeDefFixedArray)))
	{
	    IAAFTypeDefSP	    pElementTypeDef;
	    aafUInt32		    count;


	    os << "TypeDefinitionFixedArray for: ";
	    checkResult( dumpMetaDefCommon( pMetaDef, pDict, 
		dumpFlags, indent, os ));

	    // Print array elements type
	    checkResult( pTypeDefFixedArray->GetType( &pElementTypeDef ) );
	    os << "ElementType: ";
	    checkResult( printAAFName( pElementTypeDef, os ) );
	    os << endl;

	    // Print array elements count
	    checkResult( pTypeDefFixedArray->GetCount( &count ) );
	    os << "ElementCount: " << dec << (long)count << endl;
	}
	else
	//
	//  IAAFTypeDefIndirect
	//
	if (SUCCEEDED(pTypeDef->QueryInterface(IID_IAAFTypeDefIndirect,
					(void**) &pTypeDefIndirect)))
	{
	    os << "TypeDefinitionIndirect for: ";
	    checkResult( dumpMetaDefCommon( pMetaDef, pDict, 
		dumpFlags, indent, os ));
	}
	else
	//
	//  IAAFTypeDefOpaque
	//
	if (SUCCEEDED(pTypeDef->QueryInterface(IID_IAAFTypeDefOpaque,
					(void**) &pTypeDefOpaque)))
	{
	    os << "TypeDefinitionOpaque for: ";
	    checkResult( dumpMetaDefCommon( pMetaDef, pDict, 
		dumpFlags, indent, os ));
	}
	else
	//
	//  IAAFTypeDefRecord
	//
	if (SUCCEEDED(pTypeDef->QueryInterface(IID_IAAFTypeDefRecord,
					(void**) &pTypeDefRecord)))
	{
	    IAAFTypeDefSP	    pMemberTypeDef;
	    aafUInt32		    count;
	    aafUInt32		    i;
	    aafCharacter	    *aafStringBuf = NULL;
	    aafUInt32		    bufSize;


	    os << "TypeDefinitionRecord for: ";
	    checkResult( dumpMetaDefCommon( pMetaDef, pDict, 
		dumpFlags, indent, os ));

	    checkResult( pTypeDefRecord->GetCount( &count ) );

	    os << "MemberTypes:";
	    for( i=0; i<count; i++ )
	    {
		os << ' ';
		checkResult( pTypeDefRecord->GetMemberType( i, &pMemberTypeDef ) );
		checkResult( printAAFName( pMemberTypeDef, os ) );
	    }
	    os << endl;

	    os << "MemberNames: ";
	    for( i=0; i<count; i++ )
	    {
		// Get enum element name and print it out
		checkResult( pTypeDefRecord->GetMemberNameBufLen( i, &bufSize ) );
		aafStringBuf = (aafCharacter*)new aafUInt8[ bufSize ];
		checkResult( pTypeDefRecord->GetMemberName( i, aafStringBuf, bufSize ) );
		os << ' ';
		checkResult( printAAFString(aafStringBuf, os) );
		if( aafStringBuf )
		    delete[] aafStringBuf;
		aafStringBuf = NULL;
	    }
	    os << endl;
	}
	else
	//
	//  IAAFTypeDefRename
	//
	if (SUCCEEDED(pTypeDef->QueryInterface(IID_IAAFTypeDefRename,
					(void**) &pTypeDefRename)))
	{
	    IAAFTypeDefSP	    pRenamedTypeDef;


	    os << "TypeDefinitionRename for: ";
	    checkResult( dumpMetaDefCommon( pMetaDef, pDict, 
		dumpFlags, indent, os ));

	    // Print out the underlying type
	    checkResult( pTypeDefRename->GetBaseType( &pRenamedTypeDef ) );
	    os << "RenamedType: ";
	    checkResult( printAAFName( pRenamedTypeDef, os ) );
	    os << endl;
	}
	else
	//
	//  IAAFTypeDefSet
	//
	if (SUCCEEDED(pTypeDef->QueryInterface(IID_IAAFTypeDefSet,
					(void**) &pTypeDefSet)))
	{
	    IAAFTypeDefSP	    pElementTypeDef;


	    os << "TypeDefinitionSet for: ";
	    checkResult( dumpMetaDefCommon( pMetaDef, pDict, 
		dumpFlags, indent, os ));

	    // Print out set elements type
	    checkResult( pTypeDefSet->GetElementType( &pElementTypeDef ) );
	    os << "ElementType: ";
	    checkResult( printAAFName( pElementTypeDef, os ) );
	    os << endl;
	}
	else
	//
	//  IAAFTypeDefStream
	//
	if (SUCCEEDED(pTypeDef->QueryInterface(IID_IAAFTypeDefStream,
					(void**) &pTypeDefStream)))
	{
	    os << "TypeDefinitionStream for: ";
	    checkResult( dumpMetaDefCommon( pMetaDef, pDict, 
		dumpFlags, indent, os ));
	}
	else
	//
	//  IAAFTypeDefString
	//
	if (SUCCEEDED(pTypeDef->QueryInterface(IID_IAAFTypeDefString,
					(void**) &pTypeDefString)))
	{
	    IAAFTypeDefSP	    pStringTypeDef;


	    os << "TypeDefinitionString for: ";
	    checkResult( dumpMetaDefCommon( pMetaDef, pDict, 
		dumpFlags, indent, os ));

	    // Print out string elements type
	    checkResult( pTypeDefString->GetType( &pStringTypeDef ) );
	    os << "ElementType: ";
	    checkResult( printAAFName( pStringTypeDef, os ) );
	    os << endl;
	}
	else
	//
	//  IAAFTypeDefObjRef
	//
	if (SUCCEEDED(pTypeDef->QueryInterface(IID_IAAFTypeDefObjectRef,
					(void**) &pTypeDefObjectRef)))
	{
	    IAAFTypeDefObjectRefSP	pTypeDefDummy;
	    IAAFClassDefSP		pClassDef;

	    if( SUCCEEDED( pTypeDef->QueryInterface(IID_IAAFTypeDefStrongObjRef,
					(void**) &pTypeDefDummy) ) )
		os << "TypeDefinitionStrongObjectReference for: ";
	    else
	    if( SUCCEEDED( pTypeDef->QueryInterface(IID_IAAFTypeDefWeakObjRef,
					(void**) &pTypeDefDummy) ) )
		os << "TypeDefinitionWeakObjectReference for: ";
	    else
		os << "TypeDefinitionObjectReference for: ";

	    checkResult( dumpMetaDefCommon( pMetaDef, pDict, 
		dumpFlags, indent, os ));

	    os << "ReferencedType: ";
	    checkResult( pTypeDefObjectRef->GetObjectType( &pClassDef ) );
	    checkResult( printAAFName( pClassDef, os ) );
	    os << endl;
	}
	else
	//
	//  IAAFTypeDefVariableArray
	//
	if (SUCCEEDED(pTypeDef->QueryInterface(IID_IAAFTypeDefVariableArray,
					(void**) &pTypeDefVariableArray)))
	{
	    IAAFTypeDefSP	    pElementTypeDef;


	    os << "TypeDefinitionVariableArray for: ";
	    checkResult( dumpMetaDefCommon( pMetaDef, pDict, 
		dumpFlags, indent, os ));
	    
	    // Print out array elements type
	    checkResult( pTypeDefVariableArray->GetType( &pElementTypeDef ) );
	    os << "ElementType: ";
	    checkResult( printAAFName( pElementTypeDef, os ) );
	    os << endl;
	}
	else
	{
	  os << "TypeDefinition for: ";
	  checkResult(dumpMetaDefCommon(pMetaDef, pDict,
					dumpFlags, indent, os));
	}

	// QI for the concrete subclasses of TypeDefinition and print out properties
    }
  catch (HRESULT &caught)
    {
      os << endl << "***Caught hresult 0x" << hex << caught << "***" << endl;
      returnHr = caught;
    }
	
  return returnHr;
}

HRESULT dumpMetaDictionary(IAAFHeaderSP /*pHeader*/,
				   IAAFDictionary * pDict,
				   dumpFlags_t dumpFlags,
				   int indent,
				   ostream & os)
{
  HRESULT returnHr = AAFRESULT_SUCCESS;
  
  aafUInt32 countClassDefs=0;
  aafUInt32 countTypeDefs=0;
  try
    {
      os << endl;
      printIndent (indent, os);
      os << "Dumping Metadictionary of file" << endl;
      checkResult(pDict->CountTypeDefs(&countTypeDefs));
      os << "Dictionary contains 0x" << countTypeDefs << " type definitions" << endl;
      checkResult(pDict->CountClassDefs(&countClassDefs));
      os << "Dictionary contains 0x" << countClassDefs << " class definitions" << endl;
      
      if (countTypeDefs)
	{
	  IEnumAAFTypeDefsSP pTypeDefEnum;
	  IAAFTypeDefSP pTypeDef;
	  checkResult(pDict->GetTypeDefs(&pTypeDefEnum));
	  while (AAFRESULT_SUCCEEDED(pTypeDefEnum->NextOne(&pTypeDef)))
	    {
	      os << endl;
	      checkResult(dumpTypeDef(pTypeDef, pDict, dumpFlags,
				      indent, os));
	    }
	}
      if (countClassDefs)
	{
	  IEnumAAFClassDefs* pClassDefEnum=NULL;
	  IAAFClassDef* pClassDef=NULL;
	  checkResult(pDict->GetClassDefs(&pClassDefEnum));
	  while (AAFRESULT_SUCCEEDED(pClassDefEnum->NextOne(&pClassDef)))
	    {
	      os << endl;
	      checkResult(dumpClassDef(pClassDef, pDict, dumpFlags,
				       indent, os));
	      if (pClassDef)
		pClassDef->Release();
	      pClassDef=NULL;
	    }

	  if (pClassDefEnum)
	    pClassDefEnum->Release();
	  pClassDefEnum=NULL;
	}
    }
  catch (HRESULT &caught)
    {
      os << endl << "***Caught hresult 0x" << hex << caught << "***" << endl;
      returnHr = caught;
    }
	
  return returnHr;
}


HRESULT dumpHeaderObject(IAAFHeaderSP pHeader,
				   IAAFDictionary * pDict,
				   dumpFlags_t dumpFlags,
				   int indent,
				   ostream & os)
{
	HRESULT returnHr = AAFRESULT_SUCCESS;
	
	IEnumAAFPropertiesSP pPropEnum;
	IAAFPropertySP pProp;
	IAAFPropertyDefSP pPDef;
	IAAFPropertyValueSP pPVal;
	IAAFClassDefSP pClassDef;
	IAAFObjectSP pContainer;
	try
	{

	  // Get the contained properties.
	  checkResult (pHeader->QueryInterface (IID_IAAFObject,
						(void**)&pContainer));
	  checkResult(pContainer->GetProperties (&pPropEnum));
	  indent=0;
	  printIndent (indent, os);
	  os << endl << "Header: " << endl << endl;
	  // Enumerate across Properties
	  while (AAFRESULT_SUCCEEDED (pPropEnum->NextOne (&pProp)))
	    {
	      IAAFMetaDefinitionSP pPropMetaDef;
	      aafUID_t propID;
	      // Get property def
	      checkResult(pProp->GetDefinition(&pPDef));
			
	      // Skip ContentStorage, Dictionary
	      // Get MetaDef interface and get AUID and compare
	      checkResult(pPDef->QueryInterface(IID_IAAFMetaDefinition,
						    (void**) &pPropMetaDef));
	      checkResult(pPropMetaDef->GetAUID(&propID));
	      if ((memcmp(&propID,&kAAFPropID_Header_Content,16)) &&
		  (memcmp(&propID,&kAAFPropID_Header_Dictionary,16)))
		{
		  // For all properties in header except Content, Dictionary, Idents
		  // Here we print out property's name using pPDef->GetName()
		  aafUInt32 bufSize;
		  checkResult(pPDef->GetNameBufLen (&bufSize));
		  aafCharacter * nameBuf = (aafCharacter*) new aafUInt8[bufSize];
		  assert (nameBuf);
		  checkResult(pPDef->GetName(nameBuf, bufSize));
		  printIndent (indent, os);
		  checkResult(printAAFString(nameBuf, os));
		  delete[]nameBuf;
		  nameBuf=NULL;
		  os << ": ";
		  // Get the PropertyValue
		  checkResult(pProp->GetValue(&pPVal));
		  
		  // dump property value
		  checkResult (dumpPropertyValue (pPVal, pDict, dumpFlags, true, indent+1, os));
		  os << endl;
		}
	      
	    } // while ()...
	  if (dumpFlags.showAllHeaderProps)
	    {
	      aafProductVersion_t refImplVers;
	      aafVersionType_t fileVersion;
	      checkResult (pHeader->GetRefImplVersion(&refImplVers));
	      checkResult (pHeader->GetFileRevision(&fileVersion));
	      os << "Runtime Property RefImplVersion: struct major: "; 
	      os << "0x" << hex << refImplVers.major ;
	      os << " minor: ";
	      os << "0x" << hex << refImplVers.minor << endl;
	      os << " patchLevel: ";
	      os << "0x" << hex << refImplVers.patchLevel;
	      os << " releaseType: ";
	      os << "0x" << hex << refImplVers.type << endl;
	      os << "Header Property FileVersion: struct major: ";
	      aafUInt16 fileMajor=0, fileMinor=0;
	      fileMajor = fileVersion.major;
	      fileMinor = fileVersion.minor;
	      os << "0x" << hex << fileMajor ;
	      os << " minor: ";
	      os << "0x" << hex << fileMinor  << endl;
	    }
	}
	catch (HRESULT &caught)
	  {
	    os << endl << "***Caught hresult 0x" << hex << caught << "***" << endl;
	    returnHr = caught;
	  }
	
	return returnHr;
}
HRESULT dumpMobsAndCS(IAAFHeaderSP pHeader,
				   IAAFDictionary * pDict,
				   dumpFlags_t dumpFlags,
				   int indent,
				   ostream & os)
{
	HRESULT returnHr = AAFRESULT_SUCCESS;
	
	IEnumAAFPropertiesSP pPropEnum;
	IAAFPropertySP pProp;
	IAAFPropertyDefSP pPDef;
	IAAFPropertyValueSP pPVal;
	IAAFClassDefSP pClassDef;
	IAAFObjectSP pContainer;
	try
	{
	  // Check if there are any optional properties in ContentStorage
	  IAAFContentStorageSP pContStorage;
	  checkResult(pHeader->GetContentStorage(&pContStorage));
	  // Get the contained properties.
	  checkResult (pContStorage->QueryInterface (IID_IAAFObject,
						(void**)&pContainer));
	  checkResult(pContainer->GetProperties (&pPropEnum));
	  bool CShasOptionalProps=false;
	  // Enumerate across Properties
	  while (AAFRESULT_SUCCEEDED (pPropEnum->NextOne (&pProp)))
	    {
	      IAAFMetaDefinitionSP pPropMetaDef;
	      aafUID_t propID;
	      // Get property def
	      checkResult(pProp->GetDefinition(&pPDef));
			
	      // Skip ContentStorage, Dictionary
	      // Get MetaDef interface and get AUID and compare
	      checkResult(pPDef->QueryInterface(IID_IAAFMetaDefinition,
						    (void**) &pPropMetaDef));
	      checkResult(pPropMetaDef->GetAUID(&propID));
	      if ((memcmp(&propID,&kAAFPropID_ContentStorage_Mobs,16)) &&
		  (memcmp(&propID,&kAAFPropID_ContentStorage_EssenceData,16)))
		{
		  // we have an optional property
		  if (!CShasOptionalProps)
		    {
		      // first optional property
		      indent=0;
		      printIndent (indent, os);
		      os << "ContentStorage: " << endl;
		      CShasOptionalProps=true;
		    }
		  aafUInt32 bufSize;
		  checkResult(pPDef->GetNameBufLen (&bufSize));
		  aafCharacter * nameBuf = (aafCharacter*) new aafUInt8[bufSize];
		  assert (nameBuf);
		  checkResult(pPDef->GetName(nameBuf, bufSize));
		  printIndent (indent, os);
		  checkResult(printAAFString(nameBuf, os));
		  delete[]nameBuf;
		  os << ": ";
		  // Get the PropertyValue
		  checkResult(pProp->GetValue(&pPVal));
		  
		  // dump property value
		  checkResult (dumpPropertyValue (pPVal, pDict, dumpFlags, true, indent+1, os));
		  os << endl;
		}
	      
	    } // finished with ContentStorage
	  indent=0;
	  printIndent (indent, os);
	  os << "Mobs: " << endl;
	  IEnumAAFMobsSP pMobEnum;
	  IAAFMobSP pMob;
	  IAAFObjectSP pObject;
	  aafSearchCrit_t mobCrit;
	  mobCrit.searchTag= kAAFNoSearch;
	  checkResult(pHeader->GetMobs(&mobCrit,&pMobEnum));
	  // Enumerate accross all Mobs
	  while (AAFRESULT_SUCCEEDED(pMobEnum->NextOne(&pMob)))
	    {
	      // For now dump as object
	      checkResult(pMob->QueryInterface(IID_IAAFObject,
					       (void**) &pObject));
	      checkResult(dumpObject (pObject, pDict, dumpFlags, 0, os));

	    }

	    }
	catch (HRESULT &caught)
	  {
	    os << endl << "***Caught hresult 0x" << hex << caught << "***" << endl;
	    returnHr = caught;
	  }
	
	return returnHr;
}

HRESULT dumpEssence(IAAFHeaderSP pHeader,
				   IAAFDictionary * pDict,
				   dumpFlags_t dumpFlags,
				   int indent,
				   ostream & os)
{
	HRESULT returnHr = AAFRESULT_SUCCESS;
	
	IEnumAAFPropertiesSP pPropEnum;
	IAAFPropertySP pProp;
	IAAFPropertyDefSP pPDef;
	IAAFPropertyValueSP pPVal;
	IAAFClassDefSP pClassDef;
	IAAFObjectSP pContainer;
	try
	{
	  indent=0;
	  printIndent (indent, os);
	  os << "EssenceData: " << endl;
	  IEnumAAFEssenceData* pEssenceDataEnum=NULL;
	  IAAFEssenceData* pEssenceData=NULL;
	  IAAFObjectSP pObject;
	  checkResult(pHeader->EnumEssenceData(&pEssenceDataEnum));
	  // Enumerate over EssenceData
	  while (AAFRESULT_SUCCEEDED(pEssenceDataEnum->NextOne(&pEssenceData)))
	    {
	      // For now dump as object
	      checkResult(pEssenceData->QueryInterface(IID_IAAFObject,
					       (void**) &pObject));
	      checkResult(dumpObject (pObject, pDict, dumpFlags, 0, os));
	      pEssenceData->Release();
	      pEssenceData=NULL;
	    }
	 
	  if (pEssenceData)
	    pEssenceData->Release();
	  pEssenceData=NULL;
	  if (pEssenceDataEnum)
	    pEssenceDataEnum->Release();
	  pEssenceDataEnum=NULL;
	}
	catch (HRESULT &caught)
	  {
	    os << endl << "***Caught hresult 0x" << hex << caught << "***" << endl;
	    returnHr = caught;
	  }
	
	return returnHr;
}

HRESULT dumpSummary(IAAFHeaderSP pHeader,
				   IAAFDictionary * /*pDict*/,
				   dumpFlags_t /*dumpFlags*/,
				   int /*indent*/,
				   ostream & os)
{
	HRESULT returnHr = AAFRESULT_SUCCESS;
        aafUInt32 countMobs=0;
	aafUInt32 countEssence=0;

	try
	{
	  os << endl << "Summary Information:" << endl;
	  checkResult(pHeader->CountMobs(kAAFAllMob,&countMobs));
	  os << "File has 0x" << countMobs << " mobs" << endl;
	  checkResult(pHeader->CountEssenceData(&countEssence));
	  os << "File has 0x" << countEssence << " EssenceData objects" << endl;

	}
	catch (HRESULT &caught)
	{
		os << endl << "***Caught hresult 0x" << hex << caught << "***" << endl;
		returnHr = caught;
	}
	
	return returnHr;
}


HRESULT dumpObject(IAAFObjectSP pContainer,
				   IAAFDictionary * pDict,
				   dumpFlags_t dumpFlags,
				   int indent,
				   ostream & os)
{
	HRESULT returnHr = AAFRESULT_SUCCESS;
	
	IEnumAAFPropertiesSP pPropEnum;
	IAAFPropertySP pProp;
	IAAFPropertyDefSP pPDef;
	IAAFPropertyValueSP pPVal;
	IAAFClassDefSP pClassDef;
	bool showProps=true;
	
	try
	{
		// Get the contained properties.
		checkResult(pContainer->GetProperties (&pPropEnum));
		
		os << endl << endl;
		printIndent (indent, os);
		os << "Object of Class: ";
		checkResult(pContainer->GetDefinition (&pClassDef));
		aafUInt32 bufClassNameSize;
		checkResult(pClassDef->GetNameBufLen (&bufClassNameSize));
		aafCharacter * classNameBuf = new aafCharacter[bufClassNameSize];
		assert (classNameBuf);
		checkResult(pClassDef->GetName(classNameBuf, bufClassNameSize));
		char *mbString= new char[bufClassNameSize*MB_CUR_MAX];
		checkExpression(NULL != mbString, AAFRESULT_NOMEMORY);
		convert(mbString, bufClassNameSize*MB_CUR_MAX, classNameBuf);
		os << mbString << endl;
		if (dumpFlags.showOnlyClasses)
		{
		  assert(mbString);
		  char *substring;
		  substring=strstr(dumpFlags.showOnlyClasses,mbString);
		  if (!substring) // add check for whole word match
		    {
		      showProps=false;
		    }
		}
		delete[] mbString;
		delete[] classNameBuf;
		
		// Enumerate across Properties
		while (AAFRESULT_SUCCEEDED (pPropEnum->NextOne (&pProp)))
		{
				checkResult(pProp->GetDefinition(&pPDef));
				if (showProps)
			{
				// Here we print out property's name using pPDef->GetName()
				aafUInt32 bufSize;
				checkResult(pPDef->GetNameBufLen (&bufSize));
				aafCharacter * nameBuf = new aafCharacter[bufSize];
				assert (nameBuf);
				checkResult(pPDef->GetName(nameBuf, bufSize));
				printIndent (indent, os);
				checkResult(printAAFString(nameBuf, os));
				os << ": ";
				delete[] nameBuf;
			}
			// Get the PropertyValue
			checkResult(pProp->GetValue(&pPVal));
			
			// Handle DataDef specially
			// Get property def
			IAAFMetaDefinitionSP pPropMD;
			aafUID_t propID;
			checkResult(pPDef->QueryInterface(IID_IAAFMetaDefinition,
							  (void**) &pPropMD));
			checkResult(pPropMD->GetAUID(&propID));
			if (!memcmp(&propID, &kAAFPropID_Component_DataDefinition,sizeof(aafUID_t))
			     )
			  {
			    if (showProps)
			      {
				// it's component weak reference
				IAAFComponentSP pComponent;
				IAAFDataDefSP pDataDef;
				IAAFDefObjectSP pDataDefDO;
				checkResult(pContainer->QueryInterface(IID_IAAFComponent,
								       (void**) &pComponent));
				checkResult(pComponent->GetDataDef(&pDataDef));
				checkResult(pDataDef->QueryInterface(IID_IAAFDefObject,
								     (void**) &pDataDefDO));
				aafUInt32 bufSiz=0;
				checkResult(pDataDefDO->GetNameBufLen(&bufSiz));
				aafCharacter *nameBuf= (aafCharacter *) new aafUInt8[bufSiz];
				checkResult(pDataDefDO->GetName(nameBuf,bufSiz));
				printAAFString(nameBuf, os );
				delete[]nameBuf;
			      }
			  }
			else if (!memcmp(&propID,&kAAFPropID_FileDescriptor_CodecDefinition,
					 sizeof(aafUID_t)))
			  {
			    // it's a FileDescriptor weak reference
			    IAAFFileDescriptorSP pFileDescriptor;
			    IAAFCodecDefSP pCodecDef;
			    IAAFDefObjectSP pCodecDefDO;
			    checkResult(pContainer->QueryInterface(IID_IAAFFileDescriptor,
								   (void**) &pFileDescriptor));
			    checkResult(pFileDescriptor->GetCodecDef(&pCodecDef));
			    checkResult(pCodecDef->QueryInterface(IID_IAAFDefObject,
								 (void**) &pCodecDefDO));
			    aafUInt32 bufSiz=0;
			    checkResult(pCodecDefDO->GetNameBufLen(&bufSiz));
			    aafCharacter *nameBuf= (aafCharacter *) new aafUInt8[bufSiz];
			    checkResult(pCodecDefDO->GetName(nameBuf,bufSiz));
			    printAAFString(nameBuf, os);
			    delete[]nameBuf;

			  }
			else
			  {
				// dump property value
			    checkResult (dumpPropertyValue (pPVal, pDict, dumpFlags, showProps, indent+1, os));
			  }
			if (showProps)
			  os << endl;
			
		} // while ()...
	}
	catch (HRESULT &caught)
	{
		os << endl << "***Caught hresult 0x" << hex << caught << "***" << endl;
		returnHr = caught;
	}
	
	return returnHr;
}


HRESULT dumpPropertyValue (IAAFPropertyValueSP pPVal,
			   IAAFDictionary * pDict,
			   dumpFlags_t dumpFlags,
			   bool showProps,
			   int indent,
			   ostream & os)
{
  // get the type of the data value
  IAAFTypeDefSP pTD;
  HRESULT returnHr = AAFRESULT_SUCCESS;
  try
    {
  checkResult(pPVal->GetType(&pTD));
	
  if (! pTD)
    {
      // unknown type, due to damaged file.
		
      // Dump "damaged object" message.
      // Optionally, dump the property's bits using pPVal->GetBits().
      printIndent (indent, os);
      os << "***Unknown type def.***" << endl;
      return E_FAIL;
    }
	
  else // type is valid
    {
      IAAFMetaDefinitionSP pd;
      aafUID_t typeID;
      checkResult(pTD->QueryInterface(IID_IAAFMetaDefinition, (void**)&pd));
      checkResult(pd->GetAUID(&typeID));

      if (showProps && dumpFlags.showTypes)
	{
	  os << "(";
	  if (dumpFlags.identifybyname)
	    {
	      aafUInt32 bufSize=0;
	      aafCharacter* bigBuf=NULL;
	      checkResult(pd->GetNameBufLen(&bufSize));
	      bigBuf= (aafCharacter*) new aafUInt8[bufSize];
	      checkResult(pd->GetName (bigBuf, bufSize));
	      checkResult(printAAFString(bigBuf, os));
	      delete[]bigBuf;
	      bigBuf=NULL;
	      // convert(mbBigBuf, sizeof(mbBigBuf), bigBuf);
	      // os << mbBigBuf;
	      // fix convert or printAAFString
	    }
	  else
	    {
	      printAUID(typeID, os);
	    }
	  os << ")";
	}

      // get the type category of the data value
      eAAFTypeCategory_t tid;
      checkResult(pTD->GetTypeCategory (&tid));
      
     switch (tid) 
	{
	case kAAFTypeCatStrongObjRef:
	  {
	    // strong object reference; recursively dump contents.
	    IAAFTypeDefObjectRefSP pTDO;
	    AAFRESULT hr = pTD->QueryInterface(IID_IAAFTypeDefObjectRef,
					       (void**)&pTDO);
	    checkResult(hr);
	    
	    IAAFObjectSP pObj;
	    checkResult(pTDO->GetObject(pPVal, IID_IAAFObject, (IUnknown **)&pObj));
	    checkResult (dumpObject (pObj, pDict, dumpFlags, indent+1, os));
	    // can we have a strong obj ref to a MetaDefinition? yes, but not in prop direct?
	    break;	
	  }
	
	case kAAFTypeCatWeakObjRef:
	  {
	    // Not implemented in current SDK, treated as AUIDs
	    if (showProps)
	      {
		IAAFDefObjectSP pDefObj;
		IAAFMetaDefinitionSP pMetaDef;
		// weak object reference; only dump summary info (not
		// recursively)
		IAAFTypeDefObjectRefSP pTDO;
		aafCharacter *nameBuf=NULL;
		aafUInt32 bufSize=0;

		checkResult(pTD->QueryInterface(IID_IAAFTypeDefObjectRef,
						(void**)&pTDO));
		
		IAAFObjectSP pObj;
		
		if (!pTDO->GetObject(pPVal, IID_IAAFDefObject, (IUnknown**)&pDefObj))
		  {
		    printAAFName(pDefObj, os);
		  } else if (!pTDO->GetObject(pPVal, IID_IAAFMetaDefinition, 
					      (IUnknown**)&pMetaDef))
		  {
		    checkResult(printAAFName(pMetaDef, os));
		  } else
		  {
		    // treat as AUID
		    os << "**need AUID here**" << endl;
		  }
		if (nameBuf) delete[]nameBuf;
		nameBuf=NULL;
	      }		
	    break;	
	  }
						
	case kAAFTypeCatRename:
	  {
	    // Now base property value and recursively print that
	    IAAFTypeDefRenameSP pTDR;
	    checkResult(pTD->QueryInterface(IID_IAAFTypeDefRename,
					    (void**) &pTDR));
	    IAAFPropertyValueSP pBasePropVal;
	    checkResult(pTDR->GetBaseValue (pPVal, &pBasePropVal));
	    checkResult (dumpPropertyValue (pBasePropVal,
					    pDict, dumpFlags, showProps,
					    indent+1,
					    os));
	    
	    break;
	  }
	
	case kAAFTypeCatInt:
	  {
	    if (showProps)
	      {
		// integral type; get the specific type def int interface
		IAAFTypeDefIntSP pTDI;
							
		checkResult(pTD->QueryInterface(IID_IAAFTypeDefInt,
					    (void**)&pTDI));
							
		aafInt64 val;
		checkResult(pTDI->GetInteger(pPVal, (aafMemPtr_t) &val, sizeof (val)));
		
		aafInt32 hi = (aafUInt32) ((val & AAFCONSTINT64(0xffffffff00000000)) >> 32);
		aafInt32 lo = (aafInt32) val & 0xffffffff;
		if (hi && ((hi != ~0) || (lo >= 0)))
		  {
		    int width = os.width();
		    char fill = os.fill();
		    os << hex << "0x";
		    os.width(8);
		    os.fill('0');
		    os << hi << lo;
		    os.width(width);
		    os.fill(fill);
		  }
		else
		  {
		    os << "0x" << hex << lo;
		  }
	      }				
	    break;
	  }
						
	case kAAFTypeCatCharacter:
	  {
	    if (showProps)
	      {
		// aafCharacter  type; 
		IAAFTypeDefCharacterSP pTDChar;
							
		checkResult(pTD->QueryInterface(IID_IAAFTypeDefCharacter,
						(void**)&pTDChar));
							
		aafCharacter val;
		checkResult(pTDChar->GetCharacter(pPVal, &val));
							
		os << "0x" << hex << val;
	      }				
	    break;
	  }
						
	case kAAFTypeCatEnum:
	  {
	    if (showProps)
	      {
		// Print enum element's text tag as well as int value
		IAAFTypeDefEnumSP pTDE;
		checkResult(pTD->QueryInterface(IID_IAAFTypeDefEnum,
						(void**)&pTDE));
							
		// first, get the int value
		aafInt64 enumValue;
		checkResult(pTDE->GetIntegerValue(pPVal, &enumValue));
							
		// now, get the text tag for that value.  Start with name
		// buf len, and allocating a buffer to hold the name
		aafCharacter * nameBuf;
		aafUInt32 nameBufLen;
		checkResult(pTDE->GetNameBufLenFromInteger(enumValue, &nameBufLen));
		// don't forget NameBufLen is in bytes, not aafCharacters
		nameBuf = (aafCharacter*) new aafUInt8[nameBufLen];
		
		// and now get the name itself
		checkResult(pTDE->GetNameFromInteger(enumValue, nameBuf, nameBufLen));
							
		// Print the contents
		checkResult(printAAFString(nameBuf, os));
		delete[] nameBuf;
		nameBuf = 0;
	      }			
	    break;
	  }
						
	case kAAFTypeCatExtEnum:
	  {
	    if (showProps)
	      {
		// Print enum element's text tag
		IAAFTypeDefExtEnumSP pTDE;
		checkResult(pTD->QueryInterface(IID_IAAFTypeDefExtEnum,
						(void**)&pTDE));
							
		// first, get the AUID value
		aafUID_t enumValue;
		checkResult(pTDE->GetAUIDValue(pPVal, &enumValue));
		
		// now, get the text tag for that value.  Start with name
		// buf len, and allocating a buffer to hold the name
		aafCharacter * nameBuf;
		aafUInt32 nameBufLen;
		checkResult(pTDE->GetNameBufLenFromAUID(enumValue, &nameBufLen));
		// don't forget NameBufLen is in bytes, not aafCharacters
		nameBuf = (aafCharacter*) new aafUInt8[nameBufLen];
							
		// and now get the name itself
		checkResult(pTDE->GetNameFromAUID(enumValue, nameBuf, nameBufLen));
							
		// Print the contents
		checkResult(printAAFString(nameBuf,  os));
		delete[] nameBuf;
		nameBuf = 0;
	      }		
	    break;
	      }
						
	case kAAFTypeCatFixedArray:
	  {
	    // Print out elements of array.
	    IAAFTypeDefFixedArraySP pTDFA;
	    checkResult(pTD->QueryInterface(IID_IAAFTypeDefFixedArray,
					    (void**)&pTDFA));
	    
	    // Get number of elements
	    aafUInt32 numElems = 0;
	    checkResult(pTDFA->GetCount(&numElems));
	    
	    if (dumpFlags.showTypes && showProps)
	      {
		os << "fixed-sized array[0x" << numElems << "]:" << endl;
	      }
	    aafUInt32 i;
	    for (i = 0; i < numElems; i++)
	      {
		printIndent (indent, os);
		printAAFName(pTD, os);
		os << "  [" << i << "]: ";

		IAAFPropertyValueSP pElemPropVal;
		// Get array elements using index
		checkResult(pTDFA->GetElementValue(pPVal, i, &pElemPropVal));
		checkResult (dumpPropertyValue (pElemPropVal,
						pDict,
						dumpFlags, showProps,
						indent+1,
						os));
	      }
	    
	    break;
	  }
	
	case kAAFTypeCatVariableArray:
	  {
	    // Print out elements of array.
	    IAAFTypeDefVariableArraySP pTDVA;
	    checkResult(pTD->QueryInterface(IID_IAAFTypeDefVariableArray,
					    (void**)&pTDVA));
	    
	    // Get number of elements
	    aafUInt32 numElems=0;
	    checkResult(pTDVA->GetCount(pPVal, &numElems));
	    // Get type
	    IAAFTypeDefSP pElemTD;
	    IAAFTypeDefObjectRefSP pElemTDObjRef;
	    IAAFTypeDefStrongObjRefSP pElemTDStrongObjRef;
	    IAAFTypeDefWeakObjRefSP pElemTDWeakObjRef;
	    IAAFClassDefSP pElemTDObjRefCD;

	    checkResult(pTDVA->GetType(&pElemTD));
	    if (SUCCEEDED(pElemTD->QueryInterface(IID_IAAFTypeDefObjectRef,
						   (void**) &pElemTDObjRef)))
	      {
		// Treat array of strong and weak object references specially
		if (SUCCEEDED(pElemTD->QueryInterface(IID_IAAFTypeDefStrongObjRef,
						      (void**) &pElemTDStrongObjRef)))
		  os << "Strong Object Reference Array";
		else if (SUCCEEDED(pElemTD->QueryInterface(IID_IAAFTypeDefWeakObjRef,
						      (void**) &pElemTDStrongObjRef)))
		  os << "Weak Object Reference Array";
		else
		  os << "Unknown-type object reference array";
		os << " [" << numElems << "] of ";
		checkResult(pElemTDObjRef->GetObjectType(&pElemTDObjRefCD));
		printAAFName( pElemTDObjRefCD, os);
		os << endl;

		aafUInt32 i;
		for (i = 0; i < numElems; i++)
		  {
		    printIndent (indent, os);
		    printAAFName( pTD, os);
		    os << " [" << hex << i << "]: ";

		    IAAFPropertyValueSP pElemPropVal;
		    checkResult(pTDVA->GetElementValue(pPVal, i, &pElemPropVal));
		    checkResult (dumpPropertyValue (pElemPropVal,
						    pDict,
						    dumpFlags, showProps,
						    indent+1,
						    os));
		  }
	    
		break;
	      }
	    else
	      {
		// Handle arrays other than object references
		if (dumpFlags.showTypes && showProps)
		  {
		    os << "variably-sized array [0x" << hex << numElems << "]:" << endl;
		  }
		aafUInt32 i;
		for (i = 0; i < numElems; i++)
		  {
		    if (showProps)
		      {
			printIndent (indent, os);
			os << "[" << hex << i << "]: ";
		      }
		    IAAFPropertyValueSP pElemPropVal;
		    checkResult(pTDVA->GetElementValue(pPVal, i, &pElemPropVal));
		    checkResult (dumpPropertyValue (pElemPropVal,
						    pDict,
						    dumpFlags, showProps,
						    indent+1,
						    os));
		  }
	      }
	    
	    break;
	  }
	
	case kAAFTypeCatSet:
	  {
	    // Print out elements of set (only set of references implemented)
	    IAAFTypeDefSetSP pTDSet;
	    checkResult(pTD->QueryInterface(IID_IAAFTypeDefSet, (void**)&pTDSet));
	    // Get number of elements
	    aafUInt32 numElems;
	    checkResult(pTDSet->GetCount(pPVal, &numElems));
				
	    os << "Set[" << numElems << "] of " ;
	    IAAFTypeDefSP pElementTD;
	    checkResult(pTDSet->GetElementType(&pElementTD));
	    printAAFName(pElementTD, os);
	    os << endl;
	    IEnumAAFPropertyValuesSP pPVEnum;
	    checkResult(pTDSet->GetElements(pPVal, &pPVEnum));
	    HRESULT result= S_OK;
	    aafUInt32 i=0;
	    IAAFPropertyValueSP pElemPropVal;
	    while (SUCCEEDED(pPVEnum->NextOne(&pElemPropVal)))
	      {
		printIndent (indent, os);
		os << "[" << i << "]: ";
		result = dumpPropertyValue (pElemPropVal,
					    pDict,
					    dumpFlags, showProps,
					    indent+1,
					    os);
		++i;
	      }
	    
	    break;
	  }
	case kAAFTypeCatString:
	  {
	    if (showProps)
	      {
		IAAFTypeDefStringSP pTDS;
		checkResult(pTD->QueryInterface(IID_IAAFTypeDefString,
						(void**)&pTDS));
							
		// Get typedef of an element
		IAAFTypeDefSP pETD;
		checkResult(pTDS->GetType(&pETD));
							
		// get the type category of the element
		eAAFTypeCategory_t elemTID;
		checkResult(pETD->GetTypeCategory (&elemTID));
							
		//ELEM type should be EITHER :	INT or CHAR
							
		if (kAAFTypeCatCharacter == elemTID)
		  {
		    
		    IAAFTypeDefCharacterSP pETDcharacter;
		    checkResult(pETD->QueryInterface(IID_IAAFTypeDefCharacter,
						     (void**) &pETDcharacter));
								
		    // determine the sizes of elements, and of the buffer
		    // required to hold them.
		    aafUInt32 bufSize = 0;
		    aafUInt32 count = 0;
		    aafUInt32 elemSize = 0;
		    checkResult(pTDS->GetCount(pPVal, &count));
		    count ++; // make room for terminator
		    //			checkResult(pETDcharacter->GetSize (&elemSize));
		    // The element size must be the in-memory size of an aafCharacter since
		    // the character type currently only supports 2 byte <=> 2 byte and
		    // 2 byte <=> 4 byte character conversion. There is no support for
		    // non-unicode character conversion: 1 byte <=> 2 byte, or 1 byte <=> 4 byte.
		    elemSize = sizeof(aafCharacter);
		    bufSize = count * elemSize;
								
		    // First, let's actually get the bits.
								
		    // get bits
		    aafCharacter * buf = new aafCharacter[count];
		    memset (buf, 0, bufSize);  // zero all, including terminator
		    checkResult(pTDS->GetElements(pPVal, (aafDataBuffer_t)buf, bufSize));
								
		    // NULL-terminated wide character string.
		    // create an ansi/asci
		    os << "\"";
		    checkResult(printAAFString((aafCharacter*) buf, os));
		    os  << "\"" ;
								
		    assert (buf);	  delete[] buf; 	  buf = 0;
		    break;
		  }//IF CHARACTER
							
		else if (kAAFTypeCatInt == elemTID) 
		  {
		    IAAFTypeDefIntSP pETDInt;
		    checkResult(pETD->QueryInterface(IID_IAAFTypeDefInt,
						     (void**) &pETDInt));
								
		    // determine the sizes of elements, and of the buffer
		    // required to hold them.
		    aafUInt32 bufSize = 0;
		    aafUInt32 count = 0;
		    aafUInt32 elemSize = 0;
		    checkResult(pTDS->GetCount(pPVal, &count));
		    count ++; // make room for terminator
		    checkResult(pETDInt->GetSize (&elemSize));
		    bufSize = count * elemSize;
								
		    // See if this is a type we can easily represent.  We know
		    // it is some kind of integral type.  Maybe we can dump
		    // it.	First, let's actually get the bits.
								
		    // get bits
		    aafUInt8 * buf = new aafUInt8[bufSize];
		    memset (buf, 0, bufSize);  // zero all, including terminator
		    checkResult(pTDS->GetElements(pPVal, buf, bufSize));
								
		    // Now determine size of integral elements
		    if (1 == elemSize)
		      {
			// 1-byte integer characters; interpret as a
			// NULL-terminated C string.
			printIndent (indent, os);
			os << "\"" << buf << "\"" ;
		      }
		    else if (2 == elemSize)
		      {
			// 2-byte integral characters; interpret as a
			// NULL-terminated wide character string.
			// create an ansi/asci
			os << "\"";
			checkResult(printAAFString((aafCharacter*) buf, os));
			os  << "\"" ;

		      }
		    else
		      {
			// elements are too wide to be C or unicode strings;
			// perhaps just hex dump them, looking for terminator
			// character.
									
			// But for now, we won't bother dumping them... ;)
			assert (0);
		      }
								
		    assert (buf);	  delete[] buf; 	  buf = 0;
		  }//if INT
		else
		  {
		    // Hmmmm ... Elem Type is NEITHER a  Character NOR an INT ...
		    //	....  flag this as a problem ?! 
		    assert (0);
		  }
		break;
							
	      }
	    } //kAAFTypeCatString
		  
	  case kAAFTypeCatRecord:
	  {
	    if (showProps)
	      {
		aafUInt32 i;
		IAAFTypeDefRecordSP pTDR;
		checkResult(pTD->QueryInterface(IID_IAAFTypeDefRecord,
						(void**)&pTDR));
				
		// Special cases
		char buf[100];
		assert (pDict);
		IUnknown * pUnkTest = 0;
		IAAFTypeDefSP ptd;
		checkResult(pTDR->QueryInterface(IID_IUnknown,
						 (void**)&pUnkTest));
		// See if it is an AUID
		checkResult (pDict->LookupTypeDef (kAAFTypeID_AUID, &ptd));
		IUnknown * pUnkAUID = 0;
		checkResult(ptd->QueryInterface(IID_IUnknown,
						(void**)&pUnkAUID));
		// See if it is a MobID
		checkResult (pDict->LookupTypeDef (kAAFTypeID_MobID, &ptd));
		IUnknown * pUnkMobID = 0;
		checkResult(ptd->QueryInterface(IID_IUnknown,
						(void**)&pUnkMobID));
		// See if it is a timestamp
		checkResult (pDict->LookupTypeDef (kAAFTypeID_TimeStamp, &ptd));
		IUnknown * pUnkTimeStamp = 0;
		checkResult(ptd->QueryInterface(IID_IUnknown,
						(void**)&pUnkTimeStamp));

		// See if it is a rational
		checkResult (pDict->LookupTypeDef (kAAFTypeID_Rational, &ptd));
		IUnknown *pUnkRational=0;
		checkResult(ptd->QueryInterface(IID_IUnknown,
						(void**)&pUnkRational));
				
		if (pUnkTest == pUnkAUID)
		  {
		    // dump it as an auid
		    aafUID_t auidVal;
		    checkResult (pTDR->GetStruct (pPVal,
						  (aafMemPtr_t) &auidVal,
						  sizeof (auidVal)));
		    printAUID(auidVal, os);
		  }
				
		else if (pUnkTest == pUnkMobID)
		  {
		    // dump it as a MobID
		    aafMobID_t mobIDVal;
		    checkResult (pTDR->GetStruct (pPVal,
						  (aafMemPtr_t) &mobIDVal,
						  sizeof (mobIDVal)));
		    printMobID(mobIDVal, os);
		  }
		else if (pUnkTest == pUnkTimeStamp)
		  {
		    // dump it as an aafTimeStamp_t
		    aafTimeStamp_t ts;
		    checkResult (pTDR->GetStruct (pPVal,
						  (aafMemPtr_t) &ts,
						  sizeof (ts)));
					
		    printTimeStamp (ts, os);
		  }
		else if (pUnkTest == pUnkRational)
		  {
		    // dump it as a Rational
		    aafRational_t rationalVal;
		    checkResult (pTDR->GetStruct (pPVal,
						  (aafMemPtr_t) &rationalVal,
						   sizeof (rationalVal)));
		    sprintf (buf, "%i/%i",rationalVal.numerator,rationalVal.denominator);
		    os << "{ " << buf << " }";
		  }
				
		else
		  {
		    // dump it as a plain record
		    // get the number of members in this record
		    aafUInt32 numMembers = 0;
		    checkResult(pTDR->GetCount(&numMembers));
	
		    if (dumpFlags.showTypes)
		      {
			os << "record of " << numMembers << " members:" << endl;
		      }
		    else
		      {
			os << "{ ";
		      }
		    // recursively dump each struct field
		    for (i = 0; i < numMembers; i++)
		      {
			aafUInt32 nameBufLen;
			if (dumpFlags.showTypes)
			  {
			    checkResult(pTDR->GetMemberNameBufLen(i, &nameBufLen));
			    // nameBufLen is in bytes, not aafCharacters
			    aafCharacter * nameBuf = (aafCharacter*) new aafUInt8 [nameBufLen];
			    assert (nameBuf);
			    checkResult(pTDR->GetMemberName(i, nameBuf, nameBufLen));
			    // create an ansi/asci
			    printIndent (indent, os);
			    os << "  [";
			    checkResult(printAAFString(nameBuf, os));
			    os  << "]: ";
			    delete[] nameBuf;
			  }
			IAAFPropertyValueSP pMemberPropVal;
			checkResult(pTDR->GetValue(pPVal, i, &pMemberPropVal));
			// recursively dump prop value
			checkResult (dumpPropertyValue (pMemberPropVal,
							pDict,
							dumpFlags, showProps,
							indent+1,
							os));
			if (showProps)
			  {
			    if (dumpFlags.showTypes && (i < numMembers-1))
			      os << endl;
			    else if (i < numMembers-1)
			      os << ", ";
			    else
			      os << " }";
			  }
		      }
		  }
		if (pUnkTest)
		  {
		    pUnkTest->Release ();
		    pUnkTest = 0;
		  }
		if (pUnkAUID)
		  {
		    pUnkAUID->Release ();
		    pUnkAUID = 0;
		  }
		if (pUnkTimeStamp)
		  {
		    pUnkTimeStamp->Release ();
		    pUnkTimeStamp = 0;
		  }
		if (pUnkMobID)
		  pUnkMobID->Release();
		pUnkMobID=0;
		if (pUnkRational)
		  pUnkRational->Release();
		pUnkRational=0;
	      }
	    break;
	  }
		  
	case kAAFTypeCatIndirect:
	  {
	    if (showProps)
	      {
		IAAFTypeDefIndirectSP pIndirectType;
		checkResult(pTD->QueryInterface(IID_IAAFTypeDefIndirect,
						(void**)&pIndirectType));
		if (!dumpFlags.showTypes) // if we are not showing types
		  {
		    // Do print out the indirect type here
				
		    os << "(indirect type: " ;
		    IAAFTypeDefSP pActualType;
		    IAAFMetaDefinitionSP pActualMetaDef;

		    checkResult(pIndirectType->GetActualType(pPVal,&pActualType));
		    checkResult(pActualType->QueryInterface(IID_IAAFMetaDefinition,
							    (void**) &pActualMetaDef));
		    aafCharacter bigBuf[100];
		    char mbBigBuf[100 * 3 /*MB_CUR_MAX */];
		    checkResult(pActualMetaDef->GetName (bigBuf, sizeof (bigBuf)));
		    convert(mbBigBuf, sizeof(mbBigBuf), bigBuf);
		    os << mbBigBuf << ") ";
		  }

		// Print the actual value of the indirect value
		IAAFPropertyValueSP pActualValue;
		checkResult(pIndirectType->GetActualValue(pPVal, &pActualValue));
		// recursively dump prop value
		checkResult (dumpPropertyValue (pActualValue,
						pDict,
						dumpFlags, showProps,
						indent+1,
						os));
	      }
	    break;
	  }
			
	case kAAFTypeCatOpaque:
	  {
	    if (showProps)
	      {
		// Print out the actual value of the opaque type.
		IAAFTypeDefOpaqueSP pOpaqueType;
		checkResult(pTD->QueryInterface(IID_IAAFTypeDefOpaque,
						(void**)&pOpaqueType));
				
		// Get the opaque type id
		aafUID_t opaqueTypeID = {0};
		checkResult(pOpaqueType->GetActualTypeID(pPVal, &opaqueTypeID));
				
		os << "Opaque Key = " ;
		printAUID(opaqueTypeID, os);

		// Check to see if opaque key is a registered type
		IAAFTypeDefSP pType;
		if (FAILED(pDict->LookupTypeDef(opaqueTypeID, &pType)))
		  {
		    // If not then register a corresponding opaque type in the
		    // dictionary that is a rename of UInt8Array.	   
		    IAAFTypeDefSP pBaseType;
		    IAAFClassDefSP pRenamedClassDef;
		    IAAFTypeDefRenameSP pRenamedType;
		    
		    
		    checkResult(pDict->LookupTypeDef(kAAFTypeID_UInt8Array, 
						     &pBaseType));
		    checkResult(pDict->CreateMetaInstance(AUID_AAFTypeDefRename, 
							  IID_IAAFTypeDefRename, 
							  (IUnknown **)&pRenamedType));
		    checkResult(pRenamedType->Initialize (opaqueTypeID, 
							  pBaseType, 
							  L"Opaque Data"));
		    checkResult(pRenamedType->QueryInterface(IID_IAAFTypeDef, 
							     (void **)&pType));
		    checkResult(pDict->RegisterOpaqueTypeDef(pType));
		  }
				
		// Opaque types objects also implement the indirect type interface
		// to access the actual value data when the type is "known" either
		// public or opaque.
		IAAFTypeDefIndirectSP pIndirectType;
		checkResult(pTD->QueryInterface(IID_IAAFTypeDefIndirect,
						(void**)&pIndirectType));
				
		// Get the actual value
		IAAFPropertyValueSP pActualValue;
		checkResult(pIndirectType->GetActualValue(pPVal, &pActualValue));

		// print leader
		os << endl;
		printIndent (indent,os);
		os << "Opaque data: ";
		// recursively dump prop value
		checkResult (dumpPropertyValue (pActualValue,
						pDict,
						dumpFlags, showProps,
						indent+1,
						os));
	      }
	    break;
	  }
			
	case kAAFTypeCatStream:
	  {
	    if (showProps)
	      {
		// Print out the actual value of the stream type.
		checkResult(dumpRawStreamPropertyValue(pPVal, pTD, pDict, dumpFlags, indent, os));
	      }
	    break;
	  }
			
	default:
	  os << "***Unknown type category " << dec << (int) tid
	     << ".***" << endl;
	  return E_FAIL;
	  break;
	}
    } // !pTD
    }
  catch (HRESULT &caught)
    {
      os << endl << "***Caught hresult 0x" << hex << caught << "***" << endl;
      returnHr = caught;
    }
  
  return returnHr;

}

HRESULT dumpRawStreamPropertyValue
(
 IAAFPropertyValueSP pPVal,
 IAAFTypeDef *pTD,
 IAAFDictionary * /*pDict*/,  // dictionary for this file
 dumpFlags_t dumpFlags,
 int /*indent*/,
 ostream & os
 )
{
	// Interpret values 0x00 - 0xff as ASCII characters.
	// 
	static const char asciiTable[256] = 
	{
		'.',  '.',  '.',  '.',  '.',  '.',  '.',  '.',
			'.',  '.',  '.',  '.',  '.',  '.',  '.',  '.',
			'.',  '.',  '.',  '.',  '.',  '.',  '.',  '.',
			'.',  '.',  '.',  '.',  '.',  '.',  '.',  '.',
			' ',  '!',  '"',  '#',  '$',  '%',  '&', '\'',
			'(',  ')',  '*',  '+',  ',',  '-',  '.',  '/',
			'0',  '1',  '2',  '3',  '4',  '5',  '6',  '7',
			'8',  '9',  ':',  ';',  '<',  '=',  '>',  '?',
			'@',  'A',  'B',  'C',  'D',  'E',  'F',  'G',
			'H',  'I',  'J',  'K',  'L',  'M',  'N',  'O',
			'P',  'Q',  'R',  'S',  'T',  'U',  'V',  'W',
			'X',  'Y',  'Z',  '[', '\\',  ']',  '^',  '_',
			'`',  'a',  'b',  'c',  'd',  'e',  'f',  'g',
			'h',  'i',  'j',  'k',  'l',  'm',  'n',  'o',
			'p',  'q',  'r',  's',  't',  'u',  'v',  'w',
			'x',  'y',  'z',  '{',  '|',  '}',  '~',  '.',
			'.',  '.',  '.',  '.',  '.',  '.',  '.',  '.',
			'.',  '.',  '.',  '.',  '.',  '.',  '.',  '.',
			'.',  '.',  '.',  '.',  '.',  '.',  '.',  '.',
			'.',  '.',  '.',  '.',  '.',  '.',  '.',  '.',
			'.',  '.',  '.',  '.',  '.',  '.',  '.',  '.',
			'.',  '.',  '.',  '.',  '.',  '.',  '.',  '.',
			'.',  '.',  '.',  '.',  '.',  '.',  '.',  '.',
			'.',  '.',  '.',  '.',  '.',  '.',  '.',  '.',
			'.',  '.',  '.',  '.',  '.',  '.',  '.',  '.',
			'.',  '.',  '.',  '.',  '.',  '.',  '.',  '.',
			'.',  '.',  '.',  '.',  '.',  '.',  '.',  '.',
			'.',  '.',  '.',  '.',  '.',  '.',  '.',  '.',
			'.',  '.',  '.',  '.',  '.',  '.',  '.',  '.',
			'.',  '.',  '.',  '.',  '.',  '.',  '.',  '.',
			'.',  '.',  '.',  '.',  '.',  '.',  '.',  '.',
			'.',  '.',  '.',  '.',  '.',  '.',  '.',  '.'
	};
	
	
	// Print out the actual value of the stream type.
	AAFRESULT streamResult = AAFRESULT_SUCCESS;
	try
	  {
	    IAAFTypeDefStreamSP pStreamType;
	    checkResult(pTD->QueryInterface(IID_IAAFTypeDefStream,
					    (void**)&pStreamType));
	
	    // Get number of bytes
	    aafInt64 streamSize = 0;
	    checkResult(pStreamType->GetSize(pPVal, &streamSize));
	
	    if (0x00000000FFFFFFFF < streamSize)
	      {
		os << "  Large (64-bit) stream not supported in ascii dump!" << endl;
		os << "  WARNING: Stream will be truncated!" << endl;
	      }
	
	    aafUInt32 bytesLeft = (aafUInt32)(streamSize);
	    os << "{/* size: " << bytesLeft << " bytes */" << endl;
	
	    if ((dumpFlags.maxCount > 0) && (dumpFlags.maxCount < bytesLeft))
	      {
		bytesLeft=dumpFlags.maxCount;
		os << "Showing only first 0x" << bytesLeft << " bytes" << endl;
	      }
	    const aafUInt32 kStreamBytesPerLine = 16;
	    const aafUInt32 kStreamReadSize = kStreamBytesPerLine;
	    const aafUInt32 kOffsetWidth = 8;
	    aafUInt8 streamBuffer[kStreamReadSize];
	    aafUInt32 bytes = 0;
	    aafUInt32 bytesRead = 0;
	    aafUInt32 offset = 0;
	
    
	    while ((0 < bytesLeft) && AAFRESULT_SUCCEEDED(streamResult))
	      {
		if (bytesLeft > kStreamReadSize)
		  bytes = kStreamReadSize;
		else
		  bytes = bytesLeft;
		
		streamResult = pStreamType->Read(pPVal, bytes, streamBuffer, &bytesRead);
		if (AAFRESULT_FAILED(streamResult))
		  {
		    os << "  // FAILED after reading " << (aafUInt32)streamSize - bytesLeft << " bytes." << endl;
		    break;
		}
		bytesLeft -= bytesRead;
		
		
		os << " /*" << dec << setw(kOffsetWidth) << offset << " */";  
		offset += kStreamBytesPerLine;    
		
		aafUInt32 i;
		
		IOS_FMT_FLAGS savedFlags = os.setf(ios::basefield);
		char savedFill = cout.fill();
		
		for (i = 0; (i < kStreamBytesPerLine) && (i < bytesRead);)
		  {
		    os << " 0x" << hex << setw(2) << setfill('0') << (int)streamBuffer[i++];
		    if (i == bytesRead && 0 == bytesLeft)
		      {
			os << " ";
			break;
		      }
		    
		    os << ",";
		  }
		
		os.setf(savedFlags, ios::basefield);
		os.fill(savedFill);      
		
		
		// add padding on last row if necessary.
		for (aafUInt32 j = i; (j < kStreamBytesPerLine); ++j)
		  {
		    os << "      ";
		  }
		
		// dump the ascii representation of the stream bytes
		os << " /*";
		for (i = 0; i < bytesRead; ++i)
		  {
		    os << asciiTable[streamBuffer[i]];
		  } 
		os << "*/";
		
		os << endl;
	      }
	
	
	    os << "};" << endl;
	    os << dec << endl;
	  }
	catch (HRESULT &caught)
	  {
	    os << endl << "***Caught hresult 0x" << hex << caught << "***" << endl;
	    streamResult = caught;
	  }
	
	return streamResult;
}


//
// Dumps the given file.  Returns true if successful; returns false if
// an error was encountered.
//
static bool dumpFile (aafCharacter * pwFileName,
					  const char * name,
					  const dumpFlags_t dumpFlags,
					  ostream & os)
{
	assert (pwFileName);
	
	IAAFFileSP       pFile;
	IAAFHeaderSP     pHeader;
	IAAFObjectSP     pHdrObj;
	IAAFDictionarySP pDict;
	HRESULT          hr;
	int indent=0;
	hr = AAFFileOpenExistingRead (pwFileName, 0, &pFile);
	if (! SUCCEEDED (hr))
	{
	  if (hr==AAFRESULT_NOT_AAF_FILE)
	    {
	      cerr << "File " << name
		   << " is not an AAF file"
		   << endl;
	      return false;
	    }
	  else if (hr==AAFRESULT_FILE_NOT_FOUND)
	    {
	      cerr << "File " << name
		   << " not found"
		   << endl;
	      return false;
	    }
	  else
	    {
	      cerr << "Other error opening file " << name
		   << " returning status 0x"
		   << hex
		   << hr
		   << endl;
	      return false;
	    }
	}
	os << "Dump of file: " << name << endl;

	try
	  {
	    checkResult (pFile->GetHeader (&pHeader));
	    checkResult (pHeader->GetDictionary (&pDict));
	
	    if (dumpFlags.showSummary)
	      {
		hr=dumpSummary(pHeader, pDict, dumpFlags, 0, os);
	      }
	    if (dumpFlags.showDump)
	      {
		checkResult(dumpHeaderObject(pHeader, pDict, dumpFlags, 0, os));
	      }
	    if (dumpFlags.showDict)
	      {
		// Dump dictionary
		IAAFObjectSP pObject;
		indent=0;
		printIndent (indent, os);
		os << endl << "Dictionary: " << endl;
		checkResult(pDict->QueryInterface(IID_IAAFObject,
						  (void**) &pObject));
		checkResult(dumpObject(pObject, pDict, dumpFlags, 0, os));

		// debug
		os << "debug code" << endl;
		IEnumAAFCodecDefsSP pCodecDefIter;
		checkResult(pDict->GetCodecDefs( &pCodecDefIter));
		IAAFCodecDefSP pCodecDef;
		while (SUCCEEDED(pCodecDefIter->NextOne(&pCodecDef)))
		  {
		    os << "starting codec def" << endl;
		    IAAFObjectSP pCodecDefObj;
		    checkResult(pCodecDef->QueryInterface(IID_IAAFObject,
							  (void**) &pCodecDefObj));
		    checkResult(dumpObject(pCodecDefObj, pDict, dumpFlags, 0, os));
		    IEnumAAFDataDefsSP pDataDefIter;
		    checkResult(pCodecDef->GetEssenceKinds(&pDataDefIter));
		    IAAFDataDefSP pDataDef;
		    while (SUCCEEDED(pDataDefIter->NextOne(&pDataDef)))
		      {
			os << "starting data def" << endl;
			IAAFObjectSP pDataDefObj;
			checkResult(pDataDef->QueryInterface(IID_IAAFObject,
							     (void**) &pDataDefObj));
			checkResult(dumpObject(pDataDefObj, pDict, dumpFlags,1, os));
			
		      }

		    // end debug
		  }
	      }

	    if (dumpFlags.showMetaDict)
	      {
		// Dump metadictionary
		indent=0;
		printIndent (indent, os);
		os << endl << "MetaDictionary:" << endl;
		checkResult(dumpMetaDictionary(pHeader, pDict, dumpFlags, indent, os));
	      }
	    if (dumpFlags.showDump)
	      {
		// Dump ContentStorage and Mobs
		indent=0;
		printIndent (indent, os);
		checkResult(dumpMobsAndCS(pHeader, pDict, dumpFlags, indent, os));
	      }
	    if (dumpFlags.showEssence)
	      {
		// Dump Essence Data
		indent=0;
		printIndent (indent,os);
		checkResult(dumpEssence(pHeader, pDict, dumpFlags, indent, os));
	      }
	    os << endl << "End of file: " << name << endl;
	  }

	catch (HRESULT &caught)
	  {
	    os << endl << "***Caught hresult 0x" << hex << caught << "***" << endl;
	    hr = caught;
	  }

	if (pFile)
	  {
	    pFile->Close ();
	  }
	return SUCCEEDED (hr) ? true : false;
}




static void usage (const char * progname)
{
	assert (progname);
	
	cerr << "Usage: " << progname << " [option]... <aaf-file-to-dump>" << endl;
	cerr << endl;
	cerr << "Where option is:" << endl;
	cerr << "  [-o <output-filename>    ]      Specifies output filename (default stdout)" << endl;
	cerr << "  [-[no]dict               ]      Displays the dictionary (default=no)" << endl;
	cerr << "  [-[no]meta               ]      Displays the metadictionary (default=no)" << endl;
	cerr << "  [-[no]allheader          ]      Displays all Header properties (default=no)"<< endl;
	cerr << "  [-[no]types              ]      Displays the property types (default=no)" << endl;
	cerr << "  [-[no]SMPTElabels        ]      Displays AUIDs in SMPTE label format (default is no: UUID format" << endl;
	cerr << "  [-[no]summary            ]      Displays a summary before dumping the file (default)" << endl;
	cerr << "  [-[no]dump               ]      Displays dump of the Mobs and Header (default)" << endl;
	cerr << "  [-[no]essence            ]      Displays contents of Essence Data streams (default)" << endl;
	cerr << "  [-identifybyID           ]      Identifies references by unique ID (default is byname)" << endl;
	cerr << "  [-identifybyname         ]      Identifies references by name, when available (default)" << endl;
	cerr << "  [-max nnnn               ]      Displays maximum number bytes for streams (default=79, set to 0 for no maximum)" << endl;
	cerr << "  [-only <classID:>...     ]      Displays properties for only the listed classes" << endl;

	
}


int main(int argc, char* argv[])
{
	bool succeeded;
	ostream * os = 0;
	char * infilename = 0;
	
	ofstream filestream;
	bool file_opened = false;
	dumpFlags_t dumpFlags;
	int comArg;
	
	// Initial values
	dumpFlags.showDict = false;
	dumpFlags.showMetaDict = false;
	dumpFlags.showAllHeaderProps = false;
	dumpFlags.showTypes=false;
	dumpFlags.showSummary=true;
	dumpFlags.showDump=true;
	dumpFlags.showSMPTE=false;
	dumpFlags.showEssence=true;
	dumpFlags.maxCount=79;
	dumpFlags.showOnlyClasses=NULL; 
	dumpFlags.identifybyname=true;
	

	// Process command line args
	for (comArg = 1; comArg < (argc-1); comArg++)
	{
		if (!strcmp("-o", argv[comArg]) && (comArg < (argc-1) ))
		{
			char * outfilename = argv[++comArg];
			filestream.open (outfilename, ios::out);
			if (filestream.rdstate() && ios::failbit)
			{
				cerr << "Output file " << outfilename << " could not be"
					<< " opened for write." << endl;
				usage (argv[0]);
				exit (1);
			}
			os = &filestream;
			file_opened = true;
		} else if (!strcmp("-nodict", argv[comArg]))
		{
			dumpFlags.showDict = false;
		}  else if (!strcmp("-dict", argv[comArg]))
		{
			dumpFlags.showDict = true;
		} else if (!strcmp("-meta", argv[comArg]))
		{
			dumpFlags.showMetaDict = true;
		} else if (!strcmp("-nometa", argv[comArg]))
		{
			dumpFlags.showMetaDict = false;
		}  else if (!strcmp("-noallheader", argv[comArg]))
		{
			dumpFlags.showAllHeaderProps = true;
		} else if (!strcmp("-allheader", argv[comArg]))
		{
			dumpFlags.showAllHeaderProps = true;
		} else if (!strcmp("-types", argv[comArg]))
		{
			dumpFlags.showTypes=true;
		}  else if (!strcmp("-notypes", argv[comArg]))
		{
			dumpFlags.showTypes=false;
		} else if (!strcmp("-SMPTElabels", argv[comArg]))
		{
			dumpFlags.showSMPTE=true;
		} else if (!strcmp("-noSMPTElabels", argv[comArg]))
		{
			dumpFlags.showSMPTE=false;
		} else if (!strcmp("-summary", argv[comArg]))
		{
			dumpFlags.showSummary=true;
		}  else if (!strcmp("-nosummary", argv[comArg]))
		{
			dumpFlags.showSummary=false;
		} else if (!strcmp("-dump", argv[comArg]))
		{
			dumpFlags.showDump=true;
		}  else if (!strcmp("-nodump", argv[comArg]))
		{
			dumpFlags.showDump=false;
		}  else if (!strcmp("-essence", argv[comArg]))
		{
			dumpFlags.showEssence=true;
		}  else if (!strcmp("-noessence", argv[comArg]))
		{
			dumpFlags.showEssence=false;
		}  else if (!strcmp("-identifybyname", argv[comArg]))
		{
			dumpFlags.identifybyname=true;
		} else if (!strcmp("-identifybyID", argv[comArg]))
		{
			dumpFlags.identifybyname=false;
		}  else if (!strcmp("-only", argv[comArg]) && (comArg < (argc-1)))
		{
			dumpFlags.showOnlyClasses = argv[++comArg];
		} else if (!strcmp("-max", argv[comArg]) && (comArg < (argc-1)))
		{
			dumpFlags.maxCount = atoi(argv[++comArg]);
		} else if (!strcmp("-h", argv[comArg]))
		{
		  usage (argv[0]);
		} else
		{
			cerr << "Unprocessed command argument: " << argv[comArg] << endl;
		}
	
	}
	// If flag hasn't set output stream, set it now
	if (!os) os = &cout;

	
	// The last argument should be an AAF file name or -h
	// if there is only the command and no argument, display usage 
	if (argc < 2)
	  {
	    usage (argv[0]);
	    exit (1);
	  } 
	// if only a usage query
	if (!strcmp("-h",argv[argc-1]))
	  {
	    usage (argv[0]);
	    exit (0);
	  }
	// Load the AAF library explicity to catch the common error
	// that the AAF DLL is not in the user's path, otherwise this
	// error looks like an error opening the file.
	//
	HRESULT hr = AAFLoad(0);
	if (!AAFRESULT_SUCCEEDED(hr))
	  {
		cerr << "Error : Failed to load the AAF library, ";
		cerr << "check environment variables -" << endl;
		cerr << "  Windows    - $PATH" << endl;
		cerr << "  Unix/Linux - $LD_LIBRARY_PATH" << endl;
		exit(hr);
	  }

	// Last argument should be input aaf filename
	infilename = argv[argc-1];
	aafCharacter pwFileName[260];
	assert (infilename);
	assert (os);
	convert(pwFileName, 260, infilename);
	
	succeeded = dumpFile (pwFileName, infilename, dumpFlags, *os);
	
	if (succeeded)
	{
		// clog << endl << "Done." << endl;
		if (file_opened)
			filestream.close ();
		exit (0);
	}
	else
	{
		clog << endl
			<< "Errors were encountered"
		        << endl;
		if (file_opened)
			filestream.close ();
		exit (1);
	}
	
	// Should have exited before now...
	assert (0); // not reached
	return 1;   // to keep compiler happy
}

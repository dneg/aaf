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
#include <assert.h>
#include <iostream.h>
#include <iomanip.h>
#include <fstream.h>
#include <stdlib.h>
#include <stdio.h> // for sprintf()
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


// handy smart pointer typedefs
typedef IAAFSmartPointer<IUnknown>                 IUnknownSP;
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
typedef IAAFSmartPointer<IAAFFile>                 IAAFFileSP;
typedef IAAFSmartPointer<IAAFHeader>               IAAFHeaderSP;
typedef IAAFSmartPointer<IEnumAAFProperties>       IEnumAAFPropertiesSP;
typedef IAAFSmartPointer<IAAFTypeDefCharacter>       IAAFTypeDefCharacterSP;
typedef IAAFSmartPointer<IEnumAAFPropertyValues>   IEnumAAFPropertyValuesSP;
typedef IAAFSmartPointer<IAAFTypeDefSet>           IAAFTypeDefSetSP;


// convenient error handlers.
/*inline*/ static void checkResult(HRESULT r)
{
	if (FAILED(r))
		throw r;
}
/*inline*/ static void checkExpression(bool expression, HRESULT r)
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
	assert((name && *name));
	assert(wcName != 0);
	assert(length > 0);
	
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
	assert((name && *name));
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
	assert((name && *name));
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
	assert((name && *name));
	assert(cName != 0);
	assert(length > 0);
	
	size_t status = wcstombs(cName, name, length);
	if (status == (size_t)-1) {
		cerr << ": Error : Conversion failed.\n\n";
		exit(1); 
	}
}

static char * make_mbstring(size_t length, const aafCharacter* name)
{
	assert(name);
	assert(length > 0);
	
	size_t mbLength = (length * MB_CUR_MAX) + 1;
	char * mbStr = new char[mbLength];
	
	if (*name)
	{
		assert(mbStr);
		convert(mbStr, mbLength, name);
	}
	else
	{
		*mbStr = 0;
	}
	return mbStr;
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
		// I replaced assert(month_index >= 0) 2 lines below
		// with this one because the value is unsigned and will
		// never be negative. - Alex, 11-Sep-00
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


static HRESULT dumpObject
(
 IAAFObjectSP pContainer, // object to be dumped
 IAAFDictionary * pDict,  // dictionary for this file
 int indent,
 ostream & os
 );

HRESULT dumpWeakObject(IUnknown * pContainer,
           IAAFClassDef * pClassDef,
				   IAAFDictionary * pDict,
				   int indent,
				   ostream & os);

static HRESULT dumpPropertyValue
(
 IAAFPropertyValueSP pPVal,
 IAAFDictionary * pDict,  // dictionary for this file
 int indent,
 ostream & os
 );

static HRESULT dumpRawStreamPropertyValue
(
 IAAFPropertyValue * pPVal,
 IAAFTypeDef *pTD,
 IAAFDictionary * pDict,  // dictionary for this file
 int indent,
 ostream & os
 );

static HRESULT dumpPropertyValues
(
 IEnumAAFPropertyValues* pPVEnum,
 IAAFTypeDef *pTD,
 IAAFDictionary * pDict,  // dictionary for this file
 int indent,
 ostream & os
 );


HRESULT dumpObject(IAAFObjectSP pContainer,
				   IAAFDictionary * pDict,
				   int indent,
				   ostream & os)
{
	HRESULT returnHr = AAFRESULT_SUCCESS;
	
	IEnumAAFPropertiesSP pPropEnum;
	IAAFPropertySP pProp;
	IAAFPropertyDefSP pPDef;
	IAAFPropertyValueSP pPVal;
	IAAFClassDefSP pClassDef;
	
	try
	{
		// Get the contained properties.
		checkResult(pContainer->GetProperties (&pPropEnum));
		
		os << endl;
		printIndent (indent, os);
		os << "Object of Class: ";
		checkResult(pContainer->GetDefinition (&pClassDef));
		aafUInt32 bufClassNameSize;
		checkResult(pClassDef->GetNameBufLen (&bufClassNameSize));
		aafCharacter * classNameBuf = new aafCharacter[bufClassNameSize];
		assert (classNameBuf);
		checkResult(pClassDef->GetName(classNameBuf, bufClassNameSize));
		char *mbBuf = make_mbstring(bufClassNameSize, classNameBuf); // create an ansi/asci
		checkExpression(NULL != mbBuf, AAFRESULT_NOMEMORY);
		os << mbBuf << endl;
		delete[] mbBuf;
		delete[] classNameBuf;
		
		// Enumerate across Properties
		while (AAFRESULT_SUCCEEDED (pPropEnum->NextOne (&pProp)))
		{
			// Get property def
			checkResult(pProp->GetDefinition(&pPDef));
			
			// Here we print out property's name using pPDef->GetName()
			aafUInt32 bufSize;
			checkResult(pPDef->GetNameBufLen (&bufSize));
			aafCharacter * nameBuf = new aafCharacter[bufSize];
			assert (nameBuf);
			checkResult(pPDef->GetName(nameBuf, bufSize));
			char *mbBuf = make_mbstring(bufSize, nameBuf); // create an ansi/asci
			checkExpression(NULL != mbBuf, AAFRESULT_NOMEMORY);
			printIndent (indent, os);
			os << "Prop: "<< mbBuf << "; ";
			delete[] mbBuf;
			delete[] nameBuf;
			
			// Get the PropertyValue
			checkResult(pProp->GetValue(&pPVal));
			
			// dump property value
			checkResult (dumpPropertyValue (pPVal, pDict, indent+1, os));
			
		} // while ()...
	}
	catch (HRESULT &caught)
	{
		os << endl << "***Caught hresult 0x" << hex << caught << "***" << endl;
		returnHr = caught;
	}
	
	return returnHr;
}


HRESULT dumpWeakObject(IUnknown * pContainer,
           IAAFClassDef * pClassDef,
           IAAFDictionary * pDict,
           int indent,
           ostream & os)
{
  HRESULT returnHr = AAFRESULT_SUCCESS;

  IAAFObjectSP pObject;
  IAAFMetaDefinitionSP pMetaDefinition;
  IAAFPropertyDefSP pPDef;
  IAAFPropertyValueSP pPVal;
  char *mbBuf = NULL;
  aafCharacter * classNameBuf = NULL;
  
  try
  {  
    //os << endl;
    printIndent (indent, os);
    os << "Weak Reference to Class: ";

    aafUInt32 bufClassNameSize;
    checkResult(pClassDef->GetNameBufLen (&bufClassNameSize));
    classNameBuf = new aafCharacter[bufClassNameSize];
    assert (classNameBuf);
    checkResult(pClassDef->GetName(classNameBuf, bufClassNameSize));
    mbBuf = make_mbstring(bufClassNameSize, classNameBuf); // create an ansi/asci
    checkExpression(NULL != mbBuf, AAFRESULT_NOMEMORY);
    os << mbBuf << "; ";
    delete[] mbBuf;
    mbBuf = NULL; // clear for safe error handling...
    delete[] classNameBuf;
    classNameBuf = NULL; // clear for safe error handling...

    if (pContainer)
    {
      if (SUCCEEDED(pContainer->QueryInterface(IID_IAAFMetaDefinition, (void **)&pMetaDefinition)))
      {
        // Create a property value for the definition's auid
        // so that it can be "dumped" the same as the identifier
        // for weak references to objects.
        aafUID_t auid;
        IAAFTypeDefSP pType;
        IAAFTypeDefRecordSP pRecordType;

        checkResult(pMetaDefinition->GetAUID(&auid));
        checkResult(pDict->LookupTypeDef(kAAFTypeID_AUID, &pType));
        checkResult(pType->QueryInterface(IID_IAAFTypeDefRecord, (void **)&pRecordType));
        checkResult(pRecordType->CreateValueFromStruct((aafMemPtr_t)&auid, sizeof(auid), &pPVal));
      }
      else if (SUCCEEDED(pContainer->QueryInterface(IID_IAAFObject, (void **)&pObject)))
      {
        checkResult(pClassDef->GetUniqueIdentifier(&pPDef));
        checkResult(pObject->GetPropertyValue(pPDef, &pPVal));
      }
      else
      {
        os << "*** ERROR: Invalid weak reference! ***" << endl;
        checkResult(AAFRESULT_INVALID_OBJ);
      }

      // Dump the unique identifier (to use as a cross reference).
      //printIndent (indent, os);
      os << "Unique Identifier; ";
      checkResult (dumpPropertyValue (pPVal, pDict, indent+1, os));    
    }
  }
  catch (HRESULT &caught)
  {
    os << endl << "*** dumpWeakObject: Caught hresult 0x" << hex << caught << "***" << endl;
    returnHr = caught;

    delete[] mbBuf;
    delete[] classNameBuf;
  }
  
  return returnHr;
}


HRESULT dumpPropertyValue (IAAFPropertyValueSP pPVal,
						   IAAFDictionary * pDict,
						   int indent,
						   ostream & os)
{
	// get the type of the data value
	IAAFTypeDefSP pTD;
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
	
	else
	{
		{
			aafCharacter bigBuf[100];
			char mbBigBuf[100 * 3 /*MB_CUR_MAX */];
			IAAFMetaDefinitionSP pd;
			checkResult(pTD->QueryInterface(IID_IAAFMetaDefinition, (void**)&pd));
			checkResult(pd->GetName (bigBuf, sizeof (bigBuf)));
			convert(mbBigBuf, sizeof(mbBigBuf), bigBuf);
			os << "type: " << mbBigBuf << "; ";
		}
		
		// get the type category of the data value
		eAAFTypeCategory_t tid;
		checkResult(pTD->GetTypeCategory (&tid));
		
		switch (tid)
		{
		case kAAFTypeCatInt:
			{
				// integral type; get the specific type def int interface
				IAAFTypeDefIntSP pTDI;
				
				checkResult(pTD->QueryInterface(IID_IAAFTypeDefInt,
					(void**)&pTDI));
				
				aafInt64 val;
				checkResult(pTDI->GetInteger(pPVal, (aafMemPtr_t) &val, sizeof (val)));
				
				os << "value: ";
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
				os << "." << endl;
				
				break;
			}
			
		case kAAFTypeCatCharacter:
			{
				// aafCharacter  type; 
				IAAFTypeDefCharacterSP pTDChar;
				
				checkResult(pTD->QueryInterface(IID_IAAFTypeDefCharacter,
					(void**)&pTDChar));
				
				aafCharacter val;
				checkResult(pTDChar->GetCharacter(pPVal, &val));
				
				os << "value: 0x" << hex << val << endl;
				
				break;
			}
			
		case kAAFTypeCatStrongObjRef:
			{
				// strong object reference; recursively dump contents.
				IAAFTypeDefObjectRefSP pTDO;
				AAFRESULT hr = pTD->QueryInterface(IID_IAAFTypeDefObjectRef,
					(void**)&pTDO);
				checkResult(hr);
				
				IAAFObjectSP pObj;
				checkResult(pTDO->GetObject(pPVal, IID_IAAFObject, (IUnknown **)&pObj));
				os << "Value: an object:" << endl;
				checkResult (dumpObject (pObj, pDict, indent+1, os));
				
				break;	
			}
			
		case kAAFTypeCatWeakObjRef:
			{
				// weak object reference; only dump summary info (not
				// recursively)
				IAAFTypeDefObjectRefSP pTDO;
				checkResult(pTD->QueryInterface(IID_IAAFTypeDefObjectRef,
					(void**)&pTDO));

				os << "Value: weak reference to object:" << endl;
				IAAFClassDefSP pClassDef;
				checkResult(pTDO->GetObjectType(&pClassDef));
				
				IUnknownSP pUnk;
				HRESULT wkResult = pTDO->GetObject(pPVal, IID_IUnknown, (IUnknown **)&pUnk);
				if (AAFRESULT_NULLOBJECT == wkResult)
				{
					// This should be an error but the toolkit does NOT enforce this requirement!
					os << "*** WARNING: Weak reference is NULL! ***" << endl;
					wkResult = S_OK;
				}
				checkResult(wkResult);
				
				// Here is where you print the class def's name, etc. and any
				// other summary info
				checkResult (dumpWeakObject (pUnk, pClassDef, pDict, indent+1, os));
				break;	
			}
			
		case kAAFTypeCatRename:
			{
				// A renamed type; print this type's name, then dump contents
				// as source type
				IAAFTypeDefRenameSP pTDR;
				checkResult(pTD->QueryInterface(IID_IAAFTypeDefRename,
					(void**)&pTDR));
				// Here print rename's type using pTDR->GetName()
				
				// Now get base property value and recursively print that
				IAAFPropertyValueSP pBasePropVal;
				checkResult(pTDR->GetBaseValue (pPVal, &pBasePropVal));
				checkResult (dumpPropertyValue (pBasePropVal,
					pDict,
					indent+1,
					os));
				
				break;
			}
			
		case kAAFTypeCatEnum:
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
				char *mbBuf = make_mbstring(nameBufLen, nameBuf); // create an ansi/asci
				checkExpression(NULL != mbBuf, AAFRESULT_NOMEMORY);
				os << "Value: " << mbBuf << endl;
				delete[] mbBuf;
				mbBuf = 0;
				delete[] nameBuf;
				nameBuf = 0;
				
				break;
			}
			
		case kAAFTypeCatExtEnum:
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
				char *mbBuf = make_mbstring(nameBufLen, nameBuf); // create an ansi/asci
				checkExpression(NULL != mbBuf, AAFRESULT_NOMEMORY);
				os << "Value: " << mbBuf << endl;
				delete[] mbBuf;
				mbBuf = 0;
				delete[] nameBuf;
				nameBuf = 0;
				
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
				
				os << "Value: fixed-sized array[" << numElems << "]:" << endl;
				
				aafUInt32 i;
				for (i = 0; i < numElems; i++)
				{
					printIndent (indent, os);
					os << "  [" << i << "]: ";
					IAAFPropertyValueSP pElemPropVal;
					checkResult(pTDFA->GetElementValue(pPVal, i, &pElemPropVal));
					checkResult (dumpPropertyValue (pElemPropVal,
						pDict,
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
				aafUInt32 numElems;
				checkResult(pTDVA->GetCount(pPVal, &numElems));
				
				os << "Value: variably-sized array[" << numElems << "]:" << endl;

				aafUInt32 i;
				for (i = 0; i < numElems; i++)
				{
					printIndent (indent, os);
					os << "[" << i << "]: ";
					IAAFPropertyValueSP pElemPropVal;
					checkResult(pTDVA->GetElementValue(pPVal, i, &pElemPropVal));
					checkResult (dumpPropertyValue (pElemPropVal,
						pDict,
						indent+1,
						os));
				}
				break;
			}
			
		case kAAFTypeCatSet:
			{
				// Print out elements of array.
				IAAFTypeDefSetSP pTDSet;
				checkResult(pTD->QueryInterface(IID_IAAFTypeDefSet, (void**)&pTDSet));
				
				// Get number of elements
				aafUInt32 numElems;
				checkResult(pTDSet->GetCount(pPVal, &numElems));
				
				os << "Value: set[" << numElems << "]:" << endl;

				IEnumAAFPropertyValuesSP pPVEnum;
				checkResult(pTDSet->GetElements(pPVal, &pPVEnum));
				checkResult(dumpPropertyValues(pPVEnum, pTD, pDict, indent, os));
				break;
			}
			
		case kAAFTypeCatString:
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
				
				//ELEM type should be EITHER :  INT or CHAR
				
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
					char *mbBuf = make_mbstring(bufSize, (aafCharacter*) buf);
					checkExpression(NULL != mbBuf, AAFRESULT_NOMEMORY);
					os << " value: \"" << mbBuf << "\"" << endl;
					delete [] mbBuf;
					mbBuf = 0;
					
					assert (buf);     delete[] buf;       buf = 0;
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
					// it.  First, let's actually get the bits.
					
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
						os << " value: \"" << buf << "\"" << endl;
					}
					else if (2 == elemSize)
					{
						// 2-byte integral characters; interpret as a
						// NULL-terminated wide character string.
						// create an ansi/asci
						char *mbBuf = make_mbstring(bufSize, (aafCharacter*) buf);
						checkExpression(NULL != mbBuf, AAFRESULT_NOMEMORY);
						os << " value: \"" << mbBuf << "\"" << endl;
						delete [] mbBuf;
						mbBuf = 0;
					}
					else
					{
						// elements are too wide to be C or unicode strings;
						// perhaps just hex dump them, looking for terminator
						// character.
						
						// But for now, we won't bother dumping them... ;)
						assert (0);
					}
					
					assert (buf);     delete[] buf;       buf = 0;
					break;
				}//if INT
				else
				{
					// Hmmmm ... Elem Type is NEITHER a  Character NOR an INT ...
					//  ....  flag this as a problem ?! 
					assert (0);
				}
				
		  }//kAAFTypeCatString
		  
		case kAAFTypeCatRecord:
			{
				aafUInt32 i;
				IAAFTypeDefRecordSP pTDR;
				checkResult(pTD->QueryInterface(IID_IAAFTypeDefRecord,
					(void**)&pTDR));
				
				// Special case!  See if this is an AUID
				IUnknown * pUnkTest = 0;
				checkResult(pTDR->QueryInterface(IID_IUnknown,
					(void**)&pUnkTest));
				
				IAAFTypeDefSP ptd;
				assert (pDict);
				checkResult (pDict->LookupTypeDef (kAAFTypeID_AUID, &ptd));
				IUnknown * pUnkAUID = 0;
				checkResult(ptd->QueryInterface(IID_IUnknown,
					(void**)&pUnkAUID));
				assert (pDict);
				checkResult (pDict->LookupTypeDef (kAAFTypeID_TimeStamp, &ptd));
				IUnknown * pUnkTimeStamp = 0;
				checkResult(ptd->QueryInterface(IID_IUnknown,
					(void**)&pUnkTimeStamp));
				
				if (pUnkTest == pUnkAUID)
				{
					// dump it as an auid
					aafUID_t auidVal;
					checkResult (pTDR->GetStruct (pPVal,
						(aafMemPtr_t) &auidVal,
						sizeof (auidVal)));
					char buf[100];
					sprintf (buf, "{ 0x%08x, 0x%04x, 0x%04x, ",
						auidVal.Data1, auidVal.Data2, auidVal.Data3);
					
					os << "Value: " << buf << "{ ";
					for (i = 0; i < 8; i++)
					{
						sprintf (buf, "0x%x", auidVal.Data4[i]);
						os << buf;
						if (i != 7)
							os << ", ";
					}
					os << " } }" << endl;
				}
				
				else if (pUnkTest == pUnkTimeStamp)
				{
					// dump it as an aafTimeStamp_t
					aafTimeStamp_t ts;
					checkResult (pTDR->GetStruct (pPVal,
						(aafMemPtr_t) &ts,
						sizeof (ts)));
					
					os << "Value: ";
					printTimeStamp (ts, os);
					os << endl;
				}
				
				else
				{
					// dump it as a plain record
					// get the number of members in this record
					aafUInt32 numMembers = 0;
					checkResult(pTDR->GetCount(&numMembers));
					
					os << "Value: record of " << numMembers << " members:" << endl;
					
					// recursively dump each struct field
					for (i = 0; i < numMembers; i++)
					{
						aafUInt32 nameBufLen;
						checkResult(pTDR->GetMemberNameBufLen(i, &nameBufLen));
						// nameBufLen is in bytes, not aafCharacters
						aafCharacter * nameBuf = (aafCharacter*) new aafUInt8 [nameBufLen];
						assert (nameBuf);
						checkResult(pTDR->GetMemberName(i, nameBuf, nameBufLen));
						// create an ansi/asci
						char *mbBuf = make_mbstring(nameBufLen, nameBuf);
						checkExpression(NULL != mbBuf, AAFRESULT_NOMEMORY);
						printIndent (indent, os);
						os << "  [" << mbBuf << "]: ";
						delete[] nameBuf;
						delete[] mbBuf;
						
						IAAFPropertyValueSP pMemberPropVal;
						checkResult(pTDR->GetValue(pPVal, i, &pMemberPropVal));
						// recursively dump prop value
						checkResult (dumpPropertyValue (pMemberPropVal,
							pDict,
							indent+1,
							os));
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
				break;
		  }
		  
		case kAAFTypeCatIndirect:
			{
				// Print out the actual value of the indirect type.
				IAAFTypeDefIndirectSP pIndirectType;
				checkResult(pTD->QueryInterface(IID_IAAFTypeDefIndirect,
					(void**)&pIndirectType));
				
				os << "Value [indirect]:" << endl;
				
				// Get the actual value
				IAAFPropertyValueSP pActualValue;
				checkResult(pIndirectType->GetActualValue(pPVal, &pActualValue));
				// recursively dump prop value
				checkResult (dumpPropertyValue (pActualValue,
					pDict,
					indent+1,
					os));
				break;
			}
			
		case kAAFTypeCatOpaque:
			{
				// Print out the actual value of the opaque type.
				IAAFTypeDefOpaqueSP pOpaqueType;
				checkResult(pTD->QueryInterface(IID_IAAFTypeDefOpaque,
					(void**)&pOpaqueType));
				
				os << "Value [opaque]:" << endl;
				
				// Get the opaque type id
				aafUID_t opaqueTypeID = {0};
				checkResult(pOpaqueType->GetActualTypeID(pPVal, &opaqueTypeID));
				
				// If not then register a corresponding opaque type in the
				// dictionary that is a rename of UInt8Array.      
				IAAFTypeDefSP pType;
				if (FAILED(pDict->LookupTypeDef(opaqueTypeID, &pType)))
				{
					IAAFTypeDefSP pBaseType;
					IAAFClassDefSP pRenamedClassDef;
					IAAFTypeDefRenameSP pRenamedType;
					
					checkResult(pDict->LookupTypeDef(kAAFTypeID_UInt8Array, &pBaseType));
					checkResult(pDict->CreateMetaInstance(AUID_AAFTypeDefRename, IID_IAAFTypeDefRename, (IUnknown **)&pRenamedType));
					checkResult(pRenamedType->Initialize (opaqueTypeID, pBaseType, L"Opaque Data"));
					checkResult(pRenamedType->QueryInterface(IID_IAAFTypeDef, (void **)&pType));
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
				// recursively dump prop value
				checkResult (dumpPropertyValue (pActualValue,
					pDict,
					indent+1,
					os));
				break;
			}
			
		case kAAFTypeCatStream:
			{
				// Print out the actual value of the stream type.
				checkResult(dumpRawStreamPropertyValue(pPVal, pTD, pDict, indent, os));
				break;
			}
			
		default:
			os << "***Unknown type category " << dec << (int) tid
				<< ".***" << endl;
			return E_FAIL;
			break;
		}
		return AAFRESULT_SUCCESS;
	} // !pTD
}

HRESULT dumpRawStreamPropertyValue
(
 IAAFPropertyValue * pPVal,
 IAAFTypeDef *pTD,
 IAAFDictionary * /*pDict*/,  // dictionary for this file
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
	IAAFTypeDefStreamSP pStreamType;
	checkResult(pTD->QueryInterface(IID_IAAFTypeDefStream,
		(void**)&pStreamType));
	
	// Get number of bytes
	aafInt64 streamSize = 0;
	checkResult(pStreamType->GetSize(pPVal, &streamSize));
	
	os << "Value: stream: " << endl;
	if (0x00000000FFFFFFFF < streamSize)
	{
		os << "  Large (64-bit) stream not supported in ascii dump!" << endl;
		os << "  WARNING: Stream will be truncated!" << endl;
	}
	
	aafUInt32 bytesLeft = (aafUInt32)(streamSize);
	const aafUInt32 kStreamBytesPerLine = 16;
	const aafUInt32 kStreamReadSize = kStreamBytesPerLine;
	const aafUInt32 kOffsetWidth = 8;
	aafUInt8 streamBuffer[kStreamReadSize];
	aafUInt32 bytes = 0;
	aafUInt32 bytesRead = 0;
	aafUInt32 offset = 0;
	
    
	os << "{/* size: " << bytesLeft << " bytes */" << endl;
	
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
	
	return streamResult;
}



HRESULT dumpPropertyValues
(
 IEnumAAFPropertyValues* pPVEnum,
 IAAFTypeDef * /*pTD*/,
 IAAFDictionary * pDict,  // dictionary for this file
 int indent,
 ostream & os
 )
{
	HRESULT result = S_OK;
	aafUInt32 i = 0;
	IAAFPropertyValueSP pElemPropVal;
	while (SUCCEEDED(pPVEnum->NextOne(&pElemPropVal)))
	{
		printIndent (indent, os);
		os << "[" << i << "]: ";
		result = dumpPropertyValue (pElemPropVal,
			pDict,
			indent+1,
			os);
		++i;
	}
	
	return result;
}


//
// Dumps the given file.  Returns true if successful; returns false if
// an error was encountered.
//
static bool dumpFile (aafCharacter * pwFileName,
					  const char * name,
					  ostream & os)
{
	assert (pwFileName);
	
	IAAFFileSP       pFile;
	IAAFHeaderSP     pHeader;
	IAAFObjectSP     pHdrObj;
	IAAFDictionarySP pDict;
	HRESULT          hr;
	
	hr = AAFFileOpenExistingRead (pwFileName, 0, &pFile);
	if (! SUCCEEDED (hr))
	{
		cerr << "File " << name
			<< " is either missing or is an illegal AAF file."
			<< endl;
		return false;
	}
	checkResult (pFile->GetHeader (&pHeader));
	checkResult (pHeader->GetDictionary (&pDict));
	checkResult (pHeader->QueryInterface (IID_IAAFObject,
		(void**)&pHdrObj));
	
	hr = dumpObject (pHdrObj, pDict, 0, os);
	if (pFile)
	{
		pFile->Close ();
	}
	return SUCCEEDED (hr) ? true : false;
}


// simple helper class to initialize and cleanup AAF library.
struct CAAFInitialize
{
	CAAFInitialize(const char *dllname = NULL)
	{
		// clog << "Attempting to load the AAF dll..." << endl;
		HRESULT hr = AAFLoad(dllname);
		// clog << ((SUCCEEDED(hr)) ? "DONE" : "FAILED") << endl;
	}
	
	~CAAFInitialize()
	{
		AAFUnload();
	}
};


static void usage (const char * progname)
{
	assert (progname);
	
	cerr << "Usage: " << progname << " <aaf-file-to-dump>" << endl;
	cerr << "       " << progname << " -o <output-text-file>"
		<< " <aaf-file-to-dump>" << endl;
}


int main(int argc, char* argv[])
{
	bool succeeded;
	ostream * os = 0;
	char * infilename = 0;
	
	ofstream filestream;
	bool file_opened = false;
	
	CAAFInitialize aafInit;
	
	// If only two args are (correctly) given:
	// argv[0] is this program's name
	// argv[1] is the name of the AAF file to dump
	if (argc == 2)
	{
		os = &cout;
		infilename = argv[1];
	}
	
	// If four args are (correctly) given:
	// argv[0] is this program's name
	// argv[1] is the string "-o"
	// argv[2] is the desired name of the output text file
	// argv[3] is the name of the AAF file to dump
	else if (argc == 4)
	{
		infilename = argv[3];
		if ( strcmp ("-o", argv[1]))
		{
			// If more than 2 args given, first must be "-o" flag
			usage (argv[0]);
			exit (1);
		}
		char * outfilename = argv[2];
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
	}
	
	else
	{
		usage (argv[0]);
		exit (1);
	}
	
	// The first argument should be an AAF file name.
	aafCharacter pwFileName[260];
	assert (infilename);
	assert (os);
	convert(pwFileName, 260, infilename);
	
	// clog << "***Dumping file "
	// << infilename
	// << " using direct prop access***)" << endl;
	succeeded = dumpFile (pwFileName, infilename, *os);
	
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
			<< "Errors were encountered while dumping "
			<< infilename
			<< "." << endl;
		if (file_opened)
			filestream.close ();
		exit (1);
	}
	
	// Should have exited before now...
	assert (0); // not reached
	return 1;   // to keep compiler happy
}

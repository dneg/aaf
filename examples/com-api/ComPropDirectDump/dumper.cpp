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

#if defined(macintosh)
  // Make sure we have defined IID_IUnknown and IID_IClassFactory.
  #include <initguid.h>
  #include <coguid.h>	
#endif



#include "AAF.h"
#include "AAFResult.h"
#include "AAFTypeDefUIDs.h"
#include <assert.h>
#include <iostream.h>
#include <fstream.h>
#include <stdio.h> // for sprintf()

#ifndef __AAFSmartPointer_h__
#include "AAFSmartPointer.h"
#endif

#if defined(macintosh) || defined(_MAC)
#include <console.h> /* Mac command line window */
#endif

// handy smart pointer typedefs
typedef IAAFSmartPointer<IAAFObject>               IAAFObjectSP;
typedef IAAFSmartPointer<IAAFPropertyValue>        IAAFPropertyValueSP;
typedef IAAFSmartPointer<IAAFProperty>             IAAFPropertySP;
typedef IAAFSmartPointer<IAAFPropertyDef>          IAAFPropertyDefSP;
typedef IAAFSmartPointer<IAAFTypeDef>              IAAFTypeDefSP;
typedef IAAFSmartPointer<IAAFDefObject>            IAAFDefObjectSP;
typedef IAAFSmartPointer<IAAFDictionary>           IAAFDictionarySP;
typedef IAAFSmartPointer<IAAFTypeDefIndirect>      IAAFTypeDefIndirectSP;
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
typedef IAAFSmartPointer<IAAFFile>                 IAAFFileSP;
typedef IAAFSmartPointer<IAAFHeader>               IAAFHeaderSP;
typedef IAAFSmartPointer<IEnumAAFProperties>       IEnumAAFPropertiesSP;


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
    while (*wName++ = *name++)
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
	  aafUInt8 month_index = ts.date.month-1;
	  assert (month_index >= 0);
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

static HRESULT dumpPropertyValue
(
 IAAFPropertyValueSP pPVal,
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
		  dumpPropertyValue (pPVal, pDict, indent+1, os);

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
		IAAFDefObjectSP pd;
		checkResult(pTD->QueryInterface(IID_IAAFDefObject, (void**)&pd));
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
			aafInt32 hi = (aafUInt32) ((val & 0xffffffff00000000) >> 32);
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

		case kAAFTypeCatStrongObjRef:
		  {
			// strong object reference; recursively dump contents.
			IAAFTypeDefObjectRefSP pTDO;
			AAFRESULT hr = pTD->QueryInterface(IID_IAAFTypeDefObjectRef,
											   (void**)&pTDO);
			checkResult(hr);

			IAAFObjectSP pObj;
			checkResult(pTDO->GetObject(pPVal, &pObj));
			os << "Value: an object:" << endl;
			dumpObject (pObj, pDict, indent+1, os);

			break;	
		  }

#if 0
		case kAAFTypeCatWeakObjRef:
		  {
			// weak object reference; only dump summary info (not
			// recursively)
			IAAFTypeDefObjectRefSP pTDO;
			checkResult(pTD->QueryInterface(IID_IAAFTypeDefObjectRef,
										   (void**)&pTDO));

			IAAFObjectSP pObj;
			checkResult(pTDO->GetObject(pPVal, &pObj));
			IAAFClassDefSP pClassDef;
			checkResult(pObj->GetDefinition(&pClassDef));

			// Here is where you print the class def's name, etc. and any
			// other summary info

			break;	
		  }
#endif // 0

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
			checkResult(pTDR->GetValue (pPVal, &pBasePropVal));
			dumpPropertyValue (pBasePropVal, pDict, indent+1, os);

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
				dumpPropertyValue (pElemPropVal, pDict, indent+1, os);
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
				dumpPropertyValue (pElemPropVal, pDict, indent+1, os);
			  }

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

			// Strings *must* be made up of integral types.
			IAAFTypeDefIntSP pETDInt;
			checkResult(pETD->QueryInterface(IID_IAAFTypeDefInt,
											 (void**) &pETDInt));

			// get the type category of the element
			eAAFTypeCategory_t elemTID;
			checkResult(pETD->GetTypeCategory (&elemTID));
			assert (kAAFTypeCatInt == elemTID);

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
		  }

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
					dumpPropertyValue (pMemberPropVal, pDict, indent+1, os);
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
			// Print out elements of array.
			IAAFTypeDefIndirectSP pIndirectType;
			checkResult(pTD->QueryInterface(IID_IAAFTypeDefIndirect,
										   (void**)&pIndirectType));

			os << "Value [indirect]:" << endl;

			// Get the actual value
      IAAFPropertyValueSP pActualValue;
			checkResult(pIndirectType->GetActualValue(pPVal, &pActualValue));
			// recursively dump prop value
			dumpPropertyValue (pActualValue, pDict, indent+1, os);

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


struct CComInitialize
{
  CComInitialize()
  {
    CoInitialize(NULL);
  }

  ~CComInitialize()
  {
    CoUninitialize();
  }
};

// simple helper class to initialize and cleanup AAF library.
struct CAAFInitialize
{
  CAAFInitialize(const char *dllname = NULL)
  {
  	clog << "Attempting to load the AAF dll..." << endl;
    HRESULT hr = AAFLoad(dllname);
    clog << ((SUCCEEDED(hr)) ? "DONE" : "FAILED") << endl;
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

	/* console window for mac */

	#if defined(macintosh) || defined(_MAC)
	argc = ccommand(&argv);
	#endif

  CComInitialize comInit;
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
  
  clog << "***Dumping file "
	   << infilename
	   << " using direct prop access***)" << endl;
  succeeded = dumpFile (pwFileName, infilename, *os);

  if (succeeded)
	{
	  clog << endl << "Done." << endl;
	  if (file_opened)
		filestream.close ();
	  exit (0);
	}
  else
	{
	  clog << endl << "Errors were encountered." << endl;
	  if (file_opened)
		filestream.close ();
	  exit (1);
	}
  // Should have exited before now...
  assert (0); // not reached
  return 1;   // to keep compiler happy
}

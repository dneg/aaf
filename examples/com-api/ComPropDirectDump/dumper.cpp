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
typedef IAAFSmartPointer<IAAFTypeDefInt>           IAAFTypeDefIntSP;
typedef IAAFSmartPointer<IAAFTypeDefObjectRef>     IAAFTypeDefObjectRefSP;
typedef IAAFSmartPointer<IAAFClassDef>             IAAFClassDefSP;
typedef IAAFSmartPointer<IAAFTypeDefRename>        IAAFTypeDefRenameSP;
typedef IAAFSmartPointer<IAAFTypeDefEnum>          IAAFTypeDefEnumSP;
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


static void printIndent (int indent)
{
  while (indent--) cout << " ";
}


static void convert(wchar_t* wcName, size_t length, const char* name)
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

static void convert(wchar_t* wName, size_t length, const wchar_t* name)
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

static void convert(char* cName, size_t length, const wchar_t* name)
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

static char * make_mbstring(size_t length, const wchar_t* name)
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

static HRESULT dumpObject
(
 IAAFObjectSP pContainer, // object to be dumped
 IAAFDictionary * pDict,  // dictionary for this file
 int indent
);

static HRESULT dumpPropertyValue
(
 IAAFPropertyValueSP pPVal,
 IAAFDictionary * pDict,  // dictionary for this file
 int indent
);


HRESULT dumpObject(IAAFObjectSP pContainer,
				   IAAFDictionary * pDict,
				   int indent)
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

	  cout << endl;
	  printIndent (indent);
	  cout << "Object of Class: ";
	  checkResult(pContainer->GetDefinition (&pClassDef));
      aafUInt32 bufClassNameSize;
		  checkResult(pClassDef->GetNameBufLen (&bufClassNameSize));
		  wchar_t * classNameBuf = new wchar_t[bufClassNameSize];
		  assert (classNameBuf);
		  checkResult(pClassDef->GetName(classNameBuf, bufClassNameSize));
		  char *mbBuf = make_mbstring(bufClassNameSize, classNameBuf); // create an ansi/asci
		  checkExpression(NULL != mbBuf, AAFRESULT_NOMEMORY);
		  cout << mbBuf << endl;
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
		  wchar_t * nameBuf = new wchar_t[bufSize];
		  assert (nameBuf);
		  checkResult(pPDef->GetName(nameBuf, bufSize));
		  char *mbBuf = make_mbstring(bufSize, nameBuf); // create an ansi/asci
		  checkExpression(NULL != mbBuf, AAFRESULT_NOMEMORY);
		  printIndent (indent);
		  cout << "Prop: "<< mbBuf << "; ";
		  delete[] mbBuf;
		  delete[] nameBuf;

		  // Get the PropertyValue
		  checkResult(pProp->GetValue(&pPVal));

		  // dump property value
		  dumpPropertyValue (pPVal, pDict, indent+1);

		} // while ()...
	}
  catch (HRESULT &caught)
	{
	  cout << endl << "***Caught hresult 0x" << hex << caught << "***" << endl;
	  returnHr = caught;
	}

  return returnHr;
}


HRESULT dumpPropertyValue (IAAFPropertyValueSP pPVal,
						   IAAFDictionary * pDict,
						   int indent)
{
  // get the type of the data value
  IAAFTypeDefSP pTD;
  checkResult(pPVal->GetType(&pTD));

  if (! pTD)
	{
	  // unknown type, due to damaged file.

	  // Dump "damaged object" message.
	  // Optionally, dump the property's bits using pPVal->GetBits().
	  printIndent (indent);
	  cout << "Unknown type def." << endl;
	}

  else
	{
	  {
		wchar_t bigBuf[100];
		char mbBigBuf[100 * 3 /*MB_CUR_MAX */];
		IAAFDefObjectSP pd;
		checkResult(pTD->QueryInterface(IID_IAAFDefObject, (void**)&pd));
		checkResult(pd->GetName (bigBuf, sizeof (bigBuf)));
		convert(mbBigBuf, sizeof(mbBigBuf), bigBuf);
		cout << "type: " << mbBigBuf << "; ";
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

			cout << "value: ";
			aafInt32 hi = (aafUInt32) ((val & 0xffffffff00000000) >> 32);
			aafInt32 lo = (aafInt32) val & 0xffffffff;
			if (hi && ((hi != ~0) || (lo >= 0)))
			  {
				int width = cout.width();
				char fill = cout.fill();
				cout << hex << "0x";
				cout.width(8);
				cout.fill('0');
				cout << hi << lo;
				cout.width(width);
				cout.fill(fill);
			  }
			else
			  {
				cout << "0x" << hex << lo;
			  }
			cout << "." << endl;

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
			cout << "Value: an object:" << endl;
			dumpObject (pObj, pDict, indent+1);

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
			dumpPropertyValue (pBasePropVal, pDict, indent+1);

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
			wchar_t * nameBuf;
			aafUInt32 nameBufLen;
			checkResult(pTDE->GetNameBufLenFromInteger(enumValue, &nameBufLen));
			// don't forget NameBufLen is in bytes, not wchar_ts
			nameBuf = (wchar_t*) new aafUInt8[nameBufLen];

			// and now get the name itself
			checkResult(pTDE->GetNameFromInteger(enumValue, nameBuf, nameBufLen));

			// Print the contents
			char *mbBuf = make_mbstring(nameBufLen, nameBuf); // create an ansi/asci
			checkExpression(NULL != mbBuf, AAFRESULT_NOMEMORY);
			cout << "Value: " << mbBuf << endl;
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

			cout << "Value: fixed-sized array[" << numElems << "]:" << endl;

			aafUInt32 i;
			for (i = 0; i < numElems; i++)
			  {
				printIndent (indent);
				cout << "  [" << i << "]: ";
				IAAFPropertyValueSP pElemPropVal;
				checkResult(pTDFA->GetElementValue(pPVal, i, &pElemPropVal));
				dumpPropertyValue (pElemPropVal, pDict, indent+1);
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

			cout << "Value: variably-sized array[" << numElems << "]:" << endl;

			aafUInt32 i;
			for (i = 0; i < numElems; i++)
			  {
				printIndent (indent);
				cout << "[" << i << "]: ";
				IAAFPropertyValueSP pElemPropVal;
				checkResult(pTDVA->GetElementValue(pPVal, i, &pElemPropVal));
				dumpPropertyValue (pElemPropVal, pDict, indent+1);
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
			checkResult(pETDInt->GetSize (&elemSize));
			bufSize = count * elemSize;

			// See if this is a type we can easily represent.  We know
			// it is some kind of integral type.  Maybe we can dump
			// it.  First, let's actually get the bits.

			// get bits
			aafUInt8 * buf = new aafUInt8[bufSize];
			checkResult(pTDS->GetElements(pPVal, buf, bufSize));

			// Now determine size of integral elements
			if (1 == elemSize)
			  {
				// 1-byte integer characters; interpret as a
				// NULL-terminated C string.
				printIndent (indent);
				cout << " value: \"" << buf << "\"" << endl;
			  }
			else if (2 == elemSize)
			  {
				// 2-byte integral characters; interpret as a
				// NULL-terminated wide character string.
				// create an ansi/asci
				char *mbBuf = make_mbstring(bufSize, (wchar_t*) buf);
				checkExpression(NULL != mbBuf, AAFRESULT_NOMEMORY);
				cout << " value: \"" << mbBuf << "\"" << endl;
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
			IAAFTypeDefSP ptd;
			IUnknown * pUnkTest;
			checkResult(pTDR->QueryInterface(IID_IUnknown,
											 (void**)&pUnkTest));

			assert (pDict);
			checkResult (pDict->LookupType ((aafUID_t*) &kAAFTypeID_AUID, &ptd));
			IUnknown * pUnkAUID;
			checkResult(ptd->QueryInterface(IID_IUnknown,
											(void**)&pUnkAUID));
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

				cout << "Value: " << buf << "{ ";
				for (i = 0; i < 8; i++)
				  {
					sprintf (buf, "0x%x", auidVal.Data4[i]);
					cout << buf;
					if (i != 7)
					  cout << ", ";
				  }
				cout << " } }" << endl;
			  }

			else
			  {
				// dump it as a plain record
				// get the number of members in this record
				aafUInt32 numMembers = 0;
				checkResult(pTDR->GetCount(&numMembers));

				cout << "Value: record of " << numMembers << " members:" << endl;

				// recursively dump each struct field
				for (i = 0; i < numMembers; i++)
				  {
					aafUInt32 nameBufLen;
					checkResult(pTDR->GetMemberNameBufLen(i, &nameBufLen));
					wchar_t * nameBuf = (wchar_t*) new wchar_t[nameBufLen];
					assert (nameBuf);
					checkResult(pTDR->GetMemberName(i, nameBuf, nameBufLen));
					// create an ansi/asci
					char *mbBuf = make_mbstring(nameBufLen, nameBuf);
					checkExpression(NULL != mbBuf, AAFRESULT_NOMEMORY);
					printIndent (indent);
					cout << "  [" << mbBuf << "]: ";
					delete[] nameBuf;
					delete[] mbBuf;

					IAAFPropertyValueSP pMemberPropVal;
					checkResult(pTDR->GetValue(pPVal, i, &pMemberPropVal));
					// recursively dump prop value
					dumpPropertyValue (pMemberPropVal, pDict, indent+1);
				  }
			  }
			assert (pUnkTest);
			pUnkTest->Release ();
			pUnkTest = 0;
			assert (pUnkAUID);
			pUnkAUID->Release ();
			pUnkAUID = 0;
			break;
		  }

		default:
		  cout << "Unknown type category " << (int) tid << "." << endl;
		  break;
		}
	} // !pTD

  return AAFRESULT_SUCCESS;
}



static void dumpFile (wchar_t * pwFileName)
{
  assert (pwFileName);

  IAAFFileSP       pFile;
  IAAFHeaderSP     pHeader;
  IAAFObjectSP     pHdrObj;
  IAAFDictionarySP pDict;

  checkResult (AAFFileOpenExistingRead (pwFileName, 0, &pFile));
  checkResult (pFile->GetHeader (&pHeader));
  checkResult (pHeader->GetDictionary (&pDict));
  checkResult (pHeader->QueryInterface (IID_IAAFObject,
										(void**)&pHdrObj));

  dumpObject (pHdrObj, pDict, 0);
  if (pFile)
	{
	  pFile->Close ();
	}
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
  	cout << "Attempting to load the AAF dll..." << endl;
    HRESULT hr = AAFLoad(dllname);
    cout << ((SUCCEEDED(hr)) ? "DONE" : "FAILED") << endl;
  }

  ~CAAFInitialize()
  {
    AAFUnload();
  }
};


int main(int argc, char* argv[])
{
	/* console window for mac */

	#if defined(macintosh) || defined(_MAC)
	argc = ccommand(&argv);
	#endif

  CComInitialize comInit;
  CAAFInitialize aafInit;

  if (argc != 2) {
    cerr << "Error : wrong number of arguments" << endl;
    return(1);
  }

  // The first argument should be an AAF file name.
  wchar_t pwFileName[260];
  convert(pwFileName, 260, argv[1]);
  
  cout << "***Dumping file "
	   << argv[1]
	   << " using direct prop access***)" << endl;
  dumpFile (pwFileName);

  cout << "Done" << endl;

  return(0);
}

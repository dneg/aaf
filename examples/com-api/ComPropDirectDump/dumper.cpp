#include "AAF.h"
#include "AAFResult.h"
#include <assert.h>
#include <ostream.h>


#if defined(macintosh) || defined(_MAC)
#include <console.h> /* Mac command line window */
#endif


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
 IAAFObject *pContainer, // object to be dumped
 int indent
);

static HRESULT dumpPropertyValue
(
 IAAFPropertyValue * pPVal,
 int indent
);


HRESULT dumpObject(IAAFObject *pContainer, int indent)
{
  HRESULT returnHr = AAFRESULT_SUCCESS;

  IEnumAAFProperties * pPropEnum = 0;
  IAAFProperty * pProp = 0;
  IAAFPropertyDef *pPDef = 0;
  IAAFPropertyValue *pPVal = 0;
  try
	{
	  // Get the contained properties.
	  checkResult(pContainer->GetProperties (&pPropEnum));

	  cout << endl;
	  printIndent (indent);
	  cout << "***Dumping Object***\n";

	  // Enumerate across Properties
	  while (AAFRESULT_SUCCEEDED (pPropEnum->NextOne (&pProp)))
		{
		  // Get property def
		  checkResult(pProp->GetDefinition(&pPDef));
		  assert (pPDef);

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
		  dumpPropertyValue (pPVal, indent+1);

		  pPVal->Release();
		  pPVal = 0;

		  pPDef->Release ();
		  pPDef = 0;

		} // while ()...
	}
  catch (HRESULT &caught)
	{
	  cout << endl << "***Caught hresult 0x" << hex << caught << "***" << endl;
	  returnHr = caught;
	}

  if (pPropEnum) pPropEnum->Release(); pPropEnum = 0;
  if (pProp) pProp->Release(); pProp = 0;
  if (pPDef) pPDef->Release(); pPDef = 0;
  if (pPVal) pPVal->Release(); pPVal = 0;

  return returnHr;
}


HRESULT dumpPropertyValue (IAAFPropertyValue * pPVal, int indent)
{
  // get the type of the data value
  IAAFTypeDef *pTD = 0;
  checkResult(pPVal->GetType(&pTD));

  if (! pTD)
	{
	  // unknown type, due to damaged file.

	  // Dump "damaged object" message.
	  // Optionally, dump the property's bits using pPVal->GetBits().
	  printIndent (indent);
	  cout << "Unknown type def.\n";
	}

  else
	{
	  {
		wchar_t bigBuf[100];
		char mbBigBuf[100 * 3 /*MB_CUR_MAX */];
		assert (pTD);
		IAAFDefObject * pd = NULL;
		checkResult(pTD->QueryInterface(IID_IAAFDefObject, (void**)&pd));
		assert (pd);
		checkResult(pd->GetName (bigBuf, sizeof (bigBuf)));
		pd->Release ();
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
			IAAFTypeDefInt * pTDI = 0;
			
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
			cout << ".\n";

			pTDI->Release();
			pTDI = 0;
			break;
		  }

		case kAAFTypeCatStrongObjRef:
		  {
			// strong object reference; recursively dump contents.
			IAAFTypeDefObjectRef * pTDO = 0;
			AAFRESULT hr = pTD->QueryInterface(IID_IAAFTypeDefObjectRef,
										   (void**)&pTDO);
			checkResult(hr);

			IAAFObject * pObj = 0;
			checkResult(pTDO->GetObject(pPVal, &pObj));
			cout << "Value: an object:\n";
			dumpObject (pObj, indent+1);

			pObj->Release();
			pObj = 0;

			pTDO->Release();
			pTDO = 0;
			break;	
		  }

#if 0
		case kAAFTypeCatWeakObjRef:
		  {
			// weak object reference; only dump summary info (not
			// recursively)
			IAAFTypeDefObjectRef * pTDO = 0;
			checkResult(pTD->QueryInterface(IID_IAAFTypeDefObjectRef,
										   (void**)&pTDO));

			IAAFObject * pObj = 0;
			checkResult(pTDO->GetObject(pPVal, &pObj));
			IAAFClassDef * pClassDef = 0;
			checkResult(pObj->GetDefinition(&pClassDef));

			// Here is where you print the class def's name, etc. and any
			// other summary info

			pClassDef->Release;
			pClassDef = 0;

			pObj->Release;
			pObj = 0;

			pTDO->Release();
			pTDO = 0;
			break;	
		  }
#endif // 0

#if 0
		case kAAFTypeCatRename:
		  {
			// A renamed type; print this type's name, then dump contents
			// as source type
			IAAFTypeDefRename * pTDR = 0;
			checkResult(pTD->QueryInterface(IID_IAAFTypeDefRename,
										   (void**)&pTDR));
			// Here print rename's type using pTDR->GetName()

			// Now get base property value and recursively print that
			IAAFPropertyValue * pBasePropValue = 0;
			checkResult(pTDR->GetValue (pPVal, &pBasePropVal));
			dumpPropertyValue (pBasePropVal, indent+1);

			pBasePropVal->Release();
			pBasePropVal = 0;

			pTDR->Release();
			pTDR = 0;
			break;
		  }
#endif // 0

#if 0
		case kAAFTypeCatEnum:
		  {
			// Print enum element's text tag as well as int value
			IAAFTypeDefEnum * pTDE = 0;
			checkResult(pTD->QueryInterface(IID_IAAFTypeDefEnum,
										   (void**)&pTDE));

			// first, get the int value
			aafInt32 enumValue;
			checkResult(pTDE->GetValue(pPVal, &enumValue));

			// now, get the text tag for that value.  Start with name
			// buf len, and allocating a buffer to hold the name
			wchar_t * nameBuf;
			aafUInt32 nameBufLen;
			checkResult(pTDE->GetElemNameBufLen(enumValue, &nameBufLen));
			// don't forget NameBufLen is in bytes, not wchar_ts
			nameBuf = (wchar_t*) new aafUInt8[nameBufLen];

			// and now get the name itself
			checkResult(pTDE->GetElemName(elemValue, nameBuf, nameBufLen));

			// Here you print out nameBuf to indicate enum's value

			delete[] nameBuf;
			nameBuf = 0;

			pTDE->Release();
			pTDE = 0;
			break;
		  }
#endif // 0

		case kAAFTypeCatFixedArray:
		  {
			// Print out elements of array.
			IAAFTypeDefFixedArray * pTDFA = 0;
			checkResult(pTD->QueryInterface(IID_IAAFTypeDefFixedArray,
										   (void**)&pTDFA));

			// Get number of elements
			aafUInt32 numElems = 0;
			checkResult(pTDFA->GetCount(&numElems));

			cout << "Value: fixed-sized array[" << numElems << "]:\n";

			aafUInt32 i;
			for (i = 0; i < numElems; i++)
			  {
				printIndent (indent);
				cout << "  [" << i << "]: ";
				IAAFPropertyValue * pElemPropVal = 0;
				checkResult(pTDFA->GetElementValue(pPVal, i, &pElemPropVal));
				dumpPropertyValue (pElemPropVal, indent+1);
				pElemPropVal->Release();
			  }

			pTDFA->Release();
			pTDFA = 0;
			break;
		  }

		case kAAFTypeCatVariableArray:
		  {
			// Print out elements of array.
			IAAFTypeDefVariableArray * pTDVA = 0;
			checkResult(pTD->QueryInterface(IID_IAAFTypeDefVariableArray,
										   (void**)&pTDVA));

			// Get number of elements
			aafUInt32 numElems;
			checkResult(pTDVA->GetCount(pPVal, &numElems));

			cout << "Value: variably-sized array[" << numElems << "]:\n";

			aafUInt32 i;
			for (i = 0; i < numElems; i++)
			  {
				printIndent (indent);
				cout << "[" << i << "]: ";
				IAAFPropertyValue * pElemPropVal = 0;
				checkResult(pTDVA->GetElementValue(pPVal, i, &pElemPropVal));
				assert (pElemPropVal);
				dumpPropertyValue (pElemPropVal, indent+1);
				pElemPropVal->Release();
				pElemPropVal = 0;
			  }

			pTDVA->Release();
			pTDVA = 0;
			break;
		  }

		case kAAFTypeCatString:
		  {
			IAAFTypeDefString * pTDS = 0;
			checkResult(pTD->QueryInterface(IID_IAAFTypeDefString,
										   (void**)&pTDS));

			// Get typedef of an element
			IAAFTypeDef * pETD = 0;
			checkResult(pTDS->GetType(&pETD));

			// Strings *must* be made up of integral types.
			IAAFTypeDefInt * pETDInt = 0;
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
				cout << " value: \"" << buf << "\"\n";
			  }
			else if (2 == elemSize)
			  {
				// 2-byte integral characters; interpret as a
				// NULL-terminated wide character string.
				// create an ansi/asci
				char *mbBuf = make_mbstring(bufSize, (wchar_t*) buf);
				checkExpression(NULL != mbBuf, AAFRESULT_NOMEMORY);
				cout << " value: \"" << mbBuf << "\"\n";
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

			assert (pETDInt); pETDInt->Release(); pETDInt = 0;
			assert (pETD);    pETD->Release();    pETD = 0;
			assert (pTDS);    pTDS->Release();    pTDS = 0;
			assert (buf);     delete[] buf;       buf = 0;
			break;
		  }

		case kAAFTypeCatRecord:
		  {
			IAAFTypeDefRecord * pTDR = 0;
			checkResult(pTD->QueryInterface(IID_IAAFTypeDefRecord,
										   (void**)&pTDR));

			// get the number of members in this record
			aafUInt32 numMembers = 0;
			checkResult(pTDR->GetCount(&numMembers));

			cout << "Value: record of " << numMembers << " members:\n";

			// recursively dump each struct field
			aafUInt32 i;
			for (i = 0; i < numMembers; i++)
			  {
				aafUInt32 nameBufLen;
				checkResult(pTDR->GetMemberNameBufLen(i, &nameBufLen));
				wchar_t * nameBuf = (wchar_t*) new wchar_t[nameBufLen];
				assert (nameBuf);
				checkResult(pTDR->GetMemberName(i, nameBuf, nameBufLen));
				char *mbBuf = make_mbstring(nameBufLen, nameBuf); // create an ansi/asci
				checkExpression(NULL != mbBuf, AAFRESULT_NOMEMORY);
				printIndent (indent);
				cout << "  [" << mbBuf << "]: ";
				delete[] nameBuf;
				delete[] mbBuf;

				IAAFPropertyValue * pMemberPropVal = 0;
				checkResult(pTDR->GetValue(pPVal, i, &pMemberPropVal));
				// recursively dump prop value
				dumpPropertyValue (pMemberPropVal, indent+1);
				pMemberPropVal->Release();
				pMemberPropVal = 0;
			  }

			pTDR->Release();
			pTDR = 0;
			break;
		  }

		default:
		  cout << "Unknown type category." << endl;
		  break;
		}
	} // !pTD

  pTD->Release ();
  pTD = 0;

  return AAFRESULT_SUCCESS;
}



static void dumpFile (wchar_t * pwFileName)
{
  assert (pwFileName);

  IAAFFile *   pFile = NULL;
  IAAFHeader * pHeader = NULL;
  IAAFObject * pHdrObj = NULL;

  checkResult(AAFFileOpenExistingRead(pwFileName, 0, &pFile));
  checkResult(pFile->GetHeader(&pHeader));
  assert (pHeader);
  checkResult(pHeader->QueryInterface(IID_IAAFObject,
									  (void**)&pHdrObj));
  assert (pHdrObj);

  dumpObject (pHdrObj, 0);
  if (pHdrObj) pHdrObj->Release (); pHdrObj = 0;
  if (pHeader) pHeader->Release (); pHeader = 0;
  if (pFile)
	{
	  pFile->Close ();
	  pFile->Release ();
	  pFile = 0;
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
  	cout << "Attempting to load the AAF dll...\n";
    HRESULT hr = AAFLoad(dllname);
    cout << (SUCCEEDED(hr)) ? "DONE\n" : "FAILED\n";
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
    cerr << "Error : wrong number of arguments\n";
    return(1);
  }

  // The first argument should be an AAF file name.
  wchar_t pwFileName[260];
  convert(pwFileName, 260, argv[1]);
  
  cout << "***Dumping file "
	   << argv[1]
	   << " using direct prop access***)\n";
  dumpFile (pwFileName);

  cout << "Done\n";

  return(0);
}

#include "AAF.h"
#include "../AAFResult.h"
#include <assert.h>
#include <stdio.h>


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



static HRESULT dumpObject
(
 IAAFObject *pContainer // object to be dumped
);

static HRESULT dumpPropertyValue
(
 IAAFPropertyValue * pPVal
);


HRESULT dumpObject(IAAFObject *pContainer)
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
		  wprintf (L"Property definition: %s; ", nameBuf);
		  delete[] nameBuf;

		  // Get the PropertyValue
		  checkResult(pProp->GetValue(&pPVal));

		  // dump property value
		  dumpPropertyValue (pPVal);

		  pPVal->Release();
		  pPVal = 0;

		  pPDef->Release ();
		  pPDef = 0;

		} // while ()...
	}
  catch (HRESULT &caught)
	{
	  returnHr = caught;
	}

  if (pPropEnum) pPropEnum->Release(); pPropEnum = 0;
  if (pProp) pProp->Release(); pProp = 0;
  if (pPDef) pPDef->Release(); pPDef = 0;
  if (pPVal) pPVal->Release(); pPVal = 0;

  return returnHr;
}


HRESULT dumpPropertyValue (IAAFPropertyValue * pPVal)
{
  // get the type of the data value
  IAAFTypeDef *pTD = 0;
  checkResult(pPVal->GetType(&pTD));

  if (! pTD)
	{
	  // unknown type, due to damaged file.

	  // Dump "damaged object" message.
	  // Optionally, dump the property's bits using pPVal->GetBits().
	  wprintf (L"Unknown type def.\n");
	}

  else
	{
	  {
		wchar_t bigBuf[100];
		assert (pTD);
		IAAFDefObject * pd = NULL;
		checkResult(pTD->QueryInterface(IID_IAAFDefObject, (void**)&pd));
		assert (pd);
		checkResult(pd->GetName (bigBuf, sizeof (bigBuf)));
		pd->Release ();
		wprintf (L"type is %s; ", bigBuf);
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

			wprintf (L"value is %d.\n", val);

			pTDI->Release();
			pTDI = 0;
			break;
		  }

		case kAAFTypeCatStrongObjRef:
		  {
			// strong object reference; recursively dump contents.
			IAAFTypeDefObjectRef * pTDO = 0;
			checkResult(pTD->QueryInterface(IID_IAAFTypeDefObjectRef,
										   (void**)&pTDO));

			IAAFObject * pObj = 0;
			checkResult(pTDO->GetObject(pPVal, &pObj));
			wprintf (L"Value is an object:\n");
			dumpObject (pObj);

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
			dumpPropertyValue (pBasePropVal);

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

			wprintf (L"Value is a fixed-sized of size %d:\n", numElems);

			aafUInt32 i;
			for (i = 0; i < numElems; i++)
			  {
				wprintf (L"Element %d has value: ", i);
				IAAFPropertyValue * pElemPropVal = 0;
				checkResult(pTDFA->GetElementValue(pPVal, i, &pElemPropVal));
				dumpPropertyValue (pElemPropVal);
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

			wprintf (L"Value is a variably-sized array of size %d:\n", numElems);

			aafUInt32 i;
			for (i = 0; i < numElems; i++)
			  {
				wprintf (L"Element %d has value:\n", i);
				IAAFPropertyValue * pElemPropVal = 0;
				checkResult(pTDVA->GetElementValue(pPVal, i, &pElemPropVal));
				dumpPropertyValue (pElemPropVal);
				pElemPropVal->Release();
				pElemPropVal = 0;
			  }

			pTDVA->Release();
			pTDVA = 0;
			break;
		  }

#if 0
		case kAAFTypeCatString:
		  {
			IAAFTypeDefString * pTDS = 0;
			checkResult(pTD->QueryInterface(IID_IAAFTypeDefString,
										   (void**)&pTDS));

			// Get typedef of an element
			IAAFTypeDef * pETD = 0;
			checkResult(pTDS->GetType(&pETD));

			// get the type category of the element
			eAAFTypeCategory_t elemTID;
			checkResult(pETD->GetTypeCategory (&elemTID));

			// See if this is a type we can easily represent
			if (kAAFTypeCatInt == elemTID)
			  {
				// it is some kind of integral type.  Maybe we can
				// dump it.  First, let's actually get the bits.

				// get size of buffer req'd to hold it
				aafUInt32 bufSize = 0;
				checkResult(pPVal->GetBitsSize(&bufSize));
				// and allocate buffer
				aafUInt8 * buf = new aafUInt8[bufSize];
				// get bits
				checkResult(pPVal->GetBits(buf, bufSize));

				// Now check to see what the integer size is.
				aafUInt32 intSize = 0;
				IAAFTypeDefInt * pETDInt = 0;
				checkResult(pETD->QueryInterface(IID_IAAFTypeDefInt,
										   (void**)&pETDInt));
				checkResult(pETDInt->GetSize(&intSize));
				if (1 == intSize)
				  {
					// 1-byte integer characters; could be a C string;
					// check for NULL terminator
					char * term;
					checkResult(pTDS->GetTermValue(&term, sizeof (term)));
					if (NULL == term)
					  {
						// We'll interpret it as a C string.  Use printf
						// or equiv to dump raw chars in buf.
						printf ("string is: %s\n", buf);
					  }
					else
					  {
						// Not NULL-terminated; do a hex dump or
						// equivalent of contents of buf, looking for
						// terminator character.
					  }
				  } // 1==intSize
				else if (2 == intSize)
				  {
					// could be a unicode string; check for NULL terminator
					wchar_t wterm;
					checkResult(pTDS->GetTermValue(&wterm, sizeof (wterm)));
					if (NULL == wterm)
					  {
						// We'll interpret it as a unicode string.
						wprintf (L"string is %s\n", (wchar_t*) buf);
					  }
					else
					  {
						// Not NULL-terminated; do a hex dump or
						// equivalent of contents of buf, looking for
						// terminator character.
					  }
				  } // 2==intSize
				else
				  {
					// elements are too wide to be C or unicode strings;
					// perhaps just hex dump them, looking for terminator
					// character.
				  }

				delete[] buf;
				buf = 0;
			  } // elemType==typeCatInt

			else
			  {
				// type category isn't an int type; dump data in buf
				// similarly to variable array, except check for
				// terminator element.
			  }
		  }
#endif // 0

#if 0
		case kAAFTypeCatComposite:
		  {
			IAAFTypeDefComposite * pTDC = 0;
			checkResult(pTD->QueryInterface(IID_IAAFTypeDefComposite,
										   (void**)&pTDC));

			// get the number of elements in this composite
			aafInt32 numElements = 0;
			checkResult(pTDC->GetCount(&numElements));

			// recursively dump each struct field
			aafInt32 i;
			for (i = 0; i < numElements; i++)
			  {
				aafUInt32 nameBufLen;
				checkResult(pTDC->GetElemNameBufLen(i, &nameBufLen));
				wchar_t * buf = (wchar_t*) new aafUInt8[nameBufLen];
				// print elem name here
				delete[] buf;

				IAAFPropertyValue * pElemPropVal = 0;
				checkResult(pTDC->GetElemValue(pPVal, i, &pElemPropVal));
				// recursively dump prop value
				dumpPropertyValue (pElemPropVal);
				pElemPropVal->Release();
				pElemPropVal = 0;
			  }

			pTDC->Release();
			pTDC = 0;
		  }
#endif // 0

		default:
		  // Unknown type category.  Dump 'error' message.
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

  checkResult(CoCreateInstance(CLSID_AAFFile,
							   NULL, 
							   CLSCTX_INPROC_SERVER, 
							   IID_IAAFFile, 
							   (void **)&pFile));
  assert (pFile);
  checkResult(pFile->Initialize());
  checkResult(pFile->OpenExistingRead(pwFileName, 0));
  checkResult(pFile->GetHeader(&pHeader));
  assert (pHeader);
  checkResult(pHeader->QueryInterface(IID_IAAFObject,
									  (void**)&pHdrObj));
  assert (pHdrObj);

  dumpObject (pHdrObj);
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



main()
{
  CComInitialize comInit;
  wchar_t * pwFileName = L"d:/views/main/coresw/AAF-toolkit/AAFWinSDK/Debug/Examples/Com/EssenceTest.aaf";

  wprintf(L"***Dumping file %s using direct prop access)\n", pwFileName);
  dumpFile (pwFileName);

  printf("Done\n");

  return(0);
}

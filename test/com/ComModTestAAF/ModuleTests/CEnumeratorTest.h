
#ifndef __CEnumeratorTest_h__
#define __CEnumeratorTest_h__

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
 * prior written permission of Avid Technology, Inc.
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
#include "AAFResult.h"
#include "AAFSmartPointer.h"
#include "CAAFBuiltinDefs.h"

#include <stdlib.h>

typedef IAAFSmartPointer<IAAFFile> IAAFFileSP;
typedef IAAFSmartPointer<IAAFHeader> IAAFHeaderSP;
typedef IAAFSmartPointer<IAAFDefObject> IAAFDefObjectSP;
typedef IAAFSmartPointer<IAAFMetaDefinition> IAAFMetaDefinitionSP;

template<class TEnum,class TItem>
class CEnumeratorTest
{
  public:
	CEnumeratorTest() {}
	void Run();

  protected:
	// "Callbacks" to count the number of items we are enumerating over, to get an
	// enumerator, and (is possible) to validate an enumerated item.
	virtual HRESULT CountItems(IAAFDictionary *pDictionary,aafUInt32 *piCount)=0;
	virtual HRESULT GetItems(IAAFDictionary *pDictionary,TEnum **ppEnumerator)=0;
	virtual aafBool ItemIsPresent(IAAFDictionary *pDictionary,aafUID_t& Id)=0;
	// "Callback" to get unique ID of an enumerated item.  The default 
	// implementation will work for any item that implements the IAAFDefObject or
	// IAAFMetaDefinition interfaces.
	virtual void GetAUID(TItem *pItem,aafUID_t *pAUID);

	aafBoolean_t AreAUIDsEqual(aafUID_t& a1,aafUID_t& a2);
	void RemoveTestFile(const wchar_t* pFileName);
	void checkResult(HRESULT r)
	{
		if (FAILED(r))
			throw r;
	}
	void checkExpression(bool expression, HRESULT r=AAFRESULT_TEST_FAILED)
	{
		if (!expression)
			throw r;	
	}
};

template<class TEnum,class TItem>
void CEnumeratorTest<TEnum,TItem>::Run()
{
	typedef IAAFSmartPointer<TEnum> TEnumSP;
	typedef IAAFSmartPointer<TItem> TItemSP;
	typedef TItem* PItem;

	aafProductVersion_t v;
	aafProductIdentification_t	ProductInfo;
	v.major = 1;
	v.minor = 0;
	v.tertiary = 0;
	v.patchLevel = 0;
	v.type = kAAFVersionUnknown;
	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAF Enumerator Test";
	ProductInfo.productVersion = &v;
	ProductInfo.productVersionString = NULL;
	ProductInfo.platform = NULL;

	// Remove the previous test file, if any.
	RemoveTestFile(L"CEnumeratorTest.aaf");

	// Create new AAF file.
	IAAFFileSP pFile;
	checkResult(AAFFileOpenNewModify(L"CEnumeratorTest.aaf",0,&ProductInfo,
		&pFile));

	// Get AAF header & dictionary
	IAAFHeaderSP pHeader;
	checkResult(pFile->GetHeader(&pHeader));
	IAAFDictionarySP pDictionary;
	checkResult(pHeader->GetDictionary(&pDictionary));

	// Get total number of items we should be able to enumerate
	aafUInt32 iItemsCount=-1;
	checkResult(CountItems(pDictionary,&iItemsCount));
	
	// Allocate array of AUIDs in which to store AUIDs of enumerated items
	aafUID_t *pEnumeratedAUIDs=new aafUID_t[iItemsCount];
	checkExpression(pEnumeratedAUIDs!=0);

	// Enumerate through items via NextOne()
	TEnumSP pEnumerator;
	checkResult(GetItems(pDictionary,&pEnumerator));
	TItemSP pCurItem;
	HRESULT hr;
	aafUInt32 iEnumeratedItems=0;
	while((hr=pEnumerator->NextOne(&pCurItem))
		==AAFRESULT_SUCCESS)
	{
		// Make sure the enumerator doesn't give us more items than we expect.
		checkExpression(iEnumeratedItems<iItemsCount);

		// Make sure the item returned by the enumerator is valid.
		aafUID_t CurAUID;
		GetAUID((TItem*)pCurItem,&CurAUID);
		checkExpression(ItemIsPresent(pDictionary,CurAUID)==kAAFTrue);

		// Store item's AUID in array -- later we will make sure all returned
		// AUIDs are unique.
		pEnumeratedAUIDs[iEnumeratedItems]=CurAUID;
		
		iEnumeratedItems++;
	}
	// When NextOne() finally returns failure, it should return the error code
	// AAFRESULT_NO_MORE_OBJECTS.
	checkExpression(hr==AAFRESULT_NO_MORE_OBJECTS);
	
	// Make sure we enumerated all the items.
	checkExpression(iEnumeratedItems==iItemsCount);

	// Make sure all returned AUIDs are unique
	for(aafUInt32 n=0;n<iItemsCount;n++)
		for(aafUInt32 m=0;m<n;m++)
			checkExpression(AreAUIDsEqual(pEnumeratedAUIDs[n],pEnumeratedAUIDs[m])
				==kAAFFalse);

	// Test Next() with all possible fetch sizes.
	// tbd: start at fetch size of 1 when Next() semantics get cleared up
	for(aafUInt32 iFetchSize=2;iFetchSize<=iItemsCount;iFetchSize++)
	{
		aafUInt32 iItemsFetched;
		iEnumeratedItems=0;
		checkResult(pEnumerator->Reset());
		PItem *ppCurItems=new PItem[iFetchSize];

		// Right now, this loop is designed to work with two possible sets of 
		// semantics for Next(): either the function always returns success,
		// but sets iItemsFetched to 0 when there is nothing more to read, or
		// returns AAFRESULT_NO_MORE_OBJECTS whenever iItemsFetched is less than
		// the number of items requested (even if it is still non-zero).  This 
		// second approach is consistent with the Microsoft API, and is probably
		// what we will end up using for all enumerators.
		while(1)
		{
			hr=pEnumerator->Next(iFetchSize,ppCurItems,&iItemsFetched);
			checkExpression(hr==AAFRESULT_SUCCESS||hr==AAFRESULT_NO_MORE_OBJECTS);
			if(iItemsFetched==0)
			{
				// We can un-comment this line once we implement consistent
				// Microsoft-compatible semantics.
				// checkExpression(hr==AAFRESULT_NO_MORE_OBJECTS);
				break;
			}
			if(hr==AAFRESULT_NO_MORE_OBJECTS)
			{
				checkExpression(iItemsFetched<iFetchSize);
			}

			iEnumeratedItems+=iItemsFetched;
			checkExpression(iEnumeratedItems<=iItemsCount);
			// If we haven't yet enumerated all the items, then the number of items
			// fetched should be exactly equal to the argument we supplied.  
			// Otherwise, the check on the previous line ensures that the number 
			// of items fetched is exactly the number of type defs remaining.
			if(iEnumeratedItems<iItemsCount)
				checkExpression(iItemsFetched==iFetchSize);

			// Make sure ppCurItems are what we expect
			for(aafUInt32 iItem=0;iItem<iItemsFetched;iItem++)
			{
				checkExpression(ppCurItems[iItem]!=0);

				// AUID for this item should match up with AUID found when 
				// enumerating via NextOne().

				aafUID_t CurAUID;
				GetAUID((TItem*)ppCurItems[iItem],&CurAUID);
				
				// Compare with AUID stored earlier -- compensate for the fact that
				// iEnumeratedItems has already been incremented.
				checkExpression(AreAUIDsEqual(
					pEnumeratedAUIDs[iEnumeratedItems-iItemsFetched+iItem],
					CurAUID)==kAAFTrue);

				ppCurItems[iItem]->Release();
				ppCurItems[iItem]=0;
			}
		}
		checkExpression(iEnumeratedItems==iItemsCount);
		delete(ppCurItems);
	}

	for(aafUInt32 iSkipSize=1;iSkipSize<=iItemsCount-1;iSkipSize++)
	{
		checkResult(pEnumerator->Reset());
		aafUInt32 iItemIndex=0;
		do
		{
			if(iItemIndex>0)
			{
				aafUID_t CurAUID;
				GetAUID((TItem*)pCurItem,&CurAUID);
				checkExpression(AreAUIDsEqual(pEnumeratedAUIDs[iItemIndex],
					CurAUID)==kAAFTrue);
				iItemIndex++;
			}

			// Call Skip() and check return code.
			hr = pEnumerator->Skip(iSkipSize);
			if(iItemIndex+iSkipSize>iItemsCount)
			{
				// Defined behavior of Skip() is not to advance at all if it would
				// push us off the end, and to return error.
				checkExpression(hr==AAFRESULT_NO_MORE_OBJECTS);
			}
			else
			{
				// Otherwise Skip() must return success.
				checkExpression(hr==AAFRESULT_SUCCESS);
				iItemIndex+=iSkipSize;
			}
		}while((hr=pEnumerator->NextOne(&pCurItem))==AAFRESULT_SUCCESS);
	}

	// Loop through all possible enumerator states, verifying that every state can 
	// be Clone()'d.
	for(aafUInt32 iState=0;iState<=iItemsCount;iState++)
	{
		pEnumerator->Reset();
		if(iState)
			checkResult(pEnumerator->Skip(iState));
		TEnumSP pClone;
		checkResult(pEnumerator->Clone(&pClone));
		// Verify that cloned enumerator is in the correct state -- we have already
		// skipped iState items, so we should be able to read iItemsCount-iState
		// more successfully.
		for(aafUInt32 iItemIndex=iState;iItemIndex<iItemsCount;
			iItemIndex++)
		{
			checkResult(pClone->NextOne(&pCurItem));
			// Verify AUID of each item we read
			aafUID_t CurAUID;
			GetAUID((TItem*)pCurItem,&CurAUID);
			checkExpression(AreAUIDsEqual(pEnumeratedAUIDs[iItemIndex],
				CurAUID)==kAAFTrue);
		}
		// Now we should have read all the items, and the next call to NextOne()
		// should fail.
		checkExpression(pClone->NextOne(&pCurItem)==AAFRESULT_NO_MORE_OBJECTS);
	}
	
	delete(pEnumeratedAUIDs);

	pFile->Save();
	pFile->Close();
}

// Function to get AUID of AAF object
template<class TEnum,class TItem>
void CEnumeratorTest<TEnum,TItem>::GetAUID(TItem *pItem,aafUID_t *pAUID)
{
	// We can either get the AUID through IAAFMetaDefinition or through 
	// IAAFDefObject, so try both.

	IAAFMetaDefinitionSP pMetaDefinition;
	if(pItem->QueryInterface(IID_IAAFMetaDefinition,
		(void**)&pMetaDefinition)==AAFRESULT_SUCCESS)
	{
		checkResult(pMetaDefinition->GetAUID(pAUID));
	}
	else
	{
		// If object does not support IAAFMetaDefinition, it must support
		// IAAFDefObject, or we will raise an exception.
		IAAFDefObjectSP pDefObject;
		checkResult(pItem->QueryInterface(IID_IAAFDefObject,
			(void**)&pDefObject));
		checkResult(pDefObject->GetAUID(pAUID));
	}
}

// Function to test if two AUIDs are equal
template<class TEnum,class TItem>
aafBoolean_t CEnumeratorTest<TEnum,TItem>::AreAUIDsEqual(aafUID_t& a1,
														 aafUID_t& a2)
{
	// We cannot simply call memcmp() (or use the == operator), since there is 
	// padding in between the structure fields due to byte alignment, and this 
	// padding does not have to be equal in order for the AUIDs to be equal.
	if(memcmp(&a1.Data1,&a2.Data1,sizeof(a1.Data1)))
		return(kAAFFalse);
	if(memcmp(&a1.Data2,&a2.Data2,sizeof(a1.Data2)))
		return(kAAFFalse);
	if(memcmp(&a1.Data3,&a2.Data3,sizeof(a1.Data3)))
		return(kAAFFalse);
	if(memcmp(&a1.Data4,&a2.Data4,sizeof(a1.Data4)))
		return(kAAFFalse);
	return(kAAFTrue);
}

// Cross-platform utility to delete a file.
template<class TEnum,class TItem>
void CEnumeratorTest<TEnum,TItem>::RemoveTestFile(const wchar_t* pFileName)
{
  const size_t kMaxFileName = 512;
  char cFileName[kMaxFileName];

  size_t status = wcstombs(cFileName, pFileName, kMaxFileName);
  if (status != (size_t)-1)
  { // delete the file.
    remove(cFileName);
  }
}

#endif

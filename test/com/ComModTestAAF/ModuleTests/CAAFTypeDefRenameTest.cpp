// @doc INTERNAL
// @com This file implements the module test for CAAFTypeDefRename
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

#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>
#include "AAF.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDefUIDs.h"
#include "AAFSmartPointer.h"
#include "AAFStoredObjectIDs.h"
#include "CAAFBuiltinDefs.h"

typedef IAAFSmartPointer<IAAFDefObject> IAAFDefObjectSP;
typedef IAAFSmartPointer<IAAFDictionary> IAAFDictionarySP;
typedef IAAFSmartPointer<IAAFFile> IAAFFileSP;
typedef IAAFSmartPointer<IAAFHeader> IAAFHeaderSP;
typedef IAAFSmartPointer<IAAFMetaDefinition> IAAFMetaDefinitionSP;
typedef IAAFSmartPointer<IAAFObject> IAAFObjectSP;
typedef IAAFSmartPointer<IAAFPropertyDef> IAAFPropertyDefSP;
typedef IAAFSmartPointer<IAAFPropertyValue> IAAFPropertyValueSP;
typedef IAAFSmartPointer<IAAFSequence> IAAFSequenceSP;
typedef IAAFSmartPointer<IAAFTypeDef> IAAFTypeDefSP;
typedef IAAFSmartPointer<IAAFTypeDefIndirect> IAAFTypeDefIndirectSP;
typedef IAAFSmartPointer<IAAFTypeDefInt> IAAFTypeDefIntSP;
typedef IAAFSmartPointer<IAAFTypeDefRename> IAAFTypeDefRenameSP;

// {A3314970-37EA-11d4-9212-0050049C3B9D}
static const aafUID_t AUID_Word = 
{ 0xa3314970, 0x37ea, 0x11d4, { 0x92, 0x12, 0x0, 0x50, 0x4, 0x9c, 0x3b, 0x9d} };

// {70ACBB80-3B18-11d4-9212-0050049C3B9D}
static const aafUID_t AUID_WordProperty = 
{ 0x70acbb80, 0x3b18, 0x11d4, { 0x92, 0x12, 0x0, 0x50, 0x4, 0x9c, 0x3b, 0x9d } };

// Name of the alias (instance of IAAFTypeDefRename) that we will create
static aafWChar *gpAliasName=L"Word";

// convenient error handlers.
inline void checkResult(HRESULT r)
{
  if (FAILED(r))
    throw r;
}

inline void checkExpression(bool expression, HRESULT r=AAFRESULT_TEST_FAILED)
{
  if (!expression)
    throw r;
}

// Cross-platform utility to delete a file.
static void RemoveTestFile(const wchar_t* pFileName)
{
  const size_t kMaxFileName = 512;
  char cFileName[kMaxFileName];

  size_t status = wcstombs(cFileName, pFileName, kMaxFileName);
  if (status != (size_t)-1)
  { // delete the file.
    remove(cFileName);
  }
}

// Function to compare COM interface pointers, taken from 
// CAAFTypeDefFixedArrayTest.cpp.
template <class T1, class T2>
aafBoolean_t  AreUnksSame(T1& cls1, T2& cls2)
{
	IAAFSmartPointer<IUnknown>    spUnk1, spUnk2;
	
	checkResult(cls1->QueryInterface(IID_IUnknown, (void **)&spUnk1));
	checkResult(cls2->QueryInterface(IID_IUnknown, (void **)&spUnk2));
	
	if (spUnk1 == spUnk2)
		return kAAFTrue;
	else
		return kAAFFalse;
}

// Function to test if two AUIDs are equal
static aafBoolean_t AreAUIDsEqual(aafUID_t& a1, aafUID_t& a2)
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

// These two functions fill in the product version and product info structures,
// respectively, for the AAF files we will create
static void FillInProductVersion(aafProductVersion_t& v)
{
	v.major = 1;
	v.minor = 0;
	v.tertiary = 0;
	v.patchLevel = 0;
	v.type = kAAFVersionUnknown;
}

static void FillInProductInfo(aafProductIdentification_t& ProductInfo,
	aafProductVersion_t& v)
{
	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFFile Test";
	ProductInfo.productVersion = &v;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = UnitTestProductID;
	ProductInfo.platform = NULL;
}

static const 	aafMobID_t	TEST_MobID =
	{{0x07, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
	0x13, 0x00, 0x00, 0x00,
	{0x75529074, 0x0404, 0x11d4, 0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}};

// Function to check if property value has a specific type.
static aafBool PropValTypeIs(IAAFPropertyValue *pPropVal,aafUID_t *pAUID)
{
	IAAFTypeDefSP pTypeDef;
	checkResult(pPropVal->GetType(&pTypeDef));
	IAAFMetaDefinitionSP pMetaDef;
	checkResult(pTypeDef->QueryInterface(IID_IAAFMetaDefinition,
		(void**)&pMetaDef));
	aafUID_t PropValAUID;
	checkResult(pMetaDef->GetAUID(&PropValAUID));

	return(AreAUIDsEqual(PropValAUID,*pAUID));
}

static void CreateTypeDefRenameFile(aafWChar *pFilename)
{
	aafProductVersion_t v;
	aafProductIdentification_t	ProductInfo;
	FillInProductVersion(v);
	FillInProductInfo(ProductInfo,v);

	// Remove the previous test file, if any.
	RemoveTestFile(pFilename);

	// Create new AAF file.
	IAAFFileSP pFile;
	checkResult(AAFFileOpenNewModify(pFilename,0,&ProductInfo, &pFile));

	// Get AAF header & dictionary
	IAAFHeaderSP pHeader;
	checkResult(pFile->GetHeader(&pHeader));
	IAAFDictionarySP pDictionary;
	checkResult(pHeader->GetDictionary(&pDictionary));

	// Look up type definition for UInt16
	IAAFTypeDefSP pUInt16TypeDef;
	checkResult(pDictionary->LookupTypeDef(kAAFTypeID_UInt16,&pUInt16TypeDef));
		
	// Create an instance of IAAFTypeDefRename
	IAAFTypeDefRenameSP pTypeDefRenameWord;
	checkResult(pDictionary->CreateMetaInstance(AUID_AAFTypeDefRename,
		IID_IAAFTypeDefRename,(IUnknown **)&pTypeDefRenameWord));

	// Initialize alias for UInt16
	checkResult(pTypeDefRenameWord->Initialize(AUID_Word, // type ID
											pUInt16TypeDef, // Base type definition
											gpAliasName));	// Alias type name

	// Register our new type
	IAAFTypeDefSP pTypeDef;
	checkResult(pTypeDefRenameWord->QueryInterface(IID_IAAFTypeDef,
		(void**)&pTypeDef));
	checkResult(pDictionary->RegisterTypeDef(pTypeDef));

	// Look up definition of our alias type
	IAAFTypeDefSP pWordTypeDef;
	checkResult(pDictionary->LookupTypeDef(AUID_Word,&pWordTypeDef));

	// Add an optional property of type "Word" to AAFSequence.
	IAAFClassDefSP pSequenceClassDef;
	checkResult(pDictionary->LookupClassDef(AUID_AAFSequence,
		&pSequenceClassDef));
	IAAFPropertyDefSP pWordProperty;
	checkResult(pSequenceClassDef->RegisterOptionalPropertyDef(AUID_WordProperty,
			L"Word property",
			pWordTypeDef,
			&pWordProperty));
	checkResult(pDictionary->RegisterClassDef(pSequenceClassDef));

	// Create an instance of IAAFSequence
	IAAFSequenceSP pSequence;
	checkResult(pDictionary->CreateInstance(AUID_AAFSequence,IID_IAAFSequence,
		(IUnknown**)&pSequence));

	// Create integer property value
	IAAFTypeDefIntSP pTypeDefInt;
	checkResult(pUInt16TypeDef->QueryInterface(IID_IAAFTypeDefInt,
		(void**)&pTypeDefInt));
	IAAFPropertyValueSP pIntPropertyValue;
	aafInt16 five=5;
	checkResult(pTypeDefInt->CreateValue((aafMemPtr_t)&five,sizeof(five),
		&pIntPropertyValue));

	// Use CreateValue() to create work prop. val. from integer prop. val
	IAAFPropertyValueSP pWordPropertyValue;
	IAAFTypeDefRenameSP pWordTypeDefRename;
	checkResult(pWordTypeDef->QueryInterface(IID_IAAFTypeDefRename,
		(void**)&pWordTypeDefRename));
	checkResult(pWordTypeDefRename->CreateValue(pIntPropertyValue,
		&pWordPropertyValue));
	checkExpression(PropValTypeIs(pWordPropertyValue,(aafUID_t*)&AUID_Word)
		==kAAFTrue);

	// Try using GetBaseValue to cast back to type integer
	IAAFPropertyValueSP pNewIntPropVal;
	checkResult(pWordTypeDefRename->GetBaseValue(pWordPropertyValue,
		&pNewIntPropVal));
	checkExpression(PropValTypeIs(pNewIntPropVal,(aafUID_t*)&kAAFTypeID_UInt16)
		==kAAFTrue);

	// Set our word property value as the value of the new "word property" of
	// AAFSequence.
	IAAFSmartPointer<IAAFObject> pObject;
	checkResult(pSequence->QueryInterface(IID_IAAFObject,(void**)&pObject));
	checkResult(pObject->SetPropertyValue(pWordProperty,pWordPropertyValue));

	IAAFPropertyValueSP pRetrievedPropertyValue;
	checkResult(pObject->GetPropertyValue(pWordProperty,&pRetrievedPropertyValue));

	checkExpression(PropValTypeIs(pRetrievedPropertyValue,(aafUID_t*)&AUID_Word)
		==kAAFTrue);

	CAAFBuiltinDefs defs (pDictionary);

	// Create a Composition Mob
	IAAFSmartPointer<IAAFMob> pMob;
	checkResult(defs.cdCompositionMob()->CreateInstance(IID_IAAFMob, 
		(IUnknown **)&pMob));
	checkResult(pMob->SetMobID(TEST_MobID));
	checkResult(pMob->SetName(L"TypeDefRenameTest"));

	// Set up sequence
	IAAFSmartPointer<IAAFComponent> pComponent;
	checkResult(pSequence->Initialize(defs.ddSound()));
	checkResult(defs.cdFiller()->CreateInstance(IID_IAAFComponent,
		(IUnknown **)&pComponent));
	checkResult(pComponent->SetDataDef(defs.ddSound()));
	checkResult(pComponent->SetLength(1000));
	checkResult(pSequence->AppendComponent(pComponent));
	  
	// Add timeline Mob slot
	IAAFSmartPointer<IAAFSegment> pSegment;
	checkResult(pSequence->QueryInterface(IID_IAAFSegment,(void **)&pSegment));
	aafRational_t editRate = { 0, 1};
	IAAFSmartPointer<IAAFTimelineMobSlot> pTimelineMobSlot;
	checkResult(pMob->AppendNewTimelineSlot(editRate,pSegment,1,
		L"AAF Test Sequence",0,&pTimelineMobSlot));
	
	aafNumSlots_t numMobs;
	checkResult(pHeader->CountMobs(kAAFCompMob, &numMobs));

	pHeader->AddMob(pMob);

	// Save & close file
	checkResult(pFile->Save());
	checkResult(pFile->Close());
}

static void ReadTypeDefRenameFile(aafWChar *pFilename)
{
	IAAFFileSP pFile;
	checkResult(AAFFileOpenExistingRead(pFilename,0,&pFile));

	// Get AAF header & dictionary
	IAAFHeaderSP pHeader;
	checkResult(pFile->GetHeader(&pHeader));
	IAAFDictionarySP pDictionary;
	checkResult(pHeader->GetDictionary(&pDictionary));

	// Look up definition of our alias type
	IAAFTypeDefSP pWordTypeDef;
	checkResult(pDictionary->LookupTypeDef(AUID_Word,&pWordTypeDef));

	// Verify name of our alias type
	IAAFMetaDefinitionSP pWordMetaDefinition;
	checkResult(pWordTypeDef->QueryInterface(IID_IAAFMetaDefinition,
		(void**)&pWordMetaDefinition));
	aafUInt32 iNameLen;
	checkResult(pWordMetaDefinition->GetNameBufLen(&iNameLen));
	checkExpression(iNameLen==sizeof(wchar_t)*(wcslen(gpAliasName)+1));
	aafWChar *pNameBuf=new aafWChar[iNameLen/sizeof(aafWChar)];
	checkResult(pWordMetaDefinition->GetName(pNameBuf,iNameLen));
	checkExpression(!wcscmp(pNameBuf,gpAliasName));
	delete(pNameBuf);

	// Look up definition of known base type (UInt16)
	IAAFTypeDefSP pUInt16TypeDef;
	checkResult(pDictionary->LookupTypeDef(kAAFTypeID_UInt16,&pUInt16TypeDef));

	// Verify that alias base type is the one we created
	IAAFTypeDefRenameSP pWordTypeDefRename;
	checkResult(pWordTypeDef->QueryInterface(IID_IAAFTypeDefRename,
		(void**)&pWordTypeDefRename));
	IAAFTypeDefSP pBaseTypeDef;
	checkResult(pWordTypeDefRename->GetBaseType(&pBaseTypeDef));

	IAAFMetaDefinitionSP pBaseMetaDefinition;
	checkResult(pBaseTypeDef->QueryInterface(IID_IAAFMetaDefinition,
		(void**)&pBaseMetaDefinition));
	aafUID_t BaseAUID;
	checkResult(pBaseMetaDefinition->GetAUID(&BaseAUID));
	
	IAAFMetaDefinitionSP pUInt16MetaDefinition;
	checkResult(pUInt16TypeDef->QueryInterface(IID_IAAFMetaDefinition,
		(void**)&pUInt16MetaDefinition));
	aafUID_t UInt16_AUID;
	checkResult(pBaseMetaDefinition->GetAUID(&UInt16_AUID));
	checkExpression(AreAUIDsEqual(BaseAUID,UInt16_AUID)==kAAFTrue);

	// Verify that property of renamed type is correctly persisted

	IAAFSmartPointer<IAAFMob> pMob;
	checkResult(pHeader->LookupMob(TEST_MobID,&pMob));
	IAAFSmartPointer<IAAFMobSlot> pSlot;
	checkResult(pMob->GetSlotAt(0,&pSlot));

	IAAFSmartPointer<IAAFSegment> pSegment;
	checkResult(pSlot->GetSegment(&pSegment));
	IAAFObjectSP pObject;
	checkResult(pSegment->QueryInterface(IID_IAAFObject,(void**)&pObject));

	// Look up word property we created
	IAAFSmartPointer<IAAFClassDef> pClassDef;
	checkResult(pDictionary->LookupClassDef(AUID_AAFSequence,&pClassDef));

	IAAFSmartPointer<IAAFPropertyDef> pWordProperty;
	checkResult(pClassDef->LookupPropertyDef(AUID_WordProperty,&pWordProperty));

	IAAFSmartPointer<IAAFPropertyValue> pWordPropertyValue;
	checkResult(pObject->GetPropertyValue(pWordProperty,&pWordPropertyValue));

	checkExpression(PropValTypeIs(pWordPropertyValue,(aafUID_t*)&AUID_Word)
		==kAAFTrue);

	pFile->Close();
}

extern "C" HRESULT CAAFTypeDefRename_test(testMode_t mode);
extern "C" HRESULT CAAFTypeDefRename_test(testMode_t mode)
{
	aafWChar *pTestFilename=L"TypeDefRenameTest.aaf";

	try
	{
      	if(mode == kAAFUnitTestReadWrite)
			CreateTypeDefRenameFile(pTestFilename);
		ReadTypeDefRenameFile(pTestFilename);
//		RemoveTestFile(pTestFilename);
	}
	catch(HRESULT& rResult)
	{
		return(rResult);
	}

	return AAFRESULT_SUCCESS;
}




















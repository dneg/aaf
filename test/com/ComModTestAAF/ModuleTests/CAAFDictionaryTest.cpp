// @doc INTERNAL
// @com This file implements the module test for CAAFDictionary
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

#include "AAF.h"

#include <iostream.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <wchar.h>
#include <string.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "ModuleTest.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"
#include "AAFTypeDefUIDs.h"
#include "AAFClassDefUIDs.h"

#include "CAAFBuiltinDefs.h"

#include "AAFSmartPointer.h"
typedef IAAFSmartPointer<IAAFClassDef>              IAAFClassDefSP;
typedef IAAFSmartPointer<IAAFCodecDef>              IAAFCodecDefSP;
typedef IAAFSmartPointer<IAAFComponent>             IAAFComponentSP;
typedef IAAFSmartPointer<IAAFContainerDef>          IAAFContainerDefSP;
typedef IAAFSmartPointer<IAAFDataDef>               IAAFDataDefSP;
typedef IAAFSmartPointer<IAAFDefObject>             IAAFDefObjectSP;
typedef IAAFSmartPointer<IAAFMetaDefinition>        IAAFMetaDefinitionSP;
typedef IAAFSmartPointer<IAAFDictionary>            IAAFDictionarySP;
typedef IAAFSmartPointer<IAAFFile>                  IAAFFileSP;
typedef IAAFSmartPointer<IAAFFiller>                IAAFFillerSP;
typedef IAAFSmartPointer<IAAFHeader>                IAAFHeaderSP;
typedef IAAFSmartPointer<IAAFInterpolationDef>      IAAFInterpolationDefSP;
typedef IAAFSmartPointer<IAAFMob>                   IAAFMobSP;
typedef IAAFSmartPointer<IAAFMobSlot>               IAAFMobSlotSP;
typedef IAAFSmartPointer<IAAFObject>                IAAFObjectSP;
typedef IAAFSmartPointer<IAAFOperationDef>          IAAFOperationDefSP;
typedef IAAFSmartPointer<IAAFParameterDef>          IAAFParameterDefSP;
typedef IAAFSmartPointer<IAAFPluginDef>				IAAFPluginDefSP;
typedef IAAFSmartPointer<IAAFPropertyDef>           IAAFPropertyDefSP;
typedef IAAFSmartPointer<IAAFPropertyValue>         IAAFPropertyValueSP;
typedef IAAFSmartPointer<IAAFSegment>               IAAFSegmentSP;
typedef IAAFSmartPointer<IAAFSequence>              IAAFSequenceSP;
typedef IAAFSmartPointer<IAAFTimelineMobSlot>       IAAFTimelineMobSlotSP;
typedef IAAFSmartPointer<IAAFTypeDef>               IAAFTypeDefSP;
typedef IAAFSmartPointer<IAAFTypeDefInt>            IAAFTypeDefIntSP;
typedef IAAFSmartPointer<IAAFTypeDefRename>         IAAFTypeDefRenameSP;
typedef IAAFSmartPointer<IEnumAAFClassDefs>         IEnumAAFClassDefsSP;
typedef IAAFSmartPointer<IEnumAAFCodecDefs>         IEnumAAFCodecDefsSP;
typedef IAAFSmartPointer<IEnumAAFComponents>        IEnumAAFComponentsSP;
typedef IAAFSmartPointer<IEnumAAFContainerDefs>     IEnumAAFContainerDefsSP;
typedef IAAFSmartPointer<IEnumAAFDataDefs>          IEnumAAFDataDefsSP;
typedef IAAFSmartPointer<IEnumAAFInterpolationDefs> IEnumAAFInterpolationDefsSP;
typedef IAAFSmartPointer<IEnumAAFMobSlots>          IEnumAAFMobSlotsSP;
typedef IAAFSmartPointer<IEnumAAFMobs>              IEnumAAFMobsSP;
typedef IAAFSmartPointer<IEnumAAFOperationDefs>     IEnumAAFOperationDefsSP;
typedef IAAFSmartPointer<IEnumAAFParameterDefs>     IEnumAAFParameterDefsSP;
typedef IAAFSmartPointer<IEnumAAFPluginDefs>		IEnumAAFPluginDefsSP;
typedef IAAFSmartPointer<IEnumAAFTypeDefs>          IEnumAAFTypeDefsSP;
typedef IAAFSmartPointer<IUnknown>                  IUnknownSP;


#define kNumComponents	5

// {69E9DEB3-4130-11d3-843E-00600832ACB8}
static aafUID_t kClassAUID_NewFill = 
{ 0x69e9deb3, 0x4130, 0x11d3, { 0x84, 0x3e, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

// {69E9DEB4-4130-11d3-843E-00600832ACB8}
static const aafUID_t kPropAUID_NewFill_Odor = 
{ 0x69e9deb4, 0x4130, 0x11d3, { 0x84, 0x3e, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

// Create a GUID unused anywhere else; should force "not found" when
// used to look up defs.
//
// {8CF6DBAC-7BFC-11d3-844F-00600832ACB8}
static const aafUID_t kUnknownTestID = 
{ 0x8cf6dbac, 0x7bfc, 0x11d3, { 0x84, 0x4f, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };


//
// Test def IDs
//

// {8CF6DBAD-7BFC-11d3-844F-00600832ACB8}
static const aafUID_t kTestTypeID = 
{ 0x8cf6dbad, 0x7bfc, 0x11d3, { 0x84, 0x4f, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

// {8CF6DBAE-7BFC-11d3-844F-00600832ACB8}
static const aafUID_t kTestDataDefID = 
{ 0x8cf6dbae, 0x7bfc, 0x11d3, { 0x84, 0x4f, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

// {8CF6DBAF-7BFC-11d3-844F-00600832ACB8}
static const aafUID_t kTestOperationDefID = 
{ 0x8cf6dbaf, 0x7bfc, 0x11d3, { 0x84, 0x4f, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

// {8CF6DBB0-7BFC-11d3-844F-00600832ACB8}
static const aafUID_t kTestParameterDefID = 
{ 0x8cf6dbb0, 0x7bfc, 0x11d3, { 0x84, 0x4f, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

// {8CF6DBB1-7BFC-11d3-844F-00600832ACB8}
static const aafUID_t kTestCodecDefID = 
{ 0x8cf6dbb1, 0x7bfc, 0x11d3, { 0x84, 0x4f, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

// {8CF6DBB2-7BFC-11d3-844F-00600832ACB8}
static const aafUID_t kTestContainerDefID = 
{ 0x8cf6dbb2, 0x7bfc, 0x11d3, { 0x84, 0x4f, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

// {8CF6DBB3-7BFC-11d3-844F-00600832ACB8}
static const aafUID_t kTestInterpolationDefID = 
{ 0x8cf6dbb3, 0x7bfc, 0x11d3, { 0x84, 0x4f, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

// {8CF6DBB4-7BFC-11d3-844F-00600832ACB8}
static const aafUID_t kTestPluginDefID = 
{ 0x8cf6dbb4, 0x7bfc, 0x11d3, { 0x84, 0x4f, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };

// Test MobID
static const 	aafMobID_t	TEST_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0x17c73136, 0x03fe, 0x11d4, 0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}};


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


inline bool EqualGUID (aafUID_t * a,
					   aafUID_t * b)
{
  assert (a);
  assert (b);
  return memcmp (a, b, sizeof (aafUID_t)) ? false : true;
}


static void RegisterNewClass (IAAFDictionary * pDictionary)
{
  //
  // Create a new kind of filler with no new properties.
  //

  // Look up parent class
  IAAFClassDefSP pFillClass;
  CAAFBuiltinDefs defs (pDictionary);
  checkResult (pDictionary->LookupClassDef (AUID_AAFFiller, &pFillClass));
  assert (pFillClass);

  // Create a foward class reference to the new filler class (not really required).
  // This is just to test the methods on Dictionary that implement foward class
  // references.
  checkResult (pDictionary->CreateForwardClassReference(kClassAUID_NewFill));

  // Try a negative test: Attempt to create a forward class reference with the
  // same class id. This should fail.
  checkExpression(FAILED(pDictionary->CreateForwardClassReference(kClassAUID_NewFill)), AAFRESULT_TEST_FAILED);

  // Make sure that the new filler class is recognized as a foward class reference.
  aafBoolean_t isaFowardReference;
  checkResult (pDictionary->HasForwardClassReference(kClassAUID_NewFill, &isaFowardReference));
  checkExpression (kAAFTrue == isaFowardReference, AAFRESULT_TEST_FAILED);

  // Create new object for our new filler class, and initialize it.
  IAAFClassDefSP pNewFillClass;
  checkResult (pDictionary->CreateMetaInstance(AUID_AAFClassDef,
                                               IID_IAAFClassDef,
                                               (IUnknown **)&pNewFillClass));
  checkResult (pNewFillClass->Initialize (kClassAUID_NewFill,
										  pFillClass,
										  L"New Filler",
										  kAAFTrue));

  // Get type def for uint32
  IAAFTypeDefSP ptd;
  checkResult (pDictionary->LookupTypeDef (kAAFTypeID_UInt32,
										   &ptd));
  assert (ptd);

  // Initialize new property
  checkResult
	(pNewFillClass->RegisterNewPropertyDef (kPropAUID_NewFill_Odor,
											L"Odor",
											ptd,
											kAAFFalse,  // mandatory
											kAAFFalse,  // not a unique identifer
											0));

  // Register it in the dictionary.
  checkResult (pDictionary->RegisterClassDef (pNewFillClass));
 
  // If RegisterClassDef succeeds then there should be no foward reference
  // to kClassAUID_NewFill. Test: Make sure that the new filler class is no longer 
  // recognized as a foward class reference.
  checkResult (pDictionary->HasForwardClassReference(kClassAUID_NewFill, &isaFowardReference));
  checkExpression (kAAFFalse == isaFowardReference, AAFRESULT_TEST_FAILED);
}
										  
#define RegisterOneMetaDef( \
 pDict,   /* Dictionary with which to test */ \
 auid,   /* auid of class def for the def to test */ \
 \
 initTypeIID, /* IID of type to pass to Init */ \
 initTypeSP, /* Type of smart pointer to use with Init */ \
 initInvoc, /* expression to invoke to initialize */ \
 \
 qiTypeIID, /* IID of type to QI */ \
 qiTypeSP,  /* SP for type to QI */ \
 \
 registerTypeIID, /* IID of type to register */ \
 registerTypeSP, /* Type of smart pointer to register def */ \
 regFunc) /* Registration function on dict */ \
{ \
  HRESULT hr; \
  assert (pDict); \
  \
  initTypeSP pInitIfc; \
  hr = pDict->CreateMetaInstance(auid, initTypeIID, (IUnknown**) &pInitIfc); \
  if (AAFRESULT_FAILED (hr)) return hr; \
  \
  hr = pInitIfc->initInvoc; \
  if (AAFRESULT_FAILED (hr)) return hr; \
  \
  qiTypeSP pDef; \
  hr = pInitIfc->QueryInterface (qiTypeIID, (void **)&pDef); \
  if (AAFRESULT_FAILED (hr)) return hr; \
  \
  registerTypeSP pRegIfc; \
  hr = pInitIfc->QueryInterface (registerTypeIID, (void **)&pRegIfc); \
  if (AAFRESULT_FAILED (hr)) return hr; \
  \
  hr = pDict->regFunc (pRegIfc); \
  if (AAFRESULT_FAILED (hr)) return hr; \
}


#define RegisterOneDef( \
 pDict,   /* Dictionary with which to test */ \
 class,   /* class def for the def to test */ \
 \
 initTypeIID, /* IID of type to pass to Init */ \
 initTypeSP, /* Type of smart pointer to use with Init */ \
 initInvoc, /* expression to invoke to initialize */ \
 \
 qiTypeIID, /* IID of type to QI */ \
 qiTypeSP,  /* SP for type to QI */ \
 \
 registerTypeIID, /* IID of type to register */ \
 registerTypeSP, /* Type of smart pointer to register def */ \
 regFunc) /* Registration function on dict */ \
{ \
  HRESULT hr; \
  assert (pDict); \
  \
  initTypeSP pInitIfc; \
  hr = class->CreateInstance(initTypeIID, (IUnknown**) &pInitIfc); \
  if (AAFRESULT_FAILED (hr)) return hr; \
  \
  hr = pInitIfc->initInvoc; \
  if (AAFRESULT_FAILED (hr)) return hr; \
  \
  qiTypeSP pDef; \
  hr = pInitIfc->QueryInterface (qiTypeIID, (void **)&pDef); \
  if (AAFRESULT_FAILED (hr)) return hr; \
  \
  registerTypeSP pRegIfc; \
  hr = pInitIfc->QueryInterface (registerTypeIID, (void **)&pRegIfc); \
  if (AAFRESULT_FAILED (hr)) return hr; \
  \
  hr = pDict->regFunc (pRegIfc); \
  if (AAFRESULT_FAILED (hr)) return hr; \
}


#define LookupOneDef( \
 pDict,   /* Dictionary with which to test */ \
 defId,   /* Definition ID identifying the def to test */ \
 \
 name,    /* Name of def object */ \
 \
 lookupTypeIID, /* IID of type to look up */ \
 lookupTypeSP, /* Type of smart pointer to look up def */ \
 lookupFunc, /* Lookup function on dict */ \
 \
 nameTypeIID, /* IID of type to get name */ \
 nameTypeSP,  /* SP for type to get name */ \
 \
 enumTypeSP, /* type of enumerator across defs */ \
 getEnumFunc) /* func on dictionary to get enumerator */ \
{ \
  HRESULT hr; \
  assert (pDict); \
  \
  lookupTypeSP pThisDef; \
  hr = pDict->lookupFunc (kUnknownTestID, &pThisDef); \
  if (AAFRESULT_NO_MORE_OBJECTS != hr) return E_FAIL; \
  \
  hr = pDict->lookupFunc (defId, &pThisDef); \
  if (FAILED (hr)) return hr; \
  \
  nameTypeSP pDef; \
  hr = pThisDef->QueryInterface (nameTypeIID, (void **)&pDef); \
  if (FAILED (hr)) return hr; \
  \
  lookupTypeSP junk; \
  hr = pDef->QueryInterface (lookupTypeIID, (void **)&junk); \
  if (FAILED (hr)) return hr; \
  if (0 == (void*) junk) return E_FAIL; \
  \
  aafCharacter nameBuf[100]; \
  hr = pDef->GetName (nameBuf, sizeof (nameBuf)); \
  if (FAILED (hr)) return hr; \
  if (wcscmp (nameBuf, name)) return E_FAIL; \
  \
  enumTypeSP pEnum; \
  hr = pDict->getEnumFunc (&pEnum); \
  if (FAILED (hr)) return hr; \
  \
  bool found = false; \
  hr = pEnum->NextOne (&pThisDef); \
  while (AAFRESULT_SUCCEEDED (hr)) \
	{ \
	  nameTypeSP pDef; \
	  hr = pThisDef->QueryInterface (nameTypeIID, (void **)&pDef); \
	  if (FAILED (hr)) return hr; \
      \
	  aafUID_t id; \
	  hr = pDef->GetAUID (&id); \
	  if (FAILED (hr)) return hr; \
      \
	  if (EqualGUID (&id, (aafUID_t*) &defId)) \
		{ \
		  found = true; \
          \
		  nameBuf[0] = 0;	/* empty it */ \
		  hr = pDef->GetName (nameBuf, sizeof (nameBuf)); \
		  if (FAILED (hr)) return hr; \
		  if (wcscmp (nameBuf, name)) return E_FAIL; \
		  \
		  break; \
		} \
	  hr = pEnum->NextOne (&pThisDef); \
	} \
  if (! found) return E_FAIL; \
}


static HRESULT RegisterDefs (IAAFDictionary * pDict)
{
  assert (pDict);

  CAAFBuiltinDefs defs (pDict);

  
//  IAAFTypeDefSP pTypeUInt8;
//  checkResult(pDict->LookupTypeDef(kAAFTypeID_UInt8, &pTypeUInt8));
  { // Create an opaque type and register it in the dictionary.
    IAAFTypeDefSP pOpaqueType;
    IAAFTypeDefRenameSP pOpaqueTestType;
	  checkResult(pDict->CreateMetaInstance(AUID_AAFTypeDefRename, IID_IAAFTypeDefRename, (IUnknown **)&pOpaqueTestType));
    checkResult(pOpaqueTestType->Initialize(kTestTypeID, defs.tdUInt8(), L"TestUInt8"));
    checkResult(pOpaqueTestType->QueryInterface(IID_IAAFTypeDef, (void **)&pOpaqueType));
    checkResult(pDict->RegisterOpaqueTypeDef(pOpaqueType));
  }

  RegisterOneMetaDef (/* dictionary*/      pDict,
				  /* def object's class id */      AUID_AAFTypeDefRename,
				  /* IID of def to pass to Init */ IID_IAAFTypeDefRename,
				  /* SP of def to use with Init */ IAAFTypeDefRenameSP,
				  /* Init() invocation */
				  Initialize (kTestTypeID, defs.tdUInt8(), L"TestUInt8"),
				  /* IID of type to QI */          IID_IAAFMetaDefinition,
				  /* SP for type to QI */          IAAFMetaDefinitionSP,
				  /* IID of def to register */     IID_IAAFTypeDef,
				  /* SP for def to register */     IAAFTypeDefSP,
				  /* reg method on pDict */        RegisterTypeDef);

  { // Make sure that "TestUInt8" is no longer opaque.
    IAAFTypeDefSP pOpaqueTestType;
    checkExpression(FAILED(pDict->LookupOpaqueTypeDef(kTestTypeID, &pOpaqueTestType)),
                    AAFRESULT_TEST_FAILED);
  }

    
  RegisterOneDef (/* dictionary*/                  pDict,
				  /* def object's class */         defs.cdDataDef(),
				  /* IID of def to pass to Init */ IID_IAAFDataDef,
				  /* SP of def to use with Init */ IAAFDataDefSP,
				  /* Init() invocation */
				  Initialize (kTestDataDefID, L"TestDataDef", L"Desc"),
				  /* IID of type to QI */          IID_IAAFDefObject,
				  /* SP for type to QI */          IAAFDefObjectSP,
				  /* IID of def to register */     IID_IAAFDataDef,
				  /* SP for def to register */     IAAFDataDefSP,
				  /* reg method on pDict */        RegisterDataDef);
  
  RegisterOneDef (/* dictionary*/                  pDict,
				  /* def object's class */         defs.cdOperationDef(),
				  /* IID of def to pass to Init */ IID_IAAFOperationDef,
				  /* SP of def to use with Init */ IAAFOperationDefSP,
				  /* Init() invocation */
				  Initialize (kTestOperationDefID, L"TestOperationDef", L"Desc"),
				  /* IID of type to QI */          IID_IAAFDefObject,
				  /* SP for type to QI */          IAAFDefObjectSP,
				  /* IID of def to register */     IID_IAAFOperationDef,
				  /* SP for def to register */     IAAFOperationDefSP,
				  /* reg method on pDict */        RegisterOperationDef);

  // Hack!
  {
	IAAFOperationDefSP od;
	checkResult (pDict->LookupOperationDef (kTestOperationDefID,
										    &od));
	checkResult (od->SetDataDef (defs.ddMatte()));
  }  
  
  RegisterOneDef (/* dictionary*/                  pDict,
				  /* def object's class */         defs.cdParameterDef(),
				  /* IID of def to pass to Init */ IID_IAAFParameterDef,
				  /* SP of def to use with Init */ IAAFParameterDefSP,
				  /* Init() invocation */
				  Initialize (kTestParameterDefID, L"TestParameterDef", L"Desc", defs.tdRational()),
				  /* IID of type to QI */          IID_IAAFDefObject,
				  /* SP for type to QI */          IAAFDefObjectSP,
				  /* IID of def to register */     IID_IAAFParameterDef,
				  /* SP for def to register */     IAAFParameterDefSP,
				  /* reg method on pDict */        RegisterParameterDef);

				  RegisterOneDef (/* dictionary*/                  pDict,
				  /* def object's class */         defs.cdCodecDef(),
				  /* IID of def to pass to Init */ IID_IAAFCodecDef,
				  /* SP of def to use with Init */ IAAFCodecDefSP,
				  /* Init() invocation */
				  Initialize (kTestCodecDefID, L"TestCodecDef", L"Desc"),
				  /* IID of type to QI */          IID_IAAFDefObject,
				  /* SP for type to QI */          IAAFDefObjectSP,
				  /* IID of def to register */     IID_IAAFCodecDef,
				  /* SP for def to register */     IAAFCodecDefSP,
				  /* reg method on pDict */        RegisterCodecDef);

  // Hack! Codec defs must have at least one data def appended in
  // order to be saved correctly...
  {
	IAAFCodecDefSP cd;
	IAAFClassDefSP pClassDef;
	checkResult (pDict->LookupCodecDef (kTestCodecDefID,
										&cd));
	checkResult (cd->AddEssenceKind (defs.ddMatte()));
	checkResult(pDict->LookupClassDef(kAAFClassID_EssenceDescriptor, &pClassDef));
	checkResult(cd->SetFileDescriptorClass (pClassDef));
  }  

  RegisterOneDef (/* dictionary*/                  pDict,
				  /* def object's class */         defs.cdContainerDef(),
				  /* IID of def to pass to Init */ IID_IAAFContainerDef,
				  /* SP of def to use with Init */ IAAFContainerDefSP,
				  /* Init() invocation */
				  Initialize (kTestContainerDefID, L"TestContainerDef", L"Desc"),
				  /* IID of type to QI */          IID_IAAFDefObject,
				  /* SP for type to QI */          IAAFDefObjectSP,
				  /* IID of def to register */     IID_IAAFContainerDef,
				  /* SP for def to register */     IAAFContainerDefSP,
				  /* reg method on pDict */        RegisterContainerDef);
  
  RegisterOneDef (/* dictionary*/                  pDict,
				  /* def object's class */         defs.cdInterpolationDefinition(),
				  /* IID of def to pass to Init */ IID_IAAFInterpolationDef,
				  /* SP of def to use with Init */ IAAFInterpolationDefSP,
				  /* Init() invocation */
				  Initialize (kTestInterpolationDefID, L"TestInterpolationDef", L"Desc"),
				  /* IID of type to QI */          IID_IAAFDefObject,
				  /* SP for type to QI */          IAAFDefObjectSP,
				  /* IID of def to register */     IID_IAAFInterpolationDef,
				  /* SP for def to register */     IAAFInterpolationDefSP,
				  /* reg method on pDict */        RegisterInterpolationDef);
  
  RegisterOneDef (/* dictionary*/                  pDict,
				  /* def object's class */         defs.cdPluginDef(),
				  /* IID of def to pass to Init */ IID_IAAFPluginDef,
				  /* SP of def to use with Init */ IAAFPluginDefSP,
				  /* Init() invocation */
				  Initialize (kTestPluginDefID, L"TestPluginDef", L"Desc"),
				  /* IID of type to QI */          IID_IAAFObject,
				  /* SP for type to QI */          IAAFObjectSP,
				  /* IID of def to register */     IID_IAAFPluginDef,
				  /* SP for def to register */     IAAFPluginDefSP,
				  /* reg method on pDict */        RegisterPluginDef);
  
  return AAFRESULT_SUCCESS;
}


static HRESULT LookupDefs (IAAFDictionary * pDict)
{
  assert (pDict);

  LookupOneDef (/* dictionary */               pDict,
				/* ID of def to look up */     kTestTypeID,
				/* def's name */               L"TestUInt8",
				/* IID of def for lookup */    IID_IAAFTypeDef,
				/* SP of def to lookup */      IAAFTypeDefSP,
				/* lookup method on pDict */   LookupTypeDef,
				/* IID of def for name */      IID_IAAFMetaDefinition,
				/* SP of def for name */       IAAFMetaDefinitionSP,
				/* SP of enumerator */         IEnumAAFTypeDefsSP,
				/* get-enum method on pDict */ GetTypeDefs);

  LookupOneDef (/* dictionary */               pDict,
				/* ID of def to look up */     kTestDataDefID,
				/* def's name */               L"TestDataDef",
				/* IID of def for lookup */    IID_IAAFDataDef,
				/* SP of def to lookup */      IAAFDataDefSP,
				/* lookup method on pDict */   LookupDataDef,
				/* IID of def for name */      IID_IAAFDefObject,
				/* SP of def for name */       IAAFDefObjectSP,
				/* SP of enumerator */         IEnumAAFDataDefsSP,
				/* get-enum method on pDict */ GetDataDefs);

  LookupOneDef (/* dictionary */               pDict,
				/* ID of def to look up */     kTestOperationDefID,
				/* def's name */               L"TestOperationDef",
				/* IID of def for lookup */    IID_IAAFOperationDef,
				/* SP of def to lookup */      IAAFOperationDefSP,
				/* lookup method on pDict */   LookupOperationDef,
				/* IID of def for name */      IID_IAAFDefObject,
				/* SP of def for name */       IAAFDefObjectSP,
				/* SP of enumerator */         IEnumAAFOperationDefsSP,
				/* get-enum method on pDict */ GetOperationDefs);

  LookupOneDef (/* dictionary */               pDict,
				/* ID of def to look up */     kTestParameterDefID,
				/* def's name */               L"TestParameterDef",
				/* IID of def for lookup */    IID_IAAFParameterDef,
				/* SP of def to lookup */      IAAFParameterDefSP,
				/* lookup method on pDict */   LookupParameterDef,
				/* IID of def for name */      IID_IAAFDefObject,
				/* SP of def for name */       IAAFDefObjectSP,
				/* SP of enumerator */         IEnumAAFParameterDefsSP,
				/* get-enum method on pDict */ GetParameterDefs);

  LookupOneDef (/* dictionary */               pDict,
				/* ID of def to look up */     kTestCodecDefID,
				/* def's name */               L"TestCodecDef",
				/* IID of def for lookup */    IID_IAAFCodecDef,
				/* SP of def to lookup */      IAAFCodecDefSP,
				/* lookup method on pDict */   LookupCodecDef,
				/* IID of def for name */      IID_IAAFDefObject,
				/* SP of def for name */       IAAFDefObjectSP,
				/* SP of enumerator */         IEnumAAFCodecDefsSP,
				/* get-enum method on pDict */ GetCodecDefs);

  LookupOneDef (/* dictionary */               pDict,
				/* ID of def to look up */     kTestContainerDefID,
				/* def's name */               L"TestContainerDef",
				/* IID of def for lookup */    IID_IAAFContainerDef,
				/* SP of def to lookup */      IAAFContainerDefSP,
				/* lookup method on pDict */   LookupContainerDef,
				/* IID of def for name */      IID_IAAFDefObject,
				/* SP of def for name */       IAAFDefObjectSP,
				/* SP of enumerator */         IEnumAAFContainerDefsSP,
				/* get-enum method on pDict */ GetContainerDefs);

  LookupOneDef (/* dictionary */               pDict,
				/* ID of def to look up */     kTestInterpolationDefID,
				/* def's name */               L"TestInterpolationDef",
				/* IID of def for lookup */    IID_IAAFInterpolationDef,
				/* SP of def to lookup */      IAAFInterpolationDefSP,
				/* lookup method on pDict */   LookupInterpolationDef,
				/* IID of def for name */      IID_IAAFDefObject,
				/* SP of def for name */       IAAFDefObjectSP,
				/* SP of enumerator */         IEnumAAFInterpolationDefsSP,
				/* get-enum method on pDict */ GetInterpolationDefs);

  LookupOneDef (/* dictionary */               pDict,
				/* ID of def to look up */     kTestPluginDefID,
				/* def's name */               L"TestPluginDef",
				/* IID of def for lookup */    IID_IAAFPluginDef,
				/* SP of def to lookup */      IAAFPluginDefSP,
				/* lookup method on pDict */   LookupPluginDef,
				/* IID of def for name */      IID_IAAFDefObject,
				/* SP of def for name */       IAAFDefObjectSP,
				/* SP of enumerator */         IEnumAAFPluginDefsSP,
				/* get-enum method on pDict */ GetPluginDefs);

  return AAFRESULT_SUCCESS;
}


static HRESULT OpenAAFFile(aafWChar*			pFileName,
						   aafMediaOpenMode_t	mode,
						   IAAFFile**			ppFile,
						   IAAFHeader**			ppHeader)
{
	aafProductIdentification_t	ProductInfo;
	HRESULT						hr = AAFRESULT_SUCCESS;

	aafProductVersion_t v;
	v.major = 1;
	v.minor = 0;
	v.tertiary = 0;
	v.patchLevel = 0;
	v.type = kAAFVersionUnknown;
	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFDictionary Test";
	ProductInfo.productVersion = &v;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = UnitTestProductID;
	ProductInfo.platform = NULL;

	*ppFile = NULL;

	switch (mode)
	{
	case kAAFMediaOpenReadOnly:
		hr = AAFFileOpenExistingRead(pFileName, 0, ppFile);
		break;

	case kAAFMediaOpenAppend:
		hr = AAFFileOpenNewModify(pFileName, 0, &ProductInfo, ppFile);
		break;

	default:
		hr = AAFRESULT_TEST_FAILED;
		break;
	}

	if (FAILED(hr))
	{
		if (*ppFile)
		{
			(*ppFile)->Release();
			*ppFile = NULL;
		}
		return hr;
	}
  
  	hr = (*ppFile)->GetHeader(ppHeader);
	if (FAILED(hr))
	{
		(*ppFile)->Release();
		*ppFile = NULL;
		return hr;
	}
 	
	return hr;
}

static HRESULT CreateAAFFile(aafWChar * pFileName)
{
  IAAFFileSP       pFile;
  IAAFHeaderSP     pHeader;
  IAAFDictionarySP pDictionary;
  IAAFMobSP        pMob;
  IAAFTimelineMobSlotSP    pMobSlot;
  IAAFSequenceSP   pSequence;
  IAAFSegmentSP    pSegment;
  IAAFComponentSP  pComponent;
  int              i;
  HRESULT          hr = S_OK;
	
	
  try
	{  
	  // Remove the previous test file if any.
	  RemoveTestFile(pFileName);
		
	  // Create the AAF file
	  checkResult(OpenAAFFile(pFileName, kAAFMediaOpenAppend, &pFile, &pHeader));
		
	  // Get the AAF Dictionary so that we can create valid AAF objects.
	  checkResult(pHeader->GetDictionary(&pDictionary));
		
	  // Create a new class, and register it in the dictionary.
	  RegisterNewClass (pDictionary);
	  CAAFBuiltinDefs defs (pDictionary);

	  // Create a Composition Mob
	  checkResult(defs.cdCompositionMob()->
				  CreateInstance(IID_IAAFMob, 
								 (IUnknown **)&pMob));
		
	  checkResult(pMob->SetMobID(TEST_MobID));
	  checkResult(pMob->SetName(L"AAFDictionaryTest"));
		
	  // Add mob slot w/ Sequence
	  checkResult(defs.cdSequence()->
				  CreateInstance(IID_IAAFSequence, 
								 (IUnknown **)&pSequence));		

	  checkResult(pSequence->Initialize(defs.ddPicture()));
		
	  //
	  //	Add some segments.  Need to test failure conditions
	  //	(i.e. starting/ending w/ transition, two trans back
	  //	to back).
	  //
	  for(i = 0; i < kNumComponents; i++)
		{
		  aafLength_t		len = 10;
			
		  // For the first component, make it our extended filler.
		  if(i == 0)
			{
			  IAAFClassDefSP pNewFillClassDef;
			  checkResult(pDictionary->LookupClassDef(kClassAUID_NewFill,
													  &pNewFillClassDef));
			  checkResult
				(pNewFillClassDef->CreateInstance(IID_IAAFComponent, 
												  (IUnknown**)&pComponent));
			  checkResult(pComponent->SetDataDef(defs.ddPictureWithMatte()));
			}
		  else
			{
			  checkResult
				(defs.cdFiller()->CreateInstance(IID_IAAFComponent, 
												 (IUnknown**)&pComponent));

			  checkResult(pComponent->SetDataDef(defs.ddPicture()));
			}

		  checkResult(pComponent->SetLength(len));
		  checkResult(pSequence->AppendComponent(pComponent));
			
		  // For our first component, set the 'odor' value.  Must be
		  // done after the component has been inserted in sequence.
		  if (i == 0)
			{
			  // Set the odor value.
			  //
			  // 1) Get type def for uint32
			  IAAFTypeDefSP ptd;
			  checkResult (pDictionary->LookupTypeDef (kAAFTypeID_UInt32,
													   &ptd));
			  assert (ptd);
			  IAAFTypeDefIntSP pTDUint32;
			  checkResult(ptd->QueryInterface (IID_IAAFTypeDefInt,
											   (void **)&pTDUint32));
			  assert (pTDUint32);

			  // 2) Create a property value for the odor property, and
			  //    set it to 42.
			  IAAFPropertyValueSP pVal;
			  const aafUInt32 odorValue = 42;
			  checkResult (pTDUint32->CreateValue ((aafMemPtr_t) &odorValue,
												   sizeof (odorValue),
												   &pVal));

			  // 3) Look up the property def for the odor property in
			  //    the new fill class.
			  IAAFClassDefSP pNewFillClass;
			  checkResult (pDictionary->LookupClassDef (kClassAUID_NewFill,
														&pNewFillClass));
			  IAAFPropertyDefSP pPropDef;
			  checkResult (pNewFillClass->LookupPropertyDef (kPropAUID_NewFill_Odor,
															 &pPropDef));

			  // 4) Get IAAFObject interface for new fill object, and
			  //    set the odor property.
			  IAAFObjectSP pObj;
			  checkResult(pComponent->QueryInterface (IID_IAAFObject,
													  (void **)&pObj));
			  checkResult (pObj->SetPropertyValue (pPropDef, pVal));
			}
		}

	  checkResult(pSequence->QueryInterface (IID_IAAFSegment, (void **)&pSegment));
		
	  aafRational_t editRate = { 0, 1};
	  checkResult(pMob->AppendNewTimelineSlot(editRate,
											  pSegment,
											  1,
											  L"AAF Test Sequence",
											  0,
											  &pMobSlot));
		
	  // Add the master mob to the file and cleanup
	  pHeader->AddMob(pMob);

	  checkResult (RegisterDefs (pDictionary));
	}
  catch (HRESULT& rResult)
	{
	  hr = rResult;
	}
	
  // Cleanup and return
  if (pFile)
	{
	  pFile->Save();
	  pFile->Close();
	}
  return hr;
}

static HRESULT ReadAAFFile(aafWChar* pFileName)
{
  IAAFFileSP           pFile;
  IAAFHeaderSP         pHeader;
  IEnumAAFMobsSP       pMobIter;
  IAAFMobSP            pMob;
  IEnumAAFMobSlotsSP   pSlotIter;
  IAAFMobSlotSP        pSlot;
  IAAFComponentSP      pComp;
  IAAFSegmentSP        pSegment;
  IAAFDataDefSP        pDataDef;
  IAAFSequenceSP       pSequence;
  IAAFDictionarySP     pDictionary;
  IEnumAAFComponentsSP pCompIter;
  aafNumSlots_t        numMobs;
  aafSearchCrit_t      criteria;
  HRESULT              hr = S_OK;
	
  try
	{
	  // Open the AAF file
	  checkResult(OpenAAFFile(pFileName, kAAFMediaOpenReadOnly, &pFile, &pHeader));
		
	  // Validate that there is only one composition mob.
	  checkResult(pHeader->CountMobs(kAAFCompMob, &numMobs));
	  checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);
		
	  // Get the AAF Dictionary so that we can create valid AAF objects.
	  checkResult(pHeader->GetDictionary(&pDictionary));
	  CAAFBuiltinDefs defs (pDictionary);

	  // Check a data definition from a composition MOB in order to test weak references
	  criteria.searchTag = kAAFByMobKind;
	  criteria.tags.mobKind = kAAFCompMob;
	  checkResult(pHeader->GetMobs(&criteria, &pMobIter));
	  while (pMobIter && pMobIter->NextOne(&pMob) == AAFRESULT_SUCCESS)
		{					
		  // Enumerate the first MOB slot for this MOB
		  checkResult(pMob->GetSlots(&pSlotIter));
		  checkResult(pSlotIter->NextOne(&pSlot));

		  checkResult(pSlot->GetSegment(&pSegment));
		  checkResult(pSegment->QueryInterface(IID_IAAFSequence, (void **) &pSequence));
		  checkResult(pSequence->GetComponents(&pCompIter));
		  checkResult(pCompIter->NextOne(&pComp));

		  aafBool  testBool;

		  checkResult(pComp->GetDataDef(&pDataDef));
		  checkResult(pDataDef->IsSoundKind(&testBool));
		  checkExpression(testBool == kAAFFalse, AAFRESULT_TEST_FAILED);

		  checkResult(pDataDef->IsDataDefOf(defs.ddPictureWithMatte(), &testBool));
		  checkExpression(testBool == kAAFTrue, AAFRESULT_TEST_FAILED);
		
		  // Make sure first component is a filler, and is our extended
		  // class.  To do that, we'll compare the class def we looked
		  // up in the dict, with the one we got from the new object.
		  //
		  // First get the class from the object.
		  IAAFFillerSP pFill;
		  checkResult(pComp->QueryInterface(IID_IAAFFiller,
											(void **) &pFill));
		  assert (pFill);

		  IAAFObjectSP pObj;
		  checkResult(pFill->QueryInterface(IID_IAAFObject,
											(void **) &pObj));
		  assert (pObj);
		  IAAFClassDefSP pClassFromObj;
		  checkResult (pObj->GetDefinition (&pClassFromObj));
		  assert (pClassFromObj);
		  IUnknownSP pUnkFromObj;
		  checkResult(pClassFromObj->QueryInterface(IID_IUnknown,
													(void **) &pUnkFromObj));

		  // Now get the class from the dict
		  IAAFClassDefSP pClassFromDict;
		  checkResult (pDictionary->LookupClassDef (kClassAUID_NewFill,
													&pClassFromDict));
		  assert (pClassFromDict);
		  IUnknownSP pUnkFromDict;
		  checkResult(pClassFromDict->QueryInterface(IID_IUnknown,
													 (void **) &pUnkFromDict));

		  // Compare class from object with class from dict.  Compare
		  // using IUnknown pointers.
		  assert (((IUnknown*)pUnkFromObj) ==
				  ((IUnknown*)pUnkFromDict));

		  // To test GetClassDefinitions(), try explicit lookup.
		  IEnumAAFClassDefsSP pClassDefEnum;
		  checkResult (pDictionary->GetClassDefs (&pClassDefEnum));
		  bool found = false;
		  IAAFClassDefSP cd;
		  while (SUCCEEDED (pClassDefEnum->NextOne (&cd)))
			{
			  IAAFMetaDefinitionSP def;
			  checkResult(cd->QueryInterface(IID_IAAFMetaDefinition,
											 (void **) &def));
			  aafUID_t classid;
			  checkResult (def->GetAUID (&classid));
			  if (EqualGUID (&classid, &kClassAUID_NewFill))
				{
				  // Found it the hard way.
				  found = true;
				  break;
				}
			}
		  // make sure we found it the hard way.
		  checkExpression(found == kAAFTrue, AAFRESULT_TEST_FAILED);

		  // Get the 'odor' property from our new fill clip.  Make
		  // sure it is set to the value we think it should be
		  // ('42').
		  //
		  // First get the property def from the class.
		  IAAFPropertyDefSP pPropDef;
		  checkResult (pClassFromObj->LookupPropertyDef
					   (kPropAUID_NewFill_Odor,
						&pPropDef));
		  //
		  // Get the property value from the object
		  IAAFPropertyValueSP pPropVal;
		  checkResult (pObj->GetPropertyValue (pPropDef, &pPropVal));
		  // 
		  // We know the property is int32; get the int32 type def
		  IAAFTypeDefSP ptd;
		  checkResult (pDictionary->LookupTypeDef
					   (kAAFTypeID_UInt32,
						&ptd));
		  IAAFTypeDefIntSP pTDUint32;
		  checkResult(ptd->QueryInterface(IID_IAAFTypeDefInt,
										  (void **) &pTDUint32));
		  assert (pTDUint32);
		  //
		  // Ask the typedef to interpret this property value for us.
		  aafUInt32 odorValue = 0;
		  checkResult (pTDUint32->GetInteger
					   (pPropVal,
						(aafMemPtr_t) &odorValue,
						sizeof (odorValue)));
		  //
		  // make sure it's what we expect.
		  assert (42 == odorValue);
		}

	  checkResult (LookupDefs (pDictionary));
	}
  catch (HRESULT& rResult)
	{
	  hr = rResult;
	}
	
  // Cleanup and return.
  if (pFile)
	{
	  pFile->Close();
	}
  return 	hr;
}

extern "C" HRESULT CAAFDictionary_test(testMode_t mode);
extern "C" HRESULT CAAFDictionary_test(testMode_t mode)
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  aafWChar * pFileName = L"AAFDictionaryTest.aaf";

  try
	{
		if(mode == kAAFUnitTestReadWrite)
			hr = CreateAAFFile(pFileName);
		else
			hr = AAFRESULT_SUCCESS;
	  if (SUCCEEDED(hr))
		hr = ReadAAFFile(pFileName);
	}
  catch (...)
	{
	  cerr << "CAAFDictionary_test..."
		   << "Caught general C++ exception!" << endl; 
	  hr = AAFRESULT_TEST_FAILED;
	}

  return hr;
}

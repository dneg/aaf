// @doc INTERNAL
// @com This file implements the module test for CAAFDictionary
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

#include <iostream.h>
#include <stdio.h>
#include <assert.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "AAFDataDefs.h"
#include "AAFDefUIDs.h"
#include "AAFTypeDefUIDs.h"

#define kNumComponents	5

#include "AAFSmartPointer.h"
typedef IAAFSmartPointer<IAAFClassDef>              IAAFClassDefSP;
typedef IAAFSmartPointer<IAAFCodecDef>              IAAFCodecDefSP;
typedef IAAFSmartPointer<IAAFComponent>             IAAFComponentSP;
typedef IAAFSmartPointer<IAAFContainerDef>          IAAFContainerDefSP;
typedef IAAFSmartPointer<IAAFDataDef>               IAAFDataDefSP;
typedef IAAFSmartPointer<IAAFDefObject>             IAAFDefObjectSP;
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
typedef IAAFSmartPointer<IAAFPluginDescriptor>      IAAFPluginDescriptorSP;
typedef IAAFSmartPointer<IAAFPropertyDef>           IAAFPropertyDefSP;
typedef IAAFSmartPointer<IAAFPropertyValue>         IAAFPropertyValueSP;
typedef IAAFSmartPointer<IAAFSegment>               IAAFSegmentSP;
typedef IAAFSmartPointer<IAAFSequence>              IAAFSequenceSP;
typedef IAAFSmartPointer<IAAFTypeDef>               IAAFTypeDefSP;
typedef IAAFSmartPointer<IAAFTypeDefInt>            IAAFTypeDefIntSP;
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
typedef IAAFSmartPointer<IEnumAAFPluginDescriptors> IEnumAAFPluginDescriptorsSP;
typedef IAAFSmartPointer<IEnumAAFTypeDefs>          IEnumAAFTypeDefsSP;
typedef IAAFSmartPointer<IUnknown>                  IUnknownSP;


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
static const aafUID_t kTestPluginDescriptorID = 
{ 0x8cf6dbb4, 0x7bfc, 0x11d3, { 0x84, 0x4f, 0x0, 0x60, 0x8, 0x32, 0xac, 0xb8 } };


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
  checkResult (pDictionary->LookupClass (AUID_AAFFiller, &pFillClass));
  assert (pFillClass);

  // Create new object for our new filler class, and initialize it.
  IAAFClassDefSP pNewFillClass;
  checkResult (pDictionary->CreateInstance(AUID_AAFClassDef,
										   IID_IAAFClassDef,
										   (IUnknown **)&pNewFillClass));
  checkResult (pNewFillClass->Initialize (kClassAUID_NewFill,
										  pFillClass,
										  L"New Filler"));

  // Get type def for uint32
  IAAFTypeDefSP ptd;
  checkResult (pDictionary->LookupType (kAAFTypeID_UInt32,
										&ptd));
  assert (ptd);

  // Initialize new property
  checkResult
	(pNewFillClass->AppendNewPropertyDef (kPropAUID_NewFill_Odor,
										  L"Odor",
										  ptd,
										  AAFFalse,  // mandatory
										  0));

  // Register it in the dictionary.
  checkResult (pDictionary->RegisterClass (pNewFillClass));
}
										  


#define RegisterOneDef( \
 pDict,   /* Dictionary with which to test */ \
 soid,    /* Stored object ID for the def to test */ \
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
  hr = pDict->CreateInstance(soid, initTypeIID, (IUnknown**) &pInitIfc); \
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

  RegisterOneDef (/* dictionary*/                  pDict,
				  /* def object's SOID */          AUID_AAFTypeDefInt,
				  /* IID of def to pass to Init */ IID_IAAFTypeDefInt,
				  /* SP of def to use with Init */ IAAFTypeDefIntSP,
				  /* Init() invocation */
				  Initialize (kTestTypeID, 1, AAFFalse, L"TestUInt8"),
				  /* IID of type to QI */          IID_IAAFDefObject,
				  /* SP for type to QI */          IAAFDefObjectSP,
				  /* IID of def to register */     IID_IAAFTypeDef,
				  /* SP for def to register */     IAAFTypeDefSP,
				  /* reg method on pDict */        RegisterType);
  
  RegisterOneDef (/* dictionary*/                  pDict,
				  /* def object's SOID */          AUID_AAFDataDef,
				  /* IID of def to pass to Init */ IID_IAAFDefObject,
				  /* SP of def to use with Init */ IAAFDefObjectSP,
				  /* Init() invocation */
				  Initialize (kTestDataDefID, L"TestDataDef", L"Desc"),
				  /* IID of type to QI */          IID_IAAFDefObject,
				  /* SP for type to QI */          IAAFDefObjectSP,
				  /* IID of def to register */     IID_IAAFDataDef,
				  /* SP for def to register */     IAAFDataDefSP,
				  /* reg method on pDict */        RegisterDataDefinition);
  
  RegisterOneDef (/* dictionary*/                  pDict,
				  /* def object's SOID */          AUID_AAFOperationDef,
				  /* IID of def to pass to Init */ IID_IAAFDefObject,
				  /* SP of def to use with Init */ IAAFDefObjectSP,
				  /* Init() invocation */
				  Initialize (kTestOperationDefID, L"TestOperationDef", L"Desc"),
				  /* IID of type to QI */          IID_IAAFDefObject,
				  /* SP for type to QI */          IAAFDefObjectSP,
				  /* IID of def to register */     IID_IAAFOperationDef,
				  /* SP for def to register */     IAAFOperationDefSP,
				  /* reg method on pDict */        RegisterOperationDefinition);
  
  RegisterOneDef (/* dictionary*/                  pDict,
				  /* def object's SOID */          AUID_AAFParameterDef,
				  /* IID of def to pass to Init */ IID_IAAFDefObject,
				  /* SP of def to use with Init */ IAAFDefObjectSP,
				  /* Init() invocation */
				  Initialize (kTestParameterDefID, L"TestParameterDef", L"Desc"),
				  /* IID of type to QI */          IID_IAAFDefObject,
				  /* SP for type to QI */          IAAFDefObjectSP,
				  /* IID of def to register */     IID_IAAFParameterDef,
				  /* SP for def to register */     IAAFParameterDefSP,
				  /* reg method on pDict */        RegisterParameterDefinition);

  RegisterOneDef (/* dictionary*/                  pDict,
				  /* def object's SOID */          AUID_AAFCodecDef,
				  /* IID of def to pass to Init */ IID_IAAFDefObject,
				  /* SP of def to use with Init */ IAAFDefObjectSP,
				  /* Init() invocation */
				  Initialize (kTestCodecDefID, L"TestCodecDef", L"Desc"),
				  /* IID of type to QI */          IID_IAAFDefObject,
				  /* SP for type to QI */          IAAFDefObjectSP,
				  /* IID of def to register */     IID_IAAFCodecDef,
				  /* SP for def to register */     IAAFCodecDefSP,
				  /* reg method on pDict */        RegisterCodecDefinition);

  // Hack! Codec defs must have at least one data def appended in
  // order to be saved correctly...
  {
	IAAFCodecDefSP cd;
	checkResult (pDict->LookupCodecDefinition (kTestCodecDefID,
										&cd));
	checkResult (cd->AppendEssenceKind (DDEF_Matte));
  }  

  RegisterOneDef (/* dictionary*/                  pDict,
				  /* def object's SOID */          AUID_AAFContainerDef,
				  /* IID of def to pass to Init */ IID_IAAFDefObject,
				  /* SP of def to use with Init */ IAAFDefObjectSP,
				  /* Init() invocation */
				  Initialize (kTestContainerDefID, L"TestContainerDef", L"Desc"),
				  /* IID of type to QI */          IID_IAAFDefObject,
				  /* SP for type to QI */          IAAFDefObjectSP,
				  /* IID of def to register */     IID_IAAFContainerDef,
				  /* SP for def to register */     IAAFContainerDefSP,
				  /* reg method on pDict */        RegisterContainerDefinition);
  
  RegisterOneDef (/* dictionary*/                  pDict,
				  /* def object's SOID */          AUID_AAFInterpolationDefinition,
				  /* IID of def to pass to Init */ IID_IAAFDefObject,
				  /* SP of def to use with Init */ IAAFDefObjectSP,
				  /* Init() invocation */
				  Initialize (kTestInterpolationDefID, L"TestInterpolationDef", L"Desc"),
				  /* IID of type to QI */          IID_IAAFDefObject,
				  /* SP for type to QI */          IAAFDefObjectSP,
				  /* IID of def to register */     IID_IAAFInterpolationDef,
				  /* SP for def to register */     IAAFInterpolationDefSP,
				  /* reg method on pDict */        RegisterInterpolationDefinition);
  
  RegisterOneDef (/* dictionary*/                  pDict,
				  /* def object's SOID */          AUID_AAFPluginDescriptor,
				  /* IID of def to pass to Init */ IID_IAAFPluginDescriptor,
				  /* SP of def to use with Init */ IAAFPluginDescriptorSP,
				  /* Init() invocation */
				  Initialize (kTestPluginDescriptorID, L"TestPluginDescriptor", L"Desc"),
				  /* IID of type to QI */          IID_IAAFObject,
				  /* SP for type to QI */          IAAFObjectSP,
				  /* IID of def to register */     IID_IAAFPluginDescriptor,
				  /* SP for def to register */     IAAFPluginDescriptorSP,
				  /* reg method on pDict */        RegisterPluginDescriptor);
  
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
				/* lookup method on pDict */   LookupType,
				/* IID of def for name */      IID_IAAFDefObject,
				/* SP of def for name */       IAAFDefObjectSP,
				/* SP of enumerator */         IEnumAAFTypeDefsSP,
				/* get-enum method on pDict */ GetTypeDefinitions);

  LookupOneDef (/* dictionary */               pDict,
				/* ID of def to look up */     kTestDataDefID,
				/* def's name */               L"TestDataDef",
				/* IID of def for lookup */    IID_IAAFDataDef,
				/* SP of def to lookup */      IAAFDataDefSP,
				/* lookup method on pDict */   LookupDataDefinition,
				/* IID of def for name */      IID_IAAFDefObject,
				/* SP of def for name */       IAAFDefObjectSP,
				/* SP of enumerator */         IEnumAAFDataDefsSP,
				/* get-enum method on pDict */ GetDataDefinitions);

  LookupOneDef (/* dictionary */               pDict,
				/* ID of def to look up */     kTestOperationDefID,
				/* def's name */               L"TestOperationDef",
				/* IID of def for lookup */    IID_IAAFOperationDef,
				/* SP of def to lookup */      IAAFOperationDefSP,
				/* lookup method on pDict */   LookupOperationDefinition,
				/* IID of def for name */      IID_IAAFDefObject,
				/* SP of def for name */       IAAFDefObjectSP,
				/* SP of enumerator */         IEnumAAFOperationDefsSP,
				/* get-enum method on pDict */ GetOperationDefinitions);

  LookupOneDef (/* dictionary */               pDict,
				/* ID of def to look up */     kTestParameterDefID,
				/* def's name */               L"TestParameterDef",
				/* IID of def for lookup */    IID_IAAFParameterDef,
				/* SP of def to lookup */      IAAFParameterDefSP,
				/* lookup method on pDict */   LookupParameterDefinition,
				/* IID of def for name */      IID_IAAFDefObject,
				/* SP of def for name */       IAAFDefObjectSP,
				/* SP of enumerator */         IEnumAAFParameterDefsSP,
				/* get-enum method on pDict */ GetParameterDefinitions);

  LookupOneDef (/* dictionary */               pDict,
				/* ID of def to look up */     kTestCodecDefID,
				/* def's name */               L"TestCodecDef",
				/* IID of def for lookup */    IID_IAAFCodecDef,
				/* SP of def to lookup */      IAAFCodecDefSP,
				/* lookup method on pDict */   LookupCodecDefinition,
				/* IID of def for name */      IID_IAAFDefObject,
				/* SP of def for name */       IAAFDefObjectSP,
				/* SP of enumerator */         IEnumAAFCodecDefsSP,
				/* get-enum method on pDict */ GetCodecDefinitions);

  LookupOneDef (/* dictionary */               pDict,
				/* ID of def to look up */     kTestContainerDefID,
				/* def's name */               L"TestContainerDef",
				/* IID of def for lookup */    IID_IAAFContainerDef,
				/* SP of def to lookup */      IAAFContainerDefSP,
				/* lookup method on pDict */   LookupContainerDefinition,
				/* IID of def for name */      IID_IAAFDefObject,
				/* SP of def for name */       IAAFDefObjectSP,
				/* SP of enumerator */         IEnumAAFContainerDefsSP,
				/* get-enum method on pDict */ GetContainerDefinitions);

  LookupOneDef (/* dictionary */               pDict,
				/* ID of def to look up */     kTestInterpolationDefID,
				/* def's name */               L"TestInterpolationDef",
				/* IID of def for lookup */    IID_IAAFInterpolationDef,
				/* SP of def to lookup */      IAAFInterpolationDefSP,
				/* lookup method on pDict */   LookupInterpolationDefinition,
				/* IID of def for name */      IID_IAAFDefObject,
				/* SP of def for name */       IAAFDefObjectSP,
				/* SP of enumerator */         IEnumAAFInterpolationDefsSP,
				/* get-enum method on pDict */ GetInterpolationDefinitions);

  LookupOneDef (/* dictionary */               pDict,
				/* ID of def to look up */     kTestPluginDescriptorID,
				/* def's name */               L"TestPluginDescriptor",
				/* IID of def for lookup */    IID_IAAFPluginDescriptor,
				/* SP of def to lookup */      IAAFPluginDescriptorSP,
				/* lookup method on pDict */   LookupPluginDescriptor,
				/* IID of def for name */      IID_IAAFPluginDescriptor,
				/* SP of def for name */       IAAFPluginDescriptorSP,
				/* SP of enumerator */         IEnumAAFPluginDescriptorsSP,
				/* get-enum method on pDict */ GetPluginDescriptors);

  return AAFRESULT_SUCCESS;
}


static HRESULT OpenAAFFile(aafWChar*			pFileName,
						   aafMediaOpenMode_t	mode,
						   IAAFFile**			ppFile,
						   IAAFHeader**			ppHeader)
{
	aafProductIdentification_t	ProductInfo;
	HRESULT						hr = AAFRESULT_SUCCESS;

	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFDictionary Test";
	ProductInfo.productVersion.major = 1;
	ProductInfo.productVersion.minor = 0;
	ProductInfo.productVersion.tertiary = 0;
	ProductInfo.productVersion.patchLevel = 0;
	ProductInfo.productVersion.type = kVersionUnknown;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = UnitTestProductID;
	ProductInfo.platform = NULL;

	*ppFile = NULL;

	switch (mode)
	{
	case kMediaOpenReadOnly:
		hr = AAFFileOpenExistingRead(pFileName, 0, ppFile);
		break;

	case kMediaOpenAppend:
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
  IAAFMobSlotSP    pMobSlot;
  IAAFSequenceSP   pSequence;
  IAAFSegmentSP    pSegment;
  IAAFComponentSP  pComponent;
  aafUID_t         NewMobID;
  int              i;
  HRESULT          hr = S_OK;
	
	
  try
	{  
	  // Remove the previous test file if any.
	  RemoveTestFile(pFileName);
		
	  // Create the AAF file
	  checkResult(OpenAAFFile(pFileName, kMediaOpenAppend, &pFile, &pHeader));
		
	  // Get the AAF Dictionary so that we can create valid AAF objects.
	  checkResult(pHeader->GetDictionary(&pDictionary));
		
	  // Create a new class, and register it in the dictionary.
	  RegisterNewClass (pDictionary);

	  // Create a Composition Mob
	  checkResult(pDictionary->CreateInstance(AUID_AAFCompositionMob,
											  IID_IAAFMob, 
											  (IUnknown **)&pMob));
		
	  checkResult(CoCreateGuid((GUID *)&NewMobID));
	  checkResult(pMob->SetMobID(NewMobID));
	  checkResult(pMob->SetName(L"AAFDictionaryTest"));
		
	  // Add mob slot w/ Sequence
	  checkResult(pDictionary->CreateInstance(AUID_AAFSequence,
											  IID_IAAFSequence, 
											  (IUnknown **)&pSequence));		
	  checkResult(pSequence->Initialize(DDEF_Picture));
		
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
			  checkResult
				(pDictionary->CreateInstance(kClassAUID_NewFill,
											 IID_IAAFComponent, 
											 (IUnknown**)&pComponent));
			  checkResult(pComponent->SetDataDef(DDEF_PictureWithMatte));
			}
		  else
			{
			  checkResult
				(pDictionary->CreateInstance(AUID_AAFFiller,
											 IID_IAAFComponent, 
											 (IUnknown**)&pComponent));

			  checkResult(pComponent->SetDataDef(DDEF_Picture));
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
			  checkResult (pDictionary->LookupType (kAAFTypeID_UInt32,
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
			  checkResult (pDictionary->LookupClass (kClassAUID_NewFill,
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
		
	  checkResult(pMob->AppendNewSlot(pSegment, 1, L"AAF Test Sequence", &pMobSlot));
		
	  // Add the master mob to the file and cleanup
	  pHeader->AppendMob(pMob);
		
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
	  checkResult(OpenAAFFile(pFileName, kMediaOpenReadOnly, &pFile, &pHeader));
		
	  // Validate that there is only one composition mob.
	  checkResult(pHeader->GetNumMobs(kCompMob, &numMobs));
	  checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);
		
	  // Get the AAF Dictionary so that we can create valid AAF objects.
	  checkResult(pHeader->GetDictionary(&pDictionary));
		
	  // Check a data definition from a composition MOB in order to test weak references
	  criteria.searchTag = kByMobKind;
	  criteria.tags.mobKind = kCompMob;
	  checkResult(pHeader->EnumAAFAllMobs(&criteria, &pMobIter));
	  while (pMobIter && pMobIter->NextOne(&pMob) == AAFRESULT_SUCCESS)
		{					
		  // Enumerate the first MOB slot for this MOB
		  checkResult(pMob->EnumAAFAllMobSlots(&pSlotIter));
		  checkResult(pSlotIter->NextOne(&pSlot));

		  checkResult(pSlot->GetSegment(&pSegment));
		  checkResult(pSegment->QueryInterface(IID_IAAFSequence, (void **) &pSequence));
		  checkResult(pSequence->EnumComponents(&pCompIter));
		  checkResult(pCompIter->NextOne(&pComp));

		  aafUID_t dataDef;
		  aafBool  testBool;

		  checkResult(pComp->GetDataDef(&dataDef));
		  checkResult(pDictionary->LookupDataDefinition(dataDef, &pDataDef));
		  checkResult(pDataDef->IsSoundKind(&testBool));
		  checkExpression(testBool == AAFFalse, AAFRESULT_TEST_FAILED);

		  checkResult(pDataDef->IsDataDefOf(DDEF_PictureWithMatte, &testBool));
		  checkExpression(testBool == AAFTrue, AAFRESULT_TEST_FAILED);
		
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
		  checkResult (pDictionary->LookupClass (kClassAUID_NewFill,
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
		  checkResult (pDictionary->GetClassDefinitions (&pClassDefEnum));
		  bool found = false;
		  IAAFClassDefSP cd;
		  while (SUCCEEDED (pClassDefEnum->NextOne (&cd)))
			{
			  IAAFDefObjectSP def;
			  checkResult(cd->QueryInterface(IID_IAAFDefObject,
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
		  checkExpression(found == AAFTrue, AAFRESULT_TEST_FAILED);

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
		  checkResult (pDictionary->LookupType
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

extern "C" HRESULT CAAFDictionary_test()
{
  HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
  aafWChar * pFileName = L"AAFDictionaryTest.aaf";

  try
	{
	  hr = CreateAAFFile(pFileName);
	  if (SUCCEEDED(hr))
		hr = ReadAAFFile(pFileName);
	}
  catch (...)
	{
	  cerr << "CAAFDictionary_test...Caught general C++ exception!" << endl; 
	}

  return hr;
}

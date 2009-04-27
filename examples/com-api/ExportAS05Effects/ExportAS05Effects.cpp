// @com Executable example program by David Mansergh, Quantel.

// This program creates a new aaf file in memory, using code from ExportSimpleComposition program.

//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <wchar.h>

#include "AAFTypes.h"
#include "AAFTypeDefUIDs.h"
#include "AAFResult.h"
#include "AAFDefUIDs.h"
#include "AAFDataDefs.h"
#include "AAFOperationDefs.h"
#include "AAFParameterDefs.h"
#include "AAFContainerDefs.h"
#include "AAFCodecDefs.h"
#include "AAFEssenceFormats.h"
#include "AAFFileKinds.h"
#include "AAFExtEnum.h"
#include "AAFOPDefs.h"

// Include the AAF interface declarations.
#include "AAF.h"

// Include the AAF Stored Object identifiers. These symbols are defined in aaf.lib.
#include "AAFStoredObjectIDs.h"

static void LogError(HRESULT errcode, int line, const char *file)
{
  printf("Error '%0x' returned at line %d in %s\n", errcode, line, file);
}

static HRESULT moduleErrorTmp = S_OK; /* note usage in macro */

#define check(a)  \
{ moduleErrorTmp = a; \
  if (!SUCCEEDED(moduleErrorTmp)) \
  { \
      LogError(moduleErrorTmp, __LINE__, __FILE__);\
    goto cleanup; \
  } \
}

#define checkFatal(a)  \
{ moduleErrorTmp = a; \
  if (!SUCCEEDED(moduleErrorTmp)) \
     exit(1);\
}

const aafUID_t NIL_UID = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };
static aafCharacter companyName[] = L"AMW Association";
static aafCharacter productName[] = L"ExportAS05Effects";

static HRESULT CreateAAFFile(const char* pFileName, IAAFFile** ppFile)
{
  // Delete any previous test file before continuing...
  remove(pFileName);
  aafWChar wFileName[FILENAME_MAX];
  mbstowcs(wFileName,pFileName,FILENAME_MAX);

  aafProductVersion_t v;
  v.major = 1;
  v.minor = 0;
  v.tertiary = 0;
  v.patchLevel = 0;
  v.type = kAAFVersionUnknown;

  aafProductIdentification_t  ProductInfo;
  ProductInfo.companyName = companyName;
  ProductInfo.productName = productName;
  ProductInfo.productVersion = &v;
  ProductInfo.productVersionString = 0;
  ProductInfo.productID = NIL_UID;
  ProductInfo.platform = 0;

  /* Create a new AAF file IN MEMORY */
  check(AAFFileOpenNewModifyEx (wFileName, &kAAFFileKind_Aaf4KBinary, 0, &ProductInfo, ppFile));
 
cleanup:
  // Cleanup and return

  return moduleErrorTmp;
}

static HRESULT CreateSourceMob(IAAFFile* pFile, IAAFHeader* pHeader, IAAFDictionary* pDictionary,
                               IAAFDataDef* pDataDef, aafRational_t rate, IAAFSourceMob** ppSourceMob)
{
  IAAFSourceMob* pSourceMob = 0;
  IAAFMob* pMobSource = 0;
  IAAFTapeDescriptor* pTapeDescriptor = 0;
  IAAFEssenceDescriptor* pEssenceDescriptor = 0;
  IAAFTimelineMobSlot* pTimelineMobSlot = 0;
  IAAFSourceClip* pSourceClip = 0;
  IAAFSegment* pSegment = 0;
  IAAFMobSlot* pMobSlot = 0;

  IAAFTimelineMobSlot* pTcTimelineMobSlot = 0;
  IAAFMobSlot* pTcMobSlot = 0;
  IAAFTimecode* pTimecode = 0;
  IAAFDataDef* pTimecodeDataDef = 0;
  IAAFComponent* pTcComponent = 0;
  IAAFSegment* pTcSegment = 0;

  aafSlotID_t sourceSlotID = 1;
  aafUInt32 physicalNum = 1; // count up from 1 for each slot type

  aafLength_t length = 100;

  aafSourceRef_t srcRef;
  memset(&srcRef, 0, sizeof(srcRef));

  aafSlotID_t timecodeSlotID = 2;
  aafUInt32 timecodePhysicalNum = 1; // count up from 1 for each slot type

  // Source mob
  check(pDictionary->CreateInstance(AUID_AAFSourceMob, IID_IAAFSourceMob, (IUnknown **)ppSourceMob));
  pSourceMob = *ppSourceMob;

  check(pSourceMob->Initialize());
  
  check(pSourceMob->QueryInterface(IID_IAAFMob, (void **)&pMobSource));
  check(pMobSource->SetName(L"source"));
  
  check(pDictionary->CreateInstance(AUID_AAFTapeDescriptor, IID_IAAFTapeDescriptor, (IUnknown **)&pTapeDescriptor));
  check(pTapeDescriptor->Initialize());

  check(pTapeDescriptor->QueryInterface(IID_IAAFEssenceDescriptor, (void **)&pEssenceDescriptor));
  check(pSourceMob->SetEssenceDescriptor(pEssenceDescriptor));
  
  check(pHeader->AddMob(pMobSource));

  // Create mob slots
  check(pDictionary->CreateInstance(AUID_AAFTimelineMobSlot, IID_IAAFTimelineMobSlot, (IUnknown **)&pTimelineMobSlot));
  check(pTimelineMobSlot->Initialize());
  check(pTimelineMobSlot->SetEditRate(rate));
  check(pTimelineMobSlot->SetOrigin(0));

  check(pDictionary->CreateInstance(AUID_AAFSourceClip, IID_IAAFSourceClip, (IUnknown **)&pSourceClip));
  check(pSourceClip->Initialize(pDataDef, length, srcRef));

  check(pSourceClip->QueryInterface(IID_IAAFSegment, (void **)&pSegment));

  check(pTimelineMobSlot->QueryInterface(IID_IAAFMobSlot, (void **)&pMobSlot));
  check(pMobSlot->SetSegment(pSegment));
  check(pMobSlot->SetPhysicalNum(physicalNum));
  check(pMobSlot->SetName(L"Video"));
  check(pMobSlot->SetSlotID(sourceSlotID));

  check(pMobSource->AppendSlot(pMobSlot));

  // Create timecode slot
  check(pDictionary->CreateInstance(AUID_AAFTimelineMobSlot, IID_IAAFTimelineMobSlot, (IUnknown **)&pTcTimelineMobSlot));
  check(pTcTimelineMobSlot->Initialize());
  check(pTcTimelineMobSlot->SetEditRate(rate));
  check(pTcTimelineMobSlot->SetOrigin(0));

  check(pDictionary->CreateInstance(AUID_AAFTimecode, IID_IAAFTimecode, (IUnknown **)&pTimecode));

  aafTimecode_t tc;
  tc.startFrame = 0;
  tc.drop = 0;
  tc.fps = 25;
  check(pTimecode->Initialize(length, &tc));

  check(pDictionary->LookupDataDef(kAAFDataDef_Timecode, &pTimecodeDataDef));

  check(pTimecode->QueryInterface(IID_IAAFComponent, (void **)&pTcComponent));
  check(pTcComponent->SetDataDef(pTimecodeDataDef)); // force to non-legacy timecode data def

  check(pTimecode->QueryInterface(IID_IAAFSegment, (void **)&pTcSegment));

  check(pTcTimelineMobSlot->QueryInterface(IID_IAAFMobSlot, (void **)&pTcMobSlot));
  check(pTcMobSlot->SetSegment(pTcSegment));
  check(pTcMobSlot->SetPhysicalNum(timecodePhysicalNum));
  check(pTcMobSlot->SetName(L"Timecode"));
  check(pTcMobSlot->SetSlotID(timecodeSlotID));

  check(pMobSource->AppendSlot(pTcMobSlot));

cleanup:
  // Cleanup and return

  if (pTcSegment)
    pTcSegment->Release();

  if (pTcComponent)
    pTcComponent->Release();

  if (pTimecodeDataDef)
    pTimecodeDataDef->Release();

  if (pTimecode)
    pTimecode->Release();

  if (pTcMobSlot)
    pTcMobSlot->Release();

  if (pTcTimelineMobSlot)
    pTcTimelineMobSlot->Release();

  if (pMobSlot)
    pMobSlot->Release();

  if (pSegment)
    pSegment->Release();

  if (pTimelineMobSlot)
    pTimelineMobSlot->Release();

  if (pEssenceDescriptor)
    pEssenceDescriptor->Release();

  if (pTapeDescriptor)
    pTapeDescriptor->Release();

  if (pMobSource)
    pMobSource->Release();

  return moduleErrorTmp;
}

static HRESULT CreateMasterMob(IAAFFile* pFile, IAAFHeader* pHeader, IAAFDictionary* pDictionary,
                               IAAFDataDef* pDataDef, aafRational_t rate,
                               IAAFSourceMob* pSourceMob, IAAFMasterMob** ppMasterMob)
{
  IAAFMasterMob* pMasterMob = 0;
  IAAFMob* pMobMaster = 0;
  IAAFMobSlot* pMobSlot = 0;

  aafSlotID_t sourceSlotID = 1;
  aafSlotID_t masterSlotID = 1;
  aafUInt32 physicalNum = 1; // count up from 1 for each slot type

  // Master mob
  check(pDictionary->CreateInstance(AUID_AAFMasterMob, IID_IAAFMasterMob, (IUnknown **)ppMasterMob));
  pMasterMob = *ppMasterMob;

  check(pMasterMob->Initialize());
  
  check(pMasterMob->AddMasterSlot(pDataDef, sourceSlotID, pSourceMob, masterSlotID, L""));

  check(pMasterMob->QueryInterface(IID_IAAFMob, (void **)&pMobMaster));
  check(pMobMaster->SetName(L"Master"));

  check(pMobMaster->LookupSlot(masterSlotID, &pMobSlot));
  check(pMobSlot->SetPhysicalNum(physicalNum));

  check(pHeader->AddMob(pMobMaster));

cleanup:
  // Cleanup and return

  if (pMobSlot)
    pMobSlot->Release();

  if (pMobMaster)
    pMobMaster->Release();

  return moduleErrorTmp;
}

static HRESULT CreateSourceSegment(IAAFDictionary* pDictionary, IAAFDataDef* pDataDef,
                                   IAAFMob* pMasterMob, aafSlotID_t masterSlotID,
                                   aafLength_t length, IAAFSegment** ppSegment)
{
  IAAFSourceClip* pSourceClip = 0;
  check(pDictionary->CreateInstance(AUID_AAFSourceClip, IID_IAAFSourceClip, (IUnknown **)&pSourceClip));

  aafSourceRef_t srcRef;
  pMasterMob->GetMobID(&srcRef.sourceID);
  srcRef.sourceSlotID = masterSlotID;
  srcRef.startTime = 0;

  check(pSourceClip->Initialize(pDataDef, length, srcRef));

  check(pSourceClip->QueryInterface(IID_IAAFSegment, (void **)ppSegment));

cleanup:
  // Cleanup and return

  if (pSourceClip)
    pSourceClip->Release();

  return moduleErrorTmp;
}

static HRESULT GetParameterDef(IAAFDictionary* pDictionary, IAAFParameterDef** ppParamDef,
                               const aafUID_t& typeUid, const aafUID_t& paramUid,
                               const aafWChar* name, const aafWChar* description)
{
  IAAFTypeDef* pTypeDef = 0;
  IAAFParameterDef* pParamDef = 0;

  if (FAILED(pDictionary->LookupParameterDef(paramUid, ppParamDef)))
  {
    check(pDictionary->CreateInstance(AUID_AAFParameterDef, IID_IAAFParameterDef, (IUnknown **) &pParamDef));

    check(pDictionary->LookupTypeDef(typeUid, &pTypeDef));
    check(pParamDef->Initialize(paramUid, name, description, pTypeDef));

    check(pDictionary->RegisterParameterDef(pParamDef));
  }
  check(pDictionary->LookupParameterDef(paramUid, ppParamDef));

cleanup:
  // Cleanup and return

  if (pParamDef)
    pParamDef->Release();

  if (pTypeDef)
    pTypeDef->Release();

  return moduleErrorTmp;
}

static HRESULT AddParameterDef(IAAFDictionary* pDictionary, IAAFOperationDef* pOperationDef,
                               const aafUID_t& typeUid, const aafUID_t& paramUid,
                               const aafWChar* name, const aafWChar* description)
{
  IAAFParameterDef* pParameterDef = 0;

  check(GetParameterDef(pDictionary, &pParameterDef, typeUid, paramUid, name, description));

  check(pOperationDef->AddParameterDef(pParameterDef));

cleanup:
  // Cleanup and return

  if (pParameterDef)
    pParameterDef->Release();

  return moduleErrorTmp;
}

static HRESULT AddParameterValue(IAAFDictionary* pDictionary, IAAFOperationGroup* pOperationGroup,
                                 const aafUID_t& typeUid, const aafUID_t& paramUid,
                                 aafUInt32 size, aafDataBuffer_t value)
{
  IAAFConstantValue* pConstantValue = 0; 
  IAAFParameterDef* pParameterDef = 0;
  IAAFParameter* pParameter = 0;

  check(pDictionary->CreateInstance(AUID_AAFConstantValue, IID_IAAFConstantValue, (IUnknown **)&pConstantValue));

  check(GetParameterDef(pDictionary, &pParameterDef, typeUid, paramUid, L"", L""));

  check(pConstantValue->Initialize(pParameterDef, size, value));

  check(pConstantValue->QueryInterface(IID_IAAFParameter, (void **)&pParameter));
  check(pOperationGroup->AddParameter(pParameter));

cleanup:
  // Cleanup and return

  if (pParameter)
    pParameter->Release();

  if (pParameterDef)
    pParameterDef->Release();

  if (pConstantValue)
    pConstantValue->Release();

  return moduleErrorTmp;
}

static HRESULT GetColorOperation(IAAFDictionary* pDictionary, IAAFDataDef* pDataDef,
                                 const aafUID_t& uid, IAAFOperationDef** ppOpDef)
{
  IAAFOperationDef* pOperationDef = 0;

  if (FAILED(pDictionary->LookupOperationDef(uid, ppOpDef)))
  {
    check(pDictionary->CreateInstance(AUID_AAFOperationDef, IID_IAAFOperationDef, (IUnknown **) &pOperationDef));

    check(pOperationDef->Initialize(uid, L"Video Color", L"CDL Color Effect"));
    check(pOperationDef->SetDataDef(pDataDef));
    //check(pOperationDef->SetCategory(???));
    check(pOperationDef->SetNumberInputs(1));
    check(pOperationDef->SetIsTimeWarp((aafBool)0));
    
    check(AddParameterDef(pDictionary, pOperationDef, kAAFTypeID_Rational,
      kAAFParameterDef_ColorSlopeR, L"Slope R", L"Color Slope Red"));
    check(AddParameterDef(pDictionary, pOperationDef, kAAFTypeID_Rational,
      kAAFParameterDef_ColorSlopeG, L"Slope G", L"Color Slope Green"));
    check(AddParameterDef(pDictionary, pOperationDef, kAAFTypeID_Rational,
      kAAFParameterDef_ColorSlopeB, L"Slope B", L"Color Slope Blue"));
    check(AddParameterDef(pDictionary, pOperationDef, kAAFTypeID_Rational,
      kAAFParameterDef_ColorOffsetR, L"Offset R", L"Color Offset Red"));
    check(AddParameterDef(pDictionary, pOperationDef, kAAFTypeID_Rational,
      kAAFParameterDef_ColorOffsetG, L"Offset G", L"Color Offset Green"));
    check(AddParameterDef(pDictionary, pOperationDef, kAAFTypeID_Rational,
      kAAFParameterDef_ColorOffsetB, L"Offset B", L"Color Offset Blue"));
    check(AddParameterDef(pDictionary, pOperationDef, kAAFTypeID_Rational,
      kAAFParameterDef_ColorPowerR, L"Power R", L"Color Power Red"));
    check(AddParameterDef(pDictionary, pOperationDef, kAAFTypeID_Rational,
      kAAFParameterDef_ColorPowerG, L"Power G", L"Color Power Green"));
    check(AddParameterDef(pDictionary, pOperationDef, kAAFTypeID_Rational,
      kAAFParameterDef_ColorPowerB, L"Power B", L"Color Power Blue"));
    check(AddParameterDef(pDictionary, pOperationDef, kAAFTypeID_Rational,
      kAAFParameterDef_ColorSaturation, L"Saturation", L"Color Saturation"));
    check(AddParameterDef(pDictionary, pOperationDef, kAAFTypeID_String,
      kAAFParameterDef_ColorCorrectionDescription, L"Correction Description", L"Color Correction Description"));
    check(AddParameterDef(pDictionary, pOperationDef, kAAFTypeID_String,
      kAAFParameterDef_ColorInputDescription, L"Input Description", L"Color Input Description"));
    check(AddParameterDef(pDictionary, pOperationDef, kAAFTypeID_String,
      kAAFParameterDef_ColorViewingDescription, L"Viewing Description", L"Color Viewing Description"));

    check(pDictionary->RegisterOperationDef(pOperationDef));
  }
  check(pDictionary->LookupOperationDef(uid, ppOpDef));

cleanup:
  // Cleanup and return

  if (pOperationDef)
    pOperationDef->Release();

  return moduleErrorTmp;
}

static HRESULT CreateColorOperation(IAAFDictionary* pDictionary, IAAFDataDef* pDataDef,
                                    IAAFMob* pMasterMob, aafSlotID_t masterSlotId,
                                    IAAFSequence* pSequence)
{
  IAAFOperationDef* pOperationDef = 0;
  IAAFOperationGroup* pOperationGroup = 0;
  IAAFSegment* pSourceSegment = 0;
  IAAFComponent* pComponent = 0;

  aafLength_t length = 100;
  aafRational_t slopeR = {1,1};
  aafRational_t slopeG = {1,1};
  aafRational_t slopeB = {1,1};
  aafRational_t offsetR = {0,1};
  aafRational_t offsetG = {0,1};
  aafRational_t offsetB = {0,1};
  aafRational_t powerR = {1,1};
  aafRational_t powerG = {1,1};
  aafRational_t powerB = {1,1};
  aafRational_t saturation = {1,1};
  const aafWChar* correctionDescription = L"Null color correction";
  const aafWChar* inputDescription = L"Something about the input format";
  const aafWChar* viewingDescription = L"Something about the viewing conditions";

  check(GetColorOperation(pDictionary, pDataDef, kAAFOperationDef_VideoColor, &pOperationDef));

  check(pDictionary->CreateInstance(AUID_AAFOperationGroup, IID_IAAFOperationGroup, (IUnknown **)&pOperationGroup));
  check(pOperationGroup->Initialize(pDataDef, length, pOperationDef));

  check(AddParameterValue(pDictionary, pOperationGroup, kAAFTypeID_Rational,
    kAAFParameterDef_ColorSlopeR, sizeof(slopeR), (aafDataBuffer_t)&slopeR));
  check(AddParameterValue(pDictionary, pOperationGroup, kAAFTypeID_Rational,
    kAAFParameterDef_ColorSlopeG, sizeof(slopeG), (aafDataBuffer_t)&slopeG));
  check(AddParameterValue(pDictionary, pOperationGroup, kAAFTypeID_Rational,
    kAAFParameterDef_ColorSlopeB, sizeof(slopeB), (aafDataBuffer_t)&slopeB));

  check(AddParameterValue(pDictionary, pOperationGroup, kAAFTypeID_Rational,
    kAAFParameterDef_ColorOffsetR, sizeof(offsetR), (aafDataBuffer_t)&offsetR));
  check(AddParameterValue(pDictionary, pOperationGroup, kAAFTypeID_Rational,
    kAAFParameterDef_ColorOffsetG, sizeof(offsetG), (aafDataBuffer_t)&offsetG));
  check(AddParameterValue(pDictionary, pOperationGroup, kAAFTypeID_Rational,
    kAAFParameterDef_ColorOffsetB, sizeof(offsetB), (aafDataBuffer_t)&offsetB));

  check(AddParameterValue(pDictionary, pOperationGroup, kAAFTypeID_Rational,
    kAAFParameterDef_ColorPowerR, sizeof(powerR), (aafDataBuffer_t)&powerR));
  check(AddParameterValue(pDictionary, pOperationGroup, kAAFTypeID_Rational,
    kAAFParameterDef_ColorPowerG, sizeof(powerG), (aafDataBuffer_t)&powerG));
  check(AddParameterValue(pDictionary, pOperationGroup, kAAFTypeID_Rational,
    kAAFParameterDef_ColorPowerB, sizeof(powerB), (aafDataBuffer_t)&powerB));

  check(AddParameterValue(pDictionary, pOperationGroup, kAAFTypeID_Rational,
    kAAFParameterDef_ColorSaturation, sizeof(saturation), (aafDataBuffer_t)&saturation));

  check(AddParameterValue(pDictionary, pOperationGroup, kAAFTypeID_String,
    kAAFParameterDef_ColorCorrectionDescription,
    (wcslen(correctionDescription) + 1) * sizeof(aafCharacter),
    (aafDataBuffer_t)correctionDescription));
  check(AddParameterValue(pDictionary, pOperationGroup, kAAFTypeID_String,
    kAAFParameterDef_ColorInputDescription,
    (wcslen(inputDescription) + 1) * sizeof(aafCharacter),
    (aafDataBuffer_t)inputDescription));
  check(AddParameterValue(pDictionary, pOperationGroup, kAAFTypeID_String,
    kAAFParameterDef_ColorViewingDescription,
    (wcslen(viewingDescription) + 1) * sizeof(aafCharacter),
    (aafDataBuffer_t)viewingDescription));

  check(CreateSourceSegment(pDictionary, pDataDef, pMasterMob, masterSlotId, length, &pSourceSegment));

  check(pOperationGroup->AppendInputSegment(pSourceSegment));

  check(pOperationGroup->QueryInterface(IID_IAAFComponent, (void **)&pComponent));

  check(pSequence->AppendComponent(pComponent));

cleanup:
  // Cleanup and return

  if (pComponent)
    pComponent->Release();

  if (pSourceSegment)
    pSourceSegment->Release();

  if (pOperationGroup)
    pOperationGroup->Release();

  if (pOperationDef)
    pOperationDef->Release();

  return moduleErrorTmp;
}

static HRESULT CreateAlignmentTypeDef(IAAFDictionary* pDictionary)
{
  IAAFTypeDef* pTypeDef = 0;
  IAAFTypeDefEnum* pTypeDefEnum = 0;
  IAAFTypeDef* pTypeDefUInt8 = 0;

  if (FAILED(pDictionary->LookupTypeDef(kAAFTypeID_TitleAlignmentType, &pTypeDef)))
  {
    check(pDictionary->CreateMetaInstance(AUID_AAFTypeDefEnum, IID_IAAFTypeDefEnum, (IUnknown **)&pTypeDefEnum));

    check(pDictionary->LookupTypeDef(kAAFTypeID_UInt8, &pTypeDefUInt8));

    const int kNumValues = 3;

    aafInt64 values[kNumValues];
    values[0] = 0;
    values[1] = 1;
    values[2] = 2;

    aafString_t names[kNumValues];
    names[0] = const_cast<aafCharacter*>(L"Left");
    names[1] = const_cast<aafCharacter*>(L"Center");
    names[2] = const_cast<aafCharacter*>(L"Right");

    check(pTypeDefEnum->Initialize(kAAFTypeID_TitleAlignmentType, pTypeDefUInt8, values, names, kNumValues, L"Title Alignment"));

    check(pTypeDefEnum->QueryInterface(IID_IAAFTypeDef, (void **)&pTypeDef));

    check(pDictionary->RegisterTypeDef(pTypeDef));
  }
  check(pDictionary->LookupTypeDef(kAAFTypeID_TitleAlignmentType, &pTypeDef));

cleanup:
  // Cleanup and return

  if (pTypeDefUInt8)
    pTypeDefUInt8->Release();

  if (pTypeDefEnum)
    pTypeDefEnum->Release();

  if (pTypeDef)
    pTypeDef->Release();

  return moduleErrorTmp;
}

static HRESULT GetTitleOperation(IAAFDictionary* pDictionary, IAAFDataDef* pDataDef,
                                 const aafUID_t& uid, IAAFOperationDef** ppOpDef)
{
  IAAFOperationDef* pOperationDef = 0;

  if (FAILED(pDictionary->LookupOperationDef(uid, ppOpDef)))
  {
    check(pDictionary->CreateInstance(AUID_AAFOperationDef, IID_IAAFOperationDef, (IUnknown **)&pOperationDef));

    check(pOperationDef->Initialize(uid, L"Video Title", L"Video Title Effect"));
    check(pOperationDef->SetDataDef(pDataDef));
    //check(pOperationDef->SetCategory(???));
    check(pOperationDef->SetNumberInputs(1));
    check(pOperationDef->SetIsTimeWarp((aafBool)0));

    check(CreateAlignmentTypeDef(pDictionary));

    check(AddParameterDef(pDictionary, pOperationDef, kAAFTypeID_String,
      kAAFParameterDef_TitleText, L"Text", L"Text String"));
    check(AddParameterDef(pDictionary, pOperationDef, kAAFTypeID_String,
      kAAFParameterDef_TitleFontName, L"Font Name", L"Font Name"));
    check(AddParameterDef(pDictionary, pOperationDef, kAAFTypeID_Rational,
      kAAFParameterDef_TitleFontSize, L"Font Size", L"Font Size"));
    check(AddParameterDef(pDictionary, pOperationDef, kAAFTypeID_Rational,
      kAAFParameterDef_TitleFontColorR, L"Font Color R", L"Font Color Red"));
    check(AddParameterDef(pDictionary, pOperationDef, kAAFTypeID_Rational,
      kAAFParameterDef_TitleFontColorG, L"Font Color G", L"Font Color Green"));
    check(AddParameterDef(pDictionary, pOperationDef, kAAFTypeID_Rational,
      kAAFParameterDef_TitleFontColorB, L"Font Color B", L"Font Color Blue"));
    check(AddParameterDef(pDictionary, pOperationDef, kAAFTypeID_TitleAlignmentType,
      kAAFParameterDef_TitleAlignment, L"Alignment", L"Horizontal Alignment"));
    check(AddParameterDef(pDictionary, pOperationDef, kAAFTypeID_Boolean,
      kAAFParameterDef_TitleBold, L"Bold", L"Bold"));
    check(AddParameterDef(pDictionary, pOperationDef, kAAFTypeID_Boolean,
      kAAFParameterDef_TitleItalic, L"Italic", L"Italic"));
    check(AddParameterDef(pDictionary, pOperationDef, kAAFTypeID_Rational,
      kAAFParameterDef_TitlePositionX, L"Position X", L"Position X"));
    check(AddParameterDef(pDictionary, pOperationDef, kAAFTypeID_Rational,
      kAAFParameterDef_TitlePositionY, L"Position Y", L"Position Y"));

    check(pDictionary->RegisterOperationDef(pOperationDef));
  }
  check(pDictionary->LookupOperationDef(uid, ppOpDef));

cleanup:
  // Cleanup and return

  if (pOperationDef)
    pOperationDef->Release();

  return moduleErrorTmp;
}

static HRESULT CreateTitleOperation(IAAFDictionary* pDictionary, IAAFDataDef* pDataDef,
                                    IAAFMob* pMasterMob, aafSlotID_t masterSlotId,
                                    IAAFSequence* pSequence)
{
  IAAFOperationDef* pOperationDef = 0;
  IAAFOperationGroup* pOperationGroup = 0;
  IAAFSegment* pSourceSegment = 0;
  IAAFComponent* pComponent = 0;

  aafLength_t length = 100;
  const aafWChar* text = L"Text";
  const aafWChar* fontName = L"Arial";
  aafRational_t fontSize = {1,10};
  aafRational_t fontColorR = {1,1};
  aafRational_t fontColorG = {0,1};
  aafRational_t fontColorB = {0,1};
  unsigned char alignment = 1; // center
  aafBoolean_t bold = true;
  aafBoolean_t italic = true;
  aafRational_t positionX = {3,10};
  aafRational_t positionY = {9,10};

  check(GetTitleOperation(pDictionary, pDataDef, kAAFOperationDef_VideoTitle, &pOperationDef));

  check(pDictionary->CreateInstance(AUID_AAFOperationGroup, IID_IAAFOperationGroup, (IUnknown **)&pOperationGroup));
  check(pOperationGroup->Initialize(pDataDef, length, pOperationDef));

  check(AddParameterValue(pDictionary, pOperationGroup, kAAFTypeID_String,
    kAAFParameterDef_TitleText, (wcslen(text) + 1) * sizeof(aafCharacter),
    (aafDataBuffer_t)text));
  check(AddParameterValue(pDictionary, pOperationGroup, kAAFTypeID_String,
    kAAFParameterDef_TitleFontName, (wcslen(fontName) + 1) * sizeof(aafCharacter),
    (aafDataBuffer_t)fontName));
  check(AddParameterValue(pDictionary, pOperationGroup, kAAFTypeID_Rational,
    kAAFParameterDef_TitleFontSize, sizeof(fontSize), (aafDataBuffer_t)&fontSize));
  check(AddParameterValue(pDictionary, pOperationGroup, kAAFTypeID_Rational,
    kAAFParameterDef_TitleFontColorR, sizeof(fontColorR), (aafDataBuffer_t)&fontColorR));
  check(AddParameterValue(pDictionary, pOperationGroup, kAAFTypeID_Rational,
    kAAFParameterDef_TitleFontColorG, sizeof(fontColorG), (aafDataBuffer_t)&fontColorG));
  check(AddParameterValue(pDictionary, pOperationGroup, kAAFTypeID_Rational,
    kAAFParameterDef_TitleFontColorB, sizeof(fontColorB), (aafDataBuffer_t)&fontColorB));
  check(AddParameterValue(pDictionary, pOperationGroup, kAAFTypeID_TitleAlignmentType,
    kAAFParameterDef_TitleAlignment, sizeof(alignment), (aafDataBuffer_t)&alignment));
  check(AddParameterValue(pDictionary, pOperationGroup, kAAFTypeID_Boolean,
    kAAFParameterDef_TitleBold, sizeof(bold), (aafDataBuffer_t)&bold));
  check(AddParameterValue(pDictionary, pOperationGroup, kAAFTypeID_Boolean,
    kAAFParameterDef_TitleItalic, sizeof(italic), (aafDataBuffer_t)&italic));
  check(AddParameterValue(pDictionary, pOperationGroup, kAAFTypeID_Rational,
    kAAFParameterDef_TitlePositionX, sizeof(positionX), (aafDataBuffer_t)&positionX));
  check(AddParameterValue(pDictionary, pOperationGroup, kAAFTypeID_Rational,
    kAAFParameterDef_TitlePositionY, sizeof(positionY), (aafDataBuffer_t)&positionY));

  check(CreateSourceSegment(pDictionary, pDataDef, pMasterMob, masterSlotId, length, &pSourceSegment));

  check(pOperationGroup->AppendInputSegment(pSourceSegment));

  check(pOperationGroup->QueryInterface(IID_IAAFComponent, (void **)&pComponent));

  check(pSequence->AppendComponent(pComponent));

cleanup:
  // Cleanup and return

  if (pComponent)
    pComponent->Release();

  if (pSourceSegment)
    pSourceSegment->Release();

  if (pOperationGroup)
    pOperationGroup->Release();

  if (pOperationDef)
    pOperationDef->Release();

  return moduleErrorTmp;
}

static HRESULT GetOpacityOperation(IAAFDictionary* pDictionary, IAAFDataDef* pDataDef,
                                   const aafUID_t& uid, IAAFOperationDef** ppOpDef)
{
  IAAFOperationDef* pOperationDef = 0;

  if (FAILED(pDictionary->LookupOperationDef(uid, ppOpDef)))
  {
    check(pDictionary->CreateInstance(AUID_AAFOperationDef, IID_IAAFOperationDef, (IUnknown **)&pOperationDef));

    check(pOperationDef->Initialize(uid, L"Video Opacity", L"Video Opacity Effect"));
    check(pOperationDef->SetDataDef(pDataDef));
    //check(pOperationDef->SetCategory(???));
    check(pOperationDef->SetNumberInputs(1));
    check(pOperationDef->SetIsTimeWarp((aafBool)0));

    check(AddParameterDef(pDictionary, pOperationDef, kAAFTypeID_Rational,
      kAAFParameterDef_OpacityLevel, L"Opacity Level", L"Opacity Level"));

    check(pDictionary->RegisterOperationDef(pOperationDef));
  }
  check(pDictionary->LookupOperationDef(uid, ppOpDef));

cleanup:
  // Cleanup and return

  if (pOperationDef)
    pOperationDef->Release();

  return moduleErrorTmp;
}

static HRESULT CreateOpacityOperation(IAAFDictionary* pDictionary, IAAFDataDef* pDataDef,
                                      IAAFMob* pMasterMob, aafSlotID_t masterSlotId,
                                      IAAFSequence* pSequence)
{
  IAAFOperationDef* pOperationDef = 0;
  IAAFOperationGroup* pOperationGroup = 0;
  IAAFSegment* pSourceSegment = 0;
  IAAFComponent* pComponent = 0;

  aafLength_t length = 100;
  aafRational_t opacity = {1,1};

  check(GetOpacityOperation(pDictionary, pDataDef, kAAFOperationDef_VideoOpacity, &pOperationDef));

  check(pDictionary->CreateInstance(AUID_AAFOperationGroup, IID_IAAFOperationGroup, (IUnknown **)&pOperationGroup));
  check(pOperationGroup->Initialize(pDataDef, length, pOperationDef));

  check(AddParameterValue(pDictionary, pOperationGroup, kAAFTypeID_Rational,
    kAAFParameterDef_OpacityLevel, sizeof(opacity), (aafDataBuffer_t)&opacity));

  check(CreateSourceSegment(pDictionary, pDataDef, pMasterMob, masterSlotId, length, &pSourceSegment));

  check(pOperationGroup->AppendInputSegment(pSourceSegment));

  check(pOperationGroup->QueryInterface(IID_IAAFComponent, (void **)&pComponent));

  check(pSequence->AppendComponent(pComponent));

cleanup:
  // Cleanup and return

  if (pComponent)
    pComponent->Release();

  if (pSourceSegment)
    pSourceSegment->Release();

  if (pOperationGroup)
    pOperationGroup->Release();

  if (pOperationDef)
    pOperationDef->Release();

  return moduleErrorTmp;
}

static HRESULT CreateOperations(IAAFFile* pFile, IAAFHeader* pHeader, IAAFDictionary* pDictionary,
                                IAAFDataDef* pDataDef, IAAFMasterMob* pMasterMob, aafSlotID_t masterSlotID,
                                IAAFSequence* pSequence)
{
  IAAFMob* pMobMaster = 0;
  check(pMasterMob->QueryInterface(IID_IAAFMob, (void **)&pMobMaster));

  check(CreateColorOperation(pDictionary, pDataDef, pMobMaster, masterSlotID, pSequence));
  check(CreateTitleOperation(pDictionary, pDataDef, pMobMaster, masterSlotID, pSequence));
  check(CreateOpacityOperation(pDictionary, pDataDef, pMobMaster, masterSlotID, pSequence));

cleanup:
  // Cleanup and return

  if (pMobMaster)
    pMobMaster->Release();

  return moduleErrorTmp;
}

static HRESULT CreateCompositionMob(IAAFFile* pFile, IAAFHeader* pHeader, IAAFDictionary* pDictionary,
                                    IAAFDataDef* pDataDef, aafRational_t rate,
                                    IAAFMasterMob* pMasterMob)
{
  IAAFCompositionMob* pCompositionMob = 0;
  IAAFMob2* pMobComp2 = 0;
  IAAFMob* pMobComp = 0;
  IAAFSequence* pSequence = 0;
  IAAFSegment* pSegment = 0;
  IAAFTimelineMobSlot* pTimelineMobSlot = 0;
  IAAFMobSlot* pMobSlot = 0;

  aafSlotID_t compositionSlotID = 1;
  aafSlotID_t masterSlotID = 1;
  aafUInt32 physicalNum = 1; // count up from 1 for each slot type
  
  // Composition mob
  check(pDictionary->CreateInstance(AUID_AAFCompositionMob, IID_IAAFCompositionMob, (IUnknown **)&pCompositionMob));
  check(pCompositionMob->Initialize(L"Compositiion"));

  check(pCompositionMob->QueryInterface(IID_IAAFMob2, (void **)&pMobComp2));
  check(pMobComp2->SetUsageCode(kAAFUsage_TopLevel));

  check(pCompositionMob->QueryInterface(IID_IAAFMob, (void **)&pMobComp));
  check(pHeader->AddMob(pMobComp));

  // Create sequence
  check(pDictionary->CreateInstance(AUID_AAFSequence, IID_IAAFSequence, (IUnknown **)&pSequence));

  check(pSequence->Initialize(pDataDef));

  check(pSequence->QueryInterface(IID_IAAFSegment, (void **)&pSegment));
  check(pMobComp->AppendNewTimelineSlot(rate, pSegment, compositionSlotID, L"", 0, &pTimelineMobSlot));

  check(pTimelineMobSlot->QueryInterface(IID_IAAFMobSlot, (void **)&pMobSlot));
  check(pMobSlot->SetPhysicalNum(physicalNum));

  check(CreateOperations(pFile, pHeader, pDictionary, pDataDef, pMasterMob, masterSlotID, pSequence));

cleanup:
  // Cleanup and return

  if (pMobSlot)
    pMobSlot->Release();

  if (pTimelineMobSlot)
    pTimelineMobSlot->Release();

  if (pSegment)
    pSegment->Release();

  if (pSequence)
    pSequence->Release();

  if (pMobComp)
    pMobComp->Release();

  if (pMobComp2)
    pMobComp2->Release();

  if (pCompositionMob)
    pCompositionMob->Release();

  return moduleErrorTmp;
}

static HRESULT MakeAAFFile(const char* pFileName)
{
  printf("Creating file %s\n", pFileName);

  IAAFFile* pFile = 0;
  IAAFHeader* pHeader = 0;
  IAAFHeader2* pHeader2 = 0;
  IAAFDictionary* pDictionary = 0;

  IAAFSourceMob* pSourceMob = 0;
  IAAFMasterMob* pMasterMob = 0;

  IAAFDataDef* pPictureDataDef = 0;
  aafRational_t rate = { 25, 1 };

  check(CreateAAFFile(pFileName, &pFile));
  check(pFile->GetHeader(&pHeader));
  check(pHeader->GetDictionary(&pDictionary));
 
  // Set the operational pattern
  check(pHeader->QueryInterface(IID_IAAFHeader2, (void **)&pHeader2));
  check(pHeader2->SetOperationalPattern(kAAFOPDef_EditProtocol));

  check(pDictionary->LookupDataDef(kAAFDataDef_Picture, &pPictureDataDef));

  check(CreateSourceMob(pFile, pHeader, pDictionary, pPictureDataDef, rate, &pSourceMob));
  check(CreateMasterMob(pFile, pHeader, pDictionary, pPictureDataDef, rate, pSourceMob, &pMasterMob));
  check(CreateCompositionMob(pFile, pHeader, pDictionary, pPictureDataDef, rate, pMasterMob));

cleanup:
  // Cleanup and return

  if (pMasterMob)
    pMasterMob->Release();

  if (pSourceMob)
    pSourceMob->Release();

  if (pPictureDataDef)
    pPictureDataDef->Release();

  if (pDictionary)
    pDictionary->Release();

  if (pHeader2)
    pHeader2->Release();

  if (pHeader)
    pHeader->Release();

  if (pFile) 
  {
    pFile->Save();
    pFile->Close();
    pFile->Release();
  }

  printf("DONE\n\n");

  return moduleErrorTmp;
}

// simple helper class to initialize and cleanup AAF library.
struct CAAFInitialize
{
  CAAFInitialize(const char *dllname = 0)
  {
    HRESULT hr = AAFLoad(dllname);
    if (!AAFRESULT_SUCCEEDED(hr))
    {
      fprintf(stderr, "Error : Failed to load the AAF library, ");
      fprintf(stderr, "check environment variables -\n");
      fprintf(stderr, "  Windows    - $PATH\n");
      fprintf(stderr, "  Unix/Linux - $LD_LIBRARY_PATH\n");
      exit(hr);
    }
  }

  ~CAAFInitialize()
  {
    AAFUnload();
  }
};

// Make sure all of our required plugins have been registered.
static HRESULT RegisterRequiredPlugins(void)
{
  IAAFPluginManager* mgr = 0;

  // Load the plugin manager 
  check(AAFGetPluginManager(&mgr));

  // Attempt load and register all of the plugins in the shared plugin directory.
  check(mgr->RegisterSharedPlugins());

  // Attempt to register all of the plugin files in the given directorys:
  //check(mgr->RegisterPluginDirectory(directory1));
  //check(mgr->RegisterPluginDirectory(directory2));

  // Attempt to register all of the plugins in any of the given files:
  //check(mgr->RegisterPluginFile(file1));
  //check(mgr->RegisterPluginFile(file2));
  //...

cleanup:
  if (mgr)
    mgr->Release();

  return moduleErrorTmp;
}

int main(int argumentCount, char* argumentVector[])
{
  CAAFInitialize aafInit;

  // Make sure all of our required plugins have been registered.
  checkFatal(RegisterRequiredPlugins());

  const char* pFileName = "ExportAS05Effects.aaf";
  MakeAAFFile(pFileName);

  return 0;
}

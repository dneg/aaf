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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#if defined(macintosh) || defined(_MAC)
#include "DataInput.h"
#endif

#include "AAFTypes.h"
#include "AAFResult.h"
#include "AAFDefUIDs.h"
#include "AAFDataDefs.h"
#include "AAFOperationDefs.h"
#include "AAFContainerDefs.h"
#include "AAFCodecDefs.h"
#include "AAFEssenceFormats.h"
#include "AAFStoredObjectIDs.h"
#include "AAFPropertyDefs.h"
#include "AAFTypeDefUIDs.h"

// Include the AAF interface declarations.
#include "AAF.h"

// Include the AAF Stored Object identifiers. These symbols are defined in aaf.lib.
#include "AAFStoredObjectIDs.h"

// Move to Header file for example
static const aafUID_t DDEF_SceneDescription = 
{ 0x6F3C8CE1, 0x6CEF, 0x11D2, { 0x80, 0x7D, 0x20, 0x15, 0x08, 0x2E, 0x3E, 0x6F } };

// binary blob
static const aafUID_t KLVKey_BinaryBlob1 = 
{ 0x6F3C8CE1, 0x6CEF, 0x11D2, { 0x80, 0x7D, 0x20, 0x15, 0x08, 0x45, 0x3E, 0x2a } }; 

// Ascii string
//static const aafUID_t KLVKey_ASCIIString1 = 
//{ 0x6F3C8CE1, 0x6CEF, 0x11D2, { 0x80, 0x7D, 0x20, 0x15, 0x08, 0x45, 0x3E, 0x2b } };

// Int32
static const aafUID_t KLVKey_Int32_1 = 
{ 0x6F3C8CE1, 0x6CEF, 0x11D2, { 0x80, 0x7D, 0x20, 0x15, 0x08, 0x45, 0x3E, 0x2c } };

static const aafUInt8 blobData[]={0x01, 0x02, 0xFF, 0x41, 0x42};
//static const char ASCIIStringData[]="Action scene with bikes";
static const aafInt32 int32Data=2000;



#define assert(b, msg) \
  if (!(b)) {fprintf(stderr, "ASSERT: %s\n\n", msg); exit(1);}



#define TEST_PATH	L"SomeFile.dat"

static void     LogError(HRESULT errcode, int line, char *file)
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

static void convert(char* cName, size_t length, const wchar_t* name)
{
  assert((name && *name), "Valid input name");
  assert(cName != 0, "Valid output buffer");
  assert(length > 0, "Valid output buffer size");

  size_t status = wcstombs(cName, name, length);
  if (status == (size_t)-1) {
    fprintf(stderr, ": Error : Conversion failed.\n\n");
    exit(1);  
  }
}

static void MobIDtoString(aafMobID_constref uid, char *buf)
{
    sprintf( buf, "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x-" \
		  "%02x-%02x-%02x-%02x-" \
		  "%08x%04x%04x" \
		  "%02x%02x%02x%02x%02x%02x%02x%02x",

	(int)uid.SMPTELabel[0], (int)uid.SMPTELabel[1], 
	(int)uid.SMPTELabel[2], (int)uid.SMPTELabel[3],
	(int)uid.SMPTELabel[4], (int)uid.SMPTELabel[5], 
	(int)uid.SMPTELabel[6], (int)uid.SMPTELabel[7],
	(int)uid.SMPTELabel[8], (int)uid.SMPTELabel[9], 
	(int)uid.SMPTELabel[10], (int)uid.SMPTELabel[11],

	(int)uid.length, (int)uid.instanceHigh, 
	(int)uid.instanceMid, (int)uid.instanceLow,

	uid.material.Data1, uid.material.Data2, uid.material.Data3,

	(int)uid.material.Data4[0], (int)uid.material.Data4[1], 
	(int)uid.material.Data4[2], (int)uid.material.Data4[3],
	(int)uid.material.Data4[4], (int)uid.material.Data4[5], 
	(int)uid.material.Data4[6], (int)uid.material.Data4[7] );
}


typedef enum { testStandardCalls, testMultiCalls } testType_t;

typedef aafInt16	AAFByteOrder;
const AAFByteOrder INTEL_ORDER		      = 0x4949; // 'II' for Intel
const AAFByteOrder MOTOROLA_ORDER         = 0x4d4d; // 'MM' for Motorola


AAFByteOrder GetNativeByteOrder(void);
void AAFByteSwap32(
		   aafInt32 *lp);	/* IN/OUT -- Byte swap this value */
void AAFByteSwap16(
		   aafInt16 * wp);	/* IN/OUT -- Byte swap this value */
void scanWAVEData(aafUInt8 **srcBufHdl, aafInt32 maxsize, void *data);
void scanSwappedWAVEData(aafUInt8 **srcBufHdl, aafInt32 maxsize, void *data);
AAFRESULT loadWAVEHeader(aafUInt8 *buf,
			 aafUInt16 *bitsPerSample,
			 aafUInt16 *numCh,
			 aafRational_t *sampleRate,
			 aafUInt32 *dataOffset,
			 aafUInt32 *dataLen);

typedef struct
{
  aafWChar	*dataFilename;
  aafUID_t	dataFormat;
} testDataFile_t;

const aafUID_t NIL_UID = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };

static HRESULT CreateAAFFile(aafWChar * pFileName, testDataFile_t *dataFile, testType_t /* testType */)
{
  IAAFFile*					pFile = NULL;
  IAAFHeader*					pHeader = NULL;
  IAAFDictionary*				pDictionary = NULL;
  IAAFMob*					pMob = NULL;
  IAAFMasterMob*				pMasterMob = NULL;
  IAAFClassDef*   pKLVDataCD=NULL;
  IAAFClassDef*   pCommentMarkerCD=NULL;
  IAAFTypeDef*    pUnknownBaseType=NULL;
  IAAFTypeDef*	pInt32BaseType=NULL;
  // IAAFTypeDef* pASCIIStringBaseType=NULL;
  IAAFKLVData* pKLVData=NULL;

  IAAFEssenceAccess*			pEssenceAccess = NULL;
  IAAFEssenceMultiAccess*		pMultiEssence = NULL;
  IAAFEssenceFormat*			pFormat = NULL;
  IAAFEssenceFormat*	format = NULL;
  IAAFLocator*	pLocator = NULL;
  IAAFClassDef*	pCDMasterMob = 0;
  IAAFClassDef                *pCDNetworkLocator = 0;
  IAAFDataDef                 *pDdefSound = 0;
  IAAFDataDef*	pDDefSceneDesc=0;
  IAAFEventMobSlot  *pEventSlot=0;
  IAAFMobSlot   *pSlot=0;
  IAAFSequence  *pSeqSceneDesc=0;
  IAAFSegment   *pSegment=0;
  IAAFEvent     *pEventSceneDesc=0;
  IAAFComponent *pComp=0;
  HRESULT	      hr = AAFRESULT_SUCCESS;


  // !!!Previous revisions of this file contained variables here required to handle external essence
  aafMobID_t					masterMobID;
  aafProductIdentification_t	ProductInfo;
  aafRational_t   editRate = {30000, 1001};
  aafRational_t	sampleRate = {44100, 1};
  aafRational_t	eventTimebase = {30000, 1001};
  aafPosition_t	position=0;
  FILE*	pWavFile = NULL;
  unsigned char	dataBuff[4096], *dataPtr;
  //	aafUInt32	bytesWritten;
  aafUInt32	dataOffset, dataLen;
  aafUInt16	bitsPerSample, numCh;
  aafInt32	n, numSpecifiers;
  aafUID_t	essenceFormatCode, testContainer;
  aafUInt32 samplesWritten, bytesWritten;
  // delete any previous test file before continuing...
  char chFileName[1000];
  convert(chFileName, sizeof(chFileName), pFileName);
  remove(chFileName);
  if(dataFile != NULL)
    {
      // delete any previous test file before continuing...
      char chFileName[1000];
      convert(chFileName, sizeof(chFileName), dataFile->dataFilename);
      remove(chFileName);
    }


  aafProductVersion_t v;
  v.major = 1;
  v.minor = 0;
  v.tertiary = 0;
  v.patchLevel = 0;
  v.type = kAAFVersionUnknown;

  ProductInfo.companyName = L"AAF Association";
  ProductInfo.productName = L"Metadata example";
  ProductInfo.productVersion = &v;
  ProductInfo.productVersionString = NULL;
  ProductInfo.productID = NIL_UID;
  ProductInfo.platform = NULL;

  check(AAFFileOpenNewModify (pFileName, 0, &ProductInfo, &pFile));
  check(pFile->GetHeader(&pHeader));

  // test
  // Get the AAF Dictionary so that we can create valid AAF objects.
  check(pHeader->GetDictionary(&pDictionary));

  check(pDictionary->LookupClassDef(AUID_AAFMasterMob,
				    &pCDMasterMob));
  check(pDictionary->LookupClassDef(AUID_AAFNetworkLocator,
				    &pCDNetworkLocator));
  check(pDictionary->LookupDataDef(DDEF_Sound,
				   &pDdefSound));

  // !!!Previous revisions of this file contained code here required to handle external essence

  // Get a Master MOB Interface
  check(pCDMasterMob->
	CreateInstance(IID_IAAFMasterMob, 
		       (IUnknown **)&pMasterMob));
  // Get a Mob interface and set its variables.
  check(pMasterMob->QueryInterface(IID_IAAFMob, (void **)&pMob));
  check(pMob->GetMobID(&masterMobID));
  check(pMob->SetName(L"A Master Mob"));
	
  // Add it to the file 
  check(pHeader->AddMob(pMob));

  // !!!Previous revisions of this file contained code here required to handle external essence

  if(dataFile != NULL)
    {
      // Make a locator, and attach it to the EssenceDescriptor
      check(pCDNetworkLocator->
	    CreateInstance(IID_IAAFLocator, 
			   (IUnknown **)&pLocator));		
      check(pLocator->SetPath (dataFile->dataFilename));
      testContainer = dataFile->dataFormat;
    }
  else
    {
      pLocator = NULL;
      testContainer = ContainerAAF;
    }


  // open the Essence file to be included in this AAF file("Laser.wav")
  pWavFile = fopen("Laser.wav", "r");
  if (pWavFile)
    {
      // read in the essence data
      fread(dataBuff, sizeof(unsigned char), sizeof(dataBuff), pWavFile);
      check(loadWAVEHeader(dataBuff,
			   &bitsPerSample,
			   &numCh,
			   &sampleRate,
			   &dataOffset,
			   &dataLen));
      dataPtr = dataBuff + dataOffset;
      // now create the Essence data file
     check(pMasterMob->CreateEssence(1,				// Slot ID
				      pDdefSound,		// MediaKind
				      kAAFCodecWAVE,		// codecID
				      editRate,		// edit rate
				      sampleRate,		// sample rate
				      kAAFCompressionDisable,
				      pLocator,	// In current file
				      testContainer,	// In AAF Format
				      &pEssenceAccess));// Compress disabled

      check(pEssenceAccess->GetFileFormatParameterList (&format));
      check(format->NumFormatSpecifiers (&numSpecifiers));
      for(n = 0; n < numSpecifiers; n++)
	{
	  check(format->GetIndexedFormatSpecifier (n, &essenceFormatCode, 0, NULL, NULL));
	}
      format->Release();
      format = NULL;

      // Tell the AAFEssenceAccess what the format is.
      check(pEssenceAccess->GetEmptyFileFormat (&pFormat));
      check(pFormat->NumFormatSpecifiers (&numSpecifiers));

      aafInt32	sampleSize = bitsPerSample;
      check(pFormat->AddFormatSpecifier (kAAFAudioSampleBits, sizeof(sampleSize), (aafUInt8 *)&sampleSize));
      check(pEssenceAccess->PutFileFormat (pFormat));
      pFormat->Release();
      pFormat = NULL;
      // write out the data

      check(pEssenceAccess->WriteSamples( dataLen,//!!! hardcoded bytes/sample ==1// Number of Samples
						sizeof(dataBuff), // buffer size
						dataPtr,	// THE data
						&samplesWritten,
						&bytesWritten));

      // close essence data file
      fclose(pWavFile);
      pWavFile = NULL;
      // Finish writing the destination
      check(pEssenceAccess->CompleteWrite());
      pEssenceAccess->Release();
      pEssenceAccess=NULL;
      // First register DataDef for SceneDescription
      // Is SceneDescription in dictionary already
      // Try using Timecode for MC compat
      hr = pDictionary->LookupDataDef(DDEF_Timecode,
				      &pDDefSceneDesc);
      // hr = pDictionary->LookupDataDef(DDEF_SceneDescription,
      //					 &pDDefSceneDesc);
      if (hr != AAFRESULT_SUCCESS) 
	{
	  check(pDictionary->CreateInstance(AUID_AAFDataDef,IID_IAAFDataDef,
					    (IUnknown **)&pDDefSceneDesc));
	  check(pDDefSceneDesc->Initialize(DDEF_SceneDescription,
					   L"SceneDescEvent",L"DataDefinition for Scene Description Events"));
	  check(pDictionary->RegisterDataDef(pDDefSceneDesc));
	}

      // Add EventSlot to MasterMob

      // Create Slot
      check(pDictionary->CreateInstance(AUID_AAFEventMobSlot, IID_IAAFEventMobSlot,
					(IUnknown **)&pEventSlot));

      check(pEventSlot->SetEditRate(&eventTimebase));

      // Get the mob slot interface so that we can add the event segment.
      check(pEventSlot->QueryInterface(IID_IAAFMobSlot, (void **)&pSlot));
      pEventSlot->Release();
      pEventSlot = NULL;
      aafSlotID_t eventSlotID=5;
      check(pSlot->SetSlotID(eventSlotID));
      check(pSlot->SetName(L"SceneDescriptions"));

      // Set up Sequence for Scene Descriptors
      check(pDictionary->CreateInstance(AUID_AAFSequence,IID_IAAFSequence,
					(IUnknown **)&pSeqSceneDesc));
      check(pSeqSceneDesc->Initialize(pDDefSceneDesc));

      // Create first Comment Marker
      check(pDictionary->LookupClassDef(AUID_AAFCommentMarker,
					&pCommentMarkerCD));
      check(pCommentMarkerCD->CreateInstance(IID_IAAFEvent,
					     (IUnknown **)&pEventSceneDesc));
      position=0;
      check(pEventSceneDesc->SetPosition(position));
      check(pEventSceneDesc->SetComment(L"Racers heading up the hill"));
      check(pEventSceneDesc->QueryInterface(IID_IAAFComponent, (void **)&pComp));
      check(pComp->SetDataDef(pDDefSceneDesc));		
      // Add it to the sequence
      check(pSeqSceneDesc->AppendComponent(pComp));
      pComp->Release();
      pComp = NULL;
      pEventSceneDesc->Release();
      pEventSceneDesc=NULL;

      // Create second Comment Marker
      check(pCommentMarkerCD->CreateInstance(IID_IAAFEvent,
					     (IUnknown **)&pEventSceneDesc));
      position=100;
      check(pEventSceneDesc->SetPosition(position));
      check(pEventSceneDesc->SetComment(L"Racer pulls away from crowd"));
      check(pEventSceneDesc->QueryInterface(IID_IAAFComponent, (void **)&pComp));
      check(pComp->SetDataDef(pDDefSceneDesc));		
      // Add it to the sequence
      check(pSeqSceneDesc->AppendComponent(pComp));
      pComp->Release();
      pComp = NULL;
      pEventSceneDesc->Release();
      pEventSceneDesc=NULL;
      check(pSeqSceneDesc->QueryInterface(IID_IAAFSegment, (void **)&pSegment));

      // Create third Comment Marker
      check(pCommentMarkerCD->CreateInstance(IID_IAAFEvent,
					     (IUnknown **)&pEventSceneDesc));
      position=250;
      check(pEventSceneDesc->SetPosition(position));
      check(pEventSceneDesc->SetComment(L"Racer passes finish line"));
      check(pEventSceneDesc->QueryInterface(IID_IAAFComponent, (void **)&pComp));
      check(pComp->SetDataDef(pDDefSceneDesc));		
      // Add it to the sequence
      check(pSeqSceneDesc->AppendComponent(pComp));
      pComp->Release();
      pComp = NULL;
      pEventSceneDesc->Release();
      pEventSceneDesc=NULL;
      // Add the event sequence to the event mob slot
      // Sequence must have at least one component to ensure event mob slot is valid
      check(pSlot->SetSegment(pSegment));
      pSegment->Release();
      pSegment = NULL;

      // Append the event slot to the Master Mob
      check(pMob->AppendSlot(pSlot));

      // Add the KLV data to the Master Mob

      // Register the two KLVKeys
      check(pDictionary->LookupTypeDef(kAAFTypeID_UInt8Array,
				       &pUnknownBaseType));
      check(pDictionary->RegisterKLVDataKey(KLVKey_BinaryBlob1,
					    pUnknownBaseType));
      check(pDictionary->LookupTypeDef(kAAFTypeID_Int32,
				       &pInt32BaseType));
      check(pDictionary->RegisterKLVDataKey(KLVKey_Int32_1,
					    pInt32BaseType));
      // Next define new ASCII string data def
      // check(pDictionary->RegisterKLVDataKey(KLVKey_ASCIIString1,
      //				      pASCIIStringBaseType));

      // Get the KLVData ClassDefinition
      check(pDictionary->LookupClassDef(AUID_AAFKLVData,
					&pKLVDataCD));


      // Create KLVData and append it to Mob
      check(pKLVDataCD->CreateInstance(IID_IAAFKLVData, 
				       (IUnknown **)&pKLVData));
		
      check(pKLVData->Initialize(KLVKey_BinaryBlob1,
				 sizeof(blobData), (aafUInt8 *)blobData));
      check(pMob->AppendKLVData(pKLVData));
      pKLVData->Release();
      pKLVData=NULL;

      // Create KLVData and append it to Mob
      check(pKLVDataCD->CreateInstance(IID_IAAFKLVData, 
				       (IUnknown **)&pKLVData));
		
      check(pKLVData->Initialize(KLVKey_Int32_1,
				 sizeof(int32Data), (aafUInt8 *)&int32Data));
      check(pMob->AppendKLVData(pKLVData));
      pKLVData->Release();
      pKLVData=NULL;
    }
  else
    {
      printf("***Failed to open Wave file Laser.wav\n");
    }
  // Release all unnecesary interfaces

  if (pInt32BaseType)
    pInt32BaseType->Release();
  pInt32BaseType=NULL;
  if (pUnknownBaseType)
    pUnknownBaseType->Release();
  pUnknownBaseType=NULL;
  if (pKLVDataCD)
    pKLVDataCD->Release();
  pKLVDataCD=NULL;
  if (pCommentMarkerCD)
    pCommentMarkerCD->Release();
  pCommentMarkerCD=NULL;
  if(pComp)
    pComp->Release();
  pComp=NULL;
  if(pEventSceneDesc)
    pEventSceneDesc->Release();
  pEventSceneDesc=0;
  if(pSegment)
    pSegment->Release();
  pSegment=NULL;
  if(pSeqSceneDesc)
    pSeqSceneDesc->Release();
  pSeqSceneDesc=NULL;
  if(pSlot)
    pSlot->Release();
  pSlot=NULL;
  if(pEventSlot)
    pEventSlot->Release();
  pEventSlot=NULL;
  if(pDDefSceneDesc)
    pDDefSceneDesc->Release();
  pDDefSceneDesc=NULL;

  if(pMasterMob)
    pMasterMob->Release();
  pMasterMob = NULL;
  if(pMob)
    pMob->Release();
  pMob = NULL;

  if(pDictionary)
    pDictionary->Release();
  pDictionary = NULL;
  if(pHeader)
    pHeader->Release();
  pHeader = NULL;
  //!!!DebugOnly
  pFile->Save();
  pFile->Close();
  pFile->Release();
  pFile = NULL;
  if (pEssenceAccess)
    {	
      pEssenceAccess->Release();
      pEssenceAccess= NULL;
    }

 cleanup:
  // Cleanup and return
  if(pFormat)
    pFormat->Release();
  if(format)
    format->Release();
  if(pLocator)
    pLocator->Release();

  if (pWavFile)
    fclose(pWavFile);

  if (pEssenceAccess)
    pEssenceAccess->Release();
	
  if (pMultiEssence)
    pMultiEssence->Release();
	
  if (pMasterMob)
    pMasterMob->Release();

  if (pMob)
    pMob->Release();

  if (pDictionary)
    pDictionary->Release();

  if (pHeader)
    pHeader->Release();

  if (pCDMasterMob)
    {
      pCDMasterMob->Release();
      pCDMasterMob = 0;
    }

  if (pCDNetworkLocator)
    {
      pCDNetworkLocator->Release();
      pCDNetworkLocator = 0;
    }

  if (pDdefSound)
    {
      pDdefSound->Release ();
      pDdefSound = 0;
    }

  if (pFile)
    {
      pFile->Close();
      pFile->Release();
    }


  return moduleErrorTmp;
}

static HRESULT ReadAAFFile(aafWChar * pFileName, testType_t testType)
{
  IAAFFile *					pFile = NULL;
  IAAFHeader *				pHeader = NULL;
  IAAFDictionary*					pDictionary = NULL;
  IAAFEssenceAccess*			pEssenceAccess = NULL;
  IAAFEssenceMultiAccess*		pMultiEssence = NULL;
  IAAFEssenceFormat			*fmtTemplate =  NULL;
  IEnumAAFMobs*				pMobIter = NULL;
  IAAFMob*					pMob = NULL;
  IAAFMasterMob*				pMasterMob = NULL;
  IAAFEssenceFormat*			pFormat = NULL;

  aafNumSlots_t				numMobs, numSlots;
  aafSearchCrit_t				criteria;
  aafRational_t				readSampleRate;
  aafMobID_t					mobID;
  aafWChar					namebuf[1204];
  unsigned char				AAFDataBuf[4096];
  aafUInt32					AAFBytesRead, samplesRead;
  FILE*						pWavFile = NULL;
  unsigned char				WAVDataBuf[4096], *dataPtr;
  size_t						WAVBytesRead;
  aafUInt32					dataOffset, dataLen;
  aafUInt16					bitsPerSample, numCh;

  check(AAFFileOpenExistingRead ( pFileName, 0, &pFile));
  check(pFile->GetHeader(&pHeader));

  // Get the AAF Dictionary so that we can create valid AAF objects.
  check(pHeader->GetDictionary(&pDictionary));


  // Here we check on the number of mobs in the file. 
  // Get the number of master mobs in the file (should be one)
  check(pHeader->CountMobs(kAAFMasterMob, &numMobs));
  // ** causes leak

  if (1 == numMobs )
    {
      printf("Found %ld Master Mobs\n", numMobs);
      criteria.searchTag = kAAFByMobKind;
      criteria.tags.mobKind = kAAFMasterMob;
      check(pHeader->GetMobs(&criteria, &pMobIter));
      while(AAFRESULT_SUCCESS == pMobIter->NextOne(&pMob))
	{
	  char mobIDstr[256];
	  char mobName[256];


	  check(pMob->GetMobID (&mobID));
	  check(pMob->GetName (namebuf, sizeof(namebuf)));
	  convert(mobName, sizeof(mobName), namebuf);
	  MobIDtoString(mobID, mobIDstr);
	  printf("    MasterMob Name = '%s'\n", mobName);
	  printf("        (mobID %s)\n", mobIDstr);
	  // Make sure we have two slots 
	  check(pMob->CountSlots(&numSlots));
	  if (2 == numSlots)
	    {
				// The essence data is in SlotID 1
				// Get a Master Mob interface
	      check(pMob->QueryInterface(IID_IAAFMasterMob, (void **)&pMasterMob));

				// Open the Essence Data
	      check(pMasterMob->OpenEssence(	1,     // SlotID 1
						NULL,  // mediaCriteria (Don't care)
						kAAFMediaOpenReadOnly,	// Open mode
						kAAFCompressionDisable,// Compress disabled
						&pEssenceAccess));

				// Open and read the Wave file (for comparison)
	      pWavFile = fopen("Laser.wav", "r");
	      if (pWavFile)
		{
		  // read in the essence data
		  WAVBytesRead = fread(WAVDataBuf, sizeof(unsigned char), sizeof(WAVDataBuf), pWavFile);
		  fclose(pWavFile);
		  pWavFile = NULL;
		  check(loadWAVEHeader(WAVDataBuf,
				       &bitsPerSample,
				       &numCh,
				       &readSampleRate,
				       &dataOffset,
				       &dataLen));
		  dataPtr = WAVDataBuf + dataOffset;

		  aafUInt32			sampleBits;
		  aafInt32			bytesRead;
					
		  check(pEssenceAccess->GetEmptyFileFormat (&fmtTemplate));
		  check(fmtTemplate->AddFormatSpecifier (kAAFAudioSampleBits, 0, NULL));
		  check(pEssenceAccess->GetFileFormat (fmtTemplate, &pFormat));
		  fmtTemplate->Release();
		  fmtTemplate = NULL;
					
		  check(pFormat->GetFormatSpecifier (kAAFAudioSampleBits, sizeof(sampleBits),
						     (aafDataBuffer_t)&sampleBits, &bytesRead));
		  pFormat->Release();
		  pFormat = NULL;
		  if(sampleBits != bitsPerSample)
		    {
		      printf("***Wrong sample size read ( was %ld , should be %ld)\n",
			     sampleBits, bitsPerSample);
		    }

		  // Read the Data from the AAF file
		  if(testType == testStandardCalls)
		    {
		      check(pEssenceAccess->ReadSamples( dataLen, //!!! Hardcoded	// Number of Samples 
							 sizeof(AAFDataBuf),	// Maximum buffer size
							 AAFDataBuf,	  // Buffer for the data
							 &samplesRead,      // Actual number of samples read
							 &AAFBytesRead));	// Actual number of bytes read
		    }
		  else if(testType == testMultiCalls)
		    {
		      aafmMultiXfer_t		xfer;
		      aafmMultiResult_t	result;

		      check(pEssenceAccess->QueryInterface(IID_IAAFEssenceMultiAccess, (void **)&pMultiEssence));
		      xfer.numSamples = dataLen;	//!!! Hardcoded	// Number of Samples 
		      xfer.buflen = sizeof(AAFDataBuf);
		      xfer.buffer = AAFDataBuf;
		      result.bytesXfered = 0;
		      check(pMultiEssence->ReadMultiSamples(1, &xfer, &result));
		      samplesRead = result.samplesXfered;
		      AAFBytesRead = result.bytesXfered;
		      pMultiEssence->Release();
		      pMultiEssence = NULL;
		    }

		  // Now compare the data read from the AAF file to the actual WAV file
		  if (dataLen != AAFBytesRead)
		    {
		      printf("***Wrong number of bytes read ( was %ld , should be %ld)\n",
			     AAFBytesRead, WAVBytesRead);
		    }
		  if (memcmp( dataPtr, AAFDataBuf, dataLen) != 0)
		    {
		      printf("*** Data Read is different than the data in the WAV file ***\n");
		    }
		}
	      else
		{
		  printf("***Failed to open Wave file Laser.wav for comparison\n");
		}
	    }
	  else
	    {
	      printf("***Wrong number of slots in the Master Mob (was %ld should be %ld)\n",
		     numSlots, 2L);
	    }
	  if (pMasterMob)
	    {
	      pMasterMob->Release();
	      pMasterMob = NULL;
	    }

	  pMob->Release();
	  pMob = NULL;
	  if (pEssenceAccess)
	    {
	      pEssenceAccess->Release();
	      pEssenceAccess = NULL;
	    }
	} // while pMobIter->NextOne
      pMobIter->Release();
      pMobIter = NULL;

    }
  else
    {
      printf("***Wrong number of Master mobs in the file (was %ld should be %ld)\n",
	     numMobs, 1L);
    }
  printf("--------\n");

 cleanup:
  // Cleanup and return

  if (pWavFile)
    fclose(pWavFile);
  if (pMultiEssence)
    pMultiEssence->Release();
  pMultiEssence=NULL;
  if(fmtTemplate)
    {
      fmtTemplate->Release();
      fmtTemplate = NULL;
    }
  if (pEssenceAccess)
    {
      pEssenceAccess->Release();
      pEssenceAccess = NULL;
    }
  if (pDictionary)
    pDictionary->Release();
  pDictionary=NULL;

  if (pHeader)
    pHeader->Release();
  pHeader=NULL;
  if (pMobIter)
    pMobIter->Release();
  pMobIter=NULL;
  if (pFormat)
    pFormat->Release();
  pFormat=NULL;

  if (pFile) 
    {
      pFile->Close();
      pFile->Release();
      pFile=NULL;
    }

  return moduleErrorTmp;
}


// simple helper class to initialize and cleanup AAF library.
struct CAAFInitialize
{
  CAAFInitialize(const char *dllname = NULL)
  {
    printf("Attempting to load the AAF dll...\n");
    HRESULT hr = AAFLoad(dllname);
    (SUCCEEDED(hr)) ? printf("DONE\n") : printf("FAILED\n");
  }

  ~CAAFInitialize()
  {
    AAFUnload();
  }
};



//**********************
// Extra code required to scan the original WAVE headers and extract metadata parameters & data offset
AAFByteOrder GetNativeByteOrder(void)
{
  aafInt16 word = 0x1234;
  aafInt8  byte = *((aafInt8*)&word);
  AAFByteOrder result;

  //  ASSERT("Valid byte order", ((byte == 0x12) || (byte == 0x34)));

  if (byte == 0x12) {
    result = MOTOROLA_ORDER;
  } else {
    result = INTEL_ORDER;
  }
  return result;
}

void AAFByteSwap32(
		   aafInt32 *lp)	/* IN/OUT -- Byte swap this value */
{
  register unsigned char *cp = (unsigned char *) lp;
  int             t;

  t = cp[3];
  cp[3] = cp[0];
  cp[0] = t;
  t = cp[2];
  cp[2] = cp[1];
  cp[1] = t;
}

void AAFByteSwap16(
		   aafInt16 * wp)	/* IN/OUT -- Byte swap this value */
{
  register unsigned char *cp = (unsigned char *) wp;
  int             t;

  t = cp[1];
  cp[1] = cp[0];
  cp[0] = t;
}

// app work on all WAVE files, not just laser.wav.
void scanWAVEData(aafUInt8 **srcBufHdl, aafInt32 maxsize, void *data)
{
  memcpy(data, *srcBufHdl, maxsize);
  (*srcBufHdl) += maxsize;
}

void scanSwappedWAVEData(aafUInt8 **srcBufHdl, aafInt32 maxsize, void *data)
{
  AAFByteOrder	nativeByteOrder = GetNativeByteOrder()
    ;
  memcpy(data, *srcBufHdl, maxsize);
  (*srcBufHdl) += maxsize;
	
  if ((maxsize == sizeof(aafInt32)) && (INTEL_ORDER != nativeByteOrder))
    AAFByteSwap32((aafInt32 *) data);
  else if ((maxsize == sizeof(aafInt16)) && (INTEL_ORDER != nativeByteOrder))
    AAFByteSwap16((aafInt16 *) data);
}

AAFRESULT loadWAVEHeader(aafUInt8 *buf,
			 aafUInt16 *bitsPerSample,
			 aafUInt16 *numCh,
			 aafRational_t *sampleRate,
			 aafUInt32 *dataOffset,
			 aafUInt32 *dataLen)
{
  aafInt32			formSize;
  aafInt16			pcm_format, junk16;
  aafUInt32			chunkSize;
  aafBool				fmtFound = kAAFFalse, dataFound = kAAFFalse;
  aafUInt8			chunkID[4];
  aafInt32			junk32, rate, bytesPerFrame;
  aafUInt8			*ptr;

  ptr = buf;
  scanWAVEData(&ptr, sizeof(chunkID), &chunkID);	
  if (memcmp(&chunkID, "RIFF", (size_t) 4) != 0)
    return(AAFRESULT_BADWAVEDATA);
  scanSwappedWAVEData(&ptr, sizeof(formSize), &formSize);	
  scanWAVEData(&ptr, sizeof(chunkID), &chunkID);	
  if (memcmp(&chunkID, "WAVE", (size_t) 4) != 0)
    return(AAFRESULT_BADWAVEDATA);
	
  scanWAVEData(&ptr, sizeof(chunkID), &chunkID);	
  while ((ptr-buf) < formSize)
    {
      scanSwappedWAVEData(&ptr, sizeof(chunkSize), &chunkSize);	

      if (memcmp(&chunkID, "fmt ", (size_t) 4) == 0)
	{
	  // WAVE field: wFormatTag
	  scanSwappedWAVEData(&ptr, sizeof(aafUInt16), (aafUInt8 *)&pcm_format);
	  if (pcm_format != 1)
	    return(AAFRESULT_BADWAVEDATA);
	
	  // WAVE field: wChannels
	  scanSwappedWAVEData(&ptr, sizeof(aafUInt16), (aafUInt8 *)numCh);
	
	  // WAVE field: wSamplesPerSec
	  scanSwappedWAVEData(&ptr, sizeof(aafUInt32), (aafUInt8 *)&rate);
	  sampleRate->numerator = rate;
	  sampleRate->denominator = 1;
	
	  // Skip WAVE field: avgBytesPerSec (4 bytes)
	  scanSwappedWAVEData(&ptr, sizeof(junk32), (aafUInt8 *)&junk32);

	  // WAVE field wBlockAlign
	  scanSwappedWAVEData(&ptr, sizeof(aafUInt16), (aafUInt8 *)&junk16);
	
	  // WAVE field Sample Width
	  scanSwappedWAVEData(&ptr, sizeof(aafUInt16), (aafUInt8 *)bitsPerSample);
	  bytesPerFrame = (((*bitsPerSample) + 7) / 8) * (*numCh);
	  fmtFound = kAAFTrue;
	} else if (memcmp(&chunkID, "data", (size_t) 4) == 0)
	  {
	    *dataLen = chunkSize / bytesPerFrame;
	    // Positioned at beginning of audio data
	    *dataOffset = ptr - buf;
	
	    dataFound = kAAFTrue;
	  }
	
      if((ptr-buf) > formSize)
	break;
      if (fmtFound && dataFound)	// Do we have all information yet?
	break;
      scanWAVEData(&ptr, sizeof(chunkID), &chunkID);	
    }	
	
  return(AAFRESULT_SUCCESS);
}

// Make sure all of our required plugins have been registered.
static HRESULT RegisterRequiredPlugins(void)
{
  IAAFPluginManager	*mgr = NULL;

  // Load the plugin manager 
  check(AAFGetPluginManager(&mgr));

  // Attempt load and register all of the plugins
  // in the shared plugin directory.
  check(mgr->RegisterSharedPlugins());

  // Attempt to register all of the plugin files
  // in the given directorys:
  //check(mgr->RegisterPluginDirectory(directory1));
  //check(mgr->RegisterPluginDirectory(directory2));


  // Attempt to register all of the plugins in any
  // of the given files:
  //check(mgr->RegisterPluginFile(file1));
  //check(mgr->RegisterPluginFile(file2));
  //...

 cleanup:
  if (mgr)
    mgr->Release();

  return moduleErrorTmp;
}

int main(int /*argc*/, char** /*argv*/)
{
  CAAFInitialize aafInit;

  aafWChar* pwFileName = L"MetadataTest.aaf";
  char* pFileName = "";

  // Make sure all of our required plugins have been registered.
  checkFatal(RegisterRequiredPlugins());

  /**/
  printf("***Creating file %s using WriteSamples (Internal Media)\n", pFileName);
  checkFatal(CreateAAFFile(pwFileName, NULL, testStandardCalls));
  printf("***Re-opening file %s using ReadSamples\n", pFileName);
  ReadAAFFile(pwFileName, testStandardCalls);
  /**/

  printf("Done\n");


  return(0);
}





// @com Executable test program by Chris Morgan, intern for Avid Technology, Tewksbury 
// @com This is used for scalability testing of AAF code.  Last modified on 7/23/99.

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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "AAFTypes.h"
#include "AAFResult.h"
#include "AAFDataDefs.h"
#include "UtlConsole.h"

// Include the AAF interface declarations.
#include "AAF.h"

// Include the AAF Stored Object identifiers. These symbols are defined in aaf.lib.
#include "AAFStoredObjectIDs.h"

// There are differences in the microsoft and other compilers in the "Length" specifier
// used in printf for 64bit integers.
//
// NOTE: If your compiler does not support 64 bit integers then this example will NOT
// print out the correct lengths.
#ifdef _MSC_VER
#define L64 "I64"
#else
#define L64 "ll"
#endif

// MAX is used for arrays when converting between types - set here for debugging. 
const int MAX = 80;
static char* programName;
static char* niceFileName;
static void usage(void);
static aafWChar* slotName = L"SLOT1";
static aafInt32 fadeInLen  = 1000;
static aafInt32 fadeOutLen = 2000;
static aafFadeType_t fadeInType = kFadeLinearAmp;
static aafFadeType_t fadeOutType = kFadeLinearPower;
static aafSourceRef_t sourceRef; 

static aafBool	EqualAUID(aafUID_t *uid1, aafUID_t *uid2)
{
	return(memcmp((char *)uid1, (char *)uid2, sizeof(aafUID_t)) == 0 ? AAFTrue : AAFFalse);
}

#define TAPE_LENGTH			1L * 60L *60L * 30L
#define FILE1_LENGTH		60L * 30L
#define SEG_LENGTH			30L
#define FILL_LENGTH			10L
#define TEST_PATH	L"AnotherFile.aaf"

#define assert(b, msg) \
  if (!(b)) {fprintf(stderr, "ASSERT: %s\n\n", msg); exit(1);}

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

static void AUIDtoString(aafUID_t *uid, char *buf)
{
	sprintf(buf, "%08lx-%04x-%04x-%02x%02x%02x%02x%02x%02x%02x%02x",
			uid->Data1, uid->Data2, uid->Data3, (int)uid->Data4[0],
			(int)uid->Data4[1], (int)uid->Data4[2], (int)uid->Data4[3], (int)uid->Data4[4],
			(int)uid->Data4[5], (int)uid->Data4[6], (int)uid->Data4[7]);
}


static HRESULT convert(char* cName, size_t length, const wchar_t* name)
{
  assert((name && *name), "Valid input name");
  assert(cName != 0, "Valid output buffer");
  assert(length > 0, "Valid output buffer size");

  size_t status = wcstombs(cName, name, length);
  if (status == (size_t)-1) {
    fprintf(stderr, ": Error : Conversion failed.\n\n");
    return -1; 
  }
  else
  	return S_OK;
}

const aafUID_t NIL_UID = { 0, 0, 0, { 0, 0, 0, 0, 0, 0, 0, 0 } };

static HRESULT CreateAAFFile(aafWChar * pFileName, long int N)
{
	IAAFFile*					pFile = NULL;
	IAAFHeader*					pHeader = NULL;
	IAAFDictionary*					pDictionary = NULL;
	IAAFMob*					pMob = NULL;
	IAAFMob*					pCompMob = NULL;
	IAAFEssenceDescriptor*		aDesc = NULL;
	IAAFMasterMob*				pMasterMob = NULL;
	IAAFSourceMob*				pFileMob = NULL;
	IAAFSourceMob*				pTapeMob = NULL;
	IAAFSequence*				pSequence = NULL;
	IAAFComponent*				aComponent = NULL;
	IAAFFileDescriptor*			pFileDesc = NULL;
	IAAFTapeDescriptor*			pTapeDesc = NULL;
	IAAFMobSlot*				newSlot = NULL;
	IAAFSegment*				seg = NULL;
	IAAFSourceClip*				fileSclp = NULL;
	IAAFSourceClip*				masterSclp = NULL;
	IAAFSourceClip*				compSclp = NULL;
	IAAFComponent*				compFill = NULL;
	IAAFLocator*				pLocator = NULL;
	IAAFNetworkLocator*			pNetLocator = NULL;
	aafRational_t				videoRate = { 30000, 1001 };
	aafUID_t					videoDef = DDEF_Picture;
	aafUID_t					tapeMobID, fileMobID, masterMobID;
	aafTimecode_t				tapeTC = { 108000, kTcNonDrop, 30};
	aafLength_t					fileLen = FILE1_LENGTH;
	aafLength_t					fillLen = FILL_LENGTH;
	aafLength_t					segLen = SEG_LENGTH;
	aafProductIdentification_t	ProductInfo;
	long int i = 0;

  
  // delete any previous test file before continuing...
  char chFileName[1000];
  convert(chFileName, sizeof(chFileName), pFileName);
  remove(chFileName);


	ProductInfo.companyName = L"Company Name";
	ProductInfo.productName = L"Scalability Test 1:";
	ProductInfo.productVersion.major = 1;
	ProductInfo.productVersion.minor = 0;
	ProductInfo.productVersion.tertiary = 0;
	ProductInfo.productVersion.patchLevel = 0;
	ProductInfo.productVersion.type = kVersionUnknown;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = NIL_UID;
	ProductInfo.platform = NULL;

  check(AAFFileOpenNewModify (pFileName, 0, &ProductInfo, &pFile));

	check(pFile->GetHeader(&pHeader));

  // Get the AAF Dictionary so that we can create valid AAF objects.
  check(pHeader->GetDictionary(&pDictionary));


// IMPORTANT: major remodification is from this point onwards...

	// sequence creation code pulled out of the subsequent loop.
  // Create a Composition Mob
	check(pDictionary->CreateInstance( &AUID_AAFCompositionMob,
						   IID_IAAFMob, 
						   (IUnknown **)&pCompMob));

	check(pDictionary->CreateInstance( &AUID_AAFSequence,
			   IID_IAAFSequence, 
			   (IUnknown **)&pSequence));		
	check(pSequence->QueryInterface (IID_IAAFSegment, (void **)&seg));

	check(pSequence->QueryInterface(IID_IAAFComponent, (void **)&aComponent));

	check(aComponent->SetDataDef(&videoDef));
 	check(pCompMob->QueryInterface (IID_IAAFMob, (void **)&pMob));
	check(pMob->AppendNewSlot (seg, i, slotName, &newSlot));
	pMob->Release();
	pMob = NULL;
	newSlot->Release();
	newSlot = NULL;

	check(pHeader->AppendMob(pCompMob));


	// now looping around the remainder N times to make N components
	for (i=0; i<N; i++)
		{
		//Make the Tape MOB
 		check(pDictionary->CreateInstance( &AUID_AAFSourceMob,
						   IID_IAAFSourceMob, 
						   (IUnknown **)&pTapeMob));
		check(pDictionary->CreateInstance( &AUID_AAFTapeDescriptor,
						   IID_IAAFTapeDescriptor, 
						   (IUnknown **)&pTapeDesc));
		check(pTapeDesc->QueryInterface (IID_IAAFEssenceDescriptor, (void **)&aDesc));
		check(pTapeMob->SetEssenceDescriptor(aDesc));
		aDesc->Release();
		aDesc = NULL;
			
		check(pTapeMob->AppendTimecodeSlot (videoRate, 0, tapeTC, TAPE_LENGTH));
		check(pTapeMob->AddNilReference (1,TAPE_LENGTH, &videoDef, videoRate));
		check(pTapeMob->QueryInterface (IID_IAAFMob, (void **)&pMob));
		
		// NOTE: TapeMob name is updated to change with number of objects requested at cli.
		// In order to fit with the specification, it is made wide
		char TapeMobNameBuffer[MAX];
		sprintf(TapeMobNameBuffer,"Tape Mob %ld",i);
		aafWChar TapeMobName[MAX];
		mbstowcs(TapeMobName,TapeMobNameBuffer,MAX);
		
		check(pMob->SetName (TapeMobName));
		
		check(pHeader->AppendMob(pMob));
		check(pMob->GetMobID (&tapeMobID));
		pMob->Release();
		pMob = NULL;

		// Make a FileMob
		check(pDictionary->CreateInstance( &AUID_AAFSourceMob,
						   IID_IAAFSourceMob, 
						   (IUnknown **)&pFileMob));
		check(pDictionary->CreateInstance( &AUID_AAFFileDescriptor,
						   IID_IAAFFileDescriptor, 
						   (IUnknown **)&pFileDesc));
		check(pFileDesc->QueryInterface (IID_IAAFEssenceDescriptor, (void **)&aDesc));

		// Make a locator, and attach it to the EssenceDescriptor
		check(pDictionary->CreateInstance( &AUID_AAFNetworkLocator,
							IID_IAAFNetworkLocator, 
							(IUnknown **)&pNetLocator));		
		check(pNetLocator->QueryInterface (IID_IAAFLocator, (void **)&pLocator));
		
		
		
		check(pLocator->SetPath (TEST_PATH));	
		check(aDesc->AppendLocator(pLocator));

		check(pFileMob->SetEssenceDescriptor(aDesc));
		aDesc->Release();
		aDesc = NULL;
		pFileDesc->Release();
		pFileDesc = NULL;

		sourceRef.sourceID = tapeMobID;
		sourceRef.sourceSlotID = 1;
		sourceRef.startTime = 0;
		check(pFileMob->NewPhysSourceRef (videoRate,
                           1, &videoDef, sourceRef, fileLen));

		check(pFileMob->QueryInterface (IID_IAAFMob, (void **)&pMob));
		check(pMob->GetMobID (&fileMobID));
		check(pHeader->AppendMob(pMob));
		pMob->Release();
		pMob = NULL;

		//Make the Master MOB
		check(pDictionary->CreateInstance( &AUID_AAFMasterMob,
						   IID_IAAFMasterMob, 
						   (IUnknown **)&pMasterMob));

		sourceRef.sourceID = fileMobID;
		sourceRef.sourceSlotID = 1;
		sourceRef.startTime = 0;
		check(pMasterMob->NewPhysSourceRef (videoRate, 1, &videoDef, sourceRef, fileLen));
		check(pMasterMob->QueryInterface (IID_IAAFMob, (void **)&pMob));
		check(pMob->GetMobID (&masterMobID));
		
		// NOTE: MasterMob name is updated to change with number of objects requested at cli.
		// In order to fit with the specification, it is made wide
		char MasterMobNameBuffer[MAX];
		sprintf(MasterMobNameBuffer,"Master Mob %ld",i);
		aafWChar MasterMobName[MAX];
		mbstowcs(MasterMobName,MasterMobNameBuffer,MAX);
		
		check(pMob->SetName (MasterMobName));

		check(pHeader->AppendMob(pMob));
		pMob->Release();
		pMob = NULL;

		// the remaining part of the sequence code, adapted for updating slot names
		
		// This variable is about to be overwritten so we need to release the old interface.

		aComponent->Release();

		aComponent = NULL;

		// Create a SourceClip
		check(pDictionary->CreateInstance( &AUID_AAFSourceClip,
						   IID_IAAFSourceClip, 
						   (IUnknown **)&compSclp));		

		sourceRef.sourceID = masterMobID;
		sourceRef.sourceSlotID = 1;
		sourceRef.startTime = 0;
		check(compSclp->SetSourceReference (sourceRef));
		check(compSclp->QueryInterface (IID_IAAFComponent, (void **)&aComponent));
		check(aComponent->SetDataDef(&videoDef));
		check(aComponent->SetLength (&segLen));
		check(pSequence->AppendComponent (aComponent));
	
		// Create a filler - Get the component interface only (IID_IAAFComponent)
		check(pDictionary->CreateInstance( &AUID_AAFFiller,
									   IID_IAAFComponent, 
									   (IUnknown **)&compFill));		

		check(compFill->SetLength (&fillLen));

		check(compFill->SetDataDef(&videoDef));
		check(pSequence->AppendComponent (compFill));

		cleanup:
		// Cleanup and return
		if (pNetLocator)
			pNetLocator->Release();

		if (pLocator)
			pLocator->Release();

		if (compFill)
			compFill->Release();

		if (compSclp)
			compSclp->Release();

		if (masterSclp)
			masterSclp->Release();

		if (fileSclp)
			fileSclp->Release();

		if (pTapeDesc)
			pTapeDesc->Release();

		if (pFileDesc)
			pFileDesc->Release();

		if (pTapeMob)
			pTapeMob->Release();

		if (pFileMob)
			pFileMob->Release();

		if (pMasterMob)
			pMasterMob->Release();

		if (aDesc)
			aDesc->Release();

		if (newSlot)
			newSlot->Release();

		if (pMob)
			pMob->Release();
	//  end of loop since only one dictionary and header are needed
	//  the file is then saved, closed and released after all modifications are complete
	}

	if (pCompMob)
		pCompMob->Release();

	if (seg)
		seg->Release();
	
	if (aComponent)
		aComponent->Release();

	if (pSequence)
		pSequence->Release();

	if (pDictionary)
		pDictionary->Release();

	if (pHeader)
		pHeader->Release();

	if (pFile) 
	{
		// printing file save time
		aafUInt32 timerID, elapsedtime;
		moduleErrorTmp = UTLStartPeriod(&timerID);
		pFile->Save();
		moduleErrorTmp = UTLEndPeriod(timerID, &elapsedtime);
		pFile->Close();
		pFile->Release();
		printf("Save time = %ld\n", elapsedtime);
	}
	
	return moduleErrorTmp;
}
// simple helper class to initialize and cleanup COM library.
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

// now need the reading functionality
static HRESULT ReadAAFFile(aafWChar * pFileName)
{
	IAAFFile *					pFile = NULL;
  // printing file open time
	aafUInt32 timerID, elapsedtime;

	moduleErrorTmp = UTLStartPeriod(&timerID);
 	check(AAFFileOpenExistingRead ( pFileName, 0, &pFile));
	moduleErrorTmp = UTLEndPeriod(timerID, &elapsedtime);
	pFile->Close();
	pFile->Release();
	pFile=NULL;
	printf("Open time = %ld\n\n", elapsedtime);
cleanup:
	if (pFile)
		{
		pFile->Close();
		pFile->Release();
		}
	return AAFRESULT_SUCCESS;
}

//  A new usage function to make program more friendly
void usage(void)
{
	printf("Usage:\n Createsequence.exe <Number of components in file> <file name>.aaf \n");
	printf(" NB: Number is required to be integer greater than zero.\n");
	printf(" If only the number is given, the filename defaults to <number>.aaf\n\n");
}

//  Main adapted to use command-line arguments with argument checking
//  NOTE:  defining [0] program name; [1] Number N of components; [2] filename.aaf; 
int main(int argumentCount, char *argumentVector[])
{
	programName = argumentVector[0];
	
	//  First check for correct number of arguments
	//  printf("%ld\n",argumentCount);
	if ((argumentCount < 2) || (argumentCount > 3))
	{	
		usage();
		return 0;
	}
	//  Processing the second argument to be stored as global variable N
	char* Ns = argumentVector[1];
	char* expectedEnd = &Ns[strlen(Ns)];
	char* end = 0;
	long int N = strtoul(Ns, &end, 10);

	//  Testing for correct second argument
	if ((end != expectedEnd) || (N < 1))
	{ 
		printf("The first argument was of the incorrect form. [%s]\n\n",argumentVector[1]);
		usage();
		return 0;
	}
	
	//  With no second argument, set output filename to CreateSequence<N>.aaf
	if (argumentCount ==2)
	{
		niceFileName = Ns;
		strcat (niceFileName,".aaf");
	}
	else 
	{
	//  Otherwise output to filename specified in the second argument
	//  NB this case must have argC ==3 from earlier check

	niceFileName = argumentVector[2];
	strcat(niceFileName,".aaf");
	}
	//  and then carry on...

	UTLInitTimers(1000);
	CComInitialize comInit;
	
	aafWChar FileNameBuffer[MAX];
	mbstowcs(FileNameBuffer,niceFileName,MAX);
	
	aafWChar * pwFileName = FileNameBuffer;
	const char * pFileName = niceFileName;

	//  Give a nice output here too...
	printf("Creating file %s with %ld components...\n\n",niceFileName,N);
	checkFatal(CreateAAFFile(pwFileName, N));
	
	// Open the file and gather statistics
	ReadAAFFile(pwFileName);

	return(0);
}

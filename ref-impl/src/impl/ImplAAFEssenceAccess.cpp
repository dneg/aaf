/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/

#ifndef __ImplAAFMasterMob_h__
#include "ImplAAFMasterMob.h"
#endif

#ifndef __ImplAAFSourceMob_h__
#include "ImplAAFSourceMob.h"
#endif

#ifndef __ImplAAFSourceClip_h__
#include "ImplAAFSourceClip.h"
#endif

#ifndef __ImplAAFEssenceFormat_h__
#include "ImplAAFEssenceFormat.h"
#endif

#include "ImplAAFNetworkLocator.h"




#ifndef __ImplAAFEssenceAccess_h__
#include "ImplAAFEssenceAccess.h"
#endif


#include "ImplAAFFindSourceInfo.h"
#include "ImplEnumAAFEssenceData.h"
#include "ImplAAFContainerDef.h"
#include "ImplEnumAAFPluginDescriptors.h"
#include "ImplAAFDictionary.h"

#include <assert.h>
#include <string.h>

#ifndef __AAFRoot_h__
#include "AAFRoot.h"
#endif

#ifndef __AAFPlugin_h__
#include "AAFPlugin.h"
#endif

#include "ImplAAFPluginManager.h"
#include "AAFUtils.h"
#include "aafCvt.h"
#include "AAFUtils.h"
#include "ImplAAFHeader.h"
#include "ImplAAFSourceMob.h"
#include "ImplAAFSourceMob.h"
#include "ImplAAFFileDescriptor.h"
#include "AAFDefUIDs.h"
#include "ImplAAFObjectCreation.h"
#include "ImplAAFContext.h"
#include "ImplAAFWAVEDescriptor.h"	//!!!
#include "ImplEnumAAFLocators.h"
#include "ImplAAFFile.h"
#include "AAFStoredObjectIDs.h"
#include "AAFContainerDefs.h"
#include "AAFCodecDefs.h"

#define DEFAULT_FILE_SLOT	1

extern "C" const aafClassID_t CLSID_AAFEssenceFormat;
const CLSID CLSID_AAFEssenceDataStream = { 0x42A63FE1, 0x968A, 0x11d2, { 0x80, 0x89, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };

extern "C" const CLSID CLSID_AAFFile;

ImplAAFEssenceAccess::ImplAAFEssenceAccess ()
{
	_destination = NULL;

	_fileFormat = ContainerAAF;
	_codecID = NoCodec;
	_variety = NilCodecFlavour;
	_destination = NULL;
	_compFileMob = NULL;
	_numChannels = 0;
	_channels = NULL;
	_masterMob = NULL;
	_mdes = NULL;
	_codec = NULL;
	_stream = NULL;
//	_openType;
	_codecDescriptor = NULL;
	_dataFile = NULL;
	_dataFileMob = NULL;
}


ImplAAFEssenceAccess::~ImplAAFEssenceAccess ()
{
	if (_dataFile != NULL)
	{
		_dataFile->Close();		///!!!
		_dataFile->ReleaseReference();
	}
	if(_destination != NULL)
		_destination->ReleaseReference();
	if(_compFileMob != NULL)
		_compFileMob->ReleaseReference();
	if(_masterMob != NULL)
		_masterMob->ReleaseReference();
	if(_channels != NULL)
		delete [] _channels;
	if(_mdes != NULL)
		_mdes->ReleaseReference();
	if(_dataFileMob != NULL)
		_dataFileMob->ReleaseReference();
	if(_stream != NULL)
		_stream->Release();
	if(_codecDescriptor != NULL)
		_codecDescriptor->Release();
	if(_codec != NULL)
	{
		// In create(), the refcount was artificially lowerered.  Set it back up here 
		// so that the codec can release it's reference on us without calling delete again.
		AcquireReference();
		AcquireReference();
		_codec->Release();
	}
/*
	if(_dataFile != NULL)
		delete _dataFile;
*/
}	

/**ImplementationPrivate**/
AAFRESULT STDMETHODCALLTYPE
ImplAAFEssenceAccess::Create (	  ImplAAFMasterMob *masterMob,
							aafSlotID_t		masterSlotID,
							  aafUID_t			mediaKind,
							  aafUID_t			codecID,
							  aafRational_t	editRate,
							  aafRational_t	sampleRate,
							  aafCompressEnable_t  enable)
{
	IUnknown				*dataObj = NULL;
	IAAFSourceMob			*iFileMob = NULL;
	IAAFEssenceAccess		*iAccess = NULL;
	IUnknown				*iUnk = NULL;
	IAAFEssenceDataStream	*edStream = NULL;
	IAAFPlugin				*plugin = NULL;
	IAAFEssenceContainer	*container = NULL;
	IAAFPlugin				*plug = NULL;
	aafUID_t			fileMobUID;
	aafLength_t			oneLength = CvtInt32toLength(1, oneLength);
	AAFRESULT			aafError = AAFRESULT_SUCCESS;
	ImplAAFDictionary	*dataDict = NULL;
	ImplAAFHeader		*compHead = NULL;
	ImplAAFHeader		*dataHead = NULL;
	ImplAAFPluginManager *plugins = NULL;
	ImplAAFEssenceData	*implData = NULL;
	aafmMultiCreate_t	createBlock;
	wchar_t				*nameBuf = NULL;
	aafInt32			buflen;
	aafUID_t			aafFormat = ContainerAAF;

	XPROTECT()
	{
		_openType = kAAFCreated;
		_dataFile = NULL;
		
		CHECK(masterMob->MyHeadObject(&compHead));
		
		if((_destination != NULL) && EqualAUID(&_fileFormat, &ContainerAAF))
		{
			CHECK(CreateEssenceFileFromLocator (compHead, _destination, &_dataFile));
			CHECK(_dataFile->GetHeader(&dataHead));
		}
		else
		{
			CHECK(masterMob->MyHeadObject(&dataHead));
		}
		
		// Can't put raw media inside of an AAF File
		if(_destination == NULL && !EqualAUID(&_fileFormat, &aafFormat))
			RAISE(AAFRESULT_WRONG_MEDIATYPE);
		
		
		_masterMob = masterMob;
		_masterMob->AcquireReference();
		_channels = (aafSubChannel_t *) new aafSubChannel_t[1];
		XASSERT((_channels != NULL), AAFRESULT_NOMEMORY);
		_numChannels = 1;		 
		_channels[0].mediaKind = mediaKind;
		_channels[0].trackID = masterSlotID;
		_channels[0].physicalOutChan = 1;
		
		_codecID = codecID;
		
		
		plugins = ImplAAFContext::GetInstance()->GetPluginManager();
		//!!!Handle case where multiple codecs exist for a codecID
		CHECK(plugins->GetPluginInstance(_codecID, &plugin));
		CHECK(plugin->QueryInterface(IID_IAAFEssenceCodec, (void **)&_codec));
		plugin->Release();
		plugin = NULL;
		
		createBlock.mediaKind = &mediaKind;
		createBlock.subTrackNum = 0;		//!!!
		createBlock.slotID = DEFAULT_FILE_SLOT;
		createBlock.sampleRate = sampleRate;
		
		iUnk = static_cast<IUnknown *> (this->GetContainer());
		CHECK(iUnk->QueryInterface(IID_IAAFEssenceAccess, (void **)&iAccess));
		iUnk->Release();
		iUnk = NULL;
		CHECK(_codec->SetEssenceAccess(iAccess));
		// Now _codec is holding onto a reference to THIS, and we have a reference to _codec
		// Creating a cycle which will never free.  I am decrementing the AAFEssenceAccess reference
		//  Count  artificially by one, so that when EssenceAccess is Released, both objects will
		// be deleted (the correct behavior).
		iAccess->Release();
		
		// When we enable the cloneExternal (below) then Don't do this call for creating the
		// file mob twice
		CHECK(CreateFileMob(compHead, AAFTrue, DEFAULT_FILE_SLOT, NULL, mediaKind, editRate, sampleRate,
			_destination, &_compFileMob));
		CHECK(_compFileMob->GetMobID(&fileMobUID));
		if(compHead != dataHead)
		{
			CHECK(CreateFileMob(dataHead, AAFTrue, DEFAULT_FILE_SLOT, &fileMobUID, mediaKind, editRate, sampleRate,
				NULL, &_dataFileMob));
			_dataFileMob->AcquireReference();	//!!!Leaking here?
		}
		else
			_dataFileMob = NULL;
		
		CHECK(masterMob->AddMasterSlot (&mediaKind, DEFAULT_FILE_SLOT, _compFileMob, masterSlotID, L"A Slot"));	// Should be NULL or something useful!!!
		if(_destination != NULL)
		{
			if(EqualAUID(&_fileFormat, &aafFormat))
			{
				//!!!				ImplAAFMob	*destmob;
				
				//!!!				CHECK(CloneExternal (kFollowDepend, kNoIncludeMedia, _destFile, &destMob));
			}
		}
		
		CHECK(CreateCodecDef(compHead, codecID, &_codecDescriptor));
		CHECK(CreateContainerDef(compHead))
		//!!! As an optimization, use clone to move a copy of the definition objects into the media file.
		// This can be just a copy because there shouldn't be any definitions there yet...

		if((dataHead != compHead) && (dataHead != NULL))
		{
			CHECK(CreateContainerDef(dataHead))
			CHECK(CreateCodecDef(dataHead, codecID, NULL));
		}
		
		// There isn't yet a container for AAF data, so this must be special-cased
		if(EqualAUID(&_fileFormat, &aafFormat))
		{
			aafUID_t	essenceDataID;
			
			CHECK(_codec->GetEssenceDataID(&essenceDataID));
			CHECK(dataHead->GetDictionary(&dataDict));
			CHECK(dataDict->CreateInstance(&essenceDataID, (ImplAAFObject **)&implData));
			dataObj = static_cast<IUnknown *> (implData->GetContainer());
			
			CHECK(implData->SetFileMob(_dataFileMob == NULL ? _compFileMob : _dataFileMob));
			CHECK(dataHead->AppendEssenceData (implData));
			
			CHECK(plugins->CreateInstance(CLSID_AAFEssenceDataStream,
				NULL, 
				IID_IAAFEssenceStream, 
				(void **)&_stream));
//			_stream->AddRef();
			CHECK(_stream->QueryInterface(IID_IAAFEssenceDataStream, (void **)&edStream));
			edStream->Init(dataObj);
			edStream->Release();
			edStream = NULL;
		}
		else
		{
			_destination->GetPathBufLen(&buflen);
			nameBuf = new wchar_t[buflen];
			if(nameBuf == NULL)
				RAISE(AAFRESULT_NOMEMORY);
			CHECK(_destination->GetPath(nameBuf, buflen));
			
			CHECK(plugins->GetPluginInstance(_fileFormat, &plug));
			CHECK(plug->QueryInterface(IID_IAAFEssenceContainer, (void **)&container));
			plug->Release();
			plug = NULL;
			
			CHECK(container->CreateEssenceStream(nameBuf, &fileMobUID, &_stream));
			delete nameBuf;
			nameBuf = NULL;			
			container->Release();
			container = NULL;
		}
		
		CHECK(compHead->SetModified());		// To NOW
		if(dataHead != compHead)
		{
			CHECK(dataHead->SetModified());	// To NOW
		}
		
		// Call the codec to create the actual media.
		//
		//!!!Assert this		if(_codec != NULL)
		
		createBlock.mediaKind = &mediaKind;
		createBlock.subTrackNum = 0;		//!!!
		createBlock.slotID = DEFAULT_FILE_SLOT;
		createBlock.sampleRate = sampleRate;
		
		iUnk = static_cast<IUnknown *> (_compFileMob->GetContainer());	// Codec knowns about compFilemob only
		CHECK(iUnk->QueryInterface(IID_IAAFSourceMob, (void **)&iFileMob));
		iUnk->Release();
		iUnk= NULL;
		CHECK(_codec->Create(iFileMob, _variety, _stream, 1, &createBlock));
		(void)(_codec->SetCompressionEnabled(enable == kSDKCompressionEnable ? AAFTrue : AAFFalse));

//		if(dataObj != NULL)
//		{
//			dataObj->Release();
//			dataObj = NULL;
//		}
		if(dataDict != NULL)
		{
			dataDict->ReleaseReference();
			dataDict = NULL;
		}
		if(compHead != NULL)
		{
			compHead->ReleaseReference();
			compHead = NULL;
		}
		if(dataHead != NULL)
		{
			dataHead->ReleaseReference();
			dataHead = NULL;
		}
		if(plugins != NULL)
		{
			plugins->ReleaseReference();
			plugins = NULL;
		}
		if(implData != NULL)
		{
			implData->ReleaseReference();
			implData = NULL;
		}
	}
	XEXCEPT
	{
		if(dataDict != NULL)
			dataDict->ReleaseReference();
		if(compHead != NULL)
			compHead->ReleaseReference();
		if(dataHead != NULL)
			dataHead->ReleaseReference();
		if(plugins != NULL)
			plugins->ReleaseReference();
		if(implData != NULL)
			implData->ReleaseReference();
		if(nameBuf != NULL)
			delete nameBuf;
		if(edStream != NULL)
			edStream->Release();
		if(iAccess != NULL)
			iAccess->Release();
//		if(dataObj != NULL)
//			dataObj->Release();
		if(plugin != NULL)
			plugin->Release();
		if(container != NULL)
			container->Release();
		if(plug != NULL)
			plug->Release();
		_dataFile = NULL;
	}
	XEND
		
		return (AAFRESULT_SUCCESS);
}

	//@comm Creates a single channel stream of essence.  Convenience functions
	// exist to create audio or video essence, and a separate call
	// (MultiCreate) exists to create interleaved audio and
	// video data.
	//@comm The essence handle from this call can be used with
	// WriteDataSamples  and possibly WriteDataLines, but NOT with
	// WriteMultiSamples.
	//@comm If you are creating the essence, and then attaching it to a master
	// mob, then the "masterMob" field may be left NULL.
	// For video, the sampleRate should be the edit rate of the file mob.
	// For audio, the sample rate should be the actual samples per second.
	//@comm Replaces omfmMediaCreate
	
/****/
AAFRESULT STDMETHODCALLTYPE
   ImplAAFEssenceAccess::MultiCreate (ImplAAFMasterMob *masterMob,
							aafUID_t codecID,
                          aafInt16  arrayElemCount,
                           aafmMultiCreate_t *  mediaArray,
                           aafCompressEnable_t  Enable)
{
	IUnknown				*dataObj = NULL;
	IAAFSourceMob			*iFileMob = NULL;
	IAAFEssenceAccess		*iAccess = NULL;
	IUnknown				*iUnk = NULL;
	IAAFEssenceDataStream	*edStream = NULL;
	IAAFPlugin				*plugin = NULL;
	IAAFEssenceContainer	*container = NULL;
	IAAFPlugin				*plug = NULL;
	aafUID_t				fileMobUID, essenceKind;
	aafLength_t				oneLength = CvtInt32toLength(1, oneLength);
	AAFRESULT				aafError = AAFRESULT_SUCCESS;
	ImplAAFDictionary		*dataDict = NULL;
	ImplAAFHeader			*compHead = NULL;
	ImplAAFHeader			*dataHead = NULL;
	ImplAAFPluginManager	*plugins = NULL;
	ImplAAFEssenceData		*implData = NULL;
	ImplAAFMobSlot			*tmpTrack = NULL;
	ImplAAFSourceMob		*fileMob = NULL;
	aafSubChannel_t			*destPtr;
	wchar_t					*nameBuf = NULL;
	aafInt32				buflen, n;
	aafUID_t				aafFormat = ContainerAAF;
	aafmMultiCreate_t		*initPtr;
	aafRational_t			sampleRate;

	XPROTECT()
	{
		_openType = kAAFCreated;
		_dataFile = NULL;
		
		CHECK(masterMob->MyHeadObject(&compHead));
		
		if((_destination != NULL) && EqualAUID(&_fileFormat, &ContainerAAF))
		{
			CHECK(CreateEssenceFileFromLocator (compHead, _destination, &_dataFile));
			CHECK(_dataFile->GetHeader(&dataHead));
		}
		else
		{
			CHECK(masterMob->MyHeadObject(&dataHead));
		}
		
		// Can't put raw media inside of an AAF File
		if(_destination == NULL && !EqualAUID(&_fileFormat, &aafFormat))
			RAISE(AAFRESULT_WRONG_MEDIATYPE);
		
		
		_masterMob = masterMob;
		_masterMob->AcquireReference();
		
		_numChannels = arrayElemCount;
		_channels = (aafSubChannel_t *) new aafSubChannel_t[_numChannels];
		XASSERT((_channels != NULL), AAFRESULT_NOMEMORY);

		
		// When we enable the cloneExternal (below) then Don't do this call for creating the
		// file mob twice.
		// Don't add slots here, as they will be added in the loop below
		sampleRate = mediaArray[0].sampleRate;		// !!! Assumes that edit rate == sample rate of channel 1
		essenceKind = *(mediaArray[0].mediaKind);	// Assumes that mediaKind somes from track #1
		CHECK(CreateFileMob(compHead, AAFFalse, 0, NULL, essenceKind, sampleRate, sampleRate,
			_destination, &_compFileMob));
		CHECK(_compFileMob->GetMobID(&fileMobUID));
		if(compHead != dataHead)
		{
			CHECK(CreateFileMob(dataHead, AAFFalse, 0, &fileMobUID, essenceKind, sampleRate, sampleRate,
				NULL, &_dataFileMob));
			_dataFileMob->AcquireReference();	//!!!Leaking here?
		}
		else
			_dataFileMob = NULL;

		
		for (n = 0; n < arrayElemCount; n++)
		{
			initPtr = mediaArray + n;
			destPtr = _channels + n;
//!!!			if(isAAFMedia())
			{
			/* JeffB: Handle the case where an existing file=>tape mob connection exists
				*/
				if(fileMob->FindSlotBySlotID(initPtr->slotID, &tmpTrack) == AAFRESULT_SLOT_NOT_FOUND)
				{
					// !!! Assumes that the sampleRate == editRate
					CHECK(fileMob->AddNilReference(
						initPtr->slotID, oneLength, initPtr->mediaKind, initPtr->sampleRate));
					tmpTrack->ReleaseReference();
					tmpTrack = NULL;
				}
				CHECK(fileMob->FindSlotBySlotID(initPtr->slotID, &tmpTrack));
				CHECK(tmpTrack->SetPhysicalNum(initPtr->subTrackNum));
			}
			destPtr->mediaKind = *(initPtr->mediaKind);
			destPtr->trackID = initPtr->slotID;
			destPtr->physicalOutChan = initPtr->subTrackNum;
			CHECK(masterMob->AddMasterSlot (&essenceKind, initPtr->slotID, _compFileMob, initPtr->slotID, L"A Slot"));	// Should be NULL or something useful!!!
//!!!			CvtInt32toPosition(0, destPtr->dataOffset);
//!!!			CvtInt32toLength(0, destPtr->numSamples);
//!!!			destPtr->sampleRate = initPtr->sampleRate;
		}
		_codecID = codecID;
		
		
		plugins = ImplAAFContext::GetInstance()->GetPluginManager();
		//!!!Handle case where multiple codecs exist for a codecID
		CHECK(plugins->GetPluginInstance(_codecID, &plugin));
		CHECK(plugin->QueryInterface(IID_IAAFEssenceCodec, (void **)&_codec));
		plugin->Release();
		plugin = NULL;
				
		iUnk = static_cast<IUnknown *> (this->GetContainer());
		CHECK(iUnk->QueryInterface(IID_IAAFEssenceAccess, (void **)&iAccess));
		iUnk->Release();
		iUnk = NULL;
		CHECK(_codec->SetEssenceAccess(iAccess));
		// Now _codec is holding onto a reference to THIS, and we have a reference to _codec
		// Creating a cycle which will never free.  I am decrementing the AAFEssenceAccess reference
		//  Count  artificially by one, so that when EssenceAccess is Released, both objects will
		// be deleted (the correct behavior).
		iAccess->Release();
		
		
		if(_destination != NULL)
		{
			if(EqualAUID(&_fileFormat, &aafFormat))
			{
				//!!!				ImplAAFMob	*destmob;
				
				//!!!				CHECK(CloneExternal (kFollowDepend, kNoIncludeMedia, _destFile, &destMob));
			}
		}
		
		CHECK(CreateCodecDef(compHead, codecID, &_codecDescriptor));
		CHECK(CreateContainerDef(compHead))
			//!!! As an optimization, use clone to move a copy of the definition objects into the media file.
			// This can be just a copy because there shouldn't be any definitions there yet...
			
			if((dataHead != compHead) && (dataHead != NULL))
			{
				CHECK(CreateContainerDef(dataHead))
					CHECK(CreateCodecDef(dataHead, codecID, NULL));
			}
			
			// There isn't yet a container for AAF data, so this must be special-cased
			if(EqualAUID(&_fileFormat, &aafFormat))
			{
				aafUID_t	essenceDataID;
				
				CHECK(_codec->GetEssenceDataID(&essenceDataID));
				CHECK(dataHead->GetDictionary(&dataDict));
				CHECK(dataDict->CreateInstance(&essenceDataID, (ImplAAFObject **)&implData));
				dataObj = static_cast<IUnknown *> (implData->GetContainer());
				
				CHECK(implData->SetFileMob(_dataFileMob == NULL ? _compFileMob : _dataFileMob));
				CHECK(dataHead->AppendEssenceData (implData));
				
				CHECK(plugins->CreateInstance(CLSID_AAFEssenceDataStream,
					NULL, 
					IID_IAAFEssenceStream, 
					(void **)&_stream));
				//			_stream->AddRef();
				CHECK(_stream->QueryInterface(IID_IAAFEssenceDataStream, (void **)&edStream));
				edStream->Init(dataObj);
				edStream->Release();
				edStream = NULL;
			}
			else
			{
				_destination->GetPathBufLen(&buflen);
				nameBuf = new wchar_t[buflen];
				if(nameBuf == NULL)
					RAISE(AAFRESULT_NOMEMORY);
				CHECK(_destination->GetPath(nameBuf, buflen));
				
				CHECK(plugins->GetPluginInstance(_fileFormat, &plug));
				CHECK(plug->QueryInterface(IID_IAAFEssenceContainer, (void **)&container));
				plug->Release();
				plug = NULL;
				
				CHECK(container->CreateEssenceStream(nameBuf, &fileMobUID, &_stream));
				delete nameBuf;
				nameBuf = NULL;			
				container->Release();
				container = NULL;
			}
			
			CHECK(compHead->SetModified());		// To NOW
			if(dataHead != compHead)
			{
				CHECK(dataHead->SetModified());	// To NOW
			}
						
			iUnk = static_cast<IUnknown *> (_compFileMob->GetContainer());	// Codec knowns about compFilemob only
			CHECK(iUnk->QueryInterface(IID_IAAFSourceMob, (void **)&iFileMob));
			iUnk->Release();
			iUnk= NULL;
			CHECK(_codec->Create(iFileMob, _variety, _stream, arrayElemCount, mediaArray));
			(void)(_codec->SetCompressionEnabled(Enable == kSDKCompressionEnable ? AAFTrue : AAFFalse));
			
			//		if(dataObj != NULL)
			//		{
			//			dataObj->Release();
			//			dataObj = NULL;
			//		}
			if(dataDict != NULL)
			{
				dataDict->ReleaseReference();
				dataDict = NULL;
			}
			if(compHead != NULL)
			{
				compHead->ReleaseReference();
				compHead = NULL;
			}
			if(dataHead != NULL)
			{
				dataHead->ReleaseReference();
				dataHead = NULL;
			}
			if(plugins != NULL)
			{
				plugins->ReleaseReference();
				plugins = NULL;
			}
			if(implData != NULL)
			{
				implData->ReleaseReference();
				implData = NULL;
			}
			if(tmpTrack != NULL)
			{
				tmpTrack->ReleaseReference();
				tmpTrack = NULL;
			}
	}
	XEXCEPT
	{
		if(fileMob != NULL)
			fileMob->ReleaseReference();
		if(tmpTrack != NULL)
			tmpTrack->ReleaseReference();
		if(dataDict != NULL)
			dataDict->ReleaseReference();
		if(compHead != NULL)
			compHead->ReleaseReference();
		if(dataHead != NULL)
			dataHead->ReleaseReference();
		if(plugins != NULL)
			plugins->ReleaseReference();
		if(implData != NULL)
			implData->ReleaseReference();
		if(nameBuf != NULL)
			delete nameBuf;
		if(edStream != NULL)
			edStream->Release();
		if(iAccess != NULL)
			iAccess->Release();
		//		if(dataObj != NULL)
		//			dataObj->Release();
		if(plugin != NULL)
			plugin->Release();
		if(container != NULL)
			container->Release();
		if(plug != NULL)
			plug->Release();
		_dataFile = NULL;
	}
	XEND
		
	return (AAFRESULT_SUCCESS);
}

/**ImplementationPrivate**/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::Open (ImplAAFMasterMob *masterMob,
							aafSlotID_t  slotID,
                           aafMediaCriteria_t*mediaCrit,
                           aafMediaOpenMode_t  openMode,
                           aafCompressEnable_t  compEnable)
{
	IAAFEssenceDataStream	*edStream = NULL;
	IUnknown				*edUnknown = NULL;
	IAAFEssenceContainer	*container = NULL;
	IAAFPlugin				*plug = NULL;
	IUnknown				*iUnk = NULL;
	IAAFSourceMob			*iFileMob = NULL;
	IAAFEssenceAccess		*iAccess = NULL;
	ImplAAFDictionary		*dict = NULL;
	ImplAAFContainerDef		*containerDef = NULL;
	ImplAAFContentStorage	*cStore = NULL;
	ImplAAFMobSlot			*slot = NULL;
	ImplAAFSegment			*seg = NULL;
	ImplAAFFindSourceInfo	*sourceInfo = NULL;
	ImplAAFSourceMob		*fileMob = NULL;
	ImplAAFHeader			*dataHead = NULL;
	ImplAAFEssenceData		*essenceData = NULL;
	ImplAAFPluginManager	*plugins = NULL;
	ImplAAFHeader			*compHead = NULL;
	ImplEnumAAFLocators		*enumLocate = NULL;
	ImplAAFLocator			*pLoc = NULL;
	aafPosition_t	zeroPos;
	aafInt32		n;
	aafUID_t		 mediaKind, fileMobID, myFileCLSID;
	aafLength_t masterMobLength, one;
	aafSourceRef_t	fileRef;
	aafInt16		numCh;
	wchar_t				*nameBuf = NULL;
	aafInt32			buflen;
	aafUID_t			essenceDescClass;
	aafBool					found = AAFFalse, isIdentified;
	aafUID_t				testFormat;
	
	XPROTECT()
	{
		CHECK(masterMob->MyHeadObject(&compHead));

		_openType = kAAFReadOnly;
		CvtInt32toPosition(0, zeroPos);	
		CvtInt32toLength(1, one);
		CHECK(masterMob->SearchSource(slotID, zeroPos,kFileMob,
									   mediaCrit,
									   NULL,
									   &sourceInfo));
//		masterMob->AcquireReference();		//!!!DEBUG
									   
		CHECK(masterMob->FindSlotBySlotID (slotID,&slot));
		CHECK(slot->GetSegment(&seg));
		slot->ReleaseReference();
		slot = NULL;

		CHECK(seg->GetDataDef(&mediaKind));
		CHECK(seg->GetLength(&masterMobLength));
		seg->ReleaseReference();
		seg = NULL;
		
		CHECK(sourceInfo->GetMob((ImplAAFMob **)&fileMob));
		CHECK(sourceInfo->GetSourceReference(&fileRef));
		sourceInfo->ReleaseReference();
		sourceInfo = NULL;

		CHECK(fileMob->GetEssenceDescriptor((ImplAAFEssenceDescriptor **)&_mdes));
		CHECK(fileMob->GetMobID(&fileMobID));
		CHECK(_mdes->GetObjectClass(&essenceDescClass));

		plugins = ImplAAFContext::GetInstance()->GetPluginManager();
		CHECK(plugins->MakeCodecFromEssenceDesc(essenceDescClass, &_codec));


		iUnk = static_cast<IUnknown *> (this->GetContainer());
		CHECK(iUnk->QueryInterface(IID_IAAFEssenceAccess, (void **)&iAccess));
		iUnk->Release();
		iUnk = NULL;
		CHECK(_codec->SetEssenceAccess(iAccess));
		// Now _codec is holding onto a reference to THIS, and we have a reference to _codec
		// Creating a cycle which will never free.  I am decrementing the AAFEssenceAccess reference
		//  Count  artificially by one, so that when EssenceAccess is Released, both objects will
		// be deleted (the correct behavior).
		iAccess->Release();

//!!!		_physicalOutChanOpen = physicalOutChan;
		

		CHECK(_mdes->GetContainerFormat (&testFormat));
		_fileFormat = testFormat;


		CHECK(compHead->GetDictionary (&dict));
		CHECK(dict->LookupContainerDefinition (&testFormat, &containerDef));
		dict->ReleaseReference();
		dict = NULL;

		found = AAFFalse;
		CHECK(containerDef->EssenceIsIdentified (&isIdentified));
		containerDef->ReleaseReference();
		containerDef = NULL;

		if(isIdentified)
		{						
			CHECK(fileMob->GetMobID(&fileMobID));
			CHECK(compHead->GetContentStorage (&cStore));
			if(cStore->LookupEssence (&fileMobID, &essenceData) == AAFRESULT_SUCCESS)
			{
				found = AAFTrue;
				CHECK(plugins->CreateInstance(CLSID_AAFEssenceDataStream,
					NULL, 
					IID_IAAFEssenceStream, 
					(void **)&_stream));
		
				CHECK(_stream->QueryInterface(IID_IAAFEssenceDataStream, (void **)&edStream));
				// This only works with a COM API
				edUnknown = static_cast<IUnknown *> (essenceData->GetContainer());
				edStream->Init(edUnknown);
				edStream->Release();
				edStream = NULL;
				essenceData->ReleaseReference();
				essenceData = NULL;
			}
			cStore->ReleaseReference();
			cStore = NULL;
		}

		if(found == AAFFalse)
		{
			AAFRESULT				status;

			CHECK(_mdes->EnumAAFAllLocators(&enumLocate));
			while(!found && (enumLocate->NextOne (&pLoc) == AAFRESULT_SUCCESS))
			{
				
				pLoc->GetPathBufLen(&buflen);
				nameBuf = new wchar_t[buflen];
				if(nameBuf == NULL)
					RAISE(AAFRESULT_NOMEMORY);
				CHECK(pLoc->GetPath(nameBuf, buflen));
				if(EqualAUID(&_fileFormat, &ContainerAAF) == AAFTrue)
				{
					if(openMode == kMediaOpenAppend)
						status = ModifyEssenceFileFromLocator (compHead, pLoc, &_dataFile);
					else
					{
						memcpy((void *)&myFileCLSID, (void *)&CLSID_AAFFile, sizeof(aafUID_t));
						_dataFile = (ImplAAFFile *)CreateImpl(myFileCLSID);
						CHECK(_dataFile->Initialize());
						status = _dataFile->OpenExistingRead(nameBuf, 0);
					}

					if(status == AAFRESULT_SUCCESS)
					{												
						CHECK(_dataFile->GetHeader(&dataHead));
						CHECK(dataHead->GetContentStorage (&cStore));
						if(cStore->LookupEssence (&fileMobID, &essenceData) == AAFRESULT_SUCCESS)
							found = AAFTrue;
						cStore->ReleaseReference();
						cStore = NULL;

						if(found && EqualAUID(&_fileFormat, &ContainerAAF))
						{
							CHECK(plugins->CreateInstance(CLSID_AAFEssenceDataStream,
								NULL, 
								IID_IAAFEssenceStream, 
								(void **)&_stream));
							
							CHECK(_stream->QueryInterface(IID_IAAFEssenceDataStream, (void **)&edStream));
							// This only works with a COM API
							edUnknown = static_cast<IUnknown *> (essenceData->GetContainer());
							edStream->Init(edUnknown);
							edStream->Release();
							edStream = NULL;
							essenceData->ReleaseReference();
							essenceData = NULL;
						}
						dataHead->ReleaseReference();
						dataHead = NULL;
					}
					else
					{
						_dataFile->ReleaseReference();
						_dataFile = NULL;
					}
				}
				else
				{
					CHECK(plugins->GetPluginInstance(_fileFormat, &plug));
					CHECK(plug->QueryInterface(IID_IAAFEssenceContainer, (void **)&container));
					
					if(openMode == kMediaOpenReadOnly)
					{
						status = container->OpenEssenceStreamReadOnly (nameBuf, &fileMobID, &_stream);
					}
					else if(openMode == kMediaOpenAppend)
					{
						status = container->OpenEssenceStreamAppend (nameBuf, &fileMobID, &_stream);
					}
					else
						RAISE(AAFRESULT_MEDIA_OPENMODE);
					
					if(status == AAFRESULT_SUCCESS)
						found = AAFTrue;
					container->Release();
					container = NULL;
					plug->Release();
					plug = NULL;
				}

				delete nameBuf;
				nameBuf = NULL;
				pLoc->ReleaseReference();
				pLoc = NULL;
			}
			enumLocate->ReleaseReference();
			enumLocate = NULL;
			if(!found)
				RAISE(AAFRESULT_MEDIA_NOT_FOUND);
		}


		iUnk = static_cast<IUnknown *> (fileMob->GetContainer());	// Codec knowns about compFilemob only
		CHECK(iUnk->QueryInterface(IID_IAAFSourceMob, (void **)&iFileMob));
		iUnk->Release();
		iUnk= NULL;
		CHECK(_codec->GetNumChannels(iFileMob, mediaKind, _stream, &numCh));
		if (numCh == 0)
		  RAISE(AAFRESULT_INVALID_DATADEF);
		
		_channels = (aafSubChannel_t *) new aafSubChannel_t[1];
		if(_channels == NULL)
			RAISE(AAFRESULT_NOMEMORY);
		_numChannels = numCh;
		for(n = 0; n < numCh; n++)
		{
			_channels[n].mediaKind = mediaKind;
			_channels[n].physicalOutChan = n+1;
			_channels[n].trackID = slotID+n;
		}

		iUnk = static_cast<IUnknown *> (fileMob->GetContainer());	// Codec knowns about compFilemob only
		CHECK(iUnk->QueryInterface(IID_IAAFSourceMob, (void **)&iFileMob));
		iUnk->Release();
		iUnk= NULL;
		CHECK(_codec->Open(iFileMob, slotID, openMode, _stream));
		
		_masterMob = masterMob;
		_masterMob->AcquireReference();
		fileMob->ReleaseReference();
		fileMob = NULL;
		compHead->ReleaseReference();
		compHead = NULL;
		if(plugins != NULL)
			plugins->ReleaseReference();
	}
	XEXCEPT
	{
		if(_dataFile != NULL)
			_dataFile->ReleaseReference();
		_dataFile = NULL;
		if(plugins != NULL)
			plugins->ReleaseReference();
		if(edStream != NULL)
			edStream->Release();
		if(iUnk != NULL)
			iUnk->Release();
		if(iAccess != NULL)
			iAccess->Release();
//		if(edUnknown != NULL)
//			edUnknown->Release();
		if(container != NULL)
			container->Release();
		if(plug != NULL)
			plug->Release();
		if(dict != NULL)
			dict->ReleaseReference();
		if(containerDef != NULL)
			containerDef->ReleaseReference();
		if(cStore != NULL)
			cStore->ReleaseReference();
		if(slot != NULL)
			slot->ReleaseReference();
		if(seg != NULL)
			seg->ReleaseReference();
		if(sourceInfo != NULL)
			sourceInfo->ReleaseReference();
		if(fileMob != NULL)
			fileMob->ReleaseReference();
		if(dataHead != NULL)
			dataHead->ReleaseReference();
		if(essenceData != NULL)
			essenceData->ReleaseReference();
		if(compHead != NULL)
			compHead->ReleaseReference();
		if(enumLocate != NULL)
			enumLocate->ReleaseReference();
		if(pLoc != NULL)
			pLoc->ReleaseReference();
	}
	XEND
	
	return (AAFRESULT_SUCCESS);
}

	//@comm If the essence is interleaved,
	// then it will be di-interleaved when samples are read.  This routine
	// follows the locator, and may call the locator failure callback if
	// the essence can not be found.  If the failure callback finds the essence,
	// then this routine will return normally.
	//@comm The essence handle from this call can be used with
	// ReadDataSamples  and possibly ReadDataLines, but NOT with
	// ReadMultiSamples.
	//@comm Possible Errors:
	// 	Standard errors (see top of file).
	// 	AAFRESULT_NOMEMORY -- couldn't allocate memory for the essence handle
	//@comm NOTE: If a locator is followed, then essencePtr may reference ANOTHER file
	// object, which must be closed on file close.
	//@comm Replaces omfmMediaOpen*/
	
/**ImplementationPrivate**/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::MultiOpen (ImplAAFMasterMob *masterMob,
                           aafSlotID_t  slotID,
                           aafMediaCriteria_t*  mediaCrit,
                           aafMediaOpenMode_t  openMode,
                           aafCompressEnable_t compEnable)
{
	IAAFEssenceDataStream	*edStream = NULL;
	IUnknown				*edUnknown = NULL;
	IAAFEssenceContainer	*container = NULL;
	IAAFPlugin				*plug = NULL;
	IUnknown				*iUnk = NULL;
	IAAFSourceMob			*iFileMob = NULL;
	IAAFEssenceAccess		*iAccess = NULL;
	ImplAAFDictionary		*dict = NULL;
	ImplAAFContainerDef		*containerDef = NULL;
	ImplAAFContentStorage	*cStore = NULL;
	ImplAAFMobSlot			*slot = NULL;
	ImplAAFSegment			*seg = NULL;
	ImplAAFFindSourceInfo	*sourceInfo = NULL;
	ImplAAFSourceMob		*fileMob = NULL;
	ImplAAFHeader			*dataHead = NULL;
	ImplAAFEssenceData		*essenceData = NULL;
	ImplAAFPluginManager	*plugins = NULL;
	ImplAAFHeader			*compHead = NULL;
	ImplEnumAAFLocators		*enumLocate = NULL;
	ImplAAFLocator			*pLoc = NULL;
	aafPosition_t	zeroPos;
	aafInt32		n;
	aafUID_t		 mediaKind, fileMobID, myFileCLSID;
	aafLength_t masterMobLength, one;
	aafSourceRef_t	fileRef;
	aafInt16		numCh;
	wchar_t				*nameBuf = NULL;
	aafInt32			buflen;
	aafUID_t			essenceDescClass;
	aafBool					found = AAFFalse, isIdentified;
	aafUID_t				testFormat;
	
	XPROTECT()
	{
		CHECK(masterMob->MyHeadObject(&compHead));

		_openType = kAAFReadOnly;
		CvtInt32toPosition(0, zeroPos);	
		CvtInt32toLength(1, one);
		CHECK(masterMob->SearchSource(slotID, zeroPos,kFileMob,
									   mediaCrit,
									   NULL,
									   &sourceInfo));
//		masterMob->AcquireReference();		//!!!DEBUG
									   
		CHECK(masterMob->FindSlotBySlotID (slotID,&slot));
		CHECK(slot->GetSegment(&seg));
		slot->ReleaseReference();
		slot = NULL;

		CHECK(seg->GetDataDef(&mediaKind));
		CHECK(seg->GetLength(&masterMobLength));
		seg->ReleaseReference();
		seg = NULL;
		
		CHECK(sourceInfo->GetMob((ImplAAFMob **)&fileMob));
		CHECK(sourceInfo->GetSourceReference(&fileRef));
		sourceInfo->ReleaseReference();
		sourceInfo = NULL;

		CHECK(fileMob->GetEssenceDescriptor((ImplAAFEssenceDescriptor **)&_mdes));
		CHECK(fileMob->GetMobID(&fileMobID));
		CHECK(_mdes->GetObjectClass(&essenceDescClass));

		plugins = ImplAAFContext::GetInstance()->GetPluginManager();
		CHECK(plugins->MakeCodecFromEssenceDesc(essenceDescClass, &_codec));


		iUnk = static_cast<IUnknown *> (this->GetContainer());
		CHECK(iUnk->QueryInterface(IID_IAAFEssenceAccess, (void **)&iAccess));
		iUnk->Release();
		iUnk = NULL;
		CHECK(_codec->SetEssenceAccess(iAccess));
		// Now _codec is holding onto a reference to THIS, and we have a reference to _codec
		// Creating a cycle which will never free.  I am decrementing the AAFEssenceAccess reference
		//  Count  artificially by one, so that when EssenceAccess is Released, both objects will
		// be deleted (the correct behavior).
		iAccess->Release();

//!!!		_physicalOutChanOpen = physicalOutChan;
		

		CHECK(_mdes->GetContainerFormat (&testFormat));
		_fileFormat = testFormat;


		CHECK(compHead->GetDictionary (&dict));
		CHECK(dict->LookupContainerDefinition (&testFormat, &containerDef));
		dict->ReleaseReference();
		dict = NULL;

		found = AAFFalse;
		CHECK(containerDef->EssenceIsIdentified (&isIdentified));
		containerDef->ReleaseReference();
		containerDef = NULL;

		if(isIdentified)
		{						
			CHECK(fileMob->GetMobID(&fileMobID));
			CHECK(compHead->GetContentStorage (&cStore));
			if(cStore->LookupEssence (&fileMobID, &essenceData) == AAFRESULT_SUCCESS)
			{
				found = AAFTrue;
				CHECK(plugins->CreateInstance(CLSID_AAFEssenceDataStream,
					NULL, 
					IID_IAAFEssenceStream, 
					(void **)&_stream));
		
				CHECK(_stream->QueryInterface(IID_IAAFEssenceDataStream, (void **)&edStream));
				// This only works with a COM API
				edUnknown = static_cast<IUnknown *> (essenceData->GetContainer());
				edStream->Init(edUnknown);
				edStream->Release();
				edStream = NULL;
				essenceData->ReleaseReference();
				essenceData = NULL;
			}
			cStore->ReleaseReference();
			cStore = NULL;
		}

		if(found == AAFFalse)
		{
			AAFRESULT				status;

			CHECK(_mdes->EnumAAFAllLocators(&enumLocate));
			while(!found && (enumLocate->NextOne (&pLoc) == AAFRESULT_SUCCESS))
			{
				
				pLoc->GetPathBufLen(&buflen);
				nameBuf = new wchar_t[buflen];
				if(nameBuf == NULL)
					RAISE(AAFRESULT_NOMEMORY);
				CHECK(pLoc->GetPath(nameBuf, buflen));
				if(EqualAUID(&_fileFormat, &ContainerAAF) == AAFTrue)
				{
					if(openMode == kMediaOpenAppend)
						status = ModifyEssenceFileFromLocator (compHead, pLoc, &_dataFile);
					else
					{
						memcpy((void *)&myFileCLSID, (void *)&CLSID_AAFFile, sizeof(aafUID_t));
						_dataFile = (ImplAAFFile *)CreateImpl(myFileCLSID);
						CHECK(_dataFile->Initialize());
						status = _dataFile->OpenExistingRead(nameBuf, 0);
					}

					if(status == AAFRESULT_SUCCESS)
					{												
						CHECK(_dataFile->GetHeader(&dataHead));
						CHECK(dataHead->GetContentStorage (&cStore));
						if(cStore->LookupEssence (&fileMobID, &essenceData) == AAFRESULT_SUCCESS)
							found = AAFTrue;
						cStore->ReleaseReference();
						cStore = NULL;

						if(found && EqualAUID(&_fileFormat, &ContainerAAF))
						{
							CHECK(plugins->CreateInstance(CLSID_AAFEssenceDataStream,
								NULL, 
								IID_IAAFEssenceStream, 
								(void **)&_stream));
							
							CHECK(_stream->QueryInterface(IID_IAAFEssenceDataStream, (void **)&edStream));
							// This only works with a COM API
							edUnknown = static_cast<IUnknown *> (essenceData->GetContainer());
							edStream->Init(edUnknown);
							edStream->Release();
							edStream = NULL;
							essenceData->ReleaseReference();
							essenceData = NULL;
						}
						dataHead->ReleaseReference();
						dataHead = NULL;
					}
					else
					{
						_dataFile->ReleaseReference();
						_dataFile = NULL;
					}
				}
				else
				{
					CHECK(plugins->GetPluginInstance(_fileFormat, &plug));
					CHECK(plug->QueryInterface(IID_IAAFEssenceContainer, (void **)&container));
					
					if(openMode == kMediaOpenReadOnly)
					{
						status = container->OpenEssenceStreamReadOnly (nameBuf, &fileMobID, &_stream);
					}
					else if(openMode == kMediaOpenAppend)
					{
						status = container->OpenEssenceStreamAppend (nameBuf, &fileMobID, &_stream);
					}
					else
						RAISE(AAFRESULT_MEDIA_OPENMODE);
					
					if(status == AAFRESULT_SUCCESS)
						found = AAFTrue;
					container->Release();
					container = NULL;
					plug->Release();
					plug = NULL;
				}

				delete nameBuf;
				nameBuf = NULL;
				pLoc->ReleaseReference();
				pLoc = NULL;
			}
			enumLocate->ReleaseReference();
			enumLocate = NULL;
			if(!found)
				RAISE(AAFRESULT_MEDIA_NOT_FOUND);
		}


		iUnk = static_cast<IUnknown *> (fileMob->GetContainer());	// Codec knowns about compFilemob only
		CHECK(iUnk->QueryInterface(IID_IAAFSourceMob, (void **)&iFileMob));
		iUnk->Release();
		iUnk= NULL;
		CHECK(_codec->GetNumChannels(iFileMob, mediaKind, _stream, &numCh));
		if (numCh == 0)
		  RAISE(AAFRESULT_INVALID_DATADEF);
		
		_channels = (aafSubChannel_t *) new aafSubChannel_t[numCh];
		if(_channels == NULL)
			RAISE(AAFRESULT_NOMEMORY);
		_numChannels = numCh;
		for(n = 0; n < numCh; n++)
		{
			_channels[n].mediaKind = mediaKind;
			_channels[n].physicalOutChan = n+1;
			_channels[n].trackID = slotID+n;
		}

		iUnk = static_cast<IUnknown *> (fileMob->GetContainer());	// Codec knowns about compFilemob only
		CHECK(iUnk->QueryInterface(IID_IAAFSourceMob, (void **)&iFileMob));
		iUnk->Release();
		iUnk= NULL;
		CHECK(_codec->Open(iFileMob, slotID, openMode, _stream));
		
		_masterMob = masterMob;
		_masterMob->AcquireReference();
		fileMob->ReleaseReference();
		fileMob = NULL;
		compHead->ReleaseReference();
		compHead = NULL;
		if(plugins != NULL)
			plugins->ReleaseReference();
	}
	XEXCEPT
	{
		if(_dataFile != NULL)
			_dataFile->ReleaseReference();
		_dataFile = NULL;
		if(plugins != NULL)
			plugins->ReleaseReference();
		if(edStream != NULL)
			edStream->Release();
		if(iUnk != NULL)
			iUnk->Release();
		if(iAccess != NULL)
			iAccess->Release();
//		if(edUnknown != NULL)
//			edUnknown->Release();
		if(container != NULL)
			container->Release();
		if(plug != NULL)
			plug->Release();
		if(dict != NULL)
			dict->ReleaseReference();
		if(containerDef != NULL)
			containerDef->ReleaseReference();
		if(cStore != NULL)
			cStore->ReleaseReference();
		if(slot != NULL)
			slot->ReleaseReference();
		if(seg != NULL)
			seg->ReleaseReference();
		if(sourceInfo != NULL)
			sourceInfo->ReleaseReference();
		if(fileMob != NULL)
			fileMob->ReleaseReference();
		if(dataHead != NULL)
			dataHead->ReleaseReference();
		if(essenceData != NULL)
			essenceData->ReleaseReference();
		if(compHead != NULL)
			compHead->ReleaseReference();
		if(enumLocate != NULL)
			enumLocate->ReleaseReference();
		if(pLoc != NULL)
			pLoc->ReleaseReference();
	}
	XEND
	
	return (AAFRESULT_SUCCESS);
}

	//@comm This routine
	// follows the locator, and may call the locator failure callback if
	// the essence can not be found.  If the failure callback finds the essence,
	// then this routine will return normally.
	//@comm The essence handle from this call can be used with
	// WriteDataSamples or WriteMultiSamples but NOT with 
	//  WriteDataLines.
	//@comm Possible Errors:
	// 	Standard errors (see top of file).
	// 	AAFRESULT_NOMEMORY -- couldn't allocate memory for the essence handle
	//@comm Replaces omfmMediaMultiOpen*/

AAFRESULT ImplAAFEssenceAccess::SetEssenceDestination(
				ImplAAFLocator		*destination,
				aafUID_t		fileFormat)
{
	_destination = destination;
	if(destination != NULL)
		destination->AcquireReference();
	_fileFormat = fileFormat;
	return AAFRESULT_SUCCESS;
}

 //Sets which variety of the codec ID is to be used.)
AAFRESULT ImplAAFEssenceAccess::SetEssenceCodecFlavour(aafUID_t variety)
{
	_variety = variety;
	return AAFRESULT_SUCCESS;
}



	//@comm The essence handle from this call can be used with
	// WriteDataSamples or WriteMultiSamples but NOT with 
	// or WriteDataLines.
	//@comm If you are creating the essence, and then attaching it to a master
	// mob, then the "masterMob" field may be left NULL.
	//@comm Replaces omfmMediaMultiCreate
	
/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::WriteMultiSamples (aafInt16  arrayElemCount,
                           aafmMultiXfer_t *xferArray, aafmMultiResult_t *resultArray)
{

	aafAssert((_openType == kAAFCreated) ||
			  (_openType == kAAFAppended), _mainFile, AAFRESULT_MEDIA_OPENMODE);
	
	XPROTECT()
	{
		CHECK (_codec->WriteBlocks(deinterleave, arrayElemCount, xferArray, resultArray));
	}
	XEXCEPT
	XEND
	
	return(AAFRESULT_SUCCESS);
}

	//@comm arrayElemCount is the size of the array or transfer operations.
	// xferArray points to an array of transfer parameters.  All fields
	// in this array except for bytesXferred must be set up before
	// doing the transfer.
	//@comm Replaces omfmWriteMultiSamples

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::WriteSamples (aafUInt32  nSamples,
                           aafDataBuffer_t  buffer,
                           aafUInt32  buflen)
{
	aafmMultiXfer_t		xfer;
	aafmMultiResult_t	result;

	aafAssert(_numChannels == 1, _mainFile,
		AAFRESULT_SINGLE_CHANNEL_OP);
	aafAssert(buffer != NULL, _mainFile, AAFRESULT_BADDATAADDRESS);
	aafAssert((_openType == kAAFCreated) ||
				(_openType == kAAFAppended), _mainFile, AAFRESULT_MEDIA_OPENMODE);
	
	XPROTECT()
	{
		xfer.subTrackNum = _channels[0].physicalOutChan;
		xfer.numSamples = nSamples;
		xfer.buflen = buflen;
		xfer.buffer = buffer;
		result.bytesXfered = 0;
	
		CHECK (_codec->WriteBlocks(deinterleave, 1, &xfer, &result));
	}
	XEXCEPT
	XEND
	
	return(AAFRESULT_SUCCESS);
}

	//@comm Takes a essence handle, so the essence must have been opened or created.
	// A single video frame is ONE sample.
	// Buflen must be large enough to hold nSamples * the maximum sample size.
	//@comm Possible Errors:
	// Standard errors (see top of file).
	// AAFRESULT_SINGLE_CHANNEL_OP -- Tried to write to an interleaved stream.
	// AAFRESULT_BADDATAADDRESS -- The buffer must not be a NULL pointer.
	//@comm Replaces omfmWriteDataSamples

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::WriteRawData (aafUInt32  nSamples,
                           aafDataBuffer_t  buffer,
                           aafUInt32  sampleSize)
{
	IAAFEssenceStream	*pSamples = NULL;

	aafAssert(buffer != NULL, _mainFile, AAFRESULT_BADDATAADDRESS);
	aafAssert((_openType == kAAFCreated) ||
				(_openType == kAAFAppended), _mainFile,AAFRESULT_MEDIA_OPENMODE);

	XPROTECT()
	{
//!!!		CHECK(_codec->QueryInterface(IID_IAAFEssenceStream, (void **)&pSamples));
		CHECK(_codec->WriteRawData (nSamples, buffer, sampleSize));
		if(pSamples != NULL)
			pSamples->Release();
	}
	XEXCEPT
	{
		if(pSamples != NULL)
			pSamples->Release();
	}
	XEND
	
	return(AAFRESULT_SUCCESS);
}

	//@comm A single video frame is ONE sample.
	//@comm Buflen must be large enough to hold
	// nSamples * the maximum sample size.
	//@comm Possible Errors:
	// Standard errors (see top of file).
	// AAFRESULT_BADDATAADDRESS -- The buffer must not be a NULL pointer.
	//@comm Replaces omfmWriteRawData
	
/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::ReadRawData (aafUInt32  nSamples,
                           aafUInt32  buflen,
                           aafDataBuffer_t  buffer,
                           aafUInt32 *samplesRead,
                           aafUInt32 *bytesRead)
{
	aafAssert(buffer != NULL, _mainFile, AAFRESULT_BADDATAADDRESS);
	XPROTECT()
	{
		CHECK(_codec->ReadRawData (nSamples, buflen, buffer, bytesRead, samplesRead));
	}
	XEXCEPT
	{
	}
	XEND
	
	return(AAFRESULT_SUCCESS);
}

	//@comm A single video frame is ONE sample.
	//@comm Buflen must be large enough to hold nSamples * the maximum sample size.
	//@comm Possible Errors:
	// Standard errors (see top of file).
	// AAFRESULT_BADDATAADDRESS -- The buffer must not be a NULL pointer.
	//@comm Replaces omfmReadRawData
	
/****/
 AAFRESULT STDMETHODCALLTYPE
   ImplAAFEssenceAccess::WriteFractionalSample (
                           aafUInt32 nBytes,
                           aafDataBuffer_t  buffer,
                           aafUInt32 *bytesWritten)
{
	aafAssert((_openType == kAAFCreated) ||
				(_openType == kAAFAppended), _mainFile, AAFRESULT_MEDIA_OPENMODE);
	
	//!!!Pass bytesWritten doen through the codec interface
	return(_codec->WriteFractionalSample(buffer, nBytes));
 }

	//@comm Writes single lines of video to a file.  This function allows writing
	// video frames in pieces, for low-memory situations.  When enough lines
	// have been written to constitute a frame, then the number of samples will
	// be incremented by one.
	//@comm This function works only for video essence.
	//@comm The buffer must be large enough to hold an entire line of video. 
	//@comm Possible Errors:
	// Standard errors (see top of file).
	// AAFRESULT_BADRWLINES -- This function only works for video essence.
	//@comm Replaces omfmWriteDataLines
	//@devnote Previous version did not have nBytesPerLine, otherwise the actual size
	// of the buffer cannot be known.
	
/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::CompleteWrite ()
{
	IUnknown					*iUnk = NULL;
	IAAFSourceMob				*iCompFM = NULL;


	XPROTECT()
	{
		/* Close the _codec-> before creating more objects, in order to keep trailer data
		 * with the media in the file.
		 */
		if(_codec != NULL)		/* A codec was opened */
		{
			if(_dataFileMob != _compFileMob && _dataFileMob != NULL)	// codec will flush out _compFileMob, must be told to do the other
			{
				iUnk = static_cast<IUnknown *> (_dataFileMob->GetContainer());
				CHECK(iUnk->QueryInterface(IID_IAAFSourceMob, (void **)&iCompFM));
				iUnk->Release();
				iUnk = NULL;
				CHECK(_codec->CompleteWrite(iCompFM));
			}
			else
			{
				CHECK(_codec->CompleteWrite(NULL));
			}
		}

		if((_openType == kAAFCreated) || (_openType == kAAFAppended))
		{
			// Make the lengths of the affected tracks equal the sample length
			// adjusted for the edit rate
			CHECK(_compFileMob->ReconcileMobLength());
			CHECK(_masterMob->ReconcileMobLength());
			/**/
			if((_compFileMob != _dataFileMob) && (_dataFileMob != NULL))
			{
				CHECK(_dataFileMob->ReconcileMobLength());
			}
			//!!!CHECK(_dataMasterMob->ReconcileMobLength());
		}

		/* If the data is kept in an external file, then close that file.
		 * If the data is in the current file, leave it open.
		 */
		if ((_destination != NULL) && (_dataFile != NULL))
		{
			CHECK(_dataFile->Save());
		}

	}
	XEXCEPT
	{
		if(iUnk != NULL)
			iUnk->Release();
	}
	XEND
	
	return(AAFRESULT_SUCCESS);
}

	//@comm This function should be called whether the essence was opened or created.
	//@comm Replaces omfmMediaClose

	
/****/

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::GetNumChannels (ImplAAFMasterMob *masterMob,
                           aafSlotID_t  slotID,
                           aafMediaCriteria_t*  mediaCrit,
                           aafUID_t mediaKind,
                           aafInt16* numCh)
{
	aafPosition_t			zeroPos;
	ImplAAFFindSourceInfo	*sourceInfo;
	ImplAAFSourceMob		*fileMob;
	IAAFEssenceStream		*stream;
	ImplAAFDictionary		*dict = NULL;
	IUnknown				*iUnk = NULL;
	IAAFSourceMob			*iFileMob = NULL;
	ImplAAFPluginManager *plugins = NULL;

	aafAssert(numCh != NULL, _mainFile, AAFRESULT_NULL_PARAM);
	XPROTECT()
	{
		plugins = ImplAAFContext::GetInstance()->GetPluginManager();
		CvtInt32toPosition(0, zeroPos);	
		CHECK(masterMob->SearchSource(slotID, zeroPos,kFileMob,
									   mediaCrit,
									   NULL,
									   &sourceInfo));
		CHECK(sourceInfo->GetMob((ImplAAFMob **)&fileMob));
		CHECK(plugins->CreateInstance(CLSID_AAFEssenceDataStream,
				NULL, 
				IID_IAAFEssenceStream, 
				(void **)&stream));
		iUnk = static_cast<IUnknown *> (fileMob->GetContainer());	// Codec knowns about compFilemob only
		CHECK(iUnk->QueryInterface(IID_IAAFSourceMob, (void **)&iFileMob));
		iUnk->Release();
		iUnk= NULL;
		CHECK(_codec->GetNumChannels(iFileMob, mediaKind, stream, numCh));
		dict->ReleaseReference();
		dict = NULL;
		stream->Release();
		stream = NULL;
		plugins->ReleaseReference();
		plugins = NULL;
		iFileMob->Release();
		iFileMob = NULL;
	}
	XEXCEPT
	{
		if(dict)
			dict->ReleaseReference();
		if(stream)
			stream->Release();
		if (plugins)
			plugins->ReleaseReference();

	}
	XEND
	
	return(AAFRESULT_SUCCESS);
}

// INTERNAL to the SDK
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::GetSelectInfo (ImplAAFSourceMob *fileMob,
                           aafSelectInfo_t* pSelectInfo)
{
	IAAFEssenceStream		*stream;
	ImplAAFDictionary		*dict = NULL;
	IUnknown				*iUnk = NULL;
	IAAFSourceMob			*iFileMob = NULL;
	ImplAAFPluginManager *plugins = NULL;

	aafAssert(pSelectInfo != NULL, _mainFile, AAFRESULT_NULL_PARAM);
	XPROTECT()
	{
		plugins = ImplAAFContext::GetInstance()->GetPluginManager();
		CHECK(plugins->CreateInstance(CLSID_AAFEssenceDataStream,
				NULL, 
				IID_IAAFEssenceStream, 
				(void **)&stream));
		iUnk = static_cast<IUnknown *> (fileMob->GetContainer());	// Codec knowns about compFilemob only
		CHECK(iUnk->QueryInterface(IID_IAAFSourceMob, (void **)&iFileMob));
		iUnk->Release();
		iUnk= NULL;
		CHECK(_codec->GetSelectInfo(iFileMob, stream, pSelectInfo));
		dict->ReleaseReference();
		dict = NULL;
		stream->Release();
		stream = NULL;
		plugins->ReleaseReference();
		plugins = NULL;
		iFileMob->Release();
		iFileMob = NULL;
	}
	XEXCEPT
	{
		if(dict)
			dict->ReleaseReference();
		if(stream)
			stream->Release();
		if (plugins)
			plugins->ReleaseReference();

	}
	XEND
	
	return(AAFRESULT_SUCCESS);
}

	//@comm Returns the number of interleaved essence channels of a given type in the essence stream referenced by the given file mob
	//@comm If the data format is not interleaved, then the answer will
	// always be zero or one.  This function correctly returns zero
	// for essence types not handled by a given codec, and handles codecs
	// which work with multiple essence types.
	//@comm Replaces omfmGetNumChannels*/

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::GetLargestSampleSize (aafUID_t mediaKind,
                           aafLength_t *maxSize)
{
	aafAssert(maxSize != NULL, main, AAFRESULT_NULL_PARAM);
	XPROTECT()
	{
		CHECK(_codec->GetLargestSampleSize(mediaKind, maxSize));
	}
	XEXCEPT
	XEND
	
	return(AAFRESULT_SUCCESS);
}

	//@comm For uncompressed data, or the output of the software codec,
	// the sample size will propably be a constant.
	//@comm The essence type parameter exists to support codecs with multiple
	// interleaved essence types.
	//@comm Replaces omfmGetLargestSampleSize*/

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::GetSampleFrameSize (aafUID_t mediaKind,
                           aafPosition_t frameNum,
                           aafLength_t* frameSize)
{
	aafAssert(frameSize != NULL, main, AAFRESULT_NULL_PARAM);
	XPROTECT()
	{
		CHECK(_codec->GetIndexedSampleSize(mediaKind, frameNum, frameSize));
	}
	XEXCEPT
	XEND
	
	return(AAFRESULT_SUCCESS);
}

	//@comm For uncompressed data, or the output of the software codec,
	// the sample size will propably be a constant.
	//@comm The essence type parameter exists to support codecs with multiple
	// interleaved essence types.
	//@comm Possible Errors:
	// 	Standard errors (see top of file).
	// 	AAFRESULT_NULL_PARAM -- A return parameter was NULL.
	//@comm Replaces omfmGetSampleFrameSize*/



/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::SetTransformParameters (ImplAAFEssenceFormat * /*op*/)
{
	return AAFRESULT_NOT_IN_CURRENT_VERSION;
}



	//@comm Takes a essence handle, so the essence must have been opened or created.
	//@comm Possible Errors:<nl>
	//   AAFRESULT_NULL_PARAM -- A NULL rectangle pointer.<nl>
	//   AAFRESULT_INVALID_OP_CODEC -- This codec doesn't support display rect<nl>
	//   							(may not be video essence)
 	//@comm Replaces omfmGetSampledRect

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::GetSampleCount (
         aafUID_t mediaKind,
        aafLength_t *result)
{
//!!!	aafInt64		one;
	
	aafAssert(result != NULL, _mainFile, AAFRESULT_NULL_PARAM);

//!!!	CvtInt32toInt64(1, &one);
	
//!!!   if(Int64Greater(_pvt->repeatCount, one))
//	{
//	  *result = _pvt->repeatCount;
//	  return(AAFRESULT_SUCCESS);
//	}

	return(_codec->GetNumSamples(mediaKind, result));
}

	//@comm A video sample is one frame.
	//@comm Replaces omfmGetSampleCount

/****/
 AAFRESULT STDMETHODCALLTYPE
   ImplAAFEssenceAccess::ReadSamples (aafUInt32  nSamples,
                           aafUInt32  buflen,
                           aafDataBuffer_t  buffer,
                           aafUInt32*  samplesRead,
                           aafUInt32*  bytesRead)
{
	aafmMultiXfer_t		xfer;
	aafmMultiResult_t	result;
//!!!	aafInt64		one;
	
	aafAssert(buffer != NULL, _mainFile, AAFRESULT_NULL_PARAM);
	aafAssert(bytesRead != NULL, _mainFile, AAFRESULT_NULL_PARAM);

//!!!	CvtInt32toInt64(1, &one);
//!!!	if(Int64Greater(_pvt->repeatCount, one))
//!!!	  SeektoEditFrame(0);

	XPROTECT()
	{
//!!!		xfer.subTrackNum = _physicalOutChanOpen;
		xfer.numSamples = nSamples;
		xfer.buflen = buflen;
		xfer.buffer = buffer;
		result.bytesXfered = 0;
	
		CHECK(_codec->ReadBlocks(deinterleave, 1, &xfer, &result));
	}
	XEXCEPT
	{
		*bytesRead = result.bytesXfered;
	}
	XEND
	
	*bytesRead = result.bytesXfered;

	return (AAFRESULT_SUCCESS);
}

	//@comm This call will only return a single channel of essence from an interleaved
	// stream.
	//@comm A video sample is a frame.
	// Buflen is in bytes, and should be large enough to hold the samples
	// in the requested memory format.
	//@comm Replaces omfmReadDataSamples

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::ReadMultiSamples (aafInt16  elemCount,
                           aafmMultiXfer_t *xferArray, aafmMultiResult_t *resultArray)
{
	aafAssert(xferArray != NULL, _mainFile, AAFRESULT_NULL_PARAM);

	return (_codec->ReadBlocks(deinterleave, elemCount, xferArray, resultArray));
}

	//@comm arrayElemCount is the size of the array or transfer operations.
	// xferArray points to an array of transfer parameters.  All fields
	// in this array except for bytesXferred must be set up before
	// doing the transfer.
	//@comm Replaces omfmReadMultiSamples
	
/****/
 AAFRESULT STDMETHODCALLTYPE
   ImplAAFEssenceAccess::ReadFractionalSample (
						   aafUInt32 bufLen,
                           aafDataBuffer_t  buffer,
                           aafUInt32*  bytesRead)
{
	aafAssert(buffer != NULL, _mainFile, AAFRESULT_NULL_PARAM);

	return (_codec->ReadFractionalSample(bufLen, buffer, bytesRead));
}

	//@comm This function allows reading
	// video frames in pieces, for low-memory situations.  When enough lines
	// have been read to constitute a frame, then the number of samples read
	// be incremented by one.
	//@comm The buffer must be large enough to hold an entire line of video.
	//@comm This function works only for video essence.
	//@comm Replaces omfmReadDataLines

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::Seek (aafInt64  frameNum)
{
	aafInt64		one;
	AAFRESULT		status;
	
	CvtInt32toInt64(1, &one);
//!!!	if(Int64Greater(_pvt->repeatCount, one))
//		status = _codec->Seek(one);
//	else
		status = _codec->Seek(frameNum);
	return (status);
}

	//@comm Useful only on reading, you
	// can't seek aound while writing essence.
	//@comm An audio frame is one sample across all open channels.
	//@comm Replaces omfmGotoFrameNumber


			
			
/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::GetFileFormat(ImplAAFEssenceFormat * opsTemplate,
         ImplAAFEssenceFormat ** opsResult)
{
	IAAFEssenceFormat	*iFormat = NULL, *iResultFormat = NULL;
	IUnknown			*iUnknown = NULL;
    IAAFRoot *			iObj = NULL;
    ImplAAFRoot *		arg;

	XPROTECT()
	{
		iUnknown = static_cast<IUnknown *>(opsTemplate->GetContainer());
		CHECK(iUnknown->QueryInterface(IID_IAAFEssenceFormat, (void **)&iFormat));
		CHECK(_codec->GetEssenceFormat(iFormat, &iResultFormat));	// !!!COM Dependency
		iFormat->Release();
		iFormat = NULL;
		CHECK(iResultFormat->QueryInterface (IID_IAAFRoot, (void **)&iObj));
		iResultFormat->Release();
		iResultFormat = NULL;
		assert (iObj);
		CHECK(iObj->GetImplRep((void **)&arg));
		*opsResult = static_cast<ImplAAFEssenceFormat*>(arg);
	}
	XEXCEPT
	{
		if(iFormat != NULL)
			iFormat->Release();
		if(iResultFormat != NULL)
			iResultFormat->Release();
//		if(iUnknown != NULL)
//			iUnknown->Release();
		if(iObj != NULL)
			iObj->Release();
	}
	XEND

	return AAFRESULT_SUCCESS;
}

	//@comm Replaces omfmGetVideoInfoArray */
/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::GetEmptyFileFormat (ImplAAFEssenceFormat **ops)
{
	ImplAAFEssenceFormat	*fmt;

	fmt = (ImplAAFEssenceFormat *)CreateImpl(CLSID_AAFEssenceFormat);
	*ops = fmt;
	return AAFRESULT_SUCCESS;
}

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::GetFileFormatParameterList (ImplAAFEssenceFormat **opsResult)
{
	IAAFEssenceFormat	*iResultFormat = NULL;
    IAAFRoot *			iObj = NULL;
    ImplAAFRoot *		arg;

	XPROTECT()
	{
		CHECK(_codec->GetDefaultEssenceFormat(&iResultFormat));	// !!!COM Dependency
		CHECK(iResultFormat->QueryInterface (IID_IAAFRoot, (void **)&iObj));
		iResultFormat->Release();
		iResultFormat = NULL;
		assert (iObj);
		CHECK(iObj->GetImplRep((void **)&arg));
		*opsResult = static_cast<ImplAAFEssenceFormat*>(arg);
	}
	XEXCEPT
	{
		if(iResultFormat != NULL)
			iResultFormat->Release();
		if(iObj != NULL)
			iObj->Release();
	}
	XEND

	return AAFRESULT_SUCCESS;
}

	//@comm Replaces omfmGetVideoInfoArray */

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::PutFileFormat (ImplAAFEssenceFormat *ops)
{
	IAAFEssenceFormat	*iFormat = NULL;
	IUnknown			*iUnknown;

	XPROTECT()
	{
		iUnknown = static_cast<IUnknown *>(ops->GetContainer());
		CHECK(iUnknown->QueryInterface(IID_IAAFEssenceFormat, (void **)&iFormat));
		CHECK(_codec->PutEssenceFormat(iFormat));	// COM Dependency
		if(iFormat != NULL)
		{
			iFormat->Release();
			iFormat  = NULL;
		}
	}
	XEXCEPT
	{
		if(iFormat != NULL)
			iFormat->Release();
	}
	XEND

	return AAFRESULT_SUCCESS;
}

	//@comm Replaces omfmPutVideoInfoArray */



/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::IsHardwareAssistedCodec (aafCodecID_t  /*codecID*/,	//!!!Get rid of parm fromIDL
                           ImplAAFSourceMob *mob,
                           aafBool*result)
{
	ImplAAFDictionary		*dict = NULL;	
	ImplAAFPluginDescriptor	*desc = NULL;
	aafUID_t				descID;

	if(result == NULL)
		return AAFRESULT_NULL_PARAM;
	
	*result = AAFFalse;
	XPROTECT()
	{
 		CHECK(_codec->GetEssenceDescriptorID (&descID))
		CHECK(mob->GetDictionary(&dict));
		CHECK(dict->LookupPluginDescriptor(&descID, &desc));
		CHECK(desc->IsAccelerated(result));
		desc->ReleaseReference();
		desc = NULL;
		dict->ReleaseReference();
		dict = NULL;
	}
	XEXCEPT
	{
		if(desc != NULL)
			desc->ReleaseReference();
		if(dict != NULL)
			dict->ReleaseReference();
	}
	XEND
	
	return (AAFRESULT_SUCCESS);
}

	//@comm That is, does
	// it contain calls to a particular hardware device which speeds
	// up transfer and compression/decompression.
	//@comm If the hardware is not present in the system, this call should
	// return FALSE.
	//@comm If the parameters in the essence descriptor are out-of-bounds
	// for the hardware, then this call will return FALSE.
	//@comm Replaces omfmIsHardwareAssistedCodec */

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::GetCodecName (aafUInt32 namelen,
                           wchar_t *  name)
{
	IAAFDefObject	*def = NULL;
	
	XPROTECT()
	{
		//!!!Assert that _pluginDescriptor is NON-NULL
		CHECK(_codecDescriptor->QueryInterface(IID_IAAFDefObject, (void **)&def));
		CHECK(def->GetName (name, namelen));
		def->Release();
		def = NULL;
	}
	XEXCEPT
	{
		if(def != NULL)
			def->Release();
	}
	XEND
	
	return (AAFRESULT_SUCCESS);
}

	//@comm No other call uses this name, so it may be fully descriptive, esp. of limitations.
	//@comm The name will be truncated to fit within "buflen" bytes.
	//@comm Possible Errors:<nl>
	// 	Standard errors (see top of file).<nl>
	//	AAFRESULT_CODEC_INVALID - The given codec ID is not loaded.
	//@comm Replaces omfmCodecGetName */
	//@devnote Shouldn't we use a unicode aafString_t since AAF's public interface for
	// strings is supposed to be unicode?(TomR)

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::GetCodecID (aafCodecID_t *codecID)
{
	IAAFPlugin	*plug = NULL;

	XPROTECT()
	{
		CHECK(_codec->QueryInterface(IID_IAAFPlugin, (void **)&plug));
//!!!		CHECK(plug->GetPluggableID(codecID));
		plug->Release();
		plug = NULL;
	}
	XEXCEPT
	{
		if(plug != NULL)
			plug->Release();
	}
	XEND
	
	return (AAFRESULT_SUCCESS);
}

	//@comm This will be required in order to send private data to the codec.
	//@comm The name will be truncated to fit within "buflen" bytes.
	//@comm Replaces omfmMediaGetCodecID */

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::AddSampleIndexEntry (aafInt64 frameOffset)
{
	return(_codec->AddSampleIndexEntry(frameOffset));
}

	//@comm This function should NOT be called when essence is passed to
	//the reference implementation in an uncompressed format.
	//@comm Possible Errors:<nl>
	//	Standard errors (see top of file).<nl>
	//	AAFRESULT_INVALID_OP_CODEC -- This kind of essence doesn't have a frame index<nl>
	//	AAFRESULT_MEDIA_OPENMODE -- The essence is open for read-only.
	//@comm Replaces omfmAddFrameIndexEntry */

/****/

AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::GetStoredByteOrder (eAAFByteOrder_t *  /*pOrder*/)
{
	return AAFRESULT_NOT_IMPLEMENTED;		//!!!
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::GetNativeByteOrder (eAAFByteOrder_t *  /*pOrder*/)
{
	return AAFRESULT_NOT_IMPLEMENTED;		//!!!
}

AAFRESULT ImplAAFEssenceAccess::MakeAAFContainerDef(ImplAAFHeader *head, ImplAAFContainerDef **result)
{
	ImplAAFContainerDef	*obj = NULL;
	aafUID_t			uid;
	ImplAAFDictionary	*dict = NULL;

	if(result == NULL)
		return(AAFRESULT_NULL_PARAM);
	XPROTECT()
	{
		CHECK(head->GetDictionary (&dict));
		CHECK(dict->CreateInstance(&AUID_AAFContainerDef, (ImplAAFObject **)&obj));
		if(obj == NULL)
			RAISE(AAFRESULT_NOMEMORY);
		uid = ContainerAAF;
		CHECK(obj->SetAUID(&uid));
		CHECK(obj->SetName(L"AAF Container"));
		CHECK(obj->SetDescription(L"Essence is in an AAF file."));
		CHECK(obj->SetEssenceIsIdentified(AAFTrue));
		*result = obj;
		// Don't bother acquire, as we would immediately release
		if(dict != NULL)
			dict->ReleaseReference();
	}
	XEXCEPT
	{
		if(obj != NULL)
			obj->ReleaseReference();
		if(dict != NULL)
			dict->ReleaseReference();
	}
	XEND
	
	return AAFRESULT_SUCCESS;
}

AAFRESULT
ImplAAFEssenceAccess::CreateContainerDef (ImplAAFHeader *head)
{
	IAAFPlugin					*plug = NULL;
	IAAFDictionary				*dictInterface = NULL;
	IUnknown					*pUnknown = NULL;
	IAAFDefObject				*pDef = NULL;
	IAAFContainerDef			*containerDef = NULL;
	ImplAAFContainerDef			*implContainerDef = NULL;
	ImplAAFDictionary			*dict = NULL;
	ImplAAFPluginManager		*plugins = NULL;

	XPROTECT()
	{
		plugins = ImplAAFContext::GetInstance()->GetPluginManager();
		CHECK(head->GetDictionary (&dict));
		if(dict->LookupContainerDefinition (&_fileFormat, &implContainerDef) != AAFRESULT_SUCCESS)
		{
			
			//!!!This should call into the pluginmanager instead of using if?
			if(EqualAUID(&_fileFormat, &ContainerAAF))
			{
				CHECK(MakeAAFContainerDef(head, &implContainerDef));
				CHECK(dict->RegisterContainerDefinition (implContainerDef));
			}
			else
			{
				pUnknown = static_cast<IUnknown *> (dict->GetContainer());
				CHECK(pUnknown->QueryInterface(IID_IAAFDictionary, (void **)&dictInterface));
				

				CHECK(plugins->GetPluginInstance(_fileFormat, &plug));
				CHECK(plug->GetIndexedDefinitionObject (/*!!!*/0, dictInterface, &pDef));
				CHECK(pDef->QueryInterface(IID_IAAFContainerDef, (void **)&containerDef));
				plug->Release();
				plug = NULL;
				CHECK(dictInterface->RegisterContainerDefinition (containerDef));
				containerDef->Release();
				containerDef = NULL;
				dictInterface->Release();
				dictInterface = NULL;
				pDef->Release();
				pDef = NULL;
			}
		}
		if(containerDef != NULL)
			containerDef->Release();
		if(dict != NULL)
			dict->ReleaseReference();
		if(plugins != NULL)
			plugins->ReleaseReference();
		if(implContainerDef != NULL)
			implContainerDef->ReleaseReference();
		if(pDef != NULL)
			pDef->Release();
	}
	XEXCEPT
	{
		if(containerDef != NULL)
			containerDef->Release();
		if(dict != NULL)
			dict->ReleaseReference();
		if(plugins != NULL)
			plugins->ReleaseReference();
		if(plug != NULL)
			plug->Release();
		if(dictInterface != NULL)
			dictInterface->Release();
		if(implContainerDef != NULL)
			implContainerDef->ReleaseReference();
	}
	XEND
		
	return(AAFRESULT_SUCCESS);
}

AAFRESULT
ImplAAFEssenceAccess::CreateCodecDef (ImplAAFHeader *head, aafUID_t codecID, IAAFPluginDescriptor **ppPluginDesc)
{
	IAAFPlugin					*plug = NULL;
	ImplAAFCodecDef				*codecImpl = NULL;
	IAAFDefObject				*def = NULL;
	IAAFCodecDef				*codecDef = NULL;
	IAAFDictionary				*dictInterface = NULL;
	IUnknown					*pUnknown = NULL;
	IAAFPluginDescriptor		*pluginDesc = NULL;
	IEnumAAFPluginDescriptors	*descEnum = NULL;
	IAAFDefObject				*defInterface = NULL;
	ImplAAFDictionary			*dict = NULL;
	ImplAAFPluginManager		*plugins = NULL;
	aafBool						found = AAFFalse;
	aafUID_t					currentPlugDesc, testAUID;

	XPROTECT()
	{
		plugins = ImplAAFContext::GetInstance()->GetPluginManager();
		CHECK(head->GetDictionary (&dict));	//!!!Only makes essence in the current file?
		if(dict->LookupCodecDefinition (&codecID, &codecImpl) != AAFRESULT_SUCCESS)
		{
			pUnknown = static_cast<IUnknown *> (dict->GetContainer());
			CHECK(pUnknown->QueryInterface(IID_IAAFDictionary, (void **)&dictInterface));
			CHECK(_codec->QueryInterface(IID_IAAFPlugin, (void **)&plug));
			CHECK(plug->GetIndexedDefinitionObject (/*!!!*/0,dictInterface, &def));
			CHECK(def->QueryInterface(IID_IAAFCodecDef, (void **)&codecDef));
			CHECK(dictInterface->RegisterCodecDefinition (codecDef));
			CHECK(dict->LookupCodecDefinition (&codecID, &codecImpl));
			
			plug->Release();
			plug = NULL;
			dictInterface->Release();
			dictInterface = NULL;
			def->Release();
			def = NULL;
		}
		
		pUnknown = static_cast<IUnknown *> (dict->GetContainer());
		CHECK(pUnknown->QueryInterface(IID_IAAFDictionary, (void **)&dictInterface));
		CHECK(_codec->QueryInterface(IID_IAAFPlugin, (void **)&plug));
		CHECK(plug->GetPluginDescriptorID(&currentPlugDesc));
		pUnknown = static_cast<IUnknown *> (codecImpl->GetContainer());
		CHECK(pUnknown->QueryInterface(IID_IAAFDefObject, (void **)&defInterface));
		
		CHECK(defInterface->EnumPluginDescriptors (&descEnum));
		defInterface->Release();
		defInterface = NULL;
		
		while(descEnum->NextOne(&pluginDesc) == AAFRESULT_SUCCESS)
		{
			CHECK(pluginDesc->QueryInterface(IID_IAAFDefObject, (void **)&defInterface));
			CHECK(defInterface->GetAUID(&testAUID));
			if(EqualAUID(&testAUID, &currentPlugDesc))
			{
				found = AAFTrue;
				if(ppPluginDesc != NULL)
				{
					pluginDesc->AddRef();	// About to store this 
					*ppPluginDesc = pluginDesc;
				}
			}
			defInterface->Release();
			defInterface = NULL;
			pluginDesc->Release();
			pluginDesc = NULL;
		}
		descEnum->Release();
		descEnum = NULL;
		
		if(found == AAFFalse)	// If pluginDescriptor is not in place
		{
			
			CHECK(plug->CreateDescriptor (dictInterface, &pluginDesc));
			CHECK(codecDef->QueryInterface(IID_IAAFDefObject, (void **)&def));
			CHECK(def->AppendPluginDescriptor (pluginDesc));
			if(ppPluginDesc != NULL)
			{
				pluginDesc->AddRef();	// About to store this 
				*ppPluginDesc = pluginDesc;
			}
			pluginDesc->Release();
			def->Release();
		}
		
		codecDef->Release();
		codecDef = NULL;
		dictInterface->Release();
		dictInterface = NULL;
		plug->Release();
		plug = NULL;
		if(codecImpl != NULL)
			codecImpl->ReleaseReference();
		if(dict != NULL)
			dict->ReleaseReference();
		if(plugins != NULL)
			plugins->ReleaseReference();
	}
	XEXCEPT
	{
		if(codecImpl != NULL)
			codecImpl->ReleaseReference();
		if(dict != NULL)
			dict->ReleaseReference();
		if(plugins != NULL)
			plugins->ReleaseReference();
		if(plug != NULL)
			plug->Release();
		if(codecDef != NULL)
			codecDef->Release();
		if(dictInterface != NULL)
			dictInterface->Release();
//		if(pUnknown != NULL)
//			pUnknown->Release();
		if(def != NULL)
			def->Release();
		if(pluginDesc != NULL)
			pluginDesc->Release();
		if(descEnum != NULL)
			descEnum->Release();
		if(defInterface != NULL)
			defInterface->Release();
	}
	XEND
		
		return(AAFRESULT_SUCCESS);
}

AAFRESULT
ImplAAFEssenceAccess::CreateEssenceFileFromLocator (ImplAAFHeader *srcHead, ImplAAFLocator *loc, ImplAAFFile **result)
{
	ImplAAFIdentification			*xferIdent = NULL;
	aafProductIdentification_t		identSetup;
	aafUInt32						length;
	aafInt32						buflen;
	wchar_t							*nameBuf = NULL;
	ImplAAFFile						*theFile = NULL;
	aafUID_t						myFileCLSID;

	identSetup.companyName = NULL;
	identSetup.productName = NULL;
	identSetup.productVersionString = NULL;
	identSetup.platform = NULL;

	XPROTECT()
	{
		loc->GetPathBufLen(&buflen);
		nameBuf = new wchar_t[buflen];
		if(nameBuf == NULL)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(loc->GetPath(nameBuf, buflen));
		CHECK(srcHead->GetLastIdentification (&xferIdent));
		CHECK(xferIdent->GetCompanyNameBufLen(&length));
		identSetup.companyName = new wchar_t[length];
		CHECK(xferIdent->GetCompanyName(identSetup.companyName, length));

		CHECK(xferIdent->GetProductNameBufLen(&length));
		identSetup.productName = new wchar_t[length];
		CHECK(xferIdent->GetProductName(identSetup.productName, length));

		CHECK(xferIdent->GetProductVersionStringBufLen(&length));
		identSetup.productVersionString = new wchar_t[length];
		CHECK(xferIdent->GetProductVersionString(identSetup.productVersionString, length));

		//!!!    xferIdent->GetProductVersion(&identSetup.productVersion);
		//!!!    xferIdent->GetProductID(&identSetup.productID);

		CHECK(xferIdent->GetPlatformBufLen(&length));
		identSetup.platform = new wchar_t[length];
		CHECK(xferIdent->GetPlatform(identSetup.platform, length));

		memcpy((void *)&myFileCLSID, (void *)&CLSID_AAFFile, sizeof(aafUID_t));
		theFile = (ImplAAFFile *)CreateImpl(myFileCLSID);
		CHECK(theFile->Initialize());
	    CHECK(theFile->OpenNewModify(nameBuf, 0, &identSetup));
		*result = theFile;
		AcquireImplReference(theFile);

		delete [] nameBuf;
		nameBuf = NULL;			

		delete [] identSetup.companyName;
		identSetup.companyName = NULL;
		delete [] identSetup.productName;
		identSetup.productName = NULL;
		delete [] identSetup.productVersionString;
		identSetup.productVersionString = NULL;
		delete [] identSetup.platform;
		identSetup.platform = NULL;
		if(xferIdent != NULL)
			xferIdent->ReleaseReference();
		if(theFile != NULL)
			theFile->ReleaseReference();
	}
	XEXCEPT
	{
		if(theFile != NULL)
			theFile->ReleaseReference();
		if(xferIdent != NULL)
			xferIdent->ReleaseReference();
		if(identSetup.companyName != NULL)
			delete [] identSetup.companyName;
		if(identSetup.productName != NULL)
			delete [] identSetup.productName;
		if(identSetup.productVersionString != NULL)
			delete [] identSetup.productVersionString;
		if(identSetup.platform != NULL)
			delete [] identSetup.platform;
	}
	XEND;

	return AAFRESULT_SUCCESS;
}

AAFRESULT
ImplAAFEssenceAccess::ModifyEssenceFileFromLocator (ImplAAFHeader *srcHead, ImplAAFLocator *loc, ImplAAFFile **result)
{
	ImplAAFIdentification			*xferIdent;
	aafProductIdentification_t		identSetup;
	aafUInt32						length;
	aafInt32						buflen;
	wchar_t							*nameBuf = NULL;
	ImplAAFFile						*theFile;
	aafUID_t						myFileCLSID;

	identSetup.companyName = NULL;
	identSetup.productName = NULL;
	identSetup.productVersionString = NULL;
	identSetup.platform = NULL;

	XPROTECT()
	{
		loc->GetPathBufLen(&buflen);
		nameBuf = new wchar_t[buflen];
		if(nameBuf == NULL)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(loc->GetPath(nameBuf, buflen));
		CHECK(srcHead->GetLastIdentification (&xferIdent));
		CHECK(xferIdent->GetCompanyNameBufLen(&length));
		identSetup.companyName = new wchar_t[length];
		CHECK(xferIdent->GetCompanyName(identSetup.companyName, length));

		CHECK(xferIdent->GetProductNameBufLen(&length));
		identSetup.productName = new wchar_t[length];
		CHECK(xferIdent->GetProductName(identSetup.productName, length));

		CHECK(xferIdent->GetProductVersionStringBufLen(&length));
		identSetup.productVersionString = new wchar_t[length];
		CHECK(xferIdent->GetProductVersionString(identSetup.productVersionString, length));

		//!!!    xferIdent->GetProductVersion(&identSetup.productVersion);
		//!!!    xferIdent->GetProductID(&identSetup.productID);

		CHECK(xferIdent->GetPlatformBufLen(&length));
		identSetup.platform = new wchar_t[length];
		CHECK(xferIdent->GetPlatform(identSetup.platform, length));

		memcpy((void *)&myFileCLSID, (void *)&CLSID_AAFFile, sizeof(aafUID_t));
		theFile = (ImplAAFFile *)CreateImpl(myFileCLSID);
		CHECK(theFile->Initialize());
	    CHECK(theFile->OpenExistingModify(nameBuf, 0, &identSetup));
		*result = theFile;
		AcquireImplReference(theFile);

		delete [] nameBuf;
		nameBuf = NULL;			

		delete [] identSetup.companyName;
		identSetup.companyName = NULL;
		delete [] identSetup.productName;
		identSetup.productName = NULL;
		delete [] identSetup.productVersionString;
		identSetup.productVersionString = NULL;
		delete [] identSetup.platform;
		identSetup.platform = NULL;
		if(xferIdent != NULL)
			xferIdent->ReleaseReference();
		if(theFile != NULL)
			theFile->ReleaseReference();
	}
	XEXCEPT
	{
		if(theFile != NULL)
			theFile->ReleaseReference();
		if(xferIdent != NULL)
			xferIdent->ReleaseReference();
		if(identSetup.companyName != NULL)
			delete [] identSetup.companyName;
		if(identSetup.productName != NULL)
			delete [] identSetup.productName;
		if(identSetup.productVersionString != NULL)
			delete [] identSetup.productVersionString;
		if(identSetup.platform != NULL)
			delete [] identSetup.platform;
	}
	XEND;

	return AAFRESULT_SUCCESS;
}

AAFRESULT
ImplAAFEssenceAccess::CreateFileMob (ImplAAFHeader *newHead,
								aafBool				addSlots,
								aafSlotID_t			slotID,
								aafUID_t			*newMobID,
								aafUID_t			mediaKind,
								aafRational_t		editRate,
								aafRational_t		sampleRate,
								ImplAAFLocator		*addLocator,
								ImplAAFSourceMob	**result)
{
	ImplAAFDictionary	*dict = NULL;
	ImplAAFSourceMob	*fileMob = NULL;
	ImplAAFMobSlot		*tmpSlot = NULL;
	aafUID_t			essenceDescriptorID;
	ImplAAFFileDescriptor *mdes = NULL;

	XPROTECT()
	{
		CHECK(newHead->GetDictionary (&dict));
		/* Initialize the basic fields of the media handle
		 */
		CHECK(dict->CreateInstance(&AUID_AAFSourceMob, (ImplAAFObject **)&fileMob));
		if(newMobID != NULL)
		{
			fileMob->SetMobID(newMobID);
		}
		
		*result = fileMob;
		AcquireImplReference(fileMob);
		if(addSlots)
		{
			/* JeffB: Handle the case where an existing file=>tape mob connection exists
			 */
			if(fileMob->FindSlotBySlotID(slotID, &tmpSlot) == AAFRESULT_SLOT_NOT_FOUND)
			{
				CHECK(fileMob->AddNilReference(slotID, 
					0, &mediaKind, editRate));
			}
			CHECK(fileMob->FindSlotBySlotID(slotID, &tmpSlot));
			CHECK(tmpSlot->SetPhysicalNum(slotID));
		}
//		CHECK(newHead->GetDictionary (&dict));
		CHECK(_codec->GetEssenceDescriptorID(&essenceDescriptorID));
		CHECK(dict->CreateInstance(&essenceDescriptorID, (ImplAAFObject **)&mdes));
		CHECK(mdes->SetIsInContainer (_destination == NULL ? AAFTrue : AAFFalse));
		CHECK(mdes->SetContainerFormat (&_fileFormat));
		CHECK(mdes->SetSampleRate(&sampleRate));
		CHECK(fileMob->SetEssenceDescriptor(mdes));
		CHECK(newHead->AppendMob(fileMob));
		if(addLocator != NULL)
		{
			CHECK(mdes->AppendLocator(addLocator));
			CHECK(mdes->SetIsInContainer(AAFFalse));
			CHECK(mdes->SetContainerFormat (&_fileFormat));
		}
		if(dict != NULL)
			dict->ReleaseReference();
		if(tmpSlot != NULL)
			tmpSlot->ReleaseReference();
		if(mdes != NULL)
			mdes->ReleaseReference();
		if(fileMob != NULL)
			fileMob->ReleaseReference();
	}
	XEXCEPT
	{
		if(dict != NULL)
			dict->ReleaseReference();
		if(tmpSlot != NULL)
			tmpSlot->ReleaseReference();
		if(mdes != NULL)
			mdes->ReleaseReference();
		if(fileMob != NULL)
			fileMob->ReleaseReference();
	}
	XEND
		
	return AAFRESULT_SUCCESS;
}

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
#include "ImplEnumAAFPluginDefs.h"
#include "ImplAAFDictionary.h"
#include "ImplAAFDataDef.h"

#include "ImplAAFBuiltinDefs.h"
#include "ImplAAFSmartPointer.h"
typedef ImplAAFSmartPointer<ImplAAFDataDef> ImplAAFDataDefSP;

#include <assert.h>
#include <string.h>

#ifndef __AAFPrivate_h__
#include "AAFPrivate.h"
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
	_containerDefID = ContainerAAF;
	_codecID = kAAFNoCodec;
	_flavour = kAAFNilCodecFlavour;
	_destination = NULL;
	_compFileMob = NULL;
	_numChannels = 0;
	_channels = NULL;
	_masterMob = NULL;
	_mdes = NULL;
	_codec = NULL;
	_multicodec = NULL;
  _internalEssenceData = NULL;
	_stream = NULL;
  _openType = kAAFReadOnly;
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
		_dataFile = 0;
	}
	if(_destination != NULL)
	  {
		_destination->ReleaseReference();
		_destination = 0;
	  }
	if(_compFileMob != NULL)
	  {
		_compFileMob->ReleaseReference();
		_compFileMob = 0;
	  }
	if(_masterMob != NULL)
	  {
		_masterMob->ReleaseReference();
		_masterMob = 0;
	  }
	if(_channels != NULL)
		delete [] _channels;
	if(_mdes != NULL)
	  {
		_mdes->ReleaseReference();
		_mdes = 0;
	  }
	if(_dataFileMob != NULL)
	  {
		_dataFileMob->ReleaseReference();
		_dataFileMob = 0;
	  }
	if(_stream != NULL)
		_stream->Release();

  if (NULL != _internalEssenceData)
  {
    _internalEssenceData->Release();
    _internalEssenceData = NULL;
  }
	if(_codecDescriptor != NULL)
		_codecDescriptor->Release();
  if(NULL != _multicodec)
  {
    _multicodec->Release();
    _multicodec = NULL;
  }
	if(_codec != NULL)
	{
		// In create(), the refcount was artificially lowerered.  Set it back up here 
		// so that the codec can release it's reference on us without calling delete again.
		AcquireReference();
//		AcquireReference();
		_codec->Release();
    _codec = NULL;
	}
/*
	if(_dataFile != NULL)
		delete _dataFile;
*/
}	

/**ImplementationPrivate**/
AAFRESULT STDMETHODCALLTYPE
ImplAAFEssenceAccess::Create (ImplAAFMasterMob *    masterMob,
							  aafSlotID_t		    masterSlotID,
							  const aafUID_t &      mediaKind,
							  const aafUID_t &      codecID,
							  const aafRational_t & editRate,
							  const aafRational_t & sampleRate,
							  aafCompressEnable_t   compEnable)
{
	IUnknown				*dataObj = NULL;
	IAAFSourceMob			*iFileMob = NULL;
	IUnknown				*iUnk = NULL;
	IAAFEssenceDataStream	*edStream = NULL;
	IAAFPlugin				*plugin = NULL;
	IAAFEssenceContainer	*container = NULL;
	IAAFPlugin				*plug = NULL;
	aafMobID_t			fileMobUID;
	aafLength_t			oneLength = CvtInt32toLength(1, oneLength);
	ImplAAFDictionary	*dataDict = NULL;
	ImplAAFHeader		*compHead = NULL;
	ImplAAFHeader		*dataHead = NULL;
	ImplAAFPluginManager *plugins = NULL;
	ImplAAFEssenceData	*implData = NULL;
	wchar_t				*nameBuf = NULL;
	aafUInt32			buflen;
	aafUID_t			aafFormat = ContainerAAF;


	XPROTECT()
	{
		_openType = kAAFCreated;
		_dataFile = NULL;
		
		CHECK(masterMob->MyHeadObject(&compHead));
		
		if((_destination != NULL) && EqualAUID(&_containerDefID, &ContainerAAF))
		{
			CHECK(CreateEssenceFileFromLocator (compHead, _destination, &_dataFile));
			CHECK(_dataFile->GetHeader(&dataHead));
		}
		else
		{
			CHECK(masterMob->MyHeadObject(&dataHead));
		}
		
		CHECK(dataHead->GetDictionary(&dataDict));

		// Can't put raw media inside of an AAF File
		if(_destination == NULL && !EqualAUID(&_containerDefID, &aafFormat))
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
		CHECK(CreateCodecDef(compHead, codecID, &_codecDescriptor));

    // Initialize the multi-essence codec interface pointer (not required for this type of open).
    _codec->QueryInterface(IID_IAAFMultiEssenceCodec, (void **)&_multicodec);
 		
    // When we enable the cloneExternal (below) then Don't do this call for creating the
		// file mob twice
		CHECK(CreateFileMob(compHead, kAAFTrue, DEFAULT_FILE_SLOT, NULL, mediaKind,
			_codecID, editRate, sampleRate,
			_destination, &_compFileMob));
		CHECK(_compFileMob->GetMobID(&fileMobUID));
		if(compHead != dataHead)
		{
			CHECK(CreateContainerDef(dataHead))
			CHECK(CreateCodecDef(dataHead, codecID, NULL));
			CHECK(CreateFileMob(dataHead, kAAFTrue, DEFAULT_FILE_SLOT, &fileMobUID, mediaKind,
				_codecID, editRate, sampleRate,
				NULL, &_dataFileMob));
			_dataFileMob->AcquireReference();	//!!!Leaking here?
		}
		else
			_dataFileMob = NULL;
		
		ImplAAFDataDefSP pMediaKind;
		CHECK (dataDict->LookupDataDef (mediaKind, &pMediaKind));
		CHECK(masterMob->AddMasterSlot (pMediaKind, DEFAULT_FILE_SLOT, _compFileMob, masterSlotID, L"A Slot"));	// Should be NULL or something useful!!!
		if(_destination != NULL)
		{
			if(EqualAUID(&_containerDefID, &aafFormat))
			{
				//!!!				ImplAAFMob	*destmob;
				
				//!!!				CHECK(CloneExternal (kFollowDepend, kNoIncludeMedia, _destFile, &destMob));
			}
		}
		
		CHECK(CreateContainerDef(compHead))
		//!!! As an optimization, use clone to move a copy of the definition objects into the media file.
		// This can be just a copy because there shouldn't be any definitions there yet...
		
		// There isn't yet a container for AAF data, so this must be special-cased
		if(EqualAUID(&_containerDefID, &aafFormat))
		{
			aafUID_t	essenceDataID;
			CHECK(_codec->GetEssenceDataID(&essenceDataID));
			ImplAAFClassDefSP pEssenceDataClass;
			CHECK(dataDict->LookupClassDef (essenceDataID, &pEssenceDataClass));
			CHECK(pEssenceDataClass->
				  CreateInstance((ImplAAFObject **)&implData));

			CHECK(implData->SetFileMob(_dataFileMob == NULL ? _compFileMob : _dataFileMob));
			CHECK(dataHead->AddEssenceData (implData));

			CHECK(plugins->CreateInstance(CLSID_AAFEssenceDataStream,
				NULL, 
				IID_IAAFEssenceStream, 
				(void **)&_stream));
			CHECK(_stream->QueryInterface(IID_IAAFEssenceDataStream, (void **)&edStream));
			
      dataObj = static_cast<IUnknown *> (implData->GetContainer());
      edStream->Init(dataObj);
			edStream->Release();
			edStream = NULL;

      // Save the original essence data pointer so that we can use it to
      // to implement the sample index if the codec needs an index.
      CHECK(dataObj->QueryInterface(IID_IAAFEssenceData, (void **)(void **)&_internalEssenceData));

      implData->ReleaseReference();
      implData = NULL;
		}
		else
		{
			_destination->GetPathBufLen(&buflen);
			nameBuf = new wchar_t[buflen];
			if(nameBuf == NULL)
				RAISE(AAFRESULT_NOMEMORY);
			CHECK(_destination->GetPath(nameBuf, buflen));
			
			CHECK(plugins->GetPluginInstance(_containerDefID, &plug));
			CHECK(plug->QueryInterface(IID_IAAFEssenceContainer, (void **)&container));
			plug->Release();
			plug = NULL;
			
			CHECK(container->CreateEssenceStream(nameBuf, &fileMobUID, &_stream));

			delete [] nameBuf;
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
		
    CHECK(InstallEssenceAccessIntoCodec());

		iUnk = static_cast<IUnknown *> (_compFileMob->GetContainer());	// Codec knowns about compFilemob only
		CHECK(iUnk->QueryInterface(IID_IAAFSourceMob, (void **)&iFileMob));
		CHECK(_codec->Create(iFileMob, _flavour, mediaKind, sampleRate, _stream, compEnable));

    iFileMob->Release();
    iFileMob = NULL;

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
    if (NULL != iFileMob)
    {
      iFileMob->Release();
      iFileMob = NULL;
    }
		if(dataDict != NULL)
		  dataDict->ReleaseReference();
		dataDict = 0;
		if(compHead != NULL)
		  compHead->ReleaseReference();
		compHead = 0;
		if(dataHead != NULL)
		  dataHead->ReleaseReference();
		dataHead = 0;
		if(plugins != NULL)
		  plugins->ReleaseReference();
		plugins = 0;
		if(implData != NULL)
		  implData->ReleaseReference();
		implData = 0;
		if(nameBuf != NULL)
			delete [] nameBuf;
		if(edStream != NULL)
			edStream->Release();
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
							aafUID_constref codecID,
                          aafUInt16  arrayElemCount,
                           aafmMultiCreate_t *  mediaArray,
                           aafCompressEnable_t  compEnable)
{
	IUnknown				*dataObj = NULL;
	IAAFSourceMob			*iFileMob = NULL;
	IUnknown				*iUnk = NULL;
	IAAFEssenceDataStream	*edStream = NULL;
	IAAFPlugin				*plugin = NULL;
	IAAFEssenceContainer	*container = NULL;
	IAAFPlugin				*plug = NULL;
	aafMobID_t				fileMobUID;
	aafUID_t				essenceKind;
	aafLength_t				oneLength = CvtInt32toLength(1, oneLength);
	ImplAAFDictionary		*dataDict = NULL;
	ImplAAFHeader			*compHead = NULL;
	ImplAAFHeader			*dataHead = NULL;
	ImplAAFPluginManager	*plugins = NULL;
	ImplAAFEssenceData		*implData = NULL;
	ImplAAFMobSlot			*tmpTrack = NULL;
	ImplAAFSourceMob		*fileMob = NULL;
	aafSubChannel_t			*destPtr;
	wchar_t					*nameBuf = NULL;
	aafUInt32				buflen;
	aafUInt32				n;
	aafUID_t				aafFormat = ContainerAAF;
	aafmMultiCreate_t		*initPtr;
	aafRational_t			sampleRate;


	XPROTECT()
	{
		_openType = kAAFCreated;
		_dataFile = NULL;
		
		CHECK(masterMob->MyHeadObject(&compHead));
		
		if((_destination != NULL) && EqualAUID(&_containerDefID, &ContainerAAF))
		{
			CHECK(CreateEssenceFileFromLocator (compHead, _destination, &_dataFile));
			CHECK(_dataFile->GetHeader(&dataHead));
		}
		else
		{
			CHECK(masterMob->MyHeadObject(&dataHead));
		}
		
		// Can't put raw media inside of an AAF File
		if(_destination == NULL && !EqualAUID(&_containerDefID, &aafFormat))
			RAISE(AAFRESULT_WRONG_MEDIATYPE);
	
		_masterMob = masterMob;
		_masterMob->AcquireReference();
		
		_numChannels = arrayElemCount;
		_channels = (aafSubChannel_t *) new aafSubChannel_t[_numChannels];
		XASSERT((_channels != NULL), AAFRESULT_NOMEMORY);

		_codecID = codecID;
		
	
		plugins = ImplAAFContext::GetInstance()->GetPluginManager();
		//!!!Handle case where multiple codecs exist for a codecID
		CHECK(plugins->GetPluginInstance(_codecID, &plugin));
		CHECK(plugin->QueryInterface(IID_IAAFEssenceCodec, (void **)&_codec));
		plugin->Release();
		plugin = NULL;

    // Initialize the multi-essence codec interface pointer (not required for this type of open).
    CHECK(_codec->QueryInterface(IID_IAAFMultiEssenceCodec, (void **)&_multicodec));
    
	// This line moved up method as dataDict is needed earlier - Joseph.Lord@rd.bbc.co.uk
		CHECK(dataHead->GetDictionary(&dataDict));

		if(_destination != NULL)
		{
			if(EqualAUID(&_containerDefID, &aafFormat))
			{
				//!!!				ImplAAFMob	*destmob;
				
				//!!!				CHECK(CloneExternal (kFollowDepend, kNoIncludeMedia, _destFile, &destMob));
			}
		}
		
		CHECK(CreateCodecDef(compHead, codecID, &_codecDescriptor));
		CHECK(CreateContainerDef(compHead))
		

		// When we enable the cloneExternal (below) then Don't do this call for creating the
		// file mob twice.
		// Don't add slots here, as they will be added in the loop below
		sampleRate = mediaArray[0].sampleRate;		// !!! Assumes that edit rate == sample rate of channel 1
		essenceKind = *(mediaArray[0].mediaKind);	// Assumes that mediaKind somes from track #1
		CHECK(CreateFileMob(compHead, kAAFFalse, 0, NULL, essenceKind, _codecID, sampleRate, sampleRate,
			_destination, &_compFileMob));
		CHECK(_compFileMob->GetMobID(&fileMobUID));

		if(compHead != dataHead)
		{
			CHECK(CreateFileMob(dataHead, kAAFFalse, 0, &fileMobUID, essenceKind, _codecID, sampleRate, sampleRate,
				NULL, &_dataFileMob));
			_dataFileMob->AcquireReference();	//!!!Leaking here?
		}
		else
		{
			_dataFileMob = NULL;
		}

		// Added by Joseph Lord
		// Just guessing that this setting of fileMob is appropriate.
		fileMob = _compFileMob;

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
				    ImplAAFDataDefSP pInitMediaKind;
					CHECK(dataDict->LookupDataDef (*initPtr->mediaKind,
												   &pInitMediaKind));
					CHECK(fileMob->AddNilReference(initPtr->slotID,
												   oneLength,
												   pInitMediaKind,
												   initPtr->sampleRate));
					if(tmpTrack != NULL)
					{
						tmpTrack->ReleaseReference();
						tmpTrack = NULL;
					}
				}
				CHECK(fileMob->FindSlotBySlotID(initPtr->slotID, &tmpTrack));
				CHECK(tmpTrack->SetPhysicalNum(initPtr->subTrackNum));
			}
			destPtr->mediaKind = *(initPtr->mediaKind);
			destPtr->trackID = initPtr->slotID;
			destPtr->physicalOutChan = initPtr->subTrackNum;
			ImplAAFDataDefSP pEssenceKind;
			CHECK(dataDict->LookupDataDef (essenceKind, &pEssenceKind));
			CHECK(masterMob->AddMasterSlot (pEssenceKind, initPtr->slotID, _compFileMob, initPtr->slotID, L"A Slot"));	// Should be NULL or something useful!!!
			if(tmpTrack != NULL)
			{
				tmpTrack->ReleaseReference();
				tmpTrack = NULL;
			}
//!!!			CvtInt32toPosition(0, destPtr->dataOffset);
//!!!			CvtInt32toLength(0, destPtr->numSamples);
//!!!			destPtr->sampleRate = initPtr->sampleRate;
		}

			//!!! As an optimization, use clone to move a copy of the definition objects into the media file.
			// This can be just a copy because there shouldn't be any definitions there yet...
			
			if((dataHead != compHead) && (dataHead != NULL))
			{
				CHECK(CreateContainerDef(dataHead))
					CHECK(CreateCodecDef(dataHead, codecID, NULL));
			}
			
			// There isn't yet a container for AAF data, so this must be special-cased
			if(EqualAUID(&_containerDefID, &aafFormat))
			{
				aafUID_t	essenceDataID;
				
				CHECK(_codec->GetEssenceDataID(&essenceDataID));

				ImplAAFClassDefSP pEssenceDataClass;
				CHECK(dataDict->LookupClassDef(essenceDataID, &pEssenceDataClass));
				CHECK(pEssenceDataClass->
					  CreateInstance((ImplAAFObject **)&implData));
			
				CHECK(implData->SetFileMob(_dataFileMob == NULL ? _compFileMob : _dataFileMob));
				CHECK(dataHead->AddEssenceData (implData));
				
				CHECK(plugins->CreateInstance(CLSID_AAFEssenceDataStream,
					NULL, 
					IID_IAAFEssenceStream, 
					(void **)&_stream));
				CHECK(_stream->QueryInterface(IID_IAAFEssenceDataStream, (void **)&edStream));
				
        dataObj = static_cast<IUnknown *> (implData->GetContainer());
        edStream->Init(dataObj);
				edStream->Release();
				edStream = NULL;

        // Save the original essence data pointer so that we can use it to
        // to implement the sample index if the codec needs an index.
        CHECK(dataObj->QueryInterface(IID_IAAFEssenceData, (void **)&_internalEssenceData));

        implData->ReleaseReference();
        implData = NULL;
			}
			else
			{
				_destination->GetPathBufLen(&buflen);
				nameBuf = new wchar_t[buflen];
				if(nameBuf == NULL)
					RAISE(AAFRESULT_NOMEMORY);
				CHECK(_destination->GetPath(nameBuf, buflen));
				
				CHECK(plugins->GetPluginInstance(_containerDefID, &plug));
				CHECK(plug->QueryInterface(IID_IAAFEssenceContainer, (void **)&container));
				plug->Release();
				plug = NULL;
				
				CHECK(container->CreateEssenceStream(nameBuf, &fileMobUID, &_stream));

				delete [] nameBuf;
				nameBuf = NULL;			
				container->Release();
				container = NULL;
			}
			
			CHECK(compHead->SetModified());		// To NOW
			if(dataHead != compHead)
			{
				CHECK(dataHead->SetModified());	// To NOW
			}
 
      CHECK(InstallEssenceAccessIntoCodec());
     
			iUnk = static_cast<IUnknown *> (_compFileMob->GetContainer());	// Codec knowns about compFilemob only
			CHECK(iUnk->QueryInterface(IID_IAAFSourceMob, (void **)&iFileMob));
      CHECK(_multicodec->MultiCreate(iFileMob, _flavour, _stream, compEnable, arrayElemCount, mediaArray));

      iFileMob->Release();
      iFileMob = NULL;
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
    if (NULL != iFileMob)
    {
      iFileMob->Release();
      iFileMob = NULL;
    }
		if(fileMob != NULL)
		  fileMob->ReleaseReference();
		fileMob = 0;
		if(tmpTrack != NULL)
		  tmpTrack->ReleaseReference();
		tmpTrack = 0;
		if(dataDict != NULL)
		  dataDict->ReleaseReference();
		dataDict = 0;
		if(compHead != NULL)
		  compHead->ReleaseReference();
		compHead = 0;
		if(dataHead != NULL)
		  dataHead->ReleaseReference();
		dataHead = 0;
		if(plugins != NULL)
		  plugins->ReleaseReference();
		plugins = 0;
		if(implData != NULL)
		  implData->ReleaseReference();
		implData = 0;
		if(nameBuf != NULL)
			delete [] nameBuf;
		if(edStream != NULL)
			edStream->Release();
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
	ImplAAFDictionary		*dict = NULL;
	ImplAAFContainerDef		*containerDef = NULL;
	ImplAAFCodecDef			*codecDef = NULL;
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
	aafUInt16		channelIndex;
	aafUID_t		 mediaKind, myFileCLSID;
	aafMobID_t		 fileMobID;
	aafLength_t masterMobLength;
	aafSourceRef_t	fileRef;
	aafUInt16		numCh;
	wchar_t				*nameBuf = NULL;
	aafUInt32			buflen;
	aafUID_t			essenceDescClass, codecID;
	aafBool					found = kAAFFalse, isIdentified;
	aafUID_t				testFormat;

	
	XPROTECT()
	{
		CHECK(masterMob->MyHeadObject(&compHead));

		_openType = kAAFReadOnly;
		CvtInt32toPosition(0, zeroPos);	
		CHECK(masterMob->SearchSource(slotID, zeroPos,kAAFFileMob,
									   mediaCrit,
									   NULL,
									   &sourceInfo));
//		masterMob->AcquireReference();		//!!!DEBUG
									   
		CHECK(masterMob->FindSlotBySlotID (slotID,&slot));
		CHECK(slot->GetSegment(&seg));
		slot->ReleaseReference();
		slot = NULL;

		ImplAAFDataDefSP pDataDef;
		CHECK(seg->GetDataDef(&pDataDef));
		CHECK(pDataDef->GetAUID(&mediaKind));
		CHECK(seg->GetLength(&masterMobLength));
		seg->ReleaseReference();
		seg = NULL;
		
		CHECK(sourceInfo->GetMob((ImplAAFMob **)&fileMob));
		CHECK(sourceInfo->GetSourceReference(&fileRef));
		sourceInfo->ReleaseReference();
		sourceInfo = NULL;

		CHECK(fileMob->GetEssenceDescriptor((ImplAAFEssenceDescriptor **)&_mdes));
		if(_mdes->GetContainerFormat (&containerDef) == AAFRESULT_SUCCESS)
		{
			CHECK(containerDef->GetAUID(&testFormat));
			_containerDefID = testFormat;
		}
		else
		{
			_containerDefID = ContainerAAF;
			CHECK(compHead->GetDictionary (&dict));
			CHECK(dict->LookupContainerDef (_containerDefID, &containerDef));
			dict->ReleaseReference();
			dict = NULL;
		}
    
    
		CHECK(fileMob->GetMobID(&fileMobID));
		CHECK(_mdes->GetObjectClass(&essenceDescClass));
		CHECK(_mdes->GetCodecDef(&codecDef));
		CHECK(codecDef->GetAUID(&codecID));
		codecDef->ReleaseReference();
		codecDef = NULL;

		_codecID = codecID;
	
		plugins = ImplAAFContext::GetInstance()->GetPluginManager();
		CHECK(plugins->CreateInstanceFromDefinition(
								codecID, NULL, IID_IAAFEssenceCodec, (void**)&_codec));
	
		// Initialize the multi-essence codec interface pointer (not required for this type of open).
		_codec->QueryInterface(IID_IAAFMultiEssenceCodec, (void **)&_multicodec);

//!!!		_physicalOutChanOpen = physicalOutChan;
		


		found = kAAFFalse;
		if(containerDef->EssenceIsIdentified (&isIdentified) != AAFRESULT_SUCCESS)
			isIdentified = kAAFTrue;
		containerDef->ReleaseReference();
		containerDef = NULL;

		if(isIdentified)
		{						
			CHECK(fileMob->GetMobID(&fileMobID));
			CHECK(compHead->GetContentStorage (&cStore));
			if(cStore->LookupEssenceData (fileMobID, &essenceData) == AAFRESULT_SUCCESS)
			{
				found = kAAFTrue;
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

        // Save the original essence data pointer so that we can use it to
        // to implement the sample index if the codec needs an index.
        CHECK(edUnknown->QueryInterface(IID_IAAFEssenceData, (void **)&_internalEssenceData));

        essenceData->ReleaseReference();
        essenceData = NULL;
			}
			cStore->ReleaseReference();
			cStore = NULL;
		}

		if(found == kAAFFalse)
		{
			AAFRESULT				status;

			CHECK(_mdes->GetLocators(&enumLocate));
			while(!found && (enumLocate->NextOne (&pLoc) == AAFRESULT_SUCCESS))
			{
				
				pLoc->GetPathBufLen(&buflen);
				nameBuf = new wchar_t[buflen];
				if(nameBuf == NULL)
					RAISE(AAFRESULT_NOMEMORY);
				CHECK(pLoc->GetPath(nameBuf, buflen));
				if(EqualAUID(&_containerDefID, &ContainerAAF) == kAAFTrue)
				{
					if(openMode == kAAFMediaOpenAppend)
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
						if(cStore->LookupEssenceData (fileMobID, &essenceData) == AAFRESULT_SUCCESS)
							found = kAAFTrue;
						cStore->ReleaseReference();
						cStore = NULL;

						if(found && EqualAUID(&_containerDefID, &ContainerAAF))
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

              // Save the original essence data pointer so that we can use it to
              // to implement the sample index if the codec needs an index.
              CHECK(edUnknown->QueryInterface(IID_IAAFEssenceData, (void **)&_internalEssenceData));
						}

            if (essenceData)
            {
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
					CHECK(plugins->GetPluginInstance(_containerDefID, &plug));
					CHECK(plug->QueryInterface(IID_IAAFEssenceContainer, (void **)&container));
					
					if(openMode == kAAFMediaOpenReadOnly)
					{
						status = container->OpenEssenceStreamReadOnly (nameBuf, &fileMobID, &_stream);
					}
					else if(openMode == kAAFMediaOpenAppend)
					{
						status = container->OpenEssenceStreamAppend (nameBuf, &fileMobID, &_stream);
					}
					else
						RAISE(AAFRESULT_MEDIA_OPENMODE);
					
					if(status == AAFRESULT_SUCCESS)
						found = kAAFTrue;
					container->Release();
					container = NULL;
					plug->Release();
					plug = NULL;
				}

				delete [] nameBuf;
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
		CHECK(_codec->CountChannels(iFileMob, mediaKind, _stream, &numCh));
		if (numCh == 0)
		  RAISE(AAFRESULT_INVALID_DATADEF);
		
		_channels = (aafSubChannel_t *) new aafSubChannel_t[numCh];
		if(_channels == NULL)
			RAISE(AAFRESULT_NOMEMORY);
		_numChannels = numCh;
		for(channelIndex = 0; channelIndex < numCh; channelIndex++)
		{
			_channels[channelIndex].mediaKind = mediaKind;
			_channels[channelIndex].physicalOutChan = channelIndex+1;
			_channels[channelIndex].trackID = slotID+channelIndex;
		}
    
    CHECK(InstallEssenceAccessIntoCodec());
   
    CHECK(_codec->Open(iFileMob, openMode, _stream, compEnable));
		
		_masterMob = masterMob;
		_masterMob->AcquireReference();
		iFileMob->Release();
		iFileMob = NULL;
    fileMob->ReleaseReference();
    fileMob = 0;
		compHead->ReleaseReference();
		compHead = NULL;
		if(plugins != NULL)
		  plugins->ReleaseReference();
		plugins = 0;
	}
	XEXCEPT
	{
    if (NULL != iFileMob)
    {
      iFileMob->Release();
      iFileMob = NULL;
    }
		if(_dataFile != NULL)
			_dataFile->ReleaseReference();
		_dataFile = NULL;
		if(plugins != NULL)
		  plugins->ReleaseReference();
		plugins = 0;
		if(edStream != NULL)
			edStream->Release();
		if(container != NULL)
			container->Release();
		if(plug != NULL)
			plug->Release();
		if(dict != NULL)
		  dict->ReleaseReference();
		dict = 0;
		if(containerDef != NULL)
		  containerDef->ReleaseReference();
		containerDef = 0;
		if(cStore != NULL)
		  cStore->ReleaseReference();
		cStore = 0;
		if(slot != NULL)
		  slot->ReleaseReference();
		slot = 0;
		if(seg != NULL)
		  seg->ReleaseReference();
		seg = 0;
		if(sourceInfo != NULL)
		  sourceInfo->ReleaseReference();
		sourceInfo = 0;
		if(fileMob != NULL)
		  fileMob->ReleaseReference();
		fileMob = 0;
		if(dataHead != NULL)
		  dataHead->ReleaseReference();
		dataHead = 0;
		if(essenceData != NULL)
		  essenceData->ReleaseReference();
		essenceData = 0;
		if(compHead != NULL)
		  compHead->ReleaseReference();
		compHead = 0;
		if(enumLocate != NULL)
		  enumLocate->ReleaseReference();
		enumLocate = 0;
		if(pLoc != NULL)
		  pLoc->ReleaseReference();
		pLoc = 0;
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
	aafUInt16		channelIndex;
	aafUID_t		 mediaKind, myFileCLSID;
	aafMobID_t		 fileMobID;
	aafLength_t masterMobLength;
	aafSourceRef_t	fileRef;
	aafUInt16		numCh;
	wchar_t				*nameBuf = NULL;
	aafUInt32			buflen;
	aafUID_t			essenceDescClass;
	aafBool					found = kAAFFalse, isIdentified;
	aafUID_t				testFormat;

  
	XPROTECT()
	{
		CHECK(masterMob->MyHeadObject(&compHead));

		_openType = kAAFReadOnly;
		CvtInt32toPosition(0, zeroPos);	
		CHECK(masterMob->SearchSource(slotID, zeroPos,kAAFFileMob,
									   mediaCrit,
									   NULL,
									   &sourceInfo));
//		masterMob->AcquireReference();		//!!!DEBUG
									   
		CHECK(masterMob->FindSlotBySlotID (slotID,&slot));
		CHECK(slot->GetSegment(&seg));
		slot->ReleaseReference();
		slot = NULL;

		ImplAAFDataDefSP pDataDef;
		CHECK(seg->GetDataDef(&pDataDef));
		CHECK(pDataDef->GetAUID(&mediaKind));
		CHECK(seg->GetLength(&masterMobLength));
		seg->ReleaseReference();
		seg = NULL;
		
		CHECK(sourceInfo->GetMob((ImplAAFMob **)&fileMob));
		CHECK(sourceInfo->GetSourceReference(&fileRef));
		sourceInfo->ReleaseReference();
		sourceInfo = NULL;

		CHECK(fileMob->GetEssenceDescriptor((ImplAAFEssenceDescriptor **)&_mdes));

		CHECK(_mdes->GetContainerFormat (&containerDef));
		CHECK(containerDef->GetAUID(&testFormat));
		_containerDefID = testFormat;
        
    CHECK(fileMob->GetMobID(&fileMobID));
		CHECK(_mdes->GetObjectClass(&essenceDescClass));

		plugins = ImplAAFContext::GetInstance()->GetPluginManager();
		CHECK(plugins->MakeCodecFromEssenceDesc(fileMob, _stream, &_codec));

    // Initialize the multi-essence codec interface pointer (not required for this type of open).
    CHECK(_codec->QueryInterface(IID_IAAFMultiEssenceCodec, (void **)&_multicodec));
    
//!!!		_physicalOutChanOpen = physicalOutChan;

		CHECK(compHead->GetDictionary (&dict));
		CHECK(dict->LookupContainerDef (testFormat, &containerDef));
		dict->ReleaseReference();
		dict = NULL;

		found = kAAFFalse;
		CHECK(containerDef->EssenceIsIdentified (&isIdentified));
		containerDef->ReleaseReference();
		containerDef = NULL;

		if(isIdentified)
		{						
			CHECK(fileMob->GetMobID(&fileMobID));
			CHECK(compHead->GetContentStorage (&cStore));
			if(cStore->LookupEssenceData (fileMobID, &essenceData) == AAFRESULT_SUCCESS)
			{
				found = kAAFTrue;
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

        // Save the original essence data pointer so that we can use it to
        // to implement the sample index if the codec needs an index.
        CHECK(edUnknown->QueryInterface(IID_IAAFEssenceData, (void **)&_internalEssenceData));

        essenceData->ReleaseReference();
        essenceData = NULL;
			}
			cStore->ReleaseReference();
			cStore = NULL;
		}

		if(found == kAAFFalse)
		{
			AAFRESULT				status;

			CHECK(_mdes->GetLocators(&enumLocate));
			while(!found && (enumLocate->NextOne (&pLoc) == AAFRESULT_SUCCESS))
			{
				
				pLoc->GetPathBufLen(&buflen);
				nameBuf = new wchar_t[buflen];
				if(nameBuf == NULL)
					RAISE(AAFRESULT_NOMEMORY);
				CHECK(pLoc->GetPath(nameBuf, buflen));
				if(EqualAUID(&_containerDefID, &ContainerAAF) == kAAFTrue)
				{
					if(openMode == kAAFMediaOpenAppend)
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
						if(cStore->LookupEssenceData (fileMobID, &essenceData) == AAFRESULT_SUCCESS)
							found = kAAFTrue;
						cStore->ReleaseReference();
						cStore = NULL;

						if(found && EqualAUID(&_containerDefID, &ContainerAAF))
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

              // Save the original essence data pointer so that we can use it to
              // to implement the sample index if the codec needs an index.
              CHECK(edUnknown->QueryInterface(IID_IAAFEssenceData, (void **)&_internalEssenceData));
						}

            if (essenceData)
            {            
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
					CHECK(plugins->GetPluginInstance(_containerDefID, &plug));
					CHECK(plug->QueryInterface(IID_IAAFEssenceContainer, (void **)&container));
					
					if(openMode == kAAFMediaOpenReadOnly)
					{
						status = container->OpenEssenceStreamReadOnly (nameBuf, &fileMobID, &_stream);
					}
					else if(openMode == kAAFMediaOpenAppend)
					{
						status = container->OpenEssenceStreamAppend (nameBuf, &fileMobID, &_stream);
					}
					else
						RAISE(AAFRESULT_MEDIA_OPENMODE);
					
					if(status == AAFRESULT_SUCCESS)
						found = kAAFTrue;
					container->Release();
					container = NULL;
					plug->Release();
					plug = NULL;
				}

				delete [] nameBuf;
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
		CHECK(_codec->CountChannels(iFileMob, mediaKind, _stream, &numCh));
		if (numCh == 0)
		  RAISE(AAFRESULT_INVALID_DATADEF);
		
		_channels = (aafSubChannel_t *) new aafSubChannel_t[numCh];
		if(_channels == NULL)
			RAISE(AAFRESULT_NOMEMORY);
		_numChannels = numCh;
		for(channelIndex = 0; channelIndex < numCh; channelIndex++)
		{
			_channels[channelIndex].mediaKind = mediaKind;
			_channels[channelIndex].physicalOutChan = channelIndex+1;
			_channels[channelIndex].trackID = slotID+channelIndex;
		}

    CHECK(InstallEssenceAccessIntoCodec());

		CHECK(_multicodec->MultiOpen(iFileMob, openMode, _stream, compEnable));
		
		_masterMob = masterMob;
		_masterMob->AcquireReference();
    iFileMob->Release();
    iFileMob = NULL;
		fileMob->ReleaseReference();
		fileMob = NULL;
		compHead->ReleaseReference();
		compHead = NULL;
		if(plugins != NULL)
		  plugins->ReleaseReference();
		plugins = 0;
	}
	XEXCEPT
	{
		if(NULL != iFileMob)
			iFileMob->Release();
		if(_dataFile != NULL)
			_dataFile->ReleaseReference();
		_dataFile = NULL;
		if(plugins != NULL)
		  plugins->ReleaseReference();
		plugins = 0;
		if(edStream != NULL)
			edStream->Release();
//		if(edUnknown != NULL)
//			edUnknown->Release();
		if(container != NULL)
			container->Release();
		if(plug != NULL)
			plug->Release();
		if(dict != NULL)
		  dict->ReleaseReference();
		dict = 0;
		if(containerDef != NULL)
		  containerDef->ReleaseReference();
		containerDef = 0;
		if(cStore != NULL)
		  cStore->ReleaseReference();
		cStore = 0;
		if(slot != NULL)
		  slot->ReleaseReference();
		slot = 0;
		if(seg != NULL)
		  seg->ReleaseReference();
		seg = 0;
		if(sourceInfo != NULL)
		  sourceInfo->ReleaseReference();
		sourceInfo = 0;
		if(fileMob != NULL)
		  fileMob->ReleaseReference();
		fileMob = 0;
		if(dataHead != NULL)
		  dataHead->ReleaseReference();
		dataHead = 0;
		if(essenceData != NULL)
		  essenceData->ReleaseReference();
		essenceData = 0;
		if(compHead != NULL)
		  compHead->ReleaseReference();
		compHead = 0;
		if(enumLocate != NULL)
		  enumLocate->ReleaseReference();
		enumLocate = 0;
		if(pLoc != NULL)
		  pLoc->ReleaseReference();
		pLoc = 0;
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
  if (NULL != _destination)
    _destination->ReleaseReference();
	_destination = destination;
	if(destination != NULL)
		destination->AcquireReference();
	_containerDefID = fileFormat;
	return AAFRESULT_SUCCESS;
}



 //Sets which flavour of the codec ID is to be used.)
AAFRESULT ImplAAFEssenceAccess::SetEssenceCodecFlavour(aafUID_t flavour)
{
	_flavour = flavour;
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
    ImplAAFEssenceAccess::WriteMultiSamples (aafUInt16  arrayElemCount,
                           aafmMultiXfer_t *xferArray, aafmMultiResult_t *resultArray)
{

	aafAssert((_openType == kAAFCreated) ||
			  (_openType == kAAFAppended), _mainFile, AAFRESULT_MEDIA_OPENMODE);
	
	XPROTECT()
	{
    if (NULL == _multicodec)
    {
      // The codec does not support the IAAFMultiEssenceCodec interface.
      RAISE(AAFRESULT_INVALID_OP_CODEC);
    }
		CHECK (_multicodec->WriteBlocks(kAAFdeinterleave, arrayElemCount, xferArray, resultArray));
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
                           aafUInt32  buflen,
                           aafDataBuffer_t  buffer,
                           aafUInt32 *samplesWritten,
                           aafUInt32 *bytesWritten)
{
	aafAssert(_numChannels == 1, _mainFile,
		AAFRESULT_SINGLE_CHANNEL_OP);
	aafAssert(buffer != NULL, _mainFile, AAFRESULT_BADDATAADDRESS);
	aafAssert((_openType == kAAFCreated) ||
		  (_openType == kAAFAppended), _mainFile, AAFRESULT_MEDIA_OPENMODE);
	
  XPROTECT()
  {
    CHECK (_codec->WriteSamples(nSamples, buflen, buffer, samplesWritten, bytesWritten));
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
				CHECK(_codec->CompleteWrite(iCompFM));
				iCompFM->Release();
				iCompFM = NULL;
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
		if(iCompFM != NULL)
			iCompFM->Release();
	}
	XEND
	
	return(AAFRESULT_SUCCESS);
}

	//@comm This function should be called whether the essence was opened or created.
	//@comm Replaces omfmMediaClose

	
/****/

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::CountChannels (ImplAAFMasterMob *masterMob,
                           aafSlotID_t  slotID,
                           aafMediaCriteria_t*  mediaCrit,
                           aafUID_t mediaKind,
                           aafUInt16* numCh)
{
	aafPosition_t			zeroPos;
	ImplAAFFindSourceInfo	*sourceInfo = NULL;
	ImplAAFSourceMob		*fileMob = NULL;
	IAAFEssenceStream		*stream = NULL;
	ImplAAFDictionary		*dict = NULL;
	IUnknown				*iUnk = NULL;
	IAAFSourceMob			*iFileMob = NULL;
	ImplAAFPluginManager *plugins = NULL;

	aafAssert(numCh != NULL, _mainFile, AAFRESULT_NULL_PARAM);
	XPROTECT()
	{
		plugins = ImplAAFContext::GetInstance()->GetPluginManager();
		CvtInt32toPosition(0, zeroPos);	
		CHECK(masterMob->SearchSource(slotID, zeroPos,kAAFFileMob,
									   mediaCrit,
									   NULL,
									   &sourceInfo));
		CHECK(sourceInfo->GetMob((ImplAAFMob **)&fileMob));
    // TBD: Create and intialize an essence stream. (we really cannot
    // use the instance variable _stream since it is only initialized
    // AFTER by one of the Open or Create methods.

// TRR: Removed code: stream cannot be initialized!
//		CHECK(plugins->CreateInstance(CLSID_AAFEssenceDataStream,
//				NULL, 
//				IID_IAAFEssenceStream, 
//				(void **)&stream));
		iUnk = static_cast<IUnknown *> (fileMob->GetContainer());	// Codec knowns about compFilemob only
		CHECK(iUnk->QueryInterface(IID_IAAFSourceMob, (void **)&iFileMob));
		CHECK(_codec->CountChannels(iFileMob, mediaKind, stream, numCh));
		fileMob->ReleaseReference();
		fileMob = NULL;
		dict->ReleaseReference();
		dict = NULL;
//		stream->Release();
//		stream = NULL;
		plugins->ReleaseReference();
		plugins = NULL;
		iFileMob->Release();
		iFileMob = NULL;
	}
	XEXCEPT
	{
		if(iFileMob)
		  iFileMob->Release();
		if(fileMob)
		  fileMob->ReleaseReference();
		if(dict)
		  dict->ReleaseReference();
		dict = 0;
		if(stream)
			stream->Release();
		if (plugins)
		  plugins->ReleaseReference();
		plugins = 0;
	}
	XEND
	
	return(AAFRESULT_SUCCESS);
}

// INTERNAL to the SDK
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::GetSelectInfo (ImplAAFSourceMob *fileMob,
                           aafSelectInfo_t* pSelectInfo)
{
	IUnknown				*iUnk = NULL;
	IAAFSourceMob			*iFileMob = NULL;

	if (NULL == fileMob || NULL == pSelectInfo)
    return (AAFRESULT_NULL_PARAM);

	XPROTECT()
	{
    // TBD: Create and intialize an essence stream. (we really cannot
    // use the instance variable _stream since it is only initialized
    // AFTER by one of the Open or Create methods.

		iUnk = static_cast<IUnknown *> (fileMob->GetContainer());	// Codec knowns about compFilemob only
		CHECK(iUnk->QueryInterface(IID_IAAFSourceMob, (void **)&iFileMob));
		CHECK(_codec->GetSelectInfo(iFileMob, _stream, pSelectInfo));
		iFileMob->Release();
		iFileMob = NULL;
	}
	XEXCEPT
	{
		if(iFileMob)
			iFileMob->Release();
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
    ImplAAFEssenceAccess::GetLargestSampleSize
      (ImplAAFDataDef * pMediaKind,
	   aafLength_t *maxSize)
{
	aafAssert(pMediaKind != NULL, main, AAFRESULT_NULL_PARAM);
	aafAssert(maxSize != NULL, main, AAFRESULT_NULL_PARAM);

	XPROTECT()
	{
		aafUID_t mediaKind;
		CHECK(pMediaKind->GetAUID(&mediaKind));
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
    ImplAAFEssenceAccess::GetIndexedSampleSize (
                           ImplAAFDataDef * pMediaKind,
                           aafPosition_t frameNum,
                           aafLength_t* frameSize)
{
	aafAssert(pMediaKind != NULL, main, AAFRESULT_NULL_PARAM);
	aafAssert(frameSize != NULL, main, AAFRESULT_NULL_PARAM);

	XPROTECT()
	{
		aafUID_t mediaKind;
		CHECK(pMediaKind->GetAUID(&mediaKind));
		CHECK(_codec->GetIndexedSampleSize(mediaKind,
										   frameNum,
										   frameSize));
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
    ImplAAFEssenceAccess::CountSamples (
		ImplAAFDataDef * pMediaKind,
        aafLength_t *result)
{
//!!!	aafInt64		one;
	
	aafAssert(pMediaKind != NULL, _mainFile, AAFRESULT_NULL_PARAM);
	aafAssert(result != NULL, _mainFile, AAFRESULT_NULL_PARAM);

//!!!	CvtInt32toInt64(1, &one);
	
//!!!   if(Int64Greater(_pvt->repeatCount, one))
//	{
//	  *result = _pvt->repeatCount;
//	  return(AAFRESULT_SUCCESS);
//	}

	aafUID_t mediaKind;
	AAFRESULT hr = pMediaKind->GetAUID(&mediaKind);
	if (AAFRESULT_FAILED (hr))
	  return hr;
	return(_codec->CountSamples(mediaKind, result));
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
	XPROTECT()
	{
		CHECK(_codec->ReadSamples(nSamples, buflen, buffer, samplesRead, bytesRead));
	}
	XEXCEPT
	{
	}
	XEND

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
    ImplAAFEssenceAccess::ReadMultiSamples (aafUInt16  elemCount,
                           aafmMultiXfer_t *xferArray, aafmMultiResult_t *resultArray)
{
	aafAssert(xferArray != NULL, _mainFile, AAFRESULT_NULL_PARAM);
  if (NULL == _multicodec)
  {
    // The codec does not support the IAAFMultiEssenceCodec interface.
    return (AAFRESULT_INVALID_OP_CODEC);
  }
	return (_multicodec->ReadBlocks(kAAFdeinterleave, elemCount, xferArray, resultArray));
}

	//@comm arrayElemCount is the size of the array or transfer operations.
	// xferArray points to an array of transfer parameters.  All fields
	// in this array except for bytesXferred must be set up before
	// doing the transfer.
	//@comm Replaces omfmReadMultiSamples
	

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::Seek (aafPosition_t  frameNum)
{
//	aafPosition_t		one;
	AAFRESULT		status;
	
//	CvtInt32toInt64(1, &one);
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
	ImplAAFPluginDef	*desc = NULL;
	aafUID_t				descID;

	if(result == NULL)
		return AAFRESULT_NULL_PARAM;
	
	*result = kAAFFalse;
	XPROTECT()
	{
 		CHECK(_codec->GetEssenceDescriptorID (&descID))
		CHECK(mob->GetDictionary(&dict));
		CHECK(dict->LookupPluginDef(descID, &desc));
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
		desc = 0;
		if(dict != NULL)
		  dict->ReleaseReference();
		dict = 0;
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
//	IAAFDefObject	*def = NULL;
	
	XPROTECT()
	{
		//!!!Assert that _pluginDescriptor is NON-NULL
//		CHECK(_codecDescriptor->QueryInterface(IID_IAAFDefObject, (void **)&def));
//		CHECK(def->GetName (name, namelen));
		CHECK(_codec->GetCodecDisplayName (_flavour, name, namelen));
//		def->Release();
//		def = NULL;
	}
	XEXCEPT
	{
//		if(def != NULL)
//			def->Release();
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
//	IAAFPlugin	*plug = NULL;

//	XPROTECT()
//	{
		*codecID = _codecID;
//		CHECK(_codec->QueryInterface(IID_IAAFPlugin, (void **)&plug));
//!!!		CHECK(plug->GetPluggableID(codecID));
//		plug->Release();
//		plug = NULL;
//	}
//	XEXCEPT
//	{
//		if(plug != NULL)
//			plug->Release();
//	}
//	XEND
	
	return (AAFRESULT_SUCCESS);
}

	//@comm This will be required in order to send private data to the codec.
	//@comm The name will be truncated to fit within "buflen" bytes.
	//@comm Replaces omfmMediaGetCodecID */



/****/


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
		CHECK(dict->GetBuiltinDefs()->cdContainerDef()->
			  CreateInstance((ImplAAFObject **)&obj));
		if(obj == NULL)
			RAISE(AAFRESULT_NOMEMORY);
		uid = ContainerAAF;
		CHECK(obj->SetAUID(uid));
		CHECK(obj->SetName(L"AAF Container"));
		CHECK(obj->SetDescription(L"Essence is in an AAF file."));
		CHECK(obj->SetEssenceIsIdentified(kAAFTrue));
		*result = obj;
		// Don't bother acquire, as we would immediately release
		if(dict != NULL)
		  dict->ReleaseReference();
		dict = 0;
	}
	XEXCEPT
	{
		if(obj != NULL)
		  obj->ReleaseReference();
		obj = 0;
		if(dict != NULL)
		  dict->ReleaseReference();
		dict = 0;
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
		if(dict->LookupContainerDef (_containerDefID, &implContainerDef) != AAFRESULT_SUCCESS)
		{
			
			//!!!This should call into the pluginmanager instead of using if?
			if(EqualAUID(&_containerDefID, &ContainerAAF))
			{
				CHECK(MakeAAFContainerDef(head, &implContainerDef));
				CHECK(dict->RegisterContainerDef (implContainerDef));
			}
			else
			{
				pUnknown = static_cast<IUnknown *> (dict->GetContainer());
				CHECK(pUnknown->QueryInterface(IID_IAAFDictionary, (void **)&dictInterface));
				

				CHECK(plugins->GetPluginInstance(_containerDefID, &plug));
				CHECK(plug->GetIndexedDefinitionObject (/*!!!*/0, dictInterface, &pDef));
				CHECK(pDef->QueryInterface(IID_IAAFContainerDef, (void **)&containerDef));
				plug->Release();
				plug = NULL;
				CHECK(dictInterface->RegisterContainerDef (containerDef));
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
		dict = 0;
		if(plugins != NULL)
		  plugins->ReleaseReference();
		plugins = 0;
		if(implContainerDef != NULL)
		  implContainerDef->ReleaseReference();
		implContainerDef = 0;
		if(pDef != NULL)
			pDef->Release();
	}
	XEXCEPT
	{
		if(containerDef != NULL)
			containerDef->Release();
		if(dict != NULL)
		  dict->ReleaseReference();
		dict = 0;
		if(plugins != NULL)
		  plugins->ReleaseReference();
		plugins = 0;
		if(plug != NULL)
			plug->Release();
		if(dictInterface != NULL)
			dictInterface->Release();
		if(implContainerDef != NULL)
		  implContainerDef->ReleaseReference();
		implContainerDef = 0;
	}
	XEND
		
	return(AAFRESULT_SUCCESS);
}

AAFRESULT
ImplAAFEssenceAccess::CreateCodecDef (ImplAAFHeader *head, const aafUID_t & codecID, IAAFPluginDef **ppPluginDesc)
{
	IAAFPlugin					*plug = NULL;
	ImplAAFCodecDef				*codecImpl = NULL;
	IAAFDefObject				*def = NULL;
	IAAFCodecDef				*codecDef = NULL;
	IAAFDictionary				*dictInterface = NULL;
	IUnknown					*pUnknown = NULL;
	IAAFPluginDef				*pluginDesc = NULL;
	IEnumAAFPluginDefs			*descEnum = NULL;
	IAAFDefObject				*defInterface = NULL;
	ImplAAFDictionary			*dict = NULL;
	ImplAAFPluginManager		*plugins = NULL;
	aafBool						found = kAAFFalse;
	aafUID_t					currentPlugDesc, testAUID;

  if (NULL == head)
    return AAFRESULT_NULL_PARAM;

  if (ppPluginDesc)
    *ppPluginDesc = NULL;

	XPROTECT()
	{
    plugins = ImplAAFContext::GetInstance()->GetPluginManager();
		CHECK(head->GetDictionary (&dict));	//!!!Only makes essence in the current file?
    pUnknown = static_cast<IUnknown *> (dict->GetContainer());
		CHECK(pUnknown->QueryInterface(IID_IAAFDictionary, (void **)&dictInterface));
    CHECK(_codec->QueryInterface(IID_IAAFPlugin, (void **)&plug));

		if(dict->LookupCodecDef (codecID, &codecImpl) != AAFRESULT_SUCCESS)
		{
			CHECK(plug->GetIndexedDefinitionObject (/*!!!*/0,dictInterface, &def));
			CHECK(def->QueryInterface(IID_IAAFCodecDef, (void **)&codecDef));
			CHECK(dictInterface->RegisterCodecDef (codecDef));
			CHECK(dict->LookupCodecDef (codecID, &codecImpl));
			
		  codecDef->Release();
		  codecDef = NULL;
			def->Release();
			def = NULL;
		}
		
		CHECK(plug->GetPluginDescriptorID(&currentPlugDesc));
		CHECK(dictInterface->GetPluginDefs (&descEnum));
		
		while((kAAFTrue != found) && (descEnum->NextOne(&pluginDesc) == AAFRESULT_SUCCESS))
		{
			CHECK(pluginDesc->QueryInterface(IID_IAAFDefObject, (void **)&defInterface));
			CHECK(defInterface->GetAUID(&testAUID));
			if(EqualAUID(&testAUID, &currentPlugDesc))
			{
				found = kAAFTrue;
				if(ppPluginDesc != NULL)
				{
					CHECK(pluginDesc->SetDefinitionObjectID(codecID));
					pluginDesc->AddRef();	// About to store this 
					*ppPluginDesc = pluginDesc;
				}
			}
			pluginDesc->Release();
			pluginDesc = NULL;
		}
		descEnum->Release();
		descEnum = NULL;
		
//		if(found == kAAFFalse)	// If pluginDescriptor is not in place
//		{
//			
//			CHECK(plug->CreateDescriptor (dictInterface, &pluginDesc));
//			CHECK(codecDef->QueryInterface(IID_IAAFDefObject, (void **)&def));
//			CHECK(def->AppendPluginDef (pluginDesc));
//			if(ppPluginDesc != NULL)
//			{
//				pluginDesc->AddRef();	// About to store this 
//				*ppPluginDesc = pluginDesc;
//			}
//			pluginDesc->Release();
//			def->Release();
//		}
		
		dictInterface->Release();
		dictInterface = NULL;
		plug->Release();
		plug = NULL;
		if(codecImpl != NULL)
		  codecImpl->ReleaseReference();
		codecImpl = 0;
		if(dict != NULL)
		  dict->ReleaseReference();
		dict = 0;
		if(plugins != NULL)
		  plugins->ReleaseReference();
		plugins = 0;
	}
	XEXCEPT
	{
		if(codecImpl != NULL)
		  codecImpl->ReleaseReference();
		codecImpl = 0;
		if(dict != NULL)
		  dict->ReleaseReference();
		dict = 0;
		if(plugins != NULL)
		  plugins->ReleaseReference();
		plugins = 0;
		if(plug != NULL)
			plug->Release();
		if(codecDef != NULL)
			codecDef->Release();
		if(dictInterface != NULL)
			dictInterface->Release();
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
	aafProductIdentification_t		identSetup = { 0 };
	aafUInt32						length;
	aafUInt32						buflen;
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
		xferIdent = 0;
		if(theFile != NULL)
		  theFile->ReleaseReference();
		theFile = 0;
	}
	XEXCEPT
	{
		if(theFile != NULL)
		  theFile->ReleaseReference();
		theFile = 0;
		if(xferIdent != NULL)
		  xferIdent->ReleaseReference();
		xferIdent = 0;
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
	aafUInt32						buflen;
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
		xferIdent = 0;
		if(theFile != NULL)
		  theFile->ReleaseReference();
		theFile = 0;
	}
	XEXCEPT
	{
		if(theFile != NULL)
		  theFile->ReleaseReference();
		theFile = 0;
		if(xferIdent != NULL)
		  xferIdent->ReleaseReference();
		xferIdent = 0;
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
ImplAAFEssenceAccess::CreateFileMob (ImplAAFHeader *       newHead,
									 aafBool			   addSlots,
									 aafSlotID_t		   slotID,
									 aafMobID_constptr	   newMobID, /* optional */
									 const aafUID_t &	   mediaKind,
									 const aafUID_t &	   codecID,
									 const aafRational_t & editRate,
									 const aafRational_t & sampleRate,
									 ImplAAFLocator	*      addLocator,
									 ImplAAFSourceMob **   result)
{
	ImplAAFDictionary	*dict = NULL;
	ImplAAFContainerDef	*container = NULL;
	ImplAAFCodecDef		*codecDef = NULL;
	ImplAAFSourceMob	*fileMob = NULL;
	ImplAAFMobSlot		*tmpSlot = NULL;
	aafUID_t			essenceDescriptorID;
	ImplAAFFileDescriptor *mdes = NULL;

	XPROTECT()
	{
		CHECK(newHead->GetDictionary (&dict));
		/* Initialize the basic fields of the media handle
		 */
		CHECK(dict->GetBuiltinDefs()->cdSourceMob()->
			  CreateInstance((ImplAAFObject **)&fileMob));
		if(newMobID != NULL)
		{
			fileMob->SetMobID(*newMobID);
		}
		
		*result = fileMob;
		AcquireImplReference(fileMob);
		if(addSlots)
		{
			/* JeffB: Handle the case where an existing file=>tape mob connection exists
			 */
			if(fileMob->FindSlotBySlotID(slotID, &tmpSlot) == AAFRESULT_SLOT_NOT_FOUND)
			{
			    ImplAAFDataDefSP pMediaKind;
				CHECK(dict->LookupDataDef (mediaKind, &pMediaKind));
				CHECK(fileMob->AddNilReference(slotID, 
											   0,
											   pMediaKind,
											   editRate));
			}
			CHECK(fileMob->FindSlotBySlotID(slotID, &tmpSlot));
			CHECK(tmpSlot->SetPhysicalNum(slotID));
		}
//		CHECK(newHead->GetDictionary (&dict));
		CHECK(_codec->GetEssenceDescriptorID(&essenceDescriptorID));
		ImplAAFClassDefSP pClassDef;
		CHECK(dict->LookupClassDef (essenceDescriptorID, &pClassDef));
		CHECK(pClassDef->CreateInstance((ImplAAFObject **)&mdes));
		CHECK(mdes->SetSampleRate(sampleRate));
		CHECK(fileMob->SetEssenceDescriptor(mdes));
		CHECK(newHead->AddMob(fileMob));
		CHECK(dict->LookupContainerDef(_containerDefID, &container));
		CHECK(mdes->SetContainerFormat (container));
		CHECK(dict->LookupCodecDef(codecID, &codecDef));
		CHECK(mdes->SetCodecDef(codecDef));
		if(addLocator != NULL)
		{
			CHECK(mdes->AppendLocator(addLocator));
			CHECK(mdes->SetContainerFormat (container));
			CHECK(mdes->SetCodecDef(codecDef));
		}
    codecDef->ReleaseReference();
    codecDef = 0;
		if(dict != NULL)
		  dict->ReleaseReference();
		dict = 0;
		if(tmpSlot != NULL)
		  tmpSlot->ReleaseReference();
		tmpSlot = 0;
		if(mdes != NULL)
		  mdes->ReleaseReference();
		mdes = 0;
		if(fileMob != NULL)
		  fileMob->ReleaseReference();
		fileMob = 0;
		if(container != NULL)
		  container->ReleaseReference();
		container = 0;
	}
	XEXCEPT
	{
		if(codecDef != NULL)
		  codecDef->ReleaseReference();
		codecDef = 0;
		if(dict != NULL)
		  dict->ReleaseReference();
		dict = 0;
		if(tmpSlot != NULL)
		  tmpSlot->ReleaseReference();
		tmpSlot = 0;
		if(mdes != NULL)
		  mdes->ReleaseReference();
		mdes = 0;
		if(fileMob != NULL)
		  fileMob->ReleaseReference();
		fileMob = 0;
		if(container != NULL)
		  container->ReleaseReference();
		container = 0;
	}
	XEND
		
	return AAFRESULT_SUCCESS;
}


//
// InstallEssenceAccessIntoCodec
//
// This method encapsulates the call to the codec's SetEssenceAccess
// method so that we can check that the codec does not change the
// the current reference count of the this instance.
// TBD: Need class invariant to check that the reference count to
// this instance does not change when we call into an external plugin
// or codec.
// NOTE: This routine replaced the similar code that had been copy and
// pasted in four different places in this file.
//
//
AAFRESULT ImplAAFEssenceAccess::InstallEssenceAccessIntoCodec()
{
  AAFRESULT rc = AAFRESULT_SUCCESS;
  IUnknown *iUnk;
  IAAFEssenceAccess *iAccess = NULL;


  if (SUCCEEDED(rc))
  {
	  iUnk = static_cast<IUnknown *> (this->GetContainer());
	  rc = iUnk->QueryInterface(IID_IAAFEssenceAccess, (void **)&iAccess);
    if (SUCCEEDED(rc))
    {
      // Make sure that the codec does not change the reference change the
      // reference count of this object. If the codec increases the reference
      // count of the 
      aafUInt32 oldRefCount = ReferenceCount();
	    rc = _codec->SetEssenceAccess(iAccess);
      aafUInt32 newRefCount = ReferenceCount();
      if (newRefCount != oldRefCount)
      {
        if (newRefCount < oldRefCount) // then this object will be deleted
          rc = AAFRESULT_PLUGIN_INVALID_REFERENCE_COUNT;// prematurely (too many releases).
        else // newRefCount > oldRefCount then this object and the contained
          rc = AAFRESULT_PLUGIN_CIRCULAR_REFERENCE; // codec object will NEVER be deleted.
      }

      iAccess->Release();
    }
  }
  return rc;
}

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#ifndef __ImplAAFDataDef_h__
#include "ImplAAFDataDef.h"
#endif

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


#include <assert.h>
#include <string.h>


#include "AAFPlugin.h"
#include "AAFPluginManager.h"
#include "aafUtils.h"
#include "aafCvt.h"
#include "aafUtils.h"
#include "ImplAAFHeader.h"
#include "ImplAAFSourceMob.h"
#include "ImplAAFFileDescriptor.h"
#include "aafdefuids.h"
#include "ImplAAFObjectCreation.h"
#include "ImplAAFSession.h"
#include "ImplAAFWAVEDescriptor.h"	//!!!

#define DEFAULT_FILE_SLOT	1

extern "C" const aafClassID_t CLSID_AAFNetworkLocator;
extern "C" const aafClassID_t CLSID_AAFSourceMob;
extern "C" const aafClassID_t CLSID_AAFEssenceData;
extern "C" const CLSID CLSID_AAFEssenceStream = { 0x66FE33B1, 0x946D, 0x11D2, { 0x80, 0x89, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };
extern "C" const IID IID_IAAFEssenceStream = { 0x83402902, 0x9146, 0x11d2, { 0x80, 0x88, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };
extern "C" const IID IID_IAAFEssenceCodec = { 0x74867B42, 0x946E, 0x11D2, { 0x80, 0x89, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };
extern "C" const CLSID CLSID_AAFDefaultStream;

const CLSID CLSID_AAFEssenceDataStream = { 0x42A63FE1, 0x968A, 0x11d2, { 0x80, 0x89, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };
const IID IID_IAAFEssenceDataStream = { 0xCDDB6AB1, 0x98DC, 0x11d2, { 0x80, 0x8a, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };
const aafUID_t CLSID_AAFWaveCodec = { 0x8D7B04B1, 0x95E1, 0x11d2, { 0x80, 0x89, 0x00, 0x60, 0x08, 0x14, 0x3e, 0x6f } };

ImplAAFEssenceAccess::ImplAAFEssenceAccess ()
{
	_destination = NULL;

	_fileFormat = kAAFiMedia;
	_codecID = NoCodec;
	_variety = NilCodecVariety;
}


ImplAAFEssenceAccess::~ImplAAFEssenceAccess ()
{}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::CreateAudioEssence (ImplAAFMasterMob * /*masterMob*/,
                           aafSlotID_t  /*masterSlotID*/,
                           ImplAAFSourceMob * /*fileMob*/,
                           aafRational_t  /*samplerate*/,
                           aafCompressEnable_t  /*Enable*/,
                           aafInt16  /*sampleSize*/,
                           aafInt16  /*obj*/)
{
#if FULL_TOOLKIT
	aafAudioMemOp_t	opList[4];
	aafMultiCreate_t	init[MAX_DEF_AUDIO];
	aafInt16				n;
	aafUID_t				uid;
	AAFMob			*mobPtr;
	aafErr_t		status;
	aafRational_t	createEditRate;
	
	aafAssertValidFHdl(_mainFile);
	aafAssertMediaInitComplete(_mainFile);
	if(_mainFile->isAAFMedia())
	{
		aafAssert(fileMob != NULL, _mainFile, OM_ERR_INVALID_FILE_MOB);
	}
	
	XPROTECT(_mainFile)
	{
		if(numChannels == 1)
		{
			CHECK(Create(masterMob, masterTrackID, fileMob, 
										 _mainFile->_soundKind,
										 samplerate, enable));

			opList[0].opcode = kAAFSampleSize;
			opList[0].operand.sampleSize = sampleSize;
			opList[1].opcode = kAAFNumChannels;
			opList[1].operand.numChannels = numChannels;
			opList[2].opcode = kAAFSampleRate;
			opList[2].operand.sampleRate = samplerate;
			opList[3].opcode = kAAFAFmtEnd;
			CHECK(_codec->codecPutAudioInfo(this, opList));
		}
		else if(numChannels <= MAX_DEF_AUDIO)
		{
		  CHECK(fileMob->ReadUID(OMMOBJMobID, &uid));
			status = _mainFile->LookupMob(uid, &mobPtr);
			if((status != OM_ERR_NONE) || (mobPtr == NULL))
		  	{
				RAISE(OM_ERR_MISSING_MOBID);
		  	}

			for(n = 0; n < numChannels; n++)
			{
				init[n].mediaKind = _mainFile->_soundKind;
				init[n].subTrackNum = n+1;	/* Physical Output Channel */
				init[n].trackID = n+1;
				init[n].sampleRate = samplerate;
			}
			CHECK(mobPtr->GetCreateEditRate(&createEditRate));
			CHECK(MultiCreate(masterMob, fileMob, numChannels,
										init, createEditRate, enable));
			opList[0].opcode = kAAFSampleSize;
			opList[0].operand.sampleSize = sampleSize;
			opList[1].opcode = kAAFNumChannels;
			opList[1].operand.numChannels = numChannels;
			opList[2].opcode = kAAFSampleRate;
			opList[2].operand.sampleRate = samplerate;
			opList[3].opcode = kAAFAFmtEnd;
			CHECK(PutAudioInfoArray(opList));
		}
		else
			RAISE(OM_ERR_USE_MULTI_CREATE);
	}
	XEXCEPT
	{
	}
	XEND
	
	return (OM_ERR_NONE);
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

//@comm Creates either a single stream of audio essence, or interleaved
// audio-only data.  A separate call (MediaMultiCreate) exists 
// in order to create interleaved audio and video data.
// 
//@comm The essence object initialized from this call can be used with
// WriteDataSamples or WriteMultiSamples but NOT with 
// or WriteDataLines.
// 
// Argument Notes:
//@comm If you are creating the essence, and then attaching it to a master
// mob, then the "masterMob" field may be left NULL.
// 
//@comm The numChannels field refers to the number of interleaved
// channels on a single data stream.  
// 
//@comm The sample rate should be the actual samples per second, not the
// edit rate.
//@comm omfmAudioMediaCreate

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::VideoMediaCreate (ImplAAFMasterMob * /*masterMob*/,
                           aafSlotID_t  /*masterSlotID*/,
                           ImplAAFSourceMob * /*fileMob*/,
                           aafCompressEnable_t  /*Enable*/,
                           aafRational_t  /*editrate*/,
                           aafUInt32  /*StoredHeight*/,
                           aafUInt32  /*StoredWidth*/,
                           aafFrameLayout_t  /*layout*/,
                           aafRational_t  /*ratio*/)
{
#if FULL_TOOLKIT
	aafVideoMemOp_t	opList[4], op;

	aafAssertValidFHdl(_mainFile);
	aafAssertMediaInitComplete(_mainFile);
	if(_mainFile->isAAFMedia())
	{
		aafAssert(fileMob != NULL, _mainFile, OM_ERR_INVALID_FILE_MOB);
	}
	XPROTECT(_mainFile)
	{
		
		CHECK(Create(masterMob, masterTrackID, fileMob, _mainFile->_pictureKind,
					editrate, enable));
				
		CHECK(VideoOpInit(opList));
			
		op.opcode = kAAFFrameLayout;
		op.operand.expFrameLayout = layout;
		CHECK(VideoOpAppend(kAAFAppendIfAbsent, op, opList, 4L));
		
		op.opcode = kAAFStoredRect;
		op.operand.expRect.xSize = StoredWidth;
		op.operand.expRect.ySize = StoredHeight;
		op.operand.expRect.xOffset = op.operand.expRect.yOffset = 0;
		CHECK(VideoOpAppend(kAAFAppendIfAbsent, op, opList, 4L));
		
		op.opcode = kAAFAspectRatio;
		op.operand.expRational = iRatio;
		CHECK(VideoOpAppend(kAAFAppendIfAbsent, op, opList, 4L));
		
		CHECK(_codec->codecPutVideoInfo(this, opList));
	}
	XEXCEPT
	{
	}
	XEND
	
	return (OM_ERR_NONE);
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

AAFRESULT ImplAAFEssenceAccess::SetEssenceDestination(
				ImplAAFLocator		*destination,
				aafFileFormat_t		fileFormat)
{
	_destination = destination;
	_fileFormat = fileFormat;
	return AAFRESULT_SUCCESS;
}

 //Sets which variety of the codec ID is to be used.)
AAFRESULT ImplAAFEssenceAccess::SetEssenceCodecVariety(aafUID_t variety)
{
	_variety = variety;
	return AAFRESULT_SUCCESS;
}


//@comm Creates a single stream of video essence.  A separate call
// (MediaMultiCreate) exists to create interleaved audio and
// video data.
//@comm The essence object initialized from this call can be used with
// WriteDataSamples or WriteDataLines, but NOT with
// WriteMultiSamples.
//@comm If you are creating the essence, and then attaching it to a master
// mob, then the "masterMob" field may be left NULL.
//@comm The storedHeight and storedWidth are the dimensions of the frame
// as stored on disk (or as it should be restored by the codec.  The
// displayRect and sampledRect are set to:
// 	(0,0 @ sampledWidth, sampledHeight).
//@comm If the displayed rectangle is not the same as the stored rectangle
// (as with the old leadingLines and trailingLines), then you should
// call SetDisplayRect().
//@comm The frame layout contains the number of fields and whether they are
// interlaced, but does not specify field dominance.
//@comm Replaces omfmVideoMediaCreate

/****/
AAFRESULT STDMETHODCALLTYPE
ImplAAFEssenceAccess::Create (ImplAAFMasterMob *masterMob,
							  aafSlotID_t		masterSlotID,
							  aafUID_t			mediaKind,
							  aafUID_t			codecID,
							  aafRational_t	editRate,
							  aafRational_t	sampleRate,
							  aafCompressEnable_t  enable)
{
	aafUID_t			fileMobUID, audioDDEF = DDEF_Audio;
	aafLength_t			oneLength = CvtInt32toLength(1, oneLength);
	AAFRESULT			aafError = OM_ERR_NONE;
	ImplAAFSourceMob	*fileMob;
	ImplAAFMobSlot		*tmpSlot;
	ImplAAFHeader		*head;
	AAFPluginManager	*plugins;
	aafCodecMetaInfo_t	metaInfo;
	ImplAAFWAVEDescriptor *mdes;
	IUnknown			*dataObj;
	ImplAAFEssenceData	*implData;				
	CLSID				dataClass;
	
	XPROTECT()
	{
		_openType = kAAFCreated;
			
		CHECK(masterMob->MyHeadObject(&head));

		// Can't put raw media inside of an AAF File
		if(_destination == NULL && _fileFormat != kAAFiMedia)
			RAISE(AAFRESULT_WRONG_MEDIATYPE);
		
		/* Initialize the basic fields of the media handle
		*/
		fileMob = (ImplAAFSourceMob *)CreateImpl(CLSID_AAFSourceMob);
		CHECK(fileMob->GetMobID(&fileMobUID));
		
		_masterMob = masterMob;
		_fileMob = fileMob;
		_channels = (aafSubChannel_t *) new aafSubChannel_t[1];
		XASSERT((_channels != NULL), OM_ERR_NOMEMORY);
		_numChannels = 1;		 
		CvtInt32toPosition(0, _dataStart);
		CvtInt32toLength(0, _channels[0].numSamples);
		_channels[0].dataOffset = _dataStart;
		_channels[0].mediaKind = mediaKind;
		_channels[0].trackID = masterSlotID;
		_channels[0].physicalOutChan = 1;
		_channels[0].sampleRate = sampleRate;
		CvtInt32toLength(0, _channels[0].numSamples);
		
		//!!! Handle tje other cases of destination and format (raw, no locator not allowed)
		//	_destination = NULL;
		//	_fileFormat = kAAFiMedia;
		
		/* Initialize the fields which are derived from information in
		* the file mob or media descriptor.
		*/
		CHECK(fileMob->GetEssenceDescriptor((ImplAAFEssenceDescriptor **)&_mdes));
//!!!		CHECK(_mdes->SetSampleRate(&sampleRate));
		
		/* RPS-- don't use the 'best codec' method on WRITE. Instead,  */
		/*   the toolkit now stores the codec ID in omfmFileMobNew()   */
		/*   The string must be in the descriptor, assert as such.     */
		/* JeffB -- This doesn't work when adding media to a cloned File Mob */
		/* made by someone other than the toolkit.  So if the special string */
		/* isn't there, find the best codec to handle the media descriptor */
		/* */
		
		_codecID = codecID;
		if(!EqualAUID(&codecID, &NoCodec))
		{
			plugins = ImplAAFSession::GetInstance()->GetPluginManager();
			CHECK(plugins->GetCodecInstance(_codecID, _variety, &_codec));
			
			/* RPS-- back to your regularly scheduled program              */
			
			/* JEFF!! Changed masterSlotID to be 1 when creating mono 
			* audio media, so file mob track will be labeled correctly */
			if (EqualAUID(&mediaKind, &audioDDEF))
				masterSlotID = 1;
			
			/* JeffB: Handle the case where an existing file=>tape mob connection exists
			*/
			if(fileMob->FindSlotBySlotID(DEFAULT_FILE_SLOT, &tmpSlot) == AAFRESULT_SLOT_NOT_FOUND)
			{
				CHECK(fileMob->AddNilReference(DEFAULT_FILE_SLOT, 
						0, &mediaKind, editRate));
			}
			CHECK(masterMob->AddMasterSlot (&mediaKind, DEFAULT_FILE_SLOT, fileMob, masterSlotID, L"A Slot"));	// Should be NULL or something useful!!!
			CHECK(fileMob->FindSlotBySlotID(masterSlotID, &tmpSlot));
			CHECK(tmpSlot->SetPhysicalNum(masterSlotID));
			
			CHECK(_codec->GetMetaInfo(&metaInfo));
			mdes = (ImplAAFWAVEDescriptor *)CreateImpl(metaInfo.mdesClassID);
			ImplAAFNetworkLocator *netl = (ImplAAFNetworkLocator *)CreateImpl(CLSID_AAFNetworkLocator);
			CHECK(netl->SetPath (L"NULL"));
			CHECK(mdes->AppendLocator(netl));
			CHECK(mdes->SetLength (1));	//!!!
			CHECK(mdes->SetIsInContainer (AAFTrue));
			aafUID_t	fileType = NilMOBID;//!!!
			CHECK(mdes->SetContainerFormat (&fileType));	//!!!

			CHECK(fileMob->SetEssenceDescriptor(mdes));
			CHECK(head->AppendMob(fileMob));
			
			
			memcpy(&dataClass, &metaInfo.dataClassID, sizeof(dataClass));
			implData = (ImplAAFEssenceData *)CreateImpl (metaInfo.dataClassID);
			dataObj = static_cast<IUnknown *> (implData->GetContainer());

			CHECK(implData->SetFileMob(fileMob));
			CHECK(head->AppendEssenceData (implData));		
		}
		else
		{
			_codec = NULL;
		}
		
		
		//!!! Change to use the correct Subclass
		aafError = CoCreateInstance(CLSID_AAFEssenceDataStream,
			NULL, 
			CLSCTX_INPROC_SERVER, 
			IID_IAAFEssenceStream, 
			(void **)&_stream);
		
		IAAFEssenceDataStream	*edStream;

		//Assume not raw stream for now!!!
		aafError = (_stream->QueryInterface(IID_IAAFEssenceDataStream, (void **)&edStream));
		edStream->Init(dataObj);
		
		fileMob->MyHeadObject(&head);
		CHECK(head->SetModified());		// To NOW
		
		
		// Call the codec to create the actual media.
		//
		if(_codec != NULL)
		{
			IUnknown	*iFileMob;

			iFileMob = static_cast<IUnknown *> (fileMob->GetContainer());
			CHECK(_codec->Create(iFileMob, _variety, _stream));
			(void)(_codec->SetCompression(enable));
			//!!! 			SetVideoLineMap(16, kTopFieldNone);
		}
	}
	XEXCEPT
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
   ImplAAFEssenceAccess::MultiCreate (ImplAAFMasterMob * /*masterMob*/,
							aafUID_t codecID,
                          aafInt16  /*arrayElemCount*/,
                           aafmMultiCreate_t *  /*mediaArray*/,
                           aafCompressEnable_t  /*Enable*/)
{
#if FULL_TOOLKIT
	aafClassID_t 	mdesTag;
	aafUID_t        uid;
	aafTimeStamp_t  create_timestamp;
	aafInt16				n;
	aafMultiCreate_t *initPtr;
	aafSubChannel_t	*destPtr;
	aafCodecMetaInfo_t info;
	aafBool			found;
	aafLength_t		oneLength = CvtInt32toLength(1, oneLength);
	aafCodecID_t	CodecID;
	AAFMobSlot		 *tmpTrack;
	AAFHeader		*head;
	char 			codecIDString[OMUNIQUENAME_SIZE]; /* reasonable enough */
	char			*variety;
	
	aafAssertValidFHdl(_mainFile);
	aafAssertMediaInitComplete(_mainFile);
	if (_mainFile->isAAFMedia())
	{
	   aafAssert(fileMob != NULL, _mainFile, OM_ERR_INVALID_FILE_MOB);
	}
	XPROTECT(_mainFile)
	{
		_mainFile->GetHeadObject(&head);

		_masterTrackID = 1;
	
		/* Initialize the basic fields of the media handle
		 */
		CHECK(InitMediaHandle(fileMob));
		_masterMob = masterMob;
		_fileMob = fileMob;
		_compEnable = enable;
		_dataFile = _mainFile;
		if (_mainFile->isAAFMedia())
		  {
		     CHECK(fileMob->GetMediaDescription(&_mdes));
			  /* JeffB-- copied Roger's change from the single-channel case to use */
			  /* 			a private variable to store the codec ID.  Also copied    */
			  /* My fix to Roger's change */
			  /* */
			  if(_mdes->ReadString(OMMDESCodecID, codecIDString, 
											OMUNIQUENAME_SIZE) == OM_ERR_NONE)
				{
					variety = strchr(codecIDString, ':');
					if(variety != NULL)
					{
						*variety = '\0';
						variety++;			/* Skip over the separator */
						_codecVariety = (char *)_mainFile->omOptMalloc(strlen(variety)+1);
						strcpy(_codecVariety, variety);
					}
					CodecID = (aafCodecID_t)codecIDString;
					TableLookupBlock(_mainFile->_session->_codecID, CodecID, 
										 sizeof(_pvt->codecInfo), &_pvt->codecInfo, &found);
				}
		      else
		      {
		     		CHECK(_mdes->GetClassID(mdesTag));
		     		CHECK(TableClassIDLookup(_mainFile->_session->_codecMDES, mdesTag,
						sizeof(_pvt->codecInfo), &_pvt->codecInfo, &found));
				}
		  }
		else
		  {
		     CodecID = _mainFile->_rawCodecID;
		     	
		     TableLookupBlock(_mainFile->_session->_codecID, CodecID, 
							 sizeof(_pvt->codecInfo), &_pvt->codecInfo, &found);
		  }
		 if(!found)
			     RAISE(OM_ERR_CODEC_INVALID); 

		_numChannels = arrayElemCount;
		_channels = (aafSubChannel_t *)
			_mainFile->omOptMalloc(sizeof(aafSubChannel_t) * _numChannels);
		XASSERT((_channels != NULL), OM_ERR_NOMEMORY);
		

		for (n = 0; n < arrayElemCount; n++)
		{
			initPtr = mediaArray + n;
			destPtr = _channels + n;
         if(_mainFile->isAAFMedia())
         {
				/* JeffB: Handle the case where an existing file=>tape mob connection exists
				 */
				if(fileMob->FindTrackByTrackID(initPtr->trackID, &tmpTrack) == OM_ERR_TRACK_NOT_FOUND)
				{
				 	CHECK(fileMob->AddNilReference(
					  initPtr->trackID, oneLength, initPtr->mediaKind, editRate));
				}
				CHECK(fileMob->FindTrackByTrackID(initPtr->trackID, &tmpTrack));
				CHECK(tmpTrack->SetPhysicalNum(initPtr->trackID));
		   }
			destPtr->mediaKind = initPtr->mediaKind;
			destPtr->trackID = initPtr->trackID;
			destPtr->physicalOutChan = initPtr->subTrackNum;
			CvtInt32toPosition(0, destPtr->dataOffset);
			destPtr->dataOffset = _dataStart;
			CvtInt32toLength(0, destPtr->numSamples);
			destPtr->sampleRate = initPtr->sampleRate;
			CvtInt32toLength(0, destPtr->numSamples);
		}
	
		_openType = kAAFCreated;
	
		if(_mainFile->isAAFMedia())
		{	
			/* Initialize the fields which are derived from information in
			 * the _mainFile mob or media descriptor.
			 */
			CHECK(_mdes->WriteRational(OMMDFLSampleRate,
					_channels[0].sampleRate));

			CHECK(_codec->codecGetMetaInfo(_mainFile->_session, &_pvt->codecInfo,_codecVariety,NULL, 0,
											&info));
			_dataObj = AAFNewClassFromClassID(_mainFile,info.dataClassID, NULL);
		}
	
		if(_mainFile->isAAFMedia())
		{
			AAFGetDateTime(&create_timestamp);
			  {
				 CHECK(head->WriteTimeStamp(OMHEADLastModified, create_timestamp));
			  }
		
			/* do this now, delay may mess up data contiguity
			 */
			CHECK(fileMob->ReadUID(OMMOBJMobID, &uid));
			CHECK(_dataObj->WriteUID(OMMDATMobID, uid));
			CHECK(head->AppendObjRefArray(OMHEADMediaData, _dataObj));
		}
	
		/* Call the codec to create the actual media.
		 */
		CHECK(_codec->codecCreate(this));
  		SetVideoLineMap(16, kTopFieldNone);
	}
	XEXCEPT
	XEND
	
	return (OM_ERR_NONE);
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

	//@comm The essence handle from this call can be used with
	// WriteDataSamples or WriteMultiSamples but NOT with 
	// or WriteDataLines.
	//@comm If you are creating the essence, and then attaching it to a master
	// mob, then the "masterMob" field may be left NULL.
	//@comm Replaces omfmMediaMultiCreate
	
/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::SetBlockingSize (aafInt32  /*numBytes*/)
{
#if FULL_TOOLKIT
	aafAssertMediaHdl(this);
	aafAssertValidFHdl(_mainFile);
	aafAssertMediaInitComplete(_mainFile);
	aafAssert(numBytes >= 0, _mainFile, OM_ERR_BLOCKING_SIZE);

	_stream->SetBlockingSize(numBytes);
	return(OM_ERR_NONE);
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

	//@comm Allocating the space in this fashion ensures that the data will be
	// contiguous on disk (for at least numBytes bytes) even if other
	// disk operations allocate space on the disk.  If the data written
	// exceeds numBytes, then another disk block of numBytes size will be
	// allocated.
	//@comm Takes a essence handle, so the essence must have been opened or created.
	// The space is allocated in terms of bytes.
	//@comm Replaces omfmSetBlockingSize

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::SetVideoLineMap (aafInt16  /*startLine*/,
                           aafFieldTop_t  /*type*/)
{
#if FULL_TOOLKIT
	aafVideoMemOp_t		ops[3];
	
	aafAssertMediaHdl(this);
	aafAssertValidFHdl(_mainFile);
	aafAssertMediaInitComplete(_mainFile);
	
	XPROTECT(_mainFile)
	{
		ops[0] .opcode= kAAFFrameLayout;
		ops[1] .opcode= kAAFVFmtEnd;

		CHECK(GetVideoInfoArray(ops));
		
		if (ops[0].operand.expFrameLayout == kFullFrame)
			RAISE(OM_ERR_INVALID_OBJ);

		/* even field dominance makes no sense for single field video */
		if (ops[0].operand.expFrameLayout == kOneField && type == kDominantField2)
			RAISE(OM_ERR_INVALID_OBJ);
			
		ops[0] .opcode= kAAFVideoLineMap;
		ops[1] .opcode= kAAFVFmtEnd;
		
		if (startLine == 0)  /* for GRAPHICS files, no analog mapping */
		{
			switch(type)
			{
			case kDominantField1:
				ops[0].operand.expLineMap[0] = 0;
				ops[0].operand.expLineMap[1] = 1;
				break;
				
			case kDominantField2:
				ops[0].operand.expLineMap[0] = 1;
				ops[0].operand.expLineMap[1] = 0;
				break;
				
			case kNoDominant:
			default:
				ops[0].operand.expLineMap[0] = 0;
				ops[0].operand.expLineMap[1] = 0;
				break;
			}
		}
		else  /* Video mapping, need to know format */
		{
			aafInt16 offset;
			aafVideoSignalType_t signalType;
			
			CHECK(SourceGetVideoSignalType(&signalType));

			switch (signalType)
			{
			case kNTSCSignal:
				offset = 263;
				break;
				
			case kPALSignal:
				offset = 312;
				break;
			
			default:
			   RAISE(OM_ERR_INVALID_VIDEOSIGNALTYPE);
				break;
			}
			
			switch(type)
			{
			case kDominantField1:
				ops[0].operand.expLineMap[0] = (aafInt32) startLine;
				ops[0].operand.expLineMap[1] = (aafInt32) (startLine+offset+1);
				break;
				
			case kDominantField2:
				ops[0].operand.expLineMap[0] = (aafInt32) startLine;
				ops[0].operand.expLineMap[1] = (aafInt32) (startLine+offset);
				break;
				
			case kNoDominant:
			default:
			   RAISE(OM_ERR_INVALID_VIDEOSIGNALTYPE);
				break;
			}
			
		
		}
	
		CHECK(_codec->codecPutVideoInfo(this, ops));
	}
	XEXCEPT
	XEND
	
	return(OM_ERR_NONE);
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

	//@comm Takes a essence handle, so the essence must have been opened or created.
	//@comm Replaces omfmSetVideoLineMap.

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::GetVideoTopField (aafFieldTop_t *  /*type*/)
{
#if FULL_TOOLKIT
	aafVideoMemOp_t		ops[3];

	aafAssertMediaHdl(this);
	aafAssertValidFHdl(_mainFile);
	aafAssertMediaInitComplete(_mainFile);
	
	XPROTECT(_mainFile)
	{
		ops[0].opcode = kAAFVideoLineMap;
		ops[1].opcode = kAAFFrameLayout;
		ops[2].opcode = kAAFVFmtEnd;
		CHECK(_codec->codecGetVideoInfo(this, ops));

		if (ops[1].operand.expFrameLayout != kSeparateFields &&
			ops[1].operand.expFrameLayout != kMixedFields)
		{
			*type = kTopFieldNone;  /* dominance only relevant for multi-field */
			return(OM_ERR_NONE);
		}

		if(ops[0].operand.expLineMap[0] < ops[0].operand.expLineMap[1])
			*type = kTopField1;  /* dominance only relevant for multi-field */
		else
			*type = kTopField2;  /* dominance only relevant for multi-field */

	}
	XEXCEPT
	XEND
	
	return(OM_ERR_NONE);
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

	//@comm Takes a essence handle, so the essence must have been opened or created.
	//@comm Replaces omfmGetVideoTopField
	
/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::SetDisplayRect (aafRect_t *  /*DisplayRect*/)
{
#if FULL_TOOLKIT
	AAFFile *				main;
	aafVideoMemOp_t	ops[2];
	
	aafAssertMediaHdl(this);
	main = _mainFile;
	aafAssertValidFHdl(main);
	aafAssertMediaInitComplete(main);

	XPROTECT(main)
	{
		ops[0].opcode = kAAFDisplayRect;
		ops[0].operand.expRect = DisplayRect;
		ops[1].opcode = kAAFVFmtEnd;

		CHECK(_codec->codecPutVideoInfo(this, ops));
	}
	XEXCEPT
	XEND

	return (OM_ERR_NONE);
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

	//@comm Handles the case where the displayed rectangle is not the same as
	// the stored rectangle (as with the old leadingLines and trailingLines).
	// A positive "leadingLines" (from 1.5) becomes a positive yOffset, and
	// decreases the display height.
	// A positive "trailingLines" (from 1.5) also decreases the display height.
	//@comm Takes a essence handle, so the essence must have been opened or created.
	//@comm Replaces  omfmSetDisplayRect

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::SetSampledRect (aafRect_t *  /*SampledRect*/)
{
#if FULL_TOOLKIT
	AAFFile *				main;
	aafVideoMemOp_t	ops[2];

	aafAssertMediaHdl(this);
	main = _mainFile;
	aafAssertValidFHdl(main);
	aafAssertMediaInitComplete(main);

	XPROTECT(main)
	{
		ops[0].opcode = kAAFSampledRect;
		ops[0].operand.expRect = SampledRect;
		ops[1].opcode = kAAFVFmtEnd;

		CHECK(_codec->codecPutVideoInfo(this, ops));
	}
	XEXCEPT
	XEND
	
	return (OM_ERR_NONE);
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

	//@comm Takes a essence handle, so the essence must have been opened or created
	//@comm Replaces omfmSetSampledRect
	
/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::WriteMultiSamples (aafInt16  /*arrayElemCount*/,
                           aafmMultiXfer_t *  /*xferArray*/)
{
#if FULL_TOOLKIT
	aafMultiXfer_t xfer;

	aafAssertMediaHdl(this);
	aafAssertValidFHdl(_mainFile);
	aafAssertMediaInitComplete(_mainFile);
	aafAssert(_numChannels == 1, _mainFile,
		OM_ERR_SINGLE_CHANNEL_OP);
	aafAssert(buffer != NULL, _mainFile, OM_ERR_BADDATAADDRESS);
	aafAssert((_openType == kAAFCreated) ||
				(_openType == kAAFAppended), _mainFile, OM_ERR_MEDIA_OPENMODE);
	
	XPROTECT(_mainFile)
	{
		xfer.subTrackNum = _channels[0].physicalOutChan;
		xfer.numSamples = nSamples;
		xfer.buflen = buflen;
		xfer.buffer = buffer;
		xfer.bytesXfered = 0;
	
		CHECK (_codec->codecWriteBlocks(this, deinterleave, 1, &xfer));
	}
	XEXCEPT
	XEND
	
	return(OM_ERR_NONE);
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

	//@comm arrayElemCount is the size of the array or transfer operations.
	// xferArray points to an array of transfer parameters.  All fields
	// in this array except for bytesXferred must be set up before
	// doing the transfer.
	//@comm Replaces omfmWriteMultiSamples

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::WriteDataSamples (aafInt32  /*nSamples*/,
                           aafDataBuffer_t  /*buffer*/,
                           aafInt32  /*buflen*/)
{
#if FULL_TOOLKIT
	aafMultiXfer_t xfer;

	aafAssertMediaHdl(this);
	aafAssertValidFHdl(_mainFile);
	aafAssertMediaInitComplete(_mainFile);
	aafAssert(_numChannels == 1, _mainFile,
		OM_ERR_SINGLE_CHANNEL_OP);
	aafAssert(buffer != NULL, _mainFile, OM_ERR_BADDATAADDRESS);
	aafAssert((_openType == kAAFCreated) ||
				(_openType == kAAFAppended), _mainFile, OM_ERR_MEDIA_OPENMODE);
	
	XPROTECT(_mainFile)
	{
		xfer.subTrackNum = _channels[0].physicalOutChan;
		xfer.numSamples = nSamples;
		xfer.buflen = buflen;
		xfer.buffer = buffer;
		xfer.bytesXfered = 0;
	
		CHECK (_codec->codecWriteBlocks(this, deinterleave, 1, &xfer));
	}
	XEXCEPT
	XEND
	
	return(OM_ERR_NONE);
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

	//@comm Takes a essence handle, so the essence must have been opened or created.
	// A single video frame is ONE sample.
	// Buflen must be large enough to hold nSamples * the maximum sample size.
	//@comm Possible Errors:
	// Standard errors (see top of file).
	// OM_ERR_SINGLE_CHANNEL_OP -- Tried to write to an interleaved stream.
	// OM_ERR_BADDATAADDRESS -- The buffer must not be a NULL pointer.
	//@comm Replaces omfmWriteDataSamples

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::WriteRawData (aafInt32  nSamples,
                           aafDataBuffer_t  buffer,
                           aafInt32  sampleSize)
{
	aafAssert(buffer != NULL, _mainFile, AAFRESULT_BADDATAADDRESS);
	aafAssert((_openType == kAAFCreated) ||
				(_openType == kAAFAppended), _mainFile,AAFRESULT_MEDIA_OPENMODE);

	XPROTECT()
	{
		CHECK(_codec->WriteRawData (nSamples, buffer, sampleSize));
	}
	XEXCEPT
	XEND
	
	return(AAFRESULT_SUCCESS);
}

	//@comm A single video frame is ONE sample.
	//@comm Buflen must be large enough to hold
	// nSamples * the maximum sample size.
	//@comm Possible Errors:
	// Standard errors (see top of file).
	// OM_ERR_BADDATAADDRESS -- The buffer must not be a NULL pointer.
	//@comm Replaces omfmWriteRawData
	
/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::ReadRawData (aafInt32  nSamples,
                           aafUInt32  buflen,
                           aafDataBuffer_t  buffer,
                           aafUInt32 *bytesRead,
                           aafUInt32 *samplesRead)
{

	aafAssert(buffer != NULL, _mainFile, OM_ERR_BADDATAADDRESS);
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
	// OM_ERR_BADDATAADDRESS -- The buffer must not be a NULL pointer.
	//@comm Replaces omfmReadRawData
	
/****/
 AAFRESULT STDMETHODCALLTYPE
   ImplAAFEssenceAccess::WriteDataLines (aafUInt32  /*nLines*/,
                           aafUInt32  /*nBytesPerLine*/,
                           aafDataBuffer_t  /*buffer*/,
                           aafInt32 *  /*bytesWritten*/)
{
#if FULL_TOOLKIT
	aafInt16		numVideo;
	
	aafAssertMediaHdl(this);
	aafAssertValidFHdl(_mainFile);
	aafAssertMediaInitComplete(_mainFile);
	aafAssert((_openType == kAAFCreated) ||
				(_openType == kAAFAppended), _mainFile, OM_ERR_MEDIA_OPENMODE);
	
	XPROTECT(_mainFile)
	{
		CHECK(_codec->codecGetNumChannels(_mainFile, _fileMob,
											_pictureKind, &numVideo));
		if(numVideo <= 0)
			RAISE(OM_ERR_BADRWLINES);
		CHECK (_codec->codecWriteLines(this, nLines, buffer, bytesWritten));
	}
	XEXCEPT
	XEND
	
	return(OM_ERR_NONE);
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

	//@comm Writes single lines of video to a file.  This function allows writing
	// video frames in pieces, for low-memory situations.  When enough lines
	// have been written to constitute a frame, then the number of samples will
	// be incremented by one.
	//@comm This function works only for video essence.
	//@comm The buffer must be large enough to hold an entire line of video. 
	//@comm Possible Errors:
	// Standard errors (see top of file).
	// OM_ERR_BADRWLINES -- This function only works for video essence.
	//@comm Replaces omfmWriteDataLines
	//@devnote Previous version did not have nBytesPerLine, otherwise the actual size
	// of the buffer cannot be known.
	
/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::Close ()
{
#if 0
	AAFFile *       	main;
	AAFMedia  *tstMedia;
	aafInt16       	n;
	aafTrackID_t	trackID;
	aafTrackID_t   	fileTrackID;
	aafErr_t		aafError = OM_ERR_NONE;
	
	aafAssertMediaHdl(this);
	aafAssertValidFHdl(_mainFile);
	aafAssertMediaInitComplete(_mainFile);
	main = _mainFile;
#endif

	XPROTECT()
	{
		/* Close the _codec-> before creating more objects, in order to keep trailer data
		 * with the media in the file.
		 */
		if(_codec != NULL)		/* A codec was opened */
			CHECK(_codec->Close());

		if((_openType == kAAFCreated) || (_openType == kAAFAppended))
		{
#if 0
			CHECK(_fileMob->FindSlotBySlotID(1, &tmpSlot));
			CHECK(tmpSlot->GetSegment(&seg));
			CHECK(seg->SetLength(&len));
			CHECK(_masterMob->FindSlotBySlotID(1, &tmpSlot));
			CHECK(tmpSlot->GetSegment(&seg));
			CHECK(seg->SetLength(&len));
#else
			// Make the lengths of the affected tracks equal the sample length
			// adjusted for the edit rate
			CHECK(_fileMob->ReconcileMobLength());
			CHECK(_masterMob->ReconcileMobLength());
#endif
		}

#if FULL_TOOLKIT
		if((_openType == kAAFCreated) || (_openType == kAAFAppended))
		{
			if(main->isAAFMedia())
			{
				if(_numChannels == 1)
					trackID = _masterTrackID;
				else if(_masterMob != NULL)
				{
					aafNumTracks_t		numMasterTracks;
					CHECK(_masterMob->GetNumTracks(&numMasterTracks));
					trackID = numMasterTracks + 1;
				}
				else
				  trackID = 1;
				for(n = 0; n < _numChannels; n++)
				{
					CHECK(_mdes->WriteLength(OMMDFLLength, _channels[n].numSamples));
					
					if((_openType == kAAFCreated) && (_masterMob != NULL))
					{
					  /* JEFF!! Changed fileTrackID to be 1 when creating audio.
						* This should probably also be 1 for video?
						*/
					  if (_channels[n].mediaKind->IsSoundKind(kExactMatch, &aafError))
						 {
							fileTrackID = 1+n;
						 }
					  else
						 fileTrackID = trackID+n;

						CHECK(_masterMob->MobAddMasterTrack(
												 _channels[n].mediaKind, trackID+n, 
/*															 trackID+n, */
															 fileTrackID,
													 NULL, _fileMob));
					}
				}
				CHECK(ReconcileMobLength(_fileMob));
				if(_masterMob != NULL)
				{
					CHECK(ReconcileMobLength(_masterMob));
				}
			}
		}
	
		/* Unlink this media from the list maintained by the AAFFile *
		 */
		if (this == main->_topMedia)
			main->_topMedia = _pvt->nextMedia;
		else
		{
			tstMedia = main->_topMedia;
			while (tstMedia->_pvt->nextMedia != NULL)
			{
				if (tstMedia->_pvt->nextMedia == this)
					tstMedia->_pvt->nextMedia = _pvt->nextMedia;
				else
					tstMedia = tstMedia->_pvt->nextMedia;
			}
		}
	
		/* If the data is kept in an external file, then close that file.
		 * If the data is in the current file, leave it open.
		 */
		if ((_dataFile != main) && (_dataFile != NULL))
			CHECK(_dataFile->Close());

		if(_channels != NULL)
		{
			main->omOptFree(_channels);
			_channels = NULL;
		}
		
		if(_stream != NULL)
			delete _stream;
		_stream = NULL;
		if(_pvt != NULL)
			main->omOptFree(_pvt);
		_pvt = NULL;
		
		if(_codecVariety != NULL)
			main->omOptFree(_codecVariety);
		_codecVariety = NULL;
		
		/* Clear the cookie so that this handle will show up as invalid
		 * if the client continues to use it.
		 */
		_cookie = 0;
#endif
	}
	XEXCEPT
	{
	}
	XEND
	
	return(AAFRESULT_SUCCESS);
}

	//@comm This function should be called whether the essence was opened or created.
	//@comm Replaces omfmMediaClose

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::SetJPEGTables (aafJPEGcomponent_t  /*JPEGcomp*/,
                           aafUInt8 *  /*QTables*/,
                           aafUInt8 *  /*ACTables*/,
                           aafUInt8 *  /*DCTables*/,
                           aafInt16  /*QTableSize*/,
                           aafInt16  /*ACTableSize*/,
                           aafInt16  /*DCTableSize*/)
{
#if FULL_TOOLKIT
	aafJPEGTables_t tables;

	aafAssertMediaHdl(this);
	aafAssertValidFHdl(_mainFile);
	aafAssertMediaInitComplete(_mainFile);

	tables.JPEGcomp = JPEGcomp;
	tables.QTables = QTables;
	tables.ACTables = ACTables;
	tables.DCTables = DCTables;
	tables.QTableSize = QTableSize;
	tables.ACTableSize = ACTableSize;
	tables.DCTableSize = DCTableSize;

	return (_codec->codecPutInfo(this, kJPEGTables, _pictureKind,
								sizeof(tables), &tables));
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

	//@comm This function needs to be called once for each component of the video.
	//@comm Replaces omfmSetJPEGTables
	
/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::SetAudioBlockLength (aafInt16  /*blockLength*/)
{
#if FULL_TOOLKIT
	aafAudioCompressParms_t cparms;

	aafAssertMediaHdl(this);
	aafAssertValidFHdl(_mainFile);
	aafAssertMediaInitComplete(_mainFile);

	cparms.blockLength = blockLength;
	return (_codec->codecPutInfo(this, kAudioCompressParms, _soundKind, sizeof(cparms), &cparms));
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

	//@comm Replaces omfmSetAudioCompressParms
/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::GetAudioBlockLength (aafInt16*  /*blockLength*/)
{
#if FULL_TOOLKIT
	aafAudioCompressParms_t cparms;
	aafErr_t				status;
	
	aafAssertMediaHdl(this);
	aafAssertValidFHdl(_mainFile);
	aafAssertMediaInitComplete(_mainFile);
	aafAssert(blockLength != NULL, _mainFile, OM_ERR_NULL_PARAM);

	status = _codec->codecGetInfo(this, kAudioCompressParms, _soundKind,
								sizeof(cparms), &cparms);
	*blockLength = cparms.blockLength;
	return(status);
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

	//@comm Replaces omfmGetAudioCompressParms
	
/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::SetPrivateMediaData (aafUInt32  /*parmBlockSize*/,
                           aafDataBuffer_t  /*ParameterBlock*/)
{
#if FULL_TOOLKIT
	aafAssertMediaHdl(this);
	aafAssertValidFHdl(_mainFile);
	aafAssertMediaInitComplete(_mainFile);

	return (_codec->codecPutInfo(this, kCompressionParms, _nilKind,
								parmBlockSize, ParameterBlock));
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

	//@parm IN -- .
	//@comm The parameter block should be defined in the
	// "h" file of the codec, and must be included by the application in
	// order to use this call.
	//@comm NOTE: All CODECs should default to reasonable parameters,
	// in case the application doesn't know about a given codec.
	//@comm Replaces omfmCodecSendPrivateData*/

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::GetPrivateMediaData (aafUInt32  /*blocksize*/,
                           aafDataBuffer_t  /*buffer*/)
{
#if FULL_TOOLKIT
	aafAssertMediaHdl(this);
	aafAssertValidFHdl(_mainFile);
	aafAssertMediaInitComplete(_mainFile);
	aafAssert(result != NULL, _mainFile, OM_ERR_NULL_PARAM);

	XPROTECT(_mainFile)
	{
		CHECK(_codec->codecGetInfo(this, kCompressionParms, _nilKind, blocksize,
								result));
	}
	XEXCEPT
	XEND
	
	return (OM_ERR_NONE);
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

	//@comm The parameter block should be defined in the
	// "h" file of the codec, and must be included by the application in
	// order to use this call.
	//@comm NOTE: All CODECs should default to reasonable parameters,
	// in case the application doesn't know about a given codec.
	//@comm Replaces omfmGetPrivateMediaData*/
	//@devnote Added bytesRead 

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::GetNumChannels (ImplAAFMasterMob * /*masterMob*/,
                           aafSlotID_t  /*slotID*/,
                           aafMediaCriteria_t*  /*mediaCrit*/,
                           ImplAAFDataDef * /*mediaKind*/,
                           aafInt16*  /*numCh*/)
{
#if FULL_TOOLKIT
	aafPosition_t		zeroPos;
	aafFindSourceInfo_t	sourceInfo;
	AAFFileMob *			fileMob;
	
	aafAssertValidFHdl(_mainFile);
	aafAssertMediaInitComplete(_mainFile);

	aafAssert(numCh != NULL, _mainFile, OM_ERR_NULL_PARAM);
	XPROTECT(_mainFile)
	{
		CvtInt32toPosition(0, zeroPos);	
//!!!		if(IsMobKind(kFileMob))
//!!!			fileMob = (AAFFileMob *)masterMob;	//!!CASTING
//!!!		else
		{
			CHECK(masterMob->SearchSource(trackID, zeroPos,kFileMob,
									mediaCrit, NULL, NULL, &sourceInfo));
			fileMob = (AAFFileMob *)sourceInfo.mob;
		}

		CHECK(_codec->codecGetNumChannels(_mainFile, fileMob, mediaKind, numCh));
	}
	XEXCEPT
	XEND
	
	return(OM_ERR_NONE);
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

	//@comm Returns the number of interleaved essence channels of a given type in the essence stream referenced by the given file mob
	//@comm If the data format is not interleaved, then the answer will
	// always be zero or one.  This function correctly returns zero
	// for essence types not handled by a given codec, and handles codecs
	// which work with multiple essence types.
	//@comm Replaces omfmGetNumChannels*/

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::GetLargestSampleSize (ImplAAFDataDef * /*mediaKind*/,
                           aafInt32*  /*maxSize*/)
{
#if FULL_TOOLKIT
	aafMaxSampleSize_t	parms;
	AAFFile *			main;
	
	aafAssertMediaHdl(this);
	main = _mainFile;
	aafAssertValidFHdl(main);
	aafAssertMediaInitComplete(main);

	aafAssert(maxSize != NULL, main, OM_ERR_NULL_PARAM);
	*maxSize = 0;
	XPROTECT(main)
	{
		parms.mediaKind = mediaKind;
		CHECK(_codec->codecGetInfo(this, kMaxSampleSize, mediaKind, sizeof(parms),
								&parms));
		*maxSize = parms.largestSampleSize;
	}
	XEXCEPT
	XEND
	
	return(OM_ERR_NONE);
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

	//@comm For uncompressed data, or the output of the software codec,
	// the sample size will propably be a constant.
	//@comm The essence type parameter exists to support codecs with multiple
	// interleaved essence types.
	//@comm Replaces omfmGetLargestSampleSize*/

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::GetSampleFrameSize (ImplAAFDataDef * /*mediaKind*/,
                           aafPosition_t  /*frameNum*/,
                           aafLength_t*  /*frameSize*/)
{
#if FULL_TOOLKIT
	aafFrameSizeParms_t	parms;
	AAFFile *			main;
	
	aafAssertMediaHdl(this);
	main = _mainFile;
	aafAssertValidFHdl(main);
	aafAssertMediaInitComplete(main);

	aafAssert(frameSize != NULL, main, OM_ERR_NULL_PARAM);
	CvtInt32toInt64(0, frameSize);
	XPROTECT(main)
	{
		parms.mediaKind = mediaKind;
		parms.frameNum = frameNum;
		CHECK(_codec->codecGetInfo(this, kSampleSize, mediaKind, sizeof(parms),
								&parms));
		*frameSize = parms.frameSize;
	}
	XEXCEPT
	XEND
	
	return(OM_ERR_NONE);
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

	//@comm For uncompressed data, or the output of the software codec,
	// the sample size will propably be a constant.
	//@comm The essence type parameter exists to support codecs with multiple
	// interleaved essence types.
	//@comm Possible Errors:
	// 	Standard errors (see top of file).
	// 	OM_ERR_NULL_PARAM -- A return parameter was NULL.
	//@comm Replaces omfmGetSampleFrameSize*/

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::Open (ImplAAFMasterMob *masterMob,
                           aafSlotID_t  slotID,
                           aafMediaCriteria_t*mediaCrit,
                           aafMediaOpenMode_t  openMode,
                           aafCompressEnable_t  compEnable)
{
	aafPosition_t	zeroPos;
//	aafSlotID_t		tmpSlotID;
//!!!	AAFIterate		*mobIter = NULL;
//!!!	aafInt32 		numSlots, loop;
	aafInt32		n;
//!!!	aafBool			foundTrack;
	ImplAAFMobSlot		*slot = NULL;
	ImplAAFSegment		*seg = NULL;
	aafUID_t		 mediaKind, fileMobID;
	aafLength_t masterMobLength, one;
	ImplAAFFindSourceInfo *sourceInfo;
	ImplAAFSourceMob		*fileMob;
	ImplAAFHeader			*dataHead;
	aafSourceRef_t	fileRef;
	aafInt16		numCh;
	AAFRESULT		aafError;
	ImplAAFEssenceData		*essenceData = NULL;
	AAFPluginManager	*plugins;

	XPROTECT()
	{
		_openType = kAAFReadOnly;
		CvtInt32toPosition(0, zeroPos);	
		CvtInt32toLength(1, one);
		CHECK(masterMob->SearchSource(slotID, zeroPos,kFileMob,
									   mediaCrit,
									   NULL,
									   &sourceInfo));
									   
		CHECK(masterMob->FindSlotBySlotID (slotID,&slot));
		CHECK(slot->GetSegment(&seg));
		CHECK(seg->GetDataDef(&mediaKind));
		CHECK(seg->GetLength(&masterMobLength));
		/* !!! NOTE: Assumes that file trackID's are 1-N */
//!!!		CHECK(OpenFromFileMob((ImplAAFSourceMob *)sourceInfo.mob, sourceInfo.mobTrackID, openMode, mediaKind, 
//												  (aafInt16)sourceInfo.mobTrackID, compEnable));

		
		CHECK(sourceInfo->GetMob((ImplAAFMob **)&fileMob));
		CHECK(sourceInfo->GetReference(&fileRef));

//		CHECK(InitMediaHandle(fileMob));
//!!!		_compEnable = compEnable;
//!!!		if(openMode == kMediaOpenAppend)
//			_openType = kAAFAppended;
//		else
//			_openType = kAAFOpened;
	
		CHECK(fileMob->GetEssenceDescriptor((ImplAAFEssenceDescriptor **)&_mdes));
		CHECK(fileMob->GetMobID(&fileMobID));

		plugins = ImplAAFSession::GetInstance()->GetPluginManager();
		CHECK(plugins->GetCodecInstance(CLSID_AAFWaveCodec, _variety, &_codec));

		IUnknown	*iFileMob;

		iFileMob = static_cast<IUnknown *> (fileMob->GetContainer());
		CHECK(_codec->GetNumChannels(iFileMob, mediaKind, &numCh));
		if (numCh == 0)
		  RAISE(OM_ERR_INVALID_DATAKIND);

		_channels = (aafSubChannel_t *) new aafSubChannel_t[1];
		if(_channels == NULL)
			RAISE(OM_ERR_NOMEMORY);
		_numChannels = numCh;
		for(n = 0; n < numCh; n++)
		{
			CvtInt32toLength(0, _channels[n].numSamples);
			_channels[n].dataOffset = _dataStart;
			_channels[n].mediaKind = mediaKind;
			_channels[n].physicalOutChan = n+1;
			_channels[n].trackID = slotID+n;
		}
//!!!		_physicalOutChanOpen = physicalOutChan;
		
//!!!		CHECK(fileMob->LocateMediaFile(&_dataFile, &isAAF));
//!!!		if(_dataFile == NULL)
//			RAISE(OM_ERR_MEDIA_NOT_FOUND);
		
//!!!		_rawFile = _dataFile->_rawFile;			/* If non-omfi file */
//!!!		CHECK(_mdes->FindCodecForMedia(&_pvt->codecInfo));
//!!!		if(isAAF)
		{
			ImplEnumAAFEssenceData	*myEnum;
			ImplAAFEssenceData		*testData;
			aafBool					found = AAFFalse;
			aafUID_t				testMobID;

			CHECK(fileMob->GetMobID(&fileMobID));
			CHECK(fileMob->MyHeadObject(&dataHead));
			CHECK(dataHead->EnumEssenceData (&myEnum));
			while(myEnum->NextOne (&testData) == AAFRESULT_SUCCESS)
			{
				CHECK(testData->GetFileMobID (&testMobID));
				if(EqualAUID(&fileMobID, &testMobID))
				{
					found = AAFTrue;
					essenceData = testData;
					break;
				}
			}
			if(!found)
				RAISE(AAFRESULT_MEDIA_NOT_FOUND);		//!!!No external media yet
//			CHECK(dataHead->LookupDataObject(fileMobID, &_dataObj));
		}
//		else
//			RAISE(AAFRESULT_MEDIA_NOT_FOUND);		//!!!No external media yet

		aafError = CoCreateInstance(CLSID_AAFEssenceDataStream,
			NULL, 
			CLSCTX_INPROC_SERVER, 
			IID_IAAFEssenceStream, 
			(void **)&_stream);
		
		IAAFEssenceDataStream	*edStream;
		IUnknown				*edUnknown;

		//Assume not raw stream for now!!!
		aafError = (_stream->QueryInterface(IID_IAAFEssenceDataStream, (void **)&edStream));

        //!!! This only works with a COM API
		edUnknown = static_cast<IUnknown *> (essenceData->GetContainer());

//		aafError = (essenceData->QueryInterface(IID_IUnknown, (void **)&edUnknown));
		edStream->Init(edUnknown);

		iFileMob = static_cast<IUnknown *> (fileMob->GetContainer());
		CHECK(_codec->Open(iFileMob, openMode, _stream));
//!!!		if(openMode == kMediaOpenAppend)
//		{
//			CHECK(GetSampleCount(&numSamples));										  
//			CHECK(AddInt32toInt64(1, &numSamples));
//			CHECK(GotoFrameNumber(numSamples));
//		}
//!!!		if (mobIter)
//		  delete mobIter;
		
		_masterMob = masterMob;
	}
	XEXCEPT
	{
	}
	XEND
	
	return (OM_ERR_NONE);
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
	// 	OM_ERR_NOMEMORY -- couldn't allocate memory for the essence handle
	//@comm NOTE: If a locator is followed, then essencePtr may reference ANOTHER file
	// object, which must be closed on file close.
	//@comm Replaces omfmMediaOpen*/
	
/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::MultiOpen (ImplAAFMasterMob * /*masterMob*/,
                           aafSlotID_t  /*slotID*/,
                           aafMediaCriteria_t*  /*mediaCrit*/,
                           aafMediaOpenMode_t  /*openMode*/,
                           aafCompressEnable_t  /*compEnable*/)
{
#if FULL_TOOLKIT
	aafPosition_t	zeroPos;
	aafFindSourceInfo_t	sourceInfo;
	
	XPROTECT(_mainFile)
	{
		
		CvtInt32toPosition(0, zeroPos);	
		CHECK(masterMob->SearchSource(trackID, zeroPos,kFileMob,
									mediaCrit, NULL, NULL, &sourceInfo));

		CHECK(MultiOpenFromFileMob((AAFFileMob *)sourceInfo.mob, sourceInfo.mobTrackID, openMode, compEnable));
aafErr_t AAFMedia::MultiOpenFromFileMob(
			AAFFileMob *				fileMob,	/* IN -- In this master mob */
			aafTrackID_t			trackID,
			aafMediaOpenMode_t	openMode,	/* IN -- ReadOnly or Append */
			aafCompressEnable_t	compEnable)	/* IN -- optionally decompressing */
{
	aafInt16           numVideo, numAudio, numOther = 0, n;
	aafUID_t			fileMobID;
	aafBool			isAAF;
	aafPosition_t	numSamples;
	AAFHeader		*dataHead;
	
	aafAssertValidFHdl(_mainFile);
	aafAssertMediaInitComplete(_mainFile);
	
	aafAssert(fileMob != NULL, _mainFile, OM_ERR_INVALID_FILE_MOB);

	XPROTECT(_mainFile)
	{
		CHECK(InitMediaHandle(fileMob));
		_compEnable = compEnable;
		if(openMode == kMediaOpenAppend)
			_openType = kAAFAppended;
		else
			_openType = kAAFOpened;
	
		CHECK(fileMob->GetMediaDescription(&_mdes));
		CHECK(_codec->codecGetNumChannels(_mainFile, fileMob, _pictureKind,
											&numVideo));
		CHECK(_codec->codecGetNumChannels(_mainFile, fileMob, _soundKind,
											&numAudio));

		_numChannels = numVideo + numAudio + numOther;
		_channels = (aafSubChannel_t *)
			_mainFile->omOptMalloc(sizeof(aafSubChannel_t) * _numChannels);
		XASSERT((_channels != NULL), OM_ERR_NOMEMORY);

		CHECK(_fileMob->LocateMediaFile(&_dataFile, &isAAF));
		if(_dataFile == NULL)
			RAISE(OM_ERR_MEDIA_NOT_FOUND);

		_rawFile = _dataFile->_rawFile;			/* If non-omfi file */
		CHECK(_mdes->FindCodecForMedia(&_pvt->codecInfo));
		if(isAAF)
		{
			CHECK(fileMob->ReadUID(OMMOBJMobID, &fileMobID));
			/* MCX/NT Hack! */
			CHECK(_dataFile->GetHeadObject(&dataHead));
			CHECK(dataHead->LookupDataObject(fileMobID, &_dataObj));
		}
			
		for (n = 0; n < _numChannels; n++)
		{
			if (n < numVideo)
			{
				_channels[n].mediaKind = _pictureKind;
				_channels[n].physicalOutChan = n + 1;
				_channels[n].trackID = trackID + n;
			} else if (n < numVideo + numAudio)
			{
				_channels[n].mediaKind = _soundKind;
				_channels[n].physicalOutChan = n + 1 - (numVideo);
				_channels[n].trackID = trackID + n;
			}

			CvtInt32toPosition(0, _channels[0].dataOffset);
			_channels[n].dataOffset = _dataStart;
		}
		CHECK(_codec->codecOpen(this));
		if(openMode == kMediaOpenAppend)
		{
			CHECK(GetSampleCount(&numSamples));										  
			CHECK(AddInt32toInt64(1, &numSamples));
			CHECK(GotoFrameNumber(numSamples));
		}
	}
	XEXCEPT
	XEND
	
	return (OM_ERR_NONE);
}
	}
	XEXCEPT
	XEND
	
	return (OM_ERR_NONE);
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
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
	// 	OM_ERR_NOMEMORY -- couldn't allocate memory for the essence handle
	//@comm Replaces omfmMediaMultiOpen*/

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::SetVideoMemFormat (ImplAAFEssenceFormat * /*op*/)
{
#if FULL_TOOLKIT
	aafAssertMediaHdl(this);
	aafAssertValidFHdl(_mainFile);
	aafAssertMediaInitComplete(_mainFile);

	return (_codec->codecPutInfo(this, kVideoMemFormat, _pictureKind,
								sizeof(aafVideoMemOp_t), op));
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

	//@comm This is the format expected on writes and produced on reads.
	//@comm On writes, the data will be written in this format, except
	// where a software codec may be used.
	// On reads, the data will be translated to this format.
	//@comm The current CODECs should support rgb888 and YUV as formats
	// and all of the standard layouts.  A special format of
	// kVmFmtStd says to use the file's native format & layout.
	//@comm Replaces omfmSetVideoMemFormat*/

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::SetAudioMemFormat (ImplAAFEssenceFormat * /*op*/)
{
#if FULL_TOOLKIT
	aafAssertMediaHdl(this);
	aafAssertValidFHdl(_mainFile);
	aafAssertMediaInitComplete(_mainFile);

	return (_codec->codecPutInfo(this, kSetAudioMemFormat, _soundKind,
								sizeof(op), op));
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

	//@comm This is the format expected on writes and produced on reads.
	//@comm On writes, the data will be written in this format, except
	// where a software codec may be used.
	// On reads, the data will be translated to this format.
	//@comm The current CODECs should support different sample sizes and rates
	// A special format of kAmFmtStd says to use the file's native
	// size and rate.
 	//@comm Replaces omfmSetAudioMemFormat

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::GetDisplayRect (aafRect_t*  /*result*/)
{
#if FULL_TOOLKIT
	aafVideoMemOp_t	ops[2];

	aafAssertMediaHdl(this);
	aafAssertValidFHdl(_mainFile);
	aafAssertMediaInitComplete(_mainFile);
	aafAssert(result != NULL, _mainFile, OM_ERR_NULL_PARAM);
	
	XPROTECT(_mainFile)
	{
		ops[0].opcode = kAAFDisplayRect;
		ops[1].opcode = kAAFVFmtEnd;

		CHECK(_codec->codecGetVideoInfo(this, ops));
		*result = ops[0].operand.expRect;
	}
	XEXCEPT
	XEND
	
	return (OM_ERR_NONE);
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

	//@comm Handles the case where the displayed rectangle is not the same as
	// the stored rectangle (as with the old leadingLines and
	// trailingLines).<nl>
	// A positive "leadingLines" (from 1.5) becomes a positive yOffset, and
	// decreases the display height.<nl>
	// A positive "trailingLines" (from 1.5) also decreases the display
	// height.
	//@comm Takes a essence handle, so the essence must have been opened or
	// created.
	//@comm Possible Errors:
	// Standard errors (see top of file).
	//   OM_ERR_NULL_PARAM -- A NULL rectangle pointer.
	//   OM_ERR_INVALID_OP_CODEC -- This codec doesn't support display rect
	//								(may not be video essence)
 	//@comm Replaces omfmGetDisplayRect

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::GetSampledRect (aafRect_t*  /*result*/)
{
#if FULL_TOOLKIT
	aafVideoMemOp_t	ops[2];

	aafAssertMediaHdl(this);
	aafAssertValidFHdl(_mainFile);
	aafAssertMediaInitComplete(_mainFile);
	aafAssert(result != NULL, _mainFile, OM_ERR_NULL_PARAM);
	
	XPROTECT(_mainFile)
	{
		ops[0].opcode = kAAFSampledRect;
		ops[1].opcode = kAAFVFmtEnd;

		CHECK(_codec->codecGetVideoInfo(this, ops));
		*result = ops[0].operand.expRect;
	}
	XEXCEPT
	XEND
	
	return (OM_ERR_NONE);
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

	//@comm Takes a essence handle, so the essence must have been opened or created.
	//@comm Possible Errors:<nl>
	//   OM_ERR_NULL_PARAM -- A NULL rectangle pointer.<nl>
	//   OM_ERR_INVALID_OP_CODEC -- This codec doesn't support display rect<nl>
	//   							(may not be video essence)
 	//@comm Replaces omfmGetSampledRect

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::GetSampleCount (aafLength_t *  /*result*/)
{
#if FULL_TOOLKIT
	aafInt64		one;
	
	aafAssertMediaHdl(this);
	aafAssertValidFHdl(_mainFile);
	aafAssertMediaInitComplete(_mainFile);
	aafAssert(result != NULL, _mainFile, OM_ERR_NULL_PARAM);

	CvtInt32toInt64(0, result);
	CvtInt32toInt64(1, &one);
	
   if(Int64Greater(_pvt->repeatCount, one))
	{
	  *result = _pvt->repeatCount;
	  return(OM_ERR_NONE);
	}

	AddInt64toInt64(_channels[0].numSamples, result);
		
	return (OM_ERR_NONE);
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

	//@comm A video sample is one frame.
	//@comm Replaces omfmGetSampleCount

/****/
 AAFRESULT STDMETHODCALLTYPE
   ImplAAFEssenceAccess::ReadDataSamples (aafInt32  /*nSamples*/,
                           aafInt32  /*buflen*/,
                           aafDataBuffer_t  /*buffer*/,
                           aafUInt32*  /*bytesRead*/)
{
#if FULL_TOOLKIT
	aafMultiXfer_t xfer;
	aafInt64		one;
	
	aafAssertMediaHdl(this);
	aafAssertValidFHdl(_mainFile);
	aafAssertMediaInitComplete(_mainFile);
	aafAssert(buffer != NULL, _mainFile, OM_ERR_NULL_PARAM);
	aafAssert(bytesRead != NULL, _mainFile, OM_ERR_NULL_PARAM);

	CvtInt32toInt64(1, &one);
	if(Int64Greater(_pvt->repeatCount, one))
	  GotoShortFrameNumber(1);

	XPROTECT(_mainFile)
	{
		xfer.subTrackNum = _physicalOutChanOpen;
		xfer.numSamples = nSamples;
		xfer.buflen = buflen;
		xfer.buffer = buffer;
		xfer.bytesXfered = 0;
	
		CHECK(_codec->codecReadBlocks(this, deinterleave, 1, &xfer));
	}
	XEXCEPT
	{
		*bytesRead = xfer.bytesXfered;
	}
	XEND
	
	*bytesRead = xfer.bytesXfered;

	return (OM_ERR_NONE);
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

	//@comm This call will only return a single channel of essence from an interleaved
	// stream.
	//@comm A video sample is a frame.
	// Buflen is in bytes, and should be large enough to hold the samples
	// in the requested memory format.
	//@comm Replaces omfmReadDataSamples

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::ReadMultiSamples (aafInt16  /*elemCount*/,
                           aafmMultiXfer_t *  /*xferArray*/)
{
#if FULL_TOOLKIT
	aafAssertMediaHdl(this);
	aafAssertValidFHdl(_mainFile);
	aafAssertMediaInitComplete(_mainFile);
	aafAssert(xferArray != NULL, _mainFile, OM_ERR_NULL_PARAM);

	return (_codec->codecReadBlocks(this, deinterleave, elemCount, xferArray));
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

	//@comm arrayElemCount is the size of the array or transfer operations.
	// xferArray points to an array of transfer parameters.  All fields
	// in this array except for bytesXferred must be set up before
	// doing the transfer.
	//@comm Replaces omfmReadMultiSamples
	
/****/
 AAFRESULT STDMETHODCALLTYPE
   ImplAAFEssenceAccess::ReadDataLines (aafInt32  /*nLines*/,
                           aafInt32  /*bufLen*/,
                           aafDataBuffer_t  /*buffer*/,
                           aafInt32*  /*bytesRead*/)
{
#if FULL_TOOLKIT
	aafAssertMediaHdl(this);
	aafAssertValidFHdl(_mainFile);
	aafAssertMediaInitComplete(_mainFile);
	aafAssert(buffer != NULL, _mainFile, OM_ERR_NULL_PARAM);

	return (_codec->codecReadLines(this, nLines, bufLen, buffer, bytesRead));
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
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
    ImplAAFEssenceAccess::GotoFrameNumber (aafInt64  /*frameNum*/)
{
#if FULL_TOOLKIT
	aafInt64		one;
	aafErr_t		status;
	
	aafAssertMediaHdl(this);
	aafAssertValidFHdl(_mainFile);
	aafAssertMediaInitComplete(_mainFile);

	CvtInt32toInt64(1, &one);
	if(Int64Greater(_pvt->repeatCount, one))
		status = _codec->codecSetFrameNum(this, one);
	else
		status = _codec->codecSetFrameNum(this, frameNum);
	return (status);
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

	//@comm Useful only on reading, you
	// can't seek aound while writing essence.
	//@comm An audio frame is one sample across all open channels.
	//@comm Replaces omfmGotoFrameNumber

/****/
AAFRESULT STDMETHODCALLTYPE
   ImplAAFEssenceAccess::GetVideoInfo (aafFrameLayout_t *  /*layout*/,
                           aafInt32 *  /*fieldWidth*/,
                           aafInt32 *  /*fieldHeight*/,
                           aafRational_t *  /*editrate*/,
                           aafInt16 *  /*bitsPerPixel*/,
                           aafPixelFormat_t *  /*defaultPixelFmt*/)
{
#if FULL_TOOLKIT
	aafFrameLayout_t	localLayout;
	aafInt16					localBitsPerPixel;
	aafInt32 localWidth, localHeight;
	aafRational_t		localEditrate;
	aafPixelFormat_t	localMemFmt;
	aafVideoMemOp_t		opList[6];

	aafAssertMediaHdl(this);
	aafAssertValidFHdl(_mainFile);
	aafAssertMediaInitComplete(_mainFile);
	if(layout == NULL)
		layout = &localLayout;
	if(fieldWidth == NULL)
		fieldWidth = &localWidth;
	if(fieldHeight == NULL)
		fieldHeight = &localHeight;
	if(editrate == NULL)
		editrate = &localEditrate;
	if(defaultMemFmt == NULL)
		defaultMemFmt = &localMemFmt;
	if(bitsPerPixel == NULL)
		bitsPerPixel = &localBitsPerPixel;

	XPROTECT(_mainFile)
	{
		CHECK(_mdes->ReadRational(OMMDFLSampleRate, editrate));
	
		opList[0].opcode = kAAFFrameLayout;
		opList[1].opcode = kAAFStoredRect;
		opList[2].opcode = kAAFPixelSize;
		opList[3].opcode = kAAFPixelFormat;
		opList[4].opcode = kAAFVFmtEnd;
		CHECK(_codec->codecGetVideoInfo(this, opList));
		*layout = opList[0].operand.expFrameLayout;
		*fieldWidth = opList[1].operand.expRect.xSize;
		*fieldHeight = opList[1].operand.expRect.ySize;
		*bitsPerPixel = opList[2].operand.expInt16;
		*defaultMemFmt = opList[3].operand.expPixelFormat;
	}
	XEXCEPT
	{
		if(XCODE() == OM_ERR_INVALID_OP_CODEC)
			RERAISE(OM_ERR_WRONG_MEDIATYPE);
	}
	XEND
	
	return (OM_ERR_NONE);
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

	//@comm Any parameters not required may have the pointers set to NULL.
	//@comm Possible Errors:<nl>
	// Standard errors (see top of file).<nl>
	// OM_ERR_WRONG_MEDIATYPE -- Not video essence.<nl>
	//@comm Replaces omfmGetVideoInfo */
			
			
/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::GetVideoInfoArray (ImplAAFEssenceFormat ** /*ops*/)
{
#if FULL_TOOLKIT
		return(_codec->codecGetVideoInfo(this, ops));
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

	//@comm Replaces omfmGetVideoInfoArray */

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::PutVideoInfoArray (ImplAAFEssenceFormat * /*ops*/)
{
#if FULL_TOOLKIT
		return(_codec->codecPutVideoInfo(this, ops));
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

	//@comm Replaces omfmPutVideoInfoArray */

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::GetNumRepresentations (ImplAAFMasterMob * /*masterMob*/,
                           aafSlotID_t  /*slotID*/,
                           aafInt32*  /*numReps*/)
{
#if FULL_TOOLKIT
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

	//@comm Replaces omfmGetNumRepresentations */

/****/
 AAFRESULT STDMETHODCALLTYPE
   ImplAAFEssenceAccess::GetRepresentationSourceClip (ImplAAFMasterMob * /*masterMob*/,
                           aafSlotID_t  /*slotID*/,
                           aafInt32  /*index*/,
                           ImplAAFSourceClip ** /*sourceClip*/)
{
#if FULL_TOOLKIT
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

	//@comm Replaces omfmGetRepresentationSourceClip */

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::GetAudioInfo (aafRational_t*  /*rate*/,
                           aafInt32*  /*sampleSize*/,
                           aafInt32*  /*numChannels*/)
{
#if FULL_TOOLKIT
	aafRational_t			localRate;
	aafInt32						localSampleSize;
	aafInt32						localNumChannels;
	aafAudioMemOp_t		opList[3];
	
	aafAssertMediaHdl(this);
	aafAssertValidFHdl(_mainFile);
	aafAssertMediaInitComplete(_mainFile);
	if(rate == NULL)
		rate = &localRate;
	if(sampleSize == NULL)
		sampleSize = &localSampleSize;
	if(numChannels == NULL)
		numChannels = &localNumChannels;

	XPROTECT(_mainFile)
	{
		CHECK(_mdes->ReadRational(OMMDFLSampleRate, rate));

		opList[0].opcode = kAAFSampleSize;
		opList[1].opcode = kAAFNumChannels;
		opList[2].opcode = kAAFAFmtEnd;
		CHECK(_codec->codecGetAudioInfo(this, opList));
		*sampleSize = opList[0].operand.sampleSize;
		*numChannels = opList[1].operand.numChannels;
	}
	XEXCEPT
	{
		if(XCODE() == OM_ERR_INVALID_OP_CODEC)
			RERAISE(OM_ERR_WRONG_MEDIATYPE);
	}
	XEND
		
	return (OM_ERR_NONE);
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

	//@comm Any parameters not required may have the pointers set to NULL.
	//@comm Possible Errors:<nl>
	//	Standard errors (see top of file).<nl>
	//	OM_ERR_WRONG_MEDIATYPE -- Not video essence.
	//@comm Replaces omfmGetAudioInfo */


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::GetAudioInfoArray (ImplAAFEssenceFormat ** /*ops*/)
{
#if FULL_TOOLKIT
		return(_codec->codecGetAudioInfo(this, ops));
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

	//@comm Replaces omfmGetAudioInfoArray */

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::PutAudioInfoArray (ImplAAFEssenceFormat * /*ops*/)
{
#if FULL_TOOLKIT
		return(_codec->codecPutAudioInfo(this, ops));
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}


/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::IsHardwareAssistedCodec (aafCodecID_t  /*codecID*/,
                           ImplAAFSourceMob * /*mob*/,
                           aafBool*  /*result*/)
{
#if FULL_TOOLKIT
	aafCodecSelectInfo_t	select;
	codecTable_t			codec_data;
	aafBool					found;
	
	aafAssertValidFHdl(_mainFile);
	aafAssertMediaInitComplete(_mainFile);
	aafAssert(result != NULL, _mainFile, OM_ERR_NULL_PARAM);

	*result = FALSE;
	XPROTECT(_mainFile)
	{
		TableLookupBlock(_mainFile->_session->_codecID, codecID,
									sizeof(codec_data), &codec_data, &found);
		if(!found)
			RAISE(OM_ERR_CODEC_INVALID);
	
		CHECK(_codec->codecGetSelectInfo(_mainFile, &codec_data, mdes, &select));
		*result = select.hwAssisted;
	}
	XEXCEPT
	{
		if(XCODE() == OM_ERR_INVALID_OP_CODEC)
			NO_PROPAGATE();
	}
	XEND
	
	return (OM_ERR_NONE);
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
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
    ImplAAFEssenceAccess::GetCodecName (aafCodecID_t  /*codecID*/,
                           aafInt32  /*namelen*/,
                           aafUInt8 *  /*name*/)
{
#if FULL_TOOLKIT
	aafCodecMetaInfo_t	meta;
	codecTable_t		codec_data;
	aafBool				found;
	AAFSession *	session;
	
	aafAssertValidFHdl(_mainFile);
	aafAssertMediaInitComplete(_mainFile);
	aafAssert(name != NULL, _mainFile, OM_ERR_NULL_PARAM);
	session = _mainFile->_session;
	
	XPROTECT(_mainFile)
	{
		TableLookupBlock(session->_codecID, codecID, sizeof(codec_data),
									&codec_data, &found);
		if(!found)
			RAISE(OM_ERR_CODEC_INVALID);
		
		CHECK(_codec->codecGetMetaInfo(session, &codec_data, NULL, name, namelen, &meta));
	}
	XEXCEPT
	{
		if(XCODE() == OM_ERR_INVALID_OP_CODEC)
		{
			strncpy(name, "<none supplied>", namelen);
			NO_PROPAGATE();
		}
	}
	XEND
	
	return (OM_ERR_NONE);
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

	//@comm No other call uses this name, so it may be fully descriptive, esp. of limitations.
	//@comm The name will be truncated to fit within "buflen" bytes.
	//@comm Possible Errors:<nl>
	// 	Standard errors (see top of file).<nl>
	//	OM_ERR_CODEC_INVALID - The given codec ID is not loaded.
	//@comm Replaces omfmCodecGetName */
	//@devnote Shouldn't we use a unicode aafString_t since AAF's public interface for
	// strings is supposed to be unicode?(TomR)

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::GetCodecID (aafCodecID_t*  /*codecID*/)
{
#if FULL_TOOLKIT
	aafCodecMetaInfo_t	meta;
	AAFSession *	session;
	
	aafAssertMediaHdl(this);
	aafAssertValidFHdl(_mainFile);
	aafAssertMediaInitComplete(_mainFile);
	session = _mainFile->_session;
	
	XPROTECT(_mainFile)
	{
		CHECK(_codec->codecGetMetaInfo(session, &_pvt->codecInfo, NULL, NULL, 0, &meta));
		*codecID = meta.codecID;
	}
	XEXCEPT
	XEND
	
	return (OM_ERR_NONE);
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

	//@comm This will be required in order to send private data to the codec.
	//@comm The name will be truncated to fit within "buflen" bytes.
	//@comm Replaces omfmMediaGetCodecID */

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::AddFrameIndexEntry (aafInt64  /*frameOffset*/)
{
#if FULL_TOOLKIT
	return(_codec->codecAddFrameIndexEntry(this, frameOffset));
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

	//@comm This function should NOT be called when essence is passed to
	//the reference implementation in an uncompressed format.
	//@comm Possible Errors:<nl>
	//	Standard errors (see top of file).<nl>
	//	OM_ERR_INVALID_OP_CODEC -- This kind of essence doesn't have a frame index<nl>
	//	OM_ERR_MEDIA_OPENMODE -- The essence is open for read-only.
	//@comm Replaces omfmAddFrameIndexEntry */

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::SetStreamCacheSize (aafUInt32  /*cacheSize*/)
{
#if FULL_TOOLKIT
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

	//@comm Replaces omfmSetStreamCacheSize */

/****/
AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::SourceGetVideoSignalType (aafVideoSignalType_t*  /*signalType*/)
{
#if FULL_TOOLKIT
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}

	//@comm Replaces omfmSourceGetVideoSignalType */


AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::GetStoredByteOrder (eAAFByteOrder_t *  /*pOrder*/)
{
#if FULL_TOOLKIT
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFEssenceAccess::GetNativeByteOrder (eAAFByteOrder_t *  /*pOrder*/)
{
#if FULL_TOOLKIT
#else
	return AAFRESULT_NOT_IMPLEMENTED;
#endif
}


extern "C" const aafClassID_t CLSID_AAFEssenceAccess;

OMDEFINE_STORABLE(ImplAAFEssenceAccess, CLSID_AAFEssenceAccess);

// Cheat!  We're using this object's CLSID instead of object class...
AAFRESULT STDMETHODCALLTYPE
ImplAAFEssenceAccess::GetObjectClass(aafUID_t * pClass)
{
  if (! pClass)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  memcpy (pClass, &CLSID_AAFEssenceAccess, sizeof (aafClassID_t));
  return AAFRESULT_SUCCESS;
}






 #if FULL_TOOLKIT
 /************************************************************************
 *
 * Internal support functions
 *
 ************************************************************************/

/************************
 * Function: InitMediaHandle (INTERNAL)
 *
 * 		Initialize an entire media handle structure so that fields are
 *			not left with random values.
 *
 * Argument Notes:
 *		<none>.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
aafErr_t AAFMedia::InitMediaHandle(
			AAFFileMob *		fileMob)
{
	aafAssertValidFHdl(_mainFile);
	aafAssertMediaInitComplete(_mainFile);

	XPROTECT(_mainFile)
	{
		_masterMob = NULL;
		_fileMob = NULL;
		_mdes = NULL;
		_dataObj = NULL;
		_userData = NULL;
		_numChannels = 0;
		CvtInt32toPosition(0, _dataStart);
		_codecVariety = NULL;
		_compEnable = kToolkitCompressionEnable;
		_dataFile = NULL;
		_cookie = MEDIA_COOKIE;
		_openType = kAAFNone;
		_channels = NULL;
		_fileMob = fileMob;
		_nilKind = _mainFile->_nilKind;
		_pictureKind = _mainFile->_pictureKind;
		_soundKind = _mainFile->_soundKind;
		_rawFile = NULL;			/* If non-omfi file */
		_physicalOutChanOpen = 0;
		_pvt = (AAFMediaPvt_t *)
				_mainFile->omOptMalloc(sizeof(AAFMediaPvt_t));
		_pvt->codecInfo.rev = 0;
		CvtInt32toInt64(1, &_pvt->repeatCount);
		_stream = new CodecStream;
		
		_pvt->nextMedia = _mainFile->_topMedia;
		_mainFile->_topMedia = this;
	
//!!!		_mainFile->_closeMediaProc = Close;
	}
	XEXCEPT
	XEND
	
	return (OM_ERR_NONE);
}


/************************
 * Function: LocateMediaFile (INTERNAL)
 *
 * 	Given a file handle and an initialized media handle, attempt
 *		to locate the media data, following all locators until the data
 *		is found.
 *
 *		This function sets up the dataFile pointer, and selects the codec
 *		but does NOT call codec open.
 *		If the media is not found by following a locator,
 *		the local file is searched for the media.  If the media is still
 *		not found, then the locatorFailureCallback routine is called.
 *			AAFFile * FailureCallback(AAFFile * file, AAFObject * mdes);
 *
 *		The locator failure callback should attempt to open the file by
 *		other means (such as putting up a dialog and asking the user).
 *		If the file is found by the callback, it should open the file
 *		and return the file handle (AAFFile *).  If a valid file handle
 *		is returned from the callback, then the open will continue as
 *		if nothing happened, and no error will be returned from this
 *		function.
 *
 *		The locator failure callback is set by the function
 *			LocatorFailureCallback().
 *
 * Argument Notes:
 *		StuffNeededBeyondNotesInDefinition.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
aafErr_t     AAFMedia::LocateMediaFile(
			AAFFileMob *		fileMob,	/* IN */
			AAFFile *			*dataFile,
			aafBool			*isAAF)
{
	aafUID_t        uid;
	aafFileFormat_t fmt;
	aafInt32           index, numItems;
	aafBool         found;
	AAFLocator     	*aLoc;
	AAFMediaDescriptor *mdes;
	AAFFile *        refFile;
	AAFObject		*tmp;
	aafErr_t			status;
	AAFHeader		*head;
	
	aafAssertValidFHdl(_mainFile);
	refFile = _mainFile;
	XPROTECT(_mainFile)
	{
		CHECK(_mainFile->GetHeadObject(&head));
		CHECK(fileMob->GetMediaDescription(&mdes));
		CHECK(fileMob->ReadUID(OMMOBJMobID, &uid));
	
		found = FALSE;
		/* MCX/NT Hack! */
		if(head->IsMediaDataPresent(uid, kAAFMedia))
		{
			*dataFile = _mainFile;
			*isAAF = TRUE;
			found = TRUE;
		}
		
		if(!found)
		{
			status = mdes->ReadBoolean(OMMDFLIsAAF, isAAF);
			if(uid.prefix != 1)
 			{
 				fmt = (*isAAF ? kAAFMedia : kForeignMedia);
 				CHECK(status);
 			}
 			else
 			{
 				fmt = kAAFMedia;			/* Hack for MCX/NT */
 			}
			numItems = mdes->GetObjRefArrayLength(OMMDESLocator);
			for (index = 1; (index <= numItems) && !found; index++)
			{
				CHECK(mdes->ReadNthObjRefArray(OMMDESLocator,
														&tmp, index));
				aLoc = (AAFLocator *)tmp;	//!!CASTING
				CHECK(aLoc->TrialOpenFile(uid, fmt, dataFile, &found));

				/* Release Bento reference, so the useCount is decremented */
				OMLReleaseObject((OMLObject)aLoc);
			}
		}
	
		if (!found &&
			(*dataFile == NULL) &&
			(_mainFile->_locatorFailureCallback != NULL))
		{
			refFile = (*_mainFile->_locatorFailureCallback) (_mainFile, mdes);
			if (refFile != NULL)
			{
				if(refFile->IsMediaDataPresent(uid, fmt))
				  {
					 *dataFile = refFile;
					 found = TRUE;
				  }
				else
					*dataFile = NULL;
			}
		}
	}
	XEXCEPT
	{
		*dataFile = NULL;
	}
	XEND
	
	if(!found)
		*dataFile = NULL;
		
	return (OM_ERR_NONE);
}


/************************
 * Function: SetupCodecInfo (INTERNAL)
 *
 * 		Attempt to open media in the current file.  This involves
 *		searching for the correct mobID in an AAF file.  The result
 *		is then passed to the correct codec (based upon the media
 *		descriptor) which determines if the media is valid.
 *
 * Argument Notes:
 *		<none>.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */


 #define incrementListPtr(lp, t) lp = (t *) ((char *)lp + sizeof(t))

/************************
 * VideoOpCount
 *
 * 		Returns the length of an operation list.  Loop through
 *		the list, looking for the end marker, counting the 
 *		loops.
 *
 * Argument Notes:
 *		If list is NULL, count is zero.
 *
 * ReturnValue:
 *		Count of operations in list.
 *
 * Possible Errors:
 *		None.
 */

aafInt32 AAFMedia::VideoOpCount(
				aafVideoMemOp_t *list)
{
	aafInt32 count = 0;

	aafAssertValidFHdl(_mainFile);

	while (list && list->opcode != kAAFVFmtEnd)
	{
		incrementListPtr(list, aafVideoMemOp_t);		/* point to next operation */
		count++;	/* got another one! */
	}
	
	return(count);
}
	
/************************
 * VideoOpInit
 *
 * 		Sets a list to be empty.
 *
 * Argument Notes:
 *		'list' is a pointer to the head of an operation list.
 *		Insert an end marker in the head of the list.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		OM_ERR_NONE
 *		OM_ERR_NULL_PARAM
 */
 
aafErr_t AAFMedia::VideoOpInit(
				aafVideoMemOp_t *list)
{
	aafInt32 count = 0;

	aafAssertValidFHdl(_mainFile);
	aafAssert(list, _mainFile, OM_ERR_NULL_PARAM);

	list->opcode = kAAFVFmtEnd;
	
	return(OM_ERR_NONE);
}

/************************
 * VideoOpAppend
 *
 * 		Puts an operation in a list.
 *
 * Argument Notes:
 *		operation is a struct value, list is a pointer to
 *		the head of an operation list, force indicates whether 
 *		or not to overwrite a previously stored operation.  
 *		If force is kAAFForceOverwrite, then overwrite.
 *		If force is kAAFAppendIfAbsent, then don't do anything
 *		if the operation already exists.  maxLength is the
 *		number of items (including the end marker) that can
 *		be stored in 'list'
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		OM_ERR_NONE
 *		OM_ERR_NULL_PARAM
 *		OM_ERR_OPLIST_OVERFLOW
 */
 
aafErr_t AAFMedia::VideoOpAppend(
				aafAppendOption_t force,
				aafVideoMemOp_t item, 
				aafVideoMemOp_t *list,
				aafInt32 maxLength)
{
	aafVideoFmtOpcode_t savedOpcode;
	aafInt32 count = 0;

	aafAssertValidFHdl(_mainFile);
	aafAssert(list, _mainFile, OM_ERR_NULL_PARAM);

	while (list->opcode != kAAFVFmtEnd &&
		   list->opcode != item.opcode)
	{
		incrementListPtr(list, aafVideoMemOp_t);				/* point to next operation */
		count++;
	}
	
	/* if the number of items counted in the list plus
	   the end marker is greater than or equal to the 
	   maximum number of items allowed in the list, then
	   return an overflow error, since there's no way to
	   add the item to the list.  If the item matches a
	   previous entry, then this error should not be raised
	   unless the list is overflowing to begin with.
	*/
	
	if ((count + 1) >= maxLength)
		return(OM_ERR_OPLIST_OVERFLOW);
	
	savedOpcode = list->opcode; /* why did the loop terminate? */
	
	/* At this time, 'list' points to either a previously
	   stored operation, or the end marker.  If the force
	   code is kAAFForceOverwrite, then write the item into
	   the list regardless.  Otherwise, only write the item
	   if 'list' is pointing at the end marker */
	   
	if (force == kAAFForceOverwrite || 
		savedOpcode == kAAFVFmtEnd)
	{
		*list = item;
		incrementListPtr(list, aafVideoMemOp_t);
	}
	
	/* if the opcode is kAAFVFmtEnd, then the item wasn't found,
	   so it was appended, overwriting the end marker. Now restore
	   the end marker. We assume that 'list' has been incremented
	   past the old end marker, which is why we have to use 
	   'savedOpcode'. */
	if (savedOpcode == kAAFVFmtEnd)
		list->opcode = kAAFVFmtEnd;

	
	return(OM_ERR_NONE);
}
	
/************************
 * VideoOpMerge
 *
 * 		Merges two lists.
 *
 * Argument Notes:
 *		source is a pointer to the head of the source list, 
 *		destination is a pointer to the head of a destination
 *		list. Force indicates whether 
 *		or not to overwrite previously stored operations.  
 *		If force is kAAFForceOverwrite, then overwrite.
 *		If force is kAAFAppendIfAbsent, then don't do anything
 *		if the operations already exist.  maxDestLength is the
 *		number of items (including the end marker) that can
 *		be stored in 'destination'
 *
 *		ASSUMPTION: destination points to enough memory to store
 *		the additional item!
 *
 * ReturnValue:
 *		'destination' will receive the merged operations.
 *
 *		The function returns an error code (see below).
 *
 * Possible Errors:
 *		OM_ERR_NONE
 *		OM_ERR_NULL_PARAM
 *		OM_ERR_OPLIST_OVERFLOW
 */
 
aafErr_t AAFMedia::VideoOpMerge(
				aafAppendOption_t force,
				aafVideoMemOp_t *source, 
				aafVideoMemOp_t *destination,
				aafInt32 maxDestLength)
{

	/* the algorithm will be simple for now (It's not efficient,
	   n-squared, but should be okay given the small magnitude
	   expected).  Loop through the source, appending the items
	   one at a time into the destination, passing along 'force'. */
	   
	aafAssertValidFHdl(_mainFile);
	aafAssert(source, _mainFile, OM_ERR_NULL_PARAM);
	aafAssert(destination, _mainFile, OM_ERR_NULL_PARAM);

	XPROTECT(_mainFile)
	{
		while (source->opcode != kAAFVFmtEnd)
		{
			CHECK(VideoOpAppend(force, *source,
									destination, maxDestLength));
			incrementListPtr(source, aafVideoMemOp_t);
		}
	}
	XEXCEPT
	XEND
	
	return(OM_ERR_NONE);
}
	
/************************
 * SetStreamCacheSize
 *
 * 		Sets the size of the buffer used when caching read and
 *
 * Argument Notes:
 *		Zero is a calid cache size, although negative numbers are not
 *		allowed.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 *		OM_ERR_INVALID_CACHE_SIZE -- Cache size must be >= 0.
 */
aafErr_t AAFMedia::SetStreamCacheSize(
			aafUInt32			cacheSize)		/* make the cache this big */
{
	
	aafAssertMediaHdl(this);
	aafAssert(cacheSize >= 0, _mainFile, OM_ERR_INVALID_CACHE_SIZE);
	
	XPROTECT(_mainFile)
	{
#if	AAF_ENABLE_STREAM_CACHE
		if((_stream->_cachePtr == NULL) || (cacheSize != _stream->_cachePhysSize))
		{
			if(_stream->_cachePtr != NULL)
				_mainFile->omOptFree(_stream->_cachePtr);
			
			_stream->_cachePtr = (char *)_mainFile->omOptMalloc(cacheSize);
			if(_stream->_cachePtr != NULL)
				_stream->_cachePhysSize = cacheSize;
			else
			{
				_stream->_cachePhysSize = 0;
				RAISE(OM_ERR_NOMEMORY);
			}
		}
#endif
	}
	XEXCEPT
	XEND

	return(OM_ERR_NONE);
}

/************************
 *  Function: SourceGetVideoSignalType
 *
 * 		If media has video, what is it's signal type
 *
 * Argument Notes:
 *		Get from tape if possible, if not, best guess.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */

aafErr_t AAFMedia::SourceGetVideoSignalType(aafVideoSignalType_t *signalType)
{
	aafPosition_t	zeroPos;
	aafVideoSignalType_t tmpSignalType = kVideoSignalNull;
	AAFMobSlot *mslot = NULL;
	AAFIterate *iterHdl;
	aafSearchCrit_t searchCrit;
	aafRational_t editRate;
	double editRateFloat;
	aafErr_t aafError = OM_ERR_NONE;
	aafInt32 trackID = -1;
	aafInt16 i = 0;
	AAFDataKind *pictureKind = NULL;
	aafFindSourceInfo_t	sourceInfo;
	AAFHeader		*mainHead;

	XPROTECT(_mainFile)
	{
		CHECK(_mainFile->GetHeadObject(&mainHead));
		CvtInt32toPosition(0, zeroPos);

		/* find video channel in file mob */
		mainHead->DatakindLookup(PICTUREKIND, &pictureKind, &aafError);
		CHECK(aafError);				
		
		i = _numChannels;
		while (i > 0 && trackID < 0)
		{
			if (_channels[i-1].mediaKind == pictureKind)
				trackID = _channels[i-1].trackID;
			i--;
		}
		
		if (trackID >= 0)
		{
			aafError = _fileMob->SearchSource(trackID,
							zeroPos, kTapeMob, NULL, NULL, NULL, &sourceInfo);
		}
		

		if (aafError == OM_ERR_NONE && sourceInfo.mob != NULL)
		{
			aafError = ((AAFTapeMob *)sourceInfo.mob)->GetSignalType(&tmpSignalType);
			if (aafError == OM_ERR_NONE && tmpSignalType != kVideoSignalNull)
			{
				*signalType = tmpSignalType;
				return(OM_ERR_NONE);
			}
			else /* no tape descriptor with signal tape, check editRate */
			{
				iterHdl = new AAFIterate(_mainFile);
				searchCrit.searchTag = kByDatakind;
				strcpy(searchCrit.tags.datakind, PICTUREKIND);
				CHECK(iterHdl->MobGetNextSlot(sourceInfo.mob, &searchCrit, &mslot));
				
				delete iterHdl;
				iterHdl = NULL;
				
				if (mslot != NULL)
				{
					CHECK(mslot->GetEditRate(&editRate));
					editRateFloat = (double)editRate.numerator / (double)editRate.denominator;
					if (fabs(editRateFloat - 29.97) < .1)
						tmpSignalType = kNTSCSignal;
					else if (fabs(editRateFloat - 25.0) < .01)
						tmpSignalType = kPALSignal;
					else
						tmpSignalType = kVideoSignalNull;

					if (tmpSignalType != kVideoSignalNull)
					{
						*signalType = tmpSignalType;
						return(OM_ERR_NONE);
					}
				}
			}
		}
		/* if this code is being executed, then the signal format wasn't 
		   found or deduced by the tape mob.  At this point, try guessing
		   from the sample rate? */
		CHECK(_mdes->ReadRational(OMMDFLSampleRate, &editRate));
		editRateFloat = (double)editRate.numerator / (double)editRate.denominator;
		if (fabs(editRateFloat - 29.97) < .1)
			*signalType = kNTSCSignal;
		else if (fabs(editRateFloat - 25.0) < .01)
			*signalType = kPALSignal;
		else
			*signalType = kVideoSignalNull;

		return(OM_ERR_NONE);
 
	}
	XEXCEPT
		if (iterHdl)
		{
			iterHdl = NULL; /* do this first to prevent infinite loops */
			delete iterHdl;
		}
	XEND
	
	*signalType = kVideoSignalNull;
	return(OM_ERR_NONE);
}

/************************
 * Function: DisposeCodecPersist	(INTERNAL)
 *
 * 		This is a private callback from the omfsEndSession call.  Certain
 *		per-process data is allocated by the media layer, and needs to be
 *		released when the session ends.
 *
 * Argument Notes:
 *		<none>.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
aafErr_t DisposeCodecPersist(AAFSession *sess)
{
	aafBool				more;
	omTableIterate_t	iter;
	codecTable_t		codec_table;
	
	XPROTECT(NULL)
	{
		CHECK(TableFirstEntry(sess->_codecID, &iter, &more));
		while(more)
		{
			if(iter.valuePtr != NULL)
			{
				memcpy(&codec_table, iter.valuePtr, sizeof(codec_table));
				if(codec_table.persist != NULL)
					AAFFree(codec_table.persist);
			}
			CHECK(TableNextEntry(&iter, &more));
		}
	}
	XEXCEPT
	XEND
	
	return(OM_ERR_NONE);
}

/************************
 * Function: PerFileMediaInit (INTERNAL)
 *
 * 		This function is a callback from the openFile and createFile
 *		group of functions.  This callback exists in order to allow the
 *		media layer to be independant, and yet have information of its
 *		own in the opaque file handle.
 *
 *    (NOTE: the data object cache needs to be set up whether or not
 *           the media layer exists, so the guts of this function were
 *           moved to omFile.c:BuildMediaCache().  This function shell was
 *           left for future expansion.)
 *
 * Argument Notes:
 *		<none>.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
aafErr_t PerFileMediaInit(AAFFile *file)
{	
	return(OM_ERR_NONE);
}

AAFFile *AAFMedia::itsMainFile(void)
{
	return(_mainFile);
}

AAFFile *AAFMedia::itsDataFile(void)
{
	return(_dataFile);
}

AAFMediaDescriptor *AAFMedia::itsMediaDescriptor(void)
{
	return(_mdes);
}
#endif

/***********************************************************************
 *
 *              Copyright (c) 1996 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and to distribute
 * and sublicense application software incorporating this software for
 * any purpose is hereby granted, provided that (i) the above
 * copyright notice and this permission notice appear in all copies of
 * the software and related documentation, and (ii) the name Avid
 * Technology, Inc. may not be used in any advertising or publicity
 * relating to the software without the specific, prior written
 * permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT, CONSEQUENTIAL OR OTHER DAMAGES OF
 * ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE, INCLUDING, 
 * WITHOUT  LIMITATION, DAMAGES RESULTING FROM LOSS OF USE,
 * DATA OR PROFITS, AND WHETHER OR NOT ADVISED OF THE POSSIBILITY OF
 * DAMAGE, REGARDLESS OF THE THEORY OF LIABILITY.
 *
 ************************************************************************/

#ifndef _OMF_MEDIA_API_
#define _OMF_MEDIA_API_ 1

#include "omErr.h"
#include "omTypes.h"
#include "omFile.h"
#include "omCodId.h"
#include "omCodCmn.h"

#if PORT_LANG_CPLUSPLUS
extern          "C"
{
#endif

#define FULL_LENGTH		(omfFrameLength_t)0xFFFFFFFF		/* Create 24-hour timecode object */


/****************************************/
/******	Initialize Media Layer		*****/
/****************************************/

OMF_EXPORT omfErr_t        omfmInit(omfSessionHdl_t sess);
	
/****************************************/
/******		Create File Mob			*****/
/****************************************/

OMF_EXPORT omfErr_t        omfmFileMobNew(omfHdl_t file,	/* IN -- */
						   char *name,
						   omfRational_t  sampleRate, /* IN -- Sample Rate */
						   omfCodecID_t codec,
						   omfObject_t * result);	/* OUT -- Returned
															 * object */

/****************************************/
/******		Write Media Data		*****/
/****************************************/

OMF_EXPORT omfErr_t omfmAudioMediaCreate(
			omfHdl_t			file,			/* IN -- In this file */
			omfObject_t			masterMob,		/* IN -- on this master mob */
			omfTrackID_t		masterTrackID,
			omfObject_t			fileMob,		/* IN -- and this file mob, create audio */
			omfRational_t		samplerate,		/* IN -- with this sample rate */
			omfCompressEnable_t enable,		/* IN -- optionally compressing it */
			omfInt16			sampleSize,		/* IN -- with this sample size  */
			omfInt16			numChannels, 	/* IN -- and this many channels. */
			omfMediaHdl_t		*resultPtr);	/* OUT -- Return the object here. */


OMF_EXPORT omfErr_t omfmVideoMediaCreate(
			omfHdl_t			file,			/* IN -- In this file */
			omfObject_t			masterMob,		/* IN -- on this master mob */
			omfTrackID_t		masterTrackID,
			omfObject_t			fileMob,		/* IN -- and this file mob, create video */
			omfCompressEnable_t enable,		/* IN -- optionally compressing it */
			omfRational_t		editrate,
			omfUInt32			StoredHeight,	/* IN -- with this height */
			omfUInt32			StoredWidth,	/* IN -- and this width */
			omfFrameLayout_t	layout,		/* IN -- and this frame layout. */
			omfRational_t		iRatio,			/* IN -- image aspect ratio */
			omfMediaHdl_t		*resultPtr);	/* OUT -- Return the object here. */

OMF_EXPORT omfErr_t omfmMediaCreate(
			omfHdl_t			file,		/* IN -- In this file */
			omfObject_t			masterMob,	/* IN -- on this master mob */
			omfTrackID_t		masterTrackID,
			omfObject_t			fileMob,	/* IN -- and this file mob */
			omfDDefObj_t		mediaKind,	/* IN -- create media of this type */
			omfRational_t		samplerate,	/* IN -- with this sample rate */
			omfCompressEnable_t enable,		/* IN -- optionally compressing it */
			omfMediaHdl_t		*result);	/* OUT -- Return the object here. */

OMF_EXPORT omfErr_t omfmMediaMultiCreate(
			omfHdl_t		file,		/* IN -- In this file */
			omfObject_t		masterMob,	/* IN -- on this master mob */
			omfObject_t		fileMob,	/* IN -- and this file mob, create */
			omfInt16			arrayElemCount,	/* IN -- this many channels */
			omfmMultiCreate_t *mediaArray,	/* IN -- using these definitions */
			omfRational_t	primarySampleRate,		/* IN -- Primary sample rate */
			omfCompressEnable_t enable,	/* IN -- optionally compressing it */
			omfMediaHdl_t	*result);	/* OUT -- Return the object here. */

OMF_EXPORT omfErr_t omfmSetBlockingSize(
			omfMediaHdl_t	media,		/* IN -- For this media reference */
			omfInt32			numBytes);	/* IN -- Preallocate this many bytes*/

OMF_EXPORT omfErr_t  omfmSetVideoLineMap(
			omfMediaHdl_t	media,		/* IN -- For this media reference */
			omfInt16			startLine,	/* IN -- set this starting video line */
			omfFieldTop_t	type);		/* IN -- and this top field */

OMF_EXPORT omfErr_t  omfmGetVideoTopField(
			omfMediaHdl_t	media,		/* IN -- For this media reference */
			omfFieldTop_t	*type);		/* OUT -- and this top field */

OMF_EXPORT omfErr_t omfmSetDisplayRect(
			omfMediaHdl_t	media,			/* IN -- For this media reference */
			omfRect_t		DisplayRect);	/* IN -- set sampled rect */

OMF_EXPORT omfErr_t omfmSetSampledRect(
			omfMediaHdl_t	media,			/* IN -- For this media reference */
			omfRect_t		SampledRect);	/* IN -- set sampled rect */

OMF_EXPORT omfErr_t omfmWriteDataSamples(
			omfMediaHdl_t	media,			/* IN -- For this media reference */
			omfInt32			nSamples,		/* IN -- write this many samples */
			void			*buffer,		/* IN -- to a buffer */
			omfInt32			buflen);		/* IN -- of this size */

OMF_EXPORT omfErr_t omfmWriteRawData(
			omfMediaHdl_t	media,			/* IN -- For this media reference */
			omfInt32			nSamples,		/* IN -- write this many samples */
			void			*buffer,		/* IN -- to a buffer */
			omfInt32			buflen);		/* IN -- of this size */

OMF_EXPORT omfErr_t omfmWriteMultiSamples(
			omfMediaHdl_t	media,			/* IN -- For this media reference */
			omfInt16			arrayElemCount,	/* IN -- Do this many transfers */
			omfmMultiXfer_t	*xferArray);	/* IN/OUT -- referencing this array */

OMF_EXPORT omfErr_t omfmWriteDataLines(
			omfMediaHdl_t	media,			/* IN -- For this media reference */
			omfInt32			nLines,			/* IN -- write this many lines on video */
			void			*buffer,		/* IN -- from a buffer */
			omfInt32			*bytesWritten);	/* OUT -- of this size. */

OMF_EXPORT omfErr_t omfmMediaClose(
			omfMediaHdl_t	media);			/* IN -- Close this media */

OMF_EXPORT omfErr_t omfmSetJPEGTables(
			omfMediaHdl_t	media,			/* IN -- For this media ref */
			omfJPEGcomponent_t JPEGcomp,	/* IN -- change this component */
			omfUInt8			* QTables,		/* IN -- to have these Q-tables */
			omfUInt8			* ACTables,		/* IN -- and these AC-tables*/
			omfUInt8			* DCTables,		/* IN -- and these DC-tables*/
			omfInt16			QTableSize,		/* IN -- size in bytes */
			omfInt16			ACTableSize,	/* IN -- size in bytes */
			omfInt16			DCTableSize);	/* IN -- size in bytes */

OMF_EXPORT omfErr_t omfmSetAudioCompressParms(
			omfMediaHdl_t	media,			/* IN -- For this media, set */
			omfInt16			blockLength);	/* IN -- the block length. */

OMF_EXPORT omfErr_t omfmCodecSendPrivateData(
			omfMediaHdl_t	media,			/* IN -- For this media, send */
			long			parmBlockSize,	/* IN -- a parm block of this size */
			void			*ParameterBlock);/* IN -- with these values. */

/****************************************/
/******	Reference an External File	*****/
/****************************************/
OMF_EXPORT omfErr_t omfmMobAddDOSLocator(omfHdl_t file,	/* IN -- */
			             omfObject_t fileMob,	/* IN -- */
	                     omfFileFormat_t isOMFI,	/* IN -- */
				         char *filename);	/* IN -- */

OMF_EXPORT omfErr_t omfmMobAddUnixLocator(omfHdl_t file,	/* IN -- */
				                omfObject_t fileMob,	/* IN -- */
			                    omfFileFormat_t isOMFI,	/* IN -- */
					            char *filename);	/* IN -- */

OMF_EXPORT omfErr_t omfmMobAddWindowsLocator(omfHdl_t file,	/* IN -- */
				                omfObject_t fileMob,	/* IN -- */
			                    omfFileFormat_t isOMFI,	/* IN -- */
								char			*path,			/* IN -- */
				                char *shortcut,	/* IN -- */
								omfInt32 shortCutLen);	/* IN -- */

OMF_EXPORT omfErr_t omfmMobAddMacLocator(omfHdl_t file,	/* IN -- */
				                omfObject_t fileMob,	/* IN -- */
			                    omfFileFormat_t isOMFI,	/* IN -- */
				                char *volumeName,	/* IN -- */
					            omfInt32 DirID,	/* IN -- */
					            char *filename);	/* IN -- */

OMF_EXPORT omfErr_t omfmMobAddTextLocator(
			omfHdl_t file,			/* IN -- */
			omfObject_t fileMob,	/* IN -- */
			char *message);			/* IN -- */

OMF_EXPORT omfErr_t omfmMobAddNetworkLocator(
				omfHdl_t		file,			/* IN -- */
				omfObject_t		fileMob,		/* IN -- */
				omfFileFormat_t	isOMFI,			/* IN -- */
				char			*url);			/* IN -- */
				
OMF_EXPORT omfErr_t omfmLocatorFailureCallback(
			omfHdl_t			file,		/* IN -- For this file */
			omfLocatorFailureCB	callback);	/* IN -- set this callback */

/****************************************/
/******		Create Source Mobs		*****/
/****************************************/
OMF_EXPORT omfErr_t        omfmTapeMobNew(omfHdl_t file,	/* IN -- */
	                       char *name,	/* IN -- */
	                       omfObject_t * result);	/* OUT -- Returned Obj */

OMF_EXPORT omfErr_t        omfmFilmMobNew(omfHdl_t file,	/* IN -- */
		                       char *name,	/* IN -- */
		                       omfObject_t * result);	/* OUT Returned obj */

OMF_EXPORT omfErr_t omfmTapeMobSetDescriptor(
		omfHdl_t file,                 /* IN - File Handle */
        omfMobObj_t tapeMob,           /* IN - Tape Mob */
        omfTapeCaseType_t *formFactor, /* IN - Form Factor (optional) */
		omfVideoSignalType_t *videoSignal, /* IN - Video Signal *optional) */
        omfTapeFormatType_t *tapeFormat, /* IN - Tape Format (optional) */
        omfLength_t *length,           /* IN - Length in minutes (optional) */
        omfString manufacturer,        /* IN - Manufacturer (optional) */
        omfString model);              /* IN - Model Brand Name (optional) */

OMF_EXPORT omfErr_t omfmFilmMobSetDescriptor(
		  omfHdl_t file,             /* IN - File Handle */
		  omfMobObj_t filmMob,       /* IN - Film Mob */
		  omfFilmType_t *filmFormat, /* IN - Film Format (optional) 
									  *     (kFt35MM, kFt16MM, kFt8MM, kFt65MM)
									  */
		  omfUInt32 *frameRate,      /* IN - Frame Rate (optional) */
		  omfUInt8 *perfPerFrame,    /* IN - Perforations per frame
									  *      (optional)
									  */
		  omfRational_t *aspectRatio,/* IN - Film Aspect Ratio (optional) */
		  omfString manufacturer,    /* IN - Film Manufacturer (optional) */
		  omfString model);          /* IN - Manufacturer's Brand (optional) */

OMF_EXPORT omfErr_t omfmFilmMobGetDescriptor(
		  omfHdl_t file,             /* IN - File Handle */
		  omfMobObj_t filmMob,       /* IN - Film Mob */
		  omfFilmType_t *filmFormat, /* OUT - Film Format 
									  *   (kFt35MM, kFt16MM, kFt8MM, kFt65MM)
									  */
		  omfUInt32 *frameRate,      /* OUT - Frame Rate */
		  omfUInt8 *perfPerFrame,    /* OUT - Perforations per frame */
		  omfRational_t *aspectRatio,/* OUT - Film Aspect Ratio  */
		  omfInt32 manuSize,         /* IN - String size for manufact buffer */
		  omfString manufacturer,    /* OUT - Film Manufacturer */
		  omfInt32 modelSize,        /* IN - String size for model buffer */
		  omfString model);          /* OUT - Manufacturer's Brand */

OMF_EXPORT omfErr_t omfmTapeMobGetDescriptor(
		omfHdl_t file,                 /* IN - File Handle */
        omfMobObj_t tapeMob,           /* IN - Tape Mob */
        omfTapeCaseType_t *formFactor, /* OUT - Form Factor */
		omfVideoSignalType_t *videoSignal, /* OUT - Video Signal */
        omfTapeFormatType_t *tapeFormat, /* OUT - Tape Format */
        omfLength_t *length,       /* OUT - Length in minutes */
		omfInt32 manuSize,         /* IN - String size for manufact buffer */
        omfString manufacturer,    /* OUT - Manufacturer */
		omfInt32 modelSize,        /* IN - String size for model buffer */
        omfString model);          /* OUT - Model Brand Name */

OMF_EXPORT omfErr_t        omfmRawSourceMobNew(omfHdl_t file,	/* IN -- */
			                 char *name,	/* IN -- */
		                     omfClassIDPtr_t mdesClass,	/* IN -- */
	                         omfPhysicalMobType_t mobType,	/* IN -- */
		                     omfObject_t * result);	/* OUT -- Returned Obj */

OMF_EXPORT omfErr_t        omfmMobAddPhysSourceRef(omfHdl_t file,
				                     omfObject_t aMob,
				                     omfRational_t editrate,	/* IN -- */
				                     omfTrackID_t aMobTrack,
				                     omfDDefObj_t  mediaKind,
				                     omfObject_t sourceRef,
				                     omfPosition_t srcRefOffset,
				                     omfInt32 srcRefTrack,
				                     omfLength_t srcRefLength);

OMF_EXPORT omfErr_t omfmMobAddPulldownRef(
				omfHdl_t			file,
				omfObject_t			aSourceMob,
				omfRational_t		editrate,			/* IN -- */
				omfTrackID_t		aMobTrack,
				omfDDefObj_t		mediaKind,
				omfObject_t			sourceRefObj,
				omfPosition_t		srcRefOffset,
				omfInt32			srcRefTrack,
				omfLength_t			srcRefLength,
				omfPulldownKind_t	pulldownKind,
				omfPhaseFrame_t		phaseFrame,
				omfPulldownDir_t	direction);
/****************************************/
/******		Timecode Object			*****/
/****************************************/
OMF_EXPORT omfErr_t        omfmMobAddTimecodeClip(omfHdl_t file,	/* IN -- */
			                    omfObject_t aSourceMob,	/* IN -- */
			                    omfRational_t editrate,	/* IN -- */
				   				omfInt32 trackLabel,	/* IN -- */
								omfTimecode_t		startTC,		/* IN -- */
								omfFrameLength_t	length32);	/* IN -- */

OMF_EXPORT omfErr_t        omfmMobAddEdgecodeClip(omfHdl_t file,	/* IN -- */
			                     omfObject_t aSourceMob,	/* IN -- */
			                     omfRational_t editrate,	/* IN -- */
				                 omfInt32 trackLabel,	/* IN -- */
					             omfFrameOffset_t startEC,	/* IN -- */
					             omfFrameLength_t length,	/* IN -- */
				                 omfFilmType_t filmKind,	/* IN -- */
				                 omfEdgeType_t codeFormat,	/* IN -- */
					             char *header);	/* IN -- */

OMF_EXPORT omfErr_t        omfmMobValidateTimecodeRange(omfHdl_t file,	/* IN -- */
				                     omfObject_t aSourceMob,	/* IN -- */
					                 omfDDefObj_t  mediaKind,	/* IN -- */
					                 omfTrackID_t trackLabel,	/* IN -- */
				            	     omfRational_t editrate,	/* IN -- */
					                 omfFrameOffset_t startOffset,	/* IN -- */
					                 omfFrameLength_t length);	/* IN -- */

OMF_EXPORT omfErr_t omfmTimecodeToOffset( 
    omfHdl_t	file,           /* IN - File Handle */
	omfTimecode_t timecode,		/* IN - The timecode value */
	omfMobObj_t	sourceMob,      /* IN - Source Mob */
    omfTrackID_t trackID,       /* IN - Track ID of track in source mob */
	omfFrameOffset_t *result);  /* OUT - Resulting offset in source track */

OMF_EXPORT omfErr_t omfmOffsetToTimecode(
    omfHdl_t file,        		/* IN - File Handle */
	omfObject_t mob,      		/* IN - Input Mob */
	omfTrackID_t trackID,  		/* IN - Track ID of the track in the input mob */
	omfPosition_t offset,  		/* IN - Offset into the given track */
	omfTimecode_t *timecode);	/* OUT - The timecode value */


/****************************************/
/******		Create Master Mob		*****/
/****************************************/
OMF_EXPORT omfErr_t omfmMasterMobNew(
			omfHdl_t file,	/* IN -- */
			char *name,	/* IN -- */
			omfBool	isPrimary,
			omfObject_t * result);	/* OUT -- Returned object */

OMF_EXPORT omfErr_t        omfmMobAddMasterTrack(omfHdl_t file,	/* IN -- */
			                      omfObject_t masterMob,	/* IN -- */
				                  omfDDefObj_t  mediaKind,	/* IN -- */
				                  omfTrackID_t trackLabel,	/* IN -- */
			                      omfTrackID_t fileTrackLabel,	/* IN -- */
				                  char *trackName,	/* IN -- */
			                      omfObject_t fileMob);	/* IN -- */

/****************************************/
/******	High level Export wrapper	*****/
/****************************************/
OMF_EXPORT omfErr_t omfmStandardTapeMobNew(
			omfHdl_t		file,			/* IN -- */
			omfRational_t	vEditRate,		/* IN -- */
			omfRational_t	aEditRate,		/* IN -- */
			char			*name,			/* IN -- */
			omfInt16			numVideoTracks,	/* IN -- */
			omfInt16			numAudioTracks,	/* IN -- */
			omfObject_t		*result);		/* OUT -- Returned object */

/****************************************/
/******	Repeat for each file mob	*****/
/****************************************/

OMF_EXPORT omfErr_t omfmMobGetMediaDescription(
			omfHdl_t file,
			omfObject_t fileMob,
			omfObject_t *mdes);
				
/****************************************/
/******		Read Media Data			*****/
/****************************************/
OMF_EXPORT omfErr_t omfmGetNumChannels(
			omfHdl_t		file,			/* IN -- For this file */
			omfObject_t		masterMob,		/* IN -- In this master mob */
			omfTrackID_t	trackID,		/* IN -- On this track */
			omfMediaCriteria_t *mediaCrit,	/* IN -- using this media criteria */
			omfDDefObj_t  	mediaKind,		/* IN -- for this media type */
			omfInt16		*numCh);		/* OUT -- How many channels? */

OMF_EXPORT omfErr_t omfmGetLargestSampleSize(
			omfMediaHdl_t	media,		/* IN -- For this media stream */
			omfDDefObj_t  mediaKind,		/* IN -- and this media type */
			omfInt32			*maxSize);	/* OUT - the largest sample size */

OMF_EXPORT omfErr_t omfmGetSampleFrameSize(
			omfMediaHdl_t	media,		/* IN -- For this media stream */
			omfDDefObj_t	mediaKind,	/* IN -- and this media type */
			omfPosition_t	frameNum,	/* IN -- for this (1-based) sample frame number */
			omfLength_t		*frameSize);	/* OUT -- How big is the sample frame? */

OMF_EXPORT omfErr_t omfmMediaOpen(
			omfHdl_t		file,			/* IN -- For this file */
			omfObject_t		masterMob,		/* IN -- In this master mob */
			omfTrackID_t	trackID,		/* IN -- On this track */
			omfMediaCriteria_t *mediaCrit,	/* IN -- using this media criteria */
			omfMediaOpenMode_t	openMode,	/* IN -- ReadOnly or Append */
			omfCompressEnable_t compEnable,	/* IN - optionally decompressing */
			omfMediaHdl_t	*mediaPtr);		/* OUT -- and return a meida handle */

OMF_EXPORT omfErr_t omfmMediaMultiOpen(
			omfHdl_t			file,		/* IN -- For this file */
			omfObject_t			masterMob,	/* IN -- In this master mob */
			omfTrackID_t		trackID,	/* IN -- On this track */
			omfMediaCriteria_t *mediaCrit,	/* IN -- using this media criteria */
			omfMediaOpenMode_t	openMode,	/* IN -- ReadOnly or Append */
			omfCompressEnable_t compEnable,	/* IN - optionally decompressing */
			omfMediaHdl_t		*mediaPtr);	/* OUT - and return the media handle */

OMF_EXPORT omfErr_t omfmIsMediaContiguous(
			omfMediaHdl_t	media,			/* IN -- Is this media */
			omfBool			*isContiguous);	/* OUT -- contiguous? */

OMF_EXPORT omfBool  omfmIsMediaDataPresent(omfHdl_t file, 
								  omfUID_t fileMobUid,
								  omfFileFormat_t fmt);

OMF_EXPORT omfErr_t omfmSetVideoMemFormat(
			omfMediaHdl_t		media,	/* IN -- For this media use */
			omfVideoMemOp_t		*format);	/* IN -- and this mem layout */

OMF_EXPORT omfErr_t omfmSetAudioMemFormat(
			omfMediaHdl_t		media,		/* IN -- For this media use */
			omfAudioMemOp_t		*format);

OMF_EXPORT omfErr_t omfmGetDisplayRect(
			omfMediaHdl_t	media,			/* IN -- For this media */
			omfRect_t		*DisplayRect);	/* OUT -- Get the display rect */

OMF_EXPORT omfErr_t omfmGetSampledRect(
			omfMediaHdl_t	media,			/* IN -- For this media */
			omfRect_t		*SampledRect);	/* OUT -- Get the sampled rect */

OMF_EXPORT omfErr_t        omfmGetSampleCount(omfMediaHdl_t media,	/* IN -- */
			                       omfLength_t * numSamples);	/* OUT -- */

OMF_EXPORT omfErr_t        omfmReadDataSamples(omfMediaHdl_t media,	/* IN -- */
				                    omfInt32 nSamples,	/* IN -- */
				                    omfInt32 buflen,	/* IN -- */
				                    void *buffer,	/* IN/OUT -- */
					                omfUInt32 * bytesRead);	/* OUT -- */

OMF_EXPORT omfErr_t omfmReadRawData(
			omfMediaHdl_t	media,		/* IN -- For this media reference */
			omfInt32			nSamples,	/* IN -- write this many samples */
			omfUInt32			buflen,		/* IN -- of this size */
			void			*buffer,	/* IN -- to a buffer */
			omfUInt32			*bytesRead,
			omfUInt32			*samplesRead);

OMF_EXPORT omfErr_t        omfmReadMultiSamples(omfMediaHdl_t media,	/* IN -- */
		                       omfInt16 arrayElemCount,	/* IN -- */
		                       omfmMultiXfer_t * xferArray);	/* IN/OUT -- */

OMF_EXPORT omfErr_t        omfmReadDataLines(omfMediaHdl_t media,	/* IN -- */
					                  omfInt32 nLines,	/* IN -- */
					                  omfInt32 bufLen,	/* IN -- */
					                  void *buffer,	/* IN/OUT -- */
					                  omfInt32 * bytesRead);	/* OUT -- */

OMF_EXPORT omfErr_t        omfmGotoShortFrameNumber(omfMediaHdl_t media,	/* IN -- */
					                 omfInt32 frameNumber);	/* IN -- */

OMF_EXPORT omfErr_t        omfmGotoFrameNumber(omfMediaHdl_t media,	/* IN -- */
					                omfPosition_t frameNumber);	/* IN -- */

OMF_EXPORT omfErr_t        omfmGetAudioCompressParms(omfMediaHdl_t file,	/* IN -- */
				                       omfInt16 * blockLength);	/* OUT -- */

OMF_EXPORT omfErr_t        omfmGetPrivateMediaData(omfMediaHdl_t media,	/* IN -- */
					                     long blocksize,	/* IN -- */
				                      void *ParameterBlock);	/* IN/OUT -- */



/********************************************/
/******	Multiple media representations	*****/
/********************************************/
OMF_EXPORT omfErr_t omfmGetNumRepresentations(
			omfHdl_t 		file,		/* IN -- */
			omfObject_t 	masterMob,	/* IN -- */
			omfTrackID_t	trackID,	/* IN -- */
			omfNumSlots_t	*numReps);	/* OUT -- */

OMF_EXPORT omfErr_t omfmGetRepresentationSourceClip(
			omfHdl_t		file,		/* IN -- */
			omfObject_t 	masterMob,	/* IN -- */
			omfTrackID_t	trackID,	/* IN -- */
			omfInt32			index,		/* IN -- */
			omfObject_t		*sourceClip);	/* OUT -- */

OMF_EXPORT omfErr_t omfmGetCriteriaSourceClip(
			omfHdl_t file,	/* IN -- */
			omfObject_t masterMob,	/* IN -- */
			omfTrackID_t	trackID,	/* IN -- */
			omfMediaCriteria_t *criteria,		/* IN -- */
			omfObject_t		*retSrcClip);	/* OUT -- */

/****************************************/
/******		High-Level Import		*****/
/****************************************/

OMF_EXPORT omfErr_t        omfmMobGetTapeName(omfHdl_t file,	/* IN -- */
				                      omfObject_t masterMob,	/* IN -- */
				                     omfInt32 masterTrackLabel,	/* IN -- */
					                omfInt32 buflen,	/* IN -- */
					                char *buffer);	/* OUT -- */

OMF_EXPORT omfErr_t omfmMobGetNumLocators(
			    omfHdl_t        file,          /* IN -- */
				omfMobObj_t	    fileMob,	      /* IN -- */
				omfInt32		*numLocators);	/* OUT -- */

OMF_EXPORT omfErr_t omfmMobGetNextLocator(
				omfIterHdl_t	iter,		/* IN/OUT -- */
				omfMobObj_t		fileMob,	/* IN -- */
				omfObject_t		*locator);	/* OUT -- */

OMF_EXPORT omfErr_t        omfmLocatorGetInfo(omfHdl_t file,	/* IN -- */
									   omfObject_t locator,	/* IN -- */
									   omfClassIDPtr_t type,	/* OUT -- */
									   omfInt32 pathBufSize,	/* IN -- */
									   char *pathBuf);	/* OUT -- */

OMF_EXPORT omfErr_t        omfmMacLocatorGetInfo(omfHdl_t file,	/* IN -- */
										  omfObject_t locator,	/* IN -- */
										  omfInt16 * vRef,	/* OUT -- */
										  omfInt32 * DirID);	/* OUT -- */
										  
OMF_EXPORT omfErr_t omfmWindowsLocatorGetInfo(
				omfHdl_t		file,			/* IN -- */
				omfObject_t 	locator,		/* IN -- */
				omfInt32		shortCutLen,	/* IN -- */
				char			*shortCutBuf);			/* OUT -- */

OMF_EXPORT omfErr_t   omfmSourceGetVideoSignalType(omfMediaHdl_t media,
											   omfVideoSignalType_t *signalType);


/****************************************/
/******	Media codec related calls	*****/
/****************************************/

omfErr_t omfmRegisterCodec(omfSessionHdl_t file,
						   omfCodecDispPtr_t pluginRoutine,
						   omfCodecType_t		type);

OMF_EXPORT omfErr_t omfmFindCodec(
			omfHdl_t				file,
			omfObject_t			fileMob,
			omfCodecID_t		*result);

OMF_EXPORT omfErr_t omfmMediaGetCodecID(
			omfMediaHdl_t	media,
			omfCodecID_t	*codecID);

OMF_EXPORT omfErr_t omfmIsHardwareAssistedCodec(
									 omfHdl_t file, 
									 omfCodecID_t codecID, 
									 omfObject_t mdes, 
									 omfBool *hwAssisted);
OMF_EXPORT omfErr_t omfmCodecGetName(
				omfHdl_t file,
				omfCodecID_t codecID,
				omfInt32 namelen,
				char *name);

OMF_EXPORT omfErr_t omfmGetNumCodecs(omfSessionHdl_t sess,
 			omfString		dataKindString,
			omfFileRev_t	rev,
			omfInt32		*numCodecPtr);

OMF_EXPORT omfErr_t omfmGetIndexedCodecInfo(omfSessionHdl_t sess,
 						omfString		dataKindString,
						omfFileRev_t	rev,
			 			omfInt32  		index,		/* IN -- At this index */
			 			omfInt32  		idLen,		/* IN -- Into a buffer of this size */
			 			omfCodecID_t  	codecid,	/* OUT -- return the codec ID */
			 			omfInt32  		nameLen,	/* IN -- Into a buffer of this size */
			 			char		  	*codecName	/* OUT -- return the codec name */
						);

OMF_EXPORT omfErr_t omfmGetNumCodecVarieties(omfSessionHdl_t	sess,
								omfCodecID_t	codecID,
								omfFileRev_t    rev,
								char			*mediaKindString,
								omfInt32		*numVarietyPtr);

OMF_EXPORT omfErr_t omfmGetIndexedCodecVariety(omfSessionHdl_t sess,
						omfCodecID_t	codecID,
						omfFileRev_t    rev,
						char			*mediaKindString,
			 			omfInt32  		index,		/* IN -- At this index */
			 			omfInt32  		bufLen,	/* IN -- Into a buffer of this size */
			 			char		  	*varietyName	/* OUT -- return the variety name */
						);

/****************************************/
/******	External media access calls	*****/
/****************************************/

OMF_EXPORT omfErr_t omfmGetMediaFilePos(
				omfMediaHdl_t	media,
				omfInt64  *offset);	/* OUT - return file position here */
	

OMF_EXPORT omfErr_t omfmGetNumMediaSegments(
			omfMediaHdl_t	media,
			omfInt32		*numSegments);	/* OUT - return file position through here */

OMF_EXPORT omfErr_t omfmGetMediaSegmentInfo(
			omfMediaHdl_t	media,
			omfInt32		index,		/* IN -- for this segment */
			omfPosition_t	*startPos,	/* OUT -- where does the segment begin, */
			omfLength_t		*length,	/* OUT -- and how long is it? */
			omfPosition_t	*dataPos,	/* OUT -- Where does media data begin */
			omfLength_t		*dataLen);	/* OUT -- and how long is the data? */

omfErr_t omfmAddFrameIndexEntry(
			omfMediaHdl_t	media,			/* IN -- To this media*/
			omfInt64		frameOffset);	/* IN -- add a frame offset to it's frame index*/

/************************************************/
/******	Need to go to correct section		*****/
/************************************************/
OMF_EXPORT omfErr_t omfmImportRawRef(omfHdl_t main, 
						  omfObject_t fileMob, 
						  fileHandleType fh, 
						  omfInt16 fhSize);

OMF_EXPORT omfErr_t        omfmGetAudioInfo(omfMediaHdl_t media,
									omfRational_t	*rate,
									omfInt32		*sampleSize,
									omfInt32		*numChannels);

OMF_EXPORT omfErr_t        omfmGetAudioInfoArray(omfMediaHdl_t media,
			                 	 omfAudioMemOp_t *ops);
OMF_EXPORT omfErr_t        omfmPutAudioInfoArray(omfMediaHdl_t media,
			                 	 omfAudioMemOp_t *ops);

OMF_EXPORT omfErr_t        omfmGetVideoInfo(omfMediaHdl_t media,
									omfFrameLayout_t	*layout,
									omfInt32			*fieldWidth,
									omfInt32			*fieldHeight,
									omfRational_t		*editrate,
									omfInt16			*bitsPerPixel,
									omfPixelFormat_t	*defaultMemFmt);

OMF_EXPORT omfErr_t        omfmGetVideoInfoArray(omfMediaHdl_t media,
			                 	 omfVideoMemOp_t *ops);
OMF_EXPORT omfErr_t        omfmPutVideoInfoArray(omfMediaHdl_t media,
			                 	 omfVideoMemOp_t *ops);


OMF_EXPORT omfErr_t omfmMobAddNilReference(
				omfHdl_t 		file,		/* IN -- */
				omfObject_t 	mob,		/* IN -- */
				omfTrackID_t	trackID,	/* IN -- */
				omfLength_t		length,		/* IN -- */
				omfDDefObj_t 	dataKind, 
				omfRational_t	editRate);	/* IN -- */

OMF_EXPORT omfInt32 omfmVideoOpCount(
				omfHdl_t file, 
				omfVideoMemOp_t *list);

OMF_EXPORT omfErr_t omfmVideoOpInit(
				omfHdl_t file, 
				omfVideoMemOp_t *list);
				
OMF_EXPORT omfErr_t omfmVideoOpAppend(
				omfHdl_t file, 
				omfAppendOption_t force,
				omfVideoMemOp_t item, 
				omfVideoMemOp_t *list,
				omfInt32 maxLength);
				
OMF_EXPORT omfErr_t omfmVideoOpMerge(
				omfHdl_t file, 
				omfAppendOption_t force,
				omfVideoMemOp_t *source, 
				omfVideoMemOp_t *destination,
				omfInt32 maxDestLength);

omfErr_t omfmAddFrameIndexEntry(
			omfMediaHdl_t	media,			/* IN -- To this media*/
			omfInt64		frameOffset);	/* IN -- add a frame offset to it's frame index*/

#if PORT_LANG_CPLUSPLUS
}
#endif
#endif				/* _OMF_MEDIA_API_ */

/* INDENT OFF */
/*
;;; Local Variables: ***
;;; tab-width:4 ***
;;; End: ***
*/

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

/* Name: omEffect.c
 *
 * Overall Function: The building block functions to create and get
 *                   information about effects in a composition.
 *
 * Audience: Clients writing OMFI compositions with effects
 *
 * Public Functions:
 *      omfeVideoSpeedControlNew(), omfeVideoSpeedControlGetInfo()
 *      omfeVideoFrameMaskNew(), omfeVideoFrameMaskGetInfo()
 *      omfeVideoRepeatNew(), omfeVideoRepeatGetInfo()
 *      omfeVideoDissolveNew(), omfeVideoDissolveGetInfo()
 *      omfeMonoAudioDissolveNew(), omfeMonoAudioDissolveGetInfo()
 *      omfeStereoAudioDissolveNew(), omfeStereoAudioDissolveGetInfo()
 *      omfeMonoAudioGainNew(), omfeMonoAudioGainGetInfo()
 *      omfeStereoAudioGainNew(), omfeStereoAudioGainGetInfo()
 *      omfeMonoAudioPanNew(), omfeMonoAudioPanGetInfo()
 *      omfeVideoFadeToBlackNew(), omfeVideoFadeToBlackGetInfo()
 *      omfeMonoAudioMixdownNew(), omfeMonoAudioMixdownGetInfo()
 *      omfeSMPTEVideoWipeNew(), omfeSMPTEVideoWipeGetInfo()
 *      
 * General error codes returned:
 *      OM_ERR_NONE
 *             Success
 *      OM_ERR_NULLOBJECT
 *              Null object passed in as an argument
 *      OM_ERR_INVALID_EFFECTDEF
 *              An invalid effectdef was specified
 *      OM_ERR_BAD_LENGTH
 *              Segment has an illegal length
 *      OM_ERR_INTERN_TOO_SMALL
 *              Buffer size is too small
 *
 */

#ifndef _OMFI_EFFECTS_H_
#define _OMFI_EFFECTS_H_ 1

#include "omErr.h"
#include "omTypes.h"

#if PORT_LANG_CPLUSPLUS
extern          "C"
{
#endif

/* Global Arg IDs */

#define kOmfFxIDGlobalATrack		-1 /* a.k.a. Background */
#define kOmfFxIDGlobalBTrack		-2 /* a.k.a. Foreground */
#define kOmfFxIDGlobalLevel			-3
	
/* SPEED CONTROL */

#define kOmfFxSpeedControlGlobalName \
                      (omfUniqueNamePtr_t)"omfi:effect:VideoSpeedControl"

#define kOmfFxSpeedControlName (omfString)"Video Speed Control"

#define kOmfFxSpeedControlDescription (omfString) \
"Changes a segment of video by duplicating or subtracting frames according \
 to the edit ratio control"
 
#define kOmfFxIDSpeedControlRatio 		1
#define kOmfFxIDSpeedControlPhaseOffset 2

/* FRAME MASK */

typedef enum 
	{ 
		kOmfFxFrameMaskDirNone = 0,
		kOmfFxFrameMaskDirAdd,		/* output has more frames than input */
		kOmfFxFrameMaskDirDrop		/* output has fewer frames than input */
	} omfFxFrameMaskDirection_t;
	
#define kOmfFxFrameMaskGlobalName \
					  (omfUniqueNamePtr_t)"omfi:effect:VideoFrameMask"

#define kOmfFxFrameMaskName (omfString)"Video Frame Mask"

#define kOmfFxFrameMaskDescription (omfString) \
"Changes a segment of video by duplicating or subtracting frames according \
 to the repeating bits in a control mask"
 
#define kOmfFxIDFrameMaskMaskBits 		1
#define kOmfFxIDFrameMaskPhaseOffset 	2
#define kOmfFxIDFrameMaskAddOrDrop		3

/* REPEAT */

#define kOmfFxRepeatGlobalName (omfUniqueNamePtr_t)"omfi:effect:VideoRepeat"

#define kOmfFxRepeatName (omfString)"Video Repeat"

#define kOmfFxRepeatDescription (omfString) \
"Changes a segment of video by duplicating or subtracting frames according \
 to the repeating bits in a control mask"
 
#define kOmfFxIDRepeatPhaseOffset 	2


/* SIMPLE VIDEO DISSOLVE */

#define kOmfFxSVDissolveGlobalName \
  (omfUniqueNamePtr_t)"omfi:effect:VideoDissolve"

#define kOmfFxSVMCDissolveGlobalName \
  (omfUniqueNamePtr_t)"omfi:effect:SimpleVideoDissolve"

#define kOmfFxVDissolveName (omfString)"Simple Video Dissolve"

#define kOmfFxSVDissolveDescription (omfString) \
"Combines two video streams using a simple linear equation"
 

/* SIMPLE MONO AUDIO DISSOLVE */

#define kOmfFxSMADissolveGlobalName \
  (omfUniqueNamePtr_t)"omfi:effect:MonoAudioDissolve"

#define kOmfFxSMAMCDissolveGlobalName \
  (omfUniqueNamePtr_t)"omfi:effect:SimpleMonoAudioDissolve"

#define kOmfFxSMADissolveName (omfString)"Simple Mono Audio Dissolve"

#define kOmfFxSMADissolveDescription (omfString) \
"Combines two mono audio streams using a simple linear equation"
 
/* SIMPLE STEREO AUDIO DISSOLVE */

#define kOmfFxSSADissolveGlobalName \
  (omfUniqueNamePtr_t)"omfi:effect:StereoAudioDissolve"

#define kOmfFxSSAMCDissolveGlobalName \
  (omfUniqueNamePtr_t)"omfi:effect:SimpleStereoAudioDissolve"

#define kOmfFxSSADissolveName (omfString)"Simple Stereo Audio Dissolve"

#define kOmfFxSSADissolveDescription (omfString) \
"Combines two stereo audio streams using a simple linear equation"
 
/* MONO AUDIO GAIN */

#define kOmfFxMAGainGlobalName (omfUniqueNamePtr_t)"omfi:effect:MonoAudioGain"

#define kOmfFxMAGainName (omfString)"Mono Audio Gain"

#define kOmfFxMAGainDescription (omfString) \
"Changes the volume of a mono audio stream using a multiplier"

#define kOmfFxIDMAGainMultiplier 	1
 

/* STEREO AUDIO GAIN */

#define kOmfFxSAGainGlobalName \
  (omfUniqueNamePtr_t)"omfi:effect:StereoAudioGain"

#define kOmfFxSAGainName (omfString)"Stereo Audio Gain"

#define kOmfFxSAGainDescription (omfString) \
"Changes the volume of a stereo audio stream using a multiplier"

#define kOmfFxIDSAGainMultiplier 	1
 
/* MONO AUDIO PAN */

#define kOmfFxMAPanGlobalName (omfUniqueNamePtr_t)"omfi:effect:MonoAudioPan"

#define kOmfFxMAPanName (omfString)"Mono Audio Pan"

#define kOmfFxMAPanDescription (omfString) \
  "Splits a mono audio stream into a stereo stream"

#define kOmfFxIDMAPanRatio 	1
 

/* VIDEO FADE TO BLACK */

#define kOmfFxVFadeToBlackGlobalName \
  (omfUniqueNamePtr_t)"omfi:effect:VideoFadeToBlack"

#define kOmfFxVFadeToBlackName (omfString)"Video Fade to Black"

#define kOmfFxVFadeToBlackDescription (omfString) \
"Combines a video stream with black using a simple linear equation"

#define kOmfFxVFadeToBlackReverse 	1
 

/* MONO AUDIO MIXDOWN */

#define kOmfFxMAMixdownGlobalName \
  (omfUniqueNamePtr_t)"omfi:effect:MonoAudioMixdown"

#define kOmfFxMAMixdownName (omfString)"Mono Audio Mixdown"

#define kOmfFxMAMixdownDescription (omfString) \
"Combines multiple mono audio streams into a single mono stream by summing the values"

 
/* SMPTE VIDEO WIPE */

typedef struct {
	omfBool reverse;
	omfBool soft;
	omfBool border;
	omfBool position;
	omfBool modulator;
	omfBool shadow;
	omfBool tumble;
	omfBool spotlight;
	omfInt32	replicationH;
	omfInt32	replicationV;
	omfBool checkerboard;
} omfWipeArgs_t;

#define kOmfFxVSMPTEWipeGlobalName \
					  (omfUniqueNamePtr_t)"omfi:effect:SMPTEVideoWipe"

#define kOmfFxVSMPTEWipeName (omfString)"SMPTE Video Wipe"

#define kOmfFxVSMPTEWipeDescription (omfString) \
"Combines multiple mono audio streams into a single mono stream by summing the values"

#define kOmfFxIDVSMPTEWipeWipeNumber	1
#define kOmfFxIDVSMPTEWipeReverse		2
#define kOmfFxIDVSMPTEWipeSoft			3
#define kOmfFxIDVSMPTEWipeBorder		4
#define kOmfFxIDVSMPTEWipePosition		5
#define kOmfFxIDVSMPTEWipeModulator		6
#define kOmfFxIDVSMPTEWipeShadow		7
#define kOmfFxIDVSMPTEWipeTumble		8
#define kOmfFxIDVSMPTEWipeSpotLight		9
#define kOmfFxIDVSMPTEWipeReplicationH	10
#define kOmfFxIDVSMPTEWipeReplicationV	11
#define kOmfFxIDVSMPTEWipeCheckerboard    12



/* start of prototypes */
OMF_EXPORT omfErr_t omfeVideoSpeedControlNew(
    const omfHdl_t		file,          /* IN - File Handle */
    const omfLength_t	length,        /* IN - Effect Length */
	const omfSegObj_t	inputSegment,  /* IN - Input Segment */
	const omfRational_t	speedRatio,    /* IN - Speed Ratio */
	const omfUInt32		phaseOffset,   /* IN - Phase Offset */
    omfEffObj_t 	    *newEffect);   /* OUT - New effect object */

OMF_EXPORT omfErr_t omfeVideoSpeedControlGetInfo(
    const omfHdl_t		file,         /* IN - File Handle */
    const omfEffObj_t 	effect,       /* IN - Effect object */
    omfLength_t         *length,       /* OUT - Length of effect */
	omfSegObj_t			*inputSegment, /* OUT - Input segment */
	omfRational_t		*speedRatio,   /* OUT - Speed Ratio */
	omfUInt32		 	*phaseOffset); /* OUT - Phase Offset */

OMF_EXPORT omfErr_t omfeVideoFrameMaskNew(
    const omfHdl_t				file,         /* IN - File Handle */
    const omfLength_t			length,       /* IN - Effect Length */
	const omfSegObj_t			inputSegment, /* IN - Input segment */
	const omfUInt32				mask,         /* IN - Mask */
	const omfFxFrameMaskDirection_t addOrDrop,    /* IN - Specify add or drop*/
	const omfUInt32		 		phaseOffset,  /* IN - Phase Offset */
    omfEffObj_t 				*newEffect);  /* OUT - New Effect object */

OMF_EXPORT omfErr_t omfeVideoFrameMaskGetInfo(
    const omfHdl_t			file,          /* IN - File Handle */
    const omfEffObj_t 		effect,        /* IN - Effect object */
    omfLength_t             *length,       /* OUT - Length of effect */
	omfSegObj_t				*inputSegment, /* OUT - Input segment */
	omfUInt32				*mask,         /* OUT - Mask */
	omfFxFrameMaskDirection_t	*addOrDrop,    /* OUT - Add or drop */
	omfUInt32		 		*phaseOffset); /* OUT - Phase Offset */

OMF_EXPORT omfErr_t omfeVideoRepeatNew(
    const omfHdl_t 		file,         /* IN - File Handle */
    const omfLength_t	length,      /* IN - Effect Length */
	const omfSegObj_t	inputSegment, /* IN - Input segment */
	const omfUInt32		phaseOffset,  /* IN - Phase Offset */
    omfEffObj_t		    *newEffect);  /* OUT - New Effect Object */

OMF_EXPORT omfErr_t omfeVideoRepeatGetInfo(
    const omfHdl_t			file,          /* IN - File Handle */
    const omfEffObj_t 		effect,        /* IN - Effect Object */
    omfLength_t             *length,       /* OUT - Length of effect */
	omfSegObj_t				*inputSegment, /* OUT - Input segment */
	omfUInt32		 		*phaseOffset); /* OUT - Phase Offset */

OMF_EXPORT omfErr_t omfeVideoDissolveNew(
    omfHdl_t 		file,         /* IN - File Handle */
    omfLength_t		length,       /* IN - Effect Length */
	omfSegObj_t		inputSegmentA,/* IN - Input segment A */
	omfSegObj_t		inputSegmentB,/* IN - Input segment B */
	omfSegObj_t		level,        /* IN - Level */
    omfEffObj_t		*newEffect);  /* OUT - New Effect Object */

OMF_EXPORT omfErr_t omfeVideoDissolveGetInfo(
    const omfHdl_t			file,          /* IN - File Handle */
    const omfEffObj_t 		effect,        /* IN - Effect Object */
    omfLength_t             *length,       /* OUT - Length of effect */
	omfSegObj_t				*inputSegmentA,/* OUT - Input Segment A */
	omfSegObj_t				*inputSegmentB,/* OUT - Input Segment B */
	omfSegObj_t				*level);       /* OUT - Level */

OMF_EXPORT omfErr_t omfeMonoAudioDissolveNew(
    omfHdl_t 		file,          /* IN - File Handle */
    omfLength_t		length,        /* IN - Effect Length */
	omfSegObj_t		inputSegmentA, /* IN - Input Audio Segment A */
	omfSegObj_t		inputSegmentB, /* IN - Input Audio Segment B */
	omfSegObj_t		level,         /* IN - Level */
    omfEffObj_t		*newEffect);   /* OUT - New Effect Object */

OMF_EXPORT omfErr_t omfeMonoAudioDissolveGetInfo(
    const omfHdl_t			file,           /* IN - File Handle */
    const omfEffObj_t 		effect,         /* IN - Effect Object */
    omfLength_t             *length,        /* OUT - Length of effect */
	omfSegObj_t				*inputSegmentA, /* OUT - Input Audio Segment A */
	omfSegObj_t				*inputSegmentB, /* OUT - Input Audio Segment B */
	omfSegObj_t				*level);        /* OUT - Level */

OMF_EXPORT omfErr_t omfeStereoAudioDissolveNew(
    omfHdl_t 		file,          /* IN - File Handle */
    omfLength_t		length,        /* IN - Effect Length */
	omfSegObj_t		inputSegmentA, /* IN - Input segment A */
	omfSegObj_t		inputSegmentB, /* IN - Input segment B */
	omfSegObj_t		level,         /* IN - Level */
    omfEffObj_t		*newEffect);   /* OUT - New Effect Object */

OMF_EXPORT omfErr_t omfeStereoAudioDissolveGetInfo(
    const omfHdl_t			file,           /* IN - File Handle */
    const omfEffObj_t 		effect,         /* IN - Effect Object */
    omfLength_t             *length,        /* OUT - Length of effect */
	omfSegObj_t				*inputSegmentA, /* OUT - Input Segment A */
	omfSegObj_t				*inputSegmentB, /* OUT - Input Segment B */
	omfSegObj_t				*level);        /* OUT - Level */

OMF_EXPORT omfErr_t omfeMonoAudioGainNew(
    const omfHdl_t 			file,         /* IN - File Handle */
    const omfLength_t		length,       /* IN - Effect Length */
	const omfSegObj_t		inputSegment, /* IN - Input audio segment */
	const omfSegObj_t		multiplier,   /* IN - Amplitude Multiplier */
    omfEffObj_t		        *newEffect);  /* OUT - New Effect Object */

OMF_EXPORT omfErr_t omfeMonoAudioGainGetInfo(
    const omfHdl_t			file,         /* IN - File Handle */
    const omfEffObj_t 		effect,       /* IN - Effect object */
    omfLength_t             *length,      /* OUT - Length of effect */
	omfSegObj_t				*inputSegment,/* OUT - Input audio segment */
	omfSegObj_t				*multiplier); /* OUT - Amplitude Multiplier */

OMF_EXPORT omfErr_t omfeStereoAudioGainNew(
    const omfHdl_t 			file,         /* IN - File Handle */
    const omfLength_t		length,       /* IN - Effect Length */
	const omfSegObj_t		inputSegment, /* IN - Input segment */
	const omfSegObj_t		multiplier,   /* IN - Multiplier */
    omfEffObj_t		        *newEffect);  /* OUT - New Effect Object */

OMF_EXPORT omfErr_t omfeStereoAudioGainGetInfo(
    const omfHdl_t			file,         /* IN - File Handle */
    const omfEffObj_t 		effect,       /* IN - Effect Object */
    omfLength_t             *length,      /* OUT - Length of effect */
	omfSegObj_t				*inputSegment,/* OUT - Input segment */
	omfSegObj_t				*multiplier); /* OUT - Multiplier */

OMF_EXPORT omfErr_t omfeMonoAudioPanNew(
    const omfHdl_t 			file,         /* IN - File Handle */
    const omfLength_t		length,       /* IN  - Effect length */
	const omfSegObj_t		inputSegment, /* IN - Input segment */
	const omfSegObj_t		ratio,        /* IN - Ratio */
    	  omfEffObj_t		*newEffect);  /* OUT - New Effect Object */

OMF_EXPORT omfErr_t omfeMonoAudioPanGetInfo(
    const omfHdl_t			file,         /* IN - File Handle */
    const omfEffObj_t 		effect,       /* IN - Effect object */
    omfLength_t             *length,      /* OUT - Length of effect */
	omfSegObj_t				*inputSegment,/* OUT - Input segment */
	omfSegObj_t				*ratio);      /* OUT - Ratio */

OMF_EXPORT omfErr_t omfeVideoFadeToBlackNew(
    omfHdl_t 		file,         /* IN - File Handle */
    omfLength_t		length,       /* IN - Effect Length */
	omfSegObj_t		inputSegment, /* IN - Input segment */
	omfSegObj_t		level,        /* IN - Level */
	omfBool			reverse,      /* IN - Whether fade-to (TRUE) or 
								   *      fade-from (FALSE) */
    omfEffObj_t		*newEffect);  /* OUT - New Effect Object */

OMF_EXPORT omfErr_t omfeVideoFadeToBlackGetInfo(
    const omfHdl_t			file,         /* IN - File Handle */
    const omfEffObj_t 		effect,       /* IN - Effect Object */
    omfLength_t             *length,      /* OUT - Length of effect */
	omfSegObj_t				*inputSegment,/* OUT - Input segment */
	omfSegObj_t				*level,       /* OUT - Level */
	omfBool					*reverse,     /* OUT - Reverse */
									      /* OUT - Whether fade-to (TRUE) or 
								           *       fade-from (FALSE) */
	omfUInt32		 		*phaseOffset);/* OUT - Phase Offset */

OMF_EXPORT omfErr_t omfeMonoAudioMixdownNew(
    omfHdl_t 		file,             /* IN - File Handle */
    omfLength_t		length,           /* IN - Effect Length */
	omfInt32		numInputSegments, /* IN - Number of input segments */
	omfSegObj_t		*inputSegments,   /* IN - List of input segments */
    omfEffObj_t		*newEffect);      /* OUT - New Effect Object */

OMF_EXPORT omfErr_t omfeMonoAudioMixdownGetInfo(
    const omfHdl_t				file,         /* IN - File Handle */
    const omfEffObj_t 			effect,       /* IN - Effect Object */
    omfLength_t                 *length,      /* OUT - Length of effect */
	omfSegObj_t					*inputSegments, /* OUT - List of Input Segs */
	omfInt32					maxNumSegments, /* IN - Max number of Segs */
	omfInt32		 			*numSegments);/* OUT - Number of Segments */

OMF_EXPORT omfErr_t omfeSMPTEVideoWipeNew(
    omfHdl_t 		file,          /* IN - File Handle */
    omfLength_t		length,        /* IN - Effect Length */
	omfSegObj_t		inputSegmentA, /* IN - Input segment A */
	omfSegObj_t		inputSegmentB, /* IN - Input segment B */
	omfSegObj_t		level,         /* IN - Level */
	omfInt32		wipeNumber,    /* IN - Wipe Number */
	omfWipeArgs_t	*wipeControls, /* IN - Wipe Controls */
    omfEffObj_t		*newEffect);   /* OUT - New Effect Object */

OMF_EXPORT omfErr_t omfeSMPTEVideoWipeGetInfo(
    const omfHdl_t 		file,         /* IN - File Handle */
    const omfEffObj_t	effect,       /* IN - Effect object */
    omfLength_t         *length,      /* OUT - Length of effect */
	omfSegObj_t	        *inputSegmentA,  /* OUT - Input segment A */
	omfSegObj_t	        *inputSegmentB,  /* OUT - Input segment B */
	omfSegObj_t	        *level,       /* OUT - Level */
	omfInt32		    *wipeNumber,  /* OUT - Wipe Number */
	omfWipeArgs_t	    *wipeControls); /* OUT - Wipe Controls */

/* end of prototypes */

#if PORT_LANG_CPLUSPLUS
}
#endif
#endif /* _OMFI_EFFECTS_H_ */

/* INDENT OFF */
/*
;;; Local Variables: ***
;;; tab-width:4 ***
;;; End: ***
*/

// @doc INTERNAL
// @com This file handles bdirectional translation of effect metadata between OMF and AAF

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
#include <iostream.h>

namespace OMF2
{
#include "omPublic.h"
#include "omMedia.h"
}

// OMF Includes

#include "AafOmf.h"
#include "EffectTranslate.h"
#include "AAFClassDefUIDs.h"
#include "AAFException.h"

#define CHECKAAF
#define CompareUUID(a, b) (memcmp((char *)&a, (char *)&b, sizeof(aafUID_t)) == 0)

// ============================================================================
// GetEffectIDsFromAUID
//
//			This function converts an AAF effectID into an OMF unique name. 
//			
// Returns: AAFRESULT_SUCCESS if datakind is converted succesfully
//			
// NOTE :	the buffer passed to this routine should be big enough to accomodate
//			the whole OMF name.!!
// ============================================================================

typedef struct
{
	const aafUID_t	*aafID;
	const char		*omfEffectID;
	const char		*MCEffectID;
} effectXlate_t;

	effectXlate_t xlateTable[] = {
	{ &kAAFEffectVideoDissolve, "SimpleVideoDissolve", "BLEND_DISSOLVE" },
	{ &kAAFEffectVideoFadeToBlack, "VideoFadeToBlack", "BLEND_FADE_DOWN" },
	{ &kAAFEffectVideoSpeedControl, "VideoSpeedControl", "EFF_TIMEWARP.MOTION_CTL" },
	{ &kAAFEffectVideoRepeat, "VideoRepeat","EFF_TIMEWARP.REPEAT" },
	{ &kAAFEffectVideoFrameToMask, "VideoFrameMask","EFF_TIMEWARP.CAPTURE_MASK" },
	{ &kAAFEffectMonoAudioDissolve, "SimpleMonoAudioDissolve","BLEND_AUDIO_DISSOLVE" },
	{ &kAAFEffectMonoAudioPan, "MonoAudioPan","Audio Pan Volume" },
	{ &kAAFEffectMonoAudioGain, "MonoAudioGain","Audio Pan Volume" },
	{ &kAAFEffectStereoAudioDissolve, "StereoAudioDissolve" },
	{ &kAAFEffectStereoAudioGain, "StereoAudioGain" },
	{ &kAAFEffectSMPTEVideoWipe, "SMPTEVideoWipe", NULL },
	{ &kEffBlendPIPUUID, NULL, "EFF_BLEND_PIP" },
	{ &kEffBlendSuperUUID, NULL, "BLEND_SUPER" },
	{ &kEffBlendFadeUpUUID, NULL, "BLEND_FADE_UP" },
	{ &kEffBlendDipUUID, NULL, "BLEND_DIP" },
	{ &kEffXRotate, NULL, "EFF_BLEND_ROTATE_XROTATE" },
	{ &kEffYRotate, NULL,"EFF_BLEND_ROTATE_YROTATE" },
	{ &kEffZRotate, NULL,"EFF_BLEND_ROTATE_ZROTATE" },
	{ &kEffSqueezeZoom, NULL,"EFF_BLEND_SQUEEZE_ZOOM" },
	{ &kEffSqueezeHorz, NULL,"EFF_BLEND_SQUEEZE_HORZ" },
	{ &kEffSqueezeVert, NULL,"EFF_BLEND_SQUEEZE_VERT" },
	{ &kEffSqueezeBottomCenter, NULL,"EFF_BLEND_SQUEEZE_BOTTOMCENTER" },
	{ &kEffSqueezeRightCenter, NULL,"EFF_BLEND_SQUEEZE_RIGHTCENTER" },
	{ &kEffSqueezeTopCenter, NULL,"EFF_BLEND_SQUEEZE_TOPCENTER" },
	{ &kEffSqueezeLeftCenter, NULL,"EFF_BLEND_SQUEEZE_LEFTCENTER" },
	{ &kEffSqueezeTop, NULL,"EFF_BLEND_SQUEEZETOP" },
	{ &kEffSqueezeBottom, NULL,"EFF_BLEND_SQUEEZEBOTTOM" },
	{ &kEffSqueezeLeft, NULL,"EFF_BLEND_SQUEEZELEFT" },
	{ &kEffSqueezeRight, NULL,"EFF_BLEND_SQUEEZERIGHT" },
	{ &kEffSqueezeBottomLeft, NULL,"EFF_BLEND_SQUEEZE_BOTTOMLEFT" },
	{ &kEffSqueezeTopRight, NULL,"EFF_BLEND_SQUEEZE_TOPRIGHT" },
	{ &kEffSqueezeBottomRight, NULL,"EFF_BLEND_SQUEEZE_BOTTOMRIGHT" },
	{ &kEffSqueezeTopLeft, NULL,"EFF_BLEND_SQUEEZE_TOPLEFT" },
	{ &kEffAudioPanvol, NULL,"EFF_AUDIO_PANVOL" },
	{ &kEffAudioEQ, NULL,"EFF_AUDIO_EQMB" },
	{ &kEffAudioSuite, NULL,"EFF_AUDIO_AS_PLUG_IN" },
	{ &kEffTimeWarp, NULL,"EFF_TIMEWARP" },
	{ &kEffMotionStrobe, NULL,"EFF_TIMEWARP.STROBE" },
	{ &kEffAudioWarp, NULL,"EFF_TIMEWARP.AUDIO_TIME_WARP" },
	{ &kEffMask166, NULL,"EFF_BLEND_MASK_166" },
	{ &kEffMask185, NULL,"EFF_BLEND_MASK_185" },
	{ &kEffMask235, NULL,"EFF_BLEND_MASK_235" },
	{ &kEffMask177, NULL,"EFF_BLEND_MASK_177" },
	{ &kEffVDissolveLin, NULL,"BLEND_DISSOLVE_LIN" },
	{ &kEffVDissolveLog, NULL,"BLEND_DISSOLVE_LOG" },
	{ &kEffBlowUp, NULL,"EFF_BLOW_UP" },
	{ &kEffBlendMask, NULL,"EFF_BLEND_MASK" },
	{ &kEffColorCorrect, NULL,"EFF_COLORCORRECT" },
	{ &kEffRGBColorCorrect, NULL, "EFF_RGB_COLOR_CORRECTION" },
	{ &kEffFlipVert, NULL,"EFF_BLEND_FLIP_VERT" },
	{ &kEffFlipBoth, NULL,"EFF_BLEND_FLIP_BOTH" },
	{ &kEffFlipHoriz, NULL,"EFF_BLEND_FLIP_HORZ" },
	{ &kEffMaskImage, NULL,"EFF_BLEND_MASK_IMAGE" },
	{ &kEffPaint, NULL,"EFF_PAINT" },
	{ &kEffPaintMosaic, NULL,"EFF_PAINT_MOSAIC" },
	{ &kEffPaintSpotColor, NULL,"EFF_PAINT_SPOT_COLOR" },
	{ &kEffPaintMPEGOutline, NULL,"EFF_PAINT_MPEG_OUTLINE" },
	{ &kEffPaintScratch, NULL,"EFF_PAINT_SCRATCH" },
	{ &kEffPaintResize, NULL,"EFF_BLEND_RESIZE" },
	{ &kEffPanScan, NULL,"EFF_PAN_SCAN" },
	{ &kEffSubmaster, NULL,"EFF_SUBMASTER" },
	{ &kEffAnimatte, NULL,"EFF_ANIMATTE" },
	{ &kEffLumaKey, NULL,"EFF_BLEND_LUMAKEY" },
	{ &kEffMatteKey, NULL,"EFF_BLEND_MATTE_KEY" },
	{ &kEffYUVChromaKey, NULL,"EFF_BLEND_YUV_CHROMAKEY" },
	{ &kEffSimpleLumaKey, NULL,"EFF_BLEND_SIMPLE_LUMAKEY" },
	{ &kEffSimpleChromaKey, NULL,"EFF_BLEND_SIMPLE_CHROMAKEY" },
	{ &kEffChromaKey, NULL,"EFF_BLEND_CHROMAKEY" },
	{ &kEffToucanChromaKey, NULL,"EFF_BLEND_TOUCAN_CHROMAKEY" },
	{ &kEff2DMatteKey, NULL,"EFF_2DMATTE" },
	{ &kEffConceal, NULL,"EFF_BLEND_CONCEAL" },
	{ &kEffLConceal, NULL,"EFF_BLEND_LCONCEAL" },
	{ &kEffPeel, NULL,"EFF_BLEND_PEEL" },
	{ &kEffPush, NULL,"EFF_BLEND_PUSH" },
	{ &kEffSBlend, NULL,"EFF_SBLEND" },
	{ &kEff3DPIP, NULL,"EFF_3DPIP" },
	{ &kEff3DPageCurl, NULL,"EFF_3DSHAPE_PAGE_CURL" },
	{ &kEff3DQuadCurl, NULL,"EFF_3DSHAPE_QUAD_CURL" },
	{ &kEff3DShapeArrow, NULL,"EFF_3DSHAPE_ARROW" },
	{ &kEff3DShapeSlats, NULL,"EFF_3DSHAPE_SLATS" },
	{ &kEff3DShapeMultiWave, NULL,"EFF_3DSHAPE_MULTI_WAVE" },
	{ &kEff3DShapeCenterBurst, NULL,"EFF_3DSHAPE_CENTER_BURST" },
	{ &kEff3DShapeSineWave, NULL,"EFF_3DSHAPE_SINE_WAVE" },
	{ &kEff3DShapeBall, NULL,"EFF_3DSHAPE_BALL" },
	{ &kEff3DShapePageFold, NULL,"EFF_3DSHAPE_PAGE_FOLD" },
	{ &kEff3DShapeBumps, NULL,"EFF_3DSHAPE_BUMPS" },
	{ &kEff3DTitle, NULL,"EFF_3DTITLE" },
	{ &kEff3DMatteKey, NULL,"EFF_3DMATTE" },
	{ &kEffRollingTitle, NULL,"EFF_ROLLING_TITLE" },
	{ &kEffRolling2dMatte, NULL,"EFF_ROLLING_2DMATTE" },
	{ &kEffCrawlingTitle, NULL,"EFF_CRAWLING_TITLE" },
	{ &kEffCrawling2DMatte, NULL,"EFF_CRAWLING_2DMATTE" },
	{ &kEffTestOneTrack, NULL,"EFF_TEST_ONE_TRACK" },
	{ &kEffTestTwoTrack, NULL,"EFF_TEST_TWO_TRACK" },
	{ &kEffTestThreeTrack, NULL,"EFF_TEST_THREE_TRACK" }
	};

#define TAKES_LEVEL				0x00000001
#define TAKES_BORDER_WIDTH		0x00000002
#define TAKES_BORDER_SOFT		0x00000004
#define TAKES_XPOS				0x00000008
#define TAKES_YPOS				0x00000010
#define TAKES_CROP				0x00000020
#define TAKES_SCALE				0x00000040
#define TAKES_SPILL_SUPRESS		0x00000080
#define TAKES_BORDERS			0x00000100
#define TAKES_COLOR				0x00000200
#define TAKES_REVERSE			0x00000400
//#define TAKES_SCALE				0x00000800
//#define TAKES_SCALE				0x00001000

typedef struct
{
	const aafUID_t	*effectID;
	aafUInt32		mask;
} capabilityXlate_t;

static capabilityXlate_t capabilityTable[] = {
	{ &kAAFEffectVideoDissolve, TAKES_LEVEL },
//	{ &kAAFEffectVideoFadeToBlack, xxxx },
//	{ &kAAFEffectVideoSpeedControl, xxxx },
//	{ &kAAFEffectVideoRepeat, xxxx },
//	{ &kAAFEffectVideoFrameToMask, xxxx },
//	{ &kAAFEffectMonoAudioDissolve, xxxx },
//	{ &kAAFEffectMonoAudioPan, xxxx },
//	{ &kAAFEffectMonoAudioGain, xxxx },
//	{ &kAAFEffectStereoAudioDissolve, xxxx },
//	{ &kAAFEffectStereoAudioGain, xxxx },
	{ &kAAFEffectSMPTEVideoWipe, TAKES_LEVEL | TAKES_REVERSE }
//	{ &kEffBlendPIPUUID, xxxx },
//	{ &kEffBlendSuperUUID, xxxx },
//	{ &kEffBlendFadeUpUUID, xxxx },
//	{ &kEffBlendDipUUID, xxxx },
//	{ &kEffXRotate, xxxx },
//	{ &kEffYRotate, xxxx },
//	{ &kEffZRotate, xxxx },
//	{ &kEffSqueezeZoom, xxxx },
//	{ &kEffSqueezeHorz, xxxx },
//	{ &kEffSqueezeVert, xxxx },
//	{ &kEffSqueezeBottomCenter, xxxx },
//	{ &kEffSqueezeRightCenter, xxxx },
//	{ &kEffSqueezeTopCenter, xxxx },
//	{ &kEffSqueezeLeftCenter, xxxx },
//	{ &kEffSqueezeTop, xxxx },
//	{ &kEffSqueezeBottom, xxxx },
//	{ &kEffSqueezeLeft, xxxx },
//	{ &kEffSqueezeRight, xxxx },
//	{ &kEffSqueezeBottomLeft, xxxx },
//	{ &kEffSqueezeTopRight, xxxx },
//	{ &kEffSqueezeBottomRight, xxxx },
//	{ &kEffSqueezeTopLeft, xxxx },
//	{ &kEffAudioPanvol, xxxx },
//	{ &kEffAudioEQ, xxxx },
//	{ &kEffAudioSuite, xxxx },
//	{ &kEffTimeWarp, xxxx },
//	{ &kEffMotionStrobe, xxxx },
//	{ &kEffAudioWarp, xxxx },
//	{ &kEffMask166, xxxx },
//	{ &kEffMask185, xxxx },
//	{ &kEffMask235, xxxx },
//	{ &kEffMask177, xxxx },
//	{ &kEffVDissolveLin, xxxx },
//	{ &kEffVDissolveLog, xxxx },
//	{ &kEffBlowUp, xxxx },
//	{ &kEffBlendMask, xxxx },
//	{ &kEffColorCorrect, xxxx },
//	{ &kEffRGBColorCorrect, xxxx },
//	{ &kEffFlipVert, xxxx },
//	{ &kEffFlipBoth, xxxx },
//	{ &kEffFlipHoriz, xxxx },
//	{ &kEffMaskImage, xxxx },
//	{ &kEffPaint, xxxx },
//	{ &kEffPaintMosaic, xxxx },
//	{ &kEffPaintSpotColor, xxxx },
//	{ &kEffPaintMPEGOutline, xxxx },
//	{ &kEffPaintScratch, xxxx },
//	{ &kEffPaintResize, xxxx },
//	{ &kEffPanScan, xxxx },
//	{ &kEffSubmaster, xxxx },
//	{ &kEffAnimatte, xxxx },
//	{ &kEffLumaKey, xxxx },
//	{ &kEffMatteKey, xxxx },
//	{ &kEffYUVChromaKey, xxxx },
//	{ &kEffSimpleLumaKey, xxxx },
//	{ &kEffSimpleChromaKey, xxxx },
//	{ &kEffChromaKey, xxxx },
//	{ &kEffToucanChromaKey, xxxx },
//	{ &kEff2DMatteKey, xxxx },
//	{ &kEffConceal, xxxx },
//	{ &kEffLConceal, xxxx },
//	{ &kEffPeel, xxxx },
//	{ &kEffPush, xxxx },
//	{ &kEffSBlend, xxxx },
//	{ &kEff3DPIP, xxxx },
//	{ &kEff3DPageCurl, xxxx },
//	{ &kEff3DQuadCurl, xxxx },
//	{ &kEff3DShapeArrow, xxxx },
//	{ &kEff3DShapeSlats, xxxx },
//	{ &kEff3DShapeMultiWave, xxxx },
//	{ &kEff3DShapeCenterBurst, xxxx },
//	{ &kEff3DShapeSineWave, xxxx },
//	{ &kEff3DShapeBall, xxxx },
//	{ &kEff3DShapePageFold, xxxx },
//	{ &kEff3DShapeBumps, xxxx },
//	{ &kEff3DTitle, xxxx },
//	{ &kEff3DMatteKey, xxxx },
//	{ &kEffRollingTitle, xxxx },
//	{ &kEffRolling2dMatte, xxxx },
//	{ &kEffCrawlingTitle, xxxx },
//	{ &kEffCrawling2DMatte, xxxx },
//	{ &kEffTestOneTrack, xxxx },
//	{ &kEffTestTwoTrack, xxxx },
//	{ &kEffTestThreeTrack, xxxx }
	};

bool EffectCapabilityPresent(aafUID_t &effectUID, aafUInt32 capabilityMask)
{
	bool	foundID = false, result = false;
	aafUInt32	n, numEntries = sizeof(capabilityTable)/sizeof(capabilityXlate_t);
	for(n = 0; (n < numEntries) && !foundID; n++)
	{
		if(memcmp(&effectUID, capabilityTable[n].effectID, sizeof(aafUID_t)) == 0)
		{
			foundID = true;
			result = ((capabilityTable[n].mask & capabilityMask) == capabilityMask);
		}
	}

	return(result);
}

typedef struct
{
	aafUInt32	wipeCode;
	char		*effectName;
} wipeTable_t;

static wipeTable_t wipeTable[] = {
	{ 24, "EFF_BLEND_WIPE_RBOX" },
	{ 23, "EFF_BLEND_WIPE_TBOX" },
	{ 26, "EFF_BLEND_WIPE_LBOX_ID" },
	{ 25, "EFF_BLEND_WIPE_BBOX_ID" },
	{ 5, "EFF_BLEND_WIPE_LR_ID" },
	{ 6, "EFF_BLEND_WIPE_LL_ID" },
	{ 4, "EFF_BLEND_WIPE_UR_ID" },
	{ 3, "EFF_BLEND_WIPE_UL_ID" },
	{ 21, "EFF_BLEND_WIPE_VOPEN_ID" },
	{ 22, "EFF_BLEND_WIPE_HOPEN_ID" },
	{ 1, "EFF_BLEND_WIPE_VERT_ID" },
	{ 2, "EFF_BLEND_WIPE_HORZ_ID" },
	{ 41, "EFF_BLEND_WIPE_LRLDIAG_ID" },
	{ 42, "EFF_BLEND_WIPE_LLRDIAG_ID" },
	{ 42, "EFF_BLEND_WIPE_URLDIAG_ID" },
	{ 41, "EFF_BLEND_WIPE_ULRDIAG_ID" },
	{ 317, "EFF_BLEND_WIPE_SPIRAL_ID" },
	{ 8, "EFF_BLEND_WIPE_GRID_ID" },
	{ 352, "EFF_BLEND_WIPE_1ROW_ID" },
	{ 409, "EFF_BLEND_WIPE_SPECKLE_ID" },
	{ 301, "EFF_BLEND_WIPE_ZIG_ZAG_ID" },
	{ 71, "EFF_BLEND_WIPE_VOPEN_SAW_ID" },
	{ 72, "EFF_BLEND_WIPE_HOPEN_SAW_ID" },
	{ 73, "EFF_BLEND_WIPE_VSAW_ID" },
	{ 74, "EFF_BLEND_WIPE_HSAW_ID" },
	{ 7, "EFF_BLEND_WIPE_4CORNER_ID" },
	{ 101, "EFF_BLEND_WIPE_BOX_ID" },
	{ 119, "EFF_BLEND_WIPE_CIRCLE_ID" },
	{ 201, "EFF_BLEND_WIPE_RADAR_ID" },
	{ 102, "EFF_BLEND_WIPE_DIAMOND_ID" },
	{ 120, "EFF_BLEND_WIPE_ELLIPSE_ID" }
};

HRESULT GetEffectIDs(IAAFOperationGroup *effect,
									   OMF2::omfUniqueNamePtr_t effectID,
									   OMF2::omfUniqueNamePtr_t MCEffectID)
{
	IAAFOperationDef		*pOpDef = NULL;
	IAAFDefObject			*pDef = NULL;
	IAAFParameter			*pParameter = NULL;
	IAAFConstantValue		*pCVal = NULL;
	long					n, numEntries = sizeof(xlateTable)/sizeof(effectXlate_t);
	long					x, numWipeEntries = sizeof(wipeTable)/sizeof(wipeTable_t);
	aafUInt32				wipeNumber;
	aafUInt32				bytesRead;
	bool					found = false;
	aafUID_t				AAFEffectID, direction;

	try
	{
		AAFCheck rc;
		rc = effect->GetOperationDefinition(&pOpDef);
		rc = pOpDef->QueryInterface(IID_IAAFDefObject, (void **) &pDef);
		rc = pDef->GetAUID(&AAFEffectID);
		for(n = 0; (n < numEntries) && !found; n++)
		{
			if ( CompareUUID(AAFEffectID, *(xlateTable[n].aafID)))
			{
				if(xlateTable[n].omfEffectID != NULL)
				{
					strcpy(effectID, "omfi:effect:");
					strcat(effectID, xlateTable[n].omfEffectID);
				}
				else if(xlateTable[n].MCEffectID != NULL)
					strcpy(effectID, "OMFI:EFFE:AvidPrivateEffectID");
				
				if(xlateTable[n].MCEffectID != NULL)
					strcpy(MCEffectID, xlateTable[n].MCEffectID);
				else
					MCEffectID[0] = '\0';

				if (CompareUUID(effectID, kEffConceal) ||
				    CompareUUID(effectID, kEffLConceal) ||
				    CompareUUID(effectID, kEffPeel) ||
				    CompareUUID(effectID, kEffPush))
				{
					(void)(effect->GetParameterByArgID(kAAFParamID_AvidDirection,
														&pParameter));
					if(pParameter->QueryInterface(IID_IAAFConstantValue, (void **) &pCVal) == AAFRESULT_SUCCESS)
					{
						// Determine effect ID from direction
						(void)(pCVal->GetValue(sizeof(direction),(unsigned char*)&direction,&bytesRead));
						// if bytesRead != sizeof(direction) error
						if (CompareUUID(direction, kEffDirBottomLeft))
							strcat(MCEffectID, "_BOTTOM_LEFT");
						else if (CompareUUID(direction, kEffDirBottomRight))
							strcat(MCEffectID, "_BOTTOM_RIGHT");
						else if (CompareUUID(direction, kEffDirTopRight))
							strcat(MCEffectID, "_TOP_RIGHT");
						else if (CompareUUID(direction, kEffDirTopLeft))
							strcat(MCEffectID, "_TOP_LEFT");
						else if (CompareUUID(direction, kEffDirTop))
							strcat(MCEffectID, "_TOP");
						else if (CompareUUID(direction, kEffDirBottom))
							strcat(MCEffectID, "_BOTTOM");
						else if (CompareUUID(direction, kEffDirLeft))
							strcat(MCEffectID, "_LEFT");
						else if (CompareUUID(direction, kEffDirRight))
							strcat(MCEffectID, "_RIGHT");
						// else error
						pCVal->Release();
						pCVal = NULL;
					}
					// else error

					pParameter->Release();
					pParameter = NULL;
				}
				else if (CompareUUID(effectID, kAAFEffectSMPTEVideoWipe))
				{
					(void)(effect->GetParameterByArgID(kAAFParameterDefSMPTEWipeNumber,
														&pParameter));
					if(pParameter->QueryInterface(IID_IAAFConstantValue, (void **) &pCVal) == AAFRESULT_SUCCESS)
					{
						// Determine effect ID from direction
						(void)(pCVal->GetValue(sizeof(wipeNumber),(unsigned char*)&wipeNumber,&bytesRead));
						// if bytesRead != sizeof(wipeNumber) error
						// Determine effectID from wipe code	
						bool foundWipe = false;
						for(x = 0; (n < numWipeEntries) && !foundWipe; x++)
						{
							if (wipeNumber == wipeTable[x].wipeCode)
							{							
								if(wipeTable[x].effectName != NULL)
									strcpy(MCEffectID, wipeTable[x].effectName);
								// else error
								foundWipe = true;
							}
						}
						pCVal->Release();
						pCVal = NULL;
					}
					// else error

					pParameter->Release();
					pParameter = NULL;
				}

				found = true;
			}
		}
		
		if(!found)
		{
			//		AUIDtoString(&Datadef, szAUID);
			//		printf("%sInvalid DataDef Found in sequence AUID : %s\n", gpGlobals->indentLeader, szAUID);
			//		fprintf(stderr,"%sInvalid DataDef Found in sequence AUID : %s\n", gpGlobals->indentLeader, szAUID);
			rc = AAFRESULT_INVALID_DATADEF;
		}
	} // end try
	catch( ExceptionBase &e )
	{
		return e.Code();
	}

	return AAFRESULT_SUCCESS;
}

HRESULT GetAAFEffectID(	OMF2::omfUniqueNamePtr_t OMFEffectIDPtr,
						OMF2::omfUniqueNamePtr_t MCEffectIDPtr,
						aafUID_t	*aafUID)
{
	IAAFOperationDef		*pOpDef = NULL;
	IAAFDefObject			*pDef = NULL;
	IAAFParameter			*pParameter = NULL;
	IAAFConstantValue		*pCVal = NULL;
	HRESULT					rc = AAFRESULT_SUCCESS;
	long					n, numEntries = sizeof(xlateTable)/sizeof(effectXlate_t);
	bool					found = false;
	OMF2::omfUniqueName_t	OMFEffectID;
	OMF2::omfUniqueName_t	MCEffectID;
	char					*dirStr;

	strcpy(OMFEffectID, OMFEffectIDPtr);
	if(MCEffectIDPtr != NULL)
		strcpy(MCEffectID, MCEffectIDPtr);
	else
		MCEffectID[0] = '\0';
	if (strncmp(MCEffectID, "EFF_BLEND_CONCEAL", 17) == 0 ||
		strncmp(MCEffectID, "EFF_BLEND_LCONCEAL", 18) == 0 ||
		strncmp(MCEffectID, "EFF_BLEND_PEEL", 14) == 0 ||
		strncmp(MCEffectID, "EFF_BLEND_PUSH", 14) == 0 )
	{
		// Truncate off direction part of the effect ID before convert
		dirStr = strstr(MCEffectID, "_BOTTOM_LEFT");
		if(dirStr != NULL)
			*dirStr = '\0';	
		dirStr = strstr(MCEffectID, "_BOTTOM_RIGHT");
		if(dirStr != NULL)
			*dirStr = '\0';	
		dirStr = strstr(MCEffectID, "_TOP_RIGHT");
		if(dirStr != NULL)
			*dirStr = '\0';	
		dirStr = strstr(MCEffectID, "_TOP_LEFT");
		if(dirStr != NULL)
			*dirStr = '\0';	
		dirStr = strstr(MCEffectID, "_TOP");
		if(dirStr != NULL)
			*dirStr = '\0';	
		dirStr = strstr(MCEffectID, "_BOTTOM");
		if(dirStr != NULL)
			*dirStr = '\0';	
		dirStr = strstr(MCEffectID, "_LEFT");
		if(dirStr != NULL)
			*dirStr = '\0';	
		dirStr = strstr(MCEffectID, "_RIGHT");
		if(dirStr != NULL)
			*dirStr = '\0';	
	}
	else if(strncmp(MCEffectID, "EFF_BLEND_WIPE", 14) == 0)
	{
		*aafUID = kAAFEffectSMPTEVideoWipe;
		found = true;
	}


	for(n = 0; (n < numEntries) && !found; n++)
	{
		if(xlateTable[n].omfEffectID != NULL)
			found = (strcmp(OMFEffectID, xlateTable[n].omfEffectID) == 0);
		else
			found = (strcmp(MCEffectID, xlateTable[n].MCEffectID) == 0);
		if (found)
		{
			*aafUID = *(xlateTable[n].aafID);			
		}
	}
	if(!found)
	{
		//		AUIDtoString(&Datadef, szAUID);
		//		printf("%sInvalid DataDef Found in sequence AUID : %s\n", gpGlobals->indentLeader, szAUID);
		//		fprintf(stderr,"%sInvalid DataDef Found in sequence AUID : %s\n", gpGlobals->indentLeader, szAUID);
		rc = AAFRESULT_INVALID_DATADEF;
	}

	return rc;
}

IAAFInterpolationDef *CreateInterpolationDefinition(IAAFDictionary *dict, aafUID_t interpolationDefID)
{
	IAAFInterpolationDef	*interpDef;
	IAAFDefObject			*defObject;
	AAFRESULT				rc;

	rc = dict->LookupInterpolationDefinition(interpolationDefID,&interpDef);
	if(rc == AAFRESULT_SUCCESS && interpDef != NULL)
		return interpDef;

//	dprintf("AEffect::CreateInterpolationDefinition()\n");	//JeffB:

	(void)(dict->CreateInstance(kAAFClassID_InterpolationDefinition,
			IID_IAAFInterpolationDef,
			(IUnknown **)&interpDef));
	(void)(interpDef->QueryInterface(IID_IAAFDefObject, (void **) &defObject));
	if(memcmp(&interpolationDefID, &LinearInterpolator, sizeof(aafUID_t)) == 0)
	{
 		(void)(defObject->Initialize(interpolationDefID, L"LinearInterp", L"Linear keyframe interpolation"));
		dict->RegisterInterpolationDefinition(interpDef);
	}
	else
	{
		interpDef->Release();
		interpDef = NULL;
	}

	defObject->Release();

//cleanup:
	return(interpDef);
}

static void Fixed16toAAFRational(AvFixed16 in, aafRational_t &out)
{
	out.numerator = in;
	out.denominator = ONE_FIXED16;	
}

static void Fixed30toAAFRational(AvFixed16 in, aafRational_t &out)
{
	out.numerator = in;
	out.denominator = ONE_FIXED30;
}

// Break down the keyframe data into separate tracks
void ExportSeparateKeyframeData(aafUID_t &effectUID,
								aafInt32 numKF,
								OMFIPvtKFInfo_t **keyframes,
								aafRational_t *times,
								IAAFDictionary		*pDict,
								IAAFOperationGroup* pOutputEffect)
{	
	aafRational_t		ratVal;
	long				intVal, initialValue;
	unsigned long		initialLevel;
	bool				constant, optimizeConstant = true;	// May set later on a per effectDef basis
	short				i;
	IAAFVaryingValue	*pVVal = NULL;
	IAAFParameter		*pParm = NULL;
	IAAFParameterDef	*parmDef = NULL;
	IAAFTypeDef			*typeDef = NULL;
//	AAFRESULT			rc;

	if(EffectCapabilityPresent(effectUID, TAKES_LEVEL))
	{
		initialLevel = keyframes[0]->level;	
		for (i = 0, constant = TRUE; i < numKF; i++)
		{
			if(initialLevel != keyframes[i]->level)
				constant = FALSE;
		}
		
		typeDef = CreateTypeDefinition(pDict, kAAFTypeID_Rational);
		if(constant & optimizeConstant)
		{
			Fixed30toAAFRational(keyframes[0]->level, ratVal);
			pParm = AAFAddConstantVal(pDict, sizeof(ratVal), &ratVal, pOutputEffect);
		}
		else // export as VaryingValue
		{
			pVVal = AAFAddEmptyVaryingVal(pDict, pOutputEffect);
			for (i = 0; i < numKF; i++)
			{
				Fixed30toAAFRational(keyframes[i]->level, ratVal);
				AAFAddOnePoint(pDict, times[i], sizeof(ratVal), &ratVal,
					typeDef, pVVal);
			}
			CHECKAAF(pVVal->QueryInterface(IID_IAAFParameter, (void **) &pParm));
			pVVal->Release();
			pVVal = NULL;
		}
		parmDef = CreateParameterDefinition(pDict, kAAFParameterDefLevel);
		CHECKAAF(pParm->SetParameterDefinition(parmDef));
		parmDef->Release();
		parmDef = NULL;
		
		CHECKAAF(pParm->SetTypeDefinition(typeDef));
		typeDef->Release();
		typeDef = NULL;
		pParm->Release();
		pParm = NULL;
	}
	
	if(EffectCapabilityPresent(effectUID, TAKES_BORDER_WIDTH))
	{
		initialValue = keyframes[0]->borderWidth;
		for (i = 0, constant = TRUE; i < numKF; i++)
		{
			if(initialValue != keyframes[i]->borderWidth)
				constant = FALSE;
		}
		
		typeDef = CreateTypeDefinition(pDict, kAAFTypeID_Int32);
		if(constant & optimizeConstant)
		{
			intVal = keyframes[0]->borderWidth;
			pParm = AAFAddConstantVal(pDict, sizeof(intVal), &intVal, pOutputEffect);
		}
		else
		{
			pVVal = AAFAddEmptyVaryingVal(pDict, pOutputEffect);
			for (i = 0; i < numKF; i++)
			{
				intVal = keyframes[i]->borderWidth;
				AAFAddOnePoint(pDict, times[i], sizeof(intVal),
					&intVal, typeDef, pVVal);
			}
			CHECKAAF(pVVal->QueryInterface(IID_IAAFParameter, (void **) &pParm));
			pVVal->Release();
			pVVal = NULL;
		}
		
		parmDef = CreateParameterDefinition(pDict, kAAFParamID_AvidBorderWidth);
		CHECKAAF(pParm->SetParameterDefinition(parmDef));
		parmDef->Release();
		parmDef = NULL;
		
		CHECKAAF(pParm->SetTypeDefinition(typeDef));
		typeDef->Release();
		typeDef = NULL;
		pParm->Release();
		pParm = NULL;
	}
	
	if(EffectCapabilityPresent(effectUID, TAKES_BORDER_SOFT))
	{
		initialValue = keyframes[0]->borderSoft;
		for (i = 0, constant = TRUE; i < numKF; i++)
		{
			if(initialValue != keyframes[i]->borderSoft)
				constant = FALSE;
		}
		
		typeDef = CreateTypeDefinition(pDict, kAAFTypeID_Int32);
		if(constant && optimizeConstant)
		{
			intVal = keyframes[0]->borderSoft;
			pParm = AAFAddConstantVal(pDict, sizeof(intVal), &intVal, pOutputEffect);
		}
		else
		{
			pVVal = AAFAddEmptyVaryingVal(pDict, pOutputEffect);
			for (i = 0; i < numKF; i++)
			{
				intVal = keyframes[i]->borderSoft;
				AAFAddOnePoint(pDict, times[i], sizeof(intVal),
					&intVal, typeDef, pVVal);
			}
			CHECKAAF(pVVal->QueryInterface(IID_IAAFParameter, (void **) &pParm));
			pVVal->Release();
			pVVal = NULL;
		}
		
		parmDef = CreateParameterDefinition(pDict, kAAFParamID_AvidBorderSoft);
		CHECKAAF(pParm->SetParameterDefinition(parmDef));
		parmDef->Release();
		parmDef = NULL;
		
		CHECKAAF(pParm->SetTypeDefinition(typeDef));
		typeDef->Release();
		typeDef = NULL;
		pParm->Release();
		pParm = NULL;
	}
	
	
	if(EffectCapabilityPresent(effectUID, TAKES_XPOS))
	{
		AvFixed16	initialPos, initialFloor, initialCeil;
		
		initialPos = keyframes[0]->posX;
		initialFloor = keyframes[0]->XFloor;
		initialCeil = keyframes[0]->XCeiling;
		for (i = 0, constant = TRUE; i < numKF; i++)
		{
			if((initialPos != keyframes[i]->posX) ||
				(initialFloor != keyframes[i]->XFloor) ||
				(initialCeil != keyframes[i]->XCeiling))
				constant = FALSE;
		}
		
		typeDef = CreateTypeDefinition(pDict, kAAFTypeID_AvidPosition);
		if(constant && optimizeConstant)
		{
			AAFPvtPosExport pos;
			Fixed16toAAFRational(keyframes[0]->posX, pos.pos);
			Fixed16toAAFRational(keyframes[0]->XFloor, pos.floor);
			Fixed16toAAFRational(keyframes[0]->XCeiling, pos.ceiling);
			pParm = AAFAddConstantVal(pDict, sizeof(pos), &pos, pOutputEffect);
		}
		else
		{
			pVVal = AAFAddEmptyVaryingVal(pDict, pOutputEffect);
			for (i = 0; i < numKF; i++)
			{
				AAFPvtPosExport pos;
				Fixed16toAAFRational(keyframes[i]->posX, pos.pos);
				Fixed16toAAFRational(keyframes[i]->XFloor, pos.floor);
				Fixed16toAAFRational(keyframes[i]->XCeiling, pos.ceiling);
				AAFAddOnePoint(pDict, times[i], sizeof(pos),
					&pos, typeDef, pVVal);
			}
			CHECKAAF(pVVal->QueryInterface(IID_IAAFParameter, (void **) &pParm));
			pVVal->Release();
			pVVal = NULL;
		}
		
		parmDef = CreateParameterDefinition(pDict, kAAFParamID_AvidXPos);
		CHECKAAF(pParm->SetParameterDefinition(parmDef));
		parmDef->Release();
		parmDef = NULL;
		
		CHECKAAF(pParm->SetTypeDefinition(typeDef));
		typeDef->Release();
		typeDef = NULL;
		pParm->Release();
		pParm = NULL;
	}
	
	if(EffectCapabilityPresent(effectUID, TAKES_YPOS))
	{
		AvFixed16	initialPos, initialFloor, initialCeil;
		
		initialPos = keyframes[0]->posY;
		initialFloor = keyframes[0]->YFloor;
		initialCeil = keyframes[0]->YCeiling;
		for (i = 0, constant = TRUE; i < numKF; i++)
		{
			if((initialPos != keyframes[i]->posY) ||
				(initialFloor != keyframes[i]->YFloor) ||
				(initialCeil != keyframes[i]->YCeiling))
				constant = FALSE;
		}
		
		typeDef = CreateTypeDefinition(pDict, kAAFTypeID_AvidPosition);
		if(constant && optimizeConstant)
		{
			AAFPvtPosExport pos;
			Fixed16toAAFRational(keyframes[0]->posY, pos.pos);
			Fixed16toAAFRational(keyframes[0]->YFloor, pos.floor);
			Fixed16toAAFRational(keyframes[0]->YCeiling, pos.ceiling);
			pParm = AAFAddConstantVal(pDict, sizeof(pos), &pos, pOutputEffect);
		}
		else
		{
			pVVal = AAFAddEmptyVaryingVal(pDict, pOutputEffect);
			for (i = 0; i < numKF; i++)
			{
				AAFPvtPosExport pos;
				Fixed16toAAFRational(keyframes[i]->posY, pos.pos);
				Fixed16toAAFRational(keyframes[i]->YFloor, pos.floor);
				Fixed16toAAFRational(keyframes[i]->YCeiling, pos.ceiling);
				AAFAddOnePoint(pDict, times[i], sizeof(pos),
					&pos, typeDef, pVVal);
			}
			CHECKAAF(pVVal->QueryInterface(IID_IAAFParameter, (void **) &pParm));
			pVVal->Release();
			pVVal = NULL;
		}
		
		parmDef = CreateParameterDefinition(pDict, kAAFParamID_AvidYPos);
		CHECKAAF(pParm->SetParameterDefinition(parmDef));
		parmDef->Release();
		parmDef = NULL;
		
		CHECKAAF(pParm->SetTypeDefinition(typeDef));
		typeDef->Release();
		typeDef = NULL;
		pParm->Release();
		pParm = NULL;
	}
	
	if(EffectCapabilityPresent(effectUID, TAKES_CROP))
	{
		AvFixed16	initialLeft, initialTop, initialRight, initialBottom;
		AAFPvtCropExport	rect;
		
		initialLeft = keyframes[0]->cropLeft;
		initialTop = keyframes[0]->cropTop;
		initialRight = keyframes[0]->cropRight;
		initialBottom = keyframes[0]->cropBottom;
		for (i = 0, constant = TRUE; i < numKF; i++)
		{
			if((initialLeft != keyframes[i]->cropLeft) ||
				(initialTop != keyframes[i]->cropTop) ||
				(initialRight != keyframes[i]->cropRight) ||
				(initialBottom != keyframes[i]->cropBottom))
				constant = FALSE;
		}
		
		typeDef = CreateTypeDefinition(pDict, kAAFTypeID_AvidCrop);
		if(constant & optimizeConstant)
		{
			Fixed16toAAFRational(keyframes[0]->cropLeft, rect.left);
			Fixed16toAAFRational(keyframes[0]->cropTop, rect.top);
			Fixed16toAAFRational(keyframes[0]->cropRight, rect.right);
			Fixed16toAAFRational(keyframes[0]->cropBottom, rect.bottom);
			pParm = AAFAddConstantVal(pDict, sizeof(rect), &rect, pOutputEffect);
		}
		else
		{
			pVVal = AAFAddEmptyVaryingVal(pDict, pOutputEffect);
			for (i = 0; i < numKF; i++)
			{
				Fixed16toAAFRational(keyframes[i]->cropLeft, rect.left);
				Fixed16toAAFRational(keyframes[i]->cropTop, rect.top);
				Fixed16toAAFRational(keyframes[i]->cropRight, rect.right);
				Fixed16toAAFRational(keyframes[i]->cropBottom, rect.bottom);
				AAFAddOnePoint(pDict, times[i], sizeof(rect), &rect,
					typeDef, pVVal);
			}
			CHECKAAF(pVVal->QueryInterface(IID_IAAFParameter, (void **) &pParm));
			pVVal->Release();
			pVVal = NULL;
		}
		parmDef = CreateParameterDefinition(pDict, kAAFParamID_AvidCrop);
		CHECKAAF(pParm->SetParameterDefinition(parmDef));
		parmDef->Release();
		parmDef = NULL;
		
		CHECKAAF(pParm->SetTypeDefinition(typeDef));
		typeDef->Release();
		typeDef = NULL;
		pParm->Release();
		pParm = NULL;
	}
	
	if(EffectCapabilityPresent(effectUID, TAKES_SCALE))
	{
		AvFixed16	initialX, initialY;
		AAFPvtScaleExport	scale;
		
		initialX = keyframes[0]->Xscale;
		initialY = keyframes[0]->Yscale;
		for (short i = 0, constant = TRUE; i < numKF; i++)
		{
			if((initialX != keyframes[i]->Xscale) ||
				(initialY != keyframes[i]->Yscale))
				constant = FALSE;
		}
		
		typeDef = CreateTypeDefinition(pDict, kAAFTypeID_AvidScale);
		if(constant && optimizeConstant)
		{
			Fixed16toAAFRational(keyframes[0]->Xscale, scale.xScale);
			Fixed16toAAFRational(keyframes[0]->Yscale, scale.yScale);
			pParm = AAFAddConstantVal(pDict, sizeof(scale), &scale, pOutputEffect);
		}
		else
		{
			pVVal = AAFAddEmptyVaryingVal(pDict, pOutputEffect);
			for (short i = 0; i < numKF; i++)
			{
				Fixed16toAAFRational(keyframes[i]->Xscale, scale.xScale);
				Fixed16toAAFRational(keyframes[i]->Yscale, scale.yScale);
				AAFAddOnePoint(pDict, times[i], sizeof(scale), &scale,
					typeDef, pVVal);
			}
			CHECKAAF(pVVal->QueryInterface(IID_IAAFParameter, (void **) &pParm));
			pVVal->Release();
			pVVal = NULL;
		}
		
		parmDef = CreateParameterDefinition(pDict, kAAFParamID_AvidScale);
		CHECKAAF(pParm->SetParameterDefinition(parmDef));
		parmDef->Release();
		parmDef = NULL;
		
		CHECKAAF(pParm->SetTypeDefinition(typeDef));
		typeDef->Release();
		typeDef = NULL;
		pParm->Release();
		pParm = NULL;
	}
	
	if(EffectCapabilityPresent(effectUID, TAKES_SPILL_SUPRESS))
	{
		//	dprintf("exporting spill suppress\n");
		AAFPvtSpillExport	spill;
		//			kfInfoPvt->secondGain = (*kfinfohdl)->secondGain;
		//			kfInfoPvt->spillGain = (*kfinfohdl)->spillGain;	
		//			kfInfoPvt->secondSoft = (*kfinfohdl)->secondSoft;
		//			kfInfoPvt->spillSoft = (*kfinfohdl)->spillSoft;
		short initialSecondGain = keyframes[0]->secondGain;
		short initialSpillGain = keyframes[0]->spillGain;	
		short initialSecondSoft = keyframes[0]->secondSoft;
		short initialSpillSoft = keyframes[0]->spillSoft;
		for (short i = 0, constant = TRUE; i < numKF; i++)
		{
			if((initialSecondGain != keyframes[i]->secondGain) ||
				(initialSpillGain != keyframes[i]->spillGain) ||
				(initialSecondSoft != keyframes[i]->secondSoft) ||
				(initialSpillSoft != keyframes[i]->spillSoft))
				constant = FALSE;
		}
		
		typeDef = CreateTypeDefinition(pDict, kAAFTypeID_AvidSpillSupress);
		if(constant && optimizeConstant)
		{
			spill.secondGain = keyframes[0]->secondGain;
			spill.spillGain = keyframes[0]->spillGain;	
			spill.secondSoft = keyframes[0]->secondSoft;
			spill.spillSoft = keyframes[0]->spillSoft;
			pParm = AAFAddConstantVal(pDict, sizeof(spill), &spill, pOutputEffect);
		}
		else
		{
			pVVal = AAFAddEmptyVaryingVal(pDict, pOutputEffect);
			for (short i = 0; i < numKF; i++)
			{
				spill.secondGain = keyframes[i]->secondGain;
				spill.spillGain = keyframes[i]->spillGain;	
				spill.secondSoft = keyframes[i]->secondSoft;
				spill.spillSoft = keyframes[i]->spillSoft;
				AAFAddOnePoint(pDict, times[i], sizeof(spill),
					&spill, typeDef, pVVal);
			}
			CHECKAAF(pVVal->QueryInterface(IID_IAAFParameter, (void **) &pParm));
			pVVal->Release();
			pVVal = NULL;
		}
		
		parmDef = CreateParameterDefinition(pDict, kAAFParamID_AvidSpillSupress);
		CHECKAAF(pParm->SetParameterDefinition(parmDef));
		parmDef->Release();
		parmDef = NULL;
		
		CHECKAAF(pParm->SetTypeDefinition(typeDef));
		typeDef->Release();
		typeDef = NULL;
		pParm->Release();
		pParm = NULL;
	}
	
	if(EffectCapabilityPresent(effectUID, TAKES_BORDERS))
	{
		//	dprintf("exporting border frame\n");
		AvFixed16	initialLeft, initialTop, initialRight, initialBottom;
		unsigned char		initialXScale, initialYScale, initialXPos, initialYPos;
		AAFPvtBoundExport	box;
		
		initialLeft = keyframes[0]->Box.left;
		initialTop = keyframes[0]->Box.top;
		initialRight = keyframes[0]->Box.right;
		initialBottom = keyframes[0]->Box.bottom;
		initialXScale = keyframes[0]->Box.Lvl2Xscale;
		initialYScale = keyframes[0]->Box.Lvl2Yscale;
		initialXPos = keyframes[0]->Box.Lvl2Xpos;
		initialYPos = keyframes[0]->Box.Lvl2Ypos;
		for (short i = 0, constant = TRUE; i < numKF; i++)
		{
			if((initialLeft != keyframes[i]->Box.left) ||
				(initialTop != keyframes[i]->Box.top) ||
				(initialRight != keyframes[i]->Box.right) ||
				(initialBottom != keyframes[i]->Box.bottom) ||
				(initialXScale != keyframes[i]->Box.Lvl2Xscale) ||
				(initialYScale != keyframes[i]->Box.Lvl2Yscale) ||
				(initialXPos != keyframes[i]->Box.Lvl2Xpos) ||
				(initialYPos != keyframes[i]->Box.Lvl2Xpos))
				constant = FALSE;
		}
		
		typeDef = CreateTypeDefinition(pDict, kAAFTypeID_AvidBounds);
		if(constant && optimizeConstant)
		{
			Fixed16toAAFRational(keyframes[0]->Box.left, box.left);
			Fixed16toAAFRational(keyframes[0]->Box.top, box.top);
			Fixed16toAAFRational(keyframes[0]->Box.right, box.right);
			Fixed16toAAFRational(keyframes[0]->Box.bottom, box.bottom);
			box.Lvl2Xscale = keyframes[0]->Box.Lvl2Xscale ? AAFTrue : AAFFalse;
			box.Lvl2Yscale = keyframes[0]->Box.Lvl2Yscale ? AAFTrue : AAFFalse;
			box.Lvl2Xpos = keyframes[0]->Box.Lvl2Xpos ? AAFTrue : AAFFalse;
			box.Lvl2Ypos = keyframes[0]->Box.Lvl2Ypos ? AAFTrue : AAFFalse;
			pParm = AAFAddConstantVal(pDict, sizeof(box), &box, pOutputEffect);
		}
		else
		{
			pVVal = AAFAddEmptyVaryingVal(pDict, pOutputEffect);
			for (short i = 0; i < numKF; i++)
			{
				Fixed16toAAFRational(keyframes[i]->Box.left, box.left);
				Fixed16toAAFRational(keyframes[i]->Box.top, box.top);
				Fixed16toAAFRational(keyframes[i]->Box.right, box.right);
				Fixed16toAAFRational(keyframes[i]->Box.bottom, box.bottom);
				box.Lvl2Xscale = keyframes[i]->Box.Lvl2Xscale ? AAFTrue : AAFFalse;
				box.Lvl2Yscale = keyframes[i]->Box.Lvl2Yscale ? AAFTrue : AAFFalse;
				box.Lvl2Xpos = keyframes[i]->Box.Lvl2Xpos ? AAFTrue : AAFFalse;
				box.Lvl2Ypos = keyframes[i]->Box.Lvl2Ypos ? AAFTrue : AAFFalse;
				AAFAddOnePoint(pDict, times[i], sizeof(box), &box,
					typeDef, pVVal);
			}
			CHECKAAF(pVVal->QueryInterface(IID_IAAFParameter, (void **) &pParm));
			pVVal->Release();
			pVVal = NULL;
		}
		
		parmDef = CreateParameterDefinition(pDict, kAAFParamID_AvidBounds);
		CHECKAAF(pParm->SetParameterDefinition(parmDef));
		parmDef->Release();
		parmDef = NULL;
		
		CHECKAAF(pParm->SetTypeDefinition(typeDef));
		typeDef->Release();
		typeDef = NULL;
		pParm->Release();
		pParm = NULL;
	}
	
	if(EffectCapabilityPresent(effectUID, TAKES_COLOR))
	{
		//	dprintf("exporting color\n");
		AAFPvtColorExport	colorSet;
		
		pVVal = AAFAddEmptyVaryingVal(pDict, pOutputEffect);
		typeDef = CreateTypeDefinition(pDict, kAAFTypeID_AvidEffColor);
		for (short i = 0; i < numKF; i++)
		{
			colorSet.nColors = keyframes[i]->nColors;
			for (short n = 0; n < MAX_EFFECT_COLORS; n++)
				colorSet.colors[n] = keyframes[i]->colors[n];
			AAFAddOnePoint(pDict, times[i], sizeof(colorSet), &colorSet,
				typeDef, pVVal);
		}
		CHECKAAF(pVVal->QueryInterface(IID_IAAFParameter, (void **) &pParm));
		
		parmDef = CreateParameterDefinition(pDict, kAAFParamID_AvidColor);
		CHECKAAF(pParm->SetParameterDefinition(parmDef));
		parmDef->Release();
		parmDef = NULL;
		
		CHECKAAF(pParm->SetTypeDefinition(typeDef));
		typeDef->Release();
		typeDef = NULL;
		pVVal->Release();
		pVVal = NULL;
		pParm->Release();
		pParm = NULL;
	}
	
	if(keyframes[0]->userParamSize != 0)
	{
		//	dprintf("exporting user param\n");
		//JeffB: UserParam is a more complicated structure, look into it
		pVVal = AAFAddEmptyVaryingVal(pDict, pOutputEffect);
		typeDef = CreateTypeDefinition(pDict, kAAFTypeID_AvidEffUserParam);
		for (short i = 0; i < numKF; i++)
		{
			AAFAddOnePoint(pDict, times[i],
				keyframes[i]->userParamSize,
				((char *)keyframes[i]) + sizeof(struct OMFIPvtKFInfo),
				typeDef, pVVal);
		}
		CHECKAAF(pVVal->QueryInterface(IID_IAAFParameter, (void **) &pParm));
		
		parmDef = CreateParameterDefinition(pDict, kAAFParamID_AvidUserParam);
		CHECKAAF(pParm->SetParameterDefinition(parmDef));
		parmDef->Release();
		parmDef = NULL;
		
		CHECKAAF(pParm->SetTypeDefinition(typeDef));
		typeDef->Release();
		typeDef = NULL;
		pVVal->Release();
		pVVal = NULL;
		pParm->Release();
		pParm = NULL;
	}
	
#if 0	// Handle outside of this routine!!!?
	if(strncmp(*name, "EFF_BLEND_WIPE", 14) == 0)
	{
		AddWipeParameters(pOutputEffect);
	}
#endif
	// JeffB: These two parameters aren't handled yet	
	//	kfInfoPvt->selected = (long)(*kfinfohdl)->selected;
	//	kfInfoPvt->enableKeyFlags = (*kfinfohdl)->enableKeyFlags;
	
//cleanup:
	if(pVVal != NULL)
		pVVal->Release();
	if(pParm != NULL)
		pParm->Release();
	if(parmDef != NULL)
		parmDef->Release();
	if(typeDef != NULL)
		typeDef->Release();
}

void AAFAddOnePoint(IAAFDictionary *dict, aafRational_t percentTime, long buflen, void *buf, IAAFTypeDef *typeDef, IAAFVaryingValue *pVVal)
{
	IAAFControlPoint	*pPoint = NULL;
//	AAFRESULT			rc;

	CHECKAAF(dict->CreateInstance(kAAFClassID_ControlPoint,
		IID_IAAFControlPoint,
		(IUnknown **)&pPoint));
		
	CHECKAAF(pPoint->SetTypeDefinition(typeDef));
	CHECKAAF(pPoint->SetValue(buflen, (unsigned char *)buf));
	CHECKAAF(pPoint->SetTime(percentTime));
	CHECKAAF(pVVal->AppendPoint(pPoint));
//cleanup:
	if(pPoint != NULL)
		pPoint->Release();
}

IAAFParameter *AAFAddConstantVal(IAAFDictionary *dict, long buflen, void *buf, IAAFOperationGroup *pGroup)
{
	IAAFConstantValue	*pCVal = NULL;
	IAAFParameter		*pParm = NULL;
//	AAFRESULT			rc;

	CHECKAAF(dict->CreateInstance(kAAFClassID_ConstantValue,
		IID_IAAFConstantValue,
		(IUnknown **)&pCVal));
	CHECKAAF(pCVal->SetValue(buflen, (unsigned char *)buf));
	CHECKAAF(pCVal->QueryInterface(IID_IAAFParameter, (void **) &pParm));
	CHECKAAF(pGroup->AddNewParameter(pParm));
//cleanup:
	if(pCVal)
		pCVal->Release();

	return(pParm);
}

IAAFVaryingValue *AAFAddEmptyVaryingVal(IAAFDictionary *dict, IAAFOperationGroup *pOutputEffect)
{
	IAAFVaryingValue	*pVVal = NULL;
	IAAFParameter		*pParm = NULL;
//	AAFRESULT			rc;

	CHECKAAF(dict->CreateInstance(kAAFClassID_VaryingValue,
		IID_IAAFVaryingValue,
		(IUnknown **)&pVVal));
	CHECKAAF(pVVal->SetInterpolationDefinition(CreateInterpolationDefinition(
												dict, LinearInterpolator)));
	CHECKAAF(pVVal->QueryInterface(IID_IAAFParameter, (void **) &pParm));
		
	CHECKAAF(pOutputEffect->AddNewParameter(pParm));
//cleanup:
	if(pParm != NULL)
		pParm->Release();

	return(pVVal);
}

IAAFParameterDef *CreateParameterDefinition(IAAFDictionary *pDict, aafUID_t parmDefID)
{
	IAAFParameterDef	*parmDef;
	IAAFDefObject		*defObject;
	IAAFTypeDef			*typeDef;
	AAFRESULT			rc;
	aafUID_t			typeUID;

//	dprintf("AEffect::CreateParameterDefinition()\n");
	rc = pDict->LookupParameterDefinition(parmDefID,&parmDef);
	if(rc == AAFRESULT_SUCCESS && parmDef != NULL)
		return parmDef;

	CHECKAAF(pDict->CreateInstance(kAAFClassID_ParameterDefinition,
			IID_IAAFParameterDef,
			(IUnknown **)&parmDef));
	CHECKAAF(parmDef->QueryInterface(IID_IAAFDefObject, (void **) &defObject));
	if(memcmp(&parmDefID, &kAAFParamID_AvidBorderWidth, sizeof(aafUID_t)) == 0)
	{
    	CHECKAAF(defObject->Initialize(parmDefID, L"AvidBorderWidth", L"Opaque Bits"));
		typeUID = kAAFTypeID_Int32;
	}
	else if(memcmp(&parmDefID, &kAAFParamID_AvidBorderSoft, sizeof(aafUID_t)) == 0)
	{
    	CHECKAAF(defObject->Initialize(parmDefID, L"AvidBorderSoft", L"Opaque Bits"));
		typeUID = kAAFTypeID_Int32;
	}
	else if(memcmp(&parmDefID, &kAAFParamID_AvidXPos, sizeof(aafUID_t)) == 0)
	{
    	CHECKAAF(defObject->Initialize(parmDefID, L"AvidXPos", L"Opaque Bits"));
		typeUID = kAAFTypeID_AvidPosition;
	}
	else if(memcmp(&parmDefID, &kAAFParamID_AvidYPos, sizeof(aafUID_t)) == 0)
	{
    	CHECKAAF(defObject->Initialize(parmDefID, L"AvidYPos", L"Opaque Bits"));
		typeUID = kAAFTypeID_AvidPosition;
	}
	else if(memcmp(&parmDefID, &kAAFParamID_AvidCrop, sizeof(aafUID_t)) == 0)
	{
    	CHECKAAF(defObject->Initialize(parmDefID, L"AvidCrop", L"Opaque Bits"));
		typeUID = kAAFTypeID_AvidCrop;
	}
	else if(memcmp(&parmDefID, &kAAFParamID_AvidScale, sizeof(aafUID_t)) == 0)
	{
       	CHECKAAF(defObject->Initialize(parmDefID, L"AvidScale", L"Opaque Bits"));
		typeUID = kAAFTypeID_AvidScale;
	}
	else if(memcmp(&parmDefID, &kAAFParamID_AvidSpillSupress, sizeof(aafUID_t)) == 0)
	{
    	CHECKAAF(defObject->Initialize(parmDefID, L"AvidSpillSupress", L"Opaque Bits"));
		typeUID = kAAFTypeID_AvidSpillSupress;
	}
	else if(memcmp(&parmDefID, &kAAFParamID_AvidBounds, sizeof(aafUID_t)) == 0)
	{
    	CHECKAAF(defObject->Initialize(parmDefID, L"AvidBounds", L"Opaque Bits"));
		typeUID = kAAFTypeID_AvidBounds;
	}
	else if(memcmp(&parmDefID, &kAAFParamID_AvidColor, sizeof(aafUID_t)) == 0)
	{
    	CHECKAAF(defObject->Initialize(parmDefID, L"AvidColor", L"Opaque Bits"));
		typeUID = kAAFTypeID_AvidEffColor;
	}
	else if(memcmp(&parmDefID, &kAAFParamID_AvidUserParam, sizeof(aafUID_t)) == 0)
	{
    	CHECKAAF(defObject->Initialize(parmDefID, L"AvidUserParam", L"Opaque Bits"));
		typeUID = kAAFTypeID_AvidEffUserParam;
	}
	else if(memcmp(&parmDefID, &kAAFParameterDefLevel, sizeof(aafUID_t)) == 0)
	{
    	CHECKAAF(defObject->Initialize(parmDefID, L"Level", L"fractional 0-1 inclusive"));
		typeUID = kAAFTypeID_Rational;
	}
	else if(memcmp(&parmDefID, &kAAFParameterDefSMPTEWipeNumber, sizeof(aafUID_t)) == 0)
	{
    	CHECKAAF(defObject->Initialize(parmDefID, L"WipeCode", L"SMPTE Wipe Code"));
		typeUID = kAAFTypeID_Int32;
	}
	else if(memcmp(&parmDefID, &kAAFParamID_AvidEffectName, sizeof(aafUID_t)) == 0)
	{
    	CHECKAAF(defObject->Initialize(parmDefID, L"AvidName", L"Avid Effect Name"));
		typeUID = kAAFTypeID_String;
	}
	else if(memcmp(&parmDefID, &kAAFParamID_AvidGlobalKF, sizeof(aafUID_t)) == 0)
	{
    	CHECKAAF(defObject->Initialize(parmDefID, L"EffectGlobals", L"Opaque Bits"));
		typeUID = kAAFTypeID_String;
	}
	else
	{
		parmDef->Release();
		parmDef = NULL;
	}

	if(parmDef != NULL)
	{
		rc = pDict->LookupType(typeUID,&typeDef);
		if(rc != AAFRESULT_SUCCESS || typeDef == NULL)
		{
			typeDef = CreateTypeDefinition(pDict, typeUID);
		}
		CHECKAAF(parmDef->SetTypeDef(typeDef));

		CHECKAAF(pDict->RegisterParameterDefinition(parmDef));
	}
//cleanup:
	if(defObject != NULL)
		defObject->Release();

	return(parmDef);
}

IAAFTypeDef *CreateTypeDefinition(IAAFDictionary *pDict, aafUID_t typeDefID)
{
	IAAFTypeDef		*typeDef;
	IAAFDefObject	*defObject;
	AAFRESULT		rc;

//	dprintf("AEffect::CreateTypeDefinition()\n");	//JeffB:
	rc = pDict->LookupType(typeDefID,&typeDef);
	if(rc == AAFRESULT_SUCCESS && typeDef != NULL)
		return typeDef;

	CHECKAAF(pDict->CreateInstance(kAAFClassID_TypeDefinition,
			IID_IAAFTypeDef,
			(IUnknown **)&typeDef));
	CHECKAAF(typeDef->QueryInterface(IID_IAAFDefObject, (void **) &defObject));
	if(memcmp(&typeDefID, &kAAFTypeID_AvidPosition, sizeof(aafUID_t)) == 0)
	{
 		CHECKAAF(defObject->Initialize(typeDefID, L"AvidPosition", L"Opaque Bits"));
	}
	else if(memcmp(&typeDefID, &kAAFTypeID_AvidCrop, sizeof(aafUID_t)) == 0)
	{
 		CHECKAAF(defObject->Initialize(typeDefID, L"AvidCrop", L"Opaque Bits"));
	}
	else if(memcmp(&typeDefID, &kAAFTypeID_AvidScale, sizeof(aafUID_t)) == 0)
	{
 		CHECKAAF(defObject->Initialize(typeDefID, L"AvidScale", L"Opaque Bits"));
	}
	else if(memcmp(&typeDefID, &kAAFTypeID_AvidSpillSupress, sizeof(aafUID_t)) == 0)
	{
 		CHECKAAF(defObject->Initialize(typeDefID, L"AvidSpillSupress", L"Opaque Bits"));
	}
	else if(memcmp(&typeDefID, &kAAFTypeID_AvidBounds, sizeof(aafUID_t)) == 0)
	{
 		CHECKAAF(defObject->Initialize(typeDefID, L"AvidBounds", L"Opaque Bits"));
	}
	else if(memcmp(&typeDefID, &kAAFTypeID_AvidEffColor, sizeof(aafUID_t)) == 0)
	{
 		CHECKAAF(defObject->Initialize(typeDefID, L"AvidColor", L"Opaque Bits"));
	}
	else if(memcmp(&typeDefID, &kAAFTypeID_AvidEffUserParam, sizeof(aafUID_t)) == 0)
	{
 		CHECKAAF(defObject->Initialize(typeDefID, L"UserParam", L"Opaque Bits"));
	}
	else if(memcmp(&typeDefID, &kAAFTypeID_Rational, sizeof(aafUID_t)) == 0)
	{
 		CHECKAAF(defObject->Initialize(typeDefID, L"Rational", L"Rational Number"));
	}
	else if(memcmp(&typeDefID, &kAAFTypeID_Int32, sizeof(aafUID_t)) == 0)
	{
 		CHECKAAF(defObject->Initialize(typeDefID, L"Int32", L"32-bit signed integer"));
	}
	else if(memcmp(&typeDefID, &kAAFTypeID_AvidGlobalKF, sizeof(aafUID_t)) == 0)
	{
 		CHECKAAF(defObject->Initialize(typeDefID, L"EffectGlobals", L"Opaque Bits"));
	}
	else
	{
		typeDef->Release();
		typeDef = NULL;
	}

	if(typeDef != NULL)
		pDict->RegisterType(typeDef);
//cleanup:
	if(defObject != NULL)
		defObject->Release();

	return(typeDef);
}

#if 0
#define EFF_EMPTY_EFFECT				"EFF_EMPTY_EFFECT"	//	This is the ID for the effect 
//	that is returned when the requested 
//	effect is not available
#define EFF_BLEND_VDISSOLVE				"BLEND_DISSOLVE"
#define EFF_BLEND_TITLE					"EFF_BLEND_GRAPHIC"
#define BLEND_DISSOLVE_LIN				"BLEND_DISSOLVE_LIN"		// film fade
#define BLEND_DISSOLVE_LOG				"BLEND_DISSOLVE_LOG"		// film dissolve
#endif


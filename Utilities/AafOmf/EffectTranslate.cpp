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
	HRESULT					rc = AAFRESULT_SUCCESS;
	long					n, numEntries = sizeof(xlateTable)/sizeof(effectXlate_t);
	long					x, numWipeEntries = sizeof(wipeTable)/sizeof(wipeTable_t);
	aafUInt32				wipeNumber;
	aafUInt32				bytesRead;
	bool					found = false;
	aafUID_t				AAFEffectID, direction;

    rc = effect->GetOperationDefinition(&pOpDef);
	if(SUCCEEDED(rc))
	{
		rc = pOpDef->QueryInterface(IID_IAAFDefObject, (void **) &pDef);
	}

	if(SUCCEEDED(rc))
	{
        rc = pDef->GetAUID(&AAFEffectID);
	}
	
	if(SUCCEEDED(rc))
	{

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
					(void)effect->GetParameterByArgID(kAAFParamID_AvidDirection,
														&pParameter);
					if(pParameter->QueryInterface(IID_IAAFConstantValue, (void **) &pCVal) == AAFRESULT_SUCCESS)
					{
						// Determine effect ID from direction
						(void)pCVal->GetValue(sizeof(direction),(unsigned char*)&direction,&bytesRead);
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
					(void)effect->GetParameterByArgID(kAAFParameterDefSMPTEWipeNumber,
														&pParameter);
					if(pParameter->QueryInterface(IID_IAAFConstantValue, (void **) &pCVal) == AAFRESULT_SUCCESS)
					{
						// Determine effect ID from direction
						(void)pCVal->GetValue(sizeof(wipeNumber),(unsigned char*)&wipeNumber,&bytesRead);
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
	}

	return rc;
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
	strcpy(MCEffectID, MCEffectIDPtr);
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
	AAFRESULT				hr;

	hr = dict->LookupInterpolationDefinition(interpolationDefID,&interpDef);
	if(hr == AAFRESULT_SUCCESS && interpDef != NULL)
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

	return(interpDef);
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

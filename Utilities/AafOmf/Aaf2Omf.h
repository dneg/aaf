/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/
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
/************************************************************************
 *
 * Aaf2Omf.h Describes structures and prototypes for the AAF to OMF part 
 *           of the conversion utility.
 *
 ************************************************************************/
// ============================================================================
// simple helper class to convert aaf files to omf
// ============================================================================
class Aaf2Omf
{
public:
	Aaf2Omf();
	~Aaf2Omf();

public:
	HRESULT OpenInputFile( void );
		
	HRESULT ConvertFile( void );

private:
	void ConvertAUIDtoUID(aafUID_t* pMobID, OMF2::omfUID_t* pOMFMobID);
	HRESULT OpenOutputFile( void );
	void CloseInputFile( void );
	void CloseOutputFile(void );
	HRESULT AAFFileRead( void );
	HRESULT ConvertCompositionMob(IAAFCompositionMob* pCompMob, OMF2::omfMobObj_t* pOMFCompMob, char* pMobName, aafUID_t* MobID);
	HRESULT ConvertMasterMob(IAAFMasterMob* pMasterMob, OMF2::omfMobObj_t* pOMFCompMob, char* pMobName, aafUID_t* MobID);
	HRESULT ConvertSourceMob(IAAFSourceMob* pSourceMob, OMF2::omfMobObj_t* pOMFCompMob, char* pMobName, aafUID_t* MobID);
	HRESULT TraverseMob(IAAFMob* pMob, OMF2::omfMobObj_t* pOMFMob);
	HRESULT ProcessComponent(IAAFComponent* pComponent, OMF2::omfObject_t* pOMFSegment);
	HRESULT ConvertAAFDatadef(aafUID_t Datadef, OMF2::omfDDefObj_t* pDatakind);
	HRESULT GetUniqueNameFromAUID(aafUID_t Datadef, OMF2::omfUniqueNamePtr_t UniqueName);
	HRESULT TraverseSequence(IAAFSequence* pSequence, OMF2::omfObject_t* pOMFSequence );
	HRESULT ConvertSelector(IAAFSelector* pSelector, OMF2::omfObject_t* pOMFSelector );
	HRESULT ConvertLocator(IAAFEssenceDescriptor* pEssenceDesc, OMF2::omfMobObj_t*	pOMFSourceMob );
	HRESULT ConvertEssenceDataObject(IAAFEssenceData* pEssenceData);
	HRESULT ConvertEffects(IAAFOperationGroup* pEffect, OMF2::omfEffObj_t*	pOMFEffect);

private:

    OMF2::omfSessionHdl_t	OMFSession;
	OMF2::omfHdl_t			OMFFileHdl;
	OMF2::omfFileRev_t		OMFFileRev;

	IAAFFile*				pFile;
	IAAFHeader*				pHeader;
	IAAFDictionary*			pDictionary;
};

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
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
 * Omf2Aaf.h Describes structures and prototypes for the OMF to AAF 
 *          conversion utility.
 *
 ************************************************************************/
// ============================================================================
// simple helper class to convert omf files to aaf
// ============================================================================
class Omf2Aaf
{
public:
	Omf2Aaf();
	~Omf2Aaf();

public:
	HRESULT ConvertFile( void );
	HRESULT OpenOutputFile( void );
	HRESULT OMFFileOpen( char* pFileName );
	HRESULT	TOCFileCreate( void ); 
	void OMFFileClose( void );
	HRESULT AAFFileOpen( char* pFileName );
	HRESULT AAFDefinitionFileOpen( void );
	void AAFFileClose(void );
	HRESULT OMFFileRead( void );
	HRESULT ConvertOMFHeader( void );
	HRESULT ConvertOMFDataDefinitionObject( OMF2::omfObject_t obj);
	HRESULT ConvertOMFClassDictionaryObject( OMF2::omfObject_t obj);
	HRESULT ConvertOMFMediaDataObject( OMF2::omfObject_t obj);
	HRESULT ConvertOMFDatakind(OMF2::omfDDefObj_t datakind, aafUID_t* pDatadef);
	HRESULT ConvertUniqueNameToAUID(OMF2::omfUniqueName_t datakindName, aafUID_t* pDatadef);
	HRESULT ConvertOMFMOBObject( OMF2::omfObject_t obj, IAAFMob* pMob );
	HRESULT ConvertOMFCompositionObject( OMF2::omfObject_t obj,IAAFCompositionMob* pCompMob );
	HRESULT ConvertOMFMasterMob( OMF2::omfObject_t obj, IAAFMasterMob* pMasterMob );
	HRESULT ConvertOMFSourceMob( OMF2::omfObject_t obj, IAAFSourceMob* pSourceMob );
	HRESULT TraverseOMFMob( OMF2::omfObject_t obj, IAAFMob* pMob );
	HRESULT ProcessOMFComponent( OMF2::omfObject_t obj, IAAFComponent** ppComponent );
	HRESULT TraverseOMFSequence( OMF2::omfObject_t obj, IAAFSequence* pSequence );
	HRESULT	ConvertOMFSequence( OMF2::omfObject_t sequence, IAAFSequence* pSequence);
	HRESULT ConvertOMFSourceClip( OMF2::omfObject_t sourceclip, IAAFSourceClip* pSourceClip);
	HRESULT ConvertOMFComponentProperties(OMF2::omfObject_t sequence, IAAFComponent* pComponent);
	HRESULT ConvertOMFTransition(OMF2::omfObject_t transition, IAAFTransition* pTransition);
	HRESULT ConvertOMFSelector(OMF2::omfObject_t selector, IAAFSelector* pSelector);
	HRESULT ConvertOMFLocator(OMF2::omfObject_t locator, IAAFEssenceDescriptor* pEssenceDesc);
	HRESULT ConvertOMFCDCIDescriptorLocator(OMF2::omfObject_t mediaDescriptor, IAAFCDCIDescriptor* pAAFDescriptor);
	void SetEffectOptionalProperties(IAAFEffect* pEffect, IAAFParameterDef* pParameterDef, aafLength_t effectLength, aafUID_t effectDatadef);
	void CreateEffectDefinition(IAAFEffectDef**	ppEffectDef,
								IAAFParameterDef** ppParameterDef,
								aafUID_t* pEffectDefAUID, 
								aafUID_t* pEffectAUID, 
								aafWChar* pwName, 
								aafWChar* pwDesc,
								aafBool isTimeWarp,
								aafUInt32 bypassOverride);

private:

    OMF2::omfSessionHdl_t	OMFSession;
	OMF2::omfHdl_t			OMFFileHdl;
	OMF2::omfFileRev_t		OMFFileRev;

	IAAFFile*				pFile;
	IAAFHeader*				pHeader;
	IAAFDictionary*			pDictionary;
};

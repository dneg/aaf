#ifndef _OMF2AAF_
#define _OMF2AAF_ 1
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
#include "EffectTranslate.h"

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
	void ConvertFile( void );
	void OpenOutputFile( void );
	void OMFFileOpen( char* pFileName );
	void TOCFileCreate( void ); 
	void OMFFileClose( void );
	void AAFFileOpen( char* pFileName );
	void AAFDefinitionFileOpen( void );
	void AAFFileClose(void );
	void OMFFileRead( void );
	void ConvertOMFHeader( void );
	void ConvertOMFSourceMob( OMF2::omfObject_t obj, IAAFSourceMob* pSourceMob );
	void TraverseOMFMob( OMF2::omfObject_t obj, IAAFMob* pMob );
	void ConvertOMFDataDefinitionObject( OMF2::omfObject_t obj);
	void ConvertOMFClassDictionaryObject( OMF2::omfObject_t obj);
	void ConvertOMFMediaDataObject( OMF2::omfObject_t obj, OMF2::omfUID_t inMediaID);
	virtual void ConvertOMFDatakind(OMF2::omfDDefObj_t datakind, aafUID_t * pDatakind);
	virtual void ConvertOMFDataDef(OMF2::omfDDefObj_t datakind, IAAFDataDef ** ppDatadef);
	void ConvertOMFDataDefType(OMF2::omfDDefObj_t datakind, IAAFTypeDef ** ppTypeDef);
//	void ConvertUniqueNameToAUID(OMF2::omfUniqueName_t datakindName, aafUID_t* pDatadef);
	void ConvertOMFMOBObject( OMF2::omfObject_t obj, IAAFMob* pMob );
	void ConvertOMFCompositionObject( OMF2::omfObject_t obj,IAAFCompositionMob* pCompMob );
	void ConvertOMFMasterMob( OMF2::omfObject_t obj, IAAFMasterMob* pMasterMob );
	void ProcessOMFComponent( OMF2::omfObject_t obj, IAAFComponent** ppComponent );
	void TraverseOMFSequence( OMF2::omfObject_t obj, IAAFSequence* pSequence );
	void	ConvertOMFSequence( OMF2::omfObject_t sequence, IAAFSequence* pSequence);
	void ConvertOMFSourceClip( OMF2::omfObject_t sourceclip, IAAFSourceClip* pSourceClip);
	void ConvertOMFComponentProperties(OMF2::omfObject_t sequence, IAAFComponent* pComponent);
	void ConvertOMFTransition(OMF2::omfObject_t transition, IAAFTransition* pTransition);
	void ConvertOMFSelector(OMF2::omfObject_t selector, IAAFSelector* pSelector);
	void ConvertOMFLocator(OMF2::omfObject_t locator, IAAFEssenceDescriptor* pEssenceDesc);
	void ConvertOMFCDCIDescriptorLocator(OMF2::omfObject_t mediaDescriptor, IAAFCDCIDescriptor* pAAFDescriptor);
	virtual void ConvertOMFEffects(OMF2::omfEffObj_t effect, IAAFOperationGroup* pEffect);
	void ConvertOMFEffectDefinition(OMF2::omfDDefObj_t effectDef, OMF2::omfObject_t effect, IAAFOperationDef** ppEffectDef);
	void GetAAFOperationDefinition(OMF2::omfUniqueName_t datakindName, OMF2::omfUniqueName_t MCEffectID,
										char* defName, char* defDescription, aafUInt32 bypassOverride, aafBool isTimeWarp,
										aafInt32 numberInputs, aafUID_t defDataDef, IAAFOperationDef** ppEffectDef);
	void GetParameterDefinition(aafUID_t* pDefUID, IAAFTypeDef* pTypeDef,	aafWChar* pwName, aafWChar* pwDesc, aafWChar* pwDisplayUnits, IAAFParameterDef** ppParameterDef);
	void	ConvertOMFConstValue(OMF2::omfSegObj_t segment, IAAFConstantValue* pConstValue);
	void	ConvertOMFVaryingValue(OMF2::omfSegObj_t segment, IAAFVaryingValue* pVaryingValue);
	void	ConvertOMFNestedScope(OMF2::omfSegObj_t segment, IAAFNestedScope* pNestedScope);
	void	ConvertOMFScopeRef(OMF2::omfSegObj_t segment, IAAFScopeReference* pScopeRef);
	virtual void ConvertObjectProps(OMF2::omfObject_t pOMFObject, aafUID_t &classID, IAAFObject* pObj);

protected:

    OMF2::omfSessionHdl_t	OMFSession;
	OMF2::omfHdl_t			OMFFileHdl;
	OMF2::omfFileRev_t		OMFFileRev;

	IAAFFile*				pFile;
	IAAFHeader*				pHeader;
	IAAFDictionary*			pDictionary;

	AAFDomainUtils			*pAAF;
	OMFDomainUtils			*pOMF;
	EffectTranslate			*pEffectTranslate;
};

#endif

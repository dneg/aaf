#ifndef _OMF2AAF_
#define _OMF2AAF_ 1
//=---------------------------------------------------------------------=
//
// $Id$ $Name$
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=
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
	Omf2Aaf( AAFDomainUtils *aafDomainUtils, OMFDomainUtils *omfDomainUtils, EffectTranslate *effectTranslate );
	virtual ~Omf2Aaf();

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
	void ConvertOMFSourceMob( omfObject_t obj, IAAFSourceMob* pSourceMob );
	void TraverseOMFMob( omfObject_t obj, IAAFMob* pMob );
	void ConvertOMFDataDefinitionObject( omfObject_t obj);
	void ConvertOMFClassDictionaryObject( omfObject_t obj);
	void ConvertOMFMediaDataObject( omfObject_t obj, omfUID_t inMediaID);
	virtual void ConvertOMFDatakind(omfDDefObj_t datakind, aafUID_t * pDatakind);
	virtual void ConvertOMFDataDef(omfDDefObj_t datakind, IAAFDataDef ** ppDatadef);
	void ConvertOMFDataDefType(omfDDefObj_t datakind, IAAFTypeDef ** ppTypeDef);
//	void ConvertUniqueNameToAUID(omfUniqueName_t datakindName, aafUID_t* pDatadef);
	void ConvertOMFMOBObject( omfObject_t obj, IAAFMob* pMob );
	void ConvertOMFCompositionObject( omfObject_t obj,IAAFCompositionMob* pCompMob );
	void ConvertOMFMasterMob( omfObject_t obj, IAAFMasterMob* pMasterMob );
	void ProcessOMFComponent( omfObject_t obj, IAAFComponent** ppComponent );
	void TraverseOMFSequence( omfObject_t obj, IAAFSequence* pSequence );
	void	ConvertOMFSequence( omfObject_t sequence, IAAFSequence* pSequence);
	void ConvertOMFSourceClip( omfObject_t sourceclip, IAAFSourceClip* pSourceClip);
	void ConvertOMFComponentProperties(omfObject_t sequence, IAAFComponent* pComponent);
	void ConvertOMFTransition(omfObject_t transition, IAAFTransition* pTransition);
	void ConvertOMFSelector(omfObject_t selector, IAAFSelector* pSelector);
	void ConvertOMFLocator(omfObject_t locator, IAAFEssenceDescriptor* pEssenceDesc);
	void ConvertOMFCDCIDescriptorLocator(omfObject_t mediaDescriptor, IAAFCDCIDescriptor* pAAFDescriptor);
	virtual void ConvertOMFEffects(omfEffObj_t effect, IAAFOperationGroup* pEffect);
	void ConvertOMFEffectDefinition(omfDDefObj_t effectDef, omfObject_t effect, IAAFOperationDef** ppEffectDef);
	void GetAAFOperationDefinition(omfUniqueName_t datakindName, omfUniqueName_t MCEffectID,
										char* defName, char* defDescription, aafUInt32 bypassOverride, aafBool isTimeWarp,
										aafInt32 numberInputs, aafUID_t defDataDef, IAAFOperationDef** ppEffectDef);
	void GetParameterDefinition(aafUID_t* pDefUID, IAAFTypeDef* pTypeDef,	aafWChar* pwName, aafWChar* pwDesc, aafWChar* pwDisplayUnits, IAAFParameterDef** ppParameterDef);
	void	ConvertOMFConstValue(omfSegObj_t segment, IAAFParameterDef* pParameterDef, IAAFConstantValue* pConstValue);
	void	ConvertOMFVaryingValue(omfSegObj_t segment, IAAFParameterDef* pParameterDef, IAAFVaryingValue* pVaryingValue);
	void	ConvertOMFNestedScope(omfSegObj_t segment, IAAFNestedScope* pNestedScope);
	void	ConvertOMFScopeRef(omfSegObj_t segment, IAAFScopeReference* pScopeRef);
	virtual void ConvertObjectProps(omfObject_t pOMFObject, aafUID_t &classID, IAAFObject* pObj);
	virtual void FinishUpMob(omfMobObj_t  /*pOMFMob*/, IAAFMob*  /*pMob*/) { };

protected:

    omfSessionHdl_t			OMFSession;
	omfHdl_t				OMFFileHdl;
	omfFileRev_t			OMFFileRev;

	IAAFFile*				pFile;
	IAAFHeader*				pHeader;
	IAAFDictionary*			pDictionary;

	AAFDomainUtils			*pAAF;
	OMFDomainUtils			*pOMF;
	EffectTranslate			*pEffectTranslate;
};

#endif

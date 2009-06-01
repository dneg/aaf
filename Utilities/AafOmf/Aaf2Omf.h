#ifndef _AAF2OMF_
#define _AAF2OMF_ 1
//=---------------------------------------------------------------------=
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2009, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include "EffectTranslate.h"
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
	Aaf2Omf( AAFDomainUtils *aafDomainUtils, OMFDomainUtils *omfDomainUtils, EffectTranslate *effectTranslate );
	virtual ~Aaf2Omf();

public:
	void OpenInputFile( void );
		
	void ConvertFile( void );

protected:
	void ConvertMobIDtoUID(aafMobID_constptr pMobID, omfUID_t* pOMFMobID);
	void OpenOutputFile( void );
	void CloseInputFile( void );
	void CloseOutputFile(void );
	void AAFFileRead( void );
	virtual void ConvertCompositionMob(IAAFCompositionMob* pCompMob, omfMobObj_t* pOMFCompMob, char* pMobName, aafMobID_t* pMobID);
	void ConvertMasterMob(IAAFMasterMob* pMasterMob, omfMobObj_t* pOMFCompMob, char* pMobName, aafMobID_t* pMobID);
	void ConvertSourceMob(IAAFSourceMob* pSourceMob, omfMobObj_t* pOMFCompMob, char* pMobName, aafMobID_t* pMobID);
	void TraverseMob(IAAFMob* pMob, omfMobObj_t* pOMFMob);
	void ProcessComponent(IAAFComponent* pComponent, omfObject_t* pOMFSegment);
	void ConvertAAFDatadef(aafUID_t Datadef, omfDDefObj_t* pDatakind);
	virtual void ConvertAAFTypeIDDatakind(aafUID_t, omfDDefObj_t* pDatakind);
	void TraverseSequence(IAAFSequence* pSequence, omfObject_t* pOMFSequence );
	void ConvertSelector(IAAFSelector* pSelector, omfObject_t* pOMFSelector );
	void ConvertEssenceGroup(IAAFEssenceGroup* pGroup, omfObject_t* pOMFMediaGroup );
	void ConvertLocator(IAAFEssenceDescriptor* pEssenceDesc, omfMobObj_t*	pOMFSourceMob );
	void ConvertEssenceDataObject(IAAFEssenceData* pEssenceData);
	virtual void ConvertEffects(IAAFOperationGroup* pEffect, omfEffObj_t nest, omfEffObj_t*	pOMFEffect);
	virtual void ConvertParameter(IAAFParameter* pParm,
		aafUID_t			&effectDefID,
		omfSegObj_t pOMFEffect,
		omfInt32 slotNum, omfLength_t effectLen);
	virtual void	FinishUpMob(IAAFMob* /*pMob*/, omfMobObj_t /*pOMFMob*/) {};
	omfObject_t LocateSlot(omfEffObj_t pOMFEffect, aafInt32 slotID);
	virtual void ConvertValueBuf(aafUID_t &typeDefID,
								aafDataBuffer_t srcValue, aafUInt32 srcValueLen,
								aafDataBuffer_t *destValue, aafUInt32 *destValueLen,
								bool *didAllocateNew);
	virtual void ConvertObjectProps(IAAFObject* pObj, omfObject_t pOMFObject);
	virtual void ConvertNestedScope(IAAFNestedScope* pNest, omfObject_t* pOMFNest );
protected:

    omfSessionHdl_t	OMFSession;
	omfHdl_t			OMFFileHdl;
	omfFileRev_t		OMFFileRev;
	IAAFFile*				pFile;
	IAAFHeader*				pHeader;
	IAAFDictionary*			pDictionary;

	AAFDomainUtils			*pAAF;
	OMFDomainUtils			*pOMF;
	EffectTranslate			*pEffectTranslate;
};
#endif

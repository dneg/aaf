#ifndef _AAF2OMF_
#define _AAF2OMF_ 1
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
	void OpenInputFile( void );
		
	void ConvertFile( void );

protected:
	void ConvertMobIDtoUID(aafMobID_constptr pMobID, OMF2::omfUID_t* pOMFMobID);
	void OpenOutputFile( void );
	void CloseInputFile( void );
	void CloseOutputFile(void );
	void AAFFileRead( void );
	virtual void ConvertCompositionMob(IAAFCompositionMob* pCompMob, OMF2::omfMobObj_t* pOMFCompMob, char* pMobName, aafMobID_t* pMobID);
	void ConvertMasterMob(IAAFMasterMob* pMasterMob, OMF2::omfMobObj_t* pOMFCompMob, char* pMobName, aafMobID_t* pMobID);
	void ConvertSourceMob(IAAFSourceMob* pSourceMob, OMF2::omfMobObj_t* pOMFCompMob, char* pMobName, aafMobID_t* pMobID);
	void TraverseMob(IAAFMob* pMob, OMF2::omfMobObj_t* pOMFMob);
	void ProcessComponent(IAAFComponent* pComponent, OMF2::omfObject_t* pOMFSegment);
	void ConvertAAFDatadef(aafUID_t Datadef, OMF2::omfDDefObj_t* pDatakind);
	virtual void ConvertAAFTypeIDDatakind(aafUID_t, OMF2::omfDDefObj_t* pDatakind);
	void TraverseSequence(IAAFSequence* pSequence, OMF2::omfObject_t* pOMFSequence );
	void ConvertSelector(IAAFSelector* pSelector, OMF2::omfObject_t* pOMFSelector );
	void ConvertLocator(IAAFEssenceDescriptor* pEssenceDesc, OMF2::omfMobObj_t*	pOMFSourceMob );
	void ConvertEssenceDataObject(IAAFEssenceData* pEssenceData);
	virtual void ConvertEffects(IAAFOperationGroup* pEffect, OMF2::omfEffObj_t nest, OMF2::omfEffObj_t*	pOMFEffect);
	virtual void ConvertParameter(IAAFParameter* pParm,
		aafUID_t			&effectDefID,
		OMF2::omfSegObj_t pOMFEffect,
		OMF2::omfInt32 slotNum, OMF2::omfLength_t effectLen);
	virtual void	FinishUpMob(IAAFMob* pMob, OMF2::omfMobObj_t pOMFMob) {};
	OMF2::omfObject_t LocateSlot(OMF2::omfEffObj_t pOMFEffect, aafInt32 slotID);
	virtual void ConvertValueBuf(aafUID_t &typeDefID,
								aafDataBuffer_t srcValue, aafUInt32 srcValueLen,
								aafDataBuffer_t *destValue, aafUInt32 *destValueLen,
								bool *didAllocateNew);
	virtual void ConvertObjectProps(IAAFObject* pObj, OMF2::omfObject_t pOMFObject);
	virtual void ConvertNestedScope(IAAFNestedScope* pNest, OMF2::omfObject_t* pOMFNest );
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
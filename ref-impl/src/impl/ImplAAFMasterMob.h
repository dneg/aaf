//@doc
//@class    AAFMasterMob | Implementation class for AAFMasterMob
#ifndef __ImplAAFMasterMob_h__
#define __ImplAAFMasterMob_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

class ImplAAFSourceClip;

class ImplAAFSourceMob;

class ImplAAFComponent;

class ImplAAFDataDef;

class ImplAAFFindSourceInfo;





#ifndef __ImplAAFMob_h__
#include "ImplAAFMob.h"
#endif


class ImplAAFMasterMob : public ImplAAFMob
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFMasterMob ();
  virtual ~ImplAAFMasterMob ();


  virtual AAFRESULT STDMETHODCALLTYPE
	Initialize ();


  //****************
  // AddMasterSlot()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AddMasterSlot
        (// @parm [in] Data kind of new Master Mob slot
		 aafUID_t * pDataDef,

		 // @parm [in] Slot ID of the Source Mob slot to be added to the Master Mob
         aafSlotID_t  sourceSlotID,

		 // @parm [in] Source Mob containing the slot to be added to the Master Mob
         ImplAAFSourceMob * pSourceMob,

		 // @parm [in] SlotID assigned to the new Master Mob slot
		 aafSlotID_t  masterSlotID,

		 // @parm [in, string] Name to assign to new slot in Master Mob
		 aafWChar *  pSlotName);



  //****************
  // GetTapeName()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTapeName
        (// @parm [in] SlotID of the Master Mob slot
		 aafInt32  masterSlotID,

		 // @parm [out, size_is(bufSize), string] The returned name
		 aafWChar *  pTapeName,

		 // @parm [in] the size of the pTapeName buffer
         aafInt32  bufSize);


  //****************
  // GetTapeNameBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTapeNameBufLen
        (// @parm [in] SlotID of the Master Mob slot
		 aafInt32  masterSlotID,

		 // @parm [out] required buffer length
         aafInt32 *  pLen);


  //****************
  // GetNumRepresentations()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetNumRepresentations
	    (// @parm [in] SlotID
         aafSlotID_t  slotID,

		 // @parm [out, retval] number of representations
		 aafNumSlots_t *  pNumReps);


  //****************
  // GetRepresentationSourceClip()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetRepresentationSourceClip
        (// @parm [in] Slot ID
		 aafSlotID_t  slotID,

		 // @parm [in] Index of requested representation
		 aafInt32  index,

		 // @parm [out] Requested Source Clip
         ImplAAFSourceClip ** ppSourceClip);


  //****************
  // GetCriteriaSourceClip()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetCriteriaSourceClip
        (// @parm [in] Slot ID
		 aafSlotID_t  slotID,

		 // @parm [in] Index of requested representation
		 aafMediaCriteria_t *  pCriteria,

		 // @parm [out] Requested Source Clip
         ImplAAFSourceClip ** ppSourceClip);


  //***********************************************************
  // METHOD NAME: SearchSource()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFSearchSource | SearchSource |
  // This function returns the source information for a slot in a
  // Master Mob or Source Mob.  It follows the Source Clip references
  // in the specified slot until it encounters the kind of Mob
  // specified in the mobKind parameter.  This function cannot be used
  // on a Composition Mob and is not intended to be called
  // iteratively; use the omfiMobOpenSearch, omfiMobGetNextSource,
  // omfiMobGetThisSource, and omfiMobCloseSearch functions for those
  // purposes. 
  //
  // Succeeds if all of the following are true:
  // - the ppThisCpnt pointer is valid.
  // (other conditions here)
  // 
  // If this method fails nothing is written to *ppThisCpnt.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - ppCpnt is null.
  //
  // (other codes here.)
  // @end
  // 
  virtual AAFRESULT STDMETHODCALLTYPE
  SearchSource (
    // @parm [in] aafSlotID_t | slotID | Slot ID
    aafSlotID_t  slotID,

    // @parm [in] aafPosition_t | offset | Offset
    aafPosition_t  offset,

    // @parm [in] aafMobKind_t | mobKind | Mob Kind
    aafMobKind_t  mobKind,

    // @parm [in] aafMediaCriteria_t * | pMediaCrit | Media Criteria
    aafMediaCriteria_t *  pMediaCrit,

    // @parm [in] aafEffectChoice_t * | pEffectChoice | Effect Choice
    aafEffectChoice_t *  pEffectChoice,

    // @parm [out] AAFFindSourceInfo | ppSourceInfo | Source Information
    ImplAAFFindSourceInfo ** ppSourceInfo
  );



  //***********************************************************
  // METHOD NAME: GetMobKind()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFMobPrivate | GetMobKind |
  // This method returns the kind of this mob.
  // 
  // Succeeds if all of the following are true:
  // - the pMobKind pointer is valid.
  // 
  // If this method fails nothing is written to *pMobKind.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - pMobKind is null.
  // @end
  // 
  virtual AAFRESULT STDMETHODCALLTYPE
  GetMobKind (
    // @parm [out] aafMobKind_t * | pMobKind | The kind of this mob
    aafMobKind_t *  pMobKind
  );

  //****************
  // NewPhysSourceRef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    NewPhysSourceRef
        (// @parm [in] Edit rate of slot to contain reference
		 aafRational_t  editrate,

		 // @parm [in] SlotID of slot to contain reference
		 aafSlotID_t  aMobSlot,

		 // @parm [in] Data kind of slot to contain reference
         aafUID_t * pEssenceKind,

		aafSourceRef_t  ref,

		 // @parm [in] Length of the Source Clip
         aafLength_t  srcRefLength);

  //****************
  // AddPhysSourceRef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendPhysSourceRef
        (// @parm [in] Edit rate of slot to contain reference
		 aafRational_t  editrate,

		 // @parm [in] SlotID of slot to contain reference
		 aafSlotID_t  aMobSlot,

		 // @parm [in] Data kind of slot to contain reference
         aafUID_t * pEssenceKind,

		aafSourceRef_t  ref,

		 // @parm [in] Length of the Source Clip
         aafLength_t  srcRefLength);



public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFMasterMob)

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFMasterMobTest.cpp.
  static AAFRESULT test();

  // Internal to the SDK
  virtual AAFRESULT ReconcileMobLength(void);

};

#endif // ! __ImplAAFMasterMob_h__


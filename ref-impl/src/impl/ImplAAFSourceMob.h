//@doc
//@class    AAFSourceMob | Implementation class for AAFSourceMob
#ifndef __ImplAAFSourceMob_h__
#define __ImplAAFSourceMob_h__

#include "OMStorable.h"

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

class ImplAAFEssenceDescriptor;

class ImplAAFDataDef;

class ImplAAFFindSourceInfo;

class ImplAAFComponent;

class ImplAAFSegment;



#ifndef __ImplAAFMob_h__
#include "ImplAAFMob.h"
#endif

#include "ImplAAFEssenceDescriptor.h"

#include "OMProperty.h"


const int FULL_LENGTH = -1;

class ImplAAFSourceMob : public ImplAAFMob
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFSourceMob ();
  virtual ~ImplAAFSourceMob ();


  virtual AAFRESULT STDMETHODCALLTYPE
	Initialize ();


  //****************
  // GetEssenceDescriptor()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetEssenceDescriptor
		// @parm [out] Returned Essence Descriptor object
        (ImplAAFEssenceDescriptor ** ppMdes);


  //****************
  // SetEssenceDescriptor()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetEssenceDescriptor
		// @parm [in] Essence Descriptor object
        (ImplAAFEssenceDescriptor * pMdes);


  //****************
  // AddNilReference()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AddNilReference
        (// @parm [in] SlotID to be assigned to the new slot
		 aafSlotID_t  slotID,

		 // @parm [in] Duration of the Source Clip to be added to the new slot
		 const aafLength_t & length,

		 // @parm [in] Data kind of the new slot
         const aafUID_t & dataDef,

		 // @parm [in] Edit rate of the new slot
		 const aafRational_t & editRate);


  //****************
  // AppendTimecodeSlot()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendTimecodeSlot
        (// @parm [in] Edit rate of Timecode slot
		 aafRational_t  editrate,

		 // @parm [in] SlotID of Timecode slot
		 aafInt32  slotID,

		 // @parm [in] Starting time code
         aafTimecode_t  startTC,

		 // @parm [in] Duration of Timecode.
		 aafFrameLength_t  length32);


  //****************
  // AppendEdgecodeSlot()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendEdgecodeSlot
        (// @parm [in] Edit rate of the Edgecode slot
		 aafRational_t  editrate,

		 // @parm [in] SlotID to assign to the new Edgecode slot
		 aafInt32  slotID,

		 // @parm [in] Starting Edgecode
         aafFrameOffset_t  startEC,

		 // @parm [in] Length of the Edgecode component in the slot
		 aafFrameLength_t  length32,

		 // @parm [in] The film kind.
         aafFilmType_t  filmKind,

		 // @parm [in] The code format.
		 aafEdgeType_t  codeFormat,

		 // @parm [in] The Edgecode's 8-byte header
         aafEdgecodeHeader_t  header);



  //****************
  // ValidateTimecodeRange()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SpecifyValidCodeRange
        (// @parm [in] Data kind for the slot to be added
		 ImplAAFDataDef * pEssenceKind,

		 // @parm [in] SlotID for the slot to be added
		 aafSlotID_t  slotID,

		 // @parm [in] Edit rate for the slot to be added
         aafRational_t  editrate,

		 // @parm [in] Start offset for the slot to be added
		 aafFrameOffset_t  startOffset,

		 // @parm [in] Duration of the Source Clip in the slot
         aafFrameLength_t  length32);


  //****************
  // NewPhysSourceRef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    NewPhysSourceRef
        (// @parm [in] Edit rate of slot to contain reference
		 const aafRational_t & editrate,

		 // @parm [in] SlotID of slot to contain reference
		 aafSlotID_t  aMobSlot,

		 // @parm [in] Data kind of slot to contain reference
         const aafUID_t & essenceKind,

		 aafSourceRef_t  ref,

		 // @parm [in] Length of the Source Clip
         aafLength_t  srcRefLength);

  //****************
  // AddPhysSourceRef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendPhysSourceRef
        (// @parm [in] Edit rate of slot to contain reference
		 const aafRational_t & editrate,

		 // @parm [in] SlotID of slot to contain reference
		 aafSlotID_t  aMobSlot,

		 // @parm [in] Data kind of slot to contain reference
         const aafUID_t & essenceKind,

		 aafSourceRef_t  ref,

		 // @parm [in] Length of the Source Clip
         aafLength_t  srcRefLength);

  //****************
  // AddPulldownRef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AddPulldownRef
        (aafAppendOption_t  addType,
		
		// @parm [in] Edit rate of slot to contain reference
		 const aafRational_t & editrate,

		 // @parm [in] SlotID of slot to contain reference
		 aafSlotID_t  aMobSlot,

		 // @parm [in] Data kind of slot to contain reference
         const aafUID_t & essenceKind,

		 aafSourceRef_t  ref,

		 // @parm [in] Length of the Source Clip in the Source Mob
         aafLength_t  srcRefLength,

		 // @parm [in] Method of conversion
		 aafPulldownKind_t  pulldownKind,

		 // @parm [in] phase of first frame
         aafPhaseFrame_t  phaseFrame,

		 // @parm [in] Direction of the pulldown conversion.
		 aafPulldownDir_t  direction);


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

    // @parm [in] aafOperationChoice_t * | pOperationChoice | Effect Choice
    aafOperationChoice_t *  pOperationChoice,

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



public:

  // SDK Internal
  virtual AAFRESULT ReconcileMobLength(void);

private:
AAFRESULT FindTimecodeClip(
				aafFrameOffset_t	position,
				ImplAAFTimecode 	**result,
				aafFrameOffset_t	*tcStartPos,
				aafLength_t			*tcTrackLen);
AAFRESULT FindTimecodeSlot(
				ImplAAFSegment **result);

	OMStrongReferenceProperty<ImplAAFEssenceDescriptor>	_essenceDesc;
};

#endif // ! __ImplAAFSourceMob_h__


//@doc
//@class    AAFMasterMob | Implementation class for AAFMasterMob
#ifndef __ImplAAFMasterMob_h__
#define __ImplAAFMasterMob_h__


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

class ImplAAFSourceClip;

class ImplAAFSourceMob;

class ImplAAFComponent;

class ImplAAFDataDef;

class ImplAAFFindSourceInfo;

class ImplAAFEssenceAccess;

class ImplAAFLocator;

#include "AAF.h"

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
		 const aafUID_t & dataDef,

		 // @parm [in] Slot ID of the Source Mob slot to be added to the Master Mob
         aafSlotID_t  sourceSlotID,

		 // @parm [in] Source Mob containing the slot to be added to the Master Mob
         ImplAAFSourceMob * pSourceMob,

		 // @parm [in] SlotID assigned to the new Master Mob slot
		 aafSlotID_t  masterSlotID,

		 // @parm [in, string] Name to assign to new slot in Master Mob
		 const aafWChar *  pSlotName);



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
		 aafUInt32  index,

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

    // @parm [in] aafOperationChoice_t * | pOperationChoice | Operation Choice
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
  // Create()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CreateEssence
        (// @parm [in] 
         aafSlotID_t  masterSlotID,

         // @parm [in] create essence of this type
         aafUID_t	mediaKind,

 		 aafUID_t			codecID,
		 aafRational_t	editRate,
		 aafRational_t	sampleRate,

         // @parm [in] optionally compressing it
         aafCompressEnable_t  Enable,
		ImplAAFLocator		*destination,
		aafUID_t			fileFormat,
		ImplAAFEssenceAccess **result);
	//@comm Creates a single channel stream of essence.  Convenience functions
	// exist to create audio or video essence, and a separate call
	// (MultiCreate) exists to create interleaved audio and
	// video data.
	//@comm The essence handle from this call can be used with
	// WriteDataSamples  and possibly WriteDataLines, but NOT with
	// WriteMultiSamples.
	//@comm If you are creating the essence, and then attaching it to a master
	// mob, then the "masterMob" field may be left NULL.
	// For video, the sampleRate should be the edit rate of the file mob.
	// For audio, the sample rate should be the actual samples per second.
	//@comm Replaces omfmMediaCreate
	
/****/
  //****************
  // MultiCreate()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CreateMultiEssence
        (
 							aafUID_t codecID,
                          aafInt16  /*arrayElemCount*/,
                           aafmMultiCreate_t *  /*mediaArray*/,
                           aafCompressEnable_t  /*Enable*/,
							ImplAAFLocator		*destination,
							aafUID_t			fileFormat,
							IAAFEssenceMultiAccess **result);
	//@comm The essence handle from this call can be used with
	// WriteDataSamples or WriteMultiSamples but NOT with 
	// or WriteDataLines.
	//@comm If you are creating the essence, and then attaching it to a master
	// mob, then the "masterMob" field may be left NULL.
	//@comm Replaces omfmMediaMultiCreate

    //****************
  // Open()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    OpenEssence
        (// @parm [in] On this slot
         aafSlotID_t  slotID,

         // @parm [in] using this essence criteria
         aafMediaCriteria_t*  mediaCrit,

         // @parm [in] ReadOnly or Append
         aafMediaOpenMode_t  openMode,

         // @parm [in] optionally decompressing
         aafCompressEnable_t  compEnable,
		ImplAAFEssenceAccess **result);
	//@comm If the essence is interleaved,
	// then it will be di-interleaved when samples are read.  This routine
	// follows the locator, and may call the locator failure callback if
	// the essence can not be found.  If the failure callback finds the essence,
	// then this routine will return normally.
	//@comm The essence handle from this call can be used with
	// ReadDataSamples  and possibly ReadDataLines, but NOT with
	// ReadMultiSamples.
	//@comm Possible Errors:
	// 	Standard errors (see top of file).
	// 	OM_ERR_NOMEMORY -- couldn't allocate memory for the essence handle
	//@comm NOTE: If a locator is followed, then essencePtr may reference ANOTHER file
	// object, which must be closed on file close.
	//@comm Replaces omfmMediaOpen*/
	
/****/
  //****************
  // MultiOpen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    OpenMultiEssence
        ( // @parm [in] On this slot
         aafSlotID_t  slotID,

         // @parm [in] using this essence criteria
         aafMediaCriteria_t*  mediaCrit,

         // @parm [in] ReadOnly or Append
         aafMediaOpenMode_t  openMode,

         // @parm [in] optionally decompressing
         aafCompressEnable_t  compEnable,
		IAAFEssenceMultiAccess **result);
	//@comm This routine
	// follows the locator, and may call the locator failure callback if
	// the essence can not be found.  If the failure callback finds the essence,
	// then this routine will return normally.
	//@comm The essence handle from this call can be used with
	// WriteDataSamples or WriteMultiSamples but NOT with 
	//  WriteDataLines.
	//@comm Possible Errors:
	// 	Standard errors (see top of file).
	// 	OM_ERR_NOMEMORY -- couldn't allocate memory for the essence handle
	//@comm Replaces omfmMediaMultiOpen*/

  /****/
  //****************
  // GetNumChannels()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetNumChannels
        (// @parm [in] On this slot
         aafSlotID_t  slotID,

         // @parm [in] using this essence criteria
         aafMediaCriteria_t*  mediaCrit,

         // @parm [in] for this essence type
         aafUID_t mediaKind,

         // @parm [out] How many channels?
         aafInt16*  numCh);
	//@comm Returns the number of interleaved essence channels of a given type in the essence stream referenced by the given file mob
	//@comm If the data format is not interleaved, then the answer will
	// always be zero or one.  This function correctly returns zero
	// for essence types not handled by a given codec, and handles codecs
	// which work with multiple essence types.
	//@comm Replaces omfmGetNumChannels*/

public:

  // Internal to the SDK
  virtual AAFRESULT ReconcileMobLength(void);
};

#endif // ! __ImplAAFMasterMob_h__


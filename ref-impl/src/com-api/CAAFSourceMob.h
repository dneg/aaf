//@doc
//@class    AAFSourceMob | Implementation class for AAFSourceMob
#ifndef __CAAFSourceMob_h__
#define __CAAFSourceMob_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
// The Initial Developer of the Original Code of this file and the 
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=



#ifndef __AAF_h__
#include "AAF.h"
#endif










#ifndef __CAAFMob_h__
#include "CAAFMob.h"
#endif


class CAAFSourceMob
  : public IAAFSourceMob,
    public IAAFSearchSource,
    public CAAFMob
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFSourceMob (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFSourceMob ();

public:

  //***********************************************************
  //
  // Initialize()
  //
  // Initializes a newly allocated, empty IAAFSourceMob-supporting
  // object.  This method must be called after allocation, and before
  // any other method can be called.
  //
  // Succeeds if:
  // - Initialize() has not yet been called on this object.
  //
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_ALREADY_INITIALIZED
  //   - Initialize() has already been called on this object.
  //
  STDMETHOD (Initialize)
     ();



  //***********************************************************
  //
  // GetEssenceDescriptor()
  //
  // Places the Essence Descriptor object attached to this Mob into the
  // *ppEssence argument.  If none exists yet, NULL is placed into the
  // *ppEssence argument.
  //
  // The returned essence descriptor object, if it exists, is
  // AddRef()ed before it is returned.
  //
  // Succeeds if all of the following are true:
  // - the ppEssence pointer is valid.
  // - A valid essence descriptor exists.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - ppEssence is null.
  //
  // AAFRESULT_NO_ESSENCE_DESC
  //   - There is no essence descriptor.  There has to be one of some
  //     kind for this to be a valid Mob.
  //
  STDMETHOD (GetEssenceDescriptor) (
    // Returned Essence Descriptor object 
    /*[out]*/ IAAFEssenceDescriptor ** ppEssence);


  //***********************************************************
  //
  // SetEssenceDescriptor()
  //
  // Sets the Essence Descriptor of this Mob to be the given one.
  //
  // Succeeds if all of the following are true:
  // - the pEssence pointer is valid.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pEssence is null.
  //
  STDMETHOD (SetEssenceDescriptor) (
    // Essence Descriptor object 
    /*[in]*/ IAAFEssenceDescriptor * pEssence);


  //***********************************************************
  //
  // AddNilReference()
  //
  // This function adds a slot containing a NIL [sourceID 0.0....]
  // Source Clip.  This special SourceID indicates that the mob chain
  // ends here, which indicates that no record exists of what the
  // essence was derived from.  Some AAFSourceClip is still required
  // on the track to indicate that the track exists, and may be
  // referenced from other Mobs.
  //
  // Examples of Source Mobs that are not derived from a previous
  // source of essence are: Tape Source Mobs that were not created
  // from film; File Source Mobs whose digital essence data was
  // originally generated by computer and was not digitized from
  // videotape.
  //
  // Succeeds if all of the following are true:
  // - The pDataDef parameter is valid.
  // - editRate is valid.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NULL_PARAM
  //   - the pDataDef parameter is NULL.
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_BADRATE
  //   - the editRate is not valid.
  //
  STDMETHOD (AddNilReference) (
    // SlotID to be assigned to the new slot
    /*[in]*/ aafSlotID_t  slotID,

    // Duration of the Source Clip to be added to the new slot
    /*[in]*/ aafLength_t  length,

    // Data definition of the new slot
    /*[in]*/ IAAFDataDef * pDataDef,

    // Edit rate of the new slot
    /*[in]*/ aafRational_t  editRate);


  //***********************************************************
  //
  // AppendTimecodeSlot()
  //
  // This function adds a Timecode slot to a specified tape Mob or
  // film Mob, with a specified starting timecode, length, and edit
  // rate.  Your must also call SpecifyValidCodeRange to add the
  // Filler to the other essence slots to indicate that the Timecode
  // is valid for that channel.
  // 
  // Note: The startTC parameter is expressed in frames since
  // midnight.
  // 
  // The length32 parameter can be the value FULL_RANGE, in which
  // case the length is 24 hours.
  //
  // Succeeds if all of the following are true:
  // - The specified slot ID is not yet used.
  // - This source mob references an AAFTapeDescriptor as an essence
  //   descriptor.
  // 
  // If this method fails no state is changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_SLOT_EXISTS
  //   - The specified slotID already exists.
  //
  // AAFRESULT_TAPEDESC_ONLY
  //   - Valid only for AAFSourceMob referencing an AAFTapeDescriptor
  //     as EssenceDescriptor.
  //
  STDMETHOD (AppendTimecodeSlot) (
    // Edit rate of Timecode slot
    /*[in]*/ aafRational_t  editrate,

    // SlotID of Timecode slot
    /*[in]*/ aafInt32  slotID,

    // Starting time code
    /*[in]*/ aafTimecode_t  startTC,

    // Duration of Timecode.
    /*[in]*/ aafFrameLength_t  length32);


  //***********************************************************
  //
  // AppendEdgecodeSlot()
  //
  // Adds an Edgecode slot to a specified film Mob, with a specified
  // starting edgecode, length, and edit rate. You must add a essence
  // slot with SpecifyValidCodeRange to make the edgecode slot valid.
  //
  // Succeeds if all of the following are true:
  // - The specified slot ID is not yet used.
  // - This source mob references an AAFFilmDescriptor as an essence
  //   descriptor.
  // 
  // If this method fails no state is changed.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_SLOT_EXISTS
  //   - The specified slotID already exists.
  //
  // AAFRESULT_FILMDESC_ONLY
  //   - Valid only for AAFSourceMob referencing an AAFFilmDescriptor
  //   as EssenceDescriptor.
  //
  STDMETHOD (AppendEdgecodeSlot) (
    // Edit rate of the Edgecode slot
    /*[in]*/ aafRational_t  editrate,

    // SlotID to assign to the new Edgecode slot
    /*[in]*/ aafInt32  slotID,

    // Starting Edgecode
    /*[in]*/ aafFrameOffset_t  startEC,

    // Length of the Edgecode component in the slot
    /*[in]*/ aafFrameLength_t  length32,

    // The film kind.  Can be one of:
	// - kFtNull
    // - kFt35MM
    // - kFt16MM
    // - kFt8MM
    // - kFt65MM
    /*[in]*/ aafFilmType_t  filmKind,

    // The code format.  Can be one of:
    // - kEtNull
    // - kEtKeycode
    // - kEtEdgenum4
    // - kEtEdgenum5
    /*[in]*/ aafEdgeType_t  codeFormat,

    // The Edgecode's 8-byte header
    /*[in]*/ aafEdgecodeHeader_t  header);


  //***********************************************************
  //
  // SpecifyValidCodeRange()
  //
  // Adds slot containing Source Clips to a Source Mob to indicate
  // that the Timecode or Edgecode is valid for that channel.
  //
  // Note: The pEssenceKind parameter requires a data kind valid for a
  // essence stream.  Valid data kinds are:
  // - Picture
  // - Sound
  //
  // Succeeds if all of the following are true:
  // - the pEssenceKind pointer is valid.
  // - The specified slot ID is not yet used.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pEssenceKind is null.
  //
  // AAFRESULT_SLOT_EXISTS
  //   - The specified slotID already exists.
  //
  STDMETHOD (SpecifyValidCodeRange) (
    // Data kind for the slot to be added
    /*[in]*/ IAAFDataDef * pEssenceKind,

    // SlotID for the slot to be added
    /*[in]*/ aafSlotID_t  slotID,

    // Edit rate for the slot to be added
    /*[in]*/ aafRational_t  editrate,

    // Start offset for the slot to be added
    /*[in]*/ aafFrameOffset_t  startOffset,

    // Duration of the Source Clip in the slot
    /*[in]*/ aafFrameLength_t  length32);


  //***********************************************************
  //
  // AppendPhysSourceRef()
  //
  // Connects this Source Mob with the physical Source Mob that
  // describes the previous generation of essence, appending it to
  // existing Mob data.  If a physical Source Mob, such as a File
  // Source Mob or tape Source Mob, references another physical
  // Source Mob as its ancestor, with no pulldown, then this
  // function makes the connection between the two.
  //
  // Functionally, this is a helper function to create a slot with an
  // AAFSourceClip referencing a particular piece of media.  This
  // function takes many parameters because the components of an
  // aafSourceRef_t have been broken out as separate parameters.
  //
  // The ancestor of an AAFSourceMob with an AAFFileDescriptor is often an
  // AAFTapeDescriptor or NIL.
  //
  // Succeeds if all of the following are true:
  // - the pSourceRefObj pointer is valid.
  // - the pEssenceKind pointer is valid.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pSourceRefObj or pEssenceKind is null.
  //
  STDMETHOD (AppendPhysSourceRef) (
    // Edit rate of slot to contain reference
    /*[in]*/ aafRational_t  editrate,

    // SlotID of slot to contain reference
    /*[in]*/ aafSlotID_t  aMobSlot,

    // Data kind of slot to contain reference.  Requires a data kind
	// valid for a essence stream.  Valid data kinds are:
    // - Picture
    // - Sound
    /*[in]*/ IAAFDataDef * pEssenceKind,

    // Reference to a Physical Source Mob
    /*[in]*/ aafSourceRef_t  ref,

    // Length of the Source Clip
    /*[in]*/ aafLength_t  srcRefLength);


  //***********************************************************
  //
  // NewPhysSourceRef()
  //
  // Connects this Source Mob with the physical Source Mob that
  // describes the previous generation of essence, replacing any
  // existing Mob data.  If a physical Source Mob, such as a File
  // Source Mob or tape Source Mob, references another physical
  // Source Mob as its ancestor, with no pulldown, then this
  // function makes the connection between the two.
  //
  // Functionally, this is a helper function to create a slot with an
  // AAFSourceClip referencing a particular piece of media.  This
  // function takes many parameters because the components of an
  // aafSourceRef_t have been broken out as separate parameters.
  //
  // The ancestor of an AAFSourceMob with an AAFFileDescriptor is often an
  // AAFTapeDescriptor or NIL.
  //
  // Succeeds if all of the following are true:
  // - the pSourceRefObj pointer is valid.
  // - the pEssenceKind pointer is valid.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pSourceRefObj or pEssenceKind is null.
  //
  STDMETHOD (NewPhysSourceRef) (
    // Edit rate of slot to contain reference
    /*[in]*/ aafRational_t  editrate,

    // SlotID of slot to contain reference
    /*[in]*/ aafSlotID_t  aMobSlot,

    // Data kind of slot to contain reference.  Requires a data kind
	// valid for a essence stream.  Valid data kinds are:
    // - Picture
    // - Sound
    /*[in]*/ IAAFDataDef * pEssenceKind,

    // Reference to a Physical Source Mob
    /*[in]*/ aafSourceRef_t  ref,

    // Length of the Source Clip
    /*[in]*/ aafLength_t  srcRefLength);


  //***********************************************************
  //
  // AddPulldownRef()
  //
  // Connects the specified Source Mob with the physical Source Mob
  // that describes the previous generation of essence, with an
  // additional AAFPulldown as part of the reference to indicate a non
  // 1-1 relationship Between the two. 
  //
  // Functionally, this is a helper function to create a slot with an
  // AAFPulldown object which references an AAFSourceClip, which
  // references a particular piece of media.  This function takes many
  // parameters because the components of an aafSourceRef_t and the
  // AAFPulldown object have been broken out as separate parameters.
  //
  // The ancestor of an AAFSourceMob with an AAFTapeDescriptor is
  // often an AAFFilmDescriptor or NIL.
  //
  // Succeeds if all of the following are true:
  // - the pSourceRefObj pointer is valid.
  // - the pEssenceKind pointer is valid.
  // - a valid pulldown direction was specified.
  // 
  // This method will return the following codes.  If more than one of
  // the listed errors is in effect, it will return the first one
  // encountered in the order given below:
  // 
  // AAFRESULT_SUCCESS
  //   - succeeded.  (This is the only code indicating success.)
  //
  // AAFRESULT_NOT_INITIALIZED
  //   - This object has not yet had Initialize() called on it.
  //
  // AAFRESULT_NULL_PARAM
  //   - pSourceRefObj or pEssenceKind is null.
  //
  // AAFRESULT_PULLDOWN_DIRECTION
  //   - an invalid pulldown direction was specified.
  //
  STDMETHOD (AddPulldownRef) (
    // Tells whether to overwrite an existing slot segment, or create
	// a sequence and append
    /*[in]*/ aafAppendOption_t  addType,

    // Edit rate of slot to contain reference
    /*[in]*/ aafRational_t  editrate,

    // SlotID of slot to contain reference
    /*[in]*/ aafSlotID_t  aMobSlot,

    // Data kind of slot to contain reference.  Requires a data kind
	// valid for a essence stream.  Valid data kinds are:
    // - Picture
    // - Sound
    /*[in]*/ IAAFDataDef * pEssenceKind,

    // Reference to a Physical Source Mob
    /*[in]*/ aafSourceRef_t  ref,

    // Length of the Source Clip in the Source Mob
    /*[in]*/ aafLength_t  srcRefLength,

    // Method of conversion.  Possible values are:
    // - kAAFTwoThreePD   	-- Normal NTSC-20fps pulldown
    // - kAAFPALPD
    // - kAAFOneToOneNTSC   -- NTSC recorded as 1 frame == 1 film frame.
    // - kAAFOneToOnePAL    -- PAL recorded as 1 frame == 1 film frame.
    /*[in]*/ aafPulldownKind_t  pulldownKind,

    // phase of first frame
    /*[in]*/ aafPhaseFrame_t  phaseFrame,

    // Direction of the pulldown conversion. Possible values are:
    //	- kAAFTapeToFilmSpeed -- Used to link a file descriptor with a
    //							 tape descriptor.
    //	- kAAFFilmToTapeSpeed -- Used to link a tape descriptor with a
    //							 film descriptor.
    /*[in]*/ aafPulldownDir_t  direction);
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
  // iteratively; use the MobOpenSearch, MobGetNextSource,
  // MobGetThisSource, and MobCloseSearch functions for those
  // purposes. 
  //
  // The returned component and find source info are AddRef()ed
  // before they are returned.
  //
  // Succeeds if all of the following are true:
  // - ppSourceInfo is non-NULL
  // - a Mob of the requested kind is found
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
  // OM_ERR_INVALID_MOBTYPE
  //	- The enumerator is out of range (bad cast, or writing
  //      toolkit newer than reader)
  //
  // OM_ERR_TRAVERSAL_NOT_POSS
  //	- Can not find a mob of the given kind.
  // @end
  // 
  STDMETHOD (SearchSource)
   (
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
    IAAFFindSourceInfo ** ppSourceInfo
  );


protected:
  // 
  // Declare the QI that implements for the interfaces
  // for this module. This will be called by CAAFUnknown::QueryInterface().
  // 
  STDMETHOD(InternalQueryInterface)(REFIID riid, void **ppvObjOut);


public:
  //
  // This class as concrete. All AAF objects can be constructed from
  // a CLSID. This will allow subclassing all "base-classes" by
  // aggreggation.
  // 
  AAF_DECLARE_CONCRETE();
  //
  //********
};

#endif // ! __CAAFSourceMob_h__



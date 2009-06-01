//@doc
//@class    AAFSourceReference | Implementation class for AAFSourceReference
#ifndef __ImplAAFSourceReference_h__
#define __ImplAAFSourceReference_h__

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


#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif

#include "ImplAAFObject.h"

#include "OMArrayProperty.h"
#include "OMStorable.h"

class ImplAAFSourceReference : public ImplAAFSegment
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFSourceReference ();
  virtual ~ImplAAFSourceReference ();



  //****************
  // GetSourceID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSourceID
		// @parm [retval][out] Place to put source ID
        (aafMobID_t *  pSourceID);

  //****************
  // SetSourceID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetSourceID
		// @parm [in] Source ID to set
        (aafMobID_constref   sourceID);

  //****************
  // GetSourceMobSlotID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSourceMobSlotID
		// @parm [retval][out] Place to put source mob slot ID
        (aafSlotID_t *  pMobSlotID);

  //****************
  // SetSourceMobSlotID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetSourceMobSlotID
		// @parm [in] Source Mob ID to set
        (aafSlotID_t   mobSlotID);


  //****************
  // SetChannelIDs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetChannelIDs
        (// @parm [in] Number of elements in the pChannelIDs array
         aafUInt32  numberElements,

         // @parm [in] Array of channel IDs
         aafUInt32*  pChannelIDs);


  //****************
  // GetChannelIDs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetChannelIDs
        (// @parm [in] Number of elements in the pChannelIDs array
         aafUInt32  numberElements,

         // @parm [in] Array of channel IDs
         aafUInt32*  pChannelIDs);

  //****************
  // GetChannelIDsSize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetChannelIDsSize
        // @parm [out] Number of elements in the pChannelIDs array
        (aafUInt32 *  numberElements);


  //****************
  // SetMonoSourceSlotIDs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetMonoSourceSlotIDs
        (// @parm [in] Number of elements in the pMonoSourceSlotIDs array
         aafUInt32  numberElements,

         // @parm [in] Array of slot IDs
         aafUInt32*  pMonoSourceSlotIDs);


  //****************
  // GetMonoSourceSlotIDs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetMonoSourceSlotIDs
        (// @parm [in] Number of elements in the pMonoSourceSlotIDs array
         aafUInt32  numberElements,

         // @parm [in] Array of channel IDs
         aafUInt32*  pMonoSourceSlotIDs);

  //****************
  // GetMonoSourceSlotIDsSize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetMonoSourceSlotIDsSize
        // @parm [out] Number of elements in the pMonoSourceSlotIDs array
        (aafUInt32 *  numberElements);


public:
	//SDK-private

	virtual AAFRESULT ChangeContainedReferences(aafMobID_constref from,
												aafMobID_constref to);

private:
	OMFixedSizeProperty<aafMobID_t>	_sourceID;
	OMFixedSizeProperty<aafInt32>	_sourceMobSlotId;
	OMArrayProperty<aafUInt32> _channelIDs;
	OMArrayProperty<aafUInt32> _monoSourceSlotIDs;

};

#endif // ! __ImplAAFSourceReference_h__


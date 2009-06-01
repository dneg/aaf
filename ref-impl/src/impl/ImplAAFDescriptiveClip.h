//@doc
//@class    AAFDescriptiveClip | Implementation class for AAFDescriptiveClip
#ifndef __ImplAAFDescriptiveClip_h__
#define __ImplAAFDescriptiveClip_h__

//=---------------------------------------------------------------------=
//
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










#ifndef __ImplAAFSourceClip_h__
#include "ImplAAFSourceClip.h"
#endif

#include "OMSetProperty.h"

class ImplAAFDescriptiveClip : public ImplAAFSourceClip
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFDescriptiveClip ();

protected:
  virtual ~ImplAAFDescriptiveClip ();

public:


  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
        (// @parm [in] Data Definition object
         ImplAAFDataDef *    pDataDef,

         // @parm [in] Length property value
         const aafLength_t &    length,

         // @parm [in] Source Reference
         const aafSourceRef_t & sourceRef);

  //****************
  // CountDescribedSlotIDs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountDescribedSlotIDs
        // @parm [out, retval] Number of described slot IDs
        (aafUInt32*  pCount);


  //****************
  // GetDescribedSlotIDs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDescribedSlotIDs
        (// @parm [in] The size of the given pDescribedSlotIDs buffer
         aafUInt32  maxDescribedSlotIDCount,

         // @parm [out, size_is(maxDescribedSlotIDsCount)] Array to hold the described slot IDs
         aafUInt32 *  pDescribedSlotIDs);


  //****************
  // IsDescribedSlotIDPresent()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    IsDescribedSlotIDPresent
        (// @parm [in, ref] Described slot ID whose presence is to be queried
         aafUInt32  describedSlotID,

         // @parm [out,retval] True if describedSlotID is present
         aafBoolean_t*  pIsPresent);


  //****************
  // AddDescribedSlotID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AddDescribedSlotID
        // @parm [in] Described slot ID to add
        (aafUInt32  describedSlotID);


  //****************
  // RemoveDescribedSlotID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RemoveDescribedSlotID
        // @parm [in] Described slot ID to remove
        (aafUInt32  describedSlotID);





private:
	OMSetProperty<aafUInt32> _describedSlotIDs;
	
};

#endif // ! __ImplAAFDescriptiveClip_h__



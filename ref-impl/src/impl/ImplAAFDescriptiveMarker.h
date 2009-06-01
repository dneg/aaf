//@doc
//@class    AAFDescriptiveMarker | Implementation class for AAFDescriptiveMarker
#ifndef __ImplAAFDescriptiveMarker_h__
#define __ImplAAFDescriptiveMarker_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
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

#ifndef __ImplAAFCommentMarker_h__
#include "ImplAAFCommentMarker.h"
#endif

#include "ImplAAFSourceReference.h"

#include "OMSetProperty.h"
#include "OMStrongRefProperty.h"

class ImplAAFDescriptiveFramework;


class ImplAAFDescriptiveMarker : public ImplAAFCommentMarker
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFDescriptiveMarker ();

protected:
  virtual ~ImplAAFDescriptiveMarker ();

public:


  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize ();



  //****************
  // SetDescribedSlotIDs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetDescribedSlotIDs
        (// @parm [in] Number of elements in the pDescribedSlotIDs array
         aafUInt32  numberElements,

         // @parm [in] Array of slot IDs
         aafUInt32*  pDescribedSlotIDs);


  //****************
  // GetDescribedSlotIDs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDescribedSlotIDs
        (// @parm [in] Number of elements in the pDescribedSlotIDs array
         aafUInt32  numberElements,

         // @parm [in] Array of channel IDs
         aafUInt32*  pDescribedSlotIDs);

  //****************
  // GetDescribedSlotIDsSize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDescribedSlotIDsSize
        // @parm [out] Number of elements in the pDescribedSlotIDs array
        (aafUInt32 *  numberElements);

  //****************
  // SetDescriptiveFramework()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetDescriptiveFramework
        // @parm [in] DescriptorFramework object to set
        (ImplAAFDescriptiveFramework * pDescriptiveFramework);

  //****************
  // GetDescriptiveFramework()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetDescriptiveFramework
        // @parm [out,retval] DescriptorFramework object to return
        (ImplAAFDescriptiveFramework ** ppDescriptiveFramework);

 private:
    OMSetProperty<aafUInt32> _describedSlots;
    OMStrongReferenceProperty<ImplAAFDescriptiveFramework> _descriptiveFramework;
    
};

#endif // ! __ImplAAFDescriptiveMarker_h__

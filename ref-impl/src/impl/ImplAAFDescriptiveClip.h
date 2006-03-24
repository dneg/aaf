//@doc
//@class    AAFDescriptiveClip | Implementation class for AAFDescriptiveClip
#ifndef __ImplAAFDescriptiveClip_h__
#define __ImplAAFDescriptiveClip_h__

//=---------------------------------------------------------------------=
//
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
// The Original Code of this file is Copyright 1998-2005, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
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
  // SetSourceTrackIDs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetSourceTrackIDs
        (// @parm [in] Number of elements in the pSourceTrackIDs array
         aafUInt32  numberElements,

         // @parm [in] Array of source track IDs
         aafUInt32*  pSourceTrackIDs);


  //****************
  // GetSourceTrackIDs()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSourceTrackIDs
        (// @parm [in] Number of elements in the pSourceTrackIDs array
         aafUInt32  numberElements,

         // @parm [in] Array of channel IDs
         aafUInt32*  pSourceTrackIDs);

  //****************
  // GetSourceTrackIDsSize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSourceTrackIDsSize
        // @parm [out] Number of elements in the pSourceTrackIDs array
        (aafUInt32 *  numberElements);

private:
	OMSetProperty<aafUInt32> _sourceTrackIDs;
	
};

#endif // ! __ImplAAFDescriptiveClip_h__



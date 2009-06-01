//@doc
//@class    AAFEdgecode | Implementation class for AAFEdgecode
#ifndef __ImplAAFEdgecode_h__
#define __ImplAAFEdgecode_h__


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


/***********************************************\
*	Stub only.   Implementation not yet added	*
\***********************************************/

#ifndef __ImplAAFSegment_h__
#include "ImplAAFSegment.h"
#endif
#include "OMVariableSizeProperty.h"

class ImplAAFEdgecode : public ImplAAFSegment
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFEdgecode ();

protected:
  virtual ~ImplAAFEdgecode ();

public:


  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize
        (// @parm [in] Length Property Value
         aafLength_t    length,

         // @parm [in] Edgecode Value
         aafEdgecode_t  edgecode);

  //****************
  // GetEdgecode()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetEdgecode
        // @parm [out] Edgecode
        (aafEdgecode_t *  edgecode);

private:
	OMFixedSizeProperty<aafPosition_t>			_start;
	OMFixedSizeProperty<aafFilmType_t>			_filmType;
	OMFixedSizeProperty<aafEdgeType_t>			_edgeType;
	OMVariableSizeProperty<aafUInt8>			_header;

};

#endif // ! __ImplAAFEdgecode_h__



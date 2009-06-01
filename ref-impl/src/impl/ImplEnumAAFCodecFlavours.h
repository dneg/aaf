//@doc
//@class    EnumAAFCodecFlavours | Implementation class for EnumAAFCodecFlavours
#ifndef __ImplEnumAAFCodecFlavours_h__
#define __ImplEnumAAFCodecFlavours_h__


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

#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif

#include "AAFPlugin.h"

class ImplEnumAAFCodecFlavours : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplEnumAAFCodecFlavours ();

protected:
  virtual ~ImplEnumAAFCodecFlavours ();

public:


  //****************
  // NextOne()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    NextOne
        // @parm [out,retval] The Next flavour code
        (aafUID_t *  pAAFCodecFlavour);

  //****************
  // Next()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Next
        (// @parm [in] number of AAFPluggableDefs requested
         aafUInt32  count,

         // @parm [out, size_is(count), length_is(*pFetched)] array to receive flavour codes
         aafUID_t *  pAAFCodecFlavours,

         // @parm [out,ref] number of actual flavour IDs fetched into pAAFCodecFlavours array
         aafUInt32 *  pFetched);

  //****************
  // Skip()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Skip
        // @parm [in] Number of elements to skip
        (aafUInt32  count);

  //****************
  // Reset()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Reset ();


  //****************
  // Clone()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Clone
        // @parm [out,retval] new enumeration
        (ImplEnumAAFCodecFlavours ** ppEnum);

public:
	// SDK-Internal
	AAFRESULT STDMETHODCALLTYPE
		SetEnumCodec( IAAFEssenceCodec *pCodec);

private:
	aafUInt32			_current;
	IAAFEssenceCodec	*_codec;
};

#endif // ! __ImplEnumAAFCodecFlavours_h__



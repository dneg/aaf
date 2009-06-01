//@doc
//@class    AAFHTMLClip | Implementation class for AAFHTMLClip
#ifndef __ImplAAFHTMLClip_h__
#define __ImplAAFHTMLClip_h__


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



#ifndef __ImplAAFTextClip_h__
#include "ImplAAFTextClip.h"
#endif

#include "OMWideStringProperty.h"

class ImplAAFHTMLClip : public ImplAAFTextClip
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFHTMLClip ();

protected:
  virtual ~ImplAAFHTMLClip ();

public:


  //****************
  // GetBeginAnchor()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetBeginAnchor
        (// @parm [in,string] Begin Anchor Name
         aafCharacter *  pName,

         // @parm [in] length of the buffer to hold Begin Anchor Name
         aafInt32  bufSize);

  //****************
  // GetBeginAnchorBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetBeginAnchorBufLen
        // @parm [out] Length in bytes
        (aafUInt32 *  pLen);

  //****************
  // SetBeginAnchor()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetBeginAnchor
        // @parm [in, string] Specifies the HTML tag that defines the start of the text
        (const aafCharacter *  pName);

  //****************
  // GetEndAnchor()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetEndAnchor
        (// @parm [in,string] End Anchor Name
         aafCharacter *  pName,

         // @parm [in] length of the buffer to hold End Anchor Name
         aafInt32  bufSize);

  //****************
  // GetEndAnchorBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetEndAnchorBufLen
        // @parm [out] Length in bytes
        (aafUInt32 *  pLen);

  //****************
  // SetEndAnchor()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetEndAnchor
        // @parm [in, string] Specifies the HTML tag that defines the end of the text
        (const aafCharacter *  pName);

protected:
  // Persistent properties...
  OMWideStringProperty _beginAnchor;
  OMWideStringProperty _endAnchor;
};

#endif // ! __ImplAAFHTMLClip_h__



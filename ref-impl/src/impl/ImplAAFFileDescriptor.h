//@doc
//@class    AAFFileDescriptor | Implementation class for AAFFileDescriptor
#ifndef __ImplAAFFileDescriptor_h__
#define __ImplAAFFileDescriptor_h__


//=---------------------------------------------------------------------=
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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#ifndef __ImplAAFEssenceDescriptor_h__
#include "ImplAAFEssenceDescriptor.h"
#endif
#include "OMWeakRefProperty.h"

#include "ImplAAFContainerDef.h"
#include "ImplAAFCodecDef.h"


class ImplAAFFileDescriptor : public ImplAAFEssenceDescriptor
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFFileDescriptor ();
  virtual ~ImplAAFFileDescriptor ();



//@access Public Members

  virtual AAFRESULT STDMETHODCALLTYPE
	Initialize ();


  //****************
  // SetLength()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetLength
		// @parm [in] length of the essence in samples
        (aafLength_t  length);


  //****************
  // GetLength()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetLength
		// @parm [out] returns length of the essence in samples
        (aafLength_t *  pLength);


  //****************
  // SetCodecDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetCodecDef
		// @parm [in] is this AAF or raw essence
        (ImplAAFCodecDef *pDef);


  //****************
  // GetCodecDef()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetCodecDef
		// @parm [out] is this AAF or raw medi
        (ImplAAFCodecDef **ppDef);


  //****************
  // SetSampleRate()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetSampleRate
		// @parm [in] sample rate of the essence
        (const aafRational_t & rate);


  //****************
  // GetSampleRate()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSampleRate
		// @parm [out] sample rate of the essence
        (aafRational_t*  pRate);


  //****************
  // SetContainerFormat()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetContainerFormat
		// @parm [in] Identifies the file format
        (ImplAAFContainerDef *pDef);


  //****************
  // GetContainerFormat()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetContainerFormat
		// @parm [out] Identifies the file format
        (ImplAAFContainerDef **ppDef);

public:
	// Functions internal to the toolkit
	virtual AAFRESULT STDMETHODCALLTYPE
		GetOwningMobKind (aafMobKind_t *pMobKind);

  // OM deep copy notification
  virtual void onCopy(void* clientContext) const;

private:
	OMFixedSizeProperty<aafRational_t>	_sampleRate;
	OMFixedSizeProperty<aafLength_t>	_length;
	OMWeakReferenceProperty<ImplAAFCodecDef> _codecDef;
	OMWeakReferenceProperty<ImplAAFContainerDef> _containerFmt;
};

#endif // ! __ImplAAFFileDescriptor_h__

//@doc
//@class    AAFTapeDescriptor | Implementation class for AAFTapeDescriptor
#ifndef __ImplAAFTapeDescriptor_h__
#define __ImplAAFTapeDescriptor_h__

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


#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif

#ifndef __ImplAAFEssenceDescriptor_h__
#include "ImplAAFEssenceDescriptor.h"
#endif

#include "OMWideStringProperty.h"



class ImplAAFTapeDescriptor : public ImplAAFEssenceDescriptor
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTapeDescriptor ();
  virtual ~ImplAAFTapeDescriptor ();


  virtual AAFRESULT STDMETHODCALLTYPE
	Initialize ();


  //****************
  // SetTapeManufacturer()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetTapeManufacturer
		// @parm [in,string] Manufacturers name
        (const aafCharacter*  pName);
  //****************
  // GetTapeManufacturer()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTapeManufacturer
	    (// @parm [in,string] pass in a buffer which is cleared and filled.
         aafCharacter*  pName,

		 // @parm [in] Length of the buffer to hold the Manufacturers name
		 aafUInt32  buflen);
  //****************
  // GetTapeManufacturerBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTapeManufacturerBufLen
		// @parm [out] Manufacturers Name length
        (aafUInt32 *  pLen);
  //****************
  // SetTapeModel()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetTapeModel
		// @parm [in,string] Tape Manufacturers Brand name
        (const aafCharacter*  pModelName);
  //****************
  // GetTapeModel()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTapeModel
	    (// @parm [in,string] pass in a buffer which is cleared and filled.
         aafCharacter*  pModelName,

		 // @parm [in] Length of the buffer to hold the Manufacturers Brand name
		 aafUInt32  buflen);
  //****************
  // GetTapeModelBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTapeModelBufLen
		// @parm [out] Model length
        (aafUInt32 *  pLen);

  //****************
  // SetTapeFormFactor()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetTapeFormFactor
		// @parm [in] ex: kAAFVHSVideoTape,kDATCartridge 
        (aafTapeCaseType_t  formFactor);
  //****************
  // GetTapeFormFactor()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTapeFormFactor
		// @parm [out] ex: kAAFVHSVideoTape,kDATCartridge
        (aafTapeCaseType_t*  formFactor);

  //****************
  // SetSignalType()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetSignalType
		// @parm [in] ex: kNTSCSignal
        (aafVideoSignalType_t  videoSignal);
  //****************
  // GetSignalType()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSignalType
		// @parm [out] ex: kNTSCSignal
        (aafVideoSignalType_t*  videoSignal);


  //****************
  // SetTapeFormat()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetTapeFormat
		// @parm [in] ex: kBetacamFormat,kBetacamSPFormat
        (aafTapeFormatType_t  tapeFormat);
  //****************
  // GetTapeFormat()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTapeFormat
		// @parm [out] ex: kBetacamFormat,kBetacamSPFormat
        (aafTapeFormatType_t*  tapeFormat);

  //****************
  // SetTapeLength()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetTapeLength
		// @parm [in] The length of the tape in minutes.
        (aafUInt32  tapeLength);
  //****************
  // GetTapeLength()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTapeLength
		// @parm [out] The length of the tape in minutes.
        (aafUInt32*  pTapeLength);



public:

  virtual AAFRESULT	STDMETHODCALLTYPE
		GetOwningMobKind (aafMobKind_t *pMobKind);

	// Persistent properties
private:
	OMFixedSizeProperty<aafTapeCaseType_t>		_formFactor;
	OMFixedSizeProperty<aafVideoSignalType_t>	_videoSignalType;
	OMFixedSizeProperty<aafTapeFormatType_t>	_tapeFormat;
	OMFixedSizeProperty<aafUInt32>				_tapeLength;
	OMWideStringProperty						_manufacturer;
	OMWideStringProperty						_model;
};

#endif // ! __ImplAAFTapeDescriptor_h__


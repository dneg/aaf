//@doc
//@class    AAFFilmDescriptor | Implementation class for AAFFilmDescriptor
#ifndef __ImplAAFFilmDescriptor_h__
#define __ImplAAFFilmDescriptor_h__


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

class ImplAAFFilmDescriptor : public ImplAAFEssenceDescriptor
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFFilmDescriptor ();

protected:
  virtual ~ImplAAFFilmDescriptor ();

public:


  //****************
  // SetFilmManufacturer()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetFilmManufacturer
        // @parm [in] 
        (const aafCharacter *  name);


  //****************
  // GetFilmManufacturer()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetFilmManufacturer
        // @parm [out] Pass in a string object which will be erased and filled
        (aafCharacter *  name, aafUInt32 bufSize);
	//@comm Returns a zero-length string if the property was not present

    virtual AAFRESULT STDMETHODCALLTYPE
    GetFilmManufacturerBufLen
         (aafUInt32 *bufSize);


  //****************
  // SetFilmModel()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetFilmModel
        // @parm [in] 
        (const aafCharacter *  name);


  //****************
  // GetFilmModel()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetFilmModel
        // @parm [out] Pass in a string object which will be erased and filled
        (aafCharacter *  name, aafUInt32 bufSize);

  virtual AAFRESULT STDMETHODCALLTYPE
    GetFilmModelBufLen
         (aafUInt32 *bufSize);


  //****************
  // GetFilmFormat()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetFilmFormat
        // @parm [out] filmFormat
        (aafFilmType_t*  filmFormat);
	//@comm Film format may be: kAAFFt35MM, kFt16MM, kFt8MM, kFt65MM


  //****************
  // GetFrameRate()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetFrameRate
        // @parm [out] Frame Rate
        (aafUInt32*  rate);


  //****************
  // GetPerfPerFrame()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPerfPerFrame
        // @parm [out] Perforations per frame
        (aafUInt8*  perfPerFrame);


  //****************
  // GetFilmAspectRatio()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetFilmAspectRatio
        // @parm [out] Film Aspect Ratio
        (aafRational_t*  aspectRatio);


  //****************
  // SetFilmFormat()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetFilmFormat
        // @parm [in] Film Format
        (aafFilmType_t  filmFormat);
	//@comm kAAFFt35MM, kFt16MM, kFt8MM, kFt65MM


  //****************
  // SetFrameRate()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetFrameRate
        // @parm [in] Frame Rate
        (aafUInt32  rate);


  //****************
  // SetPerfPerFrame()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPerfPerFrame
        // @parm [in] Perforations per frame
        (aafUInt8  perfPerFrame);


  //****************
  // SetFilmAspectRatio()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetFilmAspectRatio
        // @parm [in] Film Aspect Ratio
        (aafRational_t  aspectRatio);


public:

  virtual AAFRESULT	STDMETHODCALLTYPE
		GetOwningMobKind (aafMobKind_t *pMobKind);

private:
	OMWideStringProperty				_manufacturer;
	OMWideStringProperty				_model;
	OMFixedSizeProperty<aafFilmType_t>	_format;
	OMFixedSizeProperty<aafUInt32>		_frameRate;
	OMFixedSizeProperty<aafUInt8>		_perfPerFrame;
	OMFixedSizeProperty<aafRational_t>	_aspectRatio;
};

#endif // ! __ImplAAFFilmDescriptor_h__

//@doc
//@class    AAFFilmDescriptor | Implementation class for AAFFilmDescriptor
#ifndef __ImplAAFFilmDescriptor_h__
#define __ImplAAFFilmDescriptor_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

/***********************************************\
*	Stub only.   Implementation not yet added	*
\***********************************************/

#ifndef __AAFTypes_h__
#include "AAFTypes.h"
#endif






#ifndef __ImplAAFEssenceDescriptor_h__
#include "ImplAAFEssenceDescriptor.h"
#endif


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

//@access Public Members

/****/
  //****************
  // SetFilmManufacturer()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetFilmManufacturer
        // @parm [in] 
        (wchar_t *  name);

/****/
  //****************
  // GetFilmManufacturer()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetFilmManufacturer
        // @parm [out] Pass in a string object which will be erased and filled
        (wchar_t *  name, aafInt32 bufSize);
	//@comm Returns a zero-length string if the property was not present

    virtual AAFRESULT STDMETHODCALLTYPE
    GetFilmManufacturerBufLen
         (aafInt32 *bufSize);

/****/
  //****************
  // SetFilmModel()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetFilmModel
        // @parm [in] 
        (wchar_t *  name);

/****/
  //****************
  // GetFilmModel()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetFilmModel
        // @parm [out] Pass in a string object which will be erased and filled
        (wchar_t *  name, aafInt32 bufSize);

  virtual AAFRESULT STDMETHODCALLTYPE
    GetFilmModelBufLen
         (aafInt32 *bufSize);

/****/
  //****************
  // GetFilmFormat()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetFilmFormat
        // @parm [out] filmFormat
        (aafFilmType_t*  filmFormat);
	//@comm Film format may be: kFt35MM, kFt16MM, kFt8MM, kFt65MM

/****/
  //****************
  // GetFrameRate()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetFrameRate
        // @parm [out] Frame Rate
        (aafUInt32*  rate);

/****/
  //****************
  // GetPerfPerFrame()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPerfPerFrame
        // @parm [out] Perforations per frame
        (aafUInt8*  perfPerFrame);

/****/
  //****************
  // GetFilmAspectRatio()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetFilmAspectRatio
        // @parm [out] Film Aspect Ratio
        (aafRational_t*  aspectRatio);

/****/
  //****************
  // SetFilmFormat()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetFilmFormat
        // @parm [in] Film Format
        (aafFilmType_t  filmFormat);
	//@comm kFt35MM, kFt16MM, kFt8MM, kFt65MM

/****/
  //****************
  // SetFrameRate()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetFrameRate
        // @parm [in] Frame Rate
        (aafUInt32  rate);

/****/
  //****************
  // SetPerfPerFrame()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPerfPerFrame
        // @parm [in] Perforations per frame
        (aafUInt8  perfPerFrame);

/****/
  //****************
  // SetFilmAspectRatio()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetFilmAspectRatio
        // @parm [in] Film Aspect Ratio
        (aafRational_t  aspectRatio);



public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFFilmDescriptor)

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFFilmDescriptorTest.cpp.
  static AAFRESULT test();

private:
	OMWideStringProperty				_manufacturer;
	OMWideStringProperty				_model;
	OMFixedSizeProperty<aafFilmType_t>	_format;
	OMFixedSizeProperty<aafUInt32>		_frameRate;
	OMFixedSizeProperty<aafUInt8>		_perfPerFrame;
	OMFixedSizeProperty<aafRational_t>	_aspectRatio;
};

#endif // ! __ImplAAFFilmDescriptor_h__


   

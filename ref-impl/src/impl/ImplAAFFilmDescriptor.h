//@doc
//@class    AAFFilmDescriptor | Implementation class for AAFFilmDescriptor
#ifndef __ImplAAFFilmDescriptor_h__
#define __ImplAAFFilmDescriptor_h__


/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 * prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/


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
	//@comm Film format may be: kFt35MM, kFt16MM, kFt8MM, kFt65MM


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
	//@comm kFt35MM, kFt16MM, kFt8MM, kFt65MM


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

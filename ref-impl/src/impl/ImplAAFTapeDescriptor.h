//@doc
//@class    AAFTapeDescriptor | Implementation class for AAFTapeDescriptor
#ifndef __ImplAAFTapeDescriptor_h__
#define __ImplAAFTapeDescriptor_h__

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


//@doc
//@class    AAFTapeDescriptor | Implementation class for AAFTapeDescriptor
#ifndef __ImplAAFTapeDescriptor_h__
#define __ImplAAFTapeDescriptor_h__

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


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
        (aafWChar*  pName);
  //****************
  // GetTapeManufacturer()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTapeManufacturer
	    (// @parm [in,string] pass in a buffer which is cleared and filled.
         aafWChar*  pName,

		 // @parm [in] Length of the buffer to hold the Manufacturers name
		 aafInt32  buflen);
  //****************
  // GetTapeManBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTapeManBufLen
		// @parm [out] Manufacturers Name length
        (aafInt32 *  pLen);
  //****************
  // SetTapeModel()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetTapeModel
		// @parm [in,string] Tape Manufacturers Brand name
        (aafWChar*  pModelName);
  //****************
  // GetTapeModel()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTapeModel
	    (// @parm [in,string] pass in a buffer which is cleared and filled.
         aafWChar*  pModelName,

		 // @parm [in] Length of the buffer to hold the Manufacturers Brand name
		 aafInt32  buflen);
  //****************
  // GetTapeModelBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTapeModelBufLen
		// @parm [out] Model length
        (aafInt32 *  pLen);

  //****************
  // SetTapeFormFactor()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetTapeFormFactor
		// @parm [in] ex: kVHSVideoTape,kDATCartridge 
        (aafTapeCaseType_t  formFactor);
  //****************
  // GetTapeFormFactor()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTapeFormFactor
		// @parm [out] ex: kVHSVideoTape,kDATCartridge
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
        (aafLength_t  pTapeLength);
  //****************
  // GetTapeLength()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTapeLength
		// @parm [out] The length of the tape in minutes.
        (aafLength_t*  pTapeLength);



public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFTapeDescriptor)

  virtual AAFRESULT	STDMETHODCALLTYPE
		GetOwningMobKind (aafMobKind_t *pMobKind);

	// Persistent properties
private:
	OMFixedSizeProperty<aafTapeCaseType_t>		_formFactor;
	OMFixedSizeProperty<aafVideoSignalType_t>	_videoSignalType;
	OMFixedSizeProperty<aafTapeFormatType_t>	_tapeFormat;
	OMFixedSizeProperty<aafLength_t>			_tapeLength;
	OMWideStringProperty						_manufacturer;
	OMWideStringProperty						_model;
};

#endif // ! __ImplAAFTapeDescriptor_h__


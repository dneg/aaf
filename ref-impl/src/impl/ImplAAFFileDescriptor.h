//@doc
//@class    AAFFileDescriptor | Implementation class for AAFFileDescriptor
#ifndef __ImplAAFFileDescriptor_h__
#define __ImplAAFFileDescriptor_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/


#ifndef __ImplAAFEssenceDescriptor_h__
#include "ImplAAFEssenceDescriptor.h"
#endif



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
  // SetIsInContainer()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetIsInContainer
		// @parm [in] is this AAF or raw essence
        (aafBool  isAAF);


  //****************
  // GetIsInContainer()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetIsInContainer
		// @parm [out] is this AAF or raw medi
        (aafBool*  pIsAAF);


  //****************
  // SetSampleRate()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetSampleRate
		// @parm [in] sample rate of the essence
        (aafRational_t *  pRate);


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
        (aafUID_t *  pFormat);


  //****************
  // GetContainerFormat()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetContainerFormat
		// @parm [out] Identifies the file format
        (aafUID_t *  pFormat);



public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFFileDescriptor)

public:
	// Functions internal to the toolkit
	virtual AAFRESULT STDMETHODCALLTYPE
		GetOwningMobKind (aafMobKind_t *pMobKind);

private:
	OMFixedSizeProperty<aafRational_t>	_sampleRate;
	OMFixedSizeProperty<aafLength_t>	_length;
	OMFixedSizeProperty<aafBool>        _isInContainer;
	OMFixedSizeProperty<aafUID_t>       _containerFmt;
};

#endif // ! __ImplAAFFileDescriptor_h__

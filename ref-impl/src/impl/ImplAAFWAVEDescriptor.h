#ifndef __ImplAAFWAVEDescriptor_h__
#define __ImplAAFWAVEDescriptor_h__

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/


#include "ImplAAFFileDescriptor.h"


class ImplAAFWAVEDescriptor : public ImplAAFFileDescriptor
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFWAVEDescriptor ();

protected:
  virtual ~ImplAAFWAVEDescriptor ();

public:
  virtual AAFRESULT STDMETHODCALLTYPE
	Initialize ();


  //****************
  // GetSummary()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSummary
	    (// [in] Size of preallocated buffer
         aafUInt32  size,

		 // [out, size_is(size)] Preallocated buffer to hold the WAVE file information
		 aafDataValue_t  pSummary);

  //****************
  // GetSummaryBufferSize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSummaryBufferSize
		// [out] required buffer size
        (aafUInt32 *  pSize);

  //****************
  // SetSummary()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetSummary
	    (// [in] Size of preallocated buffer
         aafUInt32  size,

		 // [in, size_is(size)] buffer containing value
		 aafDataValue_t  pSummary);


private:
	OMVariableSizeProperty<aafUInt8> _summary;

};

#endif // ! __ImplAAFWAVEDescriptor_h__


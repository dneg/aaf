//@doc
//@class    AAFAIFCDescriptor | Implementation class for AAFAIFCDescriptor
#ifndef __ImplAAFAIFCDescriptor_h__
#define __ImplAAFAIFCDescriptor_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
*                                          *
\******************************************/


#ifndef __ImplAAFFileDescriptor_h__
#include "ImplAAFFileDescriptor.h"
#endif


class ImplAAFAIFCDescriptor : public ImplAAFFileDescriptor
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFAIFCDescriptor ();

protected:
  virtual ~ImplAAFAIFCDescriptor ();

public:

  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize ();



  //****************
  // GetSummary()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSummary
        (// @parm [in] Size of preallocated buffer
         aafUInt32  size,

         // @parm [out, size_is(size)] Preallocated buffer to hold the AIFC file information
         aafDataValue_t  pSummary);

  //****************
  // GetSummaryBufferSize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSummaryBufferSize
        // @parm [out] required buffer size
        (aafUInt32 *  pSize);

  //****************
  // SetSummary()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetSummary
        (// @parm [in] Size of preallocated buffer
         aafUInt32  size,

         // @parm [in, size_is(size)] buffer containing value
         aafDataValue_t  pSummary);


private:
  // Persistent Properties
	OMVariableSizeProperty<aafUInt8> _summary;

};

#endif // ! __ImplAAFAIFCDescriptor_h__



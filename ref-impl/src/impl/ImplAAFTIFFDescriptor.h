//@doc
//@class    AAFTIFFDescriptor | Implementation class for AAFTIFFDescriptor
#ifndef __ImplAAFTIFFDescriptor_h__
#define __ImplAAFTIFFDescriptor_h__


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


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

  




#ifndef __ImplAAFDigitalImageDescriptor_h__
#include "ImplAAFDigitalImageDescriptor.h"
#endif


class ImplAAFTIFFDescriptor : public ImplAAFDigitalImageDescriptor
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFTIFFDescriptor ();

protected:
  virtual ~ImplAAFTIFFDescriptor ();

public:

  //****************
  // SetIsUniform()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetIsUniform
        // @parm [in] Sets the IsUniform flag value
        (aafBool  IsUniform);


  //****************
  // GetIsUniform()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetIsUniform
        // @parm [out] Gets the IsUniform flag value
        (aafBool *  pIsUniform);

  //****************
  // SetIsContiguous()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetIsContiguous
        // @parm [in] is data stores in contiguous bytes 
        (aafBool  IsContiguous);


  //****************
  // GetIsContiguous()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetIsContiguous
        // @parm [out] is this data stored in contiguous bytes
        (aafBool *  pIsContiguous);

  //****************
  // SetLeadingLines()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetLeadingLines
        // @parm [in] Number of leading lines to be thrown away Optional
        (aafInt32  LeadingLines);

  //****************
  // GetLeadingLines()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetLeadingLines
        // @parm [out] Address to store the number of leading lines
        (aafInt32 *  pLeadingLines);

  //****************
  // SetTrailingLines()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetTrailingLines
        // @parm [in] Number of trailing lines to be thrown away Optional.
        (aafInt32  TrailingLines);

  //****************
  // GetTrailingLines()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetTrailingLines
        // @parm [out] Address to store the number of trailing lines.
        (aafInt32 *  pTrailingLines);

  //****************
  // SetJPEGTableID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetJPEGTableID
        // @parm [in] Registered JPEG table code or JT_NULL.
        (aafJPEGTableID_t  JPEGTableID);

  //****************
  // GetJPEGTableID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetJPEGTableID
        // @parm [out] Address to store the nJPEG table code
        (aafJPEGTableID_t *  pJPEGTableID);

  //****************
  // GetSummary()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSummary
        (// @parm [in] Size of preallocated buffer
         aafUInt32  size,

         // @parm [out, size_is(size)] Preallocated buffer to hold the TIFF IFD file information
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



public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFTIFFDescriptor)

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFTIFFDescriptorTest.cpp.
  static AAFRESULT test();

private:
	OMFixedSizeProperty<aafBool>			_isUniform;
	OMFixedSizeProperty<aafBool>			_isContiguous;
	OMFixedSizeProperty<aafInt32>			_leadingLines;
	OMFixedSizeProperty<aafInt32>			_trailingLines;
	OMFixedSizeProperty<aafJPEGTableID_t>	_jpegTableID;
	OMVariableSizeProperty<aafUInt8>		_summary;

};

#endif // ! __ImplAAFTIFFDescriptor_h__



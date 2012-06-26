//@doc
//@class    AAFRGBADescriptor | Implementation class for AAFRGBADescriptor
#ifndef __CAAFRGBADescriptor_h__
#define __CAAFRGBADescriptor_h__

//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id$ $Name$
//
// The contents of this file are subject to the AAF SDK Public Source
// License Agreement Version 2.0 (the "License"); You may not use this
// file except in compliance with the License.  The License is available
// in AAFSDKPSL.TXT, or you may obtain a copy of the License from the
// Advanced Media Workflow Association, Inc., or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.  Refer to Section 3.3 of the License for proper use
// of this Exhibit.
//
// WARNING:  Please contact the Advanced Media Workflow Association,
// Inc., for more information about any additional licenses to
// intellectual property covering the AAF Standard that may be required
// to create and distribute AAF compliant products.
// (http://www.amwa.tv/policies).
//
// Copyright Notices:
// The Original Code of this file is Copyright 1998-2012, licensor of the
// Advanced Media Workflow Association.  All rights reserved.
//
// The Initial Developer of the Original Code of this file and the
// licensor of the Advanced Media Workflow Association is
// Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=



#ifndef __AAF_h__
#include "AAF.h"
#endif










#ifndef __CAAFDigitalImageDescriptor_h__
#include "CAAFDigitalImageDescriptor.h"
#endif


class CAAFRGBADescriptor
  : public IAAFRGBADescriptor,
    public IAAFRGBADescriptor2,
    public CAAFDigitalImageDescriptor
{
protected:

  //********
  //
  // Constructor/destructor
  //
  CAAFRGBADescriptor (IUnknown * pControllingUnknown, aafBool doInit = kAAFTrue);
  virtual ~CAAFRGBADescriptor ();

public:


  //***********************************************************
  //
  // SetPixelLayout()
  //
  // Sets the layout and structure of the components in a single pixel.
  /// The layout array is an Array of 8 aafRGBAComponent_t.  The 'Code' field of the struct is
  /// from the set:
  ///   	- kAAFCompNone
  /// 	- kAAFCompAlpha
  /// 	- kAAFCompBlue
  /// 	- kAAFCompFill
  /// 	- kAAFCompGreen
  /// 	- kAAFCompPalette
  /// 	- kAAFCompRed
  ///
  /// The 'Size' field of the struct is the component size in bits.
  /// 
  /// If you set less than 8 entries, the remaining entries will be padded with kAAFCompNone and a size of zero.
  /// If this method fails the PixelLayout property will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetPixelLayout) (
    // The number of elements in each array
    /*[in]*/ aafUInt32  numberElements,

    // Array of up to 8 aafRGBAComponent_t indicating component order and size.
    /*[in]*/ aafRGBAComponent_t*  PixelLayoutArray);

  //***********************************************************
  //
  // CountPixelLayoutElements()
  //
  // This function returns the number of components in a pixel.
  /// 
  /// Succeeds if all of the following are true:
  /// - this object has already been initialized.
  /// - the pResult pointer is valid.
  /// 
  /// If this method fails no state is changed.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NOT_INITIALIZED
  ///   - Initialize() has already been called on this object.
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pResult is null.
  //
  STDMETHOD (CountPixelLayoutElements) (
    // Number of components 
    /*[out]*/ aafUInt32 *  pResult);

  //***********************************************************
  //
  // GetPixelLayout()
  //
  // Gets the layout and structure of the components in a single pixel.
  /// The layout array is an Array of 8 aafRGBAComponent_t.  The 'Code' field of the struct is
  /// from the set:
  ///   	- kAAFCompNone
  /// 	- kAAFCompAlpha
  /// 	- kAAFCompBlue
  /// 	- kAAFCompFill
  /// 	- kAAFCompGreen
  /// 	- kAAFCompPalette
  /// 	- kAAFCompRed
  ///
  /// The 'Size' field of the struct is the component size in bits.
  /// 
  /// If you Get more entries than were set, the extra entries will be padded with kAAFCompNone and a size of zero.
  ///
  /// If this method fails, PixelLayoutArray will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pNumberElements or PixelLayoutArray or PixelStructureArray is NULL.
  ///
  /// AAFRESULT_SMALLBUF
  ///   - numberElements indicates that the array is too small to hold
  ///     the data.
  //
  STDMETHOD (GetPixelLayout) (
    // The number of elements in the array
    /*[in]*/ aafUInt32  numberElements,

    // Array to hold up to 8 aafRGBAComponent_t indicating component order and size.
    /*[out, size_is(numberElements)]*/ aafRGBAComponent_t*  PixelLayoutArray);

  //***********************************************************
  //
  // SetPalette()
  //
  // Sets the palette property.  The palette must be in the form specified
  /// by the palette layout and palette structure parameters.
  /// 
  /// Succeeds if all of the following are true:
  /// - pPalette is a valid pointer
  /// 
  /// If this method fails, the palette property will not be
  /// changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pPalette is NULL.
  //
  STDMETHOD (SetPalette) (
    // The number of bytes in the array
    /*[in]*/ aafUInt32  numberElements,

    // Array to hold the palette information
    /*[in, size_is(numberElements)]*/ aafUInt8*  pPalette);


  //***********************************************************
  //
  // GetPalette()
  //
  // Sets the palette property.  The palette must be in the form specified
  /// by the palette layout and palette structure parameters.
  /// The values are written to the array specified by pPalette,
  /// which is of size numberElements.  The required size may be found
  /// by calling GetPaletteSize().
  /// 
  /// Succeeds if all of the following are true:
  /// - pPalette is a valid pointer.
  /// - numberElements indicates the array is large enough to hold the
  ///   data.
  /// 
  /// If this method fails, pPalette will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pNumberElements is NULL.
  ///
  /// AAFRESULT_SMALLBUF
  ///   - numberElements indicates that the array is too small to hold
  ///     the data.
  //
  STDMETHOD (GetPalette) (
    // The number of bytes in the array
    /*[in]*/ aafUInt32  numberRecords,

    // Array to hold the palette information
    /*[out, size_is(numberRecords)]*/ aafUInt8*  pPalette);


  //***********************************************************
  //
  // GetPaletteSize()
  //
  // Get the number of bytes in the VideoLineMap property array.
  /// 
  /// Succeeds if all of the following are true:
  /// - pNumberBytes is a valid pointer
  /// 
  /// If this method fails, *pNumberBytes will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pNumberBytes is NULL.
  //
  STDMETHOD (GetPaletteSize) (
    // The number of bytes in the array 
    /*[out]*/ aafUInt32 *  pNumberBytes);

  //***********************************************************
  //
  // CountPaletteLayoutElements()
  //
  // This function returns the number of components in a pixel.
  /// 
  /// Succeeds if all of the following are true:
  /// - this object has already been initialized.
  /// - the pResult pointer is valid.
  /// 
  /// If this method fails no state is changed.
  /// 
  /// This method will return the following codes.  If more than one of
  /// the listed errors is in effect, it will return the first one
  /// encountered in the order given below:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NOT_INITIALIZED
  ///   - Initialize() has already been called on this object.
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pResult is null.
  //
  STDMETHOD (CountPaletteLayoutElements) (
    // Number of components 
    /*[out]*/ aafUInt32 *  pResult);

  //***********************************************************
  //
  // SetPaletteLayout()
  //
  // Sets the layout and structure of the components in a single pixel.
  /// The layout array is an Array of 8 aafRGBAComponent_t.  The 'Code' field of the struct is
  /// from the set:
  ///   	- kAAFCompNone
  /// 	- kAAFCompAlpha
  /// 	- kAAFCompBlue
  /// 	- kAAFCompFill
  /// 	- kAAFCompGreen
  /// 	- kAAFCompPalette
  /// 	- kAAFCompRed
  ///
  /// The 'Size' field of the struct is the component size in bits.
  /// 
  /// If you set less than 8 entries, the remaining entries will be padded with kAAFCompNone and a size of zero.
  /// If this method fails the PaletteLayout property will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  //
  STDMETHOD (SetPaletteLayout) (
    // The number of elements in the array
    /*[in]*/ aafUInt32  numberElements,

    // Array of up to 8 bytes indicating component size, matching layout, and terminated by a zero byte.
    /*[in]*/ aafRGBAComponent_t*  PaletteLayoutArray);

  //***********************************************************
  //
  // GetPaletteLayout()
  //
  // The layout array is an Array of 8 aafRGBAComponent_t.  The 'Code' field of the struct is
  /// from the set:
  ///   	- kAAFCompNone
  /// 	- kAAFCompAlpha
  /// 	- kAAFCompBlue
  /// 	- kAAFCompFill
  /// 	- kAAFCompGreen
  /// 	- kAAFCompPalette
  /// 	- kAAFCompRed
  ///
  /// The 'Size' field of the struct is the component size in bits.
  /// 
  /// If you Get more entries than were set, the extra entries will be padded with kAAFCompNone and a size of zero.
  ///
  /// If this method fails, PaletteLayoutArray will not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pNumberElements or PaletteLayoutArray is NULL.
  ///
  /// AAFRESULT_SMALLBUF
  ///   - numberElements indicates that the array is too small to hold
  ///     the data.
  //
  STDMETHOD (GetPaletteLayout) (
    // The number of elements in each array
    /*[in]*/ aafUInt32  numberElements,

    // Array to hold 8 aafRGBAComponent_t indicating component order and size.
    /*[out, size_is(numberElements)]*/ aafRGBAComponent_t*  PaletteLayoutArray);

  //***********************************************************
  // METHOD NAME: SetComponentMaxRef()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFRGBADescriptor2 | SetComponentMaxRef |
  // Sets the ComponentMaxRef property.
  ///
  /// Succeeds if all of the following are true:
  /// 
  /// If this method fails, the ComponentMaxRef property will not be
  /// changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  // @end
  // 
  STDMETHOD (SetComponentMaxRef)
   (
    // @parm [in] aafUInt32 | componentMaxRef | Optional.
    aafUInt32  componentMaxRef
  );

  //***********************************************************
  // METHOD NAME: GetComponentMaxRef()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFRGBADescriptor2 | GetComponentMaxRef |
  // Gets the ComponentMaxRef property.
  ///
  /// Succeeds if all of the following are true:
  /// - pComponentMaxRef is a valid pointer
  /// - the property is present.
  ///
  /// If this method fails, pComponentMaxRef will not be changed.
  ///
  /// This method will return the following codes:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pComponentMaxRef is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  // @end
  // 
  STDMETHOD (GetComponentMaxRef)
   (
    // @parm [out] aafUInt32 * | pComponentMaxRef | Optional.
    aafUInt32 *  pComponentMaxRef
  );

  //***********************************************************
  // METHOD NAME: SetComponentMinRef()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFRGBADescriptor2 | SetComponentMinRef |
  // Sets the ComponentMinRef property.
  ///
  /// Succeeds if all of the following are true:
  /// 
  /// If this method fails, the ComponentMinRef property will not be
  /// changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  // @end
  // 
  STDMETHOD (SetComponentMinRef)
   (
    // @parm [in] aafUInt32 | componentMinRef | Optional.
    aafUInt32  componentMinRef
  );

  //***********************************************************
  // METHOD NAME: GetComponentMinRef()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFRGBADescriptor2 | GetComponentMinRef |
  // Gets the ComponentMinRef property.
  ///
  /// Succeeds if all of the following are true:
  /// - pComponentMinRef is a valid pointer
  /// - the property is present.
  ///
  /// If this method fails, pComponentMinRef will not be changed.
  ///
  /// This method will return the following codes:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pComponentMinRef is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  // @end
  // 
  STDMETHOD (GetComponentMinRef)
   (
    // @parm [out] aafUInt32 * | pComponentMinRef | Optional.
    aafUInt32 *  pComponentMinRef
  );

  //***********************************************************
  // METHOD NAME: SetAlphaMaxRef()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFRGBADescriptor2 | SetAlphaMaxRef |
  // Sets the AlphaMaxRef property.
  ///
  /// Succeeds if all of the following are true:
  /// 
  /// If this method fails, the AlphaMaxRef property will not be
  /// changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  // @end
  // 
  STDMETHOD (SetAlphaMaxRef)
   (
    // @parm [in] aafUInt32 | alphaMaxRef | Optional.
    aafUInt32  alphaMaxRef
  );

  //***********************************************************
  // METHOD NAME: GetAlphaMaxRef()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFRGBADescriptor2 | GetAlphaMaxRef |
  // Gets the AlphaMaxRef property.
  ///
  /// Succeeds if all of the following are true:
  /// - pAlphaMaxRef is a valid pointer
  /// - the property is present.
  ///
  /// If this method fails, pAlphaMaxRef will not be changed.
  ///
  /// This method will return the following codes:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pAlphaMaxRef is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  // @end
  // 
  STDMETHOD (GetAlphaMaxRef)
   (
    // @parm [out] aafUInt32 * | pAlphaMaxRef | Optional.
    aafUInt32 *  pAlphaMaxRef
  );

  //***********************************************************
  // METHOD NAME: SetAlphaMinRef()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFRGBADescriptor2 | SetAlphaMinRef |
  // Sets the AlphaMinRef property.
  ///
  /// Succeeds if all of the following are true:
  /// 
  /// If this method fails, the AlphaMinRef property will not be
  /// changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  // @end
  // 
  STDMETHOD (SetAlphaMinRef)
   (
    // @parm [in] aafUInt32 | alphaMinRef | Optional.
    aafUInt32  alphaMinRef
  );

  //***********************************************************
  // METHOD NAME: GetAlphaMinRef()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFRGBADescriptor2 | GetAlphaMinRef |
  // Gets the AlphaMinRef property.
  ///
  /// Succeeds if all of the following are true:
  /// - pAlphaMinRef is a valid pointer
  /// - the property is present.
  ///
  /// If this method fails, pAlphaMinRef will not be changed.
  ///
  /// This method will return the following codes:
  ///
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pAlphaMinRef is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  // @end
  // 
  STDMETHOD (GetAlphaMinRef)
   (
    // @parm [out] aafUInt32 * | pAlphaMinRef | Optional.
    aafUInt32 *  pAlphaMinRef
  );

  //***********************************************************
  // METHOD NAME: SetScanningDirection()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFRGBADescriptor2 | SetScanningDirection |
  // Sets the ScanningDirection property.  This property is optional.
  ///
  /// Succeeds if all of the following are true:
  /// - scanningDirection is a value value.
  /// 
  /// If this method fails, the ScanningDirection property will not be
  /// changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_AAFRESULT_INVALID_ENUM_VALUE
  ///   - scanningDirection is not a valid value.
  // @end
  // 
  STDMETHOD (SetScanningDirection)
   (
    // @parm [in] aafScanningDirection_t | scanningDirection | Scanning direction value.
    aafScanningDirection_t  scanningDirection
  );

  //***********************************************************
  // METHOD NAME: GetScanningDirection()
  //
  // DESCRIPTION:
  // @mfunc AAFRESULT | AAFRGBADescriptor2 | GetScanningDirection |
  // Gets the ScanningDirection property.  This property is optional.
  ///
  /// Succeeds if all of the following are true:
  /// - pScanningDirection is a valid pointer
  /// - the property is present.
  /// 
  /// If this method fails, pScanningDirection not be changed.
  /// 
  /// This method will return the following codes:
  /// 
  /// AAFRESULT_SUCCESS
  ///   - succeeded.  (This is the only code indicating success.)
  ///
  /// AAFRESULT_NULL_PARAM
  ///   - pScanningDirection is NULL.
  ///
  /// AAFRESULT_PROP_NOT_PRESENT
  ///   - the property is not present.
  // @end
  // 
  STDMETHOD (GetScanningDirection)
   (
    // @parm [out] aafScanningDirection_t * | pScanningDirection | Scanning direction value.
    aafScanningDirection_t *  pScanningDirection
  );



protected:
  // 
  // Declare the QI that implements for the interfaces
  // for this module. This will be called by CAAFUnknown::QueryInterface().
  // 
  STDMETHOD(InternalQueryInterface)(REFIID riid, void **ppvObjOut);


public:
  //
  // This class as concrete. All AAF objects can be constructed from
  // a CLSID. This will allow subclassing all "base-classes" by
  // aggreggation.
  // 
  AAF_DECLARE_CONCRETE();
  //
  //********
};

#endif // ! __CAAFRGBADescriptor_h__


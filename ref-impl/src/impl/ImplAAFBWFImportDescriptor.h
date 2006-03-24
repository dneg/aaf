//@doc
//@class    AAFBWFImportDescriptor | Implementation class for AAFBWFImportDescriptor
#ifndef __ImplAAFBWFImportDescriptor_h__
#define __ImplAAFBWFImportDescriptor_h__

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
// The Original Code of this file is Copyright 1998-2005, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=


#include "ImplEnumAAFRIFFChunks.h"
#include "ImplAAFRIFFChunk.h"

#include "OMStrongRefVectorProperty.h"
#include "OMWideStringProperty.h"
#include "OMFixedSizeProperty.h"

#ifndef __ImplAAFImportDescriptor_h__
#include "ImplAAFImportDescriptor.h"
#endif


class ImplAAFBWFImportDescriptor : public ImplAAFImportDescriptor
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFBWFImportDescriptor ();

protected:
  virtual ~ImplAAFBWFImportDescriptor ();

private:

	OMFixedSizeProperty<aafUInt32>                      _fileSecurityReport;
	OMFixedSizeProperty<aafUInt32>                      _fileSecurityWave;
	OMWideStringProperty								_bextCodingHistory;
	OMWideStringProperty								_qltyBasicData;
	OMWideStringProperty								_qltyStartOfModulation;
	OMWideStringProperty								_qltyQualityEvent;
	OMWideStringProperty								_qltyEndOfModulation;
	OMWideStringProperty								_qltyQualityParameter;
	OMWideStringProperty								_qltyOperatorComment;
	OMWideStringProperty								_qltyCueSheet;
	OMStrongReferenceVectorProperty<ImplAAFRIFFChunk>   _unknownBWFChunks;

public:

  //****************
  // Initialize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Initialize ();



  //****************
  // SetFileSecurityReport()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetFileSecurityReport
        // @parm [in] Sets the FileSecurityReport property.
  /// 
  /// Succeeds if all of the following are true:
  /// -
  /// If this method fails
        (aafUInt32  fileSecurityReport);


  //****************
  // GetFileSecurityReport()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetFileSecurityReport
        // @parm [out] Optional.
        (aafUInt32 *  pFileSecurityReport);


  //****************
  // SetFileSecurityWave()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetFileSecurityWave
        // @parm [in] Sets the FileSecurityWave property.
  /// 
  /// Succeeds if all of the following are true:
  /// -
  /// If this method fails
        (aafUInt32  fileSecurityWave);


  //****************
  // GetQltyFileSecurityWave()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetFileSecurityWave
        // @parm [out] Optional.
        (aafUInt32 *  pFileSecurityWave);


  //****************
  // AppendUnknownBWFChunks()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendUnknownBWFChunks
        // @parm [in] RIFFChunk object
        (ImplAAFRIFFChunk * pData);


  //****************
  // CountUnknownBWFChunks()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    CountUnknownBWFChunks
        // @parm [out] Number  of RIFFChunk objects
        (aafUInt32 *  pNumData);


  //****************
  // GetUnknownBWFChunks()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetUnknownBWFChunks
        // @parm [out] RIFFChunk objects
        (ImplEnumAAFRIFFChunks ** ppEnum);


  //****************
  // RemoveUnknownBWFChunks()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RemoveUnknownBWFChunks
        // @parm [in] RIFFChunk object to remove
        (ImplAAFRIFFChunk * pData);

  //****************
  // SetBextCodingHistory()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetBextCodingHistory
        // @parm [in, string] buffer from which BextCodingHistory is to be read
        (aafCharacter_constptr  pBextCodingHistory);


  //****************
  // GetBextCodingHistory()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetBextCodingHistory
        (// @parm [out, string, size_is(bufSize)] buffer into which BextCodingHistory is to be written
         aafCharacter *  pBextCodingHistory,

         // @parm [in] size of *pBextCodingHistory buffer in bytes
         aafUInt32  bufSize);


  //****************
  // GetBextCodingHistoryBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetBextCodingHistoryBufLen
        // @parm [out] size of required buffer, in bytes
        (aafUInt32 *  pBufSize);
     
  //****************
  // SetQltyBasicData()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetQltyBasicData
        // @parm [in, string] buffer from which QltyBasicData is to be read
        (aafCharacter_constptr  pQltyBasicData);


  //****************
  // GetQltyBasicData()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetQltyBasicData
        (// @parm [out, string, size_is(bufSize)] buffer into which QltyBasicData is to be written
         aafCharacter *  pQltyBasicData,

         // @parm [in] size of *pQltyBasicData buffer in bytes
         aafUInt32  bufSize);


  //****************
  // GetQltyBasicDataBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetQltyBasicDataBufLen
        // @parm [out] size of required buffer, in bytes
        (aafUInt32 *  pBufSize);

  //****************
  // SetQltyStartOfModulation()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetQltyStartOfModulation
        // @parm [in, string] buffer from which QltyStartOfModulation is to be read
        (aafCharacter_constptr  pQltyStartOfModulation);


  //****************
  // GetQltyStartOfModulation()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetQltyStartOfModulation
        (// @parm [out, string, size_is(bufSize)] buffer into which QltyStartOfModulation is to be written
         aafCharacter *  pQltyStartOfModulation,

         // @parm [in] size of *pQltyStartOfModulation buffer in bytes
         aafUInt32  bufSize);


  //****************
  // GetQltyStartOfModulationBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetQltyStartOfModulationBufLen
        // @parm [out] size of required buffer, in bytes
        (aafUInt32 *  pBufSize);
     
  //****************
  // SetQltyQualityEvent()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetQltyQualityEvent
        // @parm [in, string] buffer from which QltyQualityEvent is to be read
        (aafCharacter_constptr  pQltyQualityEvent);


  //****************
  // GetQltyQualityEvent()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetQltyQualityEvent
        (// @parm [out, string, size_is(bufSize)] buffer into which QltyQualityEvent is to be written
         aafCharacter *  pQltyQualityEvent,

         // @parm [in] size of *pQltyQualityEvent buffer in bytes
         aafUInt32  bufSize);


  //****************
  // GetQltyQualityEventBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetQltyQualityEventBufLen
        // @parm [out] size of required buffer, in bytes
        (aafUInt32 *  pBufSize);

  //****************
  // SetQltyEndOfModulation()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetQltyEndOfModulation
        // @parm [in, string] buffer from which QltyEndOfModulation is to be read
        (aafCharacter_constptr  pQltyEndOfModulation);


  //****************
  // GetQltyEndOfModulation()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetQltyEndOfModulation
        (// @parm [out, string, size_is(bufSize)] buffer into which QltyEndOfModulation is to be written
         aafCharacter *  pQltyEndOfModulation,

         // @parm [in] size of *pQltyEndOfModulation buffer in bytes
         aafUInt32  bufSize);


  //****************
  // GetQltyEndOfModulationBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetQltyEndOfModulationBufLen
        // @parm [out] size of required buffer, in bytes
        (aafUInt32 *  pBufSize);
     
  //****************
  // SetQltyQualityParameter()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetQltyQualityParameter
        // @parm [in, string] buffer from which QltyQualityParameter is to be read
        (aafCharacter_constptr  pQltyQualityParameter);


  //****************
  // GetQltyQualityParameter()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetQltyQualityParameter
        (// @parm [out, string, size_is(bufSize)] buffer into which QltyQualityParameter is to be written
         aafCharacter *  pQltyQualityParameter,

         // @parm [in] size of *pQltyQualityParameter buffer in bytes
         aafUInt32  bufSize);


  //****************
  // GetQltyQualityParameterBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetQltyQualityParameterBufLen
        // @parm [out] size of required buffer, in bytes
        (aafUInt32 *  pBufSize);
     
  //****************
  // SetQltyOperatorComment()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetQltyOperatorComment
        // @parm [in, string] buffer from which QltyOperatorComment is to be read
        (aafCharacter_constptr  pQltyOperatorComment);


  //****************
  // GetQltyOperatorComment()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetQltyOperatorComment
        (// @parm [out, string, size_is(bufSize)] buffer into which QltyOperatorComment is to be written
         aafCharacter *  pQltyOperatorComment,

         // @parm [in] size of *pQltyOperatorComment buffer in bytes
         aafUInt32  bufSize);


  //****************
  // GetQltyOperatorCommentBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetQltyOperatorCommentBufLen
        // @parm [out] size of required buffer, in bytes
        (aafUInt32 *  pBufSize);
     
  //****************
  // SetQltyCueSheet()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetQltyCueSheet
        // @parm [in, string] buffer from which QltyCueSheet is to be read
        (aafCharacter_constptr  pQltyCueSheet);


  //****************
  // GetQltyCueSheet()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetQltyCueSheet
        (// @parm [out, string, size_is(bufSize)] buffer into which QltyCueSheet is to be written
         aafCharacter *  pQltyCueSheet,

         // @parm [in] size of *pQltyCueSheet buffer in bytes
         aafUInt32  bufSize);


  //****************
  // GetQltyCueSheetBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetQltyCueSheetBufLen
        // @parm [out] size of required buffer, in bytes
        (aafUInt32 *  pBufSize);

};

#endif // ! __ImplAAFBWFImportDescriptor_h__



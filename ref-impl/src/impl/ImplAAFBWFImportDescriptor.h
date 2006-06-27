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
  // GetFileSecurityWave()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetFileSecurityWave
        // @parm [out] Optional.
        (aafUInt32 *  pFileSecurityWave);


  //****************
  // AppendUnknownBWFChunk()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    AppendUnknownBWFChunk
        // @parm [in] RIFFChunk object
        (ImplAAFRIFFChunk * pData);


  //****************
  // PrependUnknownBWFChunk()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    PrependUnknownBWFChunk
        // @parm [in] RIFFChunk object
        (ImplAAFRIFFChunk * pData);


  //****************
  // InsertUnknownBWFChunkAt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    InsertUnknownBWFChunkAt
        (// @parm [in] index at which to insert
         aafUInt32 index,

         // @parm [in] RIFFChunk object
         ImplAAFRIFFChunk * pData);


  //****************
  // GetUnknownBWFChunkAt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetUnknownBWFChunkAt
        (// @parm [in] index of RIFFChunk object to return
         aafUInt32 index,

         // @parm [out] RIFFChunk object at the given index
         ImplAAFRIFFChunk ** ppData);


  //****************
  // RemoveUnknownBWFChunkAt()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    RemoveUnknownBWFChunkAt
        // @parm [in] index of RIFFChunk object to remove
        (aafUInt32 index);


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
  // SetCodingHistory()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetCodingHistory
        // @parm [in, string] buffer from which CodingHistory is to be read
        (aafCharacter_constptr  pCodingHistory);


  //****************
  // GetCodingHistory()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetCodingHistory
        (// @parm [out, string, size_is(bufSize)] buffer into which CodingHistory is to be written
         aafCharacter *  pCodingHistory,

         // @parm [in] size of *pCodingHistory buffer in bytes
         aafUInt32  bufSize);


  //****************
  // GetCodingHistoryBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetCodingHistoryBufLen
        // @parm [out] size of required buffer, in bytes
        (aafUInt32 *  pBufSize);
     
  //****************
  // SetBasicData()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetBasicData
        // @parm [in, string] buffer from which BasicData is to be read
        (aafCharacter_constptr  pBasicData);


  //****************
  // GetBasicData()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetBasicData
        (// @parm [out, string, size_is(bufSize)] buffer into which BasicData is to be written
         aafCharacter *  pBasicData,

         // @parm [in] size of *pBasicData buffer in bytes
         aafUInt32  bufSize);


  //****************
  // GetBasicDataBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetBasicDataBufLen
        // @parm [out] size of required buffer, in bytes
        (aafUInt32 *  pBufSize);

  //****************
  // SetStartOfModulation()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetStartOfModulation
        // @parm [in, string] buffer from which StartOfModulation is to be read
        (aafCharacter_constptr  pStartOfModulation);


  //****************
  // GetStartOfModulation()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetStartOfModulation
        (// @parm [out, string, size_is(bufSize)] buffer into which StartOfModulation is to be written
         aafCharacter *  pStartOfModulation,

         // @parm [in] size of *pStartOfModulation buffer in bytes
         aafUInt32  bufSize);


  //****************
  // GetStartOfModulationBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetStartOfModulationBufLen
        // @parm [out] size of required buffer, in bytes
        (aafUInt32 *  pBufSize);
     
  //****************
  // SetQualityEvent()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetQualityEvent
        // @parm [in, string] buffer from which QualityEvent is to be read
        (aafCharacter_constptr  pQualityEvent);


  //****************
  // GetQualityEvent()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetQualityEvent
        (// @parm [out, string, size_is(bufSize)] buffer into which QualityEvent is to be written
         aafCharacter *  pQualityEvent,

         // @parm [in] size of *pQualityEvent buffer in bytes
         aafUInt32  bufSize);


  //****************
  // GetQualityEventBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetQualityEventBufLen
        // @parm [out] size of required buffer, in bytes
        (aafUInt32 *  pBufSize);

  //****************
  // SetEndOfModulation()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetEndOfModulation
        // @parm [in, string] buffer from which EndOfModulation is to be read
        (aafCharacter_constptr  pEndOfModulation);


  //****************
  // GetEndOfModulation()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetEndOfModulation
        (// @parm [out, string, size_is(bufSize)] buffer into which EndOfModulation is to be written
         aafCharacter *  pEndOfModulation,

         // @parm [in] size of *pEndOfModulation buffer in bytes
         aafUInt32  bufSize);


  //****************
  // GetEndOfModulationBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetEndOfModulationBufLen
        // @parm [out] size of required buffer, in bytes
        (aafUInt32 *  pBufSize);
     
  //****************
  // SetQualityParameter()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetQualityParameter
        // @parm [in, string] buffer from which QualityParameter is to be read
        (aafCharacter_constptr  pQualityParameter);


  //****************
  // GetQualityParameter()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetQualityParameter
        (// @parm [out, string, size_is(bufSize)] buffer into which QualityParameter is to be written
         aafCharacter *  pQualityParameter,

         // @parm [in] size of *pQualityParameter buffer in bytes
         aafUInt32  bufSize);


  //****************
  // GetQualityParameterBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetQualityParameterBufLen
        // @parm [out] size of required buffer, in bytes
        (aafUInt32 *  pBufSize);
     
  //****************
  // SetOperatorComment()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetOperatorComment
        // @parm [in, string] buffer from which OperatorComment is to be read
        (aafCharacter_constptr  pOperatorComment);


  //****************
  // GetOperatorComment()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetOperatorComment
        (// @parm [out, string, size_is(bufSize)] buffer into which OperatorComment is to be written
         aafCharacter *  pOperatorComment,

         // @parm [in] size of *pOperatorComment buffer in bytes
         aafUInt32  bufSize);


  //****************
  // GetOperatorCommentBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetOperatorCommentBufLen
        // @parm [out] size of required buffer, in bytes
        (aafUInt32 *  pBufSize);
     
  //****************
  // SetCueSheet()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetCueSheet
        // @parm [in, string] buffer from which CueSheet is to be read
        (aafCharacter_constptr  pCueSheet);


  //****************
  // GetCueSheet()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetCueSheet
        (// @parm [out, string, size_is(bufSize)] buffer into which CueSheet is to be written
         aafCharacter *  pCueSheet,

         // @parm [in] size of *pCueSheet buffer in bytes
         aafUInt32  bufSize);


  //****************
  // GetCueSheetBufLen()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetCueSheetBufLen
        // @parm [out] size of required buffer, in bytes
        (aafUInt32 *  pBufSize);

};

#endif // ! __ImplAAFBWFImportDescriptor_h__



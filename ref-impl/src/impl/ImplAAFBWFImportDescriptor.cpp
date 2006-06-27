//=---------------------------------------------------------------------=
//
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
// The Original Code of this file is Copyright 1998-2006, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=



#include "AAFStoredObjectIDs.h"
#include "AAFPropertyIDs.h"

#ifndef __ImplAAFBWFImportDescriptor_h__
#include "ImplAAFBWFImportDescriptor.h"
#endif

extern "C" const aafClassID_t CLSID_EnumAAFRIFFChunks;

#include <string.h>
#include <wchar.h>
#include "aafErr.h"

ImplAAFBWFImportDescriptor::ImplAAFBWFImportDescriptor ():
  
  	_fileSecurityReport( PID_BWFImportDescriptor_QltyFileSecurityReport, L"QltyFileSecurityReport"),
  	_fileSecurityWave( PID_BWFImportDescriptor_QltyFileSecurityWave, L"QltyFileSecurityWave"),
  	_bextCodingHistory( PID_BWFImportDescriptor_BextCodingHistory, L"BextCodingHistory"),
  	_qltyBasicData( PID_BWFImportDescriptor_QltyBasicData, L"QltyBasicData"),
  	_qltyStartOfModulation( PID_BWFImportDescriptor_QltyStartOfModulation, L"QltyStartOfModulation"),
  	_qltyQualityEvent( PID_BWFImportDescriptor_QltyQualityEvent, L"QltyQualityEvent"),
  	_qltyEndOfModulation( PID_BWFImportDescriptor_QltyEndOfModulation, L"QltyEndOfModulation"),
  	_qltyQualityParameter( PID_BWFImportDescriptor_QltyQualityParameter, L"QltyQualityParameter"),
  	_qltyOperatorComment( PID_BWFImportDescriptor_QltyOperatorComment, L"QltyOperatorComment"),
  	_qltyCueSheet( PID_BWFImportDescriptor_QltyCueSheet, L"QltyCueSheet"),
  	_unknownBWFChunks( PID_BWFImportDescriptor_UnknownBWFChunks, L"UnknownBWFChunks")
  

{
	_persistentProperties.put(_fileSecurityReport.address());
	_persistentProperties.put(_fileSecurityWave.address());
  	_persistentProperties.put(_bextCodingHistory.address());
  	_persistentProperties.put(_qltyBasicData.address());
  	_persistentProperties.put(_qltyStartOfModulation.address());
  	_persistentProperties.put(_qltyQualityEvent.address());
  	_persistentProperties.put(_qltyEndOfModulation.address());
  	_persistentProperties.put(_qltyQualityParameter.address());
  	_persistentProperties.put(_qltyOperatorComment.address());
  	_persistentProperties.put(_qltyCueSheet.address());
  	_persistentProperties.put(_unknownBWFChunks.address());
}



ImplAAFBWFImportDescriptor::~ImplAAFBWFImportDescriptor ()
{}

AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::Initialize ()
{
  return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::SetFileSecurityReport (
      aafUInt32  fileSecurityReport)
{
  _fileSecurityReport = fileSecurityReport;
	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::GetFileSecurityReport (
      aafUInt32 *  pFileSecurityReport)
{
 	if( pFileSecurityReport == NULL )
    {
        return AAFRESULT_NULL_PARAM;
    }
	if( !_fileSecurityReport.isPresent() )
    {
		return AAFRESULT_PROP_NOT_PRESENT;
    }


    *pFileSecurityReport = _fileSecurityReport;


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::SetFileSecurityWave (
      aafUInt32  fileSecurityWave)
{
  _fileSecurityWave = fileSecurityWave;
	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::GetFileSecurityWave (
      aafUInt32 *  pFileSecurityWave)
{
  	if( pFileSecurityWave == NULL )
    {
        return AAFRESULT_NULL_PARAM;
    }
	if( !_fileSecurityWave.isPresent() )
    {
		return AAFRESULT_PROP_NOT_PRESENT;
    }


    *pFileSecurityWave = _fileSecurityWave;


    return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::AppendUnknownBWFChunk (
      ImplAAFRIFFChunk * pData)
{
	if (NULL == pData)
		return AAFRESULT_NULL_PARAM;
	if (pData->attached ())
		return AAFRESULT_OBJECT_ALREADY_ATTACHED;

	_unknownBWFChunks.appendValue(pData);
	pData->AcquireReference();
	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::PrependUnknownBWFChunk (
      ImplAAFRIFFChunk * pData)
{
	if (NULL == pData)
		return AAFRESULT_NULL_PARAM;
	if (pData->attached ())
		return AAFRESULT_OBJECT_ALREADY_ATTACHED;

	_unknownBWFChunks.prependValue(pData);
	pData->AcquireReference();
	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::InsertUnknownBWFChunkAt (
      aafUInt32 index, ImplAAFRIFFChunk * pData)
{
	if (NULL == pData)
		return AAFRESULT_NULL_PARAM;
	if (pData->attached ())
		return AAFRESULT_OBJECT_ALREADY_ATTACHED;
	if (index > _unknownBWFChunks.count())
		return AAFRESULT_BADINDEX;

	_unknownBWFChunks.insertAt(pData, index);
	pData->AcquireReference();
	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::GetUnknownBWFChunkAt (
      aafUInt32 index, ImplAAFRIFFChunk ** ppData)
{
	if (NULL == ppData)
		return AAFRESULT_NULL_PARAM;
	if (index >= _unknownBWFChunks.count())
		return AAFRESULT_BADINDEX;

	_unknownBWFChunks.getValueAt(*ppData, index);
	ASSERTU(*ppData);
	(*ppData)->AcquireReference();
	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::RemoveUnknownBWFChunkAt (
      aafUInt32 index)
{
	if (index >= _unknownBWFChunks.count())
		return AAFRESULT_BADINDEX;

	ImplAAFRIFFChunk *pData = _unknownBWFChunks.removeAt(index);
	if (pData)
		pData->ReleaseReference();
	return AAFRESULT_SUCCESS;
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::CountUnknownBWFChunks (
      aafUInt32 *  pNumData)
{
	if (pNumData == NULL)
		return AAFRESULT_NULL_PARAM;

	*pNumData = _unknownBWFChunks.count();
		
	return(AAFRESULT_SUCCESS);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::GetUnknownBWFChunks (
      ImplEnumAAFRIFFChunks ** ppEnum)
{
  if (NULL == ppEnum)
	return AAFRESULT_NULL_PARAM;
  *ppEnum = 0;
	
  ImplEnumAAFRIFFChunks *theEnum = (ImplEnumAAFRIFFChunks *)CreateImpl (CLSID_EnumAAFRIFFChunks);
	
  XPROTECT()
	{
		OMStrongReferenceVectorIterator<ImplAAFRIFFChunk>* iter = 
			new OMStrongReferenceVectorIterator<ImplAAFRIFFChunk>(_unknownBWFChunks);
		if(iter == 0)
			RAISE(AAFRESULT_NOMEMORY);
		CHECK(theEnum->Initialize(&CLSID_EnumAAFRIFFChunks, this, iter));
	  *ppEnum = theEnum;
	}
  XEXCEPT
	{
	  if (theEnum)
		{
		  theEnum->ReleaseReference();
		  theEnum = 0;
		}
	}
  XEND;
  
  return(AAFRESULT_SUCCESS);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::SetCodingHistory (
      aafCharacter_constptr  pCodingHistory)
{
  if(pCodingHistory == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(wcslen(pCodingHistory)*sizeof(OMCharacter) >= OMPROPERTYSIZE_MAX)
		return AAFRESULT_BAD_SIZE;

	_bextCodingHistory = pCodingHistory;

	return(AAFRESULT_SUCCESS); 
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::GetCodingHistory (
      aafCharacter *  pCodingHistory,
      aafUInt32  bufSize)
{
  if(pCodingHistory == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(!_bextCodingHistory.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	bool stat;
	
	stat = _bextCodingHistory.copyToBuffer(pCodingHistory, bufSize);
	if (! stat)
	{
	  return AAFRESULT_SMALLBUF;	
	}

	return(AAFRESULT_SUCCESS); 
}




AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::GetCodingHistoryBufLen (
      aafUInt32 *  pBufSize)
{
  if(pBufSize == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(! _bextCodingHistory.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	*pBufSize =  _bextCodingHistory.size();
	return(AAFRESULT_SUCCESS); 
}

     
AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::SetBasicData (
      aafCharacter_constptr  pBasicData)
{
  if(pBasicData == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(wcslen(pBasicData)*sizeof(OMCharacter) >= OMPROPERTYSIZE_MAX)
		return AAFRESULT_BAD_SIZE;

	_qltyBasicData = pBasicData;

	return(AAFRESULT_SUCCESS);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::GetBasicData (
      aafCharacter *  pBasicData,
      aafUInt32  bufSize)
{
  if(pBasicData == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(!_qltyBasicData.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	bool stat;
	
	stat = _qltyBasicData.copyToBuffer(pBasicData, bufSize);
	if (! stat)
	{
	  return AAFRESULT_SMALLBUF;	
	}

	return(AAFRESULT_SUCCESS); 
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::GetBasicDataBufLen (
      aafUInt32 *  pBufSize)
{
  if(pBufSize == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(! _qltyBasicData.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	*pBufSize =  _qltyBasicData.size();
	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::SetStartOfModulation (
      aafCharacter_constptr  pStartOfModulation)
{
  if(pStartOfModulation == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(wcslen(pStartOfModulation)*sizeof(OMCharacter) >= OMPROPERTYSIZE_MAX)
		return AAFRESULT_BAD_SIZE;

	_qltyStartOfModulation = pStartOfModulation;

	return(AAFRESULT_SUCCESS);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::GetStartOfModulation (
      aafCharacter *  pStartOfModulation,
      aafUInt32  bufSize)
{
  if(pStartOfModulation == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(!_qltyStartOfModulation.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	bool stat;
	
	stat = _qltyStartOfModulation.copyToBuffer(pStartOfModulation, bufSize);
	if (! stat)
	{
	  return AAFRESULT_SMALLBUF;	
	}

	return(AAFRESULT_SUCCESS); 
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::GetStartOfModulationBufLen (
      aafUInt32 *  pBufSize)
{
  if(pBufSize == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(! _qltyStartOfModulation.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	*pBufSize =  _qltyStartOfModulation.size();
	return(AAFRESULT_SUCCESS); 
}

     
AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::SetQualityEvent (
      aafCharacter_constptr  pQualityEvent)
{
  if(pQualityEvent == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(wcslen(pQualityEvent)*sizeof(OMCharacter) >= OMPROPERTYSIZE_MAX)
		return AAFRESULT_BAD_SIZE;

	_qltyQualityEvent = pQualityEvent;

	return(AAFRESULT_SUCCESS);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::GetQualityEvent (
      aafCharacter *  pQualityEvent,
      aafUInt32  bufSize)
{
  if(pQualityEvent == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(!_qltyQualityEvent.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	bool stat;
	
	stat = _qltyQualityEvent.copyToBuffer(pQualityEvent, bufSize);
	if (! stat)
	{
	  return AAFRESULT_SMALLBUF;	
	}

	return(AAFRESULT_SUCCESS); 
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::GetQualityEventBufLen (
      aafUInt32 *  pBufSize)
{
  if(pBufSize == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(! _qltyQualityEvent.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	*pBufSize =  _qltyQualityEvent.size();
	return(AAFRESULT_SUCCESS); 
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::SetEndOfModulation (
      aafCharacter_constptr  pEndOfModulation)
{
  if(pEndOfModulation == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(wcslen(pEndOfModulation)*sizeof(OMCharacter) >= OMPROPERTYSIZE_MAX)
		return AAFRESULT_BAD_SIZE;

	_qltyEndOfModulation = pEndOfModulation;

	return(AAFRESULT_SUCCESS);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::GetEndOfModulation (
      aafCharacter *  pEndOfModulation,
      aafUInt32  bufSize)
{
  if(pEndOfModulation == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(!_qltyEndOfModulation.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	bool stat;
	
	stat = _qltyEndOfModulation.copyToBuffer(pEndOfModulation, bufSize);
	if (! stat)
	{
	  return AAFRESULT_SMALLBUF;	
	}

	return(AAFRESULT_SUCCESS); 
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::GetEndOfModulationBufLen (
      aafUInt32 *  pBufSize)
{
 if(pBufSize == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(! _qltyEndOfModulation.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	*pBufSize =  _qltyEndOfModulation.size();
	return(AAFRESULT_SUCCESS); 
}

     
AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::SetQualityParameter (
      aafCharacter_constptr  pQualityParameter)
{
  if(pQualityParameter == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(wcslen(pQualityParameter)*sizeof(OMCharacter) >= OMPROPERTYSIZE_MAX)
		return AAFRESULT_BAD_SIZE;

	_qltyQualityParameter = pQualityParameter;

	return(AAFRESULT_SUCCESS);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::GetQualityParameter (
      aafCharacter *  pQualityParameter,
      aafUInt32  bufSize)
{
  if(pQualityParameter == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(!_qltyQualityParameter.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	bool stat;
	
	stat = _qltyQualityParameter.copyToBuffer(pQualityParameter, bufSize);
	if (! stat)
	{
	  return AAFRESULT_SMALLBUF;	
	}

	return(AAFRESULT_SUCCESS); 
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::GetQualityParameterBufLen (
      aafUInt32 *  pBufSize)
{
  if(pBufSize == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(! _qltyQualityParameter.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	*pBufSize =  _qltyQualityParameter.size();
	return(AAFRESULT_SUCCESS); 
}

     
AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::SetOperatorComment (
      aafCharacter_constptr  pOperatorComment)
{
  if(pOperatorComment == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(wcslen(pOperatorComment)*sizeof(OMCharacter) >= OMPROPERTYSIZE_MAX)
		return AAFRESULT_BAD_SIZE;

	_qltyOperatorComment = pOperatorComment;

	return(AAFRESULT_SUCCESS);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::GetOperatorComment (
      aafCharacter *  pOperatorComment,
      aafUInt32  bufSize)
{
  if(pOperatorComment == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(!_qltyOperatorComment.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	bool stat;
	
	stat = _qltyOperatorComment.copyToBuffer(pOperatorComment, bufSize);
	if (! stat)
	{
	  return AAFRESULT_SMALLBUF;	
	}

	return(AAFRESULT_SUCCESS); 
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::GetOperatorCommentBufLen (
      aafUInt32 *  pBufSize)
{
  if(pBufSize == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(! _qltyOperatorComment.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	*pBufSize =  _qltyOperatorComment.size();
	return(AAFRESULT_SUCCESS); 
}

     
AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::SetCueSheet (
      aafCharacter_constptr  pCueSheet)
{
  if(pCueSheet == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(wcslen(pCueSheet)*sizeof(OMCharacter) >= OMPROPERTYSIZE_MAX)
		return AAFRESULT_BAD_SIZE;

	_qltyCueSheet = pCueSheet;

	return(AAFRESULT_SUCCESS);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::GetCueSheet (
      aafCharacter *  pCueSheet,
      aafUInt32  bufSize)
{
  if(pCueSheet == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(!_qltyCueSheet.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	bool stat;
	
	stat = _qltyCueSheet.copyToBuffer(pCueSheet, bufSize);
	if (! stat)
	{
	  return AAFRESULT_SMALLBUF;	
	}

	return(AAFRESULT_SUCCESS); 
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::GetCueSheetBufLen (
      aafUInt32 *  pBufSize)
{
  if(pBufSize == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(! _qltyCueSheet.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	*pBufSize =  _qltyCueSheet.size();
	return(AAFRESULT_SUCCESS); 
}





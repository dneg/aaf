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
// The Original Code of this file is Copyright 1998-2005, Licensor of the
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
  
  	_fileSecurityReport( PID_BWFImportDescriptor_FileSecurityReport, L"FileSecurityReport"),
  	_fileSecurityWave( PID_BWFImportDescriptor_FileSecurityWave, L"FileSecurityWave"),
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
    ImplAAFBWFImportDescriptor::AppendUnknownBWFChunks (
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
    ImplAAFBWFImportDescriptor::CountUnknownBWFChunks (
      aafUInt32 *  pNumData)
{
  if (pNumData == NULL)
		return AAFRESULT_NULL_PARAM;

	if(!_unknownBWFChunks.isPresent())
	{
		*pNumData = 0; //return AAFRESULT_PROP_NOT_PRESENT;
	}
	else
	{
		*pNumData = _unknownBWFChunks.count();
	}
		
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
    ImplAAFBWFImportDescriptor::RemoveUnknownBWFChunks (
      ImplAAFRIFFChunk * pData)
{
  if (! pData)
		return AAFRESULT_NULL_PARAM;
  if (!pData->attached ()) // object could not possibly be in container.
    return AAFRESULT_OBJECT_NOT_ATTACHED;
	if(!_unknownBWFChunks.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
  size_t index;
  if (_unknownBWFChunks.findIndex (pData, index))
  {
	  _unknownBWFChunks.removeAt(index);
    // We have removed an element from a "stong reference container" so we must
    // decrement the objects reference count. This will not delete the object
    // since the caller must have alread acquired a reference. (transdel 2000-MAR-10)
    pData->ReleaseReference ();
  }
  else
  {
    return AAFRESULT_OBJECT_NOT_FOUND;
  }

	return(AAFRESULT_SUCCESS);
}


AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::SetBextCodingHistory (
      aafCharacter_constptr  pBextCodingHistory)
{
  if(pBextCodingHistory == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(wcslen(pBextCodingHistory)*sizeof(OMCharacter) >= OMPROPERTYSIZE_MAX)
		return AAFRESULT_BAD_SIZE;

	_bextCodingHistory = pBextCodingHistory;

	return(AAFRESULT_SUCCESS); 
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::GetBextCodingHistory (
      aafCharacter *  pBextCodingHistory,
      aafUInt32  bufSize)
{
  if(pBextCodingHistory == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(!_bextCodingHistory.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	bool stat;
	
	stat = _bextCodingHistory.copyToBuffer(pBextCodingHistory, bufSize);
	if (! stat)
	{
	  return AAFRESULT_SMALLBUF;	
	}

	return(AAFRESULT_SUCCESS); 
}




AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::GetBextCodingHistoryBufLen (
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
    ImplAAFBWFImportDescriptor::SetQltyBasicData (
      aafCharacter_constptr  pQltyBasicData)
{
  if(pQltyBasicData == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(wcslen(pQltyBasicData)*sizeof(OMCharacter) >= OMPROPERTYSIZE_MAX)
		return AAFRESULT_BAD_SIZE;

	_qltyBasicData = pQltyBasicData;

	return(AAFRESULT_SUCCESS);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::GetQltyBasicData (
      aafCharacter *  pQltyBasicData,
      aafUInt32  bufSize)
{
  if(pQltyBasicData == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(!_qltyBasicData.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	bool stat;
	
	stat = _qltyBasicData.copyToBuffer(pQltyBasicData, bufSize);
	if (! stat)
	{
	  return AAFRESULT_SMALLBUF;	
	}

	return(AAFRESULT_SUCCESS); 
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::GetQltyBasicDataBufLen (
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
    ImplAAFBWFImportDescriptor::SetQltyStartOfModulation (
      aafCharacter_constptr  pQltyStartOfModulation)
{
  if(pQltyStartOfModulation == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(wcslen(pQltyStartOfModulation)*sizeof(OMCharacter) >= OMPROPERTYSIZE_MAX)
		return AAFRESULT_BAD_SIZE;

	_qltyStartOfModulation = pQltyStartOfModulation;

	return(AAFRESULT_SUCCESS);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::GetQltyStartOfModulation (
      aafCharacter *  pQltyStartOfModulation,
      aafUInt32  bufSize)
{
  if(pQltyStartOfModulation == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(!_qltyStartOfModulation.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	bool stat;
	
	stat = _qltyStartOfModulation.copyToBuffer(pQltyStartOfModulation, bufSize);
	if (! stat)
	{
	  return AAFRESULT_SMALLBUF;	
	}

	return(AAFRESULT_SUCCESS); 
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::GetQltyStartOfModulationBufLen (
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
    ImplAAFBWFImportDescriptor::SetQltyQualityEvent (
      aafCharacter_constptr  pQltyQualityEvent)
{
  if(pQltyQualityEvent == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(wcslen(pQltyQualityEvent)*sizeof(OMCharacter) >= OMPROPERTYSIZE_MAX)
		return AAFRESULT_BAD_SIZE;

	_qltyQualityEvent = pQltyQualityEvent;

	return(AAFRESULT_SUCCESS);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::GetQltyQualityEvent (
      aafCharacter *  pQltyQualityEvent,
      aafUInt32  bufSize)
{
  if(pQltyQualityEvent == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(!_qltyQualityEvent.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	bool stat;
	
	stat = _qltyQualityEvent.copyToBuffer(pQltyQualityEvent, bufSize);
	if (! stat)
	{
	  return AAFRESULT_SMALLBUF;	
	}

	return(AAFRESULT_SUCCESS); 
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::GetQltyQualityEventBufLen (
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
    ImplAAFBWFImportDescriptor::SetQltyEndOfModulation (
      aafCharacter_constptr  pQltyEndOfModulation)
{
  if(pQltyEndOfModulation == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(wcslen(pQltyEndOfModulation)*sizeof(OMCharacter) >= OMPROPERTYSIZE_MAX)
		return AAFRESULT_BAD_SIZE;

	_qltyEndOfModulation = pQltyEndOfModulation;

	return(AAFRESULT_SUCCESS);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::GetQltyEndOfModulation (
      aafCharacter *  pQltyEndOfModulation,
      aafUInt32  bufSize)
{
  if(pQltyEndOfModulation == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(!_qltyEndOfModulation.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	bool stat;
	
	stat = _qltyEndOfModulation.copyToBuffer(pQltyEndOfModulation, bufSize);
	if (! stat)
	{
	  return AAFRESULT_SMALLBUF;	
	}

	return(AAFRESULT_SUCCESS); 
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::GetQltyEndOfModulationBufLen (
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
    ImplAAFBWFImportDescriptor::SetQltyQualityParameter (
      aafCharacter_constptr  pQltyQualityParameter)
{
  if(pQltyQualityParameter == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(wcslen(pQltyQualityParameter)*sizeof(OMCharacter) >= OMPROPERTYSIZE_MAX)
		return AAFRESULT_BAD_SIZE;

	_qltyQualityParameter = pQltyQualityParameter;

	return(AAFRESULT_SUCCESS);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::GetQltyQualityParameter (
      aafCharacter *  pQltyQualityParameter,
      aafUInt32  bufSize)
{
  if(pQltyQualityParameter == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(!_qltyQualityParameter.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	bool stat;
	
	stat = _qltyQualityParameter.copyToBuffer(pQltyQualityParameter, bufSize);
	if (! stat)
	{
	  return AAFRESULT_SMALLBUF;	
	}

	return(AAFRESULT_SUCCESS); 
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::GetQltyQualityParameterBufLen (
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
    ImplAAFBWFImportDescriptor::SetQltyOperatorComment (
      aafCharacter_constptr  pQltyOperatorComment)
{
  if(pQltyOperatorComment == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(wcslen(pQltyOperatorComment)*sizeof(OMCharacter) >= OMPROPERTYSIZE_MAX)
		return AAFRESULT_BAD_SIZE;

	_qltyOperatorComment = pQltyOperatorComment;

	return(AAFRESULT_SUCCESS);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::GetQltyOperatorComment (
      aafCharacter *  pQltyOperatorComment,
      aafUInt32  bufSize)
{
  if(pQltyOperatorComment == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(!_qltyOperatorComment.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	bool stat;
	
	stat = _qltyOperatorComment.copyToBuffer(pQltyOperatorComment, bufSize);
	if (! stat)
	{
	  return AAFRESULT_SMALLBUF;	
	}

	return(AAFRESULT_SUCCESS); 
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::GetQltyOperatorCommentBufLen (
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
    ImplAAFBWFImportDescriptor::SetQltyCueSheet (
      aafCharacter_constptr  pQltyCueSheet)
{
  if(pQltyCueSheet == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(wcslen(pQltyCueSheet)*sizeof(OMCharacter) >= OMPROPERTYSIZE_MAX)
		return AAFRESULT_BAD_SIZE;

	_qltyCueSheet = pQltyCueSheet;

	return(AAFRESULT_SUCCESS);
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::GetQltyCueSheet (
      aafCharacter *  pQltyCueSheet,
      aafUInt32  bufSize)
{
  if(pQltyCueSheet == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(!_qltyCueSheet.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;
	
	bool stat;
	
	stat = _qltyCueSheet.copyToBuffer(pQltyCueSheet, bufSize);
	if (! stat)
	{
	  return AAFRESULT_SMALLBUF;	
	}

	return(AAFRESULT_SUCCESS); 
}



AAFRESULT STDMETHODCALLTYPE
    ImplAAFBWFImportDescriptor::GetQltyCueSheetBufLen (
      aafUInt32 *  pBufSize)
{
  if(pBufSize == NULL)
		return(AAFRESULT_NULL_PARAM);

	if(! _qltyCueSheet.isPresent())
		return AAFRESULT_PROP_NOT_PRESENT;

	*pBufSize =  _qltyCueSheet.size();
	return(AAFRESULT_SUCCESS); 
}





//@doc
//@class    AAFEssenceData | Implementation class for AAFEssenceData
#ifndef __ImplAAFEssenceData_h__
#define __ImplAAFEssenceData_h__

#include "OMStorable.h"
#include "OMProperty.h"
#include "OMDataStreamProperty.h"

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

class ImplAAFSourceMob;


#ifndef __ImplAAFObject_h__
#include "ImplAAFObject.h"
#endif

#include "AAFPropertyIDs.h"

class ImplAAFEssenceData : public ImplAAFObject
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFEssenceData ();

protected:
  virtual ~ImplAAFEssenceData ();

public:

/****/
  //****************
  // Write()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Write
        (// @parm [in] write this many bytes
		 aafUInt32  bytes,

		 // @parm [out, size_is(buflen), length_is(*bytesRead)] here is the buffer
         aafDataBuffer_t  buffer,

		 // @parm [out,ref] 
		 aafUInt32 *  bytesWritten);

/****/
  //****************
  // Read()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Read
        (// @parm [in] read this many bytes
		 aafUInt32  bytes,

		 // @parm [out, size_is(buflen), length_is(*bytesRead)] here is the buffer
         aafDataBuffer_t  buffer,

		 // @parm [out,ref] 
		 aafUInt32 *  bytesRead);

/****/
  //****************
  // SetPosition()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetPosition
		// @parm [in] offset from beginning of essence
        (aafPosition_t  offset);

/****/
  //****************
  // SetPosition()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetPosition
		// @parm [in] offset from beginning of essence
        (aafPosition_t  *pOffset);

/****/
  //****************
  // GetSize()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetSize
		// @parm [out] size of essence data
        (aafLength_t *  pSize );

/****/
  //****************
  // SetFileMob()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetFileMob
		// @parm [in] reference to a file mob
        (ImplAAFSourceMob * pFileMob);

/****/
  //****************
  // GetFileMob()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetFileMob
		// @parm [in] reference to a file mob
        (ImplAAFSourceMob ** ppFileMob);

/****/
  //****************
  // GetFileMobID()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetFileMobID
		// @parm [out] the file mob id associated with essence
        (aafUID_t *  pFileMobID);



public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFEssenceData)

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFEssenceDataTest.cpp.
  static AAFRESULT test();

  // Return this objects stored object class.
  virtual AAFRESULT STDMETHODCALLTYPE
	GetObjectClass(aafUID_t * pClass);

private:
  OMFixedSizeProperty<aafUID_t> _fileMobID;
  OMDataStreamProperty _mediaData;
};

#endif // ! __ImplAAFEssenceData_h__


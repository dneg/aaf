//@doc
//@class    EnumAAFEssenceData | Implementation class for EnumAAFEssenceData
#ifndef __ImplEnumAAFEssenceData_h__
#define __ImplEnumAAFEssenceData_h__

#include "OMStorable.h"

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

class ImplAAFEssenceData;
class ImplAAFContentStorage;






#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif


class ImplEnumAAFEssenceData : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplEnumAAFEssenceData ();

protected:
  virtual ~ImplEnumAAFEssenceData ();

public:

  //****************
  // NextOne()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    NextOne
		// @parm [out,retval] The Next EssenceData
        (ImplAAFEssenceData ** ppEssenceData);

  //****************
  // Next()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Next
        (// @parm [in] number of essence data requested
		 aafUInt32  count,

		 // @parm [out, size_is(count), length_is(*pFetched)] array to receive EssenceData
		 ImplAAFEssenceData ** ppEssenceData,

		 // @parm [out,ref] number of actual EssenceData fetched into ppEssenceData array
         aafUInt32 *  pFetched);

  //****************
  // Skip()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Skip
		// @parm [in] Number of elements to skip
        (aafUInt32  count);

  //****************
  // Reset()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Reset ();


  //****************
  // Clone()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Clone
		// @parm [out,retval] new enumeration
        (ImplEnumAAFEssenceData ** ppEnum);

public:
// Internal to the toolkit
AAFRESULT SetContentStorage(ImplAAFContentStorage *pContentStorage);

private:
	aafUInt32 _current;
	ImplAAFContentStorage *_contentStorage;
};

#endif // ! __ImplEnumAAFEssenceData_h__


//@doc
//@class    EnumAAFMobs | Implementation class for EnumAAFMobs
#ifndef __ImplEnumAAFMobs_h__
#define __ImplEnumAAFMobs_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/


class ImplAAFMob;







#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif

#include "ImplAAFContentStorage.h"

class ImplEnumAAFMobs : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplEnumAAFMobs ();
  ~ImplEnumAAFMobs ();


  //****************
  // NextOne()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    NextOne
        (ImplAAFMob ** ppMob);  //@parm [out,retval] The Next Mob


  //****************
  // Next()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Next
        (aafUInt32  count,   //@parm [in] number of mobs requested
		 ImplAAFMob ** ppMobs,   //@parm [out, size_is(count), length_is(*pFetched)] array to receive mobs
         aafUInt32 *  pFetched);  //@parm [out,ref] number of actual Mobs fetched into ppMobs array


  //****************
  // Skip()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Skip
        (aafUInt32  count);  //@parm [in] Number of elements to skip


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
        (ImplEnumAAFMobs ** ppEnum);  //@parm [out,retval] new enumeration


public:
  // Declare the module test method. The implementation of the will be be
  // in /test/ImplEnumAAFMobsTest.cpp.
  static AAFRESULT test();

public:
// Internal to the toolkit
AAFRESULT
    SetContentStorage(ImplAAFContentStorage *pCStore);
AAFRESULT
    SetCriteria(aafSearchCrit_t *pCriteria);

private:
	aafInt32				_current;
	ImplAAFContentStorage	*_cStorage;
	aafSearchCrit_t			_criteria;
};

#endif // ! __ImplEnumAAFMobs_h__


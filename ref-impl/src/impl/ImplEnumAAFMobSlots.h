//@doc
//@class    EnumAAFMobSlots | Implementation class for EnumAAFMobSlots
#ifndef __ImplEnumAAFMobSlots_h__
#define __ImplEnumAAFMobSlots_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

class ImplAAFMobSlot;
class ImplAAFMob;






#ifndef __ImplAAFRoot_h__
#include "ImplAAFRoot.h"
#endif


class ImplEnumAAFMobSlots : public ImplAAFRoot
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplEnumAAFMobSlots ();
  virtual ~ImplEnumAAFMobSlots ();



  //****************
  // NextOne()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    NextOne
		// @parm [out,retval] The Next MobSlot
        (ImplAAFMobSlot ** ppMobSlot);

  //****************
  // Next()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    Next
        (// @parm [in] number of mob slots requested
		 aafUInt32  count,

		 // @parm [out, size_is(count), length_is(*pFetched)] array to receive mob slots
		 ImplAAFMobSlot ** ppMobSlots,

		 // @parm [out,ref] number of actual MobSlots fetched into ppMobSlots array
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
        (ImplEnumAAFMobSlots ** ppEnum);



public:
  // Declare the module test method. The implementation of the will be be
  // in /test/ImplEnumAAFMobSlotsTest.cpp.
  static AAFRESULT test();

//Internal
AAFRESULT SetEnumMob(ImplAAFMob *aMob);

private:
	aafInt32		_current;
	ImplAAFMob		*_mob;
};

#endif // ! __ImplEnumAAFMobSlots_h__


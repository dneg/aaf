//@doc
//@class    AAFGPITrigger | Implementation class for AAFGPITrigger
#ifndef __ImplAAFGPITrigger_h__
#define __ImplAAFGPITrigger_h__


/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/








#ifndef __ImplAAFEvent_h__
#include "ImplAAFEvent.h"
#endif


class ImplAAFGPITrigger : public ImplAAFEvent
{
public:
  //
  // Constructor/destructor
  //
  //********
  ImplAAFGPITrigger ();

protected:
  virtual ~ImplAAFGPITrigger ();

public:


  //****************
  // GetActiveState()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    GetActiveState
        // @parm [out, retval] pointer to the result
        (aafBool *  pActiveState);


  //****************
  // SetActiveState()
  //
  virtual AAFRESULT STDMETHODCALLTYPE
    SetActiveState
        // @parm [in] the active state of the trigger
        (aafBool  ActiveState);



public:
  // Declare this class to be storable.
  //
  OMDECLARE_STORABLE(ImplAAFGPITrigger)

  // Declare the module test method. The implementation of the will be be
  // in /test/ImplAAFGPITriggerTest.cpp.
  static AAFRESULT test();
};

#endif // ! __ImplAAFGPITrigger_h__



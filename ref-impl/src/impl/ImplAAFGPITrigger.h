//@doc
//@class    AAFGPITrigger | Implementation class for AAFGPITrigger
#ifndef __ImplAAFGPITrigger_h__
#define __ImplAAFGPITrigger_h__

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
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

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

protected:
  OMFixedSizeProperty<aafBool> _activeState;
};

#endif // ! __ImplAAFGPITrigger_h__



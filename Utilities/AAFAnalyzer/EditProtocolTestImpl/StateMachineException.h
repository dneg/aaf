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
//=---------------------------------------------------------------------=

#ifndef __STATEMACHINEEXCEPTION_H_
#define __STATEMACHINEEXCEPTION_H_

//Ax files
#include <AxEx.h>

class StateMachineException : public AxEx {

  public:

    StateMachineException( const wchar_t* what );
    virtual ~StateMachineException() throw();
    
  private:
  
    StateMachineException& operator=( const StateMachineException& );
    
};

#endif /*__STATEMACHINEEXCEPTION_H_*/

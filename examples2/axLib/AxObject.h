#ifndef __AxObject_h__
#define __AxObject_h__

//=---------------------------------------------------------------------=
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
// The Original Code of this file is Copyright 1998-2001, Licensor of the
// AAF Association.
// 
//=---------------------------------------------------------------------=


#include "AxTypes.h"
#include "AxSmartPointer.h"
#include "AxBaseObj.h"
#include "AxIterator.h"

#include <memory>

class AxObject : public AxBaseObj {
public:

	AxObject();
	AxObject( const AxObject& );
	AxObject( IAAFObjectSP spIaafObject );
	virtual ~AxObject();

	// FIXME - No need for auto_ptr<> here.  Better to optimize
	// AxString that to optimize use of AxString.
	auto_ptr< AxString > GetClassName() const;
	
	AxPropertyIter CreatePropertyIter() const;

private:

	AxObject& operator=( const AxObject& );
	
	mutable IAAFObjectSP _spIaafObject;
};

#endif

#ifndef __AxBaseObj_h__
#define __AxBaseObj_h__

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

#include <AxSmartPointer.h>

// AxBaseObj has no methods.  It is used to pass represent any
// object property or property value in an AAF file. Code using the
// class is expected to cast to one of the known concrete
// implementations in order to proceed with processing.

class AxBaseObj {
public:

	AxBaseObj()
	{}

	AxBaseObj( IUnknownSP sp )
		: _spIUnknown( sp ) {}

	virtual ~AxBaseObj() = 0;

	inline operator IUnknownSP ()
	{ return _spIUnknown; }

private:
	mutable IUnknownSP _spIUnknown;
};

template < class ObjType >
class AxBaseObjAny : public AxBaseObj {
public:
	AxBaseObjAny()
	{}

	AxBaseObjAny( const ObjType& obj )
	: _obj( obj )
	{}

	~AxBaseObjAny()
	{}

	const ObjType& get() const
	{
		return _obj;
	}

	ObjType& get()
	{
		return _obj;
	}

private:
	ObjType _obj;
};

//=---------------------------------------------------------------------=

#endif

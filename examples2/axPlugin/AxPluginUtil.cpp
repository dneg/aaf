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

#include "AxPluginUtil.h"

#include <memory>
#include <algorithm>

namespace {

class MaxIntFunc {
public:
	MaxIntFunc()
		: _val( 1 << (8*sizeof(int) - 1) )
	{}

	~MaxIntFunc()
	{}

	void operator()( AxPluginNameMap::PairType pair )
	{
		int size = pair.second.size();
		if ( size > _val ) {
			_val = size;
		}
	}

	int GetVal()
	{
		return _val;
	}

private:
	int _val;

};

} // end of namespace


//=---------------------------------------------------------------------=

bool AxPluginNameMap::IsFound( const aafUID_t& uid )
{
	if ( find(uid) == end() ) {
		return false;
	}
	else {
		return true;
	}
}


int AxPluginNameMap::GetMaxStringSize()
{
	MaxIntFunc max;

	std::for_each( begin(), end(), max );

	return max.GetVal();
}

int AxPluginNameMap::GetMaxCBufferSize()
{
	return (GetMaxStringSize()+1)*sizeof(CharTraits::char_type);
}
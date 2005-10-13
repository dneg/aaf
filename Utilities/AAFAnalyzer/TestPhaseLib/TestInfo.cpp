//=---------------------------------------------------------------------=
//
// $Id$
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

//Test/Result files
#include "TestInfo.h"

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer 
{

using namespace std;
using namespace boost;

TestInfo::TestInfo(const wstring& name, const shared_ptr<const vector<wstring> >& requirements)
    : _name(name), _spRequirementIds(requirements)
{
}

TestInfo::TestInfo(const TestInfo& other)
    : _name(other._name), _spRequirementIds(other._spRequirementIds)
{
}

TestInfo::~TestInfo()
{
}

const wstring TestInfo::GetName() const
{
    return _name;
}

const shared_ptr<const vector<wstring> > TestInfo::GetRequirementIds() const
{
    return _spRequirementIds;
}

} // end of namespace diskstream

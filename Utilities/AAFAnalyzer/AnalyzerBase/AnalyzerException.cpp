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

//Analyzer Base files
#include "AnalyzerException.h"

//STL files
#include <sstream>

namespace {

using namespace aafanalyzer;

} // end of namespace


//======================================================================
//======================================================================
//======================================================================

namespace aafanalyzer 
{

using namespace std;

AnalyzerException::AnalyzerException()
  : _widewhat(L""),
    _what("")
{}

AnalyzerException::AnalyzerException( const wchar_t* what )
  : _widewhat( what ),
    _what( InitWhat(what) )
{}

AnalyzerException::AnalyzerException( const wstring& what )
  : _widewhat( what ),
    _what( InitWhat(what) )
{}

AnalyzerException::~AnalyzerException() throw()
{}
    
const wchar_t* AnalyzerException::widewhat() const throw()
{
    return _widewhat.c_str();
}

const char* AnalyzerException::what() const throw()
{
  return _what.c_str();
}

string AnalyzerException::InitWhat( const wstring& s )
{
    stringstream os;
    const wchar_t* ws = s.c_str();
    
    size_t i;
    for (i = 0; i < s.length(); i++ ){ 
        char c;
        if ( -1 != wctomb( &c, ws[i] ) ) {
            os << c;
        }
    }

    return os.str();
}

} // end of namespace diskstream

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

//=---------------------------------------------------------------------=

#include "MultiGenTest.h"

#include <AAFCOMPlatformTypes.h>
#include <AAFResult.h>
#include <AAF.h>

#include <stdlib.h>
#include <assert.h>

#include <iostream>
#include <string>
#include <vector>
#include <memory>

MultiGenTestRegistry* MultiGenTestRegistry::_singleInstance = 0;

MultiGenTestRegistry& MultiGenTestRegistry::GetInstance()
{
  if ( !_singleInstance ) {
    _singleInstance = new MultiGenTestRegistry;
  }

  return *_singleInstance;
}

MultiGenTestRegistry::MultiGenTestRegistry()
  : _vec()
{
  ResetIter();
}

MultiGenTestRegistry::~MultiGenTestRegistry()
{
  // Object is never destroyed.
}

void MultiGenTestRegistry::Register( MultiGenTestFactory* factory )
{
  assert( !IsKnown( factory->GetName() ) );

  _vec.push_back( factory );

  ResetIter();
}

MultiGenTestFactory* MultiGenTestRegistry::Next()
{
  if ( _vecIter != _vec.end() ) {
    MultiGenTestFactory *ret = *_vecIter;
    ++_vecIter;
    return ret;
  }
  else {
    return 0;
  }
}

void MultiGenTestRegistry::ResetIter()
{
  _vecIter = _vec.begin();
}

MultiGenTestFactory& MultiGenTestRegistry::GetFactory( const char* name )
{
  string nameString( name );

  std::vector<MultiGenTestFactory*>::const_iterator iter;

  for( iter = _vec.begin();
       iter != _vec.end();
       ++iter ) {
    if ( nameString == (*iter)->GetName() ) {
      return **iter;
    }
  }

  throw "Uknown test name in GetFactory()";
}

bool MultiGenTestRegistry::IsKnown( const char* name )
{
  string nameString( name );

  std::vector<MultiGenTestFactory*>::const_iterator iter;

  for( iter = _vec.begin();
       iter != _vec.end();
       ++iter ) {
    if ( nameString == (*iter)->GetName() ) {
      return true;
    }
  }

  return false;
}

//=---------------------------------------------------------------------=

MultiGenTestFactory::MultiGenTestFactory( const char* name,
					  const char* desc,
					  const char* usage,
					  const char* notes )
  : _name( name ),
    _desc( desc ),
    _usage( usage ),
    _notes( notes )
{}

MultiGenTestFactory::~MultiGenTestFactory()
{}

const char* MultiGenTestFactory::GetName() const
{
  return _name;
}

const char* MultiGenTestFactory::GetDesc() const
{
  return _desc;
}

const char* MultiGenTestFactory::GetUsage() const
{
  return _usage;
}

const char* MultiGenTestFactory::GetNotes() const
{
  return _notes;
}

//=---------------------------------------------------------------------=

MultiGenTest::MultiGenTest( const char* name,
			    const char* desc,
			    const char* usage,
			    const char* notes )
  : _name( name ),
    _desc( desc ),
    _usage( usage ),
    _notes( notes )
{}

MultiGenTest::~MultiGenTest()
{}

const char* MultiGenTest::GetName() const
{
  return _name;
}

const char* MultiGenTest::GetDesc() const
{
  return _desc;
}

const char* MultiGenTest::GetUsage() const
{
  return _usage;
}

const char* MultiGenTest::GetNotes() const
{
  return _notes;
}

//=---------------------------------------------------------------------=

CmdState::CmdState()
  : _isFileSet( false )
{}

CmdState::~CmdState()
{}

void CmdState::SetFile( IAAFSmartPointer<IAAFFile> iaafFile )
{
  _iaafFile = iaafFile;
  _isFileSet = true;
}

IAAFSmartPointer<IAAFFile> CmdState::GetFile()
{
  if ( !_isFileSet ) {
    throw "No file!";
  }

  return _iaafFile;
}

void CmdState::ClearPriorToUnload()
{
	if ( _isFileSet ) {
		IAAFSmartPointer<IAAFFile> null;
		_iaafFile = null;
		_isFileSet = false;
	}
}

//=---------------------------------------------------------------------=

void Usage( const char* argv0 )
{
  cout << "Usage: " << argv0 << endl;
  cout << "        [{-r|-run} TestName Arg1 Arg2 ....] ..." << endl;              
  cout << endl;
  cout << "Registered Tests:" << endl << endl;

  MultiGenTestFactory* factory;
  MultiGenTestRegistry& registry = MultiGenTestRegistry::GetInstance();
  registry.ResetIter();
  while ( factory = registry.Next() ) {
    cout << "Name:\t" << factory->GetName()  << endl;
    cout << "Desc:\t" << factory->GetDesc()  << endl;
    cout << "Usage:\t" << factory->GetUsage() << endl;
    cout << "Notes:\t" << factory->GetNotes() << endl;
    cout << endl;
  }
}

//=---------------------------------------------------------------------=

class CmdFunc {
public:
  CmdFunc( int minArgs )
    : _minArgs( minArgs ),
      _subArgV(0),
      _subArgC(-1)
  {}
  virtual void operator()( CmdState& state ) {}
  int GetMinArgs()               { return _minArgs; }
  void SetArgC( int subArgC )    { _subArgC = subArgC; }
  void SetArgV( char **subArgV ) { _subArgV = subArgV; }
  int GetArgC()                  { return _subArgC; }
  char** GetArgV()               { return _subArgV; }
private:
  int _minArgs;
  int _subArgC;
  char** _subArgV;
};

// Currently this is the only CmdFunc implementation.
// If other are required, just derived from CmdFunc,
// and add test for in on the command line as is done
// for "-r" below.

class RunCmd : public CmdFunc {
public:
  RunCmd() : CmdFunc(1) {}

  virtual void operator()(CmdState& state)
  {
      MultiGenTestRegistry& registry = MultiGenTestRegistry::GetInstance();

      if ( !registry.IsKnown( GetArgV()[0] ) ) {
  	    string anError( string( "Can't run unkown test: " ) + string( GetArgV()[0] ) );
		throw anError;
      }

      MultiGenTestFactory& factory = registry.GetFactory( GetArgV()[0] );

      auto_ptr<MultiGenTest> test( factory.Create() );
      test->RunTest( state, GetArgC(), GetArgV() );
  }
};

void ProcessCommandLineArgs( int argc, char** argv )
{
  // Run the the command line arguments plucking out the indices of
  // those that are recognized as commands.  Save the index, and
  // create a command object to execute it.
  // 
  // Next, for each option figure the start/end argument indices that
  // the command object will pass through to the test implementation.
  //
  // Finally, execute each command options.  Inter-command state is
  // held in a state object passed as an argument to each command.

  vector<int> optionIndices;
  vector<CmdFunc*> optionCmdFuncs;

  int i;
  for( i = 1; i < argc; i++ ) {
    string arg( argv[i] );

    if ( arg == "-r" || arg == "-run" ) {
      optionIndices.push_back(i);
      optionCmdFuncs.push_back( new RunCmd );
    }
  }

  optionIndices.push_back( argc );
  for(i = 0; i < optionIndices.size()-1; i++ ) {

    if ( optionIndices[i] + optionCmdFuncs[i]->GetMinArgs() >= optionIndices[i+1] ) {
      string anError( string("not enough arguments for command: ") +
		      string(argv[optionIndices[i]]) );
      throw anError;
    }
    else {
      optionCmdFuncs[i]->SetArgV( &argv[ optionIndices[i]+1 ] );
      optionCmdFuncs[i]->SetArgC( optionIndices[i+1] - (optionIndices[i] + 1) );
    }
  }

  CmdState* state = new CmdState;
  for(i = 0; i < optionCmdFuncs.size(); i++ ) {
	cout << *optionCmdFuncs[i]->GetArgV() << "... ";
	(*optionCmdFuncs[i])( *state );
	cout << "done" << endl;
  }

  delete state;
}

//=---------------------------------------------------------------------=

wchar_t* ToWideString( const char* str )
{  
  int len = strlen( str );
  wchar_t *wstr = new wchar_t [ len + 1 ];

  int i;
  for( i = 0; i < len; i++ ) {
    mbtowc( &wstr[i], &str[i], 1 );
  }
  wstr[i] = 0;

  return wstr;
}

//=---------------------------------------------------------------------=

// True if identical
bool wstrcmp( wchar_t* a, wchar_t* b )
{
  if ( !(a && b) ) {
    return false;
  }

  while( (*a && *b) && ( *a == *b ) ) {
    a++;
    b++;
  }

  if ( *a || *b ) {
    return false;
  }
  else {
    return true;
  }
}

//=---------------------------------------------------------------------=

//
// Test multiplatform read/modify/write of AAF files.
//

int main( int argc, char **argv )
{
  using namespace std;

  HRESULT hr = AAFRESULT_SUCCESS;

 try {

    if ( argc == 1 ) {
      Usage( argv[0] );
      throw -1;
    }

    MultiGenTestRegistry& registry = MultiGenTestRegistry::GetInstance();

    ProcessCommandLineArgs( argc, argv );
 }

  catch ( const HResultEx& ex_hr ) {
    hr = ex_hr.GetHResult(); 
    cout << "Error: HRESULT value" << endl;
  }
  catch ( const string& ex_string ) {
    cout << "Error: " << ex_string << endl;
    hr = -1;
  }
  catch ( const char* ex_str ) {
    cout << "Error: " << ex_str << endl;
    hr = -1;
  }
  catch ( const UsageEx& ex_usage ) {
    cout << "Usage Error: " << ex_usage.GetMsg() << endl;
    hr = -1;
  }
  catch (...) {
	cout << "Error: caught an unknown exception" << endl;
    hr = -1;
  }

  cout << "status 0x" << hex << hr << endl;

  return hr;
}

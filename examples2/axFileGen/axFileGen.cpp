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

#include "AxFileGen.h"

#include <AxFile.h>
#include <AxHeader.h>
#include <AxDictionary.h>


#include <AxInit.h>
#include <AxUtil.h>

#include <ctype.h>

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <exception>

//=---------------------------------------------------------------------=


//=---------------------------------------------------------------------=

AxFGOp& AxFGOp::GetInstance( const AxString& instanceName )
{
	AxFGOp& instance = AxFGOpRegistry::GetInstance().Get( instanceName );
	instance._instanceName = instanceName;
	return instance;
}

void AxFGOp::RemoveInstance( const AxString& instanceName )
{
	GetInstance( instanceName ).DecRef();
	AxFGOpRegistry::GetInstance().Remove( instanceName );
}


AxFGOp::AxFGOp( const AxString& operationName  )
:  _operationName( operationName ),
   _refCount( 0 )
{}

AxFGOp::~AxFGOp()
{}

void AxFGOp::RegisterInstance( const AxString& instanceName )
{
	_instanceName = instanceName;
	AxFGOpRegistry::GetInstance().Register( this );
	AddRef();
}

void AxFGOp::AddRef()
{
	_refCount++;
}

void AxFGOp::DecRef()
{
	_refCount--;

	if ( 0 == _refCount ) {
		delete this;
	}
}

int AxFGOp::GetRef() const
{
	return _refCount;
}

AxString AxFGOp::GetName() const
{
	return _instanceName;
}

AxString AxFGOp::GetOpName() const
{
	return _operationName;
}

AxString AxFGOp::GetDesc() const
{
	return AxFGOpFactoryRegistry::GetInstance().Get(_operationName).GetDesc();
}

AxString AxFGOp::GetUsage() const
{
	return AxFGOpFactoryRegistry::GetInstance().Get(_operationName).GetUsage();
}

AxString AxFGOp::GetNotes() const
{
	return AxFGOpFactoryRegistry::GetInstance().Get(_operationName).GetNotes();
}

int AxFGOp::GetMinArgC() const
{
	return AxFGOpFactoryRegistry::GetInstance().Get(_operationName).GetMinArgC();
}

int AxFGOp::GetMaxArgC() const
{
	return AxFGOpFactoryRegistry::GetInstance().Get(_operationName).GetMaxArgC();
}

//=---------------------------------------------------------------------=

AxFGOpFactory::AxFGOpFactory( const AxString& name,
			                  const AxString& desc,
				              const AxString& usage,
				              const AxString& notes,
				              int   minArgc,
				              int   maxArgc )
 :	_name( name ),
	_desc( desc ),
	_usage( usage ),
	_notes( notes ),
	_minArgc( minArgc ),
	_maxArgc( maxArgc )
{
	if ( _minArgc < 1 ) {
		// Don't throw an error - the factories are instantiated from global objects.
		// There is nothing to catch the exception!
		std::wcout << L"*** Warning: Minimum arg count less than one used for: " << name;
		std::wcout << L".  The value was adjusted." << std::endl << std::endl;
		_minArgc = 1;
	}

	AxFGOpFactoryRegistry::GetInstance().Register( this );
}

AxFGOpFactory::~AxFGOpFactory()
{}

AxString AxFGOpFactory::GetName() const
{
	return _name;
}

AxString AxFGOpFactory::GetDesc() const
{
	return _desc;
}

AxString AxFGOpFactory::GetUsage() const
{
	return _usage;
}

AxString AxFGOpFactory::GetNotes() const
{
	return _notes;
}

int AxFGOpFactory::GetMinArgC() const
{
	return _minArgc;
}

int AxFGOpFactory::GetMaxArgC() const
{
	return _maxArgc;
}

//=---------------------------------------------------------------------=

template <class T>
AxFGRegistry<T>* AxFGRegistry<T>::_singleInstance = 0;

template <class T>
AxFGRegistry<T>& AxFGRegistry<T>::GetInstance()
{
	if ( !_singleInstance ) {
		_singleInstance = new AxFGRegistry;
	}

	return *_singleInstance;
}

template <class T>
AxFGRegistry<T>::AxFGRegistry()
: _map()
{ 
	ResetIter();
}

template <class T>
AxFGRegistry<T>::~AxFGRegistry()
{}

template <class T>
void AxFGRegistry<T>::Register( T* object )
{
	if ( IsKnown( object->GetName() ) ) {
		throw AxFGEx( L"Failed to register \"" + object->GetName() + L"\". The name is not unique." );
	}
	
	_map[ object->GetName() ] = object;
}

template <class T>
void AxFGRegistry<T>::Remove( const AxString& name )
{
	if ( !IsKnown( name ) ) {
		throw AxFGEx( L"Failed to remove \"" + name + L"\" from a registry. The name is not known." );
	}

	MapIterType iter = _map.find( name );
	_map.erase( iter );
}

template <class T>
T& AxFGRegistry<T>::Get( const AxString& name )
{
	if ( !IsKnown( name ) ) {
		throw AxFGEx( L"\"" + name + L"\" is unknown." );
	}

	return *_map[ name ];
}

template <class T>
std::pair<AxString, const T*> AxFGRegistry<T>::Next()
{
	pair<AxString, const T*> ret;
	ret.first = L"";
	ret.second = 0;

	if ( _mapIter != _map.end() ) {
		ret.first = _mapIter->first;
		ret.second = _mapIter->second;
		++_mapIter;
	}

	return ret;
}

template <class T>
void AxFGRegistry<T>::ResetIter()
{
	_mapIter = _map.begin();
}

template <class T>
bool AxFGRegistry<T>::IsKnown( const AxString& name )
{
	MapIterType iter = _map.find( name );
	
	if ( iter == _map.end() ) {
		return false;
	}
	else {
		return true;
	}
}


//=---------------------------------------------------------------------=

IAAFFileSP FileFromFileOp( AxFGOp& op )
{
	IAAFFileSP file;
	op.GetCOM( file );
	return file;
}

IAAFHeaderSP HeaderFromFileOp( AxFGOp& op )
{
	AxFile axFile( FileFromFileOp( op ) );
	return axFile.getHeader();
}

IAAFDictionarySP DictionaryFromFileOp( AxFGOp& op )
{
	AxHeader axHeader( HeaderFromFileOp( op ) );
	return axHeader.GetDictionary();
}



IAAFFileSP FileFromFileOp( const AxString& name )
{
	AxFGOp& op = AxFGOp::GetInstance( name );
	return FileFromFileOp( op );
}

IAAFHeaderSP HeaderFromFileOp( const AxString& name )
{
	AxFile axFile( FileFromFileOp( name ) );
	return axFile.getHeader();
}

IAAFDictionarySP DictionaryFromFileOp( const AxString& name )
{
	AxHeader axHeader( HeaderFromFileOp( name ) );
	return axHeader.GetDictionary();
}

//=---------------------------------------------------------------------=


AxFGOpUsageEx::AxFGOpUsageEx( const AxFGOp& op )
{
	std::wostringstream os;
	os << L"Usage: " << std::endl;
	opUseMessage( os, op );
	_what = os.str();
}

AxFGOpUsageEx::AxFGOpUsageEx( const AxFGOp& op, const AxString msg )
{
	std::wostringstream os;
	os << L"Usage: " << msg << std::endl << std::endl;
	opUseMessage( os, op );
	_what = os.str();
}

const AxString AxFGOpUsageEx::what() const
{
	return _what;
}

void AxFGOpUsageEx::opUseMessage( std::wostringstream& os, const AxFGOp& op )
{
	os << L"Op Name:\t" << op.GetOpName() << std::endl; 
	os << L"Usage:\t\t" << op.GetUsage() << std::endl;
	os << L"Desc\t\t"  << op.GetDesc()  << std::endl;
	os << L"Notes\t\t" << op.GetNotes() << std::endl;
	os << std::endl;
}

//=---------------------------------------------------------------------=


void CheckCommand( AxString opName, int argC )
{
      AxFGOpFactoryRegistry& registry =
		  AxFGOpFactoryRegistry::GetInstance();

      if ( !registry.IsKnown( opName ) ) {
  	    AxString anError( L"Unkown operation: " + opName );
		throw anError;
      }

      AxFGOpFactory& factory = registry.Get( opName );

      if ( argC < factory.GetMinArgC() ) {
		wostringstream anError;
		anError << L"Expected at least " << factory.GetMinArgC() <<
		L" arguments for operation: " << opName;
		throw anError.str();
      }

      if ( factory.GetMaxArgC() != -1 &&
	       argC > factory.GetMaxArgC() ) {
		wostringstream anError;
		anError << L"Expected at most " << factory.GetMaxArgC() <<
		L" arguments for operation: " << opName;
		throw anError.str();
      }
}

class CmdFunc {
public:	
  CmdFunc( int minArgs )
    : _minArgs( minArgs ),
      _subArgV(0),
      _subArgC(-1)
  {}

  virtual ~CmdFunc()
  {}

  virtual void Prepare() {}
  virtual void Execute() {}

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

//=---------------------------------------------------------------------=

class RunCmd : public CmdFunc {
public:
  RunCmd() : CmdFunc(1) {}

  virtual void Prepare()
  {
	  CheckCommand( AxStringUtil::mbtowc( GetArgV()[0] ), GetArgC() ) ;
  }
  
  virtual void Execute();
};

void RunCmd::Execute()
{
    AxFGOpFactoryRegistry& registry = AxFGOpFactoryRegistry::GetInstance();

	AxString opName = AxStringUtil::mbtowc( GetArgV()[0] );

    AxFGOpFactory& factory = registry.Get( opName );

    AxFGOp* op = factory.CreateInstance();

	std::vector<AxString> argList;
	int i;
	for( i = 0; i < GetArgC(); ++i ) {
		cout << GetArgV()[i] << " ";
		argList.push_back( AxStringUtil::mbtowc( GetArgV()[i] ) );
	}

	cout << "... ";
	cout.flush();

    op->Execute( argList );
	op->DecRef();

	cout << "done" << endl;
}


//=---------------------------------------------------------------------=

class FileCmd : public CmdFunc {
public:

	class OpenedFile {
	public:
		OpenedFile( FILE *fp )
		: _fp( fp )
		{}
		~OpenedFile()
		{ fclose( _fp ); }
	private:
		FILE* _fp;
	};

	
	FileCmd()
		: CmdFunc(1)
	{}

	~FileCmd()
	{}

	virtual void Prepare()
    {
		ReadFile( GetArgV()[0] );
		CheckAllCommands();
	}

	virtual void Execute();

private:

	void ReadFile( const char* fileName );
	void CheckAllCommands();

	vector< vector< string > > _cmdlines;
};


// All platform "is end of line" character.
// The parser worries about the fact that dos uses two characters.
inline bool iseol( char c )
{
	if ( c == '\n' || c == '\r' ) {
		return true;
	}
	else {
		return false;
	}
}

void FileCmd::ReadFile( const char* fileName )
{
	// Forget about streams for this... easier char by char.
	FILE *fp = fopen( fileName, "r" );

	if ( !fp ) {
		throw AxFGEx( L"Failed to open: " + AxStringUtil::mbtowc( fileName ) );
	}

	// Exception proof the open file handle... one less thing to worry about.
	OpenedFile openFile( fp );

	// This is an ad hoc parser, the structure might politely be
	// described as ... ahhh... organic.
	// ... oh let's be honest.. IT'S A POS.. nearly!  I plan to replace it with a real
	// state machine driven parser... also it looks like it not possible to avoid
	// defining a small grammer for the file.  Complications arise when supporting
	// end of line comments in a mid command position, or mid quote.  The parser
	// uses the awkward "\#" to identify end of line comments..  A clear start of
	// command grammer would resolve this..  It is also required to support white
	// space independent parsing... that would do away with all the escaped end
	// of lines.

	// Various parser state variables.
	bool quote_on = false;
	bool string_on = false;
	bool start_new_line = true;
	bool start_new_string = true;
	bool comment_on = false;
	bool escaped_comment_on = false;
	char look_ahead_one;
	char look_ahead_two;
	char c;

	for( c = ::getc(fp), look_ahead_one = ::getc(fp), look_ahead_two = ::getc(fp);
		 c != EOF;
		 c = look_ahead_one, look_ahead_one = look_ahead_two, look_ahead_two = ::getc( fp ) ) {

		if ( !isascii(c) ) {
			throw AxFGEx( L"Found non ascii character." );
		}

        else if ( c == '\\' ) {

			bool skip = false;

			if ( isascii(look_ahead_one) && iseol(look_ahead_one) &&
			  	 isascii(look_ahead_two) && iseol (look_ahead_two) ) {
				// Skip the escaped end of line.
				look_ahead_one = getc( fp );
				look_ahead_two = getc( fp );
				skip = true;
			}
			else if ( isascii(look_ahead_one) && iseol (look_ahead_one) ) {

				// Skip the escaped end of line.
				look_ahead_one = look_ahead_two;
				look_ahead_two = getc( fp );
				skip = true;
			}
			else if ( look_ahead_one == '#' ) {
				escaped_comment_on = true;
				comment_on = true;
			}

			// Should never hit end of line while in the middle of a quoted
			// string.
			if ( skip ) {
				
				if ( quote_on ) {
					throw AxFGEx( L"Open quote at escaped end of line.." );
				}

				if ( skip && comment_on ) {
					throw AxFGEx( L"Escaped end of line in comment." );
				}

				// Reset all state.
				quote_on = false;
				string_on = false;
				start_new_string = true;
				comment_on = false;
			}

			continue;

		}

		
		// Assume that the only control characters we see are
		// linefeed or carraige return.
		else if ( iseol (c) ) {
			// Should never hit end of line while in the middle of a quoted
			// string.
			if ( quote_on ) {
				throw AxFGEx( L"Open quote at end of line." );
			}

			// Reset all state.
			quote_on = false;
			string_on = false;
			start_new_string = true;

			if ( !escaped_comment_on ) {
				start_new_line = true;
			}

			escaped_comment_on = false;
			comment_on = false;

			continue;
		}

		// skip comments
		else if ( comment_on ) {
			continue;
		}

		// start of quoted string
		else if ( !quote_on && c == '"' ) {
			quote_on = true;
			string_on = true;
			start_new_string = true;
			continue;
		}

		// end of quoted string
		else if ( quote_on && c == '"' ) {
			quote_on = false;
			string_on = false;
			start_new_string = true;
			continue;
		}
		else if ( !quote_on && c == '#' ) {
			string_on = false;
			start_new_string = true;
			start_new_line = true;
			comment_on = true;
		}
		else {

			// If c is white space, and we're not in midst of a quoted
			// string, then skip it.
			if ( isspace( c ) && !quote_on ) {
				string_on = false;
				start_new_string = true;
				continue;
			}

			// It is character... figure out what to do with it.

			if ( start_new_line ) {
				_cmdlines.push_back( std::vector<string>() );
				start_new_line = false;
			}
			
			if ( start_new_string ) {
				_cmdlines.back().push_back( string() );
				start_new_string = false;
			}

			string_on = true;
			_cmdlines.back().back().insert( _cmdlines.back().back().end() , c );
		}		  
	}
}

void FileCmd::CheckAllCommands()
{
	vector< vector< string > >::iterator lines_iter;

	// Probably have to reactivate the output code
	// when the file parser is redone... so leave it.
#if 0
	cout << endl << "File commands:" << endl << endl;
#endif

	for ( lines_iter = _cmdlines.begin(); lines_iter != _cmdlines.end(); ++lines_iter ) {

		CheckCommand( AxStringUtil::mbtowc( (*lines_iter)[0].c_str() ),
				      lines_iter->size() );
		
#if 0
		for( iter = lines_iter->begin(); iter != lines_iter->end(); ++iter ) {
			cout << "[" << *iter << "] " ;
		}
		cout << endl;
#endif
	
	}
}

void FileCmd::Execute()
{
    AxFGOpFactoryRegistry& registry = AxFGOpFactoryRegistry::GetInstance();

	vector< vector< string > >::iterator lines_iter;
	vector< string >::iterator iter;

	cout << endl << "Executing: " << GetArgV()[0] << endl;
	
	for ( lines_iter = _cmdlines.begin(); lines_iter != _cmdlines.end(); ++lines_iter ) {
	
		AxString opName = AxStringUtil::mbtowc( lines_iter->begin()->c_str() );

		AxFGOpFactory& factory = registry.Get( opName );

		AxFGOp* op = factory.CreateInstance();

		std::vector<AxString> argList;

		for( iter = lines_iter->begin(); iter != lines_iter->end(); ++iter ) {
			cout << *iter << " ";
			argList.push_back( AxStringUtil::mbtowc( iter->c_str() ) );
		}	

		cout << "... ";
		cout.flush();
	
		op->Execute( argList );
		op->DecRef();

		cout << "done" << endl;
	}

	cout << "Finished executing: " << GetArgV()[0]  << endl << endl;
}

//=---------------------------------------------------------------------=

  // Slice and dice the command line.

  // Run through the command line arguments plucking out the indices
  // of those that are recognized as commands.  Save the index, and
  // create a command object to execute it.
  // 
  // For each option determine the start/end argument indices that the
  // command object will pass through.
  //
  // For each command objects, verify that the test exists and the the
  // number of command line options match the number expected by the
  // command.
  //

void ProcessCommandLineArgs( int argc, char** argv )
{
	using namespace std;

	vector<int> optionIndices;
	std::vector<CmdFunc*> optionCmdFuncs;

  // Pluck the -r, and -f options. Create a command object for each.
  int i;
  for( i = 1; i < argc; i++ ) {
    string arg( argv[i] );

    if ( arg == "-r" || arg == "-run" ) {
      optionIndices.push_back(i);
      optionCmdFuncs.push_back( new RunCmd );
    }

    else if ( arg == "-f" || arg == "-file" ) {
      optionIndices.push_back(i);
      optionCmdFuncs.push_back( new FileCmd );
    }
  }

  // Verify that each option has the required minumum number of arguments.
  optionIndices.push_back( argc );
  for(i = 0; i < optionIndices.size()-1; i++ ) {

    if ( optionIndices[i] + optionCmdFuncs[i]->GetMinArgs() >= optionIndices[i+1] ) {
      AxString anError( L"not enough arguments for command: " +
		  AxStringUtil::mbtowc(argv[optionIndices[i]]) );
      throw anError;
    }
    else {
      optionCmdFuncs[i]->SetArgV( &argv[ optionIndices[i]+1 ] );
      optionCmdFuncs[i]->SetArgC( optionIndices[i+1] - (optionIndices[i] + 1) );
    }
  }

  for(i = 0; i < optionCmdFuncs.size(); i++ ) {
      CmdFunc* cmd = optionCmdFuncs[i];
	  cmd->Prepare();
  }

  for(i = 0; i < optionCmdFuncs.size(); i++ ) {
	  optionCmdFuncs[i]->Execute();
  }

}


//=---------------------------------------------------------------------=

// Only use when the program is exiting.
// Decrementing all the references will cause
// the objects to self destruct.
void ClearInstanceRegistry()
{
	using namespace std;
	
	pair<AxString, const AxFGOp*> op;

	AxFGOpRegistry::GetInstance().ResetIter();

	for ( op = AxFGOpRegistry::GetInstance().Next();
	      op.second;
	      op = AxFGOpRegistry::GetInstance().Next() ) {

		// Drat! A const cast!
		const_cast<AxFGOp*>(op.second)->DecRef();
	}
}

//=---------------------------------------------------------------------=

void DumpFactoryRegistry()
{
	using namespace std;
	
	pair<AxString, const AxFGOpFactory*> factory;

	AxFGOpFactoryRegistry::GetInstance().ResetIter();

	for ( factory = AxFGOpFactoryRegistry::GetInstance().Next();
	      factory.second;
	      factory = AxFGOpFactoryRegistry::GetInstance().Next() ) {

		wcout << L"Op Name:\t"  << factory.first  << endl;
		wcout << L"Usage:\t\t" << factory.second->GetUsage() << endl;
		wcout << L"Desc\t\t"  << factory.second->GetDesc()  << endl;
		wcout << L"Notes\t\t" << factory.second->GetNotes() << endl;
		wcout << endl;
	}
}

//=---------------------------------------------------------------------=

void Usage( const char* argv0 )
{
  using namespace std;

  cout << "Usage: " << argv0 << endl;
  cout << "        [{-r|-run} OpName Arg1 Arg2 ....] [{-f|-file} command_file]..." << endl;              
  cout << endl;
  cout << "Registered Operations:" << endl << endl;

  DumpFactoryRegistry();
}

//=---------------------------------------------------------------------=

int main( int argc, char **argv )
{
	using namespace std;
	int err = 0;
	
	try {

		if ( 1 == argc ) {
			Usage( argv[0] );
			throw AxFGEx( L"No arguments." );
		}

		AxInit axInit;
	
		ProcessCommandLineArgs( argc, argv );

		ClearInstanceRegistry();
	}

	catch ( const AxFGEx& ex ) {
		wcout << endl << endl << L"Error: " << ex.what() << endl;
		err = -1;
	}
	catch ( const AxEx& ex ) {
		wcout << endl << endl << L"Error: " << ex.what() << endl;
		err = -1;
	}
	catch ( const AxString& ex ) {
		wcout << endl << endl << L"Error: " << ex << endl;
		err = -1;
	}
	catch ( exception& ex ) {
		cout << endl << endl << "Error: " << ex.what() << endl;
		err = -1;
	}
	catch ( ... ) {
		cout << endl << endl << "Error: caught unknown exception" << endl;
		err = -1;
	}
		
	return err;
}

//=---------------------------------------------------------------------=

// Explicit instantiations
template class AxFGRegistry<AxFGOp>;
template class AxFGRegistry<AxFGOpFactory>;

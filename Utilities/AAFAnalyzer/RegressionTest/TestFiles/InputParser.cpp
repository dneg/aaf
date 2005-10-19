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

//Regression Test Files files
#include "InputParser.h"

//Expat files
#include <expat.h>

//STL files
#include <fstream>
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
using namespace boost;

InputParser::InputParser( const char* outFile )
    : _testFile( outFile )
{}

InputParser::~InputParser()
{}

void InputParser::ParseXML( const char* filename ) const
{
    
    //Variables needed to run the expat XML parser.
    const int BUF_SIZE = 256;
    int done;
    char buffer[BUF_SIZE] = "";
    
    //Create the XML parser
    XML_Parser parser = XML_ParserCreate(NULL);
 
    //Setup event handlers for the XML parser.
    try
    {
        XML_SetElementHandler( parser, InputParser::__StartElement, InputParser::__EndElement);
        XML_SetCharacterDataHandler(parser, InputParser::__EndData);
    }
    catch (...)
    {
        XML_ParserFree(parser);
        throw string("Could not set parser's handler functions.");
    }

    //Use this object as the data used by all handlers.
    XML_SetUserData( parser, (void*)this );

    //Open the XML file for input.
    ifstream inp;
    inp.open( filename );

    //Parse the file using a buffer of the specified size.
    while ( !inp.eof() )
    {
        
        inp.read(buffer, BUF_SIZE);
        const size_t len = inp.gcount();
               
        done = len < sizeof(buffer);

        if ( !XML_Parse(parser, buffer, len, done) )
        {
            ostringstream msg;
            msg << "expat error: " << XML_ErrorString(XML_GetErrorCode(parser));
            throw string( msg.str() );
        }
    }
    
    XML_ParserFree(parser);
    
}

//Called when an open tag is encountered.
void InputParser::StartElement(const AxString& name, const char** attribs)
{
    
    bool isNamed = false;
    AxString mobName = L"";
    
    if ( name != L"eoc" && name != L"oof" && 
        name != L"aaf-file" && name != L"bad-eoc" )
    {
        wostringstream ss;
        AxString atrName;
        
        ss << attribs[0];
        atrName = ss.str().c_str();
        if ( atrName == L"name" )
        {
            isNamed = true;
            ss.str( L"" );
            ss << attribs[1];
            mobName = ss.str().c_str();
        }
    }
    
    shared_ptr<AxMob> spMob;

    if ( name == L"top-level" )
    {
        spMob = _testFile.AddTopLevel( mobName, isNamed );
    }
    else if ( name == L"lower-level" )
    {
        spMob = _testFile.AddLowerLevel( mobName, isNamed );
    }
    else if ( name == L"sub-clip" )
    {
        spMob = _testFile.AddSubClip( mobName, isNamed );
    }
    else if ( name == L"adjusted-clip" )
    {
        spMob = _testFile.AddAdjustedClip( mobName, isNamed );
    }
    else if ( name == L"template-clip" )
    {
        spMob = _testFile.AddTemplateClip( mobName, isNamed );
    }
    else if ( name == L"clip" )
    {
        spMob = _testFile.AddClip( mobName, isNamed );
    }
    else if ( name == L"file-source" )
    {
        spMob = _testFile.AddFileSource( mobName, isNamed );
    }
    else if ( name == L"recording-source" )
    {
        spMob = _testFile.AddRecordingSource( mobName, isNamed );
    }
    else if ( name == L"import-source" )
    {
        spMob = _testFile.AddImportSource( mobName, isNamed );
    }
    else if ( name == L"tape-source" )
    {
        spMob = _testFile.AddTapeSource( mobName, isNamed );
    }
    else if ( name == L"film-source" )
    {
        spMob = _testFile.AddFilmSource( mobName, isNamed );
    }
    else if ( name == L"eoc" )
    {
        _testFile.AttachEOC( *(_mobStack.top()) );
    }
    else if ( name == L"oof" )
    {
        _testFile.AttachOOF( *(_mobStack.top()) );
    }
    else
    {
        //Do nothing
    }
    
    if ( name != L"eoc" && name != L"oof" && 
         name != L"aaf-file" && name != L"bad-eoc" )
    {
        if ( !_mobStack.empty() )
        {
            _testFile.Attach( *(_mobStack.top()), *spMob );
        }
        _mobStack.push( spMob );
    }
    
}

//Called when a close tag is encountered.
void InputParser::EndElement(const AxString& name)
{

    if ( name != L"eoc" && name != L"oof" && 
         name != L"aaf-file" && name != L"bad-eoc" )
    {
        _mobStack.pop();
    }
    
}

//Called after a string of data (between tags) has been loaded.
void InputParser::EndData(const AxString& contents)
{
    //Currently there is not data espected.
}

//These three static functions are the callback functions called by the expat
//XML parser.  In all cases, the userData parameter refers to the object that'
//owns the XML parser.  The functions simply convert the necessary parameters
//to simpler data types and forward them to the necessary member function.
void InputParser::__StartElement(void *userData, const char *name, const char **atts)
{
    InputParser* me = (InputParser*) userData;
    wostringstream msg;
    msg << name;
    
    //Note: The atts parameter is not changed to a shared pointer because
    //      the memory management is done by the XML parser.    
    me->StartElement( msg.str().c_str(), atts );
}

void InputParser::__EndElement(void *userData, const char *name)
{
    InputParser* me = (InputParser*) userData;
    wostringstream msg;
    msg << name;
    me->EndElement( msg.str().c_str() );
}

void InputParser::__EndData(void *userData, const char *s, int len)
{
     if (len != 0)
    {
        InputParser* me = (InputParser*) userData;
        wostringstream msg;
        msg << s;
        me->EndData( msg.str().substr(0, len).c_str() );
    }
}

} // end of namespace diskstream

int main( int argc, char** argv )
{

    using namespace std;

    AxCmdLineArgs args( argc, argv );

    //Input file is the second last argument.
    pair<bool,const char*> inputArg = args.get( argc-2, 1 );
    
    // Requirements Filename is last argument.
    pair<bool, const char*> outputArg = args.get( argc-1, 2 );
        
    //
    //Load all of the requirements in the XML file
    //
    InputParser parser( outputArg.second );
    parser.ParseXML( inputArg.second );

    return 0;
}

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

//Ax files
#include <AxMob.h>

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
{   

    //Pointers to functions to add Mobs to an AAF file.
    _test[L"top-level"] = &TestFileBuilder::AddTopLevel;
    _test[L"lower-level"] = &TestFileBuilder::AddLowerLevel;
    _test[L"sub-clip"] = &TestFileBuilder::AddSubClip;
    _test[L"adjusted-clip"] = &TestFileBuilder::AddAdjustedClip;
    _test[L"template-clip"] = &TestFileBuilder::AddTemplateClip;
    _test[L"clip"] = &TestFileBuilder::AddClip;
    _test[L"file-source"] = &TestFileBuilder::AddFileSource;
    _test[L"recording-source"] = &TestFileBuilder::AddRecordingSource;
    _test[L"import-source"] = &TestFileBuilder::AddImportSource;
    _test[L"tape-source"] = &TestFileBuilder::AddTapeSource;
    _test[L"film-source"] = &TestFileBuilder::AddFilmSource;

}

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
    shared_ptr<AxMob> spMob;
    
    if ( _test.find( name ) != _test.end() )
    {
        wostringstream ss;
        AxString atrName;
        
        //Find the mob name.
        ss << attribs[0];
        atrName = ss.str().c_str();
        if ( atrName == L"name" )
        {
            isNamed = true;
            ss.str( L"" );
            ss << attribs[1];
            mobName = ss.str().c_str();
        }
        
        //Call the appropriate add function (using function pointers is not
        //necessary, but it gets rid of a big if statement).
        spMob = (_testFile.*_test[name])( mobName, isNamed );
        
        //Push the new mob onto the stack.
        if ( !_mobStack.empty() )
        {
            _testFile.Attach( *(_mobStack.top()), *spMob );
        }
        _mobStack.push( spMob );
        
    }
    else if ( name == L"eoc" )
    {
        _testFile.AttachEOC( *(_mobStack.top()) );
    }
    else if ( name == L"oof" )
    {
        _testFile.AttachOOF( *(_mobStack.top()) );
    }
    else if ( name == L"timecode" )
    {
        wostringstream wss;
        AxString atrName;
        bool hasTrackNum = false;
        int trackNum;
        
        wss << attribs[0];
        atrName = wss.str().c_str();
        if ( atrName == L"physical-track-number" )
        {
            hasTrackNum = true;
            stringstream ss;
            ss << attribs[1];
            ss >> trackNum;
        }
        _testFile.AddTimeCode( *(_mobStack.top()), trackNum, hasTrackNum );
    }
    else if ( name == L"edgecode" )
    {
        wostringstream wss;
        AxString atrName;
        bool hasTrackNum = false;
        int trackNum;
        
        wss << attribs[0];
        atrName = wss.str().c_str();
        if ( atrName == L"physical-track-number" )
        {
            hasTrackNum = true;
            stringstream ss;
            ss << attribs[1];
            ss >> trackNum;
        }
        _testFile.AddEdgeCode( *(_mobStack.top()), trackNum, hasTrackNum );
    }
    else
    {
        //Do nothing
    }
       
}

//Called when a close tag is encountered.
void InputParser::EndElement(const AxString& name)
{

    if ( _test.find( name ) != _test.end() )
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
    
    //Requirements Filename is last argument.
    pair<bool, const char*> outputArg = args.get( argc-1, 2 );
        
    //Convert the XML file to an AAF file.
    try
    {
        InputParser parser( outputArg.second );
        parser.ParseXML( inputArg.second );
    }
    catch (AxString ex)
    {
        wcout << ex << endl;
    }

    return 0;
}

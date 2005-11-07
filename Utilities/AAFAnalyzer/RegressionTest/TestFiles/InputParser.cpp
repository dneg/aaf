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
#include "SegmentInfo.h"

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
    
    /*
     *  Note: This class uses maps of function pointer to call the appropriate
     *       functions in the TestFileBuilder class.  The same could be
     *       accomplished using large if statements.  This method was chosen
     *       to reduce the average runtime (map search instead of linear if
     *       statement comparison) and to eliminate the need to maintain multiple
     *       if statements (only maps need to be maintained).
     * 
     * Also: The XML elements eof, oof, timecode, edgecode, aaf-file and bad-eoc
     *       are not contained in any of these maps.  This has been done because
     *       there are not a sufficient number of function (>2) with the same
     *       signature to warrant the map for the first 4 elements.  If more
     *       functions with the same signatures are implemented, maps may easily
     *       be added.  The last two elements are included in the DTD so XML
     *       test files will compile they do not have any effect on the file
     *       structure.
     * 
     */

    //Pointers to functions to add Mobs to an AAF file.
    _materialTypeMap[L"top-level"] = &TestFileBuilder::AddTopLevel;
    _materialTypeMap[L"lower-level"] = &TestFileBuilder::AddLowerLevel;
    _materialTypeMap[L"sub-clip"] = &TestFileBuilder::AddSubClip;
    _materialTypeMap[L"adjusted-clip"] = &TestFileBuilder::AddAdjustedClip;
    _materialTypeMap[L"template-clip"] = &TestFileBuilder::AddTemplateClip;
    _materialTypeMap[L"clip"] = &TestFileBuilder::AddClip;
    _materialTypeMap[L"file-source"] = &TestFileBuilder::AddFileSource;
    _materialTypeMap[L"recording-source"] = &TestFileBuilder::AddRecordingSource;
    _materialTypeMap[L"import-source"] = &TestFileBuilder::AddImportSource;
    _materialTypeMap[L"tape-source"] = &TestFileBuilder::AddTapeSource;
    _materialTypeMap[L"film-source"] = &TestFileBuilder::AddFilmSource;
    
    //Pointers to functions to create empty segments.
    _createSegmentMap[L"source-clip"] = &TestFileBuilder::CreateSourceClip;
    _createSegmentMap[L"operation-group"] = &TestFileBuilder::CreateOperationGroup;
    
    //Pointers to functions to fill segments.
    _fillSegmentMap[L"source-clip"] = &TestFileBuilder::InitializeSourceClip;
    _fillSegmentMap[L"operation-group"] = &TestFileBuilder::AddToOperationGroup;
    
    //Pointers to functions to attach slots.
    _attachSlotMap[L"timeline"] = &TestFileBuilder::AttachTimelineSlot;
    _attachSlotMap[L"static"] = &TestFileBuilder::AttachStaticSlot;
    _attachSlotMap[L"event"] = &TestFileBuilder::AttachEventSlot;
    
    //Mapping from XML Essence Types to Track Types
    _essenceMap[L"none"] = TestFileBuilder::NONE;
    _essenceMap[L"audio"] = TestFileBuilder::AUDIO;
    _essenceMap[L"picture"] = TestFileBuilder::PICTURE;
    
    //Set up optional rational parameter names
    _optRationalParam[L"top-level"] = L"";
    _optRationalParam[L"lower-level"] = L"";
    _optRationalParam[L"sub-clip"] = L"";
    _optRationalParam[L"adjusted-clip"] = L"";
    _optRationalParam[L"template-clip"] = L"";
    _optRationalParam[L"clip"] = L"";
    _optRationalParam[L"file-source"] = L"sample-rate";
    _optRationalParam[L"recording-source"] = L"";
    _optRationalParam[L"import-source"] = L"";
    _optRationalParam[L"tape-source"] = L"";
    _optRationalParam[L"film-source"] = L"";

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
       
    /* 
     * On average elements in the material type map will occur in an XML file
     * more often so search it first.  The next most often used elements will
     * be in the segment map so search it second.  EOC reference will be the
     * third most often elements, followed by timecodes, then edgecodes and
     * finally oof references.
     * 
     */
    if ( _materialTypeMap.find( name ) != _materialTypeMap.end() )
    {
        wostringstream ss;
        AxString atrName;
        
        //Find the mob name.
        OptionalStringAttrib mobName = GetOptionalStringAttribValue( L"name", attribs, 6, L"" );

        OptionalIntAttrib rationalNumerator = GetOptionalIntAttribValue( _optRationalParam[name] + L"-numerator", attribs, 6, 1 );
        OptionalIntAttrib rationalDenominator = GetOptionalIntAttribValue( _optRationalParam[name] + L"-denominator", attribs, 6, 1 );
        aafRational_t optRational;
        optRational.numerator = rationalNumerator.first;
        optRational.denominator = rationalDenominator.first;
        
        //Call the appropriate add function.
        shared_ptr<AxMob> spMob = (_testFile.*_materialTypeMap[name])( mobName.first, mobName.second, optRational );

        if ( !_segmentStack.empty() )
        {
            //Call the appropriate initialize/add function.
            (_testFile.*_fillSegmentMap[_segmentStack.top().element])( _segmentStack.top().segment, *spMob );
        }
        
        //Push the new mob onto the stack.
        _mobStack.push( spMob );
        
    }
    else if ( _createSegmentMap.find( name ) != _createSegmentMap.end() )
    {
        AxString essenceType = GetStringAttribValue( L"track-type", attribs, 12, L"none" );
        AxString slotType = GetStringAttribValue( L"slot-type", attribs, 12, L"timeline" );
        aafRational_t editRate;
        editRate.numerator = GetIntAtribValue(L"edit-rate-numerator", attribs, 12, 1);
        editRate.denominator = GetIntAtribValue(L"edit-rate-denominator", attribs, 12, 1);
        OptionalStringAttrib segName = GetOptionalStringAttribValue( L"name", attribs, 12, L"" );
        OptionalIntAttrib physicalTrackNum = GetOptionalIntAttribValue( L"physical-track-number", attribs, 12, 1 );

        //Create an empty segment and push it onto the segment stack
        shared_ptr<AxSegment> spSegment = (_testFile.*_createSegmentMap[name])( *(_mobStack.top()), _essenceMap[essenceType] );
        _segmentStack.push( SegmentInfo( name, spSegment, essenceType, slotType, editRate, segName, physicalTrackNum ) );
    }
    else if ( name == L"eoc" )
    {
        _testFile.AttachEOC( *(_mobStack.top()) );
    }
    else if ( name == L"timecode" )
    {
        OptionalIntAttrib trackNum = GetOptionalIntAttribValue( L"physical-track-number", attribs, 2, 0 );
        _testFile.AddTimeCode( *(_mobStack.top()), trackNum.first, trackNum.second );
    }
    else if ( name == L"edgecode" )
    {
        OptionalIntAttrib trackNum = GetOptionalIntAttribValue( L"physical-track-number", attribs, 2, 0 );
        _testFile.AddEdgeCode( *(_mobStack.top()), trackNum.first, trackNum.second );
    }
    else if ( name == L"oof" )
    {
        _testFile.AttachOOF( *(_mobStack.top()) );
    }
    else
    {
        //Do nothing
    }
       
}

//Called when a close tag is encountered.
void InputParser::EndElement(const AxString& name)
{

    if ( _materialTypeMap.find( name ) != _materialTypeMap.end() )
    {
        _mobStack.pop();
    }
    else if ( _createSegmentMap.find( name ) != _createSegmentMap.end() )
    {
        //Call the appropriate attach function.
        (_testFile.*_attachSlotMap[_segmentStack.top().slotType])( 
            *(_mobStack.top()), 
            *(_segmentStack.top().segment), 
            _segmentStack.top().editRate, 
            _segmentStack.top().name.first, 
            _segmentStack.top().name.second,
            _segmentStack.top().physicalTrackNum.first, 
            _segmentStack.top().physicalTrackNum.second
           );
        _segmentStack.pop();
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

const AxString InputParser::GetStringAttribValue( const AxString& attrib, const char** attribs, const unsigned int size, const AxString& default_val ) const
{
    
    for ( unsigned int i = 0; i < size; i += 2 )
    {
        wostringstream ss;
        AxString atrName;
        
        ss << attribs[i];
        atrName = ss.str().c_str();
        if ( atrName == attrib )
        {
            ss.str( L"" );
            ss << attribs[i+1];
            return ss.str().c_str();
        }
    }
    
    return default_val;
}

const int InputParser::GetIntAtribValue( const AxString& attrib, const char** attribs, const unsigned int size, const int default_val ) const
{

    for ( unsigned int i = 0; i < size; i += 2 )
    {
        wostringstream wss;
        AxString atrName;
        
        wss << attribs[i];
        atrName = wss.str().c_str();
        if ( atrName == attrib )
        {
            stringstream ss;
            int retVal;
            
            ss << attribs[i+1];
            ss >> retVal;
            return retVal;
        }
    }
 
    return default_val;
}

const InputParser::OptionalStringAttrib InputParser::GetOptionalStringAttribValue( const AxString& attrib, const char** attribs, const unsigned int size, const AxString& default_val ) const
{
    
    for ( unsigned int i = 0; i < size; i += 2 )
    {
        wostringstream ss;
        AxString atrName;
        
        ss << attribs[i];
        atrName = ss.str().c_str();
        if ( atrName == attrib )
        {
            ss.str( L"" );
            ss << attribs[i+1];
            return OptionalStringAttrib( ss.str().c_str(), true );
        }
    }
    
    return OptionalStringAttrib( default_val, false );
}

const InputParser::OptionalIntAttrib InputParser::GetOptionalIntAttribValue( const AxString& attrib, const char** attribs, const unsigned int size, const int default_val ) const
{
    
    for ( unsigned int i = 0; i < size; i += 2 )
    {
        wostringstream wss;
        AxString atrName;
        
        wss << attribs[i];
        atrName = wss.str().c_str();
        if ( atrName == attrib )
        {
            stringstream ss;
            int retVal;
            
            ss << attribs[i+1];
            ss >> retVal;
            return OptionalIntAttrib( retVal, true );
        }
    }
 
    return OptionalIntAttrib( default_val, false );
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

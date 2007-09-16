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

/* On  Windows,  this  should  be  set  if  Expat is going to be linked 
   statically with the code that calls it; this is required to get all the
   right MSVC magic annotations correct. This is ignored on other platforms.
*/
#define XML_STATIC

//Expat files
#include <expat.h>

//Ax files
#include <AxMob.h>
#include <AxComponent.h>
#include <AxInit.h>

//AAF files
#include <AAFOperationDefs.h>
#include <AAFParameterDefs.h>
#include <AAFInterpolatorDefs.h>
#include <AAFOPDefs.h>

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
     */

    /*
     *
     * This generated file has much of the setup code, the following are
     * initialized within the file:
     *
     * 1. _createSegmentMap[OperationGroup] = &TestFileBuilder::CreateOperationGroup;
     * 2. _fillSegmentMapD[OperationGroup] = &TestFileBuilder::AddToOperationGroup;
     * 3. _typeDPropertyMap[TypeDPair( OperationGroup, Segment )] = &TestFileBuilder::INPUT_SEGMENT;
     * 4. _effectMap[Non-OperationGroup Component] = L"";
     * 5. _optRationalParam[MaterialType] = L"";
     * 6. _essenceMap[EssenceType] = TestFileBuilder::Upper(EssenceType);
     *
     * For (5), a default value is provided for all Material Types but can be
     * overridden by specifying a value below the include.
     *
     */
    #include "InputParser.cpp.gen"

    /*
     *
     * The following entries must be made manually as they depend upon the type
     * of object being added.
     *
     */

    //Pointers to functions to add Definitions objects.
	SetupDefinitionMap();

    //Pointers to functions to add Mobs to an AAF file.
	SetupMaterialTypeMap();

    //Pointers to functions to create empty components.
	SetupCreateSegmentMap();

    //Pointers to functions to fill components.
	SetupFillSegmentMap();

    //Pointers to functions to attach slots.
	SetupAttachSlotMap();

    //Pointer to functions to attach parameters.
	SetupAttachParameterMap();

    //Pointer to functions to add annotations (to components).
	SetupAnnotationMap();

    //Prefix used to look up the annotation id in the xml.
	SetupAnnotationIDMap();

    //Override optional rational parameter names
	SetupOptionalRationalParamMap();

    //Set up the effect parameters
	SetupEffectMap();

    //Set up the parameter parameters
	SetupParameterTypeMap();

    //Set up the interpolation map
	SetupInterpolationTypeMap();

    //Set up property constants
    //Any new (Type D Component, object) pair must be added to this list if it is
    //not an (OperationGroup, Segment) pair.

    //Set up the operational pattern map.
	SetupOperationalPatternMap();
}

void InputParser::SetupDefinitionMap()
{
    _definitionMap[L"klv-data-def"]     = &TestFileBuilder::CreateKLVDataDefinition;
    _definitionMap[L"tagged-value-def"] = &TestFileBuilder::CreateTaggedValueDefinition;
    _definitionMap[L"operation-def"]    = &TestFileBuilder::CreateOperationDefinition;
}

void InputParser::SetupMaterialTypeMap()
{
    _materialTypeMap[L"top-level"]           = &TestFileBuilder::AddTopLevel;
    _materialTypeMap[L"lower-level"]         = &TestFileBuilder::AddLowerLevel;
    _materialTypeMap[L"sub-clip"]            = &TestFileBuilder::AddSubClip;
    _materialTypeMap[L"adjusted-clip"]       = &TestFileBuilder::AddAdjustedClip;
    _materialTypeMap[L"template-clip"]       = &TestFileBuilder::AddTemplateClip;
    _materialTypeMap[L"clip"]                = &TestFileBuilder::AddClip;
    _materialTypeMap[L"file-source"]         = &TestFileBuilder::AddFileSource;
    _materialTypeMap[L"mono-audio"]          = &TestFileBuilder::AddMonoAudioFileSource;
    _materialTypeMap[L"multi-channel-audio"] = &TestFileBuilder::AddMultiChannelAudioFileSource;
    _materialTypeMap[L"cdci-source"]         = &TestFileBuilder::AddCDCIFileSource;
    _materialTypeMap[L"rgba-source"]         = &TestFileBuilder::AddRGBAFileSource;
    _materialTypeMap[L"recording-source"]    = &TestFileBuilder::AddRecordingSource;
    _materialTypeMap[L"import-source"]       = &TestFileBuilder::AddImportSource;
    _materialTypeMap[L"tape-source"]         = &TestFileBuilder::AddTapeSource;
    _materialTypeMap[L"film-source"]         = &TestFileBuilder::AddFilmSource;
    _materialTypeMap[L"auxiliary-source"]    = &TestFileBuilder::AddAuxiliarySource;
}

void InputParser::SetupCreateSegmentMap()
{
    _createSegmentMap[L"source-clip"]         = &TestFileBuilder::CreateSourceClip;
    _createSegmentMap[L"timecode"]            = &TestFileBuilder::CreateTimecode;
    _createSegmentMap[L"edgecode"]            = &TestFileBuilder::CreateEdgecode;
    _createSegmentMap[L"timecode-stream-12m"] = &TestFileBuilder::CreateTimecodeStream12M;
    _createSegmentMap[L"eoc"]                 = &TestFileBuilder::CreateEOC;
    _createSegmentMap[L"oof"]                 = &TestFileBuilder::CreateOOF;
    _createSegmentMap[L"transition"]          = &TestFileBuilder::CreateTransition;
    _createSegmentMap[L"sequence"]            = &TestFileBuilder::CreateSequence;
    _createSegmentMap[L"comment-marker"]      = &TestFileBuilder::CreateCommentMarker;
    _createSegmentMap[L"descriptive-marker"]  = &TestFileBuilder::CreateDescriptiveMarker;
    _createSegmentMap[L"operation-group"]     = &TestFileBuilder::CreateOperationGroup;
    //Any new (non-OperationGroup) component must be added to this list.
}

void InputParser::SetupFillSegmentMap()
{
    _fillSegmentMapB[L"source-clip"]        = &TestFileBuilder::InitializeSourceClip;
    _fillSegmentMapC[L"transition"]         = &TestFileBuilder::AddToTransition;
    _fillSegmentMapC[L"sequence"]           = &TestFileBuilder::AddToSequence;
    _fillSegmentMapC[L"comment-marker"]     = &TestFileBuilder::AddToCommentMarker;
    _fillSegmentMapC[L"descriptive-marker"] = &TestFileBuilder::AddToCommentMarker;
    _fillSegmentMapD[L"operation-group"]    = &TestFileBuilder::AddToOperationGroup;
    //Any new Type B, C or D (non-OperationGroup) component must be added to this list.
}

void InputParser::SetupAttachSlotMap()
{
    _attachSlotMap[L"timeline-mob-slot"] = &TestFileBuilder::AttachTimelineSlot;
    _attachSlotMap[L"static-mob-slot"]   = &TestFileBuilder::AttachStaticSlot;
    _attachSlotMap[L"event-mob-slot"]    = &TestFileBuilder::AttachEventSlot;
}

void InputParser::SetupAttachParameterMap()
{
    _attachParameterMap[L"constant-speed-ratio"] = &TestFileBuilder::AttachConstantRationalParameter;
    _attachParameterMap[L"varying-speed-ratio"]  = &TestFileBuilder::AttachVaryingRationalParameter;
}

void InputParser::SetupAnnotationMap()
{
    _annotationMap[L"comment"]  = &TestFileBuilder::AddComment;
    _annotationMap[L"klv-data"] = &TestFileBuilder::AddKLVData;
}

void InputParser::SetupAnnotationIDMap()
{
    _annotationIds[L"comment"]  = L"";
    _annotationIds[L"klv-data"] = L"key-";
}

void InputParser::SetupOptionalRationalParamMap()
{
	_optRationalParam[L"file-source"] = L"sample-rate";
}

void InputParser::SetupEffectMap()
{
    _effectMap[L"video-dissolve"]                    = L"Video Dissolve";
    _effectMap[L"smpte-video-wipe"]                  = L"SMPTE Video Wipe";
    _effectMap[L"video-speed-control"]               = L"Video Speed Control";
    _effectMap[L"video-repeat"]                      = L"Video Repeat";
    _effectMap[L"video-flip"]                        = L"Video Flip";
    _effectMap[L"video-flop"]                        = L"Video Flop";
    _effectMap[L"video-flip-flop"]                   = L"Video Flip Flop";
    _effectMap[L"video-position"]                    = L"Video Position";
    _effectMap[L"video-crop"]                        = L"Video Crop";
    _effectMap[L"video-scale"]                       = L"Video Scale";
    _effectMap[L"video-rotate"]                      = L"Video Rotate";
    _effectMap[L"video-corner-pinning"]              = L"Video Corner Pinning";
    _effectMap[L"alpha-with-video-key"]              = L"Alpha With Video Key effect";
    _effectMap[L"separate-alpha-key"]                = L"Separate Alpha Key effect";
    _effectMap[L"luminance-key"]                     = L"Luminance Key";
    _effectMap[L"chroma-key"]                        = L"Chroma Key";
    _effectMap[L"mono-audio-gain"]                   = L"Mono Audio Gain";
    _effectMap[L"mono-audio-pan"]                    = L"Mono Audio Pan";
    _effectMap[L"mono-audio-dissolve"]               = L"Mono Audio Dissolve";
    _effectMap[L"two-parameter-mono-audio-dissolve"] = L"Two-Parameter Mono Audio Dissolve";
    //Any new effects must be added to this list.
}

void InputParser::SetupParameterTypeMap()
{
    _parameterTypeMap[L"constant-speed-ratio"] = kAAFParameterDef_SpeedRatio;
    _parameterTypeMap[L"varying-speed-ratio"]  = kAAFParameterDef_SpeedRatio;
}

void InputParser::SetupInterpolationTypeMap()
{
    _interpolationTypeMap[L"b-spline"] = kAAFInterpolationDef_BSpline;
    _interpolationTypeMap[L"constant"] = kAAFInterpolationDef_Constant;
    _interpolationTypeMap[L"linear"]   = kAAFInterpolationDef_Linear;
    _interpolationTypeMap[L"log"]      = kAAFInterpolationDef_Log;
    _interpolationTypeMap[L"none"]     = kAAFInterpolationDef_None;
    _interpolationTypeMap[L"power"]    = kAAFInterpolationDef_Power;
}

void InputParser::SetupOperationalPatternMap()
{
    _operationalPatternMap[L"edit-protocol"] = kAAFOPDef_EditProtocol;
    _operationalPatternMap[L"invalid"]       = kAAFOperationDef_Unknown;      //Any invalid AUID will do.
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

        if ( !XML_Parse(parser, buffer, static_cast<int>(len), done) )
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
        SlotInfo::OptionalStringAttrib mobName = GetOptionalStringAttribValue( L"name", attribs, 8, L"" );
        SlotInfo::OptionalIntAttrib rationalNumerator = GetOptionalIntAttribValue( _optRationalParam[name] + L"-numerator", attribs, 8, 1 );
        SlotInfo::OptionalIntAttrib rationalDenominator = GetOptionalIntAttribValue( _optRationalParam[name] + L"-denominator", attribs, 8, 1 );
        SlotInfo::OptionalStringAttrib alphaTransparency = GetOptionalStringAttribValue( L"alpha-transparency", attribs, 8, L"max" );
        SlotInfo::OptionalStringAttrib sourceId = GetOptionalStringAttribValue( L"parent-src-id", attribs, 8, L"" );
        aafRational_t optRational;
        optRational.numerator = rationalNumerator.first;
        optRational.denominator = rationalDenominator.first;

		
        //Note: This logic will have to change a bit if another type of mob
        //requires an optional aafUInt32 parameter.
        aafAlphaTransparency_t transType;
        if ( alphaTransparency.first == L"max" )
        {
            transType = kAAFMaxValueTransparent;
        }
        else
        {
            transType = kAAFMinValueTransparent;
        }
		//wcout<<"SUp3 "<<"ID: "<<sourceId.first<<" mobname: "<<mobName.first<<" ISNAMED: "<<mobName.second<<endl;
        //Call the appropriate add function.
        shared_ptr<AxMob> spMob = (_testFile.*_materialTypeMap[name])( mobName.first, mobName.second, optRational, transType, alphaTransparency.second );
        if ( !_componentStack.empty() )
        {
            //Call the appropriate initialize/add function.
            shared_ptr<AxSourceReference> axSrcRef = dynamic_pointer_cast<AxSourceReference>( _componentStack.top().second );
            (_testFile.*_fillSegmentMapB[_componentStack.top().first])( axSrcRef, *spMob, sourceId.first );
        }
		
        //Push the new mob onto the stack.
        _mobStack.push( spMob );
		
    }
    else if ( _createSegmentMap.find( name ) != _createSegmentMap.end() )
    {
        AxString essenceType = GetStringAttribValue( L"track-type", attribs, 10, L"none" );
        AxString effectType = GetStringAttribValue( L"operation-definition", attribs, 10, name );
        SlotInfo::OptionalIntAttrib length = GetOptionalIntAttribValue( L"length", attribs, 10, 0 );
        SlotInfo::OptionalIntAttrib startTime = GetOptionalIntAttribValue( L"start-time", attribs, 10, 0 );
        SlotInfo::OptionalIntAttrib position = GetOptionalIntAttribValue( L"position", attribs, 10, 0 );

        //The default value of both startTime and position is 0.  Therefore we
        //will either get 0 (if neither is present) or the value of the
        //specified property (since start-time and position do not occur on the
        //same segments).
        int intParam = startTime.first + position.first;
        bool hasIntParam = startTime.second || position.second;

        //Create an empty segment and push it onto the segment stack
        shared_ptr<AxComponent> spComponent = (_testFile.*_createSegmentMap[name])( _essenceMap[essenceType], _effectMap[effectType], length.first, length.second, intParam, hasIntParam );

        if ( !_slotStack.empty() )
        {
            if ( _slotStack.top().componentsSinceSlot != 0 )
            {
                if ( _fillSegmentMapC.find( _componentStack.top().first ) != _fillSegmentMapC.end() )
                {	
                    (_testFile.*_fillSegmentMapC[_componentStack.top().first])( _componentStack.top().second, *spComponent );
                }
                else if ( _fillSegmentMapD.find( _componentStack.top().first ) != _fillSegmentMapD.end() )
                {
                    shared_ptr<AxSegment> axParentSegment = dynamic_pointer_cast<AxSegment>( _componentStack.top().second );
                    shared_ptr<AxSegment> axChildSegment = dynamic_pointer_cast<AxSegment>( spComponent );
                    (_testFile.*_fillSegmentMapD[_componentStack.top().first])( axParentSegment, *axChildSegment, _typeDPropertyMap[TypeDPair( _componentStack.top().first, name )] );
                }
            }
            _slotStack.top().componentsSinceSlot++;
        }
        _componentStack.push( ComponentPair( name, spComponent ) );

    }
    else if ( _attachSlotMap.find( name ) != _attachSlotMap.end() )
    {
        aafRational_t editRate;
        editRate.numerator = GetIntAtribValue(L"edit-rate-numerator", attribs, 14, 1);
        editRate.denominator = GetIntAtribValue(L"edit-rate-denominator", attribs, 14, 1);
        SlotInfo::OptionalStringAttrib segName = GetOptionalStringAttribValue( L"name", attribs, 14, L"" );
        SlotInfo::OptionalIntAttrib physicalTrackNum = GetOptionalIntAttribValue( L"physical-track-number", attribs, 14, 1 );
        SlotInfo::OptionalIntAttrib markedInPoint = GetOptionalIntAttribValue( L"marked-in-point", attribs, 14, 0 );
        SlotInfo::OptionalIntAttrib markedOutPoint = GetOptionalIntAttribValue( L"marked-out-point", attribs, 14, 0 );
	SlotInfo::OptionalIntAttrib userPos = GetOptionalIntAttribValue( L"user-pos-point", attribs, 14, 0 );
        int origin = GetIntAtribValue( L"origin", attribs, 14, 0 );
        _slotStack.push( SlotInfo( name, editRate, segName, physicalTrackNum, markedInPoint, markedOutPoint, userPos, origin ) );
    }
    else if ( _attachParameterMap.find( name ) != _attachParameterMap.end() )
    {
        //Only one of these values should be defined, at most one non-zero will
        //be returned.  Sum the three to get the actual value of the parameter.
        aafUInt32 numerator = GetIntAtribValue( L"numerator", attribs, 6, 0 );
        aafUInt32 boolParam = GetBoolAtribValue( L"bool-value", attribs, 6, false );
        aafUInt32 intParam = GetIntAtribValue( L"int-value", attribs, 6, 0 );
        aafUInt32 param1 = numerator + boolParam + intParam;

        aafUInt32 param2 = GetIntAtribValue( L"denominator", attribs, 6, 0 );
        AxString interpolator = GetStringAttribValue( L"interpolator", attribs, 6, L"none" );

        shared_ptr<AxOperationGroup> axOpGroup = dynamic_pointer_cast<AxOperationGroup>( _componentStack.top().second );
        (_testFile.*_attachParameterMap[name])( *axOpGroup, _parameterTypeMap[name], param1, param2, _interpolationTypeMap[interpolator] );

    }
    else if ( _definitionMap.find( name ) != _definitionMap.end() )
    {
        AxString defName = GetStringAttribValue( L"name", attribs, 4, L"" );
        AxString desc = GetStringAttribValue( L"description", attribs, 4, L"" );
        (_testFile.*_definitionMap[name])( defName, desc );
    }
    else if ( _annotationMap.find( name) != _annotationMap.end() )
    {
        AxString annotationName = GetStringAttribValue( _annotationIds[name] + L"name", attribs, 4, L"" );
        AxString value = GetStringAttribValue( L"value", attribs, 4, L"" );
        (_testFile.*_annotationMap[name])( _componentStack.top().second, annotationName, value );
    }
    else if ( name == L"descriptive-framework")
    {
        shared_ptr<AxDescriptiveMarker> axMarker = dynamic_pointer_cast<AxDescriptiveMarker>( _componentStack.top().second );
        _testFile.AttachDescriptiveFramework( axMarker );
    }
    else if ( name == L"aaf-file" )
    {
        AxString operationalPattern = GetStringAttribValue( L"operational-pattern", attribs, 4, L"none" );
        AxString effectDataDefs = GetStringAttribValue( L"effect-data-definitions", attribs, 4, L"current" );
        if ( operationalPattern != L"none" )
        {
            _testFile.SetOperationalPattern( _operationalPatternMap[operationalPattern] );
        }
        if ( effectDataDefs != L"current" )
        {
            _testFile.UseLegacyEffectDefinitions();
        }
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
    else if ( _attachSlotMap.find( name ) != _attachSlotMap.end() )
    {
        //Call the appropriate attach function.
        shared_ptr<AxSegment> axSegment = dynamic_pointer_cast<AxSegment>( _componentStack.top().second );
        (_testFile.*_attachSlotMap[name])(
            *(_mobStack.top()),
            *axSegment,
            _slotStack.top().editRate,
            _slotStack.top().name.first,
            _slotStack.top().name.second,
            _slotStack.top().physicalTrackNum.first,
            _slotStack.top().physicalTrackNum.second,
            static_cast<int>(_slotStack.top().markedInPoint.first),
            static_cast<int>(_slotStack.top().markedInPoint.second),
            static_cast<int>(_slotStack.top().markedOutPoint.first),
            static_cast<int>(_slotStack.top().markedOutPoint.second),
            static_cast<int>(_slotStack.top().userPos.first),
            static_cast<int>(_slotStack.top().userPos.second),
            static_cast<int>(_slotStack.top().origin)
           );
        _componentStack.pop();
        _slotStack.pop();
    }
    else if ( _createSegmentMap.find( name ) != _createSegmentMap.end() )
    {
        if ( !_slotStack.empty() )
        {
            _slotStack.top().componentsSinceSlot--;
            if ( _slotStack.top().componentsSinceSlot != 0 )
            {
                _componentStack.pop();
            }
        }
        else
        {
            _componentStack.pop();
        }
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

		if ( attribs[i] != NULL )
		{
            ss << attribs[i];
            atrName = ss.str().c_str();
            if ( atrName == attrib )
            {
                ss.str( L"" );
                ss << attribs[i+1];
                return ss.str().c_str();
            }
            else if ( atrName.length() == 0 )
            {
                break;
            }
		}
		else
		{
		    break;
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

        if ( attribs[i] != NULL )
        {
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
            else if ( atrName.length() == 0 )
            {
                break;
            }
        }
        else
        {
            break;
        }
    }

    return default_val;
}

const int InputParser::GetBoolAtribValue( const AxString& attrib, const char** attribs, const unsigned int size, const int default_val ) const
{

    for ( unsigned int i = 0; i < size; i += 2 )
    {
        wostringstream wss;
        AxString atrName;

        if ( attribs[i] != NULL )
        {
            wss << attribs[i];
            atrName = wss.str().c_str();
            if ( atrName == attrib )
            {
                wstringstream ss;

                ss << attribs[i+1];
                AxString boolStr =  ss.str().c_str();
                if ( boolStr == L"true" )
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else if ( atrName.length() == 0 )
            {
                break;
            }
        }
        else
        {
            break;
        }
    }

    return default_val;
}

const SlotInfo::OptionalStringAttrib InputParser::GetOptionalStringAttribValue( const AxString& attrib, const char** attribs, const unsigned int size, const AxString& default_val ) const
{

    for ( unsigned int i = 0; i < size; i += 2 )
    {
        wostringstream ss;
        AxString atrName;

        if ( attribs[i] != NULL )
        {
            ss << attribs[i];
            atrName = ss.str().c_str();
            if ( atrName == attrib )
            {
                ss.str( L"" );
                ss << attribs[i+1];
                return SlotInfo::OptionalStringAttrib( ss.str().c_str(), true );
            }
            else if ( atrName.length() == 0 )
            {
                break;
            }
        }
        else
        {
            break;
        }
    }

    return SlotInfo::OptionalStringAttrib( default_val, false );
}

const SlotInfo::OptionalIntAttrib InputParser::GetOptionalIntAttribValue( const AxString& attrib, const char** attribs, const unsigned int size, const int default_val ) const
{

    for ( unsigned int i = 0; i < size; i += 2 )
    {
        wostringstream wss;
        AxString atrName;

        if ( attribs[i] != NULL )
        {
            wss << attribs[i];
            atrName = wss.str().c_str();
            if ( atrName == attrib )
            {
                stringstream ss;
                int retVal;

                ss << attribs[i+1];
                ss >> retVal;
                return SlotInfo::OptionalIntAttrib( retVal, true );
            }
            else if ( atrName.length() == 0 )
            {
                break;
            }
        }
        else
        {
            break;
        }
    }

    return SlotInfo::OptionalIntAttrib( default_val, false );
}

} // end of namespace diskstream

int main( int argc, char** argv )
{

    using namespace std;

    AxInit initObj;
    AxCmdLineArgs args( argc, argv );

    //Input file is the second last argument.
    pair<bool,const char*> inputArg = args.get( argc-2, 1 );
    assert( inputArg.first );

    //Requirements Filename is last argument.
    pair<bool, const char*> outputArg = args.get( argc-1, 2 );
    assert( outputArg.first );

    //Convert the XML file to an AAF file.
    try
    {
        unlink( outputArg.second );
        InputParser parser( outputArg.second );
        parser.ParseXML( inputArg.second );
    }
    catch (AxString ex)
    {
        wcout << ex << endl;
    }

    return 0;
}

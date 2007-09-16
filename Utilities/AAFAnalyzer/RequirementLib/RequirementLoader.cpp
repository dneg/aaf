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

//Requirement files
#include "RequirementLoader.h"
#include "RequirementRegistry.h"
#include "RequirementXMLException.h"

/* On  Windows,  this  should  be  set  if  Expat is going to be linked
   statically with the code that calls it; this is required to get all the
   right MSVC magic annotations correct. This is ignored on other platforms.
*/
#define XML_STATIC

//Expat files
#include <expat.h>

//Boost files
#include <boost/shared_ptr.hpp>

//STL files
#include <fstream>
#include <sstream>
#include <iostream>

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

RequirementLoader::RequirementLoader()
  : _current(),
    _categoryMap()
{
    _categoryMap[L"general"]             = Requirement::GENERAL;
    _categoryMap[L"import-export"]       = Requirement::IMPORT_EXPORT;
    _categoryMap[L"compositional"]       = Requirement::COMPOSITIONAL;
    _categoryMap[L"metadata"]            = Requirement::METADATA;
    _categoryMap[L"mixdown"]             = Requirement::MIXDOWN;
    _categoryMap[L"auxiliary-file"]      = Requirement::AUXILIARY_FILE;
    _categoryMap[L"annotations"]         = Requirement::ANNOTATIONS;
    _categoryMap[L"effect"]              = Requirement::EFFECT;
    _categoryMap[L"optional-properties"] = Requirement::OPTIONAL_PROPERTIES;
    _categoryMap[L"structured-storage"]  = Requirement::STRUCTURED_STORAGE;
    _categoryMap[L"protocol"]            = Requirement::PROTOCOL;
    _categoryMap[L"ad-hoc"]              = Requirement::ADHOC;
}

RequirementLoader::~RequirementLoader()
{}

void RequirementLoader::ParseXML( const char* filename ) const
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
        XML_SetElementHandler( parser, RequirementLoader::__StartElement, RequirementLoader::__EndElement);
        XML_SetCharacterDataHandler(parser, RequirementLoader::__EndData);
    }
    catch (...)
    {
        XML_ParserFree(parser);
        throw RequirementXMLException( L"Could not set parser's handler functions." );
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
            wostringstream msg;
            msg << L"expat error: " << XML_ErrorString(XML_GetErrorCode(parser));
            throw RequirementXMLException(msg.str().c_str() );
        }
    }

    XML_ParserFree(parser);

}

//Called when an open tag is encountered.
void RequirementLoader::StartElement(const wstring& name, const char** attribs)
{

    if ( name == L"requirement" )
    {
        // Reset _current by assigning a new, empty, Current object.
        _current = RequirementLoader::Current();

	//
	// type attribute
	//

	int i = 0;
	wstring type;
	if ( attribs[i] && "type" == string(attribs[i] ) )
	{
	  i++;
	  assert( attribs[i] );
	  wostringstream msg;
	  msg << attribs[i];
	  type = msg.str();
	}

        if ( type == L"app" )
        {
            _current.Type = Requirement::APP;
        }
        else if ( type == L"file" )
        {
            _current.Type = Requirement::FILE;
        }
        else if ( type == L"def" )
        {
            _current.Type = Requirement::DEFINITION;
        }
        else
        {
            wostringstream msg;
            msg << L"Unknown requirement type: " << type;
            throw RequirementXMLException(msg.str().c_str() );
        }
        _current.TypeAsString = type;


	//
	// category attribute
	//

	i++;
	wstring category;
	if ( attribs[i] && "category" == string(attribs[i]) )
	{
	  i++;
	  assert( attribs[i] );
	  wostringstream msg;
	  msg << attribs[i];
	  category = msg.str().c_str();
	}

        if ( _categoryMap.find( category ) != _categoryMap.end() )
        {
            _current.Category = _categoryMap[category];
            _current.CategoryAsString = category;
        }
        else
        {
            wostringstream msg;
            msg << L"Unknown category: " << category;
            throw RequirementXMLException(msg.str().c_str() );
        }

	// action attribute
	i++;
	if ( attribs[i] && "action" == string(attribs[i]) )
	{
	  i++;
	  assert( attribs[i] );
	  wostringstream msg;
	  msg << attribs[i];
	  _current.Action = msg.str();
	}

    }
    else if ( name == L"id"                         ||
              name == L"name"                       ||
              name == L"desc"                       ||
              name == L"annotate"                   ||
              name == L"note"                       ||
              name == L"doc"                        ||
              name == L"version"                    ||
              name == L"section"                    ||
              name == L"ref"                        ||
              name == L"requirement-set"            ||
              name == L"requirement-set-name"       ||
              name == L"requirement-set-version" )
    {
      _current.Data = wstring();
    }
    else
    {
        wostringstream msg;
        msg << L"Unknown tag: " << name;
        throw RequirementXMLException(msg.str().c_str() );
    }
}

//Called when a close tag is encountered.
void RequirementLoader::EndElement(const wstring& name)
{
    if ( name == L"requirement" )
    {
        shared_ptr<const Requirement> req(new Requirement(
            _current.Id,
	    _current.Type, _current.TypeAsString,
            _current.Category, _current.CategoryAsString,
	    _current.Action,
            _current.Name,
            _current.Desc,
	    _current.Annotate,
	    _current.Note,
	    _current.Document, _current.Version, _current.Section ));
        RequirementRegistry::GetInstance().Register( req );
    }
    else if ( name == L"id" )
    {
        _current.Id = _current.Data;
    }
    else if ( name == L"name" )
    {
        _current.Name = _current.Data;
    }
    else if ( name == L"desc" )
    {
        _current.Desc = _current.Data;
    }
    else if ( name == L"annotate" )
    {
        _current.Annotate = _current.Data;
    }
    else if ( name == L"note" )
    {
        _current.Note = _current.Data;
    }
    else if ( name == L"doc" )
    {
        _current.Document = _current.Data;
    }
    else if ( name == L"version" )
    {
        _current.Version = _current.Data;
    }
    else if ( name == L"section" )
    {
        _current.Section = _current.Data;
    }
    else if ( name == L"ref" )
    {
        //No defined behaviour
    }
    else if ( name == L"requirement-set" )
    {
        //No defined behaviour
    }
    else if ( name == L"requirement-set-name" )
    {
        //No defined behaviour
    }
    else if ( name == L"requirement-set-version" )
    {
        //No defined behaviour
    }
    else
    {
        wostringstream msg;
        msg << L"Unknown tag: /" << name;
        throw RequirementXMLException(msg.str().c_str() );
    }

}

//Called after a string of data (between tags) has been loaded.
void RequirementLoader::EndData(const wstring& contents)
{
    _current.Data += contents;
}

//These three static functions are the callback functions called by the expat
//XML parser.  In all cases, the userData parameter refers to the object that'
//owns the XML parser.  The functions simply convert the necessary parameters
//to simpler data types and forward them to the necessary member function.
void RequirementLoader::__StartElement(void *userData, const char *name, const char **atts)
{
    RequirementLoader* me = (RequirementLoader*) userData;
    wostringstream msg;
    msg << name;

    //Note: The atts parameter is not changed to a shared pointer because
    //      the memory management is done by the XML parser.
    me->StartElement( msg.str().c_str(), atts );
}

void RequirementLoader::__EndElement(void *userData, const char *name)
{
    RequirementLoader* me = (RequirementLoader*) userData;
    wostringstream msg;
    msg << name;
    me->EndElement( msg.str().c_str() );
}

void RequirementLoader::__EndData(void *userData, const char *s, int len)
{
     if (len != 0)
    {
        RequirementLoader* me = (RequirementLoader*) userData;
        wostringstream msg;
        msg << s;
        me->EndData( msg.str().substr(0, len).c_str() );
    }
}

} // end of namespace diskstream

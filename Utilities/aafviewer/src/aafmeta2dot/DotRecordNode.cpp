/*
 * $Id$ $Name$
 *
 *      Copyright (c) 2003, Philip de Nier (philipn@users.sourceforge.net)
 *
 *
 *      This file is part of aaf2dot.
 *
 *  aaf2dot is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  aaf2dot is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with aaf2dot; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#pragma warning (disable:4786)

#include <iostream>
#include <sstream>
#include <assert.h>
#include <fstream>
using namespace std;

#include <DotRecordNode.h>
#include <DotGraph.h>
#include <DotFactory.h>
#include <DotUtilities.h>
#include <DotLogging.h>


char* nullMobID = "00000000000000000000000000000000-00000000-0000-0000-0000000000000000";
char* nullUID = "00000000-0000-0000-0000000000000000";


//-----------------------------------------------------------------------------
void 
DotRecordNodeAttribute::Write( ofstream &dotFile, DotProfile &profile )
{
   string ltdAttributeString = LimitAttributeSize( GetAttributeString(),
						   profile.GetMaxAttributeLength(), profile.GetMaxAttributeWidth() );

   dotFile << ltdAttributeString << "\\n";
}


//-----------------------------------------------------------------------------
int
DotRecordNodeAttribute::GetWidth( DotProfile &profile )
{
   string ltdAttributeString = LimitAttributeSize( GetAttributeString(),
						   profile.GetMaxAttributeLength(), profile.GetMaxAttributeWidth() );


   int maxWidth = 0;
   int pos = 0;
   bool done = false;
   while ( !done )
   {
      int newPos = ltdAttributeString.find( "\\n", pos );
      if ( newPos == -1 )
      {
	 done = true;
	 if ( ( ltdAttributeString.size() - pos ) > maxWidth )
	 {
	    maxWidth = ltdAttributeString.size() - pos;
	 }
      }
      else
      {
	 if ( ( newPos - pos ) > maxWidth )
	 {
	    maxWidth = newPos - pos;
	 }
	 pos = newPos + 1;
      }
   }
   return maxWidth;
}


//-----------------------------------------------------------------------------
string
DotRecordNodeAttribute::GetAttributeString()
{
   string attributeString = ProcessRecordString(_name);
   attributeString.append(" = ");
   attributeString.append(ProcessRecordString(_value));

   return attributeString;
}


//-----------------------------------------------------------------------------
void 
DotRecordNodeClassAttribute::Write( ofstream &dotFile, DotProfile &profile )
{
   string ltdAttributeString = LimitAttributeSize( GetAttributeString(),
						   profile.GetMaxAttributeLength(), profile.GetMaxAttributeWidth() );

   dotFile << ltdAttributeString << "\\n";
}


//-----------------------------------------------------------------------------
int
DotRecordNodeClassAttribute::GetWidth( DotProfile &profile )
{
   string ltdAttributeString = LimitAttributeSize( GetAttributeString(),
						   profile.GetMaxAttributeLength(), profile.GetMaxAttributeWidth() );


   int maxWidth = 0;
   int pos = 0;
   bool done = false;
   while ( !done )
   {
      int newPos = ltdAttributeString.find( "\\n", pos );
      if ( newPos == -1 )
      {
	 done = true;
	 if ( ( ltdAttributeString.size() - pos ) > maxWidth )
	 {
	    maxWidth = ltdAttributeString.size() - pos;
	 }
      }
      else
      {
	 if ( ( newPos - pos ) > maxWidth )
	 {
	    maxWidth = newPos - pos;
	 }
	 pos = newPos + 1;
      }
   }
   return maxWidth;
}


//-----------------------------------------------------------------------------
string
DotRecordNodeClassAttribute::GetAttributeString()
{
   string attributeString = ProcessRecordString(_name);
   attributeString.append(": ");
   attributeString.append(ProcessRecordString(_typeName));
   
   return attributeString;
}


//-----------------------------------------------------------------------------
void 
DotRecordNode::Write( ofstream &dotFile, DotProfile &profile )
{
   // set the width attribute
   int maxWidth = _name.size();
   DotRecordNodeAttributeVector::iterator iter;
   for ( iter=_attributes.begin(); iter!=_attributes.end(); iter++ )
   {
      int width = (*iter).GetWidth( profile );
      if ( width > maxWidth )
      {
	 maxWidth = width;
      }
   }
   DotRecordNodeClassAttributeVector::iterator iterCA;
   for ( iterCA=_classAttributes.begin(); iterCA!=_classAttributes.end(); iterCA++ )
   {
      int width = (*iterCA).GetWidth( profile );
      if ( width > maxWidth )
      {
	 maxWidth = width;
      }
   }

   int fontSize = defaultNodeFontSize;
   string fontSizeString = GetElementAttribute( "fontsize" );
   if ( fontSizeString.size() != 0 )
   {
      fontSize = atoi( fontSizeString.c_str() );
   }

   float fontSizeFactor = 0.013 * fontSize;
   float nodeWidth = maxWidth * fontSizeFactor;

   ostringstream widthString;
   widthString << nodeWidth;

   SetElementAttribute( "width", widthString.str() );


   // now start writing
   dotFile << _uid << " [ ";

   DotElement::WriteElementAttributes( dotFile, profile );
   if ( GetNumElementAttributes() > 0 )
   {
      dotFile << ", ";
   }

   dotFile << " label = \"{" << _name << "\\n. ";
   for ( iter=_attributes.begin(); iter!=_attributes.end(); iter++ )
   {
      dotFile << " | ";
      (*iter).Write( dotFile, profile );
   }
   for ( iterCA=_classAttributes.begin(); iterCA!=_classAttributes.end(); iterCA++ )
   {
      dotFile << " | ";
      (*iterCA).Write( dotFile, profile );
   }
   dotFile << "}\"";
	
   dotFile << "];" << endl;
}


//-----------------------------------------------------------------------------
DotRecordNode::DotRecordNode( string name, string uid )
   :	DotElement( name, uid )
{
}


//-----------------------------------------------------------------------------
void 
DotRecordNode::AddAttribute( DotRecordNodeAttribute attribute )
{
   _attributes.push_back( attribute );
}

//-----------------------------------------------------------------------------
void 
DotRecordNode::AddClassAttribute( DotRecordNodeClassAttribute classAttribute )
{
   _classAttributes.push_back( classAttribute );
}



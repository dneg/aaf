/*
 *      Copyright (c) 2003, Philip de Nier (philipn@users.sourceforge.net)
 *
 *
 *      This file is part of aafmeta2dot.
 *
 *  aafmeta2dot is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation; either version 2.1 of the License, or
 *  (at your option) any later version.
 *
 *  aafmeta2dot is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with aafmeta2dot; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifdef WIN32
#pragma warning (disable:4786)
#endif

#include <iostream>
using namespace std;

#include <AAFDotHome.h>
#include <AAFClassDefinition.h>
#include <AAFTypeDefinition.h>

#include <DotFactory.h>
#include <DotLogging.h>
#include <Logging.h>
#include <DotProfile.h>
#include <Utilities.h>


//-----------------------------------------------------------------------------
AAFDotHome::AAFDotHome( string aafFilename, string dotFilename, AAFMetaMapProfile *profile )
   : _aafFilename( aafFilename ), _dotFilename( dotFilename ), _profile( profile ),
     AAFDefinitionHome()
{
}


//-----------------------------------------------------------------------------
AAFDotHome::~AAFDotHome()
{
}


//-----------------------------------------------------------------------------
void
AAFDotHome::Write()
{
   DotProfile dotProfile;
   if ( _profile->GetDebug() )
   {
      DotLogging::SetDebugLogStream( &cout );
      dotProfile.SetDebug( true );
   }
   else 
   {
      DotLogging::SetNullDebugLogStream();
      dotProfile.SetDebug( false );
   }

   

   DotFactory dotFactory;

   // create the graph
   DotGraph *graph = dotFactory.CreateGraph( "AAFMetaDictGraph", dotFactory.CreateGraphUID() );


   DefinitionVector::iterator iter;
   for ( iter=_definitions.begin(); iter!=_definitions.end(); iter++ )
   {
      (*iter)->InitForBuild();
   } 

   for ( iter=_definitions.begin(); iter!=_definitions.end(); iter++ )
   {
      if ( !_profile->GetTypeDefs() )
      {
	 // build class definitions
	 if ( dynamic_cast< AAFClassDefinition* > (*iter) != 0 )
	 {
	    (*iter)->BuildDot( this, &dotFactory, graph );
	 }
      }
      else
      {
	 // build type definitions
	 if ( dynamic_cast< AAFTypeDefinition* > (*iter) != 0 )
	 {
	    (*iter)->BuildDot( this, &dotFactory, graph );
	 }
      }
   } 


   // resolve references in associations
   dotFactory.ResolveReferences();
  
   // write
   Logging::DebugLogStream() << "Writing dot file..." << endl;
   ofstream dotFile( _dotFilename.c_str() );
   graph->Write( dotFile, dotProfile );
   dotFile.close();
   Logging::DebugLogStream() << "Done." << endl;
}


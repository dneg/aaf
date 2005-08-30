/*
 * $Id$ $Name$
 *
 *      Copyright (c) 2003, Philip de Nier (philipn@users.sourceforge.net)
 *
 *
 *      This file is part of aafmeta2dot.
 *
 *  aafmeta2dot is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  aafmeta2dot is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with aafmeta2dot; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifdef _MSC_VER
#pragma warning (disable:4786)
#endif

#include <AxFile.h>
#include <AxTypes.h>
#include <AxInit.h>
#include <AxObject.h>
#include <AxMetaDef.h>
#include <AxHeader.h>
#include <AxDictionary.h>
#include <AxIterator.h>
#include <AxProperty.h>
#include <AxUtil.h>


#include <iostream>
#include <sstream>
using namespace std;


#include <AAFXsMapper.h>
#include <AAFDotHome.h>
#include <Utilities.h>
#include <Logging.h>



//-----------------------------------------------------------------------------
void throwUsage()
{
   wostringstream msg;
   msg << L"Usage: " << L" -aafin <aaf filename> -dotout <dot filename>" << endl << endl
       << L"OPTIONS:" << endl
       << L"-refasproperty            Display references as properties" << endl
       << L"-typedefs                 Show the type definitions - class definition properties are not shown" << endl
       << L"-strongrefh                Show the strong reference hierarchy - overriden by -refasproperty and -typedefs" << endl
       << L"-debug                    Output debug information" << endl;
   throw AxEx( msg.str().c_str() );
}


//-----------------------------------------------------------------------------
void Export( AxString aafFilename, string dotFilename, AAFMetaMapProfile *profile )
{
   if ( profile->GetDebug() ) 
   {
      Logging::SetDebugLogStream( &cout );
   }
   else 
   {
      Logging::SetNullDebugLogStream();
   }

   // Open the file.
   AxFile axFile;
   axFile.OpenExistingRead( aafFilename );
	
   // use the header to get to the dictionary it owns
   AxHeader axHeader( axFile.getHeader() );
   AxDictionary axDictionary( axHeader.GetDictionary() );

   AAFDotHome dotHome( AxStringToString( aafFilename ), dotFilename, profile );
   AAFXsMapper mapper( &dotHome );


   // map the type definitions
   Logging::DebugLogStream() << "Mapping the type definitions..." << endl;
   AxTypeDefIter typeDefIter( axDictionary.GetTypeDefs() );
   IAAFSmartPointer2<IAAFTypeDef> spIaafTypeDef;
   while ( typeDefIter.NextOne( spIaafTypeDef ) )
   {
      AxTypeDef axTypeDef( spIaafTypeDef );
      mapper.MapAAFTypeDef( axTypeDef );
   }

   // map the class definitions
   Logging::DebugLogStream() << "Mapping the class definitions..." << endl;
   AxClassDefIter classDefIter( axDictionary.GetClassDefs() );
   IAAFSmartPointer2<IAAFClassDef> spIaafClassDef;
   while ( classDefIter.NextOne(spIaafClassDef) )
   {
      AxClassDef axClassDef( spIaafClassDef );
      mapper.MapAAFClassDef( axClassDef );
   }


   // write
   Logging::DebugLogStream() << "Writing the dot file..." << endl;
   dotHome.Write();
   Logging::DebugLogStream() << "Done." << endl;
	
		
   axFile.Close();
}


//-----------------------------------------------------------------------------
int main(int argc, char** argv)
{

   // One time init stuff. Including loading the com api library.
   AxInit initObj;

   try
   {
      AxCmdLineArgs args( argc, argv );

      pair<bool, int> aafinOpArg = args.get( "-aafin" );

      pair<bool, int> dotoutOpArg = args.get( "-dotout" );

      pair<bool, int> debugOpArg = args.get( "-debug" );

      pair<bool, int> refAsPropOpArg = args.get( "-refasproperty" );

      pair<bool, int> typeDefsOpArg = args.get( "-typedefs" );

      pair<bool, int> strongRefHOpArg = args.get( "-strongrefh" );


      if (!aafinOpArg.first) 
      {
	 throwUsage();
      }
      pair< bool, const char* > aafinFilenameArg = args.get( aafinOpArg.second + 1 );
      if ( !aafinFilenameArg.first ) 
      {
	 throwUsage();
      }
      AxString aafinFilename( AxStringUtil::mbtowc( aafinFilenameArg.second ) );


      if (!dotoutOpArg.first) 
      {
	 throwUsage();
      }
      pair< bool, const char* > dotoutFilenameArg = args.get( dotoutOpArg.second + 1 );
      if ( !dotoutFilenameArg.first ) 
      {
	 throwUsage();
      }

      AAFMetaMapProfile profile;
      if ( debugOpArg.first )
      {
	 profile.SetDebug( true );
      }
      if ( refAsPropOpArg.first )
      {
	 profile.SetRefAsProperty( true );
      }
      else if ( typeDefsOpArg.first )
      {
	 profile.SetTypeDefs( true );
      }
      else if ( strongRefHOpArg.first )
      {
	 profile.SetStrongRefH( true );
      }


      Export( aafinFilename, dotoutFilenameArg.second, &profile );
   }
   catch (const AxEx& ex) 
   {
      cout << ex.what() << endl;
      Logging::DebugLogStream() << "Export failed." << endl;
   }
   catch (...)
   {
      Logging::DebugLogStream() << "Export failed." << endl;
   }


   return 0;
}


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
//
//=---------------------------------------------------------------------=

#pragma warning (disable:4786)

#include <AxFile.h>
#include <AxTypes.h>
#include <AxInit.h>
#include <AxObject.h>
#include <AxMetaDef.h>
#include <AxHeader.h>
#include <AxDictionary.h>
#include <AxIterator.h>
#include <AxProperty.h>
#include <AxContentStorage.h>
#include <AxPropertyValueDump.h>
#include <AxBaseObj.h>
#include <AxBaseObjIter.h>
#include <AxUtil.h>
#include <AxEx.h>
#include <AAFStoredObjectIDs.h>
#include <AAFTypeDefUIDs.h>
#include <AxBaseObjIterExt.h>

#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;


#include <AAFDotInstanceMapper.h>
#include <InstanceMapperProfile.h>
#include <Logging.h>
#include <Utilities.h>


//-----------------------------------------------------------------------------
void throwUsage()
{
   wostringstream msg;
   msg << L"Usage: " << L"aaf2dot -aafin <aaf filename> -dotout <dot filename> [OPTIONS]" << endl << endl
       << L"OPTIONS:" << endl
       << L"-nodictionary             Don't include dictionary objects" << endl
       << L"-nodatadefrefs            Don't show data definition references (active if -nodictionary not selected)" << endl
       << L"-notaggedvalues           Don't include Tagged value objects" << endl
       << L"-noessencedata            Don't include Essence data objects" << endl
       << L"-noklvdata                Don't include KLV data objects" << endl
       << L"-cluster                  Group objects that belong to the same MOB" << endl
       << L"-concentrate              Merge (in graph) associations that have the same end-point" << endl
       << L"-debug                    Output debug information" << endl;
   throw AxEx( msg.str().c_str() );
}


//-----------------------------------------------------------------------------
void DotExport( AxHeader& axHeader,
		string dotOutFilename,
		InstanceMapperProfile profile ) 
{
   DotFactory dotFactory;

   // create the graph
   DotGraph *graph = dotFactory.CreateGraph( "MyGraph", dotFactory.CreateGraphUID() );

   // create the mapper
   if ( profile.GetDebug() ) 
   {
      Logging::SetDebugLogStream( &cout );
   }
   else 
   {
      Logging::SetNullDebugLogStream();
   }
   AAFDotInstanceMapper mapper( profile );
   mapper.SetDotFactory( &dotFactory );
   mapper.SetDotGraph( graph );



   // iterate through objects and map to Dot format
   auto_ptr< AxBaseObjIterPrtcl > axHeaderIter(
      new AxBaseSolitaryObjIter<AxHeader>(axHeader) );
   AxBaseObjRecIterExt recIter( axHeaderIter, &mapper );
   pair<bool,auto_ptr<AxBaseObj> > next;
   int level;
   for( next.first = recIter.NextOne( next.second, level ); next.first;
	next.first = recIter.NextOne( next.second, level ) )
   {
      if ( dynamic_cast< AxObject* >( next.second.get() ) ) 
      {
	 auto_ptr< AxObject > obj(
	    dynamic_cast< AxObject* >( next.second.release() ) );
			
	 bool popStack;
	 mapper.MapAAFObject( *obj, popStack );
	 if ( popStack )
	 {
	    recIter.PopStack();
	 }
      }

      else if ( dynamic_cast< AxProperty* >( next.second.get() ) ) 
      {
	 auto_ptr< AxProperty > prop( 
	    dynamic_cast< AxProperty* >( next.second.release() ) );
			
	 bool popStack;
	 mapper.MapAAFProperty( *prop, popStack );
	 if ( popStack )
	 {
	    recIter.PopStack();
	 }
      }

      else if ( dynamic_cast< AxPropertyValue* >( next.second.get() ) ) 
      {
	 auto_ptr< AxPropertyValue > propVal(
	    dynamic_cast< AxPropertyValue* >( next.second.release() ) );
			
	 bool popStack;
	 mapper.MapAAFPropertyValue( *propVal, popStack );
	 if ( popStack )
	 {
	    recIter.PopStack();
	 }

      }
      
      else if ( dynamic_cast< AxBaseObjAny< AxRecordIterator::Pair >* >( next.second.get() ) )
      {
	  
	 auto_ptr< AxBaseObjAny< AxRecordIterator::Pair > > recPair(
	    dynamic_cast< AxBaseObjAny< AxRecordIterator::Pair >* >( next.second.release() ) );
	  
	 // records implemented on a case-by-case basis at the property iteration level 
	 recIter.PopStack();
      }
		
      else if ( dynamic_cast< AxBaseObjAny< AxExHResult >* >( next.second.get() ) ) 
      {
	 auto_ptr< AxBaseObjAny< AxExHResult > > ex (
	    dynamic_cast< AxBaseObjAny< AxExHResult >* >( next.second.release() ) );
	  
	 // 'any' objects not implemented
	 recIter.PopStack();
      }
   }


   // resolve references in associations
   dotFactory.ResolveReferences();
  
   // write
   Logging::DebugLogStream() << "Writing dot file..." << endl;
   ofstream dotFile( dotOutFilename.c_str() );
   graph->Write( dotFile, profile );
   dotFile.close();
   Logging::DebugLogStream() << "Done." << endl;

}


//-----------------------------------------------------------------------------
class AxPropValueRenamePeskyOpaques	: public AxPropertyValueNoopPrtcl {
   public:

      AxPropValueRenamePeskyOpaques( AxDictionary& axDict )
	 : _axDict( axDict ),
	   _count(0)
      {}

      virtual ~AxPropValueRenamePeskyOpaques()
      {}

      virtual void process( IAAFPropertyValueSP& spPropVal,
			    IAAFTypeDefOpaqueSP& spTypeDefOpaque )
      {
	 AxTypeDefOpaque axOpaque( spTypeDefOpaque );

	 aafUID_t typeUid = axOpaque.GetActualTypeID( spPropVal );

	 if ( !_axDict.isKnownTypeDef( typeUid ) ) {

	    AxTypeDef axBaseTypeDef( _axDict.LookupTypeDef( kAAFTypeID_UInt8Array ) );

	    AxTypeDefRename axTypeDefRename(
	       AxCreateMetaInstance<IAAFTypeDefRename>( _axDict ) );

	    axTypeDefRename.Initialize( typeUid, axBaseTypeDef, AxString( L"Opaque Data" ) );

	    _axDict.RegisterOpaqueTypeDef( axTypeDefRename );

	    _count++;
	 }
      }

      int GetCount()
      {return _count;}

   private:
      AxDictionary& _axDict;
      int _count;
};


//-----------------------------------------------------------------------------
int renamePeskyOpaques( AxDictionary& axDict,
			AxBaseObjRecIter& recIter )
{
   using namespace std;

   pair<bool, auto_ptr< AxBaseObj > > next;

   int count = 0;
   int level;
   for( next.first = recIter.NextOne( next.second, level );
	next.first;
	next.first = recIter.NextOne( next.second, level ) ) {

      if ( dynamic_cast<AxPropertyValue*>( next.second.get() ) ) {

	 auto_ptr<AxPropertyValue> propVal(
	    dynamic_cast<AxPropertyValue*>( next.second.release() ) );

	 AxPropValueRenamePeskyOpaques axPropValueRenamePeskyOpaques( axDict );

	 propVal->Process( axPropValueRenamePeskyOpaques );

	 count += axPropValueRenamePeskyOpaques.GetCount();
      }
   }

   return count;
}

//-----------------------------------------------------------------------------
int main( int argc, char** argv )
{
   try {
      // Second command line argument is the filename
      AxCmdLineArgs args( argc, argv );


      // input aaf file
      pair<bool, int> aafInOpArg = args.get( "-aafin", 0 );
      if (!aafInOpArg.first)
      {
	 throwUsage();
      }
      pair<bool, const char*> aafInFilenameArg = args.get( aafInOpArg.second + 1 );
      if ( !aafInFilenameArg.first )
      {
	 throwUsage();
      }
      AxString aafInFilename( AxStringUtil::mbtowc( aafInFilenameArg.second ) );


      // output dot file
      pair<bool, int> dotOutOpArg = args.get( "-dotout", 0 );
      if (!dotOutOpArg.first)
      {
	 throwUsage();
      }
      pair<bool, const char*> dotOutFilenameArg = args.get( dotOutOpArg.second + 1 );
      if ( !dotOutFilenameArg.first ) 
      {
	 throwUsage();
      }
      string dotOutFilename( dotOutFilenameArg.second );


      // options
      InstanceMapperProfile profile;
      pair<bool, int> profileOp = args.get( "-nodictionary", 0 );
      if ( profileOp.first )
      {
	 profile.SetDictionary( false );
      }
      else
      {
	 profileOp = args.get( "-nodatadefrefs", 0 );
	 if ( profileOp.first ) 
	 {
	    profile.SetDataDefRefs( false );
	 }
      }
      profileOp = args.get( "-notaggedvalues", 0 );
      if ( profileOp.first ) 
      {
	 profile.SetTaggedValues( false );
      }
      profileOp = args.get( "-noklvdata" );
      if ( profileOp.first )
      {
	 profile.SetKLVData( false );
      }
      profileOp = args.get( "-noessencedata", 0 );
      if ( profileOp.first ) 
      {
	 profile.SetEssenceData( false );
      }

      profileOp = args.get( "-concentrate", 0 );
      if ( profileOp.first ) 
      {
	 profile.SetConcentrate( true );
      }

      profileOp = args.get( "-cluster", 0 );
      if ( profileOp.first )
      {
	 profile.SetCluster( true );
      }

      profileOp = args.get( "-debug", 0 );
      if ( profileOp.first )
      {
	 profile.SetDebug( true );
      }


      // One time init stuff.  Including loading the com api library.
      AxInit initObj;

      // Open the file.
      AxFile axFile;
      axFile.OpenExistingRead( aafInFilename );

		
      // Get the header to use as a root object from which to begin iteration.
      AxHeader axHeader( axFile.getHeader() );

      AxDictionary axDictionary( axHeader.GetDictionary() );

      {
	 auto_ptr< AxBaseObjIterPrtcl > axHeaderIter(
	    new AxBaseSolitaryObjIter<AxHeader>(axHeader) );

	 // Create a recursive iterator...
	 AxBaseObjRecIter recIter( axHeaderIter );

	 // ... and run through all values registering
	 // a renamed type for each opaque type.
	 int count = renamePeskyOpaques( axDictionary, recIter );
      }


      // export the AAF objects to an dot file
      DotExport( axHeader, dotOutFilename, profile );


      axFile.Close();
   }

   catch ( const AxEx& ex ) {
      wcout << ex.what() << endl;
      return -1;
   }

   catch (...)
   {
      return -1;
   }

   return 0;
}










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
#pragma warning (disable:4800)
#endif

#include <iostream>
#include <sstream>
#include <assert.h>
using namespace std;

#include <AAFXsMapper.h>
#include <Utilities.h>
#include <AxMetaDef.h>
#include <AxIterator.h>


//-----------------------------------------------------------------------------
AAFXsMapper::AAFXsMapper( AAFDefinitionHome *defHome )
  : _defHome( defHome )
{
}


//-----------------------------------------------------------------------------
AAFClassDefinition* 
AAFXsMapper::MapAAFClassDef( AxClassDef &axClassDef )
{
  // check if classdef already exists; if it does, then we are done
  aafUID_t uidt = axClassDef.GetAUID();
  string uid = UIDToString( uidt );
  AAFClassDefinition *classDef = dynamic_cast< AAFClassDefinition* > (_defHome->FindDefinition( uid ) );
  if ( classDef != 0 )
    {
      return classDef;
    }

  // first map the parent classdef
  bool hasParent = false;
  AAFClassDefinition *parentClassDef = 0;
  try
    {
      AxClassDef axParentClassDef( axClassDef.GetParent() );
      parentClassDef = MapAAFClassDef( axParentClassDef );
    }
  catch (...)
    {
      // has no parent
      // todo: don't want to use try/catch - clean this code
    }

  // map the classdef
  string name = AxStringToString( axClassDef.GetName() );
  string description = "";
  try
    {
      // description property is optional
      description = AxStringToString( axClassDef.GetDescription() );
    }
  catch (...)
    {
      description = "";
    }
  bool isConcrete = axClassDef.IsConcrete();
  classDef = new AAFClassDefinition( name, uid, description, isConcrete, parentClassDef );


  // map the property definitions
  AxPropertyDefIter propertyDefIter(axClassDef.GetPropertyDefs());
  IAAFPropertyDefSP spIaafPropertyDef;
  while (propertyDefIter.NextOne(spIaafPropertyDef))
    {
      AxPropertyDef axPropertyDef(spIaafPropertyDef);
      MapAAFPropertyDef( classDef, axClassDef, axPropertyDef );
    }


  // add the classDef to meta model or object model package
  _defHome->AddDefinition( classDef );
			
  return classDef;
}





//-----------------------------------------------------------------------------
AAFPropertyDefinition* 
AAFXsMapper::MapAAFPropertyDef( AAFClassDefinition *classDef, AxClassDef &axClassDef,
				AxPropertyDef &axPropertyDef)
{
  // get the type definition (map it if not done already)
  AxTypeDef axTypeDef( axPropertyDef.GetTypeDef() );
  aafUID_t tuidt = axTypeDef.GetAUID();
  string tuid = UIDToString( tuidt );
  AAFTypeDefinition *typeDef = dynamic_cast< AAFTypeDefinition* >( _defHome->FindDefinition( tuid ) );
  if ( typeDef == 0 )
    {
      // map the type definition
      typeDef = MapAAFTypeDef( axTypeDef );
    }


  // map the property def
  aafUID_t uidt = axPropertyDef.GetAUID();
  string uid = UIDToString( uidt );
  string name = AxStringToString( axPropertyDef.GetName() );
  string description = "";
  try
    {
      // description property is optional
      description = AxStringToString( axPropertyDef.GetDescription() );
    }
  catch (...)
    {
      description = "";
    }
  bool isOptional = axPropertyDef.GetIsOptional();
  bool isUniqueIdentifier;
  try 
    {
      isUniqueIdentifier = axPropertyDef.GetIsUniqueIdentifier();
    }
  catch (...) 
    {
      isUniqueIdentifier = false;
    }

  AAFPropertyDefinition *propertyDef = 
    new AAFPropertyDefinition( name, uid, description, isOptional,
			      typeDef, isUniqueIdentifier );

  classDef->AddProperty( propertyDef );

  return propertyDef;
}


//-----------------------------------------------------------------------------
AAFTypeDefinition* 
AAFXsMapper::MapAAFTypeDef( AxTypeDef &axTypeDef )
{
  // check if typedef already exists; if it does, then we are done
  aafUID_t uidt = axTypeDef.GetAUID();
  string uid = UIDToString( uidt );
  AAFTypeDefinition *typeDef = dynamic_cast< AAFTypeDefinition* > (_defHome->FindDefinition( uid ) );
  if ( typeDef != 0)
    {
      return typeDef;
    }

  // get metadefinition attributes
  string name = AxStringToString( axTypeDef.GetName() );
  string description = "";
  try
    {
      // description property is optional
      description = AxStringToString( axTypeDef.GetDescription() );
    }
  catch (...)
    {
      description = "";
    }


  // Map kAAFTypeCatUnknown
  if ( axTypeDef.GetTypeCategory() == kAAFTypeCatUnknown )
    {
      typeDef = new AAFTypeDefinitionUnknown( name, uid, description );
      _defHome->AddDefinition( typeDef );
    }


  // Map kAAFTypeCatInt
  else if ( axTypeDef.GetTypeCategory() == kAAFTypeCatInt )
    {
      AxTypeDefInt axTypeDefInt( AxQueryInterface< IAAFTypeDef, IAAFTypeDefInt >( axTypeDef ) );
      typeDef = new AAFTypeDefinitionInt( name, uid, description, (aafUInt8)axTypeDefInt.GetSize(), 
					      axTypeDefInt.IsSigned() );
      _defHome->AddDefinition( typeDef );
    }
	

  // Map kAAFTypeCatCharacter
  else if ( axTypeDef.GetTypeCategory() == kAAFTypeCatCharacter )
    {
      typeDef = new AAFTypeDefinitionCharacter( name, uid, description );
      _defHome->AddDefinition( typeDef );
    }


  // Map kAAFTypeCatRename
  else if ( axTypeDef.GetTypeCategory() == kAAFTypeCatRename )
    {
      // map the renamed type reference
      AxTypeDefRename axTypeDefRename( AxQueryInterface< IAAFTypeDef, IAAFTypeDefRename >( axTypeDef ) );
      AxTypeDef axBaseTypeDef( axTypeDefRename.GetBaseType() );
      AAFTypeDefinition *baseTypeDef = MapAAFTypeDef( axBaseTypeDef );

      typeDef = new AAFTypeDefinitionRename( name, uid, description, baseTypeDef );

      _defHome->AddDefinition( typeDef );
    }


  // Map kAAFTypeCatEnum
  else if ( axTypeDef.GetTypeCategory() == kAAFTypeCatEnum )
    {
      AxTypeDefEnum axTypeDefEnum( AxQueryInterface< IAAFTypeDef, IAAFTypeDefEnum > ( axTypeDef ) );

      // map the enumerated values type definition reference
      AxTypeDef axElementTypeDef( axTypeDefEnum.GetElementType() );
      AAFTypeDefinition *elementTypeDef = MapAAFTypeDef( axElementTypeDef );


      AAFTypeDefinitionEnum *typeDefEnum = new AAFTypeDefinitionEnum( name, uid, description, elementTypeDef );

      // map the enumerated values
      aafUInt32 count = axTypeDefEnum.CountElements();
      for ( aafUInt32 i=0; i<count; i++ )
	{
	  typeDefEnum->AddElement( AxStringToString( axTypeDefEnum.GetElementName( i ) ),
				   axTypeDefEnum.GetElementValue( i ) );
	}
		
      typeDef = typeDefEnum;

      _defHome->AddDefinition( typeDef );
    }

  // Map kAAFTypeCatExtEnum
  else if ( axTypeDef.GetTypeCategory() == kAAFTypeCatExtEnum )
    {
      AxTypeDefExtEnum axTypeDefExtEnum( AxQueryInterface< IAAFTypeDef, IAAFTypeDefExtEnum > ( axTypeDef ) );

      AAFTypeDefinitionExtEnum *typeDefExtEnum = 
	new AAFTypeDefinitionExtEnum( name, uid, description );

      // map the enumerated values
      aafUInt32 count = axTypeDefExtEnum.CountElements();
      for ( aafUInt32 i=0; i<count; i++ )
	{
	  aafUID_t uidt = axTypeDefExtEnum.GetElementValue( i );
	  typeDefExtEnum->AddElement( AxStringToString( axTypeDefExtEnum.GetElementName( i ) ),
				      UIDToString( uidt ) );
	}
		
      typeDef = typeDefExtEnum;

      _defHome->AddDefinition( typeDef );
    }

  // Map kAAFTypeCatFixedArray
  else if ( axTypeDef.GetTypeCategory() == kAAFTypeCatFixedArray )
    {
      AxTypeDefFixedArray axTypeDefFixedArray( AxQueryInterface< IAAFTypeDef, 
					       IAAFTypeDefFixedArray >( axTypeDef ) );
      AxTypeDef axElementTypeDef( axTypeDefFixedArray.GetType() );

      aafUInt32 count = axTypeDefFixedArray.GetCount();
      AAFTypeDefinition *elementTypeDef = MapAAFTypeDef( axElementTypeDef );

      typeDef = new AAFTypeDefinitionFixedArray( name, uid, description, elementTypeDef, count );

      _defHome->AddDefinition( typeDef );
    }


  // Map kAAFTypeCatVariableArray
  else if ( axTypeDef.GetTypeCategory() == kAAFTypeCatVariableArray )
    {
      AxTypeDefVariableArray axTypeDefVariableArray( AxQueryInterface< IAAFTypeDef,
						     IAAFTypeDefVariableArray >( axTypeDef ) );
      AxTypeDef axElementTypeDef( axTypeDefVariableArray.GetType() );

      AAFTypeDefinition *elementTypeDef = MapAAFTypeDef( axElementTypeDef );

      typeDef = new AAFTypeDefinitionVariableArray( name, uid, description, elementTypeDef );

      _defHome->AddDefinition( typeDef );
    }


  // Map kAAFTypeCatSet
  else if ( axTypeDef.GetTypeCategory() == kAAFTypeCatSet )
    {
      AxTypeDefSet axTypeDefSet( AxQueryInterface< IAAFTypeDef, IAAFTypeDefSet >( axTypeDef ) );
      AxTypeDef axElementTypeDef( axTypeDefSet.GetElementType() );

      AAFTypeDefinition *elementTypeDef = MapAAFTypeDef( axElementTypeDef );

      typeDef = new AAFTypeDefinitionVariableArray( name, uid, description, elementTypeDef );

      _defHome->AddDefinition( typeDef );
    }


  // Map kAAFTypeCatString
  else if ( axTypeDef.GetTypeCategory() == kAAFTypeCatString )
    {
      typeDef = new AAFTypeDefinitionString( name, uid, description );
      _defHome->AddDefinition( typeDef );
    }


  // Map kAAFTypeCatStrongObjRef
  else if ( axTypeDef.GetTypeCategory() == kAAFTypeCatStrongObjRef )
    {
      AxTypeDefStrongObjRef axTypeDefStrongObjRef( AxQueryInterface< IAAFTypeDef,
						   IAAFTypeDefStrongObjRef >( axTypeDef ) );
      AxClassDef axRefClassDef( axTypeDefStrongObjRef.GetObjectType() );
      
      aafUID_t uidt = axRefClassDef.GetAUID();
      string refClassDefAuid = UIDToString( uidt );
      
      typeDef = new AAFTypeDefinitionStrongObjRef( name, uid, description, refClassDefAuid );

      _defHome->AddDefinition( typeDef );
    }



  // Map kAAFTypeCatRecord
  else if ( axTypeDef.GetTypeCategory() == kAAFTypeCatRecord )
    {
      AAFTypeDefinitionRecord *typeDefRecord = 
	new AAFTypeDefinitionRecord( name, uid, description );


      // map the record members
      AxTypeDefRecord axTypeDefRecord( AxQueryInterface< IAAFTypeDef,
				       IAAFTypeDefRecord >( axTypeDef ) );
      aafUInt32 count = axTypeDefRecord.GetCount();
      for ( aafUInt32 i=0; i<count; i++ )
	{
	  AxTypeDef axMemberTypeDef( axTypeDefRecord.GetMemberType( i ) );
	  AAFTypeDefinition *memberTypeDef = MapAAFTypeDef( axMemberTypeDef );

	  typeDefRecord->AddMember( AxStringToString( axTypeDefRecord.GetMemberName( i ) ), memberTypeDef );
	}

      typeDef = typeDefRecord;

      _defHome->AddDefinition( typeDef );
    }


  // Map kAAFTypeCatWeakObjRef
  else if ( axTypeDef.GetTypeCategory() == kAAFTypeCatWeakObjRef )
    {
      AxTypeDefWeakObjRef axTypeDefWeakObjRef( AxQueryInterface< IAAFTypeDef,
					       IAAFTypeDefWeakObjRef >( axTypeDef ) );
      AxClassDef axRefClassDef( axTypeDefWeakObjRef.GetObjectType() );
      
      aafUID_t uidt = axRefClassDef.GetAUID();
      string refClassDefAuid = UIDToString( uidt );
      
      typeDef = new AAFTypeDefinitionWeakObjRef( name, uid, description, refClassDefAuid );

      _defHome->AddDefinition( typeDef );
    }


  // Map kAAFTypeCatStream
  else if ( axTypeDef.GetTypeCategory() == kAAFTypeCatStream )
    {
      typeDef = new AAFTypeDefinitionStream( name, uid, description );
      _defHome->AddDefinition( typeDef );
    }


  // Map kAAFTypeCatIndirect
  else if ( axTypeDef.GetTypeCategory() == kAAFTypeCatIndirect )
    {
      typeDef = new AAFTypeDefinitionIndirect( name, uid, description );
      _defHome->AddDefinition( typeDef );
    }


  // Map kAAFTypeCatOpaque
  else if ( axTypeDef.GetTypeCategory() == kAAFTypeCatOpaque )
    {
      typeDef = new AAFTypeDefinitionOpaque( name, uid, description );
      _defHome->AddDefinition( typeDef );
    }


  // shouldn't be here
  else
    {
      // todo: throw a proper exception here
      cerr << "Error: Unknown type category encountered.";
      throw;
    }

  return typeDef;
}


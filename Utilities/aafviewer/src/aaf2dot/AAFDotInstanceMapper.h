#ifndef __AAFDOTINSTANCEMAPPER_H__
#define __AAFDOTINSTANCEMAPPER_H__

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

#ifdef _MSC_VER
#pragma warning (disable:4786)
#endif

#include <InstanceMapperProfile.h>
#include <DotFactory.h>
#include <AxBaseObjIterExt.h>

#include <AxPropertyValue.h>
#include <AxObject.h>
#include <AxProperty.h>
#include <AxBaseObjIter.h>
#include <AxMetaDef.h>
#include <AAFTypes.h>
  
#include <deque>
#include <map>



//-----------------------------------------------------------------------------
// base class for classes that follow the AAF recursive iterator through the 
// AAF object tree and represent the equivalent DOT tree
class Stalker
{
  public:
   Stalker() {};
   virtual ~Stalker() {};
};

typedef std::deque< Stalker* > StalkerStack;


//-----------------------------------------------------------------------------
// Follows the AAF objects
class ObjectStalker : public Stalker
{
  public:
   ObjectStalker( DotRecordNode *node, std::string name ) : _name( name ), _node( node ),
      _subGraph( 0 ) {};
   virtual ~ObjectStalker() {};

   DotRecordNode* GetNode() { return _node; }

   std::string GetName() { return _name; }

   // remember the subgraph that is the parent for all child objects
   // this is used when the -cluster commandline option is set
   void SetSubGraph( DotSubGraph *subGraph ) { _subGraph = subGraph; }
   bool HasSubGraph() { return _subGraph != 0; }
   DotSubGraph* GetSubGraph() { return _subGraph; }


  private:
   std::string _name;
   DotRecordNode *_node;
   DotSubGraph *_subGraph;
};


//-----------------------------------------------------------------------------
// Follows the AAF property value
class PropertyValueStalker : public Stalker
{
  public:
   PropertyValueStalker( std::string name );
   virtual ~PropertyValueStalker() {};

   // remember the reference in the stalker for resolving later using CompleteReference
   void SetReference( DotEdgeEnd *reference )
      {
	 _reference = reference;
      }
   // Set the node referenced by the edge in the edge object
   void CompleteReference( DotRecordNode *referencedNode );


   std::string GetName()
      {
	 return _name;
      }

   // index for ordered-arrays
   void IncreaseArrayIndex();
   void InitArrayIndex();
   aafUInt32 GetArrayIndex() { return _arrayIndex; };

  private:
   std::string _name;
   DotEdgeEnd *_reference;
   aafUInt32 _arrayIndex;
};


//-----------------------------------------------------------------------------
// This class provides the mapping between the AAF data and DOT data
class AAFDotInstanceMapper : public AxBaseObjRecIterListener
{
  public:
   AAFDotInstanceMapper( InstanceMapperProfile profile );
   virtual ~AAFDotInstanceMapper();

   void SetDotFactory( DotFactory *dotFactory );
   void SetDotGraph( DotGraph *graph );

   // the map functions for the AAF data
   void MapAAFObject( AxObject axObject, bool &popStack );
   void MapAAFProperty( AxProperty axProperty, bool &popStack );
   void MapAAFPropertyValue( AxPropertyValue axPropertyValue, bool &popStack );
	

  protected:
   DotFactory *_dotFactory;
   DotGraph *_dotGraph;
   InstanceMapperProfile _profile;

   StalkerStack _stalkers;

  protected:
   // record types in general are not mapped. Only known record types are mapped
   void MapKnownAAFRecordTypes( AxProperty axProperty, bool &popStack );

   // map generically the property values
   void MapAAFPropertyValueGeneric( AxTypeDef &axTypeDef, AxPropertyValue axPropertyValue,
				    bool &popStack );

   // empty map for unknown/unsupported property types ( the property is given the value "?" )
   void MapEmptyPropertyValue();

   // called to complete the information of the parents strong reference to the current object
   void CompleteStrongReferences( DotRecordNode *referencedNode );

   // filter out objects according to user commandline options
   bool FilterAAFObject( AxObject axObject, bool &popStack );

   // extract int value from the int property value; possibly display in hex ( see MapperSettings.h)
   std::string GetIntValue( AxTypeDefInt &axTypeDefInt, AxPropertyValue axPropertyValue, bool displayHex );
   std::string GetIntValue( AxTypeDefInt &axTypeDefInt, AxPropertyValue axPropertyValue,
			    std::string objectName, std::string propertyName );
   std::string GetIntValue( AxTypeDefInt &axTypeDefInt, AxPropertyValue axPropertyValue );
   std::string GetIntValue( AxTypeDefInt &axTypeDefInt, IAAFPropertyValueSP axPropertyValue );

   // find the node referenced by the scope reference. 
   // relative scope > 0 not implemented.  
   DotRecordNode* GetScopeReference( aafUInt32 relativeScope, aafUInt32 relativeSlot );


   // The stalkers follow the AAF base object iterator, linking the DOT data tree and the AAF data tree
   Stalker* PopStalker();
   void PushStalker( Stalker *stalker );

   // from AxBaseObjRecIterListener
   virtual void ObjectPop();
   virtual void PropertyPop();
};



#endif //__AAFDOTINSTANCEMAPPER_H__



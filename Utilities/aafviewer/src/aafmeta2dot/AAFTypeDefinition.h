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

#ifndef __AAFTYPEDEFINITION_H__
#define __AAFTYPEDEFINITION_H__

#include <vector>
#include <map>

#include <AAFMetaDefinition.h>
#include <AAFTypes.h>


class AAFClassDefinition;


//-----------------------------------------------------------------------------
class AAFTypeDefinition : public AAFMetaDefinition
{
  public:
   AAFTypeDefinition( std::string name, std::string auid, std::string description );
   virtual ~AAFTypeDefinition() {};

  public:
   virtual void BuildDot( AAFDotHome *dotHome, DotFactory *factory, DotGraph *subGraph ) = 0;
};


//-----------------------------------------------------------------------------
class AAFTypeDefinitionUnknown : public AAFTypeDefinition
{
  public:
   AAFTypeDefinitionUnknown( std::string name, std::string auid, std::string description );

  public:
   virtual void BuildDot( AAFDotHome *dotHome, DotFactory *factory, DotGraph *graph );

};


//-----------------------------------------------------------------------------
class AAFTypeDefinitionInt : public AAFTypeDefinition
{
  public:
   AAFTypeDefinitionInt( std::string name, std::string auid, std::string description, 
			 aafUInt8 size, bool isSigned );

  public:
   virtual void BuildDot( AAFDotHome *dotHome, DotFactory *factory, DotGraph *graph );

  protected:
   aafUInt8 _size;
   bool _isSigned;
};


//-----------------------------------------------------------------------------
class AAFTypeDefinitionCharacter : public AAFTypeDefinition
{
  public:
   AAFTypeDefinitionCharacter( std::string name, std::string auid, std::string description );

  public:
   virtual void BuildDot( AAFDotHome *dotHome, DotFactory *factory, DotGraph *graph );

};


//-----------------------------------------------------------------------------
class AAFTypeDefinitionRename : public AAFTypeDefinition
{
  public:
   AAFTypeDefinitionRename( std::string name, std::string auid, std::string description, 
			    AAFTypeDefinition *baseDef );

  public:
   virtual void BuildDot( AAFDotHome *dotHome, DotFactory *factory, DotGraph *graph );

  protected:
   AAFTypeDefinition *_baseDef;
};


//-----------------------------------------------------------------------------
class AAFTypeDefinitionEnum : public AAFTypeDefinition
{
  public:
   AAFTypeDefinitionEnum( std::string name, std::string auid, std::string description,
			  AAFTypeDefinition *elementTypeDef );

  public:
   virtual void BuildDot( AAFDotHome *dotHome, DotFactory *factory, DotGraph *graph );

  public:
   void AddElement( std::string name, aafUInt64 value );

  protected:
   AAFTypeDefinition *_elementTypeDef;
   std::map< std::string, aafUInt64 > _elements;
};


//-----------------------------------------------------------------------------
class AAFTypeDefinitionExtEnum : public AAFTypeDefinition
{
  public:
   AAFTypeDefinitionExtEnum( std::string name, std::string auid, std::string description );


  public:
   virtual void BuildDot( AAFDotHome *dotHome, DotFactory *factory, DotGraph *graph );

  public:
   void AddElement( std::string name, std::string value );

  protected:
   std::map< std::string, std::string > _elements;
};


//-----------------------------------------------------------------------------
class AAFTypeDefinitionFixedArray : public AAFTypeDefinition
{
  public:
   AAFTypeDefinitionFixedArray( std::string name, std::string auid, std::string description, 
				AAFTypeDefinition *elementTypeDef, aafUInt32 count );

  public:
   virtual void BuildDot( AAFDotHome *dotHome, DotFactory *factory, DotGraph *graph );

  public:
   AAFTypeDefinition* GetElementTypeDef() { return _elementTypeDef; }
   aafUInt32 GetCount() { return _count; }

  protected:
   AAFTypeDefinition *_elementTypeDef;
   aafUInt32 _count;
};


//-----------------------------------------------------------------------------
class AAFTypeDefinitionVariableArray : public AAFTypeDefinition
{
  public:
   AAFTypeDefinitionVariableArray( std::string name, std::string auid, std::string description, 
				   AAFTypeDefinition *elementTypeDef );
  public:
   virtual void BuildDot( AAFDotHome *dotHome, DotFactory *factory, DotGraph *graph );

  public:
   AAFTypeDefinition* GetElementTypeDef() { return _elementTypeDef; }

  protected:
   AAFTypeDefinition *_elementTypeDef;
};


//-----------------------------------------------------------------------------
class AAFTypeDefinitionSet : public AAFTypeDefinition
{
  public:
   AAFTypeDefinitionSet( std::string name, std::string auid, std::string description, 
			 AAFTypeDefinition *elementTypeDef );
  public:
   virtual void BuildDot( AAFDotHome *dotHome, DotFactory *factory, DotGraph *graph );

  public:
   AAFTypeDefinition* GetElementTypeDef() { return _elementTypeDef; }

  protected:
   AAFTypeDefinition *_elementTypeDef;
};


//-----------------------------------------------------------------------------
// TODO: The meta dictionary has a reference to an element type; am assuming
// that this is incorrect (or possibly a way to define strings of different characters
// types??)
class AAFTypeDefinitionString : public AAFTypeDefinition
{
  public:
   AAFTypeDefinitionString( std::string name, std::string auid, std::string description );

  public:
   virtual void BuildDot( AAFDotHome *dotHome, DotFactory *factory, DotGraph *graph );
};


//-----------------------------------------------------------------------------
class AAFTypeDefinitionStream : public AAFTypeDefinition
{
  public:
   AAFTypeDefinitionStream( std::string name, std::string auid, std::string description );

  public:
   virtual void BuildDot( AAFDotHome *dotHome, DotFactory *factory, DotGraph *graph );

};


//-----------------------------------------------------------------------------
class AAFTypeDefinitionRecord : public AAFTypeDefinition
{
  public:
   AAFTypeDefinitionRecord( std::string name, std::string auid, std::string description );

  public:
   virtual void BuildDot( AAFDotHome *dotHome, DotFactory *factory, DotGraph *graph );

  public:
   void AddMember( std::string name, AAFTypeDefinition *typeDef );

  protected:
   std::map< std::string, AAFTypeDefinition* > _members;
};


//-----------------------------------------------------------------------------
class AAFTypeDefinitionRef : public AAFTypeDefinition
{
  public:
   AAFTypeDefinitionRef( std::string name, std::string auid, std::string description,
			 std::string refTypeDefAuid );

  public:
   AAFClassDefinition* GetRefClassDef( AAFDefinitionHome *home );

  public:
   virtual void BuildDot( AAFDotHome *dotHome, DotFactory *factory, DotGraph *graph ) = 0;

  protected:
   std::string _refClassDefAuid;
   AAFClassDefinition* _refClassDef;
};


//-----------------------------------------------------------------------------
class AAFTypeDefinitionStrongObjRef : public AAFTypeDefinitionRef
{
  public:
   AAFTypeDefinitionStrongObjRef( std::string name, std::string auid, std::string description, 
				  std::string refTypeDefAuid );
  public:
   virtual void BuildDot( AAFDotHome *dotHome, DotFactory *factory, DotGraph *graph );

};


//-----------------------------------------------------------------------------
// TODO: the target set
class AAFTypeDefinitionWeakObjRef : public AAFTypeDefinitionRef
{
  public:
   AAFTypeDefinitionWeakObjRef( std::string name, std::string auid, std::string description, 
				std::string refTypeDefAuid );
  public:
   virtual void BuildDot( AAFDotHome *dotHome, DotFactory *factory, DotGraph *graph );

};


//-----------------------------------------------------------------------------
class AAFTypeDefinitionIndirect : public AAFTypeDefinition
{
  public:
   AAFTypeDefinitionIndirect( std::string name, std::string auid, std::string description );

  public:
   virtual void BuildDot( AAFDotHome *dotHome, DotFactory *factory, DotGraph *graph );

};

//-----------------------------------------------------------------------------
class AAFTypeDefinitionOpaque : public AAFTypeDefinitionIndirect
{
  public:
   AAFTypeDefinitionOpaque( std::string name, std::string auid, std::string description );

  public:
   virtual void BuildDot( AAFDotHome *dotHome, DotFactory *factory, DotGraph *graph );

};





#endif  //__AAFTYPEDEFINITION_H__


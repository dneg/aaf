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

#ifndef __AAFPROPERTYDEFINITION_H__
#define __AAFPROPERTYDEFINITION_H__

#include <vector>

#include <AAFTypes.h>

#include <AAFMetaDefinition.h>
#include <AAFTypeDefinition.h>


class AAFClassDefinition;

class AAFPropertyDefinition : public AAFMetaDefinition
{
  public:
   AAFPropertyDefinition( std::string name, std::string auid, std::string description,
			  bool isOptional, AAFTypeDefinition *typeDef, 
			  bool isUniqueIdentifier );
   virtual ~AAFPropertyDefinition() {};


  public:
   AAFTypeDefinition* GetTypeDefinition() { return _typeDef; }
   bool IsUniqueIdentifier() { return _isUniqueIdentifier; }


  public:
   virtual void BuildDot( AAFDotHome *dotHome, DotFactory *factory, DotGraph *graph ) {};

   void BuildDot( AAFDotHome *dotHome, DotFactory *factory, DotGraph *graph, DotRecordNode *node );


  protected:
   bool _isOptional;
   AAFTypeDefinition *_typeDef;
   bool _isUniqueIdentifier;
};


#endif  //__AAFPROPERTYDEFINITION_H__


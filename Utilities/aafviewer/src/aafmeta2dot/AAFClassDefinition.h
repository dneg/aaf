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

#ifndef __AAFCLASSDEFINITION_H__
#define __AAFCLASSDEFINITION_H__

#include <vector>

#include <AAFMetaDefinition.h>
#include <AAFPropertyDefinition.h>



typedef std::vector< AAFPropertyDefinition* > PropertyVector;


class AAFClassDefinition : public AAFMetaDefinition
{
  public:
   AAFClassDefinition( std::string name, std::string auid, std::string description,
		       bool isConcrete, AAFClassDefinition *parent );
   virtual ~AAFClassDefinition();

  public:
   virtual void InitForBuild();


  public:
   virtual void BuildDot( AAFDotHome *dotHome, DotFactory *factory, DotGraph *graph );


  public:
   void AddProperty( AAFPropertyDefinition *property );

   AAFPropertyDefinition* FindPropertyDefinition( std::string auid );


  protected:
   bool _isConcrete;
   PropertyVector _properties;

   AAFClassDefinition *_parent;

};



#endif  //__AAFCLASSDEFINITION_H__


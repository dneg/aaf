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

#ifndef __AAFMETADEFINITION_H__
#define __AAFMETADEFINITION_H__


#include <DotFactory.h>

class AAFDefinitionHome;
class AAFDotHome;

class AAFMetaDefinition
{
  public:
   AAFMetaDefinition( std::string name, std::string auid, std::string description );
   virtual ~AAFMetaDefinition() {};

  public:
   virtual void InitForBuild();

  protected:
   bool _wasBuilt;


  public:
   virtual void BuildDot( AAFDotHome *dotHome, DotFactory *factory, DotGraph *graph ) = 0;


  public:
   std::string GetAUID() { return _auid; }
   std::string GetName() { return _name; }
   std::string GetDescription() { return _description; }


  protected:
   std::string _name;
   std::string _auid;
   std::string _description;

};


#endif //__AAFMETADEFINITION_H__




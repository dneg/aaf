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

#ifndef __AAFMETAMAPPROFILE_H__
#define __AAFMETAMAPPROFILE_H__


// holds the information relevant to the way the AAF meta-model data is mapped onto DOT data
class AAFMetaMapProfile
{
  public:
   AAFMetaMapProfile();
   ~AAFMetaMapProfile();

   bool GetDebug() { return _debug; }
   void SetDebug( bool debug ) { _debug = debug; }

   bool GetRefAsProperty() { return _refAsProperty; }
   void SetRefAsProperty( bool refAsProperty ) { _refAsProperty = refAsProperty; }

   bool GetTypeDefs() { return _typeDefs; }
   void SetTypeDefs( bool typeDefs ) { _typeDefs = typeDefs; }

   bool GetStrongRefH() { return _strongRefH; }
   void SetStrongRefH( bool strongRefH ) { _strongRefH = strongRefH; }

   std::string GetGeneralisationEdgeWeight()
      {
	 if ( _strongRefH )
	 {
	    return "0.1";
	 }
	 else
	 {
	    return "10.0";
	 }
      }

   std::string GetStrongRefEdgeWeight()
      {
	 if ( _strongRefH )
	 {
	    return "10.0";
	 }
	 else
	 {
	    return "1.0";
	 }
      }
  private:
   bool _debug;
   bool _refAsProperty;
   bool _typeDefs;
   bool _strongRefH;
};

#endif	//__AAFMETAMAPPROFILE_H__


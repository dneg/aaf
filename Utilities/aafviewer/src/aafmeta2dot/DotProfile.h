#ifndef __DOTPROFILE_H__
#define __DOTPROFILE_H__

/*
 *      Copyright (c) 2003, Philip de Nier (philipn@users.sourceforge.net)
 *
 *
 *      This file is part of aaf2dot.
 *
 *  aaf2dot is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Lesser General Public License as published by
 *  the Free Software Foundation; either version 2.1 of the License, or
 *  (at your option) any later version.
 *
 *  aaf2dot is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License
 *  along with aaf2dot; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#include <string>
#include <sstream>

// holds the information relevant to the way the AAF data is mapped onto DOT data
class DotProfile
{
  public:
   DotProfile();

   ~DotProfile();

   void SetMaxAttributeLength( int length ) { _maxAttributeLength = length; }

   int GetMaxAttributeLength() { return _maxAttributeLength; }

   void SetMaxAttributeWidth( int width ) { _maxAttributeWidth = width; }

   int GetMaxAttributeWidth() { return _maxAttributeWidth; }

   void SetConcentrate( bool concentrate ) { _concentrate = concentrate; }

   int GetConcentrate() { return _concentrate; }

   void SetCluster( bool cluster ) { _cluster = cluster; }

   int GetCluster() { return _cluster; }

   void SetDebug( bool debug ) { _debug = debug; }

   int GetDebug() { return _debug; }


  private:
   int _maxAttributeLength;
   int _maxAttributeWidth;
   bool _concentrate;
   bool _cluster;
   bool _debug;
};

#endif	//__DOTPROFILE_H__


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

#ifndef __UTILITIES_H__
#define __UTILITIES_H__

#include <AxTypes.h>

#include <string>


template< class T >
inline void ErasePointerVector( std::vector< T > &theVector )
{
  typedef typename std::vector< T >::iterator iterator;

  iterator iter;
  for ( iter=theVector.begin(); iter!=theVector.end(); iter++ )
  {
    delete *iter;
  }
  theVector.erase( theVector.begin(), theVector.end() );
}


std::string AxStringToString( AxString axString );

//std::string UIDToString( aafUID_t& uid );

std::string UIDToString( aafUID_t uid );



#endif //__UTILITIES_H__

#ifndef __DOTUTILITIES_H__
#define __DOTUTILITIES_H__

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


#include <string>
#include <vector>
#include <iostream>


/// \brief erases the a vector of pointers and deletes the memory pointed at by 
// the elements of the vector
template< class T >
inline void ErasePointerVector( std::vector< T > &theVector )
{
   typename std::vector< T >::iterator iter;

   for ( iter=theVector.begin(); iter!=theVector.end(); iter++ )
   {
      delete *iter;
   }
   theVector.erase( theVector.begin(), theVector.end() );
}


std::string ProcessStringForQuoting( std::string s );

std::string LimitAttributeSize( std::string name, int maxLength, int maxWidth );

#endif //__DOTUTILITIES_H__


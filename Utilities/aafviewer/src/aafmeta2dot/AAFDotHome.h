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

#ifndef __AAFDOTHOME_H__
#define __AAFDOTHOME_H__

#include <vector>

#include <AAFDefinitionHome.h>
#include <AAFMetaMapProfile.h>

class AAFDotHome : public AAFDefinitionHome
{
 public:
  AAFDotHome( std::string aafFilename, std::string dotFilename, AAFMetaMapProfile *profile );
  virtual ~AAFDotHome();

  void Write();

  public:
  AAFMetaMapProfile* GetProfile() { return _profile; }

 private:
  std::string _aafFilename;
  std::string _dotFilename;

  AAFMetaMapProfile *_profile;
};


#endif // __AAFDOTHOME_H__


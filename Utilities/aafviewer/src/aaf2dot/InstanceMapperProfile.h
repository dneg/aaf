#ifndef __INSTANCEMAPPERPROFILE_H__
#define __INSTANCEMAPPERPROFILE_H__

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
class InstanceMapperProfile
{
  public:
   InstanceMapperProfile();

   ~InstanceMapperProfile();

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

   void SetDataDefRefs( bool d )
      {
	 _dataDefRefs = d;
      }
   void SetOnlyFirstDataDefRefs( bool d )
      {
	 _onlyFirstDataDefRefs = d;
      }
   void SetDictionary( bool d )
      {
	 _dictionary = d;
      }
   void SetTaggedValues( bool t )
      {
	 _taggedValues = t;
      }
   void SetKLVData( bool k )
      {
	 _klvData = k;
      }
   void SetEssenceData( bool e )
      {
	 _essenceData = e;
      }

   bool GetDataDefs()
      {
	 return _dataDefRefs;
      }
   bool GetOnlyFirstDataDefRefs()
      {
	 return _onlyFirstDataDefRefs;
      }
   bool GetDictionary()
      {
	 return _dictionary;
      }
   bool GetTaggedValues()
      {
	 return _taggedValues;
      }
   bool GetKLVData()
      {
	 return _klvData;
      }
   bool GetEssenceData()
      {
	 return _essenceData;
      }

   std::string GetStrongRefEdgeWeight()
      {
	 if ( _cluster )
	 {
	    return "5.0";
	 }
	 else
	 {
	    return "10.0";
	 }
      }
   std::string GetWeakRefEdgeWeight()
      {
	 if ( _cluster )
	 {
	    return "1.0";
	 }
	 else
	 {
	    return "1.0";
	 }
      }
   std::string GetSourceRefEdgeWeight()
      {
	 if ( _cluster )
	 {
	    return "10.0";
	 }
	 else
	 {
	    return "5.0";
	 }
      }

   void AppendComment( std::string &comment )
      {
	 std::ostringstream tmp;
	 tmp << "Property name and value length limited to "
	     << _maxAttributeLength << " characters" << std::endl;
	 comment.append( tmp.str() );
	 if ( !GetDictionary() )
	 {
	    comment.append( "No Dictionary\n" );
	 }
	 else if ( !GetDataDefs() )
	 {
	    comment.append( "No references to Data Definitions\n" );
	 }
	 else if ( GetOnlyFirstDataDefRefs() )
	 {
	    comment.append( "Only Data Definition references of first segment in slot shown\n" );
	 }
	 if ( !GetTaggedValues() )
	 {
	    comment.append( "No Tagged Values\n" );
	 }
	 if ( !GetKLVData() )
	 {
	    comment.append( "No KLV Data\n" );
	 }
	 if ( !GetEssenceData() )
	 {
	    comment.append( "No Essence Data\n" );
	 }
      }

  private:
   bool _dataDefRefs;
   bool _onlyFirstDataDefRefs;
   bool _dictionary;
   bool _taggedValues;
   bool _klvData;
   bool _essenceData;
   int _maxAttributeLength;
   int _maxAttributeWidth;
   bool _concentrate;
   bool _cluster;
   bool _debug;

};

#endif	//__INSTANCEMAPPERPROFILE_H__


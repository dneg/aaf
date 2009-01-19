//=---------------------------------------------------------------------=
//
// $Id$
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
//
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
//=---------------------------------------------------------------------=

//Edit Protocol Test files
#include "AllowedEditRateTable.h"

#include <cmath>

namespace {

using namespace aafanalyzer;

} // end of namespace

namespace aafanalyzer {

using namespace boost;

Section71TableEntry::Section71TableEntry(Section71TableEntry::TrackType_e t,
					 int exactNum, int exactDen,
					 int roundedNum, int roundedDen)
{
  _type = t;
  _exactRate.numerator     = exactNum;
  _exactRate.denominator   = exactDen;
  _roundedRate.numerator   = roundedNum;
  _roundedRate.denominator = roundedDen;
}

Section71TableEntry::TrackType_e Section71TableEntry::GetTrackType() const
{
  return _type;
}

const aafRational_t& Section71TableEntry::GetExactRate() const
{
  return _exactRate;
}

const aafRational_t& Section71TableEntry::GetRoundedRate() const
{
  return _roundedRate;
}

bool Section71TableEntry::IsEquivalent(const aafRational_t& rate, bool isVideo) const
{
  // Zero denominator: always invalid.
  if (rate.denominator == 0)
  {
    return false;
  }

  // Video is never equivalent to and audio rate.
  if (isVideo && _type == A)
  {
    return false;
  }
  
  // Else, cross multiple to compare rates (both exact and rounded)
  if (_exactRate.numerator * rate.denominator == rate.numerator * _exactRate.denominator ||
      _roundedRate.numerator * rate.denominator == rate.numerator * _roundedRate.denominator)
  {
    return true;
  }
  
  return false;
}

 //========================================

AllowedEditRateTable::AllowedEditRateTable()
{
    this->InitializeTable();
}

AllowedEditRateTable::~AllowedEditRateTable()
{
}

void AllowedEditRateTable::InitializeTable()
{
  Section71TableEntry::TrackType_e av = Section71TableEntry::AV;
  Section71TableEntry::TrackType_e a = Section71TableEntry::A;

   // 59.94 pull down
  _rates.push_back(Section71TableEntry(av,    24000, 1001,  23976, 1000));
  _rates.push_back(Section71TableEntry(av,    25000, 1001,  24975, 1000));
  _rates.push_back(Section71TableEntry(av,    30000, 1001,  29970, 1000));
  _rates.push_back(Section71TableEntry(av,    50000, 1001,  49950, 1000));
  _rates.push_back(Section71TableEntry(av,    60000, 1001,  59940, 1000));
  _rates.push_back(Section71TableEntry(a,  44100000, 1001,  44056, 1));
  _rates.push_back(Section71TableEntry(a,  48000000, 1001,  47952, 1));
  _rates.push_back(Section71TableEntry(a,  88200000, 1001,  88112, 1));
  _rates.push_back(Section71TableEntry(a,  96000000, 1001,  95904, 1));
  _rates.push_back(Section71TableEntry(a, 176400000, 1001, 176224, 1));
  _rates.push_back(Section71TableEntry(a, 192000000, 1001, 191808, 1));

  // 59.94i pull up
  _rates.push_back(Section71TableEntry(av,    24024, 1000,  24024, 1000));
  _rates.push_back(Section71TableEntry(av,    25025, 1000,  25025, 1000));
  _rates.push_back(Section71TableEntry(av,    30030, 1000,  30030, 1000));
  _rates.push_back(Section71TableEntry(av,    50050, 1000,  50050, 1000));
  _rates.push_back(Section71TableEntry(av,    60060, 1000,  60060, 1000));
  _rates.push_back(Section71TableEntry(a,  44144100, 1000,  44144, 1));
  _rates.push_back(Section71TableEntry(a,  48048000, 1000,  48048, 1));
  _rates.push_back(Section71TableEntry(a,  88288200, 1000,  88288, 1));
  _rates.push_back(Section71TableEntry(a,  96096000, 1000,  96096, 1));
  _rates.push_back(Section71TableEntry(a, 176576400, 1000, 176576, 1));
  _rates.push_back(Section71TableEntry(a, 192192000, 1000, 192192, 1));

  // 50i pull down
  _rates.push_back(Section71TableEntry(av,    576, 25,  23040, 1000));
  _rates.push_back(Section71TableEntry(av,    600, 25,  24000, 1000));
  _rates.push_back(Section71TableEntry(av,    720, 25,  28800, 1000));
  _rates.push_back(Section71TableEntry(av,   1200, 25,  48000, 1000));
  _rates.push_back(Section71TableEntry(av,   1440, 25,  57600, 1000));
  _rates.push_back(Section71TableEntry(a, 1058400, 25,  42336, 1));
  _rates.push_back(Section71TableEntry(a, 1152000, 25,  46080, 1));
  _rates.push_back(Section71TableEntry(a, 2116800, 25,  84672, 1));
  _rates.push_back(Section71TableEntry(a, 2304000, 25,  92160, 1));
  _rates.push_back(Section71TableEntry(a, 4233600, 25, 169344, 1));
  _rates.push_back(Section71TableEntry(a, 4608000, 25, 184320, 1));

  // 50i pull up
  _rates.push_back(Section71TableEntry(av,    600, 24, 25000, 1000));
  _rates.push_back(Section71TableEntry(av,    625, 24, 26042, 1000));
  _rates.push_back(Section71TableEntry(av,    750, 24, 31250, 1000));
  _rates.push_back(Section71TableEntry(av,   1250, 24, 52083, 1000));
  _rates.push_back(Section71TableEntry(av,   1500, 24, 62500, 1000));
  _rates.push_back(Section71TableEntry(a, 1102500, 24, 45938, 1));
  _rates.push_back(Section71TableEntry(a, 1200000, 24, 50000, 1));
  _rates.push_back(Section71TableEntry(a, 2205000, 24, 91875, 1));
  _rates.push_back(Section71TableEntry(a, 2400000, 24, 100000, 1));
  _rates.push_back(Section71TableEntry(a, 4410000, 24, 183750, 1));
  _rates.push_back(Section71TableEntry(a, 4800000, 24, 200000, 1));
}

bool AllowedEditRateTable::IsInTable( const aafRational_t& editRate, bool isVideo ) const
{
  for(vector<Section71TableEntry>::const_iterator iter = _rates.begin();
      iter != _rates.end();
      ++iter)
  {
    if (iter->IsEquivalent(editRate, isVideo))
    {
      return true;
    }
  }

  return false;
}
 
// Round positive numbers <= 99999 to 5 digits, Round numbers >= 99999 to integers.
// This method is used to generate error messages not to test equivalence.
double AllowedEditRateTable::Round( double num ) const
{
    double digits = ceil( log10( num ) );
    
    double multiplier;
    
    if ( digits < 5 )
    {
        multiplier = pow( 10.0, 5.0 - digits );
    }
    else
    {
        multiplier = 1;
    }
    
    double allDigitsNum = num * multiplier;
    double extraDigitNum = allDigitsNum * 10;
    double roundAllDigitsNum = floor( allDigitsNum );
    double extraRoundAllDigitsNum = roundAllDigitsNum * 10;
    double roundExtraDigitNum = floor( extraDigitNum );
    if ( roundExtraDigitNum - extraRoundAllDigitsNum >= 5 )
    {
        roundAllDigitsNum++;
    }
    
    double roundedNum = roundAllDigitsNum/multiplier;
    
    return roundedNum;
}
   
} // end of namespace aafanalyzer

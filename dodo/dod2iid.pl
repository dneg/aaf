###############################################################################
#
# The contents of this file are subject to the AAF SDK Public
# Source License Agreement (the "License"); You may not use this file
# except in compliance with the License.  The License is available in
# AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
# Association or its successor.
# 
# Software distributed under the License is distributed on an "AS IS"
# basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
# the License for the specific language governing rights and limitations
# under the License.
# 
# The Original Code of this file is Copyright 1998-2001, Licensor of the
# AAF Association.
# 
# The Initial Developer of the Original Code of this file and the
# Licensor of the AAF Association is Avid Technology.
# All rights reserved.
#
###############################################################################

###############################################################################
# FILE:    dod2iid.pl
# AUTHOR:  Tom Ransdell, Avid Technology, Inc.
# CREATED: 2000-APR-22
# PURPOSE: 
#   Generate the midl equivalent IID const definition for platforms
# that do not support midl.
#
# perl dod2iid.pl AAFPulldown[.dod]
# perl dod2iid.pl *.dod
# perl dod2iid.pl AAFHeader AAFObject
#
###############################################################################

my $arg  = ''; # the current argument from @ARGV
my $name = ''; # the name to use to build the IID constant
my $file = ''; # the dodo file to open and parse

foreach $arg (@ARGV)
{
  # build the variable from the argv name
  $name = $arg;
  $name =~ s/\..*//;

  # make sure the file name ends in .dod
  $file = join('.', $name, 'dod');

  open (DODOFILE , "$file") or die "Could not open \"$file\".\n";
  while (<DODOFILE>)
  {
    if (/AD_DEFAULT_UUID/)
    {
      s/\s+//g; # strip all of the white space from the current line ($_)

      # split up macro and its arguments into separate variables
      ($macro, $d1, $d2, $d3, $d41, $d42, $d43, $d44, $d45, $d46, $d47, $d48) = split(/[(),]/);

      # build the midl equivalent const definition
      printf("const IID IID_I%s = {0x%s,0x%s,0x%s,{0x%s,0x%s,0x%s,0x%s,0x%s,0x%s,0x%s,0x%s}};\n\n\n" , $name, $d1, $d2, $d3, $d41, $d42, $d43, $d44, $d45, $d46, $d47, $d48);

      last; # "There Can Be Only One!"...we don't need to continue reading the file
    }
  }
  close (DODOFILE);
}

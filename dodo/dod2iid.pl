##################################################################### 
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
##################################################################### 

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

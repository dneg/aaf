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
#
# file: sync_copyright.pl
# author: Tom Ransdell
# created: 27-MAR-2001
# purpase:
#    Common tool to keep the contents of the copyright macros synchrozied with the
# contents copyright text file specified on the command line. This tool also
# serves as the single source for transformations 
#
# examples
# Update the base.mac file with the current copyright test:
# perl sync_copyright.pl --macro macros/base.mac --copyright CopyrightMessage.txt
#
# Print the copyright message with "C Style" comments:
# perl sync_copyright.pl --copyright CopyrightMessage.txt --c_style
#
# Print the copyright message with "C++ Style" comments:
# perl sync_copyright.pl --copyright CopyrightMessage.txt --cpp_style
#
# Print the copyright message with "IDL Style" comments:
# perl sync_copyright.pl --copyright CopyrightMessage.txt --idl_style
#
# Print the copyright message with "Shell Style" comments (without the GENERATED for AAF SDK line):
# perl sync_copyright.pl --copyright CopyrightMessage.txt --shell_style --nogen
#
###############################################################################


use Getopt::Long;

# Debugging flag
my $debug = 0;

# True if generation comment should be included (default).
my $gen = 1;

# The name of the macro file to be searched
my $macro_path = '';

# The name of file that is the source for the copyright text.
my $copyright_path = '';

# Command line options that control the type of the formatted copyright.
my $c_style = 0;
my $cpp_style = 0;
my $idl_style = 0;
my $shell_style = 0;

# Setup option tags and local variable assignment.
my %optctl = (
  "debug" => \$debug,
  "gen" => \$gen,
  "macro" => \$macro_path,
  "copyright" => \$copyright_path,
  "c_style" => \$c_style,
  "cpp_style" => \$cpp_style,
  "idl_style" => \$idl_style,
  "shell_style" => \$shell_style
);

# Get the options
my $result = GetOptions (\%optctl, "debug", "gen!", "macro:s", "copyright=s", "c_style", "cpp_style", "idl_style", "shell_style");
exit 1 if (!$result);

# The number of bytes in the largest line of the copyright message.
my $max_line_length = 0;

# Read the copyright file into a variable.
&read_copyright_message;




# State variable that holds a reference to the current line
# matching subroutine.
my $processLine = \&find_start_of_macro;

if ($macro_path ne '') {
  # Read the dodo macro file and substitute the appropriate copyright text
  # for the current macro definitions
  open (MACRO_HANDLE, "<$macro_path") or die "Macro file could not be found!\n";
  while (<MACRO_HANDLE>) {
    &$processLine; # Parse the current line.
  }
  close (MACRO_HANDLE);
} else {
  # Print out the copyright message with the requested 
  # commenting style.
  &print_copyright_message if ($debug);
  &print_c_style_copyright_message if ($c_style || $debug);
  &print_cpp_style_copyright_message if ($cpp_style || $debug);
  &print_idl_style_copyright_message if ($idl_style || $debug);
  &print_shell_style_copyright_message if ($shell_style || $debug);  
  &print_dodo_cpp_style_copyright_message if ($debug);  
  &print_dodo_idl_style_copyright_message if ($debug);  
}






#
# Exit this script. All subroutines are defined below
# the exit statement (this may not be necessary).
#
exit 0;


# Subroutine to read the AAF copywrite notice
sub read_copyright_message
{
  my $line_length = 0;
  open (COPYRIGHT_HANDLE, "<$copyright_path") or die "Copyright file could not be found!\n";
  
  if ($gen) {
    # Prepend the GENERATED file text...
    $copyright_message = "This file was GENERATED for the AAF SDK\n\n";
  }

  while (<COPYRIGHT_HANDLE>) {
    chomp;
    $line_length = length($_) - 1;
    $max_line_length = $line_length if ($line_length > $max_line_length);
    $copyright_message .= $_ . "\n";
  }
  close (COPYRIGHT_HANDLE);
}



#
# Filters to be applied the current line buffer (i.e. $_) to decarate the
# it with appropriate comment symbols.
#
sub shell_style
{
  s/^/\# /mg;
	s/^\# $/\#/mg; # remove trailing blanks new comments
  $line = '#' x ($max_line_length + 3);
  $_  =  $line . "\n#\n" . $_;
  $_ .= "#\n" . $line . "\n";
}

sub c_style
{
  s/^/\* /mg;
  s/^\* $/\*/mg; # remove trailing blanks new comments
  $line = '*' x ($max_line_length + 2);
  $_  =  "/" . $line . "\n*\n" . $_;
  $_ .= "*\n" . $line . "*/\n";
}

sub cpp_style
{
  s/^/\/\/ /mg;
  s/^\/\/ $/\/\//mg; # remove trailing blanks new comments
  $line = "//=" . '-' x $max_line_length . "=\n";
  $_  = $line . "//\n" . $_ . "//\n" . $line;
}

sub cpp_quote
{
  s/\"/\\\"/mg; # escape any double-quotes.
  s/^/cpp_quote\(\"/mg;    
  s/$/\"\)/mg;
  s/^\"\)$//mg; # clip off the last end-of-buffer match.
}

sub idl_style
{
  &cpp_style;  
  &cpp_quote;
}

sub dodo_style
{
  s/\\/\\\\/mg; # dodo processes '\' escape characters.
}

sub add_trailing_lines
{
  $_ .= "\n"
}

# Subroutine to print out the copyright message decarated with C style comments.
sub print_copyright_message
{
  print "TEXT_STYLE_BEGIN_DEBUG\n" if ($debug);
  print $copyright_message;
  print "TEXT_STYLE_END_DEBUG\n\n" if ($debug);
}


# Subroutine to print out the copyright message decarated with C style comments.
sub print_c_style_copyright_message
{
  print "C_STYLE_BEGIN_DEBUG\n" if ($debug);
  $_ = $copyright_message;
  &c_style;
  &add_trailing_lines;
  print;
  print "C_STYLE_END_DEBUG\n\n" if ($debug);
}


# Subroutine to print out the copyright message decarated with C++ style comments.
sub print_cpp_style_copyright_message
{
  print "CPP_STYLE_BEGIN_DEBUG\n" if ($debug);
  $_ = $copyright_message;
  &cpp_style;
  &add_trailing_lines;
  print;
  print "CPP_STYLE_END_DEBUG\n\n" if ($debug);
}


# Subroutine to print out the copyright message decarated with IDL style comments.
sub print_idl_style_copyright_message
{
  print "IDL_STYLE_BEGIN_DEBUG\n" if ($debug);
  $_ = $copyright_message;
  &idl_style;
  &add_trailing_lines;
  print;
  print "IDL_STYLE_END_DEBUG\n\n" if ($debug);
}


# Subroutine to print out the copyright message decarated with 
# shell style comments.
sub print_shell_style_copyright_message
{
  print "SHELL_STYLE_BEGIN_DEBUG\n" if ($debug);
  $_ = $copyright_message;
  &shell_style;
  &add_trailing_lines;
  print;
  print "SHELL_STYLE_END_DEBUG\n\n" if ($debug);
}


# Subroutine to print out the copyright message decarated with IDL style comments.
sub print_dodo_idl_style_copyright_message
{
  print "DODO_IDL_STYLE_BEGIN_DEBUG\n" if ($debug);
  $_ = $copyright_message;
  &idl_style;
  &dodo_style;
  &add_trailing_lines;
  print;
  print "DODO_IDL_STYLE_END_DEBUG\n\n" if ($debug);
}


# Subroutine to print out the copyright message decarated with cpp style comments.
sub print_dodo_cpp_style_copyright_message
{
  print "DODO_CPP_STYLE_BEGIN_DEBUG\n" if ($debug);
  $_ = $copyright_message;
  &cpp_style;
  &dodo_style;
  &add_trailing_lines;
  print;
  print "DODO_CPP_STYLE_END_DEBUG\n\n" if ($debug);
}


#
#

# Subroutine to process characters until the we match the
# starting line of a macro definition (that we care about)
sub find_start_of_macro
{
  printf STDOUT "%s", $_; # always print out the current line.
  
  if (/^#startm/) {
    if (/\.copyright-message/) {
      &print_dodo_cpp_style_copyright_message;
      # start looking for the end of the macro definition.
      $processLine = \&find_end_of_macro;
    }
    if (/\.idl-copyright-message/) {
      &print_dodo_idl_style_copyright_message;
      # start looking for the end of the macro definition.
      $processLine = \&find_end_of_macro;
    }
  }
}

# Subroutine to process characters until the we match the
# end line of a macro definition
sub find_end_of_macro
{
  # Skip lines until we find the end of the current macro definition.
  if (/^#endm/) {
    printf STDOUT "%s", $_;
    # start looking for the end of the macro definition.
    $processLine = \&find_start_of_macro;
  }
}

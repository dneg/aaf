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
# FILE:    GenObjectInterfaces.pl
# AUTHOR:  Tom Ransdell, Avid Technology, Inc.
# CREATED: 2000-OCT-29
# PURPOSE: 
#   For a given AAF object or (list of objects) produce a list of
# interfaces that it supports.
#
# perl GenerateObjectInterfaces.pl AAFPulldown[.dod]
# perl GenerateObjectInterfaces.pl *.dod
# perl GenerateObjectInterfaces.pl AAFHeader AAFObject
#
###############################################################################
# The script implements a simple state machine to parse dodo
# source files and produce a formatted list of interfaces that
# a given dodo object supports.
###############################################################################


require 5.002; # Needed for Class::Struct

use Class::Struct;


#
# Define a structure to represent an enum.
#
struct(ObjectStruct => [name => '$', parent => '$', ex => '$']);



# The hash that contains all of the ObjectStruct objects allocated
# during processing.
my %objects = ();

my $arg  = ''; # the current argument from @ARGV
my $name = ''; # the name to use to build the IID constant

foreach $arg (@ARGV) {
  # build the variable from the argv name
  $name = $arg;
  $name =~ s/\..*//;

  &processObjectName ($name);
  &printSupportedInterfaces ($name);
}




# Exit this script. All subroutines are defined below
# the exit statement (this may not be necessary).
exit;



# Subroutine to process the given object name: AAFObject, AAFClassDef, ...etc.
#
sub processObjectName 
{
  my ($name) = @_;
  my $file = ''; # the dodo file to open and parse
  my $obj;  
  # State variable that holds a reference to the current line
  # matching subroutine.
  my $processLine;

  local (*DODOFILE);
  
  # Terminate processing it we reach the root object.
  if (($name ne 'AAFRoot' && $name ne 'AAFMetaDictionary') && (!defined($objects{$name}))) {

    # make sure the file name ends in .dod
    $file = $name . '.dod';

    open (DODOFILE , "$file") or die "Could not open \"$file\".\n";
  
    # Initially we want to start out by searching for the first line 
    # that contains a parent module macro.
    $processLine = \&find_start_of_parent_module;
    
    LINE: while (<DODOFILE>)
    {
      # Skip any lines that contain a dodo comments.
      next LINE if (/\#c/ || /^\s*$/);

      # Parse the current line using the current $processLine subroutine.
      $processLine = &$processLine ($name, $processLine, $_);
      
      # Stop if we have finished processing the file...
      last if ($processLine eq \&finished_processing)
    }
    close (DODOFILE);
  }
}


# Subroutine to process characters until the we match the
# starting line of the parent module.
sub find_start_of_parent_module {
  my ($name, $processLine, $line) = @_;
  my $obj;  

  #print STDERR "# find_start_of_parent_module: ", $line, "\n";
  
  if (/\.parent-module/) {
    # Create an new object structure
    $obj = new ObjectStruct;
    $obj->name($name);
      
    # Save the new object in the hash table.
    $objects{$obj->name} = $obj;
 
    # Extract the parent module name from the next line.
    \&get_parent_module;
  }
  else
  {
    $processLine;
  }
}

# Subroutine to process characters to extract parent
# module name.
sub get_parent_module {
  my ($name, $processLine, $line) = @_;
  my $obj = $objects{$name};  

  $line =~ s/\s+//g; # strip all of the white space from the current line ($_)
  $line =~ s/\\+//g; # strip dodo escape characters from the current line ($_)
  
  #print STDERR "# get_parent_module: ", $line, "\n";

  # What's left should be the name of the parent object.
  $obj->parent($line);


  # Recursively process the parent class
  &processObjectName ($obj->parent);
  
  # Find the start of the class/com extention interface.
  \&find_start_of_class;
}


# Subroutine to process characters until the we match the
# starting line of one of the CLASS_BEGIN macros. If
# It is a com extension class (interface) then extra
# the name and save it with the corresponding object
# from the hash table.
sub find_start_of_class {
  my ($name, $processLine, $line) = @_;
  my ($macro, $access, $classEx);
  my ($obj);  

  #print STDERR "# find_start_of_class: ", $line, "\n";
  
  if (/CLASS_BEGIN/) {
  
    if (/AD_COM_EXT_CLASS_BEGIN/) {
      ($macro, $access, $classEx) = split(/[() ]/);
      
      $obj = $objects{$name};
      $obj->ex($classEx);
      
      #print STDERR "# find_start_of_class: ", $classEx, "\n";
    }
  
    # We are done. There are no more lines to process...
    \&finished_processing;
  }
  else
  {
    $processLine;
  }
}

# stub to indicate processing of the file has been finished.
sub finished_processing {
  my ($name, $processLine, $line) = @_;

  print STDERR "# WARNING: finished_processing should not be called!\n";

  \&finished_processing;
}


# Subroutine to process the given object name: AAFObject, AAFClassDef, ...etc.
#
sub printSupportedInterfaces {
  my ($name) = @_;
  
  if (($name ne 'AAFRoot' && $name ne 'AAFMetaDictionary') ) {
    $obj = $objects{$name};
    if (defined($obj) && ($obj->parent ne 'AAFRoot' || defined($obj->ex))) {
      print "//\n";	
      print "// Objects that implement I", $obj->name, " also implement the following interfaces:\n";
      
      if (defined($obj->ex)) {
        print "//  - ", $obj->ex, "\n";
      }
      
      while ($obj->parent ne 'AAFRoot') {
        $obj = $objects{$obj->parent};
        print "//  - I", $obj->name, "\n";

        if (defined($obj->ex)) {
          print "//  - ", $obj->ex, "\n";
        }
      }
      
      print "\n";
    }
  }
}

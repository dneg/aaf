##################################################################### 
# FILE:    GenerateObjectInterfaces.pl
# AUTHOR:  Tom Ransdell, Avid Technology, Inc.
# CREATED: 2000-OCT-29
# PURPOSE: 
#   
#
# perl GenerateObjectInterfaces.pl AAFPulldown[.dod]
# perl GenerateObjectInterfaces.pl *.dod
# perl GenerateObjectInterfaces.pl AAFHeader AAFObject
#
##################################################################### 


require 5.002;
use FileHandle;

use Class::Struct;


#
# Define a structure to represent an enum.
#
struct(ObjectStruct => [name => '$', num => '$', parent => '$', ex => '$']);



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
		# that contains an enum declaration.
		$processLine = \&find_start_of_parent_module;
		
LINE: while (<DODOFILE>)
		{
			# Skip any lines that contain a dodo comments.
			next LINE if (/\#c/);

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
# starting line of the parent module.
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
			print "// Objects that implement I", $obj->name, " also implement the following interfaces:\n";
			
			if (defined($obj->ex)) {
				print "//  - ", $obj->ex, " (extended interface)\n";
			}
			
			while ($obj->parent ne 'AAFRoot') {
				$obj = $objects{$obj->parent};
				print "//  - I", $obj->name, "\n";
			}
			
			print "\n";
		}
	}
}
#!/usr/bin/perl
# Program to summarise CVS history in a format suitable for HTML build page
# (c) Copyright AAF Association Inc 2001
#


use Time::Local;

$cvs2cl = $ENV{CVS2CL} or die "need Environment VAR CVS2CL to point at cvs2cl.pl";


# get current timestamp into CVS style '2001-8-22 12:15:00'

sub getTimeStamp
	{
	my ($sec,$min,$hour,$day,$mon,$year,$wday,$yday,$isdst) = localtime(time);
	$mon++;
	$year += 1900;
	$timeStamp = sprintf "%d/%02d/%02d %02d:%02d:%02d", ($year, $mon, $day, $hour, $min, $sec);
	}
	
sub writeTimeStamp
	{
	local($timestamp) = @_[0];
	open(TSTAMP, "> .timestamp") or die "Can't create .timestamp file";
	print TSTAMP "$timestamp\n";
	close TSTAMP;
	}

# Fetch last timestamp

sub getLastTimeStamp
	{
	if(!open(TSTAMP, ".timestamp"))
		{
		print "No existing Timestamp\n";
		$lastTimeStamp = "2000-1-1 00:00:00";
		}
	else {
		($lastTimeStamp) = <TSTAMP>;
		chomp $lastTimeStamp;
		close (TSTAMP);
		}
	}


#-----------------------------

getLastTimeStamp();
getTimeStamp();

print "Current Time $timeStamp\n";
print "Last    Time $lastTimeStamp\n";

# run the commmand

$lastTimeStamp = "-d\'\>$lastTimeStamp\'";
print "$cvs2cl -l $lastTimeStamp\n";
system($cvs2cl, "-l", $lastTimeStamp) == 0 or die "Can't run cvs2cl";

	open(LOG, "ChangeLog");
	(@logfile) = <LOG>;
	close (LOG);


# Save current timestamp

writeTimeStamp($timeStamp);

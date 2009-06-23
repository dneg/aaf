#!/usr/bin/perl -w
# $Id$
use strict;
use Getopt::Long;
use File::Basename;
use Data::Dumper;

use constant ERR_TIMEOUT => 1;
use constant ERR_SOURCE_PREP => 2;
use constant ERR_CONFIGURE => 3;
use constant ERR_BUILD => 4;
use constant ERR_TEST => 5;

# perform all CVS date operations in UTC
$ENV{TZ} = 'UTC';

# Command line options
my @opt_target;							# try a single target
my $opt_date;							# use specified date instead of latest change
my $opt_rebuild = 0;
my $opt_shortcut_copy = 0;				# don't remove and copy files
my $opt_no_sourceforge_update = 0;
my $opt_local_build = 0;
my $opt_config_file;
my $opt_show_targets = 0;
my $opt_help = 0;
my $opt_verbose = 0;
my $quiet;
my $getopt_res = GetOptions(
            'date=s', \$opt_date,
            'target=s', \@opt_target,
            'r', \$opt_rebuild,
            'f', \$opt_shortcut_copy,
            'n', \$opt_no_sourceforge_update,
            'l=s', \$opt_local_build,
            'c=s', \$opt_config_file,
            'show-targets', \$opt_show_targets,
            'q', \$quiet,
            'v', \$opt_verbose,
            'h', \$opt_help,
            'help', \$opt_help);

if ($opt_help or ! $getopt_res) {
	print "  --date=<cvs date>     Retrieve from CVS and build using specified date\n";
	print "                        rather than using latest CVS version e.g.\n";
	print "                          --date=2006/12/31\n";
	print "                          --date='2006/12/31 09:40:20'\n";
	print "  --target=<target>     specify target(s) in of building all configured targets,\n";
	print "                        use more than once for more targets e.g.\n";
	print "                          --target=i686Win-cl --target=i686Linux-rel\n";
	print "  -r                    rebuild even if existing working copy is up-to-date\n";
	print "  -f                    shortcut the removal, copy and setup of source tree\n";
	print "                        i.e. source tree is already on compile farm nodes\n";
	print "  -n                    no sourceforge update (use source tree as-is, don't update web)\n";
	print "  -l path               local build from specified source tree\n";
	print "                        i.e do not checkout CVS, do not update web page\n";
	print "  --show-targets        list the targets which would be built then exit\n";
	print "  -c config.dumper      read a different config file to the default in\n";
	print "                        \$HOME/.nightly_cvs_build.dumper\n";
	print "  -q                    quiet, fewer logging messages\n";
	print "  -v                    verbose, log remote commands\n";
	print "  -h                    help message\n";
	exit 0;
}


=head1 NAME

nightly_cvs_build.pl - script to automate nightly builds and generation of web page results

=head1 SYNOPSIS

perl nightly_cvs_build.pl

=head1 DESCRIPTION

B<nightly_cvs_build.pl> is a developer's tool to run nightly builds
across a variety of build hosts.  It has been designed to only require
readily available tools such as B<perl> and B<ssh> to execute builds
on remote hosts, collect the results and to create a static web page of
results.

Features include:

=over 4

=item *

Compile farm hosts are simple to setup.  All you need is a compiler
and sshd running.  No networked filesystems are used, instead scp
or rsync copies the source tree to the compile farm host.

=item *

Supports remote building with MS Visual C++ using the 'devenv' command.
A convenient way to remotely access the MS Windows host is to install
Cygwin with sshd running as a service.

=item *

Collection of results from multiple compile farms.  When the script is run from
different compile farms, results are merged into the single results table.  If
a developer has different hardware or compiler versions their results will
be included in a new column in the results table.

=item *

Compile farm hosts can be woken from suspension using Wake-On-Lan and reliably
used for unattended compilation.  After the build has finished a host can return
to idle and go back to sleep.

=back

There are two main usage patterns:

=over 8

=item B<nightly_cvs_build.pl>

Using the configuration in $HOME/.nightly_cvs_build.dumper run an
unattended build across the configured compile farm hosts by first
checking out the latest CVS changes, building, running the tests,
collecting the
results and publishing a web page.  The build state is stored using
perl Data::Dumper to a file on the web host, so that future runs
will preserve and update the results.

This usage can be automated using a cronjob.  A typical crontab
entry to run the script every morning at 0100 would be:
    00 01 * * * $HOME/bin/nightly_cvs_build.pl

=item B<nightly_cvs_build.pl -l path_to_local_source_tree>

Given a source tree, copy it to all configured compile farm hosts,
build, test and collect the results in log files and a summary on
the terminal.  No update of files on the web host is made.
This is very useful for developers to check their changes on
several local machines before committing.

=back

=head1 CONFIGURATION

The configuration file (default location $HOME/.nightly_cvs_build.dumper) is in
the format of a Perl hash containing all configuration variables.  The variables
are best described using an example.  The 'config' section sets the location
for the local source tree and the remote CVS and web host paths.


  'config' => {
       # parent directory where CVS source tree will be created or updated
       tree => '/home/stuartc/source',

       # CVS module name
       cvs_module => 'AAF',

       # build only a subdirectory of a csv module, not the whole tree
       cvs_subdir => 'libMXF'

       # command for accessing CVS (in this example a socks proxy is used)
       cvs_command => 'env CVS_RSH=ssh socksify cvs -z3 -d:ext:stuart_hc@aaf.cvs.sourceforge.net:/cvsroot/aaf',

       # command to update source tree on compile farm hosts
       rsync_command => 'rsync -aiv --delete -e ssh'

       # Web host destination for generated html and build state
       scp_web_dest => 'stuart_hc@shell.sf.net:/home/groups/a/aa/aaf/htdocs/build'
   },

All other entries will describe a single compile farm host target.
E.g.

  'x86_64Linux' => {
       host=>'igor',         # hostname
       prefix=>'nightly',    # directory on remote host to store build tree
       wakeup=>'pilot35',    # hostname or ethernet address for wake-on-lan command
       copy=>'rsync',        # use rsync instead of simple copy command 'cp' to update  source tree

                             # 'tarball' will package up a snapshot of binaries or libraries
                             # which will be uploaded to the web site
       tarball=>'cd AAFi686LinuxSDK/g++/bin/debug && tar cf - libcom-api.so aafext | gzip'
   },

  'x86_64Linux-rel' => {
       host=>'igor',
       prefix=>'nightly',
       aafopts => q{AAFTARGET=Release},  # args passed to make command for Release
   },

  'i686msdev' => {
       host=>'Administrator@pilot60',    # remote username is different to local username
       prefix=>'nightly',
       init=>'. MSVC8_env_vars.sh ;'     # command to run before 'make' e.g. to setup environment variables
       # A special case is needed for MS Visual C++ using 'devenv'
       aafopts => q{. $HOME/vc71_vars.sh; devenv AAFWinSDK\\\\AAFWinSDK.sln /build Debug /project "Everything" /out devenv.log; cat devenv.log},
   },


=head1 BUILD STATE

The build state file stores the results of all previous builds in Data::Dumper format.
It is stored at $config{scp_web_dest}/build_state_$config{cvs_module}.dumper on the
remote host.  Before each build, unless it is a local build with the -l option, the 
previous build state is read so that new build results can be merged into the
previous results and an updated table of results can be generated.

The first time nightly_cvs.build.pl is run, an initial build state file can be
a plain text file consisting of one character, the digit 1.


=head1 INTERNAL DESIGN DETAILS

The parent process forks creating multiple children: one for each build.
Parent waits with timeout for children to return
  - if any child returns error 1 or does not return in time, ignore build
  - if child returns 0 or > 1 return, process build & test logs to report

Each child will run the build and log output to file on local filesystem.
Each child will use ssh command to run remote builds and read all output over
the ssh link.
Each child will exit with an exit status as follows
  0 build and test succeeded
  1 timeout, either network related or build took much too long
  2 preparation failed (autoreconf && make dist && scp / rm -rf && scp)
  3 ./configure failed
  4 make failed
  5 test failed

Results are collated, HTML formatted and scp'd to sourceforge web site.

Remote execution for MS Windows builds is a little tricky:
  Cygwin's sshd is a reliable way to remotely "login" and execute shell
  commands in a cygwin environment.
  To get a MinGW environment (MinGW gcc, ld etc) a remote cygwin shell can
  be used as follows:
    $ c:/msys/1.0/bin/sh --login -c "make && make check"
    or
    $ c:/msys/1.0/bin/sh --login -c "./configure && make"

=head1 AUTHOR

Stuart Cunningham <stuart_hc@users.sourceforge.net> on behalf of the British Broadcasting Corporation.

=cut

my %farm = read_config_file($opt_config_file);

if (! exists $farm{config}) {
	die "'config' entry missing from farm configuration\n";
}
my %config = %{ $farm{config} };
delete $farm{config};				# stops spurious build attempts
my $scp = exists $config{scp_command} ? $config{scp_command} : 'scp';
my $rsync = exists $config{rsync_command} ? $config{rsync_command} : 'rsync';

# Setup list of all targets to build and check config exists
my @farm_targets = sort keys %farm;
if (scalar @opt_target) {
	@farm_targets = @opt_target;
}
for my $k (@farm_targets) {
	if (! exists $farm{$k}) {
		die "Target $k has no configuration";
	}
}

if ($opt_show_targets) {
	for my $k (@farm_targets) { print "$k\n"; }
	exit 0;
}

my $cvs_proj = $config{cvs_module};

my $tree = $config{tree};	# source tree directory
my $tree_arg;				# arg to cvs to checkout tree e.g. branch tag

my $date;
my @to_build;
if (! $opt_local_build)
{
	my $date_path;
	if ($opt_date) {
		# change date-time to use '-' instead of '/' between date components
		$opt_date =~ s/\//-/g;

		$tree_arg .= " -D \"$opt_date\"";
		$date_path = date_to_filepath($opt_date);
		$tree .= "/$config{cvs_module}-$date_path";
	}
	else {
		$tree .= "/$config{cvs_module}-cvs-tip";
	}

	# update or checkout tree depending upon $opt_date and whether it exists
	if ($opt_date) {
		# checkout CVS tree
		chop(my $orig_pwd = `pwd`);
		chdir $config{tree} || die "chdir $config{tree} $!";
		my $dir = "$config{cvs_module}-$date_path";
		system("rm -rf $dir");
		my $output = cvs_retry(20, $config{cvs_command}, "-q checkout -D '$opt_date' -d $dir $config{cvs_module}");
		if (! defined $output)
			{ die "cvs checkout failed\n"; }
		chdir $orig_pwd;

		push @to_build, $tree;
		$date = $opt_date;
	}
	elsif (! $opt_no_sourceforge_update) {
		# Update CVS tree or checkout fresh copy if tree does not exist.
		# The date of last change (if any) is returned in the 3rd arg.
		if (update_tree($tree, $config{cvs_subdir}, $tree_arg, \$date)) {
			push @to_build, $tree;
		}
	}

	# Force build if -r or -n option set
	if (($opt_rebuild  || $opt_no_sourceforge_update) && ! @to_build) {
		push @to_build, $tree;

		my $log_tree = $tree;
		$log_tree .= "/$config{cvs_subdir}" if defined $config{cvs_subdir};
		$date = read_date_from_cvs_log($log_tree);
	}

	if (! @to_build) {
		print "Nothing to build\n";
		exit 0;
	}
}
else	# local build only
{
	$tree = $opt_local_build;
	push @to_build, $tree;
	$date = 'local';
}

if (! defined $date) {
	print "date undefined";
	exit 0;
}

my $date_path = date_to_filepath($date);

my $log_prefix = "log/$date_path-";

print "to build for \"$date\": ", join(' ', @to_build), "\n";

##################
# fork and monitor
##################

# Load the results from the last build state unless local build
# Results will be updated by when a build status is returned
my $internal_build_state_file = "/tmp/build_state_$cvs_proj.dumper";
my (%build_state);
%build_state = load_build_state();

my %running_children;					# stores all child pids
for my $tree (@to_build)				# eg. AAF-cvs-tip, cvs-rosebud
{
	if (! prepare_build($tree))
	{
		warn "Could not prepare $tree for building\n";
		next;
	}

	# Ensure log directory exists if needed
	if ($log_prefix =~ /\//) {
		my ($dir) = $log_prefix =~ /(.*)\//;
		my $logpath = "$tree/$dir";
		if (! -d $logpath) {
			mkdir $logpath or die "$logpath: $!\n";
		}
	}

	# Fork a child process for each build
	for my $build (@farm_targets)		# eg. i686Linux-release Win-debug
	{
		my $pid = fork();
		die "fork: $!" if !defined $pid;
		if ($pid == 0)		# child
		{
			# modify $0 to identify children in a ps listing
			$0 .= " (child) $tree $build";

			# run a single build then exit
			my $res = build($tree, $build);
			exit $res;
		}
		else				# parent
		{
			# update build state
			$build_state{$date}{$build}{pid} = $pid;
			$build_state{$date}{$build}{tree} = $tree;
			$build_state{$date}{$build}{date} = $date;
			$build_state{$date}{$build}{status} = undef;

			$running_children{$pid} = $build;
		}
	}
}

if (! keys %{ $build_state{$date} })
{
	# This happens when none of the selected builds can start
	print "No builds started\n";
	exit 1;
}

printf "spawned %d children, waiting for reports...\n", scalar(keys %running_children);
for my $b (sort values %running_children) {
	printf "    %d %s\n", $build_state{$date}{$b}{pid}, $b;
}
printf "\n";

# Wait for all child processes to finish.
# The eval {} block is necessary according to "perldoc -f alarm".
eval
{
	local $SIG{ALRM} = sub { die "alarm\n" }; # NB: \n required (see perldoc -f alarm)
	alarm 60*60*6;		# 6 hours should be long enough for build and test

	while (keys %running_children)
	{
		my $child_pid = wait();

		# get the build name for this pid
		my $key = $running_children{$child_pid};

		# cleanup data about child
		delete $running_children{$child_pid};
		delete $build_state{$date}{$key}{pid};

		my $status = $? >> 8;
		print "child_pid=$child_pid tree=$build_state{$date}{$key}{tree} build=$key status=$status\n";
		$build_state{$date}{$key}{status} = $status;

		# we have this info for a key such as 'i686Linux-rel'
		#	{tree}		AAF-cvs-tip
		#	{status}	0, 1, 2, 3, 4, 5
		save_build_state(\%build_state);
	}
	alarm 0;
};
# Record any timed-out builds
if ($@)
{
	die $@ unless $@ eq "alarm\n";   # propagate unexpected errors

	print "timeout occured\n";
	print "  children: ", join(',', sort keys %running_children), "\n";
	for my $pid (sort keys %running_children)
	{
		my $key = $running_children{$pid};
		$build_state{$date}{$key}{status} = ERR_TIMEOUT;

		print "  $pid: $date-$key,";

		# clean up timed-out build
		kill("TERM", $pid);
		sleep 60;
		if (kill(0, $pid) == 0)
			{ print "process $pid killed\n"; }
		else
			{ print "process $pid didn't die\n"; }
	}
}
save_build_state(\%build_state);

# Now copy results up to sourceforge.
# TODO: A results summary should be emailed to aaf-commits.
if (! $opt_local_build) {
	save_build_state_as_webpage(\%build_state, "/tmp/nightly_$cvs_proj");
}

exit 0;


##########################################################################
#
# Functions

sub read_config_file
{
	my ($arg) = @_;

	my $file = "$ENV{HOME}/.nightly_cvs_build.dumper";
	$file = $arg if defined $arg;

	my $str = `cat $file`;
	if ($? != 0)
		{ die "Could not read config file: $file"; }

	# Evaluate Data::Dumper written string.
	# Security warning: anything can happen during the eval
	# so don't use files from an untrusted source.
	my $p;
	{
		no strict;
		$p = eval $str;
		if ($@)
			{ die "error loading config file - eval returned: $@"; }
	}

	if (! defined $p) {
		die "error loading config file: could not evalutate string";
	}

	return %{ $p };
}

sub load_build_state
{
	my $file;
	my $str;

	if ($opt_local_build) {
		$file = $internal_build_state_file;
	}
	else {
		$file = "/tmp/build_state_$cvs_proj.dumper";
		system("$scp $config{scp_web_dest}/build_state_$cvs_proj.dumper $file");
		if ($? != 0) { die "scp build state file failed (\$?=$?) $config{scp_web_dest}/build_state_$cvs_proj.dumper $file\n" }
	}

	$str = `cat $file`;
	if ($? != 0) {
		return () if $opt_local_build;
		die "Could not load build state: $file";
	}

	# Evaluate Data::Dumper written string.
	# Security warning: anything can happen during the eval
	# so don't use files from an untrusted source.
	my $p;
	{
		no strict;
		$p = eval $str;
		if ($@)
			{ die "error loading build state - eval returned: $@"; }
	}

	if (! defined $p) {
		die "loading build state failed: could not evalutate build state string";
	}

	return %{ $p };
}

sub save_build_state
{
	my ($p) = @_;
	my $file = $internal_build_state_file;

	if (! open(STATE, ">$file"))
		{ warn "saving build state - $file: $!"; return }

	print STATE Dumper($p);

	close(STATE);
}

sub css_class
{
	my ($state) = @_;
	my %class = (
			0 => 'green',
			1 => 'gray',
			2 => 'gray',
			3 => 'red',
			4 => 'red',
			5 => 'yellow',
			);
	return defined $state ? $class{$state} : 'gray';
}

sub save_build_state_as_webpage
{
	my ($p, $file) = @_;

	my @logfiles;
	my @tarballs;

	if (! open(OUT, ">$file"))
		{ warn "saving build state - $file: $!"; return }

	# Make list of all build targets to use in first row of table
	# Not all dates will have all targets, so search all results
	my %alltargets;
	for my $date (sort {$b cmp $a} keys %{ $p })
	{
		for my $targ (keys %{ $p->{$date} }) {
			# remove "-rel" so we get only one label for the target
			$targ =~ s/-rel$//;
			$alltargets{$targ} = 1;
		}
	}

	# Sort targets by their platform importance to make results table easier to read
	my @sorted_targets = sort by_platform_importance keys(%alltargets);

	# Create the headings of the table
	print OUT q{<TR> <TH rowspan="2" valign="bottom">snapshot or tag (times are UTC)};
	for my $key (@sorted_targets) {
		print OUT qq{\t<TH colspan="2">$key\n};
	}
	print OUT q{<TR> };
	for my $key (@sorted_targets) {
		print OUT "\t<TH>Dbg<TH>Rel\n";
	}

	# Construct the table
	for my $date (sort {$b cmp $a} keys %{ $p })
	{
		print OUT "<TR> <TD>$date\n";

		my $dp = $p->{$date};

		for my $key (@sorted_targets)
		{
			if (! exists $dp->{$key})
			{
				print OUT "\t" . q{<TD class="} . css_class(1) . q{">} . "\n";	# Dbg
				print OUT "\t" . q{<TD class="} . css_class(1) . q{">} . "\n";	# Rel
				next;
			}

			for my $rel ( ('', '-rel') )
			{
				my $rkey = $key . $rel;

				my $lasttree = $dp->{$rkey}{tree} if exists $dp->{$rkey}{tree};

				print OUT "\t";
				print OUT	q{<TD class="} . css_class($dp->{$rkey}{status}) . q{">};

				if (defined $dp->{$rkey}{status}) {
					my $date_path = date_to_filepath($date);
					$date_path =~ s/_*\(.*\)$//;		# remove annotation e.g. " (V112_DR1)

					# Can we give a link to make log? (status 1 or 2 mean no make log)
					if ($dp->{$rkey}{status} != 1 && $dp->{$rkey}{status} != 2)
					{
						print OUT	qq{<a href="log/$date_path-make-$rkey.log.gz">M</a>};
						push @logfiles, "$lasttree/log/$date_path-make-$rkey.log";
					}

					# Link to make-check log and libs tarball if status indicates one should be available
					if ($dp->{$rkey}{status} == 0 || $dp->{$rkey}{status} == 5)
					{
						print OUT	qq{ <a href="log/$date_path-check-$rkey.log.gz">T</a>};
						push @logfiles, "$lasttree/log/$date_path-check-$rkey.log";

						my $tarball = "log/$date_path-libs-$rkey.tar.gz";
						if (-e "$lasttree/$tarball") {
							print OUT qq{ <a href="$tarball">&nabla;</a>};
							push @tarballs, "$lasttree/$tarball";
						}
					}
				}
				print OUT "\n";
			}
		}
		print OUT "\n";
	}
	close(OUT);

	if ($opt_no_sourceforge_update) {
		print "Not updating sourceforge\n";
		return;
	}

	print "Copying build state to sourceforge...\n";
	system("$scp $internal_build_state_file $config{scp_web_dest}/");
	if ($? != 0)
		{ warn "scp build state failed\n" }

	@logfiles = grep { -e $_ } @logfiles;		# get existing logfiles
	if (@logfiles) {
		print "Compressing log files...\n";
		# create list of uncompressed logs which exist
		my $logfiles = join(' ', @logfiles);
		system("gzip -f $logfiles");
		if ($? != 0)
			{ warn "gzip log files failed\n" }

		print "Copying log files to sourceforge...\n";
		my @gz_logfiles = map { $_ . '.gz' } @logfiles; 	# modify each logfile to add '.gz'
		my $gz_logfiles = join(' ', @gz_logfiles);
		system("$rsync $gz_logfiles $config{scp_web_dest}/log");
		if ($? != 0)
			{ warn "scp log files failed\n" }
	}

	if (@tarballs) {
		print "Copying (rsync) tarballs to sourceforge...\n";
		system("$rsync @tarballs $config{scp_web_dest}/log");
		if ($? != 0)
			{ warn "scp tarballs failed\n" }
	}

	print "Copying html table to sourceforge...\n";
	system("$rsync $file $config{scp_web_dest}/nightly_trunk_rows");
	if ($? != 0)
		{ warn "scp html table failed\n" }
	print "sourceforge updated\n";
}

sub by_platform_importance
{
	# Sort comparison function to sort by "platform inportance".
	#
	# Convert platform into score representing importance where
	# lower score means higher in importance (sorted to front of list)
	my ($a_score, $b_score) = (0, 0);

	# Least important is Apple platform
	$a_score = -16 if $a =~ /darwin/i;
	$b_score = -16 if $b =~ /darwin/i;

	# then Linux platform
	$a_score = -32 if $a =~ /linux/i;
	$b_score = -32 if $b =~ /linux/i;

	# then Win platform, MSVC compiler first
	$a_score = -64 if $a =~ /\bWin|\dWin|VC[\d]/i;
	$b_score = -64 if $b =~ /\bWin|\dWin|VC[\d]/i;

	# Visual C compiler environment is most important
	$a_score = -128 if $a =~ /msdev|devenv|vcexpress/i;
	$b_score = -128 if $b =~ /msdev|devenv|vcexpress/i;

	my $r = $a_score <=> $b_score	# compare by score
			|| $a cmp $b;			# then alphanumeric sort if score equal

	return $r;
}

##################
# ssh remote build
##################
sub build_type
{
	my ($tree) = @_;
	return 'autoconf' if -e "$tree/configure.in" or -e "$tree/configure.ac";
	return 'make';
}

sub prepare_build
{
	my ($tree) = @_;

	#return 1 if 1;

	if (build_type($tree) eq 'autoconf')
	{
		printf "Preparing autoconf distribution in $tree\n";
		system("(cd $tree && (./bootstrap || ./bootstrap.sh) && ./configure && make dist) > /dev/null 2>&1");
		return 0 if ($? != 0);
	}
	return 1;
}

sub build
{
	# e.g. tree = /dp_sources/AAF-cvs-tip, build = i686Linux
	my ($tree, $build) = @_;

	my $name = basename($tree);
	my $path = "$farm{$build}{prefix}/$name-$build";
	my $host = $farm{$build}{host};

	my ($init, $options) = ('', '');
	if (exists $farm{$build}{init})
		{ $init = $farm{$build}{init} }
	if (exists $farm{$build}{options})
		{ $options = $farm{$build}{options} }

	# wake up machine if necessary
	if (exists $farm{$build}{wakeup}) {
		system("wol $farm{$build}{wakeup}");
		if ($? != 0) {
			print "wol $farm{$build}{wakeup} failed\n";
		}
		else {
			my $max = 5;
			my $hostup = 0;
			for my $i (1 .. $max) {
				system("ping -c 1 $host > /dev/null");
				if ($? == 0) {
					print "Host $host is up\n";
					$hostup = 1;
					last;
				}
			}
			if (! $hostup) {
				print "Host $host is not up after $max attempts - aborting build\n";
				return ERR_TIMEOUT;
			}
		}
	}

	if (build_type($tree) eq 'autoconf')
	{
		# Prepare target disk space
		qssh($host, '', "rm -rf $path") || return ERR_SOURCE_PREP;
		qssh($host, $init, "mkdir $path") || return ERR_SOURCE_PREP;

		my ($distfile, $unpackdir) = get_last_distfile($tree);
		return ERR_SOURCE_PREP unless defined $distfile;
		my $basedist = basename($distfile);

		qscp($build, $distfile, $host, $path) || return ERR_SOURCE_PREP;

		qssh($host, $init, "cd $path && gzip -dc $basedist | tar xf -") || return ERR_SOURCE_PREP;

		qssh($host, $init, "cd $path/$unpackdir && ./configure $options", "> $tree/$log_prefix$build-make.log 2>&1") || return ERR_CONFIGURE;

		qssh($host, $init, "cd $path/$unpackdir && make", ">> $tree/$log_prefix$build-make.log 2>&1") || return ERR_BUILD;

		qssh($host, $init, "cd $path/$unpackdir && make check", "> $tree/$log_prefix$build-check.log 2>&1") || return ERR_TEST;
	}
	elsif (build_type($tree) eq 'make')
	{
		if (! $opt_shortcut_copy) {
			# Copy source tree
			qscp($build, $tree, $host, $path) || return ERR_SOURCE_PREP;
		}

		# subdir is an optional subdir to limit what is built
		$path .= "/$config{cvs_subdir}" if exists $config{cvs_subdir};
		my $make_opts = '';
		$make_opts = $farm{$build}{make_opts} if exists $farm{$build}{make_opts};
		my $make_command = "make $make_opts";
		my $check_command = "make $make_opts check";
		$make_command = $farm{$build}{make_command} if exists $farm{$build}{make_command};
		$check_command = $farm{$build}{check_command} if exists $farm{$build}{check_command};

		print "Building path=$path\n";
		qssh($host, $init, "cd $path && $make_command", "> $tree/${log_prefix}make-$build.log 2>&1") || return ERR_BUILD;

		# Create a binary tarball containing a binary release
		if (exists $farm{$build}{tarball}) {
			qssh($host, $init, "cd $path && $farm{$build}{tarball}", "> $tree/${log_prefix}libs-$build.tar.gz");
		}

		# Kludge for AAF SDK to prevent ScaleTest taking a very long time
		if ($cvs_proj eq 'AAF') {
			# Until we have an environment variable to turn off ScaleTest for nightly builds
			# turn off test by modifying makefile (this could surprise users though).
			qssh($host, $init, "cd $path && perl -p -i -e s/..APP_RUN_ENV./true/ test/com/ScaleTest/GNUmakefile");
		}

		# Run tests after tarball is made since tests may fail
		qssh($host, $init, "cd $path && $check_command", "> $tree/${log_prefix}check-$build.log 2>&1") || return ERR_TEST;
	}
	return 0;
}

sub get_last_distfile
{
	my ($path) = @_;

	if (! opendir(DIR, $path))
		{ warn "opendir failed for $path: $!"; return }

	my $package;
	my $latest = -1;
	while (my $f = readdir(DIR))
	{
		next unless $f =~ /\.tar\.gz$|\.tar\.bz2$/i;

		my ($dev,$ino,$mode,$nlink,$uid,$gid,$rdev,$size,$atime,$mtime,$ctime,$blksize,$blocks) = stat("$path/$f");
		if ($mtime > $latest)
			{ $package = "$path/$f"; $latest = $mtime }
	}
	closedir(DIR);

	my $decomp = 'gzip';
	$decomp = 'bzip2' if ($package =~ /bz2$/i);
	if (! open(TARPIPE, "$decomp -dc $package | tar tvf - |"))
		{ warn "Could not open tar pipeline: $!"; return }
	my $unpackdir = <TARPIPE>;
	close(TARPIPE);
	chop($unpackdir);
	$unpackdir =~ s/.* //;
	$unpackdir =~ s/\/.*//;

	return ($package, $unpackdir);
}

sub qssh
{
	my ($host, $init, $cmd, $redirection) = @_;

	my $redir = '';
	$redir = $redirection if defined $redirection;

	my $command;

	# Kludge sourceforge compile farm hosts
	if ($host =~ /\.cf\.sf\.net/)
	{
		my ($cf_host) = ($host =~ /([a-zA-Z0-9-]+)\.cf/);
		# change host to cf-shell host
		$host =~ s/[a-zA-Z0-9-]+\.cf/cf-shell/;

		# add extra ssh to $cf_host
		$command = qq{socksify ssh $host ssh $cf_host "'$init $cmd'" $redir};
	}
	# Kludge MSYS shell via cygwin sshd
	elsif ($init =~ /\bsh\s+(?:--login\s+)?-c\b/)	# "sh -c" or "sh --login -c"
	{
		# if it looks like an MSYS 'sh -c ...' then add extra quoting
		$command = qq{ssh $host $init "'$cmd'" $redir};
	}
	else
	{
		$command = qq{ssh $host '$init $cmd' $redir};
	}

	print "executing \"$command\"\n" if $opt_verbose;

	system($command);
	if ($? != 0)
	{
		#warn "ssh failed: host=$host init=$init command=$command\n";
		return 0;
	}
	return 1;
}

sub qscp
{
	# E.g. (i686Linux, /dp_videoedit/AAF-cvs-tip, stuartc@igor, nightly/AAF-cvs-tip-SparcSolaris)
	my ($build, $src, $host, $dst) = @_;

	my $copy;
	# Use custom copy command (usually rsync) if set
	if (exists $farm{$build}{copy}) {
		$copy = $farm{$build}{copy};
		if ($copy eq 'rsync') {
			$copy = 'rsync -aiv --delete -e ssh';	# expand shorthand 'rsync'
		}

		# To make contents of src tree sync'd with dst
		# add trailing '/' if not present
		# E.g. src=AAF-cvs-tip dest=igor:nightly/AAF-cvs-tip-x86_64Linux
		$src .= '/' unless $src =~ m,/$,;

		print "Executing: $copy $src $host:$dst\n";
		system("$copy $src $host:$dst > /tmp/$$-$host-scp.log 2>&1");
		my $res = $? >> 8;
		if ($res == 0 || $res == 24) {
			# 0 means success
			# 24 means "file vanished" which happens when deleting UTF filename tests
			#system("rm /tmp/$$-$host-scp.log");
			return 1;
		}

		# Otherwise it failed, so display error
		system("cat /tmp/$$-$host-scp.log");
		return 0;
	}
	else {
		# Clean out old directory if any
		#
		# Note that rm -rf stalls for WIN32 targets so use cmd /c rmdir
		my $aafopts = '';
		$aafopts = $farm{$build}{aafopts} if exists $farm{$build}{aafopts};
		my $path = $dst;
		if ($aafopts =~ /msdev|devenv|vcexpress|VC\d|Win/i) {		# msdev or devenv build?
			my $qpath = $path;
			$qpath =~ s,/,\\\\,g;			# convert / to \\
			qssh($host, '', "cmd /c rmdir /s /q $qpath");
			qssh($host, '', "rm -rf $path") || return ERR_SOURCE_PREP;
		}
		else {
			qssh($host, '', "rm -rf $path") || return ERR_SOURCE_PREP;
		}

		$copy = 'scp -q -pr';

		# Kludge sourceforge compile farm hosts
		if ($host =~ /\.cf\.sf\.net/)
		{
			$host =~ s/[a-zA-Z0-9-]+\.cf/cf-shell/;
			$copy = "socksify $copy";
		}

		print "Executing: $copy $src $host:$dst\n";
		system("$copy $src $host:$dst > /tmp/$$-$host-scp.log 2>&1");
		if ($? != 0) {
			# Work around sung1 bug where commands that succeed return non-zero
			# but stderr is zero-size
			if ($host =~ /sung1|compression1/ && (-s "/tmp/$$-$host-scp.log") == 0)
			{
				system("rm /tmp/$$-$host-scp.log");
				return 1;
			}
			warn "scp failed: scp -q -pr $src $host:$dst\n";
			system("cat /tmp/$$-$host-scp.log");			# display exact scp error
			return 0
		}
		system("rm /tmp/$$-$host-scp.log");
		return 1;
	}
}

#################
# CVS interaction
#################

# TODO make cvs command robust against password prompt hanging process
# (or is this unnecessary since it will timeout?)
#
# FIXME cvs update will return non-zero if there is a conflict!
# ideally this function would remove the conflicts and retry
# or we can just practice safe trees which do not have conflicts
sub cvs_retry
{
	my ($retries, $cvs_command, $argstr) = @_;

	my $log = '';
	for my $c (1 .. $retries)
	{
		print "pwd=", `pwd`;
		print "Executing: $cvs_command $argstr\n";
		open(PIPE, "$cvs_command $argstr 2>&1 |") || die "pipe failed: $!";
		while (<PIPE>)
			{ $log .= $_ }
		close(PIPE);
		return $log if ($? == 0);	# success
		print $log;					# $log will contain cvs error message
		sleep 5;
	}
	return undef;
}

sub cvs_history
{
	my $history = cvs_retry(20, $config{cvs_command}, 'history -a -l -c');
	my @history = split(/\n/, $history);

	# sort by date, most recent first
	my @sorted_history = sort { substr($b, 2) cmp substr($a, 2) } @history;

	# return date of last update, in the form "YYYY-MM-DD HH:MM"
	my ($date) = ($sorted_history[0] =~ /^\S+\s+(\S+\s+\S+)/);
	return $date;
}

# update_tree
# cd to tree, perform CVS update -PAd, using optional -r branch
# or checkout new tree if it doesn't exist
#
# returns 0 for no updates found or update failed (conflicts, disk space etc)
# returns 1 for update found - i.e. need to rebuild
sub update_tree
{
	my ($tree, $subdir, $branch, $p_last_date) = @_;			# branch is optional

	my $result = 0;
	chop(my $orig_pwd = `pwd`);

	if (! -e $tree) {
		# no tree yet, so use checkout command
		if (! chdir(dirname($tree)))
			{ die "chdir: dirname($tree): $!\n"; }

		my $basename = basename($tree);
		my $output = cvs_retry(20, $config{cvs_command}, "-q checkout -d $basename $config{cvs_module}");
		if (! defined $output)
			{ die "cvs checkout failed\n"; }

		# chdir into newly checked out tree so that log command will work
		if (! chdir($tree))
			{ die "chdir: $tree: $!\n"; }

		$result = 1;		# indicate new build
	}
	else {
		# tree exists, use update command
		if (! chdir($tree))
			{ die "chdir: $tree: $!\n"; }

		# cd to subdir if specified
		if (defined $subdir && !chdir($subdir))
			{ die "chdir: $subdir: $!\n"; }

		# cvs update
		my $cvs_args = '-q update -PAd';
		$cvs_args .= " -r $branch" if defined $branch;

		my $cvs_update = cvs_retry(20, $config{cvs_command}, $cvs_args);
		if (! defined $cvs_update) {
			die "cvs update failed\n";
		}

		my @lines = split(/\n/, $cvs_update);
		my @new = grep(!/^\?/, @lines);			# ignore lines with '?'
		if (@new)
		{
			if (grep(/^C /, @new)) {	# conflicts mean broken tree
				die "cvs update found conflicts:\n" . join("\n", grep(/^C /, @new)) . "\n";
			}
			
			print join("\n", @new), "\n";
			$result = 1;
		}
	}

	if ($result) {
		# Something changed so update cvs log
		my $cvs_log = cvs_retry(20, $config{cvs_command}, 'log');
		if (defined $cvs_log)
		{
			# save cvs.log for later perusal
			if (! open(OUT, "> cvs.log"))
				{ warn "saving cvs.log failed: $!" }
			else
			{
				print OUT $cvs_log;
				close(OUT);
			}

			# work out the date of the last change
			my @sorted = sort( $cvs_log =~ /^date: (\d\d\d\d...............)/gm );
			my $last_date = $sorted[-1];
			# log file dates are in the form '2007/04/16 17:08:50'
			# For consistency with other uses, change '/' to '-'
			$last_date =~ s/\//-/g;
			$$p_last_date = $last_date;
		}
	}
	else
	{
		print "cvs update: nothing changed\n";
		$result = 0;
	}

	chdir($orig_pwd);
	return $result;
}

sub read_date_from_cvs_log
{
	my ($tree) = @_;

	my $logfile = "$tree/cvs.log";
	if (! open(IN, $logfile)) {
		die "$logfile: $!";
	}

	my $cvs_log = '';
	while (<IN>) {
		$cvs_log .= $_ if /^date: /;
	}
	close(IN);

	# work out the date of the last change
	my @sorted = sort( $cvs_log =~ /^date: (\d\d\d\d...............)/gm );
	my $last_date = $sorted[-1];
	# log file dates are in the form '2007/04/16 17:08:50'
	# For consistency with other uses, change '/' to '-'
	$last_date =~ s/\//-/g;

	return $last_date;
}

sub date_to_filepath
{
	my ($date_time) = @_;
	$date_time =~ s/\//-/g;			# convert '/' to underscore
	$date_time =~ s/[ :]/_/g;		# convert ' ' or ':'  to underscore
	return $date_time;
}

#!/usr/bin/perl
use Getopt::Long;
use strict;
use Env;

sub getTime{
	my @months = qw(Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec);
	my @weekDays = qw(Sun Mon Tue Wed Thu Fri Sat Sun);
	my ($second, $minute, $hour, $dayOfMonth, $month, $yearOffset, $dayOfWeek, $dayOfYear, $daylightSavings) = localtime();
	my $year = 1900 + $yearOffset;
	my $theTime = "$hour:$minute:$second, $weekDays[$dayOfWeek] $months[$month] $dayOfMonth, $year";
	return $theTime;

}

sub execCommand {
	my $retval;
	my ($command) = @_;
	print LOGFILE getTime() ."\t\t". $command . "\n";
	print "Exec at " . getTime() . " $command...\n";
	$retval  = `$command`;
	if ( $? == -1 ) {
		print "command failed: $!";
	}
	chomp $retval;
	print $retval . "\n";
	return $retval;
}

sub usage{
	print ( "Usage:
		--map_list <filename>      : A short text file containing the absolute filenames (one per line) of all the bam files you would like CNVer to use.
		--ref_names <filename>     : file with names of chromosomes of interest (default: allchr.txt in ref_folder directory, with all the autosomes)

		Note:
		1. Please give all filenames in absolute paths\n");
	exit;
}


my %options=();
my $dummy;
my $retval;
my $i;


GetOptions ('map_list=s' => \$map_list, 'ref_folder=s' => \$ref_folder, 'ref_names=s' => \$ref_name_file, 'read_len=i' => \$read_len, 'mean_insert=i' => \$mean_insert,
	'mem_lim=i' => \$mem_lim, 'ref_single=s' => \$ref_single ) or usage();



open ( LOGFILE, ">>log.txt") or die ("$0 : failed to open log file for output: $!\n");
my $paramLine = "";
foreach (@ARGV) { $paramLine .= "$_ " }
print LOGFILE getTime() . "\t called with $paramLine\n";


getopts("p:S:f:t:", \%options) or die();
if (defined $options{S}) {
	$maxMem = $options{S};
} else {
	$maxMem = 1000000000;
}
if (defined $options{f}) {
	$inFilename = $options{f};
} else {
	die "Input file (-f option) must be specified.\n";
}

print  stdout "\nUsing parameters:\n\tNumber of threads: $numThreads \n\tMaximum memory: ";
print  stdout "$maxMem \n\tFile to sort: $inFilename \n\tTemp dir: $tmpDir \n\n";

#my $numLines;
#execCommand("rm -Rf $tmpDir");
#$retval = execCommand("wc $inFilename");
#($numLines, $dummy, $numBytes, $dummy) = split " ", $retval;
#my @files = split "\n", execCommand("ls $tmpDir");
#foreach my $file (@files) {
#my @curfiles = 0 .. scalar(@files) - 1;
#my $curindex = scalar(@files);
#while (scalar(@curfiles) > 1) {
#for ($i = 0; $i < scalar(@curfiles); $i++) {
#       if ($i == $#curfiles) { #last element
#           push (@newfiles, $curfiles[$i]); #change the number here?




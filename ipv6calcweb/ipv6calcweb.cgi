#!/usr/bin/perl -w -T
#
# Project    : ipv6calc
# File       : ipv6calcweb.cgi
# Version    : $Id: ipv6calcweb.cgi,v 1.3 2002/03/18 22:07:02 peter Exp $
# Copyright  : 2002 by Peter Bieringer <pb (at) bieringer.de>
# License    : GPL, but copyright always has to be displayed in output
#
# Simple Perl web interface and security wrapper
#  It's too dangerous to call the binary 'ipv6calc' directly...
#
# Todo: more functions by query string...

### Uses environment variables:
## Created by http server before invoking CGI:
#  REMOTE_ADDR    : remote client address
#  REMOTE_HOST    : remote client name (DNS resolved)
#  HTTP_USER_AGENT: user agent string
#  SERVER_ADDR    : local server address
#  SERVER_NAME    : local server name (by http server config)
#  QUERY_STRING   : for language setting
#  SERVER_PROTOCOL: check for INCLUDED (called by SSI)
#    Currently supported: "lang=$lang" with $lang in @supported_languages

use strict;

## Defines

# Debug value
#my $debug = 0xffff;
my $debug = 0x0;

# Location of binary
my $bin_ipv6calc = "/bin/ipv6calc";
my $options_ipv6calc = "-m -i -q";

# Whois server url
my $url_ipv4_whoisserver="http://uwhois.com/cgi/whois.cgi?query=";

#my $lang_default = "de";
my $lang_default = "en";

my $lang = $lang_default;

## Output format: text, html, htmlfull
#my $outputformat = "text";
#my $outputformat = "html";
my $outputformat = "htmlfull"; # switched to "html", if called by SSI

## Output type
# full = with description
# simple = without description
#my $outputtype = "full"
my $outputtype = "simple";

## Text
# Language
my @supported_languages = ( "de", "en");

# Tokens to be formatted usin <TT>
my @format_tt = ( "EUI48", "EUI64", "IPV6", "IPV4", "SLA", "IID" );

my %text = (
	'EUI48' => {
		'de' => "EUI-48 Identifizierungsnummer (MAC Adresse)",
		'en' => "EUI-48 identifier (MAC address)",
	},
	'EUI64' => {
		'de' => "EUI-64 Identifizierungsnummer",
		'en' => "EUI-64 identifier",
	},
	'IPV6' => {
		'de' => "IPv6 Adresse",
		'en' => "IPv6 address",
	},
	'IPV4' => {
		'de' => "IPv4 Adresse",
		'en' => "IPv4 address",
	},
	'OUI' => {
		'de' => "Hersteller-Identifizierung der Netzwerkarte",
		'en' => "Vendor identification of network interface card",
	},
	'REMOTE' => {
		'de' => "Ihr Client",
		'en' => "Your client",
	},
	'SERVER' => {
		'de' => "Dieser Server",
		'en' => "This server",
	},
	'SLA' => {
		'de' => "Site Level Aggregator (Ihr Subnetz)",
		'en' => "Site Level Aggregator (your subnet)",
	},
	'IIDSCOPE' => {
		'de' => "Art der Interface-Identifierungsnummer",
		'en' => "Scope of interface identifier",
	},
	'IID' => {
		'de' => "Interface-Identifierungsnummer",
		'en' => "Interface identifier",
	},
	'TYPE' => {
		'de' => "Adresstyp",
		'en' => "Address type",
	},
	'NAME' => {
		'de' => "Reverse DNS Aufl�sung",
		'en' => "Reverse DNS resolution",
	},
	'title' => {
		'de' => "Adresstyp Information",
		'en' => "Addresstype information",
	},
	'nodata' => {
		'de' => "Keine Daten verf�gbar",
		'en' => "No data availabe",
	},
	'USERAGENT' => {
		'de' => "Browseridentifikation",
		'en' => "User agent identification",
	}
);

###### Normally nothing to change here

## Cleanup environment
# Please report, if more cleanup is needed on other systems

# Hardwire path to well known
if ( defined %ENV->{'PATH'} ) { %ENV->{'PATH'}="/bin:/usr/bin:/usr/local/bin"; };
# Clear shell environment
if ( defined %ENV->{'BASH_ENV'} ) { %ENV->{'BASH_ENV'}=""; };

## Fallbacks
if (! defined $outputformat) { $outputformat = "text" };
if (! defined $outputtype) { $outputtype = "simple" };
if (! defined $lang_default) { $lang_default = "en"};
if (! defined $lang) { $lang = $lang_default};

## Variables
my $addr_remote;
my $name_remote;
my $user_agent;
my $addr_server;
my $name_server;
my @info_remote;
my @info_server;
my %infoh_remote;
my %infoh_server;
my $length_max_key = 0;
my $length_max_description = 0;
my $query_string;

my $maxenvlength = 256;

my $ipv6calc_version = "";
my $ipv6calc_copyright = "(P) & (C) by Peter Bieringer";
my $ipv6calc_name = "ipv6calc";


############### Functions

## Error message
sub print_error ($) {
	my $message = shift;
	if ( defined $message ) {
		print $message . "\n";
	};
	exit 1;
};

## Print conditional html
sub print_tagoutput ($) {
	my $text = shift;
	if ( defined $text ) {
		if ($outputformat eq "html" || $outputformat eq "htmlfull") {
			print $text;
		};
	};
};

sub print_textonly ($) {
	my $text = shift;
	if ( defined $text ) {
		if ($outputformat eq "text") {
			print $text;
		};
	};
};

## Print one table part
sub print_infohash ($) {
	my $phash = shift;
	if (! defined $phash) { return; };

	my $flag_tt;

	if ( ! defined \$phash ) {
		&print_tagoutput ( "      <tr>\n" );
		&print_tagoutput ( "        <td colspan=\"3\">" );
		print %text->{'nodata'}->{$lang};
		&print_textonly ("\n");
		&print_tagoutput ( "</td>\n" );
		&print_tagoutput ( "</tr>\n" );
		return;
	};

	for my $key (sort keys %$phash) {
		# catch internal keys
		if ( $key =~ /^IPV6CALC_/ ) {
			if ( $key eq "IPV6CALC_COPYRIGHT" ) {
				$ipv6calc_copyright = %$phash->{$key};
				$ipv6calc_copyright =~ s/^\"//;
				$ipv6calc_copyright =~ s/\"$//;
			};
			if ( $key eq "IPV6CALC_VERSION" ) {
				$ipv6calc_version = %$phash->{$key};
				$ipv6calc_version =~ s/^\"//;
				$ipv6calc_version =~ s/\"$//;
			};
			if ( $key eq "IPV6CALC_NAME" ) {
				$ipv6calc_name = %$phash->{$key};
				$ipv6calc_name =~ s/^\"//;
				$ipv6calc_name =~ s/\"$//;
			};
			next;
		};

		$flag_tt = 0;

		if (grep(/^$key$/, @format_tt)) {
			$flag_tt = 1;
		};

		# print key
		&print_tagoutput ( "      <tr>\n" );
		&print_tagoutput ( "        <td><b>" );
		print $key;
		&print_textonly (' ' x ($length_max_key - length($key)) );
		&print_textonly (" | ");

		# print description
		if ($outputtype ne "simple") {
			&print_tagoutput ( "</b></td>\n" );
			&print_tagoutput ( "        <td>" );
			if (defined %text->{$key}->{$lang}) {
				print %text->{$key}->{$lang};
			};
			&print_textonly (' ' x ($length_max_description - length(%text->{$key}->{$lang})) );
			&print_textonly (" | ");
			&print_tagoutput ( "</td>\n" );
		};

		# print data
		&print_tagoutput ( "        <td>" );

		if ($flag_tt) {
			&print_tagoutput ( "<tt>" );
		};
		if ($key eq "IPV4") {
			&print_tagoutput ( "<a href=\"" . $url_ipv4_whoisserver . %$phash->{$key} . "\">" );
		};
		print %$phash->{$key};
		if ($key eq "IPV4") {
			&print_tagoutput ( "</a>" );
		};
		if ($flag_tt) {
			&print_tagoutput ( "</tt>" );
		};
		&print_tagoutput ( "</td>\n" );
		&print_tagoutput ( "      </tr>\n" );
		&print_textonly ("\n");
	};
};

############### Main
&print_tagoutput ("Content-type: text/html\n\n");
&print_textonly  ("Content-type: text/plain\n\n");

## Check for binary ipv6calc is executable
if ( ! -f $bin_ipv6calc ) {
	&print_error("Error: missing needed program");
};
if ( ! -x $bin_ipv6calc ) {
	&print_error("Error: missing needed program");
};

## Check type
if ( defined $ENV{'SERVER_PROTOCOL'} ) {
	if ( $ENV{'SERVER_PROTOCOL'} eq "INCLUDED" ) {
		if ( $outputformat eq "htmlfull" ) {
			# Switch back to included html
			$outputformat = "html";
		};
	};
};


## Get variables
if ( defined $ENV{'REMOTE_ADDR'} ) {
	$ENV{'REMOTE_ADDR'} =~ /^([[:xdigit:]\.:]*)$/;
	if ( ! defined $1 || (length($1) > $maxenvlength)) {
		&print_error("Error: problem with data");
	};
	$addr_remote = $1;
};

if ( defined $ENV{'REMOTE_HOST'} ) {
	$ENV{'REMOTE_HOST'} =~ /^([[:alnum:]\.-_]*)$/;
	if ( ! defined $1 || (length($1) > $maxenvlength)) {
		&print_error("Error: problem with data");
	};
	$name_remote = $1;
};

if ( defined $ENV{'HTTP_USER_AGENT'} ) {
	$ENV{'HTTP_USER_AGENT'} =~ /^([[:alnum:]\[\]\/\(\)\\\.\-+\;\:]*)$/;
	if ( ! defined $1 || (length($1) > $maxenvlength)) {
		# not a problem, skip it
	} else {
		$user_agent = $1;
	};
};

if ( defined $ENV{'SERVER_ADDR'} ) {
	$ENV{'SERVER_ADDR'} =~ /^([[:xdigit:]\.:]*)$/;
	if ( ! defined $1 || (length($1) > $maxenvlength)) {
		&print_error("Error: problem with data");
	};
	$addr_server = $1;
};

if ( defined $ENV{'SERVER_NAME'} ) {
	$ENV{'SERVER_NAME'} =~ /^([[:alnum:]\.-_]*)$/;
	if ( ! defined $1 || (length($1) > $maxenvlength)) {
		&print_error("Error: problem with data");
	};
	$name_server = $1;
};

if ( defined $ENV{'QUERY_STRING'} ) {
	$ENV{'QUERY_STRING'} =~ /^([[:alnum:]=]*)$/;
	if ( ! defined $1 || (length($1) > $maxenvlength)) {
		&print_error("Error: problem with data");
	};
	$query_string = $1;

	## Check for language in query string
	for my $langtest (@supported_languages) {
		if ($query_string eq "lang=" . $langtest) {
			$lang = $langtest;
			last;
		};
	};
};


## Get and fill information

if ( defined $addr_remote ) {
	@info_remote = `$bin_ipv6calc $options_ipv6calc $addr_remote`;
	if ( $? != 0 ) {
		&print_error("Error: problem retrieving data");
	};

	for my $line (@info_remote) {
		my ( $key, $content ) = split /=/, $line, 2;
		if ( (! defined $key) || (! defined $content) ) {
			&print_error("Error: problem parsing data");
		};
		chomp $content;
		%infoh_remote->{$key} = $content;
	};
	if (defined $name_remote) {
		if ($name_remote ne $addr_remote) {
			%infoh_remote->{'NAME'} = $name_remote;
		};
	};
	if (defined $user_agent) {
		%infoh_remote->{'USERAGENT'} = $user_agent;
	};
};

if ( defined $addr_server ) {
	@info_server = `$bin_ipv6calc $options_ipv6calc  $addr_server`;
	if ( $? != 0 ) {
		&print_error("Error: problem retrieving data");
	};
	for my $line (@info_server) {
		my ( $key, $content ) = split /=/, $line, 2;
		if ( (! defined $key) || (! defined $content) ) {
			&print_error("Error: problem parsing data");
		};
		chomp $content;
		%infoh_server->{$key} = $content;
	};
	if (defined $name_server) {
		if ($name_server ne $addr_server) {
			%infoh_server->{'NAME'} = $name_server;
		};
	};
};


## Print content
if ($debug & 0x01) {
	print "REMOTE\n";
	for my $key (keys %infoh_remote) {
		print " ". $key . "=" .  %infoh_remote->{$key} . "\n";
	};
	print "SERVER\n";
	for my $key (keys %infoh_server) {
		print " " . $key . "=" .  %infoh_server->{$key} . "\n";
	};
};


## Calculate max lengths
for my $key (keys %text) {
	if (length($key) > $length_max_key) {
		$length_max_key = length($key);
	};

	if (length(%text->{$key}->{$lang}) > $length_max_description) {
		$length_max_description = length(%text->{$key}->{$lang});

	};
};
	

## Print output
if ($outputformat eq "htmlfull") {
	&print_tagoutput ( "<html>\n" );
	&print_tagoutput ( "  <head>\n" );
	&print_tagoutput ( "    <meta name=\"Author\" content=\"Peter Bieringer\">\n" );
	&print_tagoutput ( "    <title>" );
	print %text->{'title'}->{$lang};
	&print_tagoutput ( "</title>\n" );
	&print_tagoutput ( "  </head>\n" );
	&print_tagoutput ( "  <body>\n" );

};

&print_tagoutput ( "    <table border CELLSPACING=0>\n" );

# Client
&print_textonly ("\n");
&print_tagoutput ( "      <tr>\n" );
&print_tagoutput ( "        <th colspan=\"3\">" );
print %text->{'REMOTE'}->{$lang};
&print_textonly ("\n");
&print_tagoutput ( "</th>\n" );
&print_tagoutput ( "      </tr>\n" );
&print_infohash (\%infoh_remote);


# Server
&print_textonly ("\n");
&print_tagoutput ( "      <tr>\n" );
&print_tagoutput ( "        <th colspan=\"3\">" );
print %text->{'SERVER'}->{$lang};
&print_textonly ("\n");
&print_tagoutput ( "</th>\n" );
&print_tagoutput ( "      </tr>\n" );
&print_infohash (\%infoh_server);

# Footer
&print_textonly ("\n");
&print_tagoutput ( "      <tr>\n" );
&print_tagoutput ( "        <td colspan=\"3\">" );
if ($outputformat eq "html" || $outputformat eq "htmlfull") {
	print "<font size=-2>Powered by <a href=\"http://www.bieringer.de/linux/IPv6/ipv6calc/\">" . $ipv6calc_name . "</a> " . $ipv6calc_version . ", " . $ipv6calc_copyright . "</font>";
} else {
	print "Powered by " . $ipv6calc_name . " " . $ipv6calc_version . ", " . $ipv6calc_copyright . " (http://www.bieringer.de/linux/IPv6/ipv6calc/)" . "\n";
};
&print_tagoutput ( "</td>\n" );
&print_tagoutput ( "      </tr>\n" );
&print_tagoutput ( "    </table>\n" );

if ($outputformat eq "htmlfull") {
	&print_tagoutput ( "  </body>\n" );
	&print_tagoutput ( "</html>\n" );
};

exit (0);

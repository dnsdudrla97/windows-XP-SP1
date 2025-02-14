# $Id: Protocol.pm,v 1.35 1999/08/02 22:57:09 gisle Exp $

package LWP::Protocol;

=head1 NAME

LWP::Protocol - Base class for LWP protocols

=head1 SYNOPSIS

 package LWP::Protocol::foo;
 require LWP::Protocol;
 @ISA=qw(LWP::Protocol);

=head1 DESCRIPTION

This class is used a the base class for all protocol implementations
supported by the LWP library.

When creating an instance of this class using
C<LWP::Protocol::create($url)>, and you get an initialised subclass
appropriate for that access method. In other words, the
LWP::Protocol::create() function calls the constructor for one of its
subclasses.

All derived LWP::Protocol classes need to override the request()
method which is used to service a request. The overridden method can
make use of the collect() function to collect together chunks of data
as it is received.

The following methods and functions are provided:

=over 4

=cut

#####################################################################

require LWP::MemberMixin;
@ISA = qw(LWP::MemberMixin);
$VERSION = sprintf("%d.%02d", q$Revision: 1.35 $ =~ /(\d+)\.(\d+)/);

use strict;
use Carp ();
use HTTP::Status ();
use HTTP::Response;
require HTML::HeadParser;

my %ImplementedBy = (); # scheme => classname


=item $prot = LWP::Protocol->new()

The LWP::Protocol constructor is inherited by subclasses. As this is a
virtual base class this method should B<not> be called directly.

=cut

sub new
{
    my($class) = @_;

    my $self = bless {
	'timeout' => 0,
	'parse_head' => 1,
    }, $class;
    $self;
}


=item $prot = LWP::Protocol::create($url)

Create an object of the class implementing the protocol to handle the
given scheme. This is a function, not a method. It is more an object
factory than a constructor. This is the function user agents should
use to access protocols.

=cut

sub create
{
    my $scheme = shift;
    my $impclass = LWP::Protocol::implementor($scheme) or
	Carp::croak("Protocol scheme '$scheme' is not supported");

    # hand-off to scheme specific implementation sub-class
    return $impclass->new($scheme);
}


=item $class = LWP::Protocol::implementor($scheme, [$class])

Get and/or set implementor class for a scheme.  Returns '' if the
specified scheme is not supported.

=cut

sub implementor
{
    my($scheme, $impclass) = @_;

    if ($impclass) {
	$ImplementedBy{$scheme} = $impclass;
    }
    my $ic = $ImplementedBy{$scheme};
    return $ic if $ic;

    return '' unless $scheme =~ /^([.+\-\w]+)$/;  # check valid URL schemes
    $scheme = $1; # untaint
    $scheme =~ s/[.+\-]/_/g;  # make it a legal module name

    # scheme not yet known, look for a 'use'd implementation
    $ic = "LWP::Protocol::$scheme";  # default location
    $ic = "LWP::Protocol::nntp" if $scheme eq 'news'; #XXX ugly hack
    no strict 'refs';
    # check we actually have one for the scheme:
    unless (@{"${ic}::ISA"}) {
	# try to autoload it
	eval "require $ic";
	if ($@) {
	    if ($@ =~ /Can't locate/) { #' #emacs get confused by '
		$ic = '';
	    } else {
		die "$@\n";
	    }
	}
    }
    $ImplementedBy{$scheme} = $ic if $ic;
    $ic;
}


=item $prot->request(...)

 $response = $protocol->request($request, $proxy, undef);
 $response = $protocol->request($request, $proxy, '/tmp/sss');
 $response = $protocol->request($request, $proxy, \&callback, 1024);

Dispactches a request over the protocol, and returns a response
object. This method needs to be overridden in subclasses.  Referer to
L<LWP::UserAgent> for description of the arguments.

=cut

sub request
{
    my($self, $request, $proxy, $arg, $size, $timeout) = @_;
    Carp::croak('LWP::Protocol::request() needs to be overridden in subclasses');
}


=item $prot->timeout($seconds)

Get and set the timeout value in seconds


=item $prot->parse_head($yesno)

Should we initialize response headers from the <head> section of HTML
documents.

=cut

sub timeout    { shift->_elem('timeout',    @_); }
sub parse_head { shift->_elem('parse_head', @_); }
sub max_size   { shift->_elem('max_size',   @_); }


=item $prot->collect($arg, $response, $collector)

Called to collect the content of a request, and process it
appropriately into a scalar, file, or by calling a callback.  If $arg
is undefined, then the content is stored within the $response.  If
$arg is a simple scalar, then $arg is interpreted as a file name and
the content is written to this file.  If $arg is a reference to a
routine, then content is passed to this routine.

The $collector is a routine that will be called and which is
reponsible for returning pieces (as ref to scalar) of the content to
process.  The $collector signals EOF by returning a reference to an
empty sting.

The return value from collect() is the $response object reference.

B<Note:> We will only use the callback or file argument if
$response->is_success().  This avoids sendig content data for
redirects and authentization responses to the callback which would be
confusing.

=cut

sub collect
{
    my ($self, $arg, $response, $collector) = @_;
    my $content;
    my($parse_head, $timeout, $max_size) =
      @{$self}{qw(parse_head timeout max_size)};

    my $parser;
    if ($parse_head && $response->content_type eq 'text/html') {
	$parser = HTML::HeadParser->new($response->{'_headers'});
    }
    my $content_size = 0;

    if (!defined($arg) || !$response->is_success) {
	# scalar
	while ($content = &$collector, length $$content) {
	    if ($parser) {
		$parser->parse($$content) or undef($parser);
	    }
	    LWP::Debug::debug("read " . length($$content) . " bytes");
	    $response->add_content($$content);
	    $content_size += length($$content);
	    if ($max_size && $content_size > $max_size) {
		LWP::Debug::debug("Aborting because size limit exceeded");
		my $tot = $response->header("Content-Length") || 0;
		$response->header("X-Content-Range", "bytes 0-$content_size/$tot");
		last;
	    }
	}
    }
    elsif (!ref($arg)) {
	# filename
	open(OUT, ">$arg") or
	    return HTTP::Response->new(&HTTP::Status::RC_INTERNAL_SERVER_ERROR,
			  "Cannot write to '$arg': $!");
        binmode(OUT);
        local($\) = ""; # ensure standard $OUTPUT_RECORD_SEPARATOR
	while ($content = &$collector, length $$content) {
	    if ($parser) {
		$parser->parse($$content) or undef($parser);
	    }
	    LWP::Debug::debug("read " . length($$content) . " bytes");
	    print OUT $$content;
	    $content_size += length($$content);
	    if ($max_size && $content_size > $max_size) {
		LWP::Debug::debug("Aborting because size limit exceeded");
		my $tot = $response->header("Content-Length") || 0;
		$response->header("X-Content-Range", "bytes 0-$content_size/$tot");
		last;
	    }
	}
	close(OUT);
    }
    elsif (ref($arg) eq 'CODE') {
	# read into callback
	while ($content = &$collector, length $$content) {
	    if ($parser) {
		$parser->parse($$content) or undef($parser);
	    }
	    LWP::Debug::debug("read " . length($$content) . " bytes");
            eval {
		&$arg($$content, $response, $self);
	    };
	    if ($@) {
	        chomp($@);
		$response->header('X-Died' => $@);
		last;
	    }
	}
    }
    else {
	return HTTP::Response->new(&HTTP::Status::RC_INTERNAL_SERVER_ERROR,
				  "Unexpected collect argument  '$arg'");
    }
    $response;
}


=item $prot->collect_once($arg, $response, $content)

Can be called when the whole response content is available as
$content.  This will invoke collect() with a collector callback that
returns a reference to $content the first time and an empty string the
next.

=cut

sub collect_once
{
    my($self, $arg, $response) = @_;
    my $content = \ $_[3];
    my $first = 1;
    $self->collect($arg, $response, sub {
	return $content if $first--;
	return \ "";
    });
}

1;

=head1 SEE ALSO

Inspect the F<LWP/Protocol/file.pm> and F<LWP/Protocol/http.pm> files
for examples of usage.

=head1 COPYRIGHT

Copyright 1995-1997 Gisle Aas.

This library is free software; you can redistribute it and/or
modify it under the same terms as Perl itself.

=cut

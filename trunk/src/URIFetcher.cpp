/* \file
 * Interface for implementations fetching HTMLs from URIs
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include "URIFetcher.h"
#include "SoupURIFetcher.h"

URIFetcher* URIFetcher::create()
{
	return new SoupURIFetcher;
}

URIFetcher::~URIFetcher() {}



// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4 :

#ifndef __URI_FETCHER_H
#define __URI_FETCHER_H

/* \file
 * Interface for implementations fetching HTMLs from URIs
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <glibmm/ustring.h>

/// URIFetcher

class URIFetcher 
{
public:
	static URIFetcher* create();
	virtual ~URIFetcher() = 0;
	
	virtual Glib::ustring fetch (Glib::ustring uri) = 0;
	virtual void finish (void (*)(URIFetcher*)) = 0;
	virtual const Glib::ustring& str() = 0;
};

#endif

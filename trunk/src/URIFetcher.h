#ifndef __URI_FETCHER_H
#define __URI_FETCHER_H

/* \file
 * Interface for implementations fetching HTMLs from URIs
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <glibmm/ustring.h>

class URIFetchInfo;

typedef void (*UF_quit_func_t)(void*,URIFetchInfo*);

/// URIFetcher

class URIFetcher 
{
public:
	static URIFetcher* create();
	virtual ~URIFetcher() = 0;
	
	/// Name a URI to fetch
	virtual void fetch (const Glib::ustring& uri) = 0;
	
	/// Set callback to call after URI is fetched.
	virtual void set_quit_func (UF_quit_func_t func, void *data) = 0;

	/// Start session and try to get all URIs.
	virtual void start() = 0;

	/// Interrupt session.
	virtual void stop() = 0;
};

#endif

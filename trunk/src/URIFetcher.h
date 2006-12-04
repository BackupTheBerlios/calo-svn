#ifndef __URI_FETCHER_H
#define __URI_FETCHER_H

/* \file
 * Interface for implementations fetching HTMLs from URIs
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <list>
#include <glibmm/ustring.h>

class URIFetchInfo;
class FetchInfoHandler;

typedef std::list <std::pair <Glib::ustring,Glib::ustring> > str_pair_list_t;

/// URIFetcher

class URIFetcher 
{
public:
	static URIFetcher* create();
	virtual ~URIFetcher() = 0;
	
	/// Name a URI to fetch
	virtual void add_uri (const Glib::ustring&, str_pair_list_t* = NULL) = 0;
	
	/// Set callback to call after URI is fetched.
	virtual void set_pline (FetchInfoHandler*) = 0;

	/// Start session and try to get all URIs.
	virtual void start() = 0;

	/// Interrupt session.
	virtual void stop() = 0;
};

#endif


// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4 :

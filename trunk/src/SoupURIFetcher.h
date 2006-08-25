#ifndef __SOUP_URI_FETCHER_H
#define __SOUP_URI_FETCHER_H

/* \file
 * Fetching HTMLs from URIs using libsoup.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include "URIFetcher.h"

struct SoupMessage;
struct SoupSession;

typedef void (*quit_func_t)(URIFetcher*);

/// SoupURIFetcher
class SoupURIFetcher : public URIFetcher
{
public:
	SoupURIFetcher();
	virtual ~SoupURIFetcher();
	virtual Glib::ustring fetch (Glib::ustring uri);
	virtual void finish (quit_func_t quit_func);
	virtual const Glib::ustring& str() { return _html; }

private:
	friend void got_data (SoupMessage *msg, gpointer data);
	
	SoupSession *_session;
	Glib::ustring _html;
	quit_func_t _quit_func;
};

#endif

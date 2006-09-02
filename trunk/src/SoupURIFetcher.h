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


/// SoupURIFetcher
class SoupURIFetcher : public URIFetcher
{
public:
	SoupURIFetcher();
	virtual ~SoupURIFetcher();
	
	virtual void fetch (const Glib::ustring& uri);
	virtual void set_quit_func (UF_quit_func_t func, void* data);
	virtual void start();
	virtual void stop();

private:
	friend void got_data (SoupMessage *msg, gpointer data);
	
	SoupSession 	*_session;
	Glib::ustring 	_html;
	UF_quit_func_t 	_quit_func;
	void 		*_quit_data;
};

#endif

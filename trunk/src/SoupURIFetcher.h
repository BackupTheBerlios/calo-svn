#ifndef __SOUP_URI_FETCHER_H
#define __SOUP_URI_FETCHER_H

/* \file
 * Fetching HTMLs from URIs using libsoup.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <list>
#include "URIFetcher.h"

struct SoupUri;
struct SoupMessage;
struct SoupSession;
class FetchAndRenderPipeline;


/// SoupURIFetcher
class SoupURIFetcher : public URIFetcher
{
public:
	SoupURIFetcher();
	virtual ~SoupURIFetcher();
	
	virtual void add_uri (const Glib::ustring& uri);
	virtual void set_pline (FetchAndRenderPipeline* p) { _pline = p; }
	virtual void start();
	virtual void stop();

private:
	friend void got_data (SoupMessage *msg, gpointer data);
	URIFetchInfo* handle_msg (SoupMessage *msg);
	void queue_uri (SoupUri*);
	
	std::list<SoupUri*> 	_uri_list;
	SoupSession 		*_session;
	FetchAndRenderPipeline 	*_pline;
	int			_fetched;
};

#endif

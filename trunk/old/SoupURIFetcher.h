#ifndef __SOUP_URI_FETCHER_H
#define __SOUP_URI_FETCHER_H

/* \file
 * Fetching HTMLs from URIs using libsoup.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <map>
#include "URIFetcher.h"

struct SoupUri;
struct SoupMessage;
struct SoupSession;
class FetchInfoHandler;


/// SoupURIFetcher
class SoupURIFetcher : public URIFetcher
{
public:
	SoupURIFetcher();
	virtual ~SoupURIFetcher();
	
	virtual void add_uri (const Glib::ustring& uri, str_pair_list_t* list = NULL);
	virtual void set_pline (FetchInfoHandler* p) { _pline = p; }
	virtual void start();
	virtual void stop();

private:
	friend void got_data (SoupMessage *msg, gpointer data);
	friend void handle_header (gpointer key, gpointer val, gpointer datap);
	URIFetchInfo* handle_msg (SoupMessage *msg);
	void queue_msg (std::pair<SoupMessage*,int>);
	
	std::map<SoupMessage*,int> 	_msg_index;
	SoupSession 			*_session;
	FetchInfoHandler		*_pline;
	unsigned int			_added, _fetched;
};

#endif

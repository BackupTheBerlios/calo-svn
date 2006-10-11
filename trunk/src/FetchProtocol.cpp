/* \file
 * Implementations of the fetch protocol.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <iostream>
#include <libsoup/soup.h>
#include "FetchProtocol.h"
#include "URIFetcher.h"
#include "URIFetchInfo.h"
#include "Feed.h"

static FetchProtocol* _instance = NULL;

FetchProtocol* FetchProtocol::get()
{
	if (_instance != NULL)
		return _instance;
	_instance = new FetchProtocol;
	return _instance;
}

FetchProtocol::FetchProtocol() {}

//-OVERRIDES--------------------------------------------------------------------

void 
FetchProtocol::handle_header (const Glib::ustring& key, const Glib::ustring& val)
{
	_curr_feed->set_property (key, val);
//std::cout<< key << ": " << val <<std::endl;
}

void 
FetchProtocol::quit_fetch (URIFetchInfo* info)
{
//std::cout<< info->html <<std::endl;
}

//---------------------------------------------------------------------------

void FetchProtocol::run (Glib::ustring& uri, Feed* theFeed)
{
	_curr_feed = theFeed;
	str_pair_list_t headers;
	if (!theFeed->is_unset())
		headers.push_back (std::make_pair ("If-Modified-Since", 
				theFeed->get_property ("Last-Visited")));
	
	URIFetcher *fetcher = URIFetcher::create();
	fetcher->add_uri (uri, &headers);
	fetcher->set_pline (this);
	fetcher->start();
}


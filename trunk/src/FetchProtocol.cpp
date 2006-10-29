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
#include "RSSParser.h"
#include "AppContext.h"
#include "Feed.h"
#include "utils.h"

/// The FetchProtocol singleton
static FetchProtocol* _instance = NULL;

/// Creates and returns the FetchProtocol singleton
FetchProtocol* FetchProtocol::get()
{
	if (_instance != NULL)
		return _instance;
	_instance = new FetchProtocol;
	return _instance;
}

FetchProtocol::FetchProtocol() {}

/// Callback that is called for every encountered HTML response header. 
/// It sets the current feed properties with it, changing "Date" to "Last-Visited". 
/// Overrides FetchInfoHandler::handle_header().
void 
FetchProtocol::handle_header (const Glib::ustring& key, const Glib::ustring& val)
{
	if (key == "Date") 
		_curr_feed->set_property (Glib::ustring ("Last-Visited"), val);
	else
		_curr_feed->set_property (key, val);
}

/// Callback that is called for every fetched HTML body.
/// Parses the body, adds every news item to the current feed (and other
/// listeners), layouts the item list, and sends a signal to the view to
/// draw itself. Overrides FetchInfoHandler::quit_fetch().
void 
FetchProtocol::quit_fetch (URIFetchInfo* info)
{
	RSSParser parser;
	parser.add_item_listener (_curr_feed);
	parser.add_item_listener (&AppContext::get());
	parser.parse_memory (info->html);
	_curr_feed->layout_items();
	AppContext::get().draw_view();
}

//---------------------------------------------------------------------------
/// Run the FetchProtocol by setting up HTML request headers, callbacks,
/// and calling the URIFetcher.
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


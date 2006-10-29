/* \file
 * Fetching HTMLs from URIs using libsoup.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <functional>
#include <glibmm/i18n.h>
#include <libsoup/soup.h>

#include "SoupURIFetcher.h"
#include "FetchInfoHandler.h"
#include "URIFetchInfo.h"
#include "utils.h"


/// Friend callback that ia called after every fetch. It puts the result 
/// in the Fetcher's UTF-8 storage (the URIFetchInfo) and calls its callback.
void got_data (SoupMessage *msg, gpointer datap)
{
	if (!SOUP_STATUS_IS_SUCCESSFUL (msg->status_code))
	{
		g_warning ("%d - %s", msg->status_code, msg->reason_phrase);
		return;
	}
	
	SoupURIFetcher* context = static_cast<SoupURIFetcher*>(datap);
	URIFetchInfo* info = context->handle_msg (msg);
	context->_pline->quit_fetch (info);
}

/// Friend callback that is called for every HTML response header.
/// It itself calls the respective Fetcher callback.
void handle_header (gpointer key, gpointer val, gpointer datap)
{
	SoupURIFetcher* context = static_cast<SoupURIFetcher*>(datap);
	context->_pline->handle_header ((char*)key, (char*)val);
}

//----------------------------------------------------------------------

SoupURIFetcher::SoupURIFetcher()
{
	_pline = NULL;
	_added = 0;
	_fetched = 0;
}

SoupURIFetcher::~SoupURIFetcher()
{
	soup_session_abort (_session);
}

/// Add an URI to the list of the to-be-fetched URIs by creating a SoupMessage.
void 
SoupURIFetcher::add_uri (const Glib::ustring& uri, str_pair_list_t* theHeaders)
{
	SoupUri *up = soup_uri_new (uri.c_str());
	if (up == NULL)
		g_warning (_("Could not parse URI: %s\n"), uri.c_str());

	SoupMessage* msg = soup_message_new_from_uri (SOUP_METHOD_GET, up);
	if (msg == NULL)
		g_error (_("Error creating message.\n"));
	else
		_msg_index[msg] = ++_added;

	soup_message_set_http_version (msg, SOUP_HTTP_1_1);
	soup_message_add_header (msg->request_headers, "User-Agent", "Calo/0.25pre-alpha libsoup/2.2.96");
	if (theHeaders != NULL)
		for (str_pair_list_t::iterator it = theHeaders->begin();
			it != theHeaders->end(); ++it) 
			soup_message_add_header (msg->request_headers, it->first.c_str(), it->second.c_str());
	soup_uri_free (up);
}

/// Callback that queues a single SoupMessage.
void SoupURIFetcher::queue_msg (std::pair<SoupMessage*,int> thePair)
{
	sleepms (50);
	SoupMessage* msg = thePair.first;
	soup_session_queue_message (_session, msg, got_data, this);
}

/// Queues all stored SoupMessages.
void SoupURIFetcher::start()
{
	if (_msg_index.size() == 0)
		return;

	_session = soup_session_async_new();
	for_each (_msg_index.begin(), _msg_index.end(), 
		bind1st (std::mem_fun (&SoupURIFetcher::queue_msg), this));
}

/// Helper that creates a URIFetchInfo from an HTML response.
URIFetchInfo* SoupURIFetcher::handle_msg (SoupMessage *msg)
{
	const SoupUri* su = soup_message_get_uri (msg);
	URIFetchInfo *info = new URIFetchInfo;
	info->is_fetched = true;
	if (++_fetched >= _added)
		info->is_last = true;
	char *buf = msg->response.body;
	buf[msg->response.length] = '\0';
	info->html = msg->response.body;
	info->uri = soup_uri_to_string (su, false);
	info->no = (_msg_index.find (msg))->second;
	soup_message_foreach_header (msg->response_headers, handle_header, this);

	return info;
}

void SoupURIFetcher::stop()
{
}


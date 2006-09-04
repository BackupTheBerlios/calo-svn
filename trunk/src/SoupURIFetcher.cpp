/* \file
 * Fetching HTMLs from URIs using libsoup.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <functional>
#include <glibmm/i18n.h>
#include <libsoup/soup.h>

#include "SoupURIFetcher.h"
#include "FetchAndRenderPipeline.h"
#include "URIFetchInfo.h"


/// Friend callback that puts the result in the Fetcher's UTF-8 storage
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

//----------------------------------------------------------------------

SoupURIFetcher::SoupURIFetcher()
{
	_pline = NULL;
	_fetched = 0;
}

SoupURIFetcher::~SoupURIFetcher()
{
	for_each (_uri_list.begin(), _uri_list.end(), soup_uri_free);
	soup_session_abort (_session);
}

void SoupURIFetcher::add_uri (const Glib::ustring& uri)
{
	SoupUri *up = soup_uri_new (uri.c_str());
	if (up != NULL)
		_uri_list.push_back (up);
	else
		g_warning (_("Could not parse URI: %s\n"), uri.c_str());
}

void SoupURIFetcher::queue_uri (SoupUri *up)
{
	SoupMessage* msg = soup_message_new_from_uri (SOUP_METHOD_GET, up);
	if (msg != NULL)
		soup_session_queue_message (_session, msg, got_data, this);
	else
		g_warning (_("Error creating message.\n"));
}

void SoupURIFetcher::start()
{
	_session = soup_session_async_new();
	for_each (_uri_list.begin(), _uri_list.end(), 
		bind1st (std::mem_fun (&SoupURIFetcher::queue_uri), this));
}

URIFetchInfo* SoupURIFetcher::handle_msg (SoupMessage *msg)
{
	const SoupUri* su = soup_message_get_uri (msg);
	URIFetchInfo *info = new URIFetchInfo;
	info->is_fetched = true;
	if (++_fetched >= _uri_list.size())
		info->is_last = true;
	info->html = msg->response.body;
	info->uri = soup_uri_to_string (su);
	return info;
}

void SoupURIFetcher::stop()
{
}


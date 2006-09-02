/* \file
 * Fetching HTMLs from URIs using libsoup.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <config.h>
#include <libsoup/soup.h>
#include "SoupURIFetcher.h"

/// This class can only handle one connection overall!

/// Friend callback that puts the result in the Fetcher's UTF-8 storage
void got_data (SoupMessage *msg, gpointer datap)
{
	if (!SOUP_STATUS_IS_SUCCESSFUL (msg->status_code))
	{
		g_warning ("%d - %s", msg->status_code, msg->reason_phrase);
		return;
	}
	
	SoupURIFetcher* context = static_cast<SoupURIFetcher*>(datap);
	context->_html = Glib::ustring (msg->response.body);
	if (context->_quit_func != NULL)
		context->_quit_func (context->_quit_data, NULL);
	else
		g_error ("Attempt to call _quit_func = NULL!\n");
	soup_session_abort (context->_session);
}

//----------------------------------------------------------------------

SoupURIFetcher::SoupURIFetcher()
{
	_quit_func = NULL;
}

SoupURIFetcher::~SoupURIFetcher()
{
	soup_session_abort (_session);
}

/// Do fetch
void SoupURIFetcher::fetch (const Glib::ustring& uri)
{
	// code skeleton stolen from gtkhtml
	_session = soup_session_async_new();
	SoupMessage* msg = soup_message_new (SOUP_METHOD_GET, uri.c_str());
	soup_session_queue_message (_session, msg, got_data, this);
}

/// No more fetches will happen, set callback
void SoupURIFetcher::set_quit_func (UF_quit_func_t func, void *data)
{
	_quit_func = func;
	_quit_data = data;
}

void SoupURIFetcher::start()
{
}

void SoupURIFetcher::stop()
{
}


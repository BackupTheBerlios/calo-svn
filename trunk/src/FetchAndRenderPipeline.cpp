/* \file
 * Fetching and rendering of articles
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <algorithm>
#include <functional>

#include "FetchAndRenderPipeline.h"
#include "URIFetcher.h"

// but when do we destroy the soup session?
static URIFetcher *_fetcher = NULL;

/// Starts fetching articles after which they are processed for the
/// PDFCreator. Only when all texts are available or timeout happens,
/// the named PDF file is written. For every single successful fetch, a 
/// callback is called giving this and the fetched URI.

FetchAndRenderPipeline::FetchAndRenderPipeline 
(const std::vector<Glib::ustring>& theURIs, const Glib::ustring& fname)
{
	set_callback();
	set_timeout_ms();
	_theURIs = &theURIs;
	_fname = fname;
	
	_size = _theURIs->size();
	_fetched.assign (_size, false);
	_all_fetched = false;
	if (_fetcher == NULL)
		_fetcher = URIFetcher::create();
}
	
FetchAndRenderPipeline::~FetchAndRenderPipeline()
{
}

//----------------------------------------------------------------------
static void fetch (Glib::ustring uri)
{
	_fetcher->fetch (uri);
}

static void quit_fetch (void* data, URIFetchInfo* info)
{
//	static_cast<FetchAndRenderPipeline*>(data)->quit_fetch (info);
}

// Called for every message the fetcher returns ?
//void FetchAndRenderPipeline::quit_fetch (URIFetchInfo* info)
//{
	// the info struct contains bool _is_last_call;
	// to determine if all URIs were fetched
//}

void FetchAndRenderPipeline::start()
{
	_fetcher->set_quit_func (quit_fetch, this);
	std::for_each (_theURIs->begin(), _theURIs->end(), fetch);
	_fetcher->start();

	// now wait for timeout, then stop

	if (!_all_fetched)
		_fetcher->stop();
}

status_t FetchAndRenderPipeline::stop()
{
	if (!_all_fetched)
		_fetcher->stop();
	return FULLY_WRITTEN;
}



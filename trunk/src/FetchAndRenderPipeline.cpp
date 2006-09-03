/* \file
 * Fetching and rendering of articles. This implementation of the
 * FetchAndRenderPipeline supposes it is the only instance using the
 * URIFetcher for fetching.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <algorithm>
#include <functional>

#include "FetchAndRenderPipeline.h"
#include "URIFetcher.h"
#include "URIFetchInfo.h"

// but when do we destroy the SoupURIFetcher?
static URIFetcher *_fetcher = NULL;

/// Creates all structures necessary for the pipeline protocol.
/// \param theURIs Container holding all URs to be fetched
/// \param fname Filename of PDF to be written
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
	_something_dumped = false;
	if (_fetcher == NULL)
		_fetcher = URIFetcher::create();
}
	
FetchAndRenderPipeline::~FetchAndRenderPipeline()
{
}

//----------------------------------------------------------------------
void quit_fetch (void* data, URIFetchInfo* info)
{
	static_cast<FetchAndRenderPipeline*>(data)->quit_fetch (info);
}

/// Helper, called for each uri.
static void fetch (Glib::ustring uri)
{
	_fetcher->fetch (uri);
}

/// Starts fetching articles after which they are processed for the
/// PDFCreator. Only when all dumps are available in the container,
/// or timeout happens, the named PDF file is written.
status_t FetchAndRenderPipeline::start()
{
	if (_theURIs->size() == 0)
		return NOTHING_FETCHED;
		
	_fetcher->set_quit_func (::quit_fetch, this);
	std::for_each (_theURIs->begin(), _theURIs->end(), fetch);

	_fetcher->start();

	// Now wait for timeout, then stop and maybe process further.
	// Handle signals!!!
	
	// If all dumps are already available before timeout, writing
	// of PDF is initiated by the last callback thread called. So,
	// we need to handle here only the other cases.
	if (!_all_fetched)
	{
		_fetcher->stop();
		if (!_something_dumped)
			return NOTHING_FETCHED;
		make_pdf();
		return PARTLY_WRITTEN;
	}
	/// all fetched, wait a bit?
	return FULLY_WRITTEN;
}

/// Called asynchronously for every message the fetcher returns ?
/// Postprocesses fully received HTMLs (Dumper, Processor), then
/// saves dump in a container.
void FetchAndRenderPipeline::quit_fetch (URIFetchInfo* info)
{
	// the info struct contains bool _is_last_call;
	// to determine if all URIs were fetched

	make_dump (info);
	if (info->is_last)
		make_pdf();
	_progress_cb (this, info->uri, info->is_fetched, info->is_last);
}

/// Interrupt the pipeline even before timeout is reached. Needed?
status_t FetchAndRenderPipeline::stop()
{
	if (!_all_fetched)
		_fetcher->stop();
	return FULLY_WRITTEN;
}

//-----------------------------------------------------------
void FetchAndRenderPipeline::make_dump (URIFetchInfo* info)
{
	_something_dumped = true;
}

void FetchAndRenderPipeline::make_pdf()
{
}

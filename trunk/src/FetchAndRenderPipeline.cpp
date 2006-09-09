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
#include <sigc++/sigc++.h>
#include <glibmm/main.h>

#include "FetchAndRenderPipeline.h"
#include "TextDumper.h"
#include "DumpProcessor.h"
#include "PDFCreator.h"
#include "URIFetcher.h"
#include "URIFetchInfo.h"
#include "utils.h"


/// Creates all structures necessary for the pipeline protocol.
FetchAndRenderPipeline::FetchAndRenderPipeline()
{
	set_fname();
	set_callback();
	set_timeout_ms();
	
	_size = 0;
	_all_fetched = false;
	_something_dumped = false;
	_fetcher = URIFetcher::create();
}

/// Adds an entry for this uri to the internal database of URIs
/// that will be fetched
void FetchAndRenderPipeline::add_uri (const Glib::ustring& uri)
{
	_fetcher->add_uri (uri);
	++_size;
}

FetchAndRenderPipeline::~FetchAndRenderPipeline()
{
	delete _fetcher;
}

//----------------------------------------------------------------------
/// Starts fetching articles and wait for timeout.
status_t FetchAndRenderPipeline::start()
{
	if (_size == 0)
		return NOTHING_FETCHED;
		
	_fetcher->set_pline (this);
	_fetcher->start();
	Glib::signal_timeout().connect (
		sigc::mem_fun (this, &FetchAndRenderPipeline::post_fetch), _timeout_ms);
	return WAITING;
}

/// Callback to call after timeout, after which fetch results are processed 
/// for the PDFCreator. Only when all dumps are available in the container,
/// or timeout happens, the named PDF file is written.
bool
FetchAndRenderPipeline::post_fetch()
{
	// If all dumps are already available before timeout, writing
	// of PDF is initiated by the last callback thread called. So,
	// we need to handle here only the other cases.
	if (!_all_fetched)
	{
		_fetcher->stop();
		if (!_something_dumped)
			return false; // NOTHING_FETCHED;
		make_pdf();
		return false; //PARTLY_WRITTEN;
	}
	/// all fetched, wait a bit?
	return true; //	FULLY_WRITTEN;
}

/// Called asynchronously for every message the fetcher returns ?
/// Postprocesses fully received HTMLs (Dumper, Processor), then
/// saves dump in a container.
void 
FetchAndRenderPipeline::quit_fetch (URIFetchInfo* info)
{
	// the info struct contains bool _is_last_call;
	// to determine if all URIs were fetched

	_dumps[info->no] = make_dump (info->html, info->uri);
	
	if (info->is_last)
	{
		make_pdf();
		_all_fetched = true;
	}
	_progress_cb (this, info->uri, info->is_fetched, info->is_last);
}

/// Interrupt the pipeline even before timeout is reached. Needed?
status_t 
FetchAndRenderPipeline::stop()
{
	if (!_all_fetched)
		_fetcher->stop();
	return FULLY_WRITTEN;
}

//-----------------------------------------------------------
const Glib::ustring&
FetchAndRenderPipeline::make_dump (const Glib::ustring& html, const Glib::ustring& uri)
{
	TextDumper *dumper = TextDumper::create();
	const Glib::ustring& dump = dumper->render (html);

	DumpProcessor *dp = DumpProcessor::create();
	dp->set_uri (uri);
	const Glib::ustring& res = dp->process (dump);
	
	_something_dumped = true;
	return res;
}

void 
FetchAndRenderPipeline::make_pdf()
{
	PDFCreator *pdf = PDFCreator::create();
	for (unsigned int i=1; i<=_dumps.size(); ++i)
		*pdf << _dumps[i];
	pdf->save();
	delete pdf;
}

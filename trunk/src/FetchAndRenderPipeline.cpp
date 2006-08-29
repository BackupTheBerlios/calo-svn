/* \file
 * Fetching and rendering of articles
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include "FetchAndRenderPipeline.h"

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
}
	
FetchAndRenderPipeline::~FetchAndRenderPipeline()
{
}

void FetchAndRenderPipeline::start()
{
}

status_t FetchAndRenderPipeline::stop()
{
	return FULLY_WRITTEN;
}



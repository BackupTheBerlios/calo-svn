/* \file
 * Fetching and rendering of articles
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include "FetchAndRenderPipeline.h"

FetchAndRenderPipeline::FetchAndRenderPipeline 
(const std::vector<Glib::ustring>& theURIs, 
 const Glib::ustring& fname, 
 void (*progress_cb)(FetchAndRenderPipeline*, const Glib::ustring&) = NULL,
 unsigned long timeout_ms = 300000L)
{
}
	
FetchAndRenderPipeline::~FetchAndRenderPipeline()
{
}

status_t FetchAndRenderPipeline::interrupt()
{
	return FULLY_WRITTEN;
}



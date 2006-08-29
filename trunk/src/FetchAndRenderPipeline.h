#ifndef __FETCH_AND_RENDERER_H
#define __FETCH_AND_RENDERER_H

/* \file
 * Interface for the protocol that fetches and renders articles.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <vector>
#include <glibmm/ustring.h>

/// FetchAndRenderPipeline

typedef enum { PARTLY_FETCHED=0, PARTLY_WRITTEN, FULLY_WRITTEN } status_t;

class FetchAndRenderPipeline
{
public:
	FetchAndRenderPipeline (const std::vector<Glib::ustring>& theURIs, 
			const Glib::ustring& fname, 
			void (*progress_cb)(FetchAndRenderPipeline*, const Glib::ustring&) = NULL,
			unsigned long timeout_ms = 300000L);
	
	status_t interrupt();
	~FetchAndRenderPipeline();

private:
	status_t _status;
};

#endif

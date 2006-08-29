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

class FetchAndRenderPipeline;

typedef enum { PARTLY_FETCHED=0, PARTLY_WRITTEN, FULLY_WRITTEN } status_t;
typedef void (*progress_func_t)(FetchAndRenderPipeline*, const Glib::ustring&);

class FetchAndRenderPipeline
{
public:
	FetchAndRenderPipeline (const std::vector<Glib::ustring>& theURIs, 
			const Glib::ustring& fname = "calo.pdf");
	
	void set_callback (progress_func_t progress_cb = NULL) 	
		{ _progress_cb = progress_cb; }
	void set_timeout_ms (unsigned long timeout_ms = 300000L)
		{ _timeout_ms = timeout_ms; }
	
	void start();
	status_t stop();
	~FetchAndRenderPipeline();

private:
	status_t 			_status;
	const std::vector<Glib::ustring> *_theURIs;
	Glib::ustring			_fname;
	progress_func_t 		_progress_cb;
	unsigned long 			_timeout_ms;
	int				_size;
	std::vector<bool>		_fetched;
};

#endif

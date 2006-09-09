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


class FetchAndRenderPipeline;
class URIFetcher;
class URIFetchInfo;

typedef enum { NOTHING_FETCHED=0, WAITING, PARTLY_FETCHED, PARTLY_WRITTEN, 
	FULLY_WRITTEN } status_t;
typedef void (*progress_func_t)(FetchAndRenderPipeline*, const Glib::ustring&,bool,bool);

/// FetchAndRenderPipeline

class FetchAndRenderPipeline
{
public:
	FetchAndRenderPipeline();
	~FetchAndRenderPipeline();
			
	void set_fname (const Glib::ustring& fname = "calo.pdf")
		{ _fname = fname; }
	void set_callback (progress_func_t progress_cb = NULL) 	
		{ _progress_cb = progress_cb; }
	void set_timeout_ms (unsigned long timeout_ms = 30000L)
		{ _timeout_ms = timeout_ms; }
	void add_uri (const Glib::ustring& uri);
 	
	status_t start();
	status_t stop();
	void quit_fetch (URIFetchInfo*);

private:
	bool post_fetch();
	const Glib::ustring& make_dump (const Glib::ustring& html, 
					const Glib::ustring& uri);
	void make_pdf();

	URIFetcher			*_fetcher;
	std::vector<Glib::ustring> 	_dumps;
	Glib::ustring			_fname;
	progress_func_t 		_progress_cb;
	unsigned long 			_timeout_ms;
	int				_size;
	bool				_all_fetched, _something_dumped;
};

#endif

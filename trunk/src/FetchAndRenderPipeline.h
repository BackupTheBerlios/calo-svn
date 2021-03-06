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
#include <sigc++/sigc++.h>
#include "FetchInfoHandler.h"

class AppContext;
class FetchAndRenderPipeline;
class URIFetcher;

typedef enum { NOTHING_FETCHED=0, WAITING, PARTLY_FETCHED, PARTLY_WRITTEN, 
	FULLY_WRITTEN } status_t;

/// FetchAndRenderPipeline

class FetchAndRenderPipeline : public FetchInfoHandler
{
public:
	FetchAndRenderPipeline();
	~FetchAndRenderPipeline();
			
	void set_fname (const Glib::ustring& fname = "calo.pdf")
		{ _fname = fname; }
	void set_timeout_ms (unsigned long timeout_ms = 30000L)
		{ _timeout_ms = timeout_ms; }
	void set_render_to_pdf (bool pdf_f = true)
		{ _pdf_f = pdf_f; }
	void add_uri (const Glib::ustring& uri, const Glib::ustring& title);
 	
	status_t start();
	status_t stop();

	// Override
	virtual void quit_fetch (URIFetchInfo*);
	virtual void handle_header (const Glib::ustring&, const Glib::ustring&) {}

	sigc::signal<void,const Glib::ustring&,const Glib::ustring&,bool,bool> signal_msg_fetched;

private:
	bool post_fetch();
	const Glib::ustring& make_dump (const Glib::ustring& html, 
			const Glib::ustring& uri, const Glib::ustring& title);
	void make_pdf();

	URIFetcher			*_fetcher;
	std::vector<Glib::ustring> 	_dumps, _titles;
	Glib::ustring			_fname;
	sigc::connection		_timeout_connection;
	unsigned long 			_timeout_ms;
	int				_size;
	bool				_all_fetched, _something_dumped, _pdf_f;
};

#endif


// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4 :

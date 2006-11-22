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

class FetchAndRenderPipeline;

typedef enum { NOTHING_FETCHED=0, WAITING, PARTLY_FETCHED, PARTLY_WRITTEN, 
	FULLY_WRITTEN } status_t;
typedef void (*progress_func_t)(FetchAndRenderPipeline*, const Glib::ustring&,const Glib::ustring&, bool, bool);

/// FetchAndRenderPipeline

class FetchAndRenderPipeline 
{
public:
	FetchAndRenderPipeline();
	~FetchAndRenderPipeline();
	
	Glib::ustring single_get (const Glib::ustring& uri);
	void set_fname (const Glib::ustring& fname = "calo.pdf")
		{ _fname = fname; }
	void set_timeout_ms (unsigned long timeout_ms = 30000L)
		{ _timeout_ms = timeout_ms; }
	void set_render_to_pdf (bool pdf_f = true)
		{ _pdf_f = pdf_f; }
	void add_uri (const Glib::ustring& uri);
 	
	//status_t start();


private:
	//bool post_fetch();
	const Glib::ustring& make_dump (const Glib::ustring& html, 
					const Glib::ustring& uri);
	void make_pdf();

	std::vector<Glib::ustring> 	_dumps;
	Glib::ustring			_fname;
	sigc::connection		_timeout_connection;
	progress_func_t 		_progress_cb;
	unsigned long 			_timeout_ms;
	int				_size;
	bool				_all_fetched, _something_dumped, _pdf_f;
};

#endif

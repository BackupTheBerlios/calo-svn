/* \file
 * The main source file.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <iostream>
#include <gtkmm/main.h>

#include "FetchAndRenderPipeline.h"

static void callback (FetchAndRenderPipeline*, const Glib::ustring&,bool,bool);

Glib::RefPtr< Glib::MainLoop > loop;

int main (int argc , char **argv)
{
	g_type_init();
	g_thread_init (NULL);           // needed by libsoup

	std::vector<Glib::ustring> uri_list;
	
	uri_list.push_back ("http://localhost/index.html");
	uri_list.push_back ("http://localhost/index.html");
	uri_list.push_back ("http://localhost/index.html");
	
	FetchAndRenderPipeline pline (uri_list, "test.pdf");

	pline.set_callback (callback);
	pline.start();

	loop = Glib::MainLoop::create();
	loop->run();

}

static void callback (FetchAndRenderPipeline* line, const Glib::ustring& uri, bool was_fetched, bool is_last)
{
	std::cout << "URI: " << uri << " was" << (was_fetched? " ":" not ") << "fetched." << std::endl;
	if (is_last)
		loop->quit();
}



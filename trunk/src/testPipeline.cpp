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

	FetchAndRenderPipeline pline;
///	pline.add_uri ("http://localhost:8080/index.html");
//	pline.add_uri ("http://localhost:8080/index.html");
	pline.add_uri ("http://localhost:8080/test001.html");
	pline.add_uri ("http://localhost:8080/test002.html");
	pline.add_uri ("http://localhost:8080/test003.html");
	pline.add_uri ("http://localhost:8080/index.html");
	pline.set_callback (callback);
	pline.start();

	loop = Glib::MainLoop::create();
	loop->run();

}

static void callback (FetchAndRenderPipeline* line, const Glib::ustring& uri, bool was_fetched, bool is_last)
{
	std::cout << "URI: " << uri << " was" << (was_fetched? " ":" not ") << "fetched." << std::endl << std::flush;
	if (is_last)
		loop->quit();
}




// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4 :

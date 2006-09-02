/* \file
 * The main source file.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <gtkmm/main.h>


Glib::RefPtr< Glib::MainLoop > loop;

int main (int argc , char **argv)
{
	g_type_init();
	g_thread_init (NULL);           // needed by libsoup

	loop = Glib::MainLoop::create();
	loop->run();

}

//static void quit (URIFetcher* fetcher)
//{
//	loop->quit();
//}



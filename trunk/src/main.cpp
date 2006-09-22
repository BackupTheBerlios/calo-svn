/* \file
 * The main source file.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <gtkmm/main.h>
#include "AppWindow.h"

Glib::RefPtr< Glib::MainLoop > loop;

int main (int argc , char **argv)
{
	Gtk::Main kit (argc, argv);
	AppWindow aw;
	Gtk::Main::run (aw);

	return 0;
}

//static void quit (URIFetcher* fetcher)
//{
//	loop->quit();
//}



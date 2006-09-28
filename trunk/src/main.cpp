/* \file
 * The main source file.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <gtkmm/main.h>
#include "AppContext.h"
#include "AppWindow.h"

static bool quit();


int main (int argc , char **argv)
{
	g_thread_init (NULL);
	Gtk::Main kit (argc, argv);
	AppContext::get().init();
	Gtk::Main::signal_quit().connect (sigc::ptr_fun (quit));
	AppWindow aw;
	AppContext::get().set_appwindow (&aw);
	Gtk::Main::run (aw);

	return 0;
}

static bool quit()
{
	AppContext::get().save();
	AppContext::destroy();
	return true;
}



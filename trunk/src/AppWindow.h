#ifndef __APP_WINDOW_H
#define __APP_WINDOW_H

/* \file
 * Topmost application window.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <gtkmm/window.h>
#include <gtkmm/paned.h>
#include <gtkmm/entry.h>
#include "FeedList.h"
#include "HeaderList.h"


/// AppWindow

class AppWindow : public Gtk::Window
{
public:
	AppWindow();
	virtual ~AppWindow();

protected:
	virtual void on_realize();

	Gtk::Entry _entry;
	Gtk::HPaned _vpaned;
	FeedList _flist;
	HeaderList _hlist;
};



#endif

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
#include "ViewContainer.h"


/// AppWindow

class AppWindow : public Gtk::Window
{
public:
	AppWindow();
	virtual ~AppWindow();

	ViewContainer* get_viewcontainer() 	{ return &_vc; }
	FeedList* get_feedlist() 		{ return &_flist; }

protected:
	virtual void on_realize();
	virtual bool on_delete_event (GdkEventAny*);

	Gtk::Entry _entry;
	Gtk::HPaned _vpaned;
	FeedList _flist;
	ViewContainer _vc;
};



#endif


// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4 :

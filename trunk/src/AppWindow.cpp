/* \file
 * Implementation of topmost application window.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <iostream>
#include <gtkmm/box.h>
#include <gtkmm/separator.h>
#include <gtkmm/statusbar.h>
#include <gtkmm/uimanager.h>
#include "AppWindow.h"
#include "AppContext.h"
#include "MenuBar.h"

AppWindow::AppWindow()
{
	set_title ("Calo");
	set_border_width (0);
	set_default_size (450, 400);
	
	Gtk::VBox *_vbox = new Gtk::VBox;
	add (*manage (_vbox));
	Gtk::Widget *wid  = MenuBar::create (this);
	if (wid != NULL)
		_vbox->pack_start (*wid, Gtk::PACK_SHRINK);
	_vbox->pack_start (_entry, false, false, 0);
	_vbox->pack_start (*manage (new Gtk::HSeparator()), false, false, 2);
	_vbox->add (_vpaned);

	_vpaned.add1 (_flist);
	_vpaned.add2 (_vc);

	Gtk::Statusbar *bar = new Gtk::Statusbar();
	_vbox->pack_end (*manage (bar), Gtk::PACK_SHRINK);
	bar->set_has_resize_grip();

	unsigned int x, y, w, h;
	AppContext::get().get_appwindow_pos (&x, &y, &w, &h);
	set_default_size (w? w:400, h? h:300);
	w = AppContext::get().get_lpanew();
	_flist.set_size_request (w? w:150, -1);

	show_all_children();
}

AppWindow::~AppWindow()
{
}

//--------------------------------------------------------------
void
AppWindow::on_realize()
{
	unsigned int x, y, w, h;
	AppContext::get().get_appwindow_pos (&x, &y, &w, &h);
	if (w)
		move (x, y);
	Gtk::Widget::on_realize();
}

bool
AppWindow::on_delete_event (GdkEventAny*)
{
	AppContext::get().save_geometry();
	return false;
}

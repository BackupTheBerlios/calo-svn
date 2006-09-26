/* \file
 * Implementation of topmost application window.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <gtkmm/box.h>
#include <gtkmm/separator.h>
#include <gtkmm/uimanager.h>
#include "AppWindow.h"
#include "MenuBar.h"

AppWindow::AppWindow()
{
	set_title ("Calo");
	set_border_width (10);
	set_default_size (450, 400);
	
	Gtk::VBox *_vbox = new Gtk::VBox;
	add (*manage (_vbox));
	Gtk::Widget *w  = MenuBar::create (this);
	if (w != NULL)
		_vbox->pack_start (*w, Gtk::PACK_SHRINK);
	_vbox->pack_start (_entry, false, false, 0);
	_vbox->pack_start (*manage (new Gtk::HSeparator()), false, false, 2);
	_vbox->add (_vpaned);

	_vpaned.add1 (_flist);
	_vpaned.add2 (_hlist);

	show_all_children();
}

AppWindow::~AppWindow()
{
}

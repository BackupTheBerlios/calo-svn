#ifndef __VIEW_CONTAINER_H
#define __VIEW_CONTAINER_H

/* \file
 * Pane containing the main view (view window and filter widgets).
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/buttonbox.h>
#include "ViewWindow.h"


/// ViewContainer

class ViewContainer : public Gtk::HBox
{
public:
	ViewContainer();
	virtual ~ViewContainer();

	ViewWindow* get_viewwindow() { return &_vw; }

private:
	void on_simple_button();
	void on_normal_button();

	ViewWindow 		_vw;
	Gtk::VBox 		_box;
	Gtk::VButtonBox 	_buttons;
	Gtk::Button	_but_simple_disp, _but_norm_disp, _but_full_text;
};



#endif


// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4 :

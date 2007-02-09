/* \file
 * Implementing the main view pane.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <glibmm/i18n.h>
#include <gtkmm/image.h>
#include <gtkmm/tooltips.h>
#include "ViewContainer.h"
#include "AppContext.h"
#include "gtkutils.h"

ViewContainer::ViewContainer()
{
	Glib::RefPtr<Gdk::Pixbuf> buf = PIXCACHE->load ("list-view.png");
	Gtk::Image *img = manage (new Gtk::Image (buf));
	_but_simple_disp.set_image (*img);
    _but_simple_disp.set_size_request (54);
    AppContext::get().get_tooltips()->set_tip (_but_simple_disp, _("Simple display"));
	buf = PIXCACHE->load ("norm-view.png");
	img = manage (new Gtk::Image (buf));
	_but_norm_disp.set_image (*img);
    _but_norm_disp.set_size_request (54);
    AppContext::get().get_tooltips()->set_tip (_but_norm_disp, _("Normal display"));
	buf = PIXCACHE->load ("full-view.png");
	img = manage (new Gtk::Image (buf));
	_but_full_text.set_image (*img);
    _but_full_text.set_size_request (54);
    AppContext::get().get_tooltips()->set_tip (_but_full_text, _("Full display"));
		
	_box.pack_start (_but_simple_disp, Gtk::PACK_SHRINK);
	_box.pack_start (_but_norm_disp, Gtk::PACK_SHRINK);
	_box.pack_start (_but_full_text, Gtk::PACK_SHRINK);
	pack_start (_box, Gtk::PACK_SHRINK);
	pack_start (_vw, Gtk::PACK_EXPAND_WIDGET, 5);
	
	_but_simple_disp.signal_clicked().connect (sigc::mem_fun (*this, &ViewContainer::on_simple_button));
	_but_norm_disp.signal_clicked().connect (sigc::mem_fun (*this, &ViewContainer::on_normal_button));
	
	show_all_children();
}

ViewContainer::~ViewContainer()
{
}

//-----------------------------------------------------------------
void 
ViewContainer::on_simple_button()
{
	AppContext::get().set_display_type (SIMPLE);
	_vw._darea.set_display_modus_switched();
	AppContext::get().draw_view();
}

void
ViewContainer::on_normal_button()
{
	AppContext::get().set_display_type (NORMAL);
	_vw._darea.set_display_modus_switched();
	AppContext::get().draw_view();
}



// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4 :

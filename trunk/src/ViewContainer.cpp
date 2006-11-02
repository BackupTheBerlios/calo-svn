/* \file
 * Implementing the main view pane.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include "ViewContainer.h"
#include "AppContext.h"


ViewContainer::ViewContainer()
{
	_buttons.add (_but_simple_disp);
	_buttons.add (_but_norm_disp);
	_buttons.add (_but_full_text);
	_buttons.set_layout (Gtk::BUTTONBOX_START);
	_box.pack_start (_buttons, Gtk::PACK_SHRINK);
	pack_start (_box, Gtk::PACK_SHRINK);
	pack_start (_vw, Gtk::PACK_EXPAND_WIDGET);
	
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
	AppContext::get().draw_view();
}

void
ViewContainer::on_normal_button()
{
	AppContext::get().set_display_type (NORMAL);
	AppContext::get().draw_view();
}


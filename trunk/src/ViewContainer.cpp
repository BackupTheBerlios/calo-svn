/* \file
 * Implementing the main view pane.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include "ViewContainer.h"


ViewContainer::ViewContainer()
{
	_buttons.add (_but_simple_disp);
	_buttons.add (_but_norm_disp);
	_buttons.add (_but_full_text);
	_buttons.set_layout (Gtk::BUTTONBOX_START);
	_box.pack_start (_buttons, Gtk::PACK_SHRINK);
	pack_start (_box, Gtk::PACK_SHRINK);
	pack_start (_vw, Gtk::PACK_EXPAND_WIDGET);
	show_all_children();
}

ViewContainer::~ViewContainer()
{
}

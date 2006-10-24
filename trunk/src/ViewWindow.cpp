/* \file
 * Implementing the text view.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <iostream>
#include <gtkmm/widget.h>
#include <gdkmm/drawable.h>
#include "ViewWindow.h"
#include "AppContext.h"
#include "Feed.h"
#include "Item.h"


ViewWindow::ViewWindow()
{
	add (_darea);


	show_all_children();
}

ViewWindow::~ViewWindow()
{
}

bool
ViewDrawingArea::on_expose_event (GdkEventExpose* event)
{
	Glib::RefPtr<Gdk::Window> window = get_window();
	if (window)
	{
#ifdef DEBUG
		std::cerr << "on_expose_event()" << std::endl << std::flush;
#endif
		Gtk::Allocation allocation = get_allocation();
		const int width = allocation.get_width();
		const int height = allocation.get_height();

		Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();
	// clip to the area indicated by the expose event so that we only redraw
	// the portion of the window that needs to be redrawn
		cr->rectangle (event->area.x, event->area.y,
			event->area.width, event->area.height);
		cr->clip();

		cr->set_source_rgb (1.0, 1.0, 1.0);
		cr->paint();
		cr->set_source_rgb (0.0, 0.0, 0.0);

		Feed *feed = AppContext::get().get_feed();
		if (feed == NULL) 
			return true;
#ifdef DEBUG
		std::cerr << "Rendering..." << std::endl << std::flush;
#endif
		item_list_t items = feed->get_items();
		double h = 0.0;
		for (item_list_t::iterator it = items.begin(); it != items.end(); ++it)
		{
			ItemDisplayUnit *du = (*it)->get_display_unit();
			du->render (cr, 0, h);
			h += du->get_height();
		}
	
	}

  return true;
}

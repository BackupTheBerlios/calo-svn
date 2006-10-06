/* \file
 * Implementing the text view.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <gtkmm/widget.h>
#include <gdkmm/drawable.h>
#include "ViewWindow.h"


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
// This is where we draw on the window
  Glib::RefPtr<Gdk::Window> window = get_window();
  if(window)
  {
    Gtk::Allocation allocation = get_allocation();
    const int width = allocation.get_width();
    const int height = allocation.get_height();

    // coordinates for the center of the window
    int xc, yc;
    xc = width / 2;
    yc = height / 2;

    Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();
    cr->set_line_width(10.0);

    // clip to the area indicated by the expose event so that we only redraw
    // the portion of the window that needs to be redrawn
    cr->rectangle(event->area.x, event->area.y,
            event->area.width, event->area.height);
    cr->clip();

    // draw red lines out from the center of the window
    cr->set_source_rgb(0.8, 0.0, 0.0);
    cr->move_to(0, 0);
    cr->line_to(xc, yc);
    cr->line_to(0, height);
    cr->move_to(xc, yc);
    cr->line_to(width, yc);
    cr->stroke();
  }

  return true;
}

/* \file
 * Implementing the text view.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#undef DEBUG

#include <iostream>
#include <gtkmm/adjustment.h>
#include <gtkmm/scrollbar.h>
#include <gtkmm/widget.h>
#include <gdkmm/drawable.h>
#include "ViewWindow.h"
#include "AppContext.h"
#include "Feed.h"
#include "Item.h"


ViewWindow::ViewWindow()
{
	attach (_darea, 0, 1, 0, 1, Gtk::FILL|Gtk::EXPAND, Gtk::FILL|Gtk::EXPAND);
	attach (_hbar, 0, 1, 1, 2, Gtk::FILL|Gtk::EXPAND, Gtk::SHRINK);
	attach (_vbar, 1, 2, 0, 1, Gtk::SHRINK, Gtk::FILL|Gtk::EXPAND);
	
	_darea._hadj = _hbar.get_adjustment();
	_darea._vadj = _vbar.get_adjustment();
	_darea._vadj->set_lower (0.0);
	_darea._hadj->set_lower (0.0);
	_darea._vadj->set_value (0.0);
	_darea._hadj->set_value (0.0);

	_hbar.signal_value_changed().connect (sigc::bind 
		(sigc::mem_fun (*this, &ViewWindow::on_value_changed), &_hbar));

	_vbar.signal_value_changed().connect (sigc::bind 
		(sigc::mem_fun (*this, &ViewWindow::on_value_changed), &_vbar));

	show_all_children();
}

ViewWindow::~ViewWindow()
{
}

/// Called when view scrollbar changes value.
/// TODO: Increase performance.
void
ViewWindow::on_value_changed (Gtk::Scrollbar* theBar)
{
	Glib::RefPtr<Gdk::Window> window = _darea.get_window();
	if (window)
	{
		Cairo::RefPtr<Cairo::Context> cr = window->create_cairo_context();

	
		// Dimensions of drawing area
		Gtk::Allocation area = _darea.get_allocation();
		cr->reset_clip();
		cr->rectangle (0, 0, area.get_width(), area.get_height());
		cr->clip();

		cr->set_source_rgb (1.0, 1.0, 1.0);
		cr->paint();
		cr->set_source_rgb (0.0, 0.0, 0.0);

		Feed *feed = AppContext::get().get_feed();
		if (feed == NULL) 
			return;

		item_list_t items = feed->get_items();
		double h = -_vbar.get_value(), w = 0.0;
		for (item_list_t::iterator it = items.begin(); it != items.end(); ++it)
		{
			ItemDisplayUnit *du = (*it)->get_display_unit();
			du->render (cr, -_hbar.get_value(), h);
			h += du->get_height();
			if (du->get_width() > w)
				w = du->get_width();
		}

	}
}

//-----------------------------------------------------------------------------
/// Called on every new expose event.
/// TODO: Increase performance.
bool
ViewDrawingArea::on_expose_event (GdkEventExpose* event)
{
#ifdef DEBUG
	std::cerr << "on_expose_event()" << std::endl << std::flush;
#endif
	Feed *feed = AppContext::get().get_feed();
	if (feed == NULL) 
		return true;

#ifdef DEBUG
		std::cerr << "VA->val:"<<_vadj->get_value() <<" VA->lower:"<<_vadj->get_lower() <<" ev->y:"<<event->area.y <<" ev->h:"<<event->area.height << std::endl << std::flush;
#endif

	// First determine dimensions of text area
	item_list_t items = feed->get_items();
	double h = 0.0, w = 0.0;
	for (item_list_t::iterator it = items.begin(); it != items.end(); ++it)
	{
		(*it)->make_display_unit();
		ItemDisplayUnit *du = (*it)->get_display_unit();
		h += du->get_height();
		if (du->get_width() > w)
			w = du->get_width();
	}

	// Dimensions of drawing area
	Gtk::Allocation allocation = get_allocation();
	const int width = allocation.get_width();
	const int height = allocation.get_height();

	// Scrollbar settings can now be specified
	_vadj->set_upper (h);
	_vadj->set_page_size (height);
	_vadj->set_step_increment (height/16.0);
	_vadj->set_page_increment (height*15.0/16.0);
	_hadj->set_upper (w);
	_hadj->set_page_size (width);
	_hadj->set_step_increment (width/16.0);
	_hadj->set_page_increment (width*15.0/16.0);
		
	// Set position of text top (<=0.0) and scrollbar value.
	// Several cases are necessary to get the behaviour right.
	double x, y;
	if (h < height)
	{
		y = 0.0;
		_vadj->set_value (0.0);
	}
	else if (h - _vadj->get_value() < height)
	{
		y = height - h;
		_vadj->set_value (-h);
	}
	else
		y = -_vadj->get_value();
		
	if (w < width)
	{
		x = 0.0;
		_hadj->set_value (0.0);
	}
	else
		x = -_hadj->get_value();
	_vadj->changed();
	_hadj->changed();
		
	// clip to the area indicated by the expose event so that we only redraw
	// the portion of the window that needs to be redrawn
	/// TODO: more efficiently, start with the right line

	Cairo::RefPtr<Cairo::Context> cr = get_window()->create_cairo_context();
	cr->reset_clip();
	cr->rectangle (event->area.x, event->area.y,
			event->area.width, event->area.height);
	cr->clip();

	// Black on white
	cr->set_source_rgb (1.0, 1.0, 1.0);
	cr->paint();
	cr->set_source_rgb (0.0, 0.0, 0.0);

	// Render text
	for (item_list_t::iterator it = items.begin(); it != items.end(); ++it)
	{
		ItemDisplayUnit *du = (*it)->get_display_unit();
		du->render (cr, x, y);
		y += du->get_height();
	}

	return false;
}

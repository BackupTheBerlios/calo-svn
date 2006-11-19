/* \file
 * Implementing the text view.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#undef DEBUG

#include <iostream>
#include <math.h>

#include <gdkmm/drawable.h>
#include <gdkmm/window.h>
#include <gtkmm/adjustment.h>
#include <gtkmm/scrollbar.h>
#include <gtkmm/widget.h>
#include <pangomm/layout.h>

#include "ViewWindow.h"
#include "AppContext.h"
#include "Feed.h"
#include "Item.h"


//======VIEWWINDOW====================================================

ViewWindow::ViewWindow()
{
	_darea._hadj = _hbar.get_adjustment();
	_darea._vadj = _vbar.get_adjustment();
	_darea._vadj->set_lower (0.0);
	_darea._hadj->set_lower (0.0);
	_darea._vadj->set_value (0.0);
	_darea._hadj->set_value (0.0);

	attach (_darea, 0, 1, 0, 1, Gtk::FILL|Gtk::EXPAND, Gtk::FILL|Gtk::EXPAND);
	attach (_hbar, 0, 1, 1, 2, Gtk::FILL|Gtk::EXPAND, Gtk::SHRINK);
	attach (_vbar, 1, 2, 0, 1, Gtk::SHRINK, Gtk::FILL|Gtk::EXPAND);
	
	_hbar.signal_value_changed().connect (sigc::mem_fun (_darea, 	
		&ViewDrawingArea::on_hvalue_changed));

	_vbar.signal_value_changed().connect (sigc::mem_fun (_darea, 
		&ViewDrawingArea::on_vvalue_changed));

	show_all_children();
}

ViewWindow::~ViewWindow()
{
}

///-----------------------------------------------------------------


//======VIEWDRAWINGAREA======================================================
ViewDrawingArea::ViewDrawingArea() 
: _top_item (NULL), _old_vval(0.0), _old_hval(0.0), _disp_mode_switched (false),
  _layout_prepared(false)
{
	modify_bg (Gtk::STATE_NORMAL, Gdk::Color ("white"));
	set_double_buffered (false);

	//signal_realize().connect_notify (sigc::mem_fun (*this,
	//	&ViewDrawingArea::on_realize));

	signal_expose_event().connect (sigc::mem_fun (*this,
		&ViewDrawingArea::on_expose_event));

	signal_configure_event().connect (sigc::mem_fun (*this,
		&ViewDrawingArea::on_configure_event));
}

///-----------------------------------------------------------------
/// Called when horizontal view scrollbar changes value.
void
ViewDrawingArea::on_hvalue_changed()
{
	draw_buffer();
	queue_draw();
}

/// Called when vertical view scrollbar changes value.
void
ViewDrawingArea::on_vvalue_changed()
{
	draw_buffer();
	queue_draw();
}


bool
ViewDrawingArea::on_configure_event (GdkEventConfigure *event)
{
#ifdef DEBUG
	std::cerr << "on_configure_event()" << std::endl << std::flush;
#endif
	if (_pixmap)
		_pixmap.clear();

	const Gtk::Allocation& all = get_allocation();
	_pixmap = Gdk::Pixmap::create (get_window(),
		all.get_width(),
		all.get_height());

	if (AppContext::get().get_feed() == NULL)
	{
		Cairo::RefPtr<Cairo::Context> cr = _pixmap->create_cairo_context();
		cr->set_source_rgb (1.0, 1.0, 1.0);
		cr->paint();
		cr->show_page();
	}
	else
	{
		Feed *feed = AppContext::get().get_feed();
		const item_list_t items = feed->get_items();
		for_each (items.begin(), items.end(), 
			std::mem_fun (&Item::reset_display_unit));
	}

	draw_buffer();

	return true;
}

bool
ViewDrawingArea::on_expose_event (GdkEventExpose* event)
{
#ifdef DEBUG
	std::cerr << "on_expose_event()" << std::endl << std::flush;
#endif
	get_window()->draw_drawable (get_style()->get_fg_gc (get_state()),
		_pixmap,
		event->area.x, event->area.y,
		event->area.x, event->area.y,
		event->area.width, event->area.height);

	return false;
}

void 
ViewDrawingArea::draw_buffer()
{
#ifdef DEBUG
std::cerr << "VA->val:"<<_vadj->get_value() <<" VA->upper:"<<_vadj->get_upper() << std::endl << std::flush;
#endif

	Feed *feed = AppContext::get().get_feed();
	if (feed == NULL) 
		return;

	// Dimensions of drawing area
	Gtk::Allocation allocation = get_allocation();
	const int width = allocation.get_width();
	const int height = allocation.get_height();

	// First determine start item
	Cairo::RefPtr<Cairo::Context> cr = _pixmap->create_cairo_context();

#if DEBUG
	/// This code was meant to investigate the 500ms delay with pango
	/// at start of rendering.
	if (!_layout_prepared)
	{
Glib::Timer sw;
sw.start();
		Glib::RefPtr<Pango::Layout> pl = Pango::Layout::create (cr);
		const char *fonts[] = {"Sans 10", "Sans 14"};
		for (unsigned i = 0; i < sizeof(fonts)/sizeof(const char*); ++i)
		{
			Pango::FontDescription fdesc (fonts[i]);
			pl->set_font_description (fdesc);
			pl->set_width (10 * Pango::SCALE); // force line break
			pl->set_markup ("<b>Show Us Your Freaky Geek Costumes</b>\nHitting the streets in a scary, tech-themed outfit this Halloween? We want to see it. Find out how your Sergey Brin costume (or is it David Duchovny?) could be featured on Wired News. test\n test");
			Pango::Rectangle irect, lrect;
			pl->get_extents (irect, lrect);
		}
		_layout_prepared = true;
sw.stop();
unsigned long l;
sw.elapsed(l);
std::cerr << "Time spent rendering: " << l << " us" << std::endl << std::flush;
	}
#endif

	const item_list_t items = feed->get_items();
	const int n_items = items.size();
	if (n_items == 0)
		return;
	
	// Set start_item and h
	int start = static_cast<int> (_vadj->get_value());
	item_list_t::const_iterator start_item;
	double h = 0.0, y;
	if (_vadj->get_value() > 0 && _vadj->get_value() == _vadj->get_upper())
	{
		// This will give a blank page when pulling the handle
		// full down. While unusual, it might make sense with
		// an ever-growing newslist.
		y = 0.0;
		start_item = items.end();
	}
	else
	{
		start_item = items.begin();
		for (int i = 0; i < start; ++i)
			++start_item;
		if (start < 0)
			start = 0;

		double prop = _vadj->get_value() - start;
		if (prop > 0.0)
		{
			(*start_item)->make_display_unit();
			ItemDisplayUnit *du = (*start_item)->get_display_unit();
			du->layout (cr);
			h = du->get_height();
		}
		y = - h * prop;
#ifdef DEBUG
std::cerr << "prop:"<<prop <<" y:"<<y << std::endl << std::flush;
#endif

	}

	// clip to the area indicated by the expose event so that we only redraw
	// the portion of the window that needs to be redrawn
	cr->reset_clip();
	cr->rectangle (0.0, 0.0, width, height);
	cr->clip();

	// Black on white
	cr->set_source_rgb (1.0, 1.0, 1.0);
	cr->paint();
	cr->set_source_rgb (0.0, 0.0, 0.0);

	// Render text
	item_list_t::const_iterator it = start_item;
	int count = 0;
	for (; it != items.end() && y < height; ++it, ++count)
	{
		(*it)->make_display_unit();
		ItemDisplayUnit *du = (*it)->get_display_unit();
		du->render (cr, _hadj->get_value(), y);
		y += du->get_height();
	}

	cr->show_page();

	// Scrollbar settings can now be specified
	_vadj->set_upper (n_items);
	//_vadj->set_page_size (count - 1 + );
	_vadj->set_step_increment (1);
	_vadj->set_page_increment (count);
	_hadj->set_upper (n_items);
	//_hadj->set_page_size (width);
	_hadj->set_step_increment (width/16.0);
	_hadj->set_page_increment (width*15.0/16.0);
	_hadj->changed();
	_vadj->changed();
}

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
#include "utils.h"


//======VIEWWINDOW====================================================

ViewWindow::ViewWindow()
{
	_darea._vadj = _vbar.get_adjustment();
	_darea._vadj->set_lower (0.0);
	_darea._vadj->set_value (0.0);

	attach (_darea, 0, 1, 0, 1, Gtk::FILL|Gtk::EXPAND, Gtk::FILL|Gtk::EXPAND);
	attach (_vbar, 1, 2, 0, 1, Gtk::SHRINK, Gtk::FILL|Gtk::EXPAND);
	
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
	set_events (Gdk::BUTTON_PRESS_MASK);

	signal_expose_event().connect (sigc::mem_fun (*this,
		&ViewDrawingArea::on_expose_event));

	signal_configure_event().connect (sigc::mem_fun (*this,
		&ViewDrawingArea::on_configure_event));

	signal_button_press_event().connect (sigc::mem_fun (*this,
		&ViewDrawingArea::on_button_press_event), false);
}

///-----------------------------------------------------------------
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
		const AppContext& ac = AppContext::get();
		if (ac.get_display_type() == FULL)
			ac.get_curr_item()->reset_display_unit();
		else
		{
			Feed *feed = ac.get_feed();
			const item_list_t items = feed->get_items();
			for_each (items.begin(), items.end(), 
				std::mem_fun (&Item::reset_display_unit));
		}
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

	if (AppContext::get().get_display_type() == FULL)
	{
		render_full_article();
		return;
	}

	// Dimensions of drawing area
	Gtk::Allocation allocation = get_allocation();
	const int width = allocation.get_width();
	const int height = allocation.get_height();

	// First determine start item
	Cairo::RefPtr<Cairo::Context> cr = _pixmap->create_cairo_context();

#ifdef DEBUG
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
	item_list_t::const_iterator start_item;
	const int n_items = items.size();
	double y = 0.0;

	if (n_items == 0)
		start_item = items.end();
	else
	{
		// Set start_item and h
		int start = static_cast<int> (_vadj->get_value());
		double h = 0.0;
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
	_top_item = *start_item;
	_topitem_y = y;
	int count = 0;
	for (; it != items.end() && y < height; ++it, ++count)
	{
		(*it)->make_display_unit();
		ItemDisplayUnit *du = (*it)->get_display_unit();
		du->render (cr, 0, y);
		y += du->get_height();
	}

	cr->show_page();

	// Scrollbar settings can now be specified
	_vadj->set_upper (n_items);
	//_vadj->set_page_size (count - 1 + );
	_vadj->set_step_increment (1);
	_vadj->set_page_increment (count);
	_vadj->changed();

}

/// Same as draw_buffer, with only the curr_item's full text
void
ViewDrawingArea::render_full_article()
{
	// Dimensions of drawing area
	Gtk::Allocation allocation = get_allocation();
	const int height = allocation.get_height();
	const int width = allocation.get_width();

	Cairo::RefPtr<Cairo::Context> cr = _pixmap->create_cairo_context();
	cr->reset_clip();
	cr->rectangle (0.0, 0.0, width, height);
	cr->clip();
	cr->set_source_rgb (1.0, 1.0, 1.0);
	cr->paint();
	cr->set_source_rgb (0.0, 0.0, 0.0);

	Item *item = AppContext::get().get_curr_item();
	item->make_display_unit();
	ItemDisplayUnit *du = item->get_display_unit();
	du->render (cr, 0, -_vadj->get_value());
	cr->show_page();
	
	double h = du->get_height();
	if (h > height)
		_vadj->set_upper (h - height);
	else
		_vadj->set_upper (0);

	_vadj->set_page_size (height);
	_vadj->set_step_increment (height * 1.0/16.0);
	_vadj->set_page_increment (height * 15.0/16.0);
	_vadj->changed();
}

bool
ViewDrawingArea::on_button_press_event (GdkEventButton *event)
{
#ifdef DEBUG
std::cerr << "ButtonEvent y = " << event->y << std::endl << std::flush;
#endif
	
	Feed *feed = AppContext::get().get_feed();
	if (feed == NULL)
		return true;
	
	Gtk::Allocation allocation = get_allocation();
	const int height = allocation.get_height();
	const item_list_t items = feed->get_items();
	item_list_t::const_iterator it = items.begin(); 
	for (; it != items.end(); ++it)
		if ((*it) == _top_item)
			break;
	double y = _topitem_y;
	for (; it != items.end() && y < height; ++it)
	{
		ItemDisplayUnit *du = (*it)->get_display_unit();
		y += du->get_height();
		if (event->y < y)
			break;
	}

	AppContext::get().set_curr_item (*it);
	_vadj->set_value (0.0);
	(*it)->reset_display_unit();

#ifdef DEBUG
if (it!=items.end())std::cerr << "Clicked: " << conv((*it)->_title) << std::endl << std::flush;
#endif

	return true;
}


// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4 :

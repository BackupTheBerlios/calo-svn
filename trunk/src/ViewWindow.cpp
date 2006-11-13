/* \file
 * Implementing the text view.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#undef DEBUG

#include <iostream>
#include <math.h>
#include <gtkmm/adjustment.h>
#include <gtkmm/scrollbar.h>
#include <gtkmm/widget.h>
#include <gdkmm/drawable.h>
#include <gdkmm/window.h>
#include "ViewWindow.h"
#include "AppContext.h"
#include "Feed.h"
#include "Item.h"


//======VIEWWINDOW====================================================

ViewWindow::ViewWindow()
{
	_darea._hadj = _hbar.get_adjustment();
	_darea._vadj = _vbar.get_adjustment();
	
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
: _top_item (NULL), _old_vval(0.0), _old_hval(0.0), _disp_mode_switched (false)
{
	_vadj->set_lower (0.0);
	_hadj->set_lower (0.0);
	_vadj->set_value (0.0);
	_hadj->set_value (0.0);

	modify_bg (Gtk::STATE_NORMAL, Gdk::Color ("white"));
	set_double_buffered (false);

	//_darea.signal_expose_event().connect (sigc::mem_fun (_darea,
	//	&ViewDrawingArea::on_expose_event));

}

///-----------------------------------------------------------------
/// Called when horizontal view scrollbar changes value.
/// TODO: The move_region() call appears to be inexact at times.
void
ViewDrawingArea::on_hvalue_changed()
{
	int new_hval = static_cast<int> (_hadj->get_value());
	int old_hval = static_cast<int> (_old_hval);
	if (abs (new_hval - old_hval) <= 1)
		return;
	
	// Dimensions of drawing area
	Gtk::Allocation area = get_allocation();

	if (abs (new_hval - old_hval) >= area.get_height())
	{
		draw_on_pixmap (0, 0, area.get_width(), area.get_height());
	}
	else
	{
		Gdk::Rectangle rect ((new_hval > old_hval)? (new_hval - old_hval) : 0,
			0,
			area.get_width() - abs (new_hval - old_hval),
			area.get_height());
		Gdk::Region reg (rect);
		get_window()->move_region (reg, old_hval - new_hval, 0);
		draw_on_pixmap ((new_hval > old_hval)? 0 : area.get_width() - abs (old_hval - new_hval),
			0,
			abs (old_hval - new_hval),
			area.get_height());
	}
	_old_hval = new_hval;
}

/// Called when vertical view scrollbar changes value.
/// TODO: The move_region() call appears to be inexact at times.
void
ViewDrawingArea::on_vvalue_changed()
{
	int new_vval = static_cast<int> (_vadj->get_value());
	int old_vval = static_cast<int> (_old_vval);
	if (abs (new_vval - old_vval) <= 1)
		return;
	
	// Dimensions of drawing area
	Gtk::Allocation area = get_allocation();

	if (abs (new_vval - old_vval) >= area.get_height())
	{
		draw_on_pixmap (0, 0, area.get_width(), area.get_height());
	}
	else
	{
		Gdk::Rectangle rect (0, 
			(new_vval > old_vval)? (new_vval - old_vval) : 0, 
			area.get_width(), 
			area.get_height() - abs (new_vval - old_vval));
		Gdk::Region reg (rect);
		get_window()->move_region (reg, 0, old_vval - new_vval);
		draw_on_pixmap (0, 
			(new_vval > old_vval)? 0 : 
				area.get_height() - abs (old_vval - new_vval),
			area.get_width(),
			abs (old_vval - new_vval));
	}
	_old_vval = new_vval;
}


bool
ViewDrawingArea::on_configure_event (GdkEventConfigure *event)
{
	if (_pixmap)
		_pixmap.clear();

	_pixmap = Gdk::Pixmap::create (get_window(),
		get_allocation().get_width(),
		get_allocation().get_height());

	//_pixmap->modify_bg (Gtk::STATE_NORMAL, Gdk::Color ("white"));
	//draw_on_pixmap();

	return true;
}

bool
ViewDrawingArea::on_expose_event (GdkEventExpose* event)
{
	get_window()->draw_drawable (get_style()->get_fg_gc (get_state()),
		_pixmap,
		event->area.x, event->area.y,
		event->area.x, event->area.y,
		event->area.width, event->area.height);

	return false;
}

void 
ViewDrawingArea::draw_on_pixmap (int ex, int ey, int ew, int eh)
{
#ifdef DEBUG
	std::cerr << "on_expose_event()" << std::endl << std::flush;
#endif
	Feed *feed = AppContext::get().get_feed();
	if (feed == NULL) 
		return;

#ifdef DEBUG
		std::cerr << "VA->val:"<<_vadj->get_value() <<" VA->lower:"<<_vadj->get_lower() <<" ev->y:"<<ey <<" ev->h:"<<eh << std::endl << std::flush;
#endif

	// First determine dimensions of text area
	Cairo::RefPtr<Cairo::Context> cr = _pixmap->create_cairo_context();
	const item_list_t items = feed->get_items();
	double h = 0.0, w = 0.0;
	for (item_list_t::const_iterator it = items.begin(); it != items.end(); ++it)
	{
		(*it)->make_display_unit();
		ItemDisplayUnit *du = (*it)->get_display_unit();
		du->layout (cr);
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
	_hadj->changed();
		
	// clip to the area indicated by the expose event so that we only redraw
	// the portion of the window that needs to be redrawn

	cr->reset_clip();
	cr->rectangle (ex, ey, ew, eh);
	cr->clip();

	// Black on white
	cr->set_source_rgb (1.0, 1.0, 1.0);
	cr->paint();
	cr->set_source_rgb (0.0, 0.0, 0.0);

	// Render text
	// 1. Go through items until the first to display
	item_list_t::const_iterator it = items.begin();

	if (!_disp_mode_switched)
	{
		while (y < ey)
		{
			double y_top = y;
			y += (*it)->get_display_unit()->get_height();
			if (y_top < 0.0 && y > 0.0)
				_top_item = *it;
			if (++it == items.end())
				break;
		}
		if (it != items.begin())
		{
			--it;
			y -= (*it)->get_display_unit()->get_height();
		}
	}
	else
	{
		/// Display mode was switched, full redraw.
		/// Start with _top_item, set _vadj accordingly.
		double h = 0.0;
		if (_top_item == NULL)
			_top_item = *(items.begin());
		while (it != items.end() && *it != _top_item)
			h += (*it++)->get_display_unit()->get_height();
		_disp_mode_switched = false;
		if (h > _vadj->get_upper() - height)
			h = _vadj->get_upper() - height;
		_vadj->set_value (h);
		y = 0.0; 
	}
	_vadj->changed(); // delayed because possibly the _disp_mode_switched
	
	// 2. Display all until the first not to display
	for (; it != items.end() && y < ey+eh; ++it)
	{
		ItemDisplayUnit *du = (*it)->get_display_unit();
		du->render (cr, x, y);
		y += du->get_height();
	}
}


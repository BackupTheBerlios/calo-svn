#ifndef __VIEW_WINDOW_H
#define __VIEW_WINDOW_H

/* \file
 * Part of the main view showing titles and other text.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <gtkmm/drawingarea.h>
#include <gtkmm/scrollbar.h>
#include <gtkmm/table.h>
#include <gdkmm/pixmap.h>
#include <cairomm/refptr.h>
#include <cairomm/context.h>

namespace Gtk { class Scrollbar; }
class Item;


/// ViewDrawingArea

class ViewDrawingArea : public Gtk::DrawingArea
{
public:
	ViewDrawingArea();

	void set_display_modus_switched() { _disp_mode_switched = true; }
	void draw_buffer();

private:
	friend class ViewWindow;
	bool on_expose_event (GdkEventExpose* event);
	void render_full_article();
	bool on_configure_event (GdkEventConfigure *event);
	bool on_button_press_event (GdkEventButton *event);
	void on_vvalue_changed();
	void on_hvalue_changed();

	Gtk::Adjustment 		*_vadj;
	Glib::RefPtr<Gdk::Pixmap> 	_pixmap;
	Item 				*_top_item;
	double 				_old_vval, _old_hval, _topitem_y;
	bool 				_disp_mode_switched, _layout_prepared;
};

/// ViewWindow

class ViewWindow : public Gtk::Table
{
public:
	ViewWindow();
	virtual ~ViewWindow();

	ViewDrawingArea _darea;

private:
	Gtk::HScrollbar _hbar;
	Gtk::VScrollbar _vbar;
};



#endif

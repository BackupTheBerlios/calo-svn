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
#include <cairomm/refptr.h>
#include <cairomm/context.h>

namespace Gtk { class Scrollbar; }
class Item;


/// ViewDrawingArea

class ViewDrawingArea : public Gtk::DrawingArea
{
public:
	ViewDrawingArea() : _top_item (NULL), _disp_mode_switched (false) {}

	bool on_expose_event (GdkEventExpose* event);
	void set_display_modus_switched() { _disp_mode_switched = true; }

	Gtk::Adjustment *_hadj, *_vadj;

private:
	Item *_top_item;
	bool _disp_mode_switched;
};

/// ViewWindow

class ViewWindow : public Gtk::Table
{
public:
	ViewWindow();
	virtual ~ViewWindow();

	Cairo::RefPtr<Cairo::Context> get_cairo_context() { return _cctx; }

	ViewDrawingArea _darea;
private:
	void on_vvalue_changed();
	void on_hvalue_changed();
	
	Cairo::RefPtr<Cairo::Context> _cctx;
	Gtk::HScrollbar _hbar;
	Gtk::VScrollbar _vbar;
	double _old_vval, _old_hval;
};



#endif

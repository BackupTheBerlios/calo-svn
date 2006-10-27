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


/// ViewDrawingArea

class ViewDrawingArea : public Gtk::DrawingArea
{
public:
	bool on_expose_event (GdkEventExpose* event);
	Gtk::Adjustment *_hadj, *_vadj;
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
	Cairo::RefPtr<Cairo::Context> _cctx;
	Gtk::HScrollbar _hbar;
	Gtk::VScrollbar _vbar;

	void on_value_changed (Gtk::Scrollbar* theBar);
};



#endif

#ifndef __VIEW_WINDOW_H
#define __VIEW_WINDOW_H

/* \file
 * Part of the main view showing titles and other text.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <gtkmm/drawingarea.h>
#include <gtkmm/scrolledwindow.h>
#include <cairomm/refptr.h>
#include <cairomm/context.h>


/// ViewDrawingArea

class ViewDrawingArea : public Gtk::DrawingArea
{
public:
	bool on_expose_event (GdkEventExpose* event);
	Gtk::Adjustment *_hadj, *_vadj;
};

/// ViewWindow

class ViewWindow : public Gtk::ScrolledWindow
{
public:
	ViewWindow();
	virtual ~ViewWindow();

	Cairo::RefPtr<Cairo::Context> get_cairo_context() { return _cctx; }

private:
	ViewDrawingArea _darea;
	Cairo::RefPtr<Cairo::Context> _cctx;
};



#endif

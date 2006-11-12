/* \file
 * Implementation of displaying an item on-screen by showing its header only.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <iostream>
#include <cairomm/context.h>

#include "AppContext.h"
#include "AppWindow.h"
#include "ViewContainer.h"
#include "ViewWindow.h"
#include "SimpleItemDU.h"
#include "Item.h"
#include "utils.h"


SimpleItemDU::SimpleItemDU (Item *theItem)
{
	_item = theItem;
	ViewWindow *vw = AppContext::get().get_appwindow()->get_viewcontainer()->get_viewwindow();
	_play = Pango::Layout::create (vw->_darea.get_cairo_context());

	// Link layout to widget
	Pango::Layout *p = _play.operator->();
	vw->signal_style_changed().connect (sigc::hide (sigc::mem_fun (*p, &Pango::Layout::context_changed)));
	vw->signal_direction_changed().connect (sigc::hide (sigc::mem_fun (*p, &Pango::Layout::context_changed)));
}

SimpleItemDU::~SimpleItemDU()
{
}

//---------------------------------------------------------------------
/// Write the item's title into the PangoLayout, sets _height
void
SimpleItemDU::layout()
{
#ifdef DEBUG
	std::cerr << "layouting " << conv(_item->_title) << std::endl << std::flush;
#endif
	Pango::FontDescription fdesc ("Sans 14");
	_play->set_font_description (fdesc);
	_play->set_markup (_item->_title.c_str());
	Glib::RefPtr<const Pango::LayoutLine> line = _play->get_line (0);
	Pango::Rectangle irect, lrect;
	line->get_extents (irect, lrect);
	_height = lrect.get_height()/1024.0;
	_width = lrect.get_width()/1024.0;
}

/// Render the PangoLayout (a single line of text)
void 
SimpleItemDU::render (const Cairo::RefPtr<Cairo::Context>& cctx, double x, double y)
{
#ifdef DEBUG
	std::cerr << "rendering " << conv(_item->_title) << std::endl << std::flush;
#endif
	Glib::RefPtr<Pango::LayoutLine> line = _play->get_line (0);
	Pango::LayoutIter iter;
	_play->get_iter (iter);
	int baseline = iter.get_baseline();
	cctx->move_to (x, y + baseline/1024.0);
	line->show_in_cairo_context (cctx);
}


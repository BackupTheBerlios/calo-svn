/* \file
 * Implementation of displaying an item on-screen by showing header plus desc.
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
#include "NormalItemDU.h"
#include "Item.h"
#include "utils.h"


NormalItemDU::NormalItemDU (Item *theItem)
{
	_item = theItem;
	ViewWindow *vw = AppContext::get().get_appwindow()->get_viewcontainer()->get_viewwindow();
	Cairo::RefPtr<Cairo::Context> cc = vw->get_window()->create_cairo_context();
	_play = Pango::Layout::create (cc);

	// Link layout to widget
	Pango::Layout *p = _play.operator->();
	vw->signal_style_changed().connect (sigc::hide (sigc::mem_fun (*p, &Pango::Layout::context_changed)));
	vw->signal_direction_changed().connect (sigc::hide (sigc::mem_fun (*p, &Pango::Layout::context_changed)));
}

NormalItemDU::~NormalItemDU()
{
}

//---------------------------------------------------------------------
/// Write the item's title & description into the PangoLayouts,  sets _height
void
NormalItemDU::layout()
{
#ifdef DEBUG
	std::cerr << "layouting " << conv(_item->_title) << std::endl << std::flush;
#endif
	Pango::FontDescription fdesc ("Sans 10");
	_play->set_font_description (fdesc);
	Glib::ustring s ("<h3>");
	s += _item->_title;
	s += "</h3><p>";
	s += _item->_description;
	s += "</p>";
	_play->set_markup (s.c_str());
	Glib::RefPtr<const Pango::LayoutLine> line = _play->get_line (0);
	Pango::Rectangle irect, lrect;
	line->get_extents (irect, lrect);
	_height = lrect.get_height()/1024.0;
	_width = lrect.get_width()/1024.0;
}

/// Render both PangoLayouts (a single line of header plus space plus body 
/// plus space).
void 
NormalItemDU::render (const Cairo::RefPtr<Cairo::Context>& cctx, double x, double y)
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


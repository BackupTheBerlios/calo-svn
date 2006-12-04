/* \file
 * Implementation of displaying an item on-screen by showing header plus desc.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#undef DEBUG

#include <iostream>
#ifdef DEBUG
#include <glibmm/timer.h>
#endif
#include <pangomm/layoutiter.h>
#include <pangomm/layoutline.h>

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
	_play.clear();
	_vw = AppContext::get().get_appwindow()->get_viewcontainer()->get_viewwindow();
}

NormalItemDU::~NormalItemDU()
{
}

//---------------------------------------------------------------------
/// Write the item's title & description into the PangoLayouts,  sets _height
void
NormalItemDU::layout (const Cairo::RefPtr<Cairo::Context>& cr)
{
	if (!_play)
		_play = Pango::Layout::create (cr);
	Pango::FontDescription fdesc ("Sans 10");
	_play->set_font_description (fdesc);
	Gtk::Allocation allocation = _vw->_darea.get_allocation();
	_play->set_width (static_cast<int> (allocation.get_width() * Pango::SCALE * 11.0/12.0));
	Glib::ustring s ("<b>");
	s += _item->_title;
	s += "</b>\n";
	s += _item->_description;
	s += "\n";
#ifdef DEBUG
Glib::Timer sw;
sw.start();
#endif

	_play->set_markup (s.c_str());
	Pango::Rectangle irect, lrect;
	_play->get_extents (irect, lrect);
	_height = lrect.get_height()/1024.0;
	_width = lrect.get_width()/1024.0;
	_has_layout = true;
#ifdef DEBUG
sw.stop();
unsigned long l;
sw.elapsed(l);
std::cerr << "Time spent rendering: " << l << " us" << std::endl << std::flush;
#endif
}

/// Render both PangoLayouts (a single line of header plus space plus body 
/// plus space).
void 
NormalItemDU::render (const Cairo::RefPtr<Cairo::Context>& cctx, double x, double y)
{
#ifdef DEBUG
	std::cerr << "rendering " << conv(_item->_title) << std::endl << std::flush;
#endif
	if (!_has_layout)
		layout (cctx);
	Pango::LayoutIter iter;
	_play->get_iter (iter);
	
	double start_y = 0;

	do
	{
		Glib::RefPtr<Pango::LayoutLine> line = iter.get_line();
		Pango::Rectangle dummy, lrect;
		iter.get_line_extents (dummy, lrect);
		int baseline = iter.get_baseline();

	//cctx->move_to (lrect.x/1024.0 + x, baseline/1024.0 - startpos);
		cctx->move_to (x, y + baseline/1024.0);
		line->show_in_cairo_context (cctx);
		start_y += lrect.get_height()/1024.0;
	}
	while (iter.next_line());

}



// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4 :

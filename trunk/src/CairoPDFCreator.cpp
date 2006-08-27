/* \file
 * Layouting text content into a PDF document using Cairo
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 *           (C) 2006 gtkmm development team
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <iostream>
#include <vector>
#include <cstring>
#include <cstdio>
#include <pango/pangocairo.h>
#include <pango/pango-types.h>
#include <pango/pango-layout.h>

#include "CairoPDFCreator.h"
#include "PDFContext.h"

CairoPDFCreator::CairoPDFCreator()
{
	PDFContext& pctx = PDFContext::get();
	_name = pctx.get_fname();
	_w = pctx.get_pdf_width();
	_h = pctx.get_pdf_height();
	
	_csfc = cairo_pdf_surface_create (_name.c_str(), _w, _h);
	_cctx = cairo_create (_csfc);

	_play = pango_cairo_create_layout (_cctx);
}

CairoPDFCreator::~CairoPDFCreator()
{
}

void CairoPDFCreator::operator<< (const Glib::ustring& text)
{
	PDFContext& pctx = PDFContext::get();
	PangoFontDescription *fdesc = pango_font_description_from_string (pctx.get_font().c_str());
	pango_layout_set_font_description (_play, fdesc);
	pango_layout_set_width (_play, static_cast<int>(pctx.get_layout_width() * PANGO_SCALE));
	pango_layout_set_markup (_play, text.c_str(), strlen (text.c_str()));

	const int linecount = pango_layout_get_line_count (_play);
	PangoLayoutLine *layoutline;
	double pageheight = 0;
	for (int line=0; line < linecount; ++line)
	{
		PangoRectangle irect, lrect;
		layoutline = pango_layout_get_line (_play, line);
		pango_layout_line_get_extents (layoutline, &irect, &lrect);
		const double lineheight = lrect.height/1024.0;
		if (pageheight + lineheight > pctx.get_layout_height())
		{
			_pagebreaks.push_back (line);
			pageheight = 0;
		}
		pageheight += lineheight;
	}
}

void CairoPDFCreator::save()
{
	double r, g, b;
	PDFContext::get().get_rgb_background (&r, &g, &b);
	cairo_set_source_rgb (_cctx, r, g, b);
	cairo_paint (_cctx);
	PDFContext::get().get_rgb_foreground (&r, &g, &b);
	cairo_set_source_rgb (_cctx, r, g, b);
	
	PangoLayoutIter *iter = pango_layout_get_iter (_play);
	double startpos = 0;
	int lineindex = 0;
	int nlines = pango_layout_get_line_count (_play);
	std::vector<int>::iterator last_line_page = _pagebreaks.begin();

	do
	{
		PangoLayoutLine *layoutline = pango_layout_iter_get_line (iter);
		PangoRectangle lrect;
		pango_layout_iter_get_line_extents (iter, NULL, &lrect);
		int baseline = pango_layout_iter_get_baseline (iter);

		if (lineindex == 0)
			startpos = lrect.y/1024.0;

		cairo_move_to (_cctx, lrect.x/1024.0, baseline/1024.0-startpos);
std::cerr << "ypos=" << baseline/1024.0-startpos << std::endl; std::cerr.flush();
		pango_cairo_show_layout_line (_cctx, layoutline);
		
		if (++lineindex > *last_line_page)
		{
			cairo_show_page (_cctx);
			cairo_destroy (_cctx);
			
			_cctx = cairo_create (_csfc);
			double r, g, b;
			PDFContext::get().get_rgb_background (&r, &g, &b);
			cairo_set_source_rgb (_cctx, r, g, b);
			cairo_paint (_cctx);
			PDFContext::get().get_rgb_foreground (&r, &g, &b);
			cairo_set_source_rgb (_cctx, r, g, b);

			startpos += PDFContext::get().get_layout_height();
			++last_line_page;
		}
	}
	while (lineindex < nlines && pango_layout_iter_next_line (iter));

	cairo_show_page (_cctx);
	cairo_destroy (_cctx);
	cairo_surface_destroy (_csfc);
}


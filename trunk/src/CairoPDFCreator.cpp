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

CairoPDFCreator::CairoPDFCreator (const Glib::ustring& name, double w, double h)
{
	_name = name;
	_w = w;
	_h = h;
	
	_csfc = cairo_pdf_surface_create (_name.c_str(), _w, _h);
	_cctx = cairo_create (_csfc);

	double r, g, b;
	PDFContext& pctx = PDFContext::get();
	pctx.get_rgb_background (&r, &g, &b);
	cairo_set_source_rgb (_cctx, r, g, b);
	cairo_paint (_cctx);

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
	pango_layout_set_width (_play, static_cast<int>(_w * PANGO_SCALE));
	pango_layout_set_markup (_play, text.c_str(), strlen (text.c_str()));

	const int linecount = pango_layout_get_line_count (_play);
std::cerr << "size=" << strlen (text.c_str()) << std::endl; std::cerr.flush();
	std::vector<int> pagebreaks;
	PangoLayoutLine *layoutline;
	double pageheight = 0;
	for (int line=0; line < linecount; ++line)
	{
		PangoRectangle irect, lrect;
		layoutline = pango_layout_get_line (_play, line);
		pango_layout_line_get_extents (layoutline, &irect, &lrect);
		const double lineheight = lrect.height/1024.0;
		if (pageheight + lineheight > _h)
		{
			pagebreaks.push_back (line);
			pageheight = 0;
		}
		pageheight += lineheight;
	}
}

void CairoPDFCreator::save()
{
	cairo_set_source_rgb (_cctx, 0,0,0);
	PangoLayoutIter *iter = pango_layout_get_iter (_play);
	double startpos = 0;
	int lineindex = 0;
	int nlines = pango_layout_get_line_count (_play);

	do
	{
		if (lineindex >= 0)
		{
			PangoLayoutLine *layoutline = pango_layout_iter_get_line (iter);
			PangoRectangle lrect;
			pango_layout_iter_get_line_extents (iter, NULL, &lrect);
			int baseline = pango_layout_iter_get_baseline (iter);

			if (lineindex == 0)
				startpos = lrect.y/1024.0;

			cairo_move_to (_cctx, lrect.x/1024.0, baseline/1024.0-startpos);
			pango_cairo_show_layout_line (_cctx, layoutline);
		}
		
	lineindex++;
	}
	while (lineindex < nlines && pango_layout_iter_next_line (iter));

	cairo_show_page (_cctx);
	cairo_destroy (_cctx);
	cairo_surface_destroy (_csfc);
}


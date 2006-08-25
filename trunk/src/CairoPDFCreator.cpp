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

CairoPDFCreator::CairoPDFCreator (const Glib::ustring& name, double w, double h)
{
	_name = name;
	_w = w;
	_h = h;
	
	_csfc = cairo_pdf_surface_create (_name.c_str(), _w, _h);
	_cctx = cairo_create (_csfc);
	cairo_set_source_rgb (_cctx, 1.0, 1.0, 1.0);
	cairo_paint (_cctx);
	_play = pango_cairo_create_layout (_cctx);
}

CairoPDFCreator::~CairoPDFCreator()
{
}

void CairoPDFCreator::operator<< (const Glib::ustring& text)
{
	PangoFontDescription *fdesc = pango_font_description_from_string ("Sans 16");
	pango_layout_set_font_description (_play, fdesc);
	pango_layout_set_width (_play, static_cast<int>(_w * PANGO_SCALE));

	char *ttext =
"                    <b>Lynx source distribution and potpourri</b>\
\n\
   Lynx is the text web browser. This is the top level page for the Lynx\
   software distribution site hosted by the Internet Software Consortium.\
\
   The current development sources have the latest version of Lynx\
   available (development towards 2.8.6). The main help page for\
   lynx-current is online; the current User Guide is part of the online\
   documentation.\
\
   The most recent stable release is lynx2-8-5. The main help page is\
   online, as well as the User Guide.\
\n\n\
   Other resources include:\
     * ftp and http mirrors\
     * Mailing list archives\
     * pgp/gpg signatures äöüß العربية\
     _________________________________________________________________\
\
   Viewable with any browser; valid HTML.\n\n\n";
//const char *p=text.c_str();while(*p){fprintf(stderr,"%X ",*p); ++p;}
	pango_layout_set_markup (_play, text.c_str(), text.size());

	const int linecount = pango_layout_get_line_count (_play);
std::cerr << "linecount=" << linecount << std::endl; std::cerr.flush();
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


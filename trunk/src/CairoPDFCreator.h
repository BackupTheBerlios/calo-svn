#ifndef __CAIRO_PDF_CREATOR_H
#define __CAIRO_PDF_CREATOR_H

/* \file
 * Layouting text content into a PDF document using the Cairo library
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <vector>
#include <cairo/cairo-pdf.h>
#include <pango/pango-layout.h>
#include "PDFCreator.h"

/// CairoPDFCreator

class CairoPDFCreator : public PDFCreator
{
public:
	CairoPDFCreator();
	virtual ~CairoPDFCreator();

	virtual void operator<< (const Glib::ustring& text);
	virtual void save();

private:
	Glib::ustring _name;
	double _w, _h;
	
	cairo_surface_t *_csfc;
	cairo_t *_cctx;
	PangoLayout* _play;
	std::vector<int> _pagebreaks;
};

#endif

/* \file
 * Implementation of the PDFContext singleton.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include "PDFContext.h"

static PDFContext* _theContext = NULL;

PDFContext& PDFContext::get()
{
	if (_theContext != NULL)
		return *_theContext;
	_theContext = new PDFContext;
	return *_theContext;
}

PDFContext::PDFContext()
{
	_font = "Sans 11";
	_rb = _gb = _bb = 1.0;
	_rf = _gf = _bf = 0.0;
}

const Glib::ustring& 
PDFContext::get_font() const
{
	return _font;
}

void PDFContext::get_rgb_background (double* r, double* g, double* b) const
{
	*r = _rb;
	*g = _gb;
	*b = _bb;
}

void PDFContext::get_rgb_foreground (double* r, double* g, double* b) const
{
	*r = _rf;
	*g = _gf;
	*b = _bf;
}

void PDFContext::set_font (const Glib::ustring& font)
{
	_font = font;
}

void PDFContext::set_rgb_background (double r, double g, double b)
{
	_rb = r;
	_gb = g;
	_bb = b;
}

void PDFContext::set_rgb_foreground (double r, double g, double b)
{
	_rf = r;
	_gf = g;
	_bf = b;
}


#ifndef __PDF_CONTEXT_H
#define __PDF_CONTEXT_H

/* \file
 * Singleton holding info important for PDF creation
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <glibmm/ustring.h>

/// PDFContext

class PDFContext
{
public:
	static PDFContext& get();

	const Glib::ustring& get_fname() const;
	const Glib::ustring& get_font() const;
	void get_rgb_background (double* r, double* g, double* b) const;
	void get_rgb_foreground (double* r, double* g, double* b) const;
	double get_pdf_width() const      { return _pdfw; }
	double get_pdf_height() const     { return _pdfh; }
	double get_layout_width() const   { return _layow; }
	double get_layout_height() const  { return _layoh; }
	double get_left_margin() const    { return _leftm; }
	double get_top_margin() const     { return _topm; }
	void set_fname (const Glib::ustring& fname);
	void set_font (const Glib::ustring& font);
	void set_rgb_background (double r, double g, double b);
	void set_rgb_foreground (double r, double g, double b);
	void set_pdf_width (double w)     { _pdfw = w; }
	void set_pdf_height (double h)    { _pdfh = h; }
	void set_layout_width (double w)  { _layow = w; }
	void set_layout_height (double h) { _layoh = h; } 
	void set_left_margin (double m)   { _leftm = m; }
	void set_top_margin (double m)    { _topm = m; }

private:
	PDFContext();
	
	Glib::ustring _font, _fname;
	double _rb, _gb, _bb, _rf, _gf, _bf;
	double _pdfw, _pdfh, _layow, _layoh, _leftm, _topm;
};

#endif


// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4 :

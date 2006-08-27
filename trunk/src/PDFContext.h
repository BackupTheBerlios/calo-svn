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

	const Glib::ustring& get_font() const;
	void get_rgb_background (double* r, double* g, double* b) const;
	void set_font (const Glib::ustring& font);
	void set_rgb_background (double r, double g, double b);

private:
	PDFContext();
	
	Glib::ustring _font;
	double _r, _g, _b;
};

#endif

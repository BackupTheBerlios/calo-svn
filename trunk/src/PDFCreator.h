#ifndef __PDF_CREATOR_H
#define __PDF_CREATOR_H

/* \file
 * Abstract base class for layouting text content into a PDF document
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <glibmm/ustring.h>

/// PDFCreator

class PDFCreator
{
public:
	static PDFCreator* create (const Glib::ustring& name, double w, double h);
	virtual ~PDFCreator() = 0;

	virtual void operator<< (const Glib::ustring& text) = 0;
	virtual void save() = 0;
};

#endif

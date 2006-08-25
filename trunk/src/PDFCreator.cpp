/* \file
 * Layouting text content into a PDF document
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 *           (C) 2006 gtkmm development team
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include "PDFCreator.h"
#include "CairoPDFCreator.h"

PDFCreator* PDFCreator::create (const Glib::ustring& name, double w, double h)
{
	return new CairoPDFCreator (name, w, h);
}

PDFCreator::~PDFCreator() {}


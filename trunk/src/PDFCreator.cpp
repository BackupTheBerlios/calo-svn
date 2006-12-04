/* \file
 * Layouting text content into a PDF document
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 *           (C) 2006 gtkmm development team
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include "PDFCreator.h"
#include "CairoPDFCreator.h"

PDFCreator* PDFCreator::create()
{
	return new CairoPDFCreator();
}

PDFCreator::~PDFCreator() {}



// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4 :

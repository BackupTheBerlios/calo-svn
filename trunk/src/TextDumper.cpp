/* \file
 * Interface for implementations rendering HTMLs text-only
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include "TextDumper.h"
#include "LynxTextDumper.h"

TextDumper* TextDumper::create()
{
	return new LynxTextDumper;
}

TextDumper::~TextDumper() {}




// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4 :

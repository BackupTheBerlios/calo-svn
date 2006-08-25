/* \file
 * Interface for implementations rendering HTMLs text-only
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include "TextDumper.h"
#include "LynxTextRenderer.h"

TextDumper* TextDumper::create()
{
	return new LynxTextRenderer;
}

TextDumper::~TextDumper() {}



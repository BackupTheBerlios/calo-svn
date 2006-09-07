/* \file
 * Processing the LynxTextDumper output such that it can be handed 
 * to the CairoPDFCreator.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include "LynxToCairoProcessor.h"

LynxToCairoProcessor::LynxToCairoProcessor()
{
}

void
LynxToCairoProcessor::set_uri (const Glib::ustring& uri)
{
	_uri = uri;
}

const Glib::ustring&
LynxToCairoProcessor::process (const Glib::ustring& dump)
{
	return dump;
}


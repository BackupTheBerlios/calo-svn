/* \file
 * Processing the LynxTextDumper output such that it can be handed 
 * to the CairoPDFCreator.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

//#include <iostream>

#include <glib/gmessages.h>
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
	// Replace possible HTML syntax characters
	Glib::ustring* str = new Glib::ustring (dump);
	size_t pos = 0;
	while ((pos = str->find_first_of ("<>&", pos)) != Glib::ustring::npos)
	{
		switch (str->at(pos)) 
		{
		case '<': str->replace (pos, 1, "&lt;", 4); break;
		case '>': str->replace (pos, 1, "&gt;", 4); break;
		case '&': str->replace (pos, 1, "&amp;", 5); break;
		default: 
			g_return_val_if_reached (*str);
		}

		++pos;
	}

	// Crop at bottom
	pos = str->find ("\n\n   Related Links"); // newsforge
	if (pos != Glib::ustring::npos)
		str->erase (pos);
	
	return *str;
}


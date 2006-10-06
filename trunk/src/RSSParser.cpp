/* \file
 * Implementation of parsing an RSS file.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include "RSSParser.h"

RSSParser::RSSParser (const Glib::ustring& str)
: xmlpp::DomParser()
{
	xmlpp::DomParser::parse_memory (str);
}

RSSParser::~RSSParser() {}





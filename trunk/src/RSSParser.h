#ifndef __RSS_PARSER_H
#define __RSS_PARSER_H

/* \file
 * Parsing an RSS file.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <glibmm/ustring.h>
#include <libxml++/parsers/domparser.h>

/// RSSParser

class RSSParser : public xmlpp::DomParser
{
public:
	RSSParser (const Glib::ustring& str);
	virtual ~RSSParser();

protected:
};



#endif

#ifndef __FEEDFILE_PARSER_H
#define __FEEDFILE_PARSER_H

/* \file
 * Reading feeds from file into tree model.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <glibmm/ustring.h>
#include <libxml++/parsers/saxparser.h>

/// FeedFileParser

class FeedFileParser : public xmlpp::SaxParser
{
public:
	FeedFileParser();
	virtual ~FeedFileParser();

private:
	
};

#endif

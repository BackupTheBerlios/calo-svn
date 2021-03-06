#ifndef __URI_FETCH_INFO_H
#define __URI_FETCH_INFO_H

/* \file
 * Container for messages to and from the URIFetcher
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <glibmm/ustring.h>

class URIFetchInfo
{
public:
	URIFetchInfo() : is_fetched(false), is_last(false) {}

	Glib::ustring html, uri;
	unsigned int no, status;
	bool is_fetched, is_last;
};



#endif


// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4 :

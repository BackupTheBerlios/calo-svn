#ifndef __URI_FETCH_INFO_H
#define __URI_FETCH_INFO_H

/* \file
 * Container for messages from the URIFetcher
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

//#include <glibmm/ustring.h>

class URIFetchInfo;
{
public:
	URIFetchInfo() : is_last(false) {}

	bool is_last;
};

#endif

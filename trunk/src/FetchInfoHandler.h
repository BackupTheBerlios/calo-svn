#ifndef __FETCH_INFO_HANDLER_H
#define __FETCH_INFO_HANDLER_H

/* \file
 * Abstract interface for classes handling fetched URIFetchInfo
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include "URIFetchInfo.h"

class FetchInfoHandler
{
public:
	virtual ~FetchInfoHandler() {}
	virtual void handle_header (const Glib::ustring&, const Glib::ustring&) = 0;
	virtual void quit_fetch (URIFetchInfo* info) = 0;
};



#endif


// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4 :

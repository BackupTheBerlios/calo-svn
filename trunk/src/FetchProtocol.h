#ifndef __FETCH_PROTOCOL_H
#define __FETCH_PROTOCOL_H

/* \file
 * Declarations for functions used for fetching from a feed.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <iostream>
#include <glibmm/ustring.h>
#include "FetchInfoHandler.h"

class FetchProtocol : public FetchInfoHandler 
{
public:
	static FetchProtocol* get();
	virtual void quit_fetch (URIFetchInfo* info);
	void run (Glib::ustring& uri);

private:
	FetchProtocol();
};

#endif

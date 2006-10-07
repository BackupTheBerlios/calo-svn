/* \file
 * Implementations of the fetch protocol.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include "FetchProtocol.h"
#include "URIFetcher.h"
#include "URIFetchInfo.h"

static FetchProtocol* _instance = NULL;

FetchProtocol* FetchProtocol::get()
{
	if (_instance != NULL)
		return _instance;
	_instance = new FetchProtocol;
	return _instance;
}

FetchProtocol::FetchProtocol() {}

void 
FetchProtocol::quit_fetch (URIFetchInfo* info)
{
std::cout<< info->html <<std::endl;
}

void FetchProtocol::run (Glib::ustring& uri)
{
	URIFetcher *fetcher = URIFetcher::create();
	fetcher->add_uri (uri);
	fetcher->set_pline (this);
	fetcher->start();
}


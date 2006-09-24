/* \file
 * Implementation of the AppContext singleton.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include "AppContext.h"

static AppContext* _theContext = NULL;

AppContext& AppContext::get()
{
	if (_theContext != NULL)
		return *_theContext;
	_theContext = new AppContext;
	return *_theContext;
}

AppContext::AppContext()
{
}

const Glib::ustring&
AppContext::get_feeds_filename()
{
	static Glib::ustring name ("/home/ralf/.calo/feeds.opml");
	return name;
}


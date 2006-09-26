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
	_app_x = _app_y = _app_w = _app_h = 0;
	_cfg_was_read = false;
}

const Glib::ustring&
AppContext::get_feeds_filename()
{
	static Glib::ustring name ("/home/ralf/.calo/feeds.opml");
	return name;
}

bool 
AppContext::get_appwindow_pos (unsigned int* x, unsigned int* y, unsigned int* w, unsigned int* h)
{
	*x = _app_x;
	*y = _app_y;
	*w = _app_w;
	*h = _app_h;
	return _cfg_was_read;
}

void 
AppContext::set_appwindow_pos (unsigned int x, unsigned int y,	unsigned int w, unsigned int h)
{
	_app_x = x;
	_app_y = y;
	_app_w = w;
	_app_h = h;
}



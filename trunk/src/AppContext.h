#ifndef __APP_CONTEXT_H
#define __APP_CONTEXT_H

/* \file
 * Singleton holding info important for the application
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <glibmm/ustring.h>

/// AppContext

class AppContext
{
public:
	static AppContext& get();

	const Glib::ustring& get_feeds_filename();
	bool get_appwindow_pos (unsigned int* x, unsigned int* y,
			unsigned int* w, unsigned int* h);
	void set_appwindow_pos (unsigned int x, unsigned int y,
			unsigned int w, unsigned int h);
	bool cfg_was_read() { return _cfg_was_read; }

private:
	AppContext();
	
	unsigned int _app_x, _app_y, _app_w, _app_h;
	bool _cfg_was_read;
};

#endif

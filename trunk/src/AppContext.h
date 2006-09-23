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

private:
	AppContext();
	
};

#endif

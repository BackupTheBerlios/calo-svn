/* \file
 * Implementations of small utility functions.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <time.h>
#include <glibmm/convert.h>


void sleepms (unsigned long ms)
{
	struct timespec ti;
	ti.tv_sec = ms / 1000L;
	ti.tv_nsec = (ms - 1000L*int(ms/1000L)) * 1000000L;
	nanosleep (&ti, &ti);
}

std::string conv (const std::string& str)
{
	const std::string& to = "";
	const std::string& from = "UTF-8";
	const Glib::ustring& fallback = "?";
	
	return Glib::convert_with_fallback (str, to, from, fallback);
}


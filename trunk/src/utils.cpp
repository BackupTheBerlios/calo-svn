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


void sleepms (unsigned long ms)
{
	struct timespec ti;
	ti.tv_sec = ms / 1000L;
	ti.tv_nsec = (ms - 1000L*int(ms/1000L)) * 1000000L;
	nanosleep (&ti, &ti);
}


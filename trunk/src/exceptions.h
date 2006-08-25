#ifndef __EXCEPTIONS_H
#define __EXCEPTIONS_H

/* \file
 * Declarations for all exceptions
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <glibmm/ustring.h>

class PipeException 
{
public:
	PipeException() { _str = _("Pipe problem: "); }
	PipeException (const Glib::ustring& s) { _str += s; }

	Glib::ustring _str;
};

class TempfileException 
{
public:
	TempfileException() { _str = _("Tempfile problem: "); }
	TempfileException (const Glib::ustring& s) { _str += s; }

	Glib::ustring _str;
};

#endif

#ifndef __EXCEPTIONS_H
#define __EXCEPTIONS_H

/* \file
 * Declarations for all exceptions
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <errno.h>
#include <glibmm/ustring.h>
#include <glibmm/stringutils.h>
#include <glibmm/i18n.h>

class PipeException 
{
public:
	PipeException() { _str = _("Pipe problem: ") + Glib::strerror (errno); }
	PipeException (const Glib::ustring& s) { _str += s; }

	Glib::ustring _str;
};

class TempfileException 
{
public:
	TempfileException() { _str = _("Tempfile problem: ") + Glib::strerror (errno); }
	TempfileException (const Glib::ustring& s) { _str += s; }

	Glib::ustring _str;
};

class FileException 
{
public:
	FileException() { _str = _("File problem: ") + Glib::strerror (errno); }
	FileException (const Glib::ustring& s) { _str += s; }

	Glib::ustring _str;
};

#endif

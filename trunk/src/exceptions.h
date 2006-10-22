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
#include <glibmm/exception.h>
#include <glibmm/stringutils.h>
#include <glibmm/i18n.h>

class PipeException : public Glib::Exception
{
public:
	PipeException() { _str = _("Pipe problem: ") + Glib::strerror (errno); }
	PipeException (const Glib::ustring& s) { _str = s; }

	virtual ~PipeException() throw() {}
	virtual Glib::ustring what() const { return _str; }

	Glib::ustring _str;
};

class TempfileException  : public Glib::Exception
{
public:
	TempfileException() { _str = _("Tempfile problem: ") + Glib::strerror (errno); }
	TempfileException (const Glib::ustring& s) { _str = s; }

	virtual ~TempfileException() throw() {}
	virtual Glib::ustring what() const { return _str; }

	Glib::ustring _str;
};

class FileException  : public Glib::Exception
{
public:
	FileException() { _str = _("File problem: ") + Glib::strerror (errno); }
	FileException (const Glib::ustring& s) { _str = s; }

	virtual ~FileException() throw() {}
	virtual Glib::ustring what() const { return _str; }

	Glib::ustring _str;
};

class MiscException  : public Glib::Exception
{
public:
	MiscException (const Glib::ustring& s) { _str = _("Problem encountered: ") + s; }

	virtual ~MiscException() throw() {}
	virtual Glib::ustring what() const { return _str; }

	Glib::ustring _str;
};

#endif

#ifndef __TEMP_FILE_H
#define __TEMP_FILE_H

/* \file
 * Interface for implementations rendering HTMLs text-only
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <glibmm/ustring.h>

/// TempFile

class TempFile
{
public:
	TempFile();
	~TempFile();

	const char* name() const	{ return _fname; }
	int fd() const			{ return _fdes; }
	void operator<< (const Glib::ustring& str);

private:
	char _fname[16];
	int _fdes;
};

#endif

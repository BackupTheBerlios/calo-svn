#ifndef __CONFIG_FILE_H
#define __CONFIG_FILE_H

/* \file
 * Interface for implementations rendering HTMLs text-only
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <glibmm/ustring.h>

/// ConfigFile

class ConfigFile
{
public:
	ConfigFile (const Glib::ustring& fname);
	~ConfigFile();

	void init();
	void read();
	void save();
	bool has (const Glib::ustring& prop);
	int get_i (const Glib::ustring& prop);
	void set (const Glib::ustring& prop, int i);
	

private:
	Glib::ustring _fname;
};

#endif

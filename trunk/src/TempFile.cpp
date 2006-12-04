/* \file
 * Creating unique temporary files in /tmp
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <fstream>
#include <stdlib.h>
#include <errno.h>
#include <stddef.h>
#include <unistd.h>
#include <glibmm/stringutils.h>
#include <glibmm/i18n.h>

#include "TempFile.h"
#include "exceptions.h"

/// TODO: Either C or C++ file i/o but not both

/// Check to see if directory exists by attempting to open a new file
/// for output within it. Stolen from Bruce Eckel.
static bool exists (Glib::ustring fname) 
{
	size_t len = fname.length();
	if(fname[len-1] != '/' && fname[len-1] != '\\')
	fname.append("/");
	fname.append("000.tmp");
	std::ofstream outf(fname.c_str());
	bool existFlag = outf;
	if (outf) 
	{
		outf.close();
		remove(fname.c_str());
	}
	return existFlag;
}

/// Create the temporary file
TempFile::TempFile()
{
	char path[32];
	path[0] = '\0';
	if (exists ("/tmp"))
		strcpy (path, "/tmp/");
	else if (exists ("/var/tmp"))
		strcpy (path, "/var/tmp/");
	strcat (path, "calo.tmpXXXXXX");
	_fdes = mkstemp (path);
	_fname = path;
	if (_fdes < 0)
		throw TempfileException (_("Could not create.\n"));
}

/// Closes and destroys the file
TempFile::~TempFile()
{
	int ret = close (_fdes);
	if (ret)
		throw TempfileException (_("Could not close.\n"));

	ret = unlink (_fname.c_str());
	if (ret)
		throw TempfileException (_("Could not remove.\n"));
}

/// Write the contents of the string into the tempfile
void TempFile::operator<< (const Glib::ustring& str)
{
	size_t nbytes = str.size();
	const char *buf = str.data();
	while (nbytes > 0)
	{
		int n = TEMP_FAILURE_RETRY (write (_fdes, buf, nbytes));
		if (n < 0)
			throw TempfileException (Glib::strerror (errno));
		nbytes -= n;
		buf += n;
	}
}




// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4 :

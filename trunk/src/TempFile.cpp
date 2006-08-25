/* \file
 * Creating unique temporary files in /tmp
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <stdlib.h>
#include <errno.h>
#include <stddef.h>
#include <unistd.h>
#include <glibmm/stringutils.h>
#include <glibmm/i18n.h>

#include "TempFile.h"
#include "exceptions.h"


/// Create the temporary file
TempFile::TempFile()
{
	/// TODO 
	/// Use /tmp or /var/tmp
	strcpy (_fname, "calo.tmpXXXXXX");
	_fdes = mkstemp (_fname);
	if (_fdes < 0)
		throw TempfileException (_("Could not create.\n"));
}

/// Closes and destroys the file
TempFile::~TempFile()
{
	int ret = close (_fdes);
	if (ret)
		throw TempfileException (_("Could not close.\n"));

	ret = unlink (_fname);
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




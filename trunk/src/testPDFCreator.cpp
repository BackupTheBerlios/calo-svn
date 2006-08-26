/* \file
 * The main testPDFCreator source file.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <fstream>
#include <sys/stat.h>

#include "exceptions.h"
#include "PDFCreator.h"

/// Returns raw file size.
off_t get_file_size (char *fname)
{
	struct stat stats;
	if (stat (fname, &stats) != 0)
		throw FileException();
	return stats.st_size;
}

int main (int argc , char **argv)
{
	std::ifstream dumpfile (argv[1], std::_S_bin);
	off_t filesize = get_file_size (argv[1]);
	char *buf = new char (filesize + 1);
	dumpfile.read (buf, filesize);
	buf[filesize] = '\0';

	PDFCreator *pdf = PDFCreator::create ("test.pdf", 580, 860);
	Glib::ustring str (buf);
	*pdf << str;
	pdf->save();

	delete pdf;
	delete buf;
}




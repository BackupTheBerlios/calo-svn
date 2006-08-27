/* \file
 * The main testPDFCreator source file.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <cstdio>
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

/// Read a block of data into buf. We know the size of the file to read.
void read_file_buf (char *fname, char *buf, off_t size)
{
	FILE *fp = fopen (fname, "rb");
	if (fp == NULL)
		throw FileException();
	size_t s;
	while ((s = fread (buf, sizeof(char), size, fp)) > 0)
	{
		size -= s;
		buf += s;
	}
	fclose (fp);
}

int main (int argc , char **argv)
{
	off_t filesize = get_file_size (argv[1]);
	char *buf = (char*) malloc (filesize + 1);
	read_file_buf (argv[1], buf, filesize);
	buf[filesize] = '\0';

	PDFCreator *pdf = PDFCreator::create();
	Glib::ustring str (buf);
	*pdf << str;
	pdf->save();

	delete pdf;
	free (buf);
}




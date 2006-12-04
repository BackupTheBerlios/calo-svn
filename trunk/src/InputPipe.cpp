/* \file
 * Implementation of an input pipe. A file is opened for reading by
 * an external program, the output of which can be put into a string.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <stdio.h>
#include <errno.h>
#include <glibmm/stringutils.h>
#include <glibmm/i18n.h>

#include "InputPipe.h"
#include "exceptions.h"

#define LINEBUFLEN 1024

/// Opens the input pipe
InputPipe::InputPipe (const Glib::ustring& cmd)
{
	_fp = popen (cmd.c_str(), "r");
	if (!_fp)
		throw PipeException (Glib::strerror (errno));
}

/// Call pclose on th pipe.
InputPipe::~InputPipe()
{
	if (pclose (_fp) < 0)
	{
		throw PipeException (_("pclose() returned nonzero value."));
	}
}

/// Write everything from the pipe into the string
void InputPipe::operator>> (Glib::ustring &str)
{
	char *line = (char *) malloc (LINEBUFLEN);
	size_t len = LINEBUFLEN;
	ssize_t nbytes = 0;
	
	while (nbytes >= 0)
	{
		nbytes = getline (&line, &len, _fp);
		/// TODO 
		/// Should convert from HTML encoding to UTF-8
		if (nbytes > 0) 
			str.append (line);
	}

	free (line);
}




// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4 :

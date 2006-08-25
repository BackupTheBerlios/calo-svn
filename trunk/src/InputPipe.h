#ifndef __INPUT_PIPE_H
#define __INPUT_PIPE_H

/* \file
 * Helper that allows piping a file through an external command.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <stdio.h>
#include <glibmm/ustring.h>


/// InputPipe

class InputPipe
{
public:
	InputPipe (const Glib::ustring& cmd);
	~InputPipe();

	void operator>> (Glib::ustring &str);

private:
	FILE *_fp;
};



#endif

#ifndef __DUMP_PROCESSOR_H
#define __DUMP_PROCESSOR_H

/* \file
 * Interface for implementations processing the TextDumper output
 * such that it can be handed to the PDFCreator.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <glibmm/ustring.h>

/// DumpProcessor

class DumpProcessor
{
public:
	static DumpProcessor* create();
	virtual ~DumpProcessor() = 0;
	virtual void set_uri (const Glib::ustring&) = 0;
	virtual void set_title (const Glib::ustring&) = 0;
	virtual const Glib::ustring& process (const Glib::ustring& dump) = 0;
};

#endif

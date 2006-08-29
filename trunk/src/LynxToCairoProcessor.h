#ifndef __LYNX2CAIRO_PROCESSOR_H
#define __LYNX2CAIRO_PROCESSOR_H

/* \file
 * Interface for processing the LynxTextDumper output
 * such that it can be handed to the CairoPDFCreator.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <glibmm/ustring.h>
#include "DumpProcessor.h"

/// LynxToCairoProcessor

class LynxToCairoProcessor : public DumpProcessor
{
public:
	LynxToCairoProcessor();
	virtual ~LynxToCairoProcessor() {}

	virtual const Glib::ustring& process (const Glib::ustring& dump);
};

#endif

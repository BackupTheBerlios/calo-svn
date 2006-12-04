/* \file
 * Interface for implementations processing the TextDumper output
 * such that it can be handed to the PDFCreator.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include "DumpProcessor.h"
#include "LynxToCairoProcessor.h"

DumpProcessor* DumpProcessor::create()
{
	return new LynxToCairoProcessor;
}

DumpProcessor::~DumpProcessor() {}




// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4 :

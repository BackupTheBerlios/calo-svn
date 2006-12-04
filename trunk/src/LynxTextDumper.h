#ifndef __LYNX_TEXT_RENDERER_H
#define __LYNX_TEXT_RENDERER_H

/* \file
 * Interface for implementations rendering HTMLs text-only
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <glibmm/ustring.h>
#include "TextDumper.h"

/// LynxTextDumper

class LynxTextDumper : public TextDumper
{
public:
	virtual ~LynxTextDumper() {}
	virtual const Glib::ustring& render (const Glib::ustring& html);
};

#endif


// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4 :

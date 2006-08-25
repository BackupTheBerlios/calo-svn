#ifndef __LYNX_TEXT_RENDERER_H
#define __LYNX_TEXT_RENDERER_H

/* \file
 * Interface for implementations rendering HTMLs text-only
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <glibmm/ustring.h>
#include "TextRenderer.h"

/// LynxTextRenderer

class LynxTextRenderer:public TextRenderer
{
public:
	virtual ~LynxTextRenderer() {}
	virtual const Glib::ustring& render (const Glib::ustring& html);
};

#endif

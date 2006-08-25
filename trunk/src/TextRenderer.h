#ifndef __TEXT_RENDERER_H
#define __TEXT_RENDERER_H

/* \file
 * Interface for implementations rendering HTMLs text-only
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <glibmm/ustring.h>

/// TextRenderer

class TextRenderer
{
public:
	static TextRenderer* create();
	virtual ~TextRenderer() = 0;
	virtual const Glib::ustring& render (const Glib::ustring& html) = 0;
};

#endif

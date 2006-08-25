/* \file
 * Interface for implementations rendering HTMLs text-only
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include "TextRenderer.h"
#include "LynxTextRenderer.h"

TextRenderer* TextRenderer::create()
{
	return new LynxTextRenderer;
}

TextRenderer::~TextRenderer() {}



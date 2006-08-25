/* \file
 * Implementation rendering HTMLs text-only using Lynx
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include "LynxTextRenderer.h"
#include "TempFile.h"
#include "InputPipe.h"

/// Renders HTML into a new text string wich has to be deleted by the user.
/// Opens/closes a temp file and an input pipe from the Lynx browser.
const Glib::ustring& LynxTextRenderer::render (const Glib::ustring& html)
{
	TempFile tmp;
	tmp << html;
	Glib::ustring cmd = Glib::ustring ("lynx -dump -force-html -nolist -assume_charset=utf-8 -display_charset=utf-8 ") + tmp.name();
	InputPipe pipe (cmd);
	Glib::ustring *str = new Glib::ustring;
	pipe >> *str;

	return *str;
}




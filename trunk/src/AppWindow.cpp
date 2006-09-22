/* \file
 * Implementation of topmost application window.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include "AppWindow.h"


AppWindow::AppWindow()
{
	set_title ("Calo");
	set_border_width (10);
	set_default_size (450, 400);

	add (_vpaned);

	_vpaned.add1 (_flist);
	_vpaned.add2 (_hlist);

	show_all_children();
}

AppWindow::~AppWindow()
{
}

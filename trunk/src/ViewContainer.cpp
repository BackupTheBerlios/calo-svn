/* \file
 * Implementing the main view pane.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include "ViewContainer.h"


ViewContainer::ViewContainer()
{
	add (_vw);
	show_all_children();
}

ViewContainer::~ViewContainer()
{
}

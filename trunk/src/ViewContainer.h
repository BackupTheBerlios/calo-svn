#ifndef __VIEW_CONTAINER_H
#define __VIEW_CONTAINER_H

/* \file
 * Pane containing the main view (view window and filter widgets).
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <gtkmm/box.h>
#include "ViewWindow.h"


/// ViewContainer

class ViewContainer : public Gtk::HBox
{
public:
	ViewContainer();
	virtual ~ViewContainer();

private:
	ViewWindow _vw;
};



#endif

#ifndef __HEADER_LIST_H
#define __HEADER_LIST_H

/* \file
 * Pane containing the main view.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <gtkmm/scrolledwindow.h>


/// HeaderList

class HeaderList : public Gtk::ScrolledWindow
{
public:
	HeaderList();
	virtual ~HeaderList();

private:
};



#endif

#ifndef __VIEW_WINDOW_H
#define __VIEW_WINDOW_H

/* \file
 * Part of the main view showing titles and other text.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <gtkmm/scrolledwindow.h>


/// ViewWindow

class ViewWindow : public Gtk::ScrolledWindow
{
public:
	ViewWindow();
	virtual ~ViewWindow();

private:
};



#endif

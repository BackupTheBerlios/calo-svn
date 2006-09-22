#ifndef __FEED_LIST_H
#define __FEED_LIST_H

/* \file
 * Pane containing the feed hierarchy.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <gtkmm/scrolledwindow.h>


/// FeedList

class FeedList : public Gtk::ScrolledWindow
{
public:
	FeedList();
	virtual ~FeedList();

private:
};



#endif

#ifndef __FEED_LIST_COLUMN_RECORD_H
#define __FEED_LIST_COLUMN_RECORD_H

/* \file
 * What records the feed list tree model columns contain.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <gtkmm/treemodelcolumn.h>
#include "Feed.h"

/// FeedListColumnRecord

class FeedListColumnRecord : public Gtk::TreeModelColumnRecord
{
public:
	static class FeedListColumnRecord& get();

	Gtk::TreeModelColumn<Glib::ustring> _col_string;
	Gtk::TreeModelColumn<Glib::ustring> _col_url;
	Gtk::TreeModelColumn<Feed*> _col_feed;

private:
	FeedListColumnRecord();
};


#endif

#ifndef __FEED_LIST_H
#define __FEED_LIST_H

/* \file
 * Pane containing the feed hierarchy.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <gtkmm/scrolledwindow.h>
#include <gtkmm/treemodelcolumn.h>
#include <gtkmm/treestore.h>
#include <gtkmm/treeview.h>


/// FeedList

class FeedList : public Gtk::ScrolledWindow
{
public:
	FeedList();
	virtual ~FeedList();

protected:
	class StringModelColumn : public Gtk::TreeModelColumnRecord
	{
	public:
		StringModelColumn() { add (_col_string); }

		Gtk::TreeModelColumn<Glib::ustring> _col_string;
	};

	StringModelColumn 		_smcol;
	Glib::RefPtr<Gtk::TreeStore> 	_tstore;
	Gtk::TreeView 			_tview;
};



#endif

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
#include "FeedListColumnRecord.h"

/// FeedList

class FeedList : public Gtk::ScrolledWindow
{
public:
	FeedList();
	virtual ~FeedList();

	Glib::RefPtr<Gtk::TreeModel> get_tmodel() { return _tview.get_model(); }

protected:

	void on_tview_button_press (GdkEventButton*);
	void on_tview_key_press (GdkEventKey*);
	void on_tview_motion_notify (GdkEventMotion*);
	void on_selection_changed();

	void delete_row (const Gtk::TreeModel::iterator& iter);

	FeedListColumnRecord 		*_smcol;
	Glib::RefPtr<Gtk::TreeStore> 	_tstore;
	Gtk::TreeView 			_tview;
};



#endif


// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4 :

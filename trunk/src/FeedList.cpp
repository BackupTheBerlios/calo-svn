/* \file
 * Implementing the feed hierachy pane.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include "FeedList.h"


FeedList::FeedList()
{
	add (_tview);
	set_policy (Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	_tstore = Gtk::TreeStore::create (_smcol);
	_tview.set_model (_tstore);
	_tview.set_reorderable();

	show_all_children();
}

FeedList::~FeedList()
{
}

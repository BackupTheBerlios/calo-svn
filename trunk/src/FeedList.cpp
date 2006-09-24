/* \file
 * Implementing the feed hierachy pane.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include "AppContext.h"
#include "FeedList.h"
#include "FeedFileParser.h"

FeedList::FeedList()
{
	add (_tview);
	set_policy (Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	_tstore = Gtk::TreeStore::create (_smcol);
	_tview.set_model (_tstore);
	_tview.set_reorderable();

	FeedFileParser _parser (_tstore);
	_parser.set_substitute_entities (true);
	_parser.parse_file (AppContext::get().get_feeds_filename());

	show_all_children();
}

FeedList::~FeedList()
{
}

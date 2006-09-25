/* \file
 * Implementing the feed hierachy pane.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <iostream>
//#include <sigc++/sigc++.h>
#include <gtkmm/treeselection.h>
#include "AppContext.h"
#include "FeedList.h"
#include "FeedFileParser.h"
#include "URIFetcher.h"

FeedList::FeedList()
{
	add (_tview);
	set_policy (Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	_tstore = Gtk::TreeStore::create (_smcol);
	_tview.set_model (_tstore);
	_tview.set_reorderable();

	try 
	{
		FeedFileParser _parser (_tstore);
		_parser.set_column_record (&_smcol);
		_parser.set_substitute_entities (true);
		_parser.parse_file (AppContext::get().get_feeds_filename());
	}
	catch (const xmlpp::exception& ex)
	{
		std::cerr << "parse exception: " << ex.what() << std::endl;
	}

	_tview.append_column ("Feeds", _smcol._col_string);

	show_all_children();

	Glib::RefPtr<Gtk::TreeSelection> _slctn = _tview.get_selection();
	_slctn->signal_changed().connect (sigc::mem_fun (*this, &FeedList::on_selection_changed));
}

FeedList::~FeedList()
{
}

void 
FeedList::quit_fetch (URIFetchInfo* info)
{
std::cout<< info->html <<std::endl;
}

void 
FeedList::on_selection_changed()
{
	Glib::RefPtr<Gtk::TreeSelection> _slctn = _tview.get_selection();
	Gtk::TreeModel::iterator iter = _slctn->get_selected();
	Glib::ustring str = (*iter)[_smcol._col_url];
std::cout<<"selected: " << str << std::endl;
	URIFetcher *fetcher = URIFetcher::create();
	fetcher->add_uri (str);
	fetcher->set_pline (this);
	fetcher->start();
}

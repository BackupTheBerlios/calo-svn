/* \file
 * Implementing the feed hierachy pane.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <iostream>
#include <gtkmm/treeselection.h>
#include "AppContext.h"
#include "FeedList.h"
#include "OPMLParser.h"
#include "FetchProtocol.h"

FeedList::FeedList()
{
	add (_tview);
	set_policy (Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
	_smcol = &FeedListColumnRecord::get();
	_tstore = Gtk::TreeStore::create (*_smcol);
	_tview.set_model (_tstore);
	_tview.set_reorderable();

	try 
	{
		OPMLParser _parser (_tstore);
		_parser.set_column_record (_smcol);
		_parser.set_substitute_entities (true);
		_parser.parse_file (AppContext::get().get_feeds_filename());
	}
	catch (const xmlpp::exception& ex)
	{
		std::cerr << "parse exception: " << ex.what() << std::endl;
	}

	_tview.append_column ("Feeds", _smcol->_col_string);

	show_all_children();

	Glib::RefPtr<Gtk::TreeSelection> _slctn = _tview.get_selection();
	_slctn->signal_changed().connect (sigc::mem_fun (*this, &FeedList::on_selection_changed));
	_tview.signal_event().connect (sigc::mem_fun (*this, &FeedList::on_event));
}

FeedList::~FeedList()
{
}

void 
FeedList::select (Gtk::TreeModel::iterator it)
{
	Glib::ustring uri = (*it)[_smcol->_col_url];
	Feed* feed = (*it)[_smcol->_col_feed];
	AppContext::get().set_feed (feed);
	FetchProtocol::get()->run (uri, feed);
}

//-----------------------------------------------------------------------
void 
FeedList::on_selection_changed()
{
//	Glib::RefPtr<Gtk::TreeSelection> _slctn = _tview.get_selection();
//	select (_slctn->get_selected());
}

void
FeedList::on_delete()
{
std::cerr<<"FeedList::on_delete()"<<std::endl<<std::flush;
}

bool
FeedList::on_event (GdkEvent* event)
{
	Gdk::Event ev (event);
	GdkEventType type = event->type;
	if (type == GDK_BUTTON_PRESS)
	{
		double dx, dy;
		int x, y, cell_x, cell_y;
		ev.get_coords (dx, dy);
		x = static_cast<int> (dx);
		y = static_cast<int> (dy);
		Gtk::TreeModel::Path path;
		Gtk::TreeViewColumn *col;
		_tview.get_path_at_pos (x, y, path, col, cell_x, cell_y);
		Glib::RefPtr<Gtk::TreeSelection> _slctn = _tview.get_selection();
		select (_tstore->get_iter (path));
		if (!_slctn->is_selected (path))
			_slctn->select (path);
		return true;
	}
	return false;
}


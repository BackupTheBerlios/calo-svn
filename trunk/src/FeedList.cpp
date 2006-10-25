/* \file
 * Implementing the feed hierachy pane.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <iostream>
#include <gtkmm/tooltips.h>
#include <gtkmm/treeselection.h>
#include "AppContext.h"
#include "FeedList.h"
#include "OPMLParser.h"
#include "FetchProtocol.h"

FeedList::FeedList()
: _tview()
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
	_tview.set_headers_visible (true);

//	Glib::RefPtr<Gtk::TreeSelection> _slctn = _tview.get_selection();
//	_slctn->signal_changed().connect (sigc::mem_fun (*this, &FeedList::on_selection_changed));
//	_tview.set_events (Gdk::BUTTON_PRESS_MASK|Gdk::POINTER_MOTION_MASK);
	_tview.signal_button_press_event().connect_notify (sigc::mem_fun (*this, &FeedList::on_tview_button_press));
	_tview.signal_motion_notify_event().connect_notify (sigc::mem_fun (*this, &FeedList::on_tview_motion_notify));

	AppContext::get().get_tooltips()->set_tip (_tview, Glib::ustring(""));
	AppContext::get().get_tooltips()->enable();

	show_all_children();
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

//-Overrides-------------------------------------------------------------
void 
FeedList::on_selection_changed()
{
//	Glib::RefPtr<Gtk::TreeSelection> _slctn = _tview.get_selection();
//	select (_slctn->get_selected());
}

//-Callbacks-------------------------------------------------------------
void
FeedList::on_tview_button_press (GdkEventButton* event)
{
	int x, y, cell_x, cell_y;
	x = static_cast<int> (event->x);
	y = static_cast<int> (event->y);
	Gtk::TreeModel::Path path;
	Gtk::TreeViewColumn *col;
	_tview.get_path_at_pos (x, y, path, col, cell_x, cell_y);
	Glib::RefPtr<Gtk::TreeSelection> _slctn = _tview.get_selection();
	select (_tstore->get_iter (path));
	if (!_slctn->is_selected (path))
		_slctn->select (path);
}

void
FeedList::on_tview_motion_notify (GdkEventMotion* event)
{
	static Gtk::TreeModel::Path _old_path;
	int x, y, cell_x, cell_y;
	x = static_cast<int> (event->x);
	y = static_cast<int> (event->y);
	Gtk::TreeModel::Path path;
	Gtk::TreeViewColumn *col;
	_tview.get_path_at_pos (x, y, path, col, cell_x, cell_y);
	if (_old_path.to_string() == path.to_string())
		return;
	
	Gtk::TreeModel::iterator it = _tstore->get_iter (path);
	Feed* feed = (*it) [_smcol->_col_feed];
	Glib::ustring br = "\n";
	Glib::ustring tipstr = "";
	tipstr += (*it)[_smcol->_col_string] + br; 
	tipstr += (*it)[_smcol->_col_url] + br; 
	tipstr += "Last-Visited: " + feed->get_property ("Last-Visited") + br;
	tipstr += "Last-Modified: " + feed->get_property ("Last-Modified") + br;
	tipstr += "Content-Encoding: " + feed->get_property ("Content-Encoding") + br;
	tipstr += "Content-Language: " + feed->get_property ("Content-Language") + br;
	AppContext::get().get_tooltips()->set_tip (_tview, tipstr);
	AppContext::get().get_tooltips()->enable();
	_old_path = path;
}



/* \file
 * Implementing the feed hierachy pane.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <iostream>
#include <gdk/gdkkeysyms.h>
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
	_tview.signal_key_press_event().connect_notify (sigc::mem_fun (*this, &FeedList::on_tview_key_press));
	_tview.signal_motion_notify_event().connect_notify (sigc::mem_fun (*this, &FeedList::on_tview_motion_notify));

	AppContext::get().get_tooltips()->set_tip (_tview, Glib::ustring(""));
	AppContext::get().get_tooltips()->enable();

	show_all_children();
}

FeedList::~FeedList()
{
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
	if (!_slctn->is_selected (path))
		_slctn->select (path);
	
	Gtk::TreeModel::iterator it = _tstore->get_iter (path);
	Glib::ustring uri = (*it)[_smcol->_col_url];
	Feed* feed = (*it)[_smcol->_col_feed];
	AppContext::get().set_feed (feed);
	if (event->button == 3 || feed->get_items().empty())
		FetchProtocol::get()->run (uri, feed);
	AppContext::get().draw_view();
}

/// Callback: Action to take when row is to be deleted.
/// TODO: does not handle multiple entries for one feed!
void
FeedList::delete_row (const Gtk::TreeModel::iterator& iter)
{
	_tstore->erase (iter);
}

/// Callback to handle keyboard input
void
FeedList::on_tview_key_press (GdkEventKey* event)
{
	if (event->keyval == GDK_Delete)
	{
		Glib::RefPtr<Gtk::TreeSelection> _slctn = _tview.get_selection();
		_slctn->selected_foreach_iter (sigc::mem_fun (*this, 
			&FeedList::delete_row));
	}
}

/// Show feed tooltips when mouse is over the feed list, if so configured.
void
FeedList::on_tview_motion_notify (GdkEventMotion* event)
{
	if (!AppContext::get().get_has_feed_tips())
	{
		AppContext::get().get_tooltips()->unset_tip (_tview);
		return;
	}

	// Only necessary to change tip text when changing path mouse is over
	static Gtk::TreeModel::Path _old_path;
	int x, y, cell_x, cell_y;
	x = static_cast<int> (event->x);
	y = static_cast<int> (event->y);
	Gtk::TreeModel::Path path;
	Gtk::TreeViewColumn *col;
	_tview.get_path_at_pos (x, y, path, col, cell_x, cell_y);
	if (_old_path.to_string() == path.to_string())
		return;
	
	// Build up tip string
	Gtk::TreeModel::iterator it = _tstore->get_iter (path);
	Feed* feed = (*it) [_smcol->_col_feed];
	Glib::ustring br = "\n", prop;
	Glib::ustring tipstr = "";
	tipstr += (*it)[_smcol->_col_string] + br; 
	tipstr += (*it)[_smcol->_col_url];
	
	prop = feed->get_property ("Last-Visited");
	if (!prop.empty())
		tipstr += br + "Last-Visited: " + prop;
	prop = feed->get_property ("Last-Modified");
	if (!prop.empty())
		tipstr += br + "Last-Modified: " + prop;
	prop = feed->get_property ("Content-Encoding");
	if (!prop.empty())
		tipstr += br + "Content-Encoding: " + prop;
	prop = feed->get_property ("Content-Language");
	if (!prop.empty())
		tipstr += br + "Content-Language: " + prop;

	AppContext::get().get_tooltips()->set_tip (_tview, tipstr);
	AppContext::get().get_tooltips()->enable();
	_old_path = path;
}



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


//=HELPERS=============================================================
//
/// Select given TreeNodeChildren in TreeView, set current feed.
void
FeedList::_select (const Gtk::TreeModel::iterator& iter)
{
	Glib::RefPtr<Gtk::TreeSelection> _slctn = _tview.get_selection();
    _slctn->select (get_tmodel()->get_path (iter));
	Feed* feed = (*iter)[_smcol->_col_feed];
	AppContext::get().set_feed (feed);
}

/// Delete selected feeds, select next unselected or none.
/// TODO: does not handle multiple entries for one feed!
void
FeedList::_delete_selected()
{
    typedef std::list<Gtk::TreePath> path_list_t;
    Glib::RefPtr<Gtk::TreeSelection> _slctn = _tview.get_selection();
    path_list_t sel = path_list_t (_slctn->get_selected_rows());
    
    if (sel.empty())
        return;

    // All nodes selected? Then erase all, select nothing.
	Gtk::TreeNodeChildren all = get_tmodel()->children();
    if (sel.size() == all.size())
    {
        _tstore->clear();
	    AppContext::get().set_feed (NULL);
        return;
    }

    // Find first selected node. Do not presume selection is ordered.
    path_list_t::iterator pit;
	Gtk::TreeModel::Children::const_iterator it = all.begin(), next;
    bool first_sel_found = false;
	for (; it != all.end() && !first_sel_found; it++)
	{
        Gtk::TreePath path = get_tmodel()->get_path (it);
        for (pit = sel.begin(); pit != sel.end(); pit++)
            if (path == *pit)
            {
                first_sel_found = true;
                break;
            }
    }
    g_assert (first_sel_found);

    // Find first unselected node (or end()) after first selected.
    while (it != all.end() && _slctn->is_selected (it))
        it++;

    // Delete all selected nodes.
    _slctn->selected_foreach_iter (sigc::hide_return (sigc::mem_fun 
                    (*_tstore.operator->(), &Gtk::TreeStore::erase)));

    // Now select one. There is one guaranteed to be left.
    if (it == all.end())
    {
        // All selected nodes were listed at end. Select last.
        it--;
    }
    _select (it);
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
	{
		FetchProtocol::get()->run (uri, feed);
	}
	AppContext::get().draw_view();
}

/// Callback to handle keyboard input
void
FeedList::on_tview_key_press (GdkEventKey* event)
{
    switch (event->keyval)
    {
    case GDK_Delete: _delete_selected(); break;
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

// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4 :

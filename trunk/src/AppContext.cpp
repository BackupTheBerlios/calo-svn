/* \file
 * Implementation of the AppContext singleton.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <sys/stat.h>
#include <iostream>
#include <gtkmm/tooltips.h>
#include "AppContext.h"
#include "AppWindow.h"
#include "FeedListColumnRecord.h"
#include "FetchAndRenderPipeline.h"
#include "ConfigFile.h"
#include "Item.h"
#include "exceptions.h"
#include "gtkutils.h"

/// The AppContext singleton
static AppContext* _theContext = NULL;


/// Make sure the app context is created once only.
AppContext& AppContext::get()
{
	if (_theContext != NULL)
		return *_theContext;
	_theContext = new AppContext;
	return *_theContext;
}

/// Calls the destructor of the AppContext
void AppContext::destroy()
{
	if (_theContext == NULL)
		return;
	delete _theContext;
	_theContext = NULL;
}

AppContext::AppContext()
{
	_curr_display_type = SIMPLE;
	_has_feed_tips = true;
	_app_x = _app_y = _app_w = _app_h = 0;
	_aw = NULL;
	_curr_feed = NULL;
	_ttips = new Gtk::Tooltips;
	_view_is_opened = false;
	_cfg = new ConfigFile;
	PIXCACHE->add_search_path (std::string (DATADIR) + "/calo/icons");
}

AppContext::~AppContext()
{
	delete _cfg;
}

//-------------------------------------------------------------
/// Initialize app context by setting up file paths and loading the config file.
void 
AppContext::init()
{
	/// Build $HOME/.calo directory path
	_calodir = Glib::build_filename 
		(Glib::get_home_dir(), std::string (".calo"));
	
	/// Create $HOME/.calo if it doesn't exist
	if (mkdir (_calodir.c_str(), 493) < 0 && errno != EEXIST)
		throw FileException();

	/// Build $HOME/.calo/items directory path
	_itemdir = Glib::build_filename 
		(_calodir, std::string ("items"));
	
	/// Create $HOME/.calo/items if it doesn't exist
	if (mkdir (_itemdir.c_str(), 493) < 0 && errno != EEXIST)
		throw FileException();
	
	/// Build $HOME/.calo/config.xml path
	_cfg->init (Glib::ustring (Glib::build_filename 
			(_calodir, std::string ("config.xml"))));
	/// Parse file
	try 
	{
		_cfg->set_substitute_entities (true);
		_cfg->do_parse();
	}
	catch (const xmlpp::exception& ex)
	{
		std::cerr << "parse exception: " << ex.what() << std::endl;
	}
	
	/// Set config vars from config database
	if (_cfg->has ("app_x"))
		_app_x = _cfg->get_i ("app_x");
	if (_cfg->has ("app_y"))
		_app_y = _cfg->get_i ("app_y");
	if (_cfg->has ("app_w"))
		_app_w = _cfg->get_i ("app_w");
	if (_cfg->has ("app_h"))
		_app_h = _cfg->get_i ("app_h");
	if (_cfg->has ("lpane_w"))
		_lpane_w = _cfg->get_i ("lpane_w");
	if (_cfg->has ("has_feed_tips"))
		_has_feed_tips = (_cfg->get_i ("has_feed_tips") != 0);
}

/// Helper for saving the feed properties that gets called for every feed
static bool
save_treenode (const Gtk::TreeModel::Path& path, const Gtk::TreeModel::iterator& it, xmlpp::Element* root)
{
	/// There is only one element per feed
	xmlpp::Element* child = root->add_child ("outline");
	const FeedListColumnRecord& cr = FeedListColumnRecord::get();

	/// Feed name and uri are attributes of the element 
	child->set_attribute ("text", (*it)[cr._col_string]);
	child->set_attribute ("xmlUrl", (*it)[cr._col_url]);

	/// Every property of the feed gets an attribute too
	const Feed* feed = ((*it)[cr._col_feed]);
	const str_str_map_t& map = feed->get_map();
	for (str_str_map_t::const_iterator iit = map.begin(); iit != map.end(); ++iit)
		if (iit->second.length() > 0)
			child->set_attribute (iit->first, iit->second);
	
	return false;
}

/// Helper for saving the feed items that gets called for every feed
/// TODO: This saves once per feed list entry, so could save more than once
/// per feed.
static bool
save_items (const Gtk::TreeModel::Path& path, const Gtk::TreeModel::iterator& it, const std::string& itemdir)
{	
	const FeedListColumnRecord& cr = FeedListColumnRecord::get();
	const Feed* feed = ((*it)[cr._col_feed]);
	const std::string& url = Glib::ustring ((*it)[cr._col_url]);
	feed->save_items (itemdir, url);
	return false;
}

/// Save config file, feed file, feed items.
void 
AppContext::save()
{
	/// Set config database from config vars, save it
	_cfg->set ("app_x", _app_x);
	_cfg->set ("app_y", _app_y);
	_cfg->set ("app_w", _app_w);
	_cfg->set ("app_h", _app_h);
	_cfg->set ("lpane_w", _lpane_w);
	_cfg->set ("has_feed_tips", _has_feed_tips? 1:0);
	_cfg->save();

	/// Build XML document from feeds
	xmlpp::Document doc;
	Glib::RefPtr<Gtk::TreeModel> tmodel = _aw->get_feedlist()->get_tmodel();
	xmlpp::Element* root = doc.create_root_node ("root");
	tmodel->foreach (sigc::bind (sigc::ptr_fun (&save_treenode), root));

	/// Save feeds as XML document
	try
	{
		doc.write_to_file_formatted (get_feeds_filename());
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Exception while writing feeds file: " << ex.what()
			<< std::endl;
	}
	
	/// Save items into items dir
	try
	{
		tmodel->foreach (sigc::bind (sigc::ptr_fun (&save_items), _itemdir));
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Exception while writing items: " << ex.what()
			<< std::endl << std::flush;
	}
}

/// Builds and returns feed file path $HOME/.calo/feeds.xml
const std::string&
AppContext::get_feeds_filename()
{
	static const std::string str (Glib::build_filename 
			(_calodir, std::string ("feeds.xml")));
	return str;
}

/// Queue draw signal with view
void
AppContext::draw_view() const
{
	if (_curr_feed == NULL) 
		return;
	ViewWindow *vw = _aw->get_viewcontainer()->get_viewwindow();
	vw->_darea.draw_buffer();
	vw->_darea.queue_draw();
}

void 
AppContext::set_feed (Feed* theFeed)			
{
	_curr_feed = theFeed;
	draw_view();
}

void
AppContext::fill_curr_item (const Glib::ustring& uri, 
	const Glib::ustring& dump, bool f1, bool f2)
{
	_curr_item->_article = dump;
	draw_view();
	delete _pline;
}

void 
AppContext::set_curr_item (Item* i)
{ 
	_curr_item = i;
	set_display_type (FULL);
	_pline = new FetchAndRenderPipeline;
	_pline->add_uri (i->_link, _curr_item->_title);
	_pline->signal_msg_fetched.connect (sigc::mem_fun (*this,
		&AppContext::fill_curr_item));
	_pline->set_render_to_pdf (false);
	_pline->start();
}

//-------------------------------------------------------------
void 
AppContext::set_appwindow (AppWindow* theWindow) 	
{ 
	_aw = theWindow;
} 

void
AppContext::save_geometry()
{
	int x, y;
	_aw->get_position (x, y);
	Gtk::Allocation all = _aw->get_allocation();
	set_appwindow_pos (x, y, all.get_width(), all.get_height());

	all = _aw->get_feedlist()->get_allocation();
	set_lpanew (all.get_width());
}

void 
AppContext::get_appwindow_pos (unsigned int* x, unsigned int* y, unsigned int* w, unsigned int* h) const
{
	*x = _app_x;
	*y = _app_y;
	*w = _app_w;
	*h = _app_h;
}

void 
AppContext::set_appwindow_pos (unsigned int x, unsigned int y,	unsigned int w, unsigned int h)
{
	_app_x = x;
	_app_y = y;
	_app_w = w;
	_app_h = h;
}

int
AppContext::get_lpanew() const
{
	return _lpane_w;
}

void 
AppContext::set_lpanew (int i)
{
	_lpane_w = i;
}

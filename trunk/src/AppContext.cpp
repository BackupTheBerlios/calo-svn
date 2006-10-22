/* \file
 * Implementation of the AppContext singleton.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <iostream>
#include "AppContext.h"
#include "AppWindow.h"
#include "FeedListColumnRecord.h"
#include "ConfigFile.h"

static AppContext* _theContext = NULL;

/// Make sure the app context is created once only.
AppContext& AppContext::get()
{
	if (_theContext != NULL)
		return *_theContext;
	_theContext = new AppContext;
	return *_theContext;
}

void AppContext::destroy()
{
	if (_theContext == NULL)
		return;
	delete _theContext;
	_theContext = NULL;
}

AppContext::AppContext()
{
	_curr_display_type = SIMPLE_ITEM_DU;
	_app_x = _app_y = _app_w = _app_h = 0;
	_aw = NULL;
	_view_is_opened = false;
	_cfg = new ConfigFile (get_config_filename());
}

AppContext::~AppContext()
{
	delete _cfg;
}

//-------------------------------------------------------------
/// Initialize app context by loading the config file.
void AppContext::init()
{
	_cfg->init();
	try 
	{
		_cfg->set_substitute_entities (true);
		_cfg->parse_file (get_config_filename());
	}
	catch (const xmlpp::exception& ex)
	{
		std::cerr << "parse exception: " << ex.what() << std::endl;
	}
}

/// Helper for saving the feed list that gets called for every feed
static bool
save_treenode (const Gtk::TreeModel::Path& path, const Gtk::TreeModel::iterator& it, xmlpp::Element* root)
{
	xmlpp::Element* child = root->add_child ("outline");
	const FeedListColumnRecord& cr = FeedListColumnRecord::get();
	child->set_attribute ("text", (*it)[cr._col_string]);
	child->set_attribute ("xmlUrl", (*it)[cr._col_url]);
	const Feed* feed = ((*it)[cr._col_feed]);
	const str_str_map_t& map = feed->get_map();
	for (str_str_map_t::const_iterator iit = map.begin(); iit != map.end(); ++iit)
		if (iit->second.length() > 0)
			child->set_attribute (iit->first, iit->second);
	return false;
}

/// Save config file, feed file
void 
AppContext::save()
{
	_cfg->set ("app_x", _app_x);
	_cfg->set ("app_y", _app_y);
	_cfg->set ("app_w", _app_w);
	_cfg->set ("app_h", _app_h);
	_cfg->set ("lpane_w", _lpane_w);
	_cfg->save();

	xmlpp::Document doc;
	Glib::RefPtr<Gtk::TreeModel> tmodel = _aw->get_feedlist()->get_tmodel();
	xmlpp::Element* root = doc.create_root_node ("root");
	tmodel->foreach (sigc::bind (sigc::ptr_fun (&save_treenode), root));

	try
	{
		doc.write_to_file_formatted (get_feeds_filename());
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Exception while writing feeds file: " << ex.what()
			<< std::endl;
	}
}

// TODO: remove hardcoded paths before 1st release
const Glib::ustring&
AppContext::get_feeds_filename()
{
	static Glib::ustring name ("/home/ralf/.calo/feeds.opml");
	return name;
}

const Glib::ustring&
AppContext::get_config_filename()
{
	static Glib::ustring name ("/home/ralf/.calo/config.xml");
	return name;
}

//-------------------------------------------------------------
void 
AppContext::get_appwindow_pos (unsigned int* x, unsigned int* y, unsigned int* w, unsigned int* h)
{
	if (_cfg->has ("app_x"))
		_app_x = _cfg->get_i ("app_x");
	if (_cfg->has ("app_y"))
		_app_y = _cfg->get_i ("app_y");
	if (_cfg->has ("app_w"))
		_app_w = _cfg->get_i ("app_w");
	if (_cfg->has ("app_h"))
		_app_h = _cfg->get_i ("app_h");
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
AppContext::get_lpanew()
{
	if (_cfg->has ("lpane_w"))
		_lpane_w = _cfg->get_i ("lpane_w");
	return _lpane_w;
}

void 
AppContext::set_lpanew (int i)
{
	_lpane_w = i;
}


#ifndef __APP_CONTEXT_H
#define __APP_CONTEXT_H

/* \file
 * Singleton holding info important for the application
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <list>
#include <glibmm/ustring.h>
#include "ItemAccumulator.h"
#include "ItemDisplayUnit.h"

class AppWindow;
class ConfigFile;
class Feed;
class Item;

typedef std::list<Item*> item_list_t;

/// AppContext

class AppContext : public ItemAccumulator
{
public:
	static AppContext& get();
	static void destroy();

	void init();
	void save();
	const Glib::ustring& get_feeds_filename();
	const Glib::ustring& get_config_filename();
	void get_appwindow_pos (unsigned int* x, unsigned int* y,
			unsigned int* w, unsigned int* h);
	void set_appwindow_pos (unsigned int x, unsigned int y,
			unsigned int w, unsigned int h);
	AppWindow* get_appwindow() const 		{ return _aw; }
	void set_appwindow (AppWindow* theWindow) 	{ _aw = theWindow; } 
	Feed* get_feed() const				{ return _curr_feed; }
	void set_feed (Feed* theFeed)			{ _curr_feed = theFeed; }
	int get_lpanew();
	void set_lpanew (int i);
	bool get_view_is_opened() 			{ return _view_is_opened; }
	void set_view_is_opened (bool opened) 		{ _view_is_opened = opened; }
	item_display_unit_t get_display_type()	{ return _curr_display_type; }
	void set_display_type (item_display_unit_t t) { _curr_display_type = t; }
	
	virtual void add_item (Item* i) 		{ _all_items.push_back (i); }

private:
	AppContext();
	~AppContext();
	
	AppWindow		*_aw;
	ConfigFile		*_cfg;
	Feed			*_curr_feed;
	item_display_unit_t 	_curr_display_type;
	item_list_t		_all_items;
	unsigned int 		_app_x, _app_y, _app_w, _app_h, _lpane_w;
	bool			_view_is_opened;
};

#endif

#ifndef __APP_CONTEXT_H
#define __APP_CONTEXT_H

/* \file
 * Singleton holding info important for the application
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <list>
#include <string>
#include <glibmm/ustring.h>
#include "ItemAccumulator.h"
#include "ItemDisplayUnit.h"

class AppWindow;
class ConfigFile;
class FetchAndRenderPipeline;
class Feed;
class Item;
namespace Gtk { class Tooltips; }

typedef std::list<Item*> item_list_t;

/// AppContext

class AppContext : public ItemAccumulator
{
public:
	static AppContext& get();
	static void destroy();

	void init();
	void save();
	const std::string& get_feeds_filename();
	const std::string& get_item_dir() const         { return _itemdir; }
	Gtk::Tooltips* get_tooltips()			{ return _ttips; }
	void save_geometry();
	void get_appwindow_pos (unsigned int* x, unsigned int* y,
			unsigned int* w, unsigned int* h) const;
	void set_appwindow_pos (unsigned int x, unsigned int y,
			unsigned int w, unsigned int h);
	AppWindow* get_appwindow() const 		{ return _aw; }
	void set_appwindow (AppWindow* theWindow);
	void draw_view() const;
	Feed* get_feed() const				{ return _curr_feed; }
	void set_feed (Feed* theFeed);
	int get_lpanew() const;
	void set_lpanew (int i);
	bool get_has_feed_tips() const		     { return _has_feed_tips; }
	void set_has_feed_tips (bool f)			{ _has_feed_tips = f; }
	bool get_view_is_opened() const 	    { return _view_is_opened; }
	void set_view_is_opened (bool opened) 	  { _view_is_opened = opened; }
	item_display_unit_t get_display_type() const { return _curr_display_type; }
	void set_display_type (item_display_unit_t t) { _curr_display_type = t; }
	Item *get_curr_item() const			 { return _curr_item; }
	void set_curr_item (Item* i);
	
	virtual void add_item (Item* i) 		{ _all_items.push_back (i); }
	void fill_curr_item (const Glib::ustring& uri, 
		const Glib::ustring& dump, bool f1, bool f2);

private:
	AppContext();
	~AppContext();

	Gtk::Tooltips		*_ttips;
	AppWindow		*_aw;
	ConfigFile		*_cfg;
	Feed			*_curr_feed;
	FetchAndRenderPipeline  *_pline;
        Item			*_curr_item;
	item_display_unit_t 	_curr_display_type;
	item_list_t		_all_items;
	std::string		_calodir, _itemdir;
	unsigned int 		_app_x, _app_y, _app_w, _app_h, _lpane_w;
	bool			_has_feed_tips, _view_is_opened;
};

#endif

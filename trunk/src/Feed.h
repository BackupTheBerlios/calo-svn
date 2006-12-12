#ifndef __FEED_H
#define __FEED_H

/* \file
 * Main feed datastructure, part of the feed tree which builds the feed list.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <map>
#include <list>
#include <glibmm/ustring.h>
#include "ItemAccumulator.h"


class Feed;

typedef std::map<Glib::ustring,Glib::ustring> str_str_map_t;
typedef std::list<Item*> item_list_t;

/// Feed
/// TODO: delete all feeds
//

class Feed : public ItemAccumulator
{
public:
	Feed();
	virtual ~Feed();

	//Feed *next();
	//Feed *child();

	bool is_unset() const;
	void set_property (const Glib::ustring& key, const Glib::ustring& val);
	const Glib::ustring& get_property (const Glib::ustring& key) const; 
	const str_str_map_t& get_map() const 	{ return _props; }
	item_list_t& get_items() 		{ return _items; }
	void layout_items();

	virtual void add_item (Item*);
	void save_items (const std::string& dir, const std::string& url) const;
	void read_items_from_disk (const Glib::ustring& uri);
    void expire_items (unsigned int ndays);

protected:
	str_str_map_t _props;
	item_list_t _items;
};



#endif


// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4 :

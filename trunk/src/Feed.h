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
#include "Item.h"


class Feed;

typedef std::map<Glib::ustring,Glib::ustring> str_str_map_t;
typedef std::list<Item*> item_list_t;

/// Feed
/// TODO: delete all feeds
//

class Feed
{
public:
	Feed();
	~Feed();

	//Feed *next();
	//Feed *child();

	bool is_unset() const;
	void set_property (const Glib::ustring& key, const Glib::ustring& val);
	const Glib::ustring& get_property (const Glib::ustring& key) const; 
	const str_str_map_t& get_map() const { return _props; }
	item_list_t& get_items() { return _items; }


protected:
	str_str_map_t _props;
	item_list_t _items;
};



#endif

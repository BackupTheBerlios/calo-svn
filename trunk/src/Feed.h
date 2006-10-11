#ifndef __FEED_H
#define __FEED_H

/* \file
 * Main feed datastructure, part of the feed tree which builds the feed list.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <map>
#include <glibmm/ustring.h>

class Feed;

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

	bool is_unset();
	void set_property (const Glib::ustring& key, const Glib::ustring& val);
	const Glib::ustring& get_property (const Glib::ustring& key);

protected:
	std::map<Glib::ustring,Glib::ustring> _props;
};



#endif

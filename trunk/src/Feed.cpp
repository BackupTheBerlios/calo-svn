/* \file
 * Implementing the feed hierachy.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <iostream>
#include "Feed.h"


static char* _possible_props[] = { 
	"Content-Encoding",
	"Content-Language",
	"Content-Type",
	"ETag",
	"Last-Modified",
	"Last-Visited", 
	"Retry-After",
};

Feed::Feed() 
{
	for (unsigned int i=0; i<sizeof(_possible_props)/sizeof(char*); ++i)
		_props[Glib::ustring(_possible_props[i])] = "";
}

Feed::~Feed() {}

//-------------------------------------------------------------------
bool
Feed::is_unset()
const
{
	return _props.find ("ETag")->second.length() == 0;
}

void 
Feed::set_property (const Glib::ustring& key, const Glib::ustring& val)
{
	str_str_map_t::iterator it = _props.find (key);
	if (it == _props.end())
		return;

	it->second = val;
}

const Glib::ustring& 
Feed::get_property (const Glib::ustring& key)
const
{	
	str_str_map_t::const_iterator it = _props.find (key);
	if (it == _props.end())
		return "";

	return it->second;

}


/* \file
 * Implementing the feed hierachy.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <stdio.h>
#include <iostream>
#include <algorithm>
#include <glibmm/miscutils.h>
#include <libxml++/libxml++.h>
#include "Feed.h"
#include "Item.h"


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
	static const Glib::ustring empty = "";
	str_str_map_t::const_iterator it = _props.find (key);
	if (it == _props.end())
		return empty;

	return it->second;
}

//------------------------------------------------------------------
void 
Feed::add_item (Item *i)
{
	_items.push_back (i); 
}

static void
add_rss_item (Item *theItem, xmlpp::Element *theRoot)
{
	xmlpp::Element *item = theRoot->add_child ("item");
	xmlpp::Element *title = item->add_child ("title");
	title->set_child_text (theItem->_title);
	xmlpp::Element *link = item->add_child ("link");
	link->set_child_text (theItem->_link);
	xmlpp::Element *desc = item->add_child ("description");
	desc->set_child_text (theItem->_description);
	xmlpp::Element *pubdate = item->add_child ("pubDate");
	pubdate->set_child_text (theItem->_pubdate);
}

void 
Feed::save_items (const std::string& dir, const std::string& theURL) const
{
	if (_items.empty())
		return;
	
	/// Replace every / with ' in URL.
	/// TODO: xmlpp does not allow / in file names
	std::string url;
	std::replace_copy (theURL.begin(), theURL.end(), 
		std::back_inserter (url), '/', '\'');
	const std::string& path = Glib::build_filename (dir, url);
	remove (path.c_str());

	/// Build XML document from items
	xmlpp::Document doc;
	xmlpp::Element* root = doc.create_root_node ("channel");
	for_each (_items.begin(), _items.end(), 
		std::bind2nd (std::ptr_fun (add_rss_item), root));

	/// Save feeds as XML document
	try
	{
		doc.write_to_file_formatted (path);
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Exception while writing feeds file " 
			<< path << " : " << ex.what()
			<< std::endl << std::flush;
	}
	
}

void
Feed::layout_items()
{
	for (item_list_t::iterator it = _items.begin(); it != _items.end(); ++it)
		(*it)->make_display_unit();
}

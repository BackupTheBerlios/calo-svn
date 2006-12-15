/* \file
 * Implementing the feed hierachy.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <stdio.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <glibmm/miscutils.h>
#include <libxml++/libxml++.h>
#include "AppContext.h"
#include "Feed.h"
#include "Item.h"
#include "RSSParser.h"


static char* _possible_props[] = { 
	"Content-Encoding",
	"Content-Language",
	"Content-Type",
	"ETag",
	"Last-Modified",
	"Last-Visited", 
	"Retry-After",
};

bool _reading_from_disk = false;


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
/// Callback called for every item received from URL or disk
void
Feed::add_item (Item *i)
{
	item_list_t::iterator it = _items.begin();
	for (; it != _items.end(); ++it)
		if ((*it)->_link == i->_link)
		{
			delete *it;
			_items.erase (it);
			break;	/// TODO: remove this and hell breaks loose
		}

	_items.push_back (i);
    
    if (!_reading_from_disk)
    {
        time_t now;
        time (&now);
        char buf[16];
        buf[0] = '\0';
        (void)strftime (buf, 16, "%Y-%m-%d", localtime (&now));
        i->_rcvdate = buf;
    }
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
    xmlpp::Element *rcvdate = item->add_child ("rcvDate");
    rcvdate->set_child_text (theItem->_rcvdate);
}

void 
Feed::save_items (const std::string& dir, const std::string& theURL) const
{
	if (_items.empty())
		return;
	
	/// Replace every / with ' in URL.
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

/// Read all items which were received from a specific URL from disk into feed
void
Feed::read_items_from_disk (const Glib::ustring& theURL)
{
	/// Replace every / with ' in URL.
	std::string url;
	std::replace_copy (theURL.begin(), theURL.end(), 
		std::back_inserter (url), '/', '\'');
	const std::string& dir = AppContext::get().get_item_dir();
	const std::string& path = Glib::build_filename (dir, url);

	std::ifstream in;
	in.open (path.c_str());
	if (in.fail())
		return;
	
	in.close();
	RSSParser parser;
    _reading_from_disk = true;
	parser.add_item_listener (this);
	parser.add_item_listener (&AppContext::get());
	parser.parse_file (path);
    _reading_from_disk = false;
}

void
Feed::layout_items()
{
    for_each (_items.begin(), _items.end(),
            std::mem_fun (&Item::make_display_unit));
}

void 
Feed::expire_items (unsigned int ndays)
{
    remove_if (_items.begin(), _items.end(), 
            std::bind2nd (std::mem_fun (&Item::older_than), ndays));
}

// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4 :

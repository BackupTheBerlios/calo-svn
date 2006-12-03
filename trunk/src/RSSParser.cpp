/* \file
 * Implementation of parsing an RSS file.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#undef DEBUG

#include <iostream>
#include <functional>
#include <glib/gmessages.h>
#include <glibmm/convert.h>
#include "Item.h"
#include "RSSParser.h"
#include "utils.h"

RSSParser::RSSParser() : xmlpp::SaxParser()
{
	_curr_string = NULL;
	_channel = _in_item = false;
}

RSSParser::~RSSParser() {}

//-------------------------------------------------------------
void
RSSParser::parse_memory (const Glib::ustring& str)
{
	_str = str;
	xmlpp::SaxParser::parse_memory (str);
}


void RSSParser::on_start_document()
{
#ifdef DEBUG
	std::cout << "on_start_document()" << std::endl;
#endif
}

void RSSParser::on_end_document()
{
#ifdef DEBUG
	std::cout << "on_end_document()" << std::endl;
#endif
}

void 
RSSParser::on_start_element(const Glib::ustring& theName, const AttributeList& attributes)
{
#ifdef DEBUG
	std::cerr << "----> " << theName << std::endl << std::flush;
#endif
	if (_in_item)
	{
		const Glib::ustring& name = theName.lowercase();
		if (name == "title")
			_curr_string = &_item->_title;
		else if (name == "description")
			_curr_string = &_item->_description;
		else if (name == "link")
			_curr_string = &_item->_link;
		else if (name == "pubdate")
			_curr_string = &_item->_pubdate;
		else if (name == "guid")
			_curr_string = &_item->_guid;
		else _curr_string = NULL;
	}
	else if (theName == "item")
	{
		_in_item = true;
		_item = new Item;
	}
	else if (theName == "channel")
	{
		if (_channel)
			g_warning ("Multiple channel elements: not implemented.\n");
		_channel = true;
	}
}

void 
RSSParser::on_characters(const Glib::ustring& text)
{
	if (_in_item && _curr_string != NULL)
	{
		*_curr_string += text;
	}
}

void 
RSSParser::on_end_element(const Glib::ustring& name)
{
#ifdef DEBUG
	if (_curr_string) 
		std::cerr << conv (*_curr_string) << std::endl << std::flush;
#endif
	_curr_string = NULL;
	if (name == "item")
	{
		_in_item = false;
		for (item_listener_list_t::iterator it = _listeners.begin();
			it != _listeners.end(); ++it)
			(*it)->add_item (_item);
	}
}

void RSSParser::on_comment(const Glib::ustring& text)
{
}

void RSSParser::on_warning(const Glib::ustring& text)
{
#ifdef DEBUG
	std::cout << "on_warning(): " << text << std::endl;
#endif
}

void RSSParser::on_error(const Glib::ustring& text)
{
#ifdef DEBUG
	std::cout << "on_error(): " << text << std::endl;
	std::cout << conv(_str) << std::endl << std::flush;
#endif
}

void RSSParser::on_fatal_error(const Glib::ustring& text)
{
#ifdef DEBUG
	 std::cout << "on_fatal_error(): " << text << std::endl;
#endif
}




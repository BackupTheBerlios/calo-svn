/* \file
 * Implementation of parsing an RSS file.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <iostream>
#include <functional>
#include <glib/gmessages.h>
#include "Item.h"
#include "RSSParser.h"

RSSParser::RSSParser (const Glib::ustring& str)
: xmlpp::SaxParser()
{
	_curr_string = NULL;
	_channel = _in_item = false;
	xmlpp::SaxParser::parse_memory (str);
}

RSSParser::~RSSParser() {}


//-------------------------------------------------------------
void RSSParser::on_start_document()
{
  std::cout << "on_start_document()" << std::endl;
}

void RSSParser::on_end_document()
{
  std::cout << "on_end_document()" << std::endl;
}

void 
RSSParser::on_start_element(const Glib::ustring& name, const AttributeList& attributes)
{
	if (_in_item)
	{
		if (name == "title")
			_curr_string = &_item->_title;
		else if (name == "description")
			_curr_string = &_item->_description;
		else if (name == "link")
			_curr_string = &_item->_link;
		else if (name == "pubDate")
			_curr_string = &_item->_pubdate;
	}
	else if (name == "item")
	{
		_in_item = true;
		_item = new Item;
	}
	if (name == "channel")
	{
		if (_channel)
			g_warning ("Multiple channel elements: not implemented.\n");
		_channel = true;
	}
}

//void
//RSSParser::add_item (_item_listener_t listener_func)
//{
//	listener_func (_item);
//}

void 
RSSParser::on_end_element(const Glib::ustring& name)
{
	if (name == "item")
	{
		_in_item = false;
		_curr_string = NULL;
		for_each (_listeners.begin(), _listeners.end(), 
			std::bind2nd (std::mem_fun (&ItemAccumulator::add_item), 
				_item));
//			bind1st (std::mem_fun (&RSSParser::add_item), this));
	}
}

void 
RSSParser::on_characters(const Glib::ustring& text)
{
	if (_curr_string != NULL)
	{
		*_curr_string = text;
		_curr_string = NULL;
	}
}

void RSSParser::on_comment(const Glib::ustring& text)
{
}

void RSSParser::on_warning(const Glib::ustring& text)
{
  std::cout << "on_warning(): " << text << std::endl;
}

void RSSParser::on_error(const Glib::ustring& text)
{
  std::cout << "on_error(): " << text << std::endl;
}

void RSSParser::on_fatal_error(const Glib::ustring& text)
{
  std::cout << "on_fatal_error(): " << text << std::endl;
}




/* \file
 * Implementation of parsing an XML file into a Gtk tree view.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <iostream>
#include <functional>
#include "FeedFileParser.h"

FeedFileParser::FeedFileParser (Glib::RefPtr<Gtk::TreeStore>& theStore)
: xmlpp::SaxParser()
{
	_store = theStore;
}

FeedFileParser::~FeedFileParser()
{
}

//-------------------------------------------------------------
void FeedFileParser::on_start_document()
{
  std::cout << "on_start_document()" << std::endl;
}

void FeedFileParser::on_end_document()
{
  std::cout << "on_end_document()" << std::endl;
}

void FeedFileParser::on_start_element(const Glib::ustring& name,
                                   const AttributeList& attributes)
{
	if (name == "outline")
	{
		_stringmap.clear();
		_curr_miter = _store->append();
		for (xmlpp::SaxParser::AttributeList::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter)
		{
			_stringmap[iter->name] = iter->value;
// std::cout << "  Attribute " << iter->name << " = " << iter->value << std::endl;
		}
	}
}

void FeedFileParser::set_model (std::pair<Glib::ustring,Glib::ustring> thePair)
{
	(*_curr_miter)[_rec->_col_feed] = new Feed;

	if (thePair.first == "text")
	{
		(*_curr_miter)[_rec->_col_string] = thePair.second;
	}
	if (thePair.first == "xmlUrl")
	{
		(*_curr_miter)[_rec->_col_url] = thePair.second;
	}
}

void FeedFileParser::on_end_element(const Glib::ustring& name)
{
	for_each (_stringmap.begin(), _stringmap.end(),
		bind1st (std::mem_fun (&FeedFileParser::set_model), this));
}

void FeedFileParser::on_characters(const Glib::ustring& text)
{
}

void FeedFileParser::on_comment(const Glib::ustring& text)
{
}

void FeedFileParser::on_warning(const Glib::ustring& text)
{
  std::cout << "on_warning(): " << text << std::endl;
}

void FeedFileParser::on_error(const Glib::ustring& text)
{
  std::cout << "on_error(): " << text << std::endl;
}

void FeedFileParser::on_fatal_error(const Glib::ustring& text)
{
  std::cout << "on_fatal_error(): " << text << std::endl;
}


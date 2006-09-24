/* \file
 * Implementation of parsing an XML file into a Gtk tree view.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <iostream>
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
		for (xmlpp::SaxParser::AttributeList::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter)
		{
			_stringmap[iter->name] = iter->value;
//    std::cout << "  Attribute " << iter->name << " = " << iter->value << std::endl;
		}
	}
}

void FeedFileParser::on_end_element(const Glib::ustring& name)
{
	std::map<Glib::ustring,Glib::ustring>::iterator miter;
	miter = _stringmap.find ("text");
	if (miter != _stringmap.end())
	{
		Gtk::TreeModel::iterator iter = _store->append();
		(*iter)[_rec->_col_string] = miter->second;
	}
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


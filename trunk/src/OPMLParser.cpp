/* \file
 * Implementation of parsing an XML file into a Gtk tree view.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <iostream>
#include <functional>
#include "OPMLParser.h"
#include "Feed.h"

OPMLParser::OPMLParser (Glib::RefPtr<Gtk::TreeStore>& theStore)
: xmlpp::SaxParser()
{
	_store = theStore;
}

OPMLParser::~OPMLParser()
{
}

//-------------------------------------------------------------
void OPMLParser::on_start_document()
{
#ifdef DEBUG
	std::cout << "on_start_document()" << std::endl;
#endif
}

void OPMLParser::on_end_document()
{
#ifdef DEBUG
	std::cout << "on_end_document()" << std::endl;
#endif
}

void OPMLParser::on_start_element(const Glib::ustring& name,
                                   const AttributeList& attributes)
{
	if (name == "outline")
	{
		_stringmap.clear();
		_curr_miter = _store->append();
		(*_curr_miter)[_rec->_col_feed] = new Feed;
		for (xmlpp::SaxParser::AttributeList::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter)
		{
			_stringmap[iter->name] = iter->value;
#ifdef DEBUG
	std::cout << "  Attribute " << iter->name << " = " << iter->value << std::endl;
#endif
		}
	}
}

void OPMLParser::set_model (std::pair<Glib::ustring,Glib::ustring> thePair)
{
	if (thePair.first == "text")
	{
		(*_curr_miter)[_rec->_col_string] = thePair.second;
	}
	else if (thePair.first == "xmlUrl")
	{
		(*_curr_miter)[_rec->_col_url] = thePair.second;
	}
	else
	{
		Feed *fp = (*_curr_miter)[_rec->_col_feed];
		fp->set_property (thePair.first, thePair.second);
	}
}

void OPMLParser::on_end_element(const Glib::ustring& name)
{
	for_each (_stringmap.begin(), _stringmap.end(),
		bind1st (std::mem_fun (&OPMLParser::set_model), this));
}

void OPMLParser::on_characters(const Glib::ustring& text)
{
}

void OPMLParser::on_comment(const Glib::ustring& text)
{
}

void OPMLParser::on_warning(const Glib::ustring& text)
{
	std::cout << "on_warning(): " << text << std::endl;
}

void OPMLParser::on_error(const Glib::ustring& text)
{
	std::cout << "on_error(): " << text << std::endl;
}

void OPMLParser::on_fatal_error(const Glib::ustring& text)
{
	std::cout << "on_fatal_error(): " << text << std::endl;
}


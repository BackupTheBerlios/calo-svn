/* \file
 * Implementation of parsing an XML file into a Gtk tree view.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <iostream>
#include <gtkmm/treestore.h>
#include "FeedFileParser.h"

FeedFileParser::FeedFileParser (Glib::RefPtr<Gtk::TreeStore>& theStore)
: xmlpp::SaxParser()
{
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
  std::cout << "node name=" << name << std::endl;

  // Print attributes:
  for(xmlpp::SaxParser::AttributeList::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter)
  {
    std::cout << "  Attribute " << iter->name << " = " << iter->value << std::endl;
  }
}

void FeedFileParser::on_end_element(const Glib::ustring& name)
{
  std::cout << "on_end_element()" << std::endl;
}

void FeedFileParser::on_characters(const Glib::ustring& text)
{
  std::cout << "on_characters(): " << text << std::endl;
}

void FeedFileParser::on_comment(const Glib::ustring& text)
{
  std::cout << "on_comment(): " << text << std::endl;
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


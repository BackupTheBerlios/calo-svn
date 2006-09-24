#ifndef __FEEDFILE_PARSER_H
#define __FEEDFILE_PARSER_H

/* \file
 * Reading feeds from file into tree model.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <glibmm/ustring.h>
//#include <libxml++/parsers/saxparser.h>
#include <libxml++/libxml++.h>

/// FeedFileParser

class FeedFileParser : public xmlpp::SaxParser
{
public:
	FeedFileParser (Glib::RefPtr<Gtk::TreeStore>& theStore);
	virtual ~FeedFileParser();

protected:
  //overrides:
  virtual void on_start_document();
  virtual void on_end_document();
  virtual void on_start_element(const Glib::ustring& name,
                                const AttributeList& properties);
  virtual void on_end_element(const Glib::ustring& name);
  virtual void on_characters(const Glib::ustring& characters);
  virtual void on_comment(const Glib::ustring& text);
  virtual void on_warning(const Glib::ustring& text);
  virtual void on_error(const Glib::ustring& text);
  virtual void on_fatal_error(const Glib::ustring& text);
};



#endif

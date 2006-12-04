#ifndef __OPML_PARSER_H
#define __OPML_PARSER_H

/* \file
 * Reading feeds from file into tree model.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <map>
#include <glibmm/ustring.h>
#include <gtkmm/treestore.h>
#include <libxml++/parsers/saxparser.h>
#include "FeedListColumnRecord.h"

/// OPMLParser

class OPMLParser : public xmlpp::SaxParser
{
public:
	OPMLParser (Glib::RefPtr<Gtk::TreeStore>& theStore);
	virtual ~OPMLParser();

	void set_column_record (const FeedListColumnRecord* theRec)
		{ _rec = theRec; }

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

	void set_model (std::pair<Glib::ustring,Glib::ustring> thePair);

	std::map<Glib::ustring,Glib::ustring> 	_stringmap;
	const FeedListColumnRecord 		*_rec;
	Glib::RefPtr<Gtk::TreeStore> 		_store;
	Gtk::TreeModel::iterator 		_curr_miter;
};



#endif


// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4 :

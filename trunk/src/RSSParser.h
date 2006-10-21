#ifndef __RSS_PARSER_H
#define __RSS_PARSER_H

/* \file
 * Parsing an RSS file.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <list>
#include <glibmm/ustring.h>
#include <libxml++/parsers/saxparser.h>
#include "ItemAccumulator.h"


//typedef std::map<Glib::ustring,Glib::ustring> str_str_map_t;
typedef std::list<ItemAccumulator*> item_listener_list_t;


/// RSSParser

class RSSParser : public xmlpp::SaxParser
{
public:
	RSSParser();
	virtual ~RSSParser();
	
	Item* get_item() { return _item; }
	void add_item_listener (ItemAccumulator* acc) {_listeners.push_back (acc); }
	virtual void parse_memory (const Glib::ustring& str);

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

// 	str_str_map_t	_stringmap;
	item_listener_list_t _listeners;
	Glib::ustring	*_curr_string, _str;
	Item		*_item;
	bool		_channel, _in_item;
};


#endif

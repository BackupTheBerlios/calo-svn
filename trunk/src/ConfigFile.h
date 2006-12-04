#ifndef __CONFIG_FILE_H
#define __CONFIG_FILE_H

/* \file
 * Interface for implementations rendering HTMLs text-only
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <map>
#include <glibmm/ustring.h>
#include <libxml++/parsers/saxparser.h>


/// ConfigFile

class ConfigFile : public xmlpp::SaxParser
{
public:
	ConfigFile();
	~ConfigFile();

	void init (const Glib::ustring& fname);
	void read();
	void save();
	bool has (const Glib::ustring& prop);
	int get_i (const Glib::ustring& prop);
	void set (const Glib::ustring& prop, int i);
	void do_parse() { parse_file (_fname); }
	

private:
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

	std::map<Glib::ustring,Glib::ustring> _map;
	Glib::ustring _fname, _name, _value;
};



#endif


// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4 :

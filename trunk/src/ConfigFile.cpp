/* \file
 * Handling the config file and content.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <iostream>
#include <exception>
#include <libxml++/document.h>
#include "ConfigFile.h"

ConfigFile::ConfigFile()
{
}

ConfigFile::~ConfigFile()
{
}

//--------------------------------------------------------
void 
ConfigFile::init (const Glib::ustring& fname)
{
	_fname = fname;
	read();
}

void ConfigFile::read()
{
	
}

void ConfigFile::save()
{
	xmlpp::Document doc;
	xmlpp::Element* root = doc.create_root_node ("root");
	std::map<Glib::ustring,Glib::ustring>::iterator it = _map.begin();
	for (; it != _map.end(); ++it)
	{
		xmlpp::Element* child = root->add_child ("property");
		child->set_attribute ("name", it->first);
		child->set_attribute ("value", it->second);
	}

	try
	{
		doc.write_to_file_formatted (_fname);
	}
	catch (const std::exception& ex)
	{
		std::cerr << "Exception while writing config file: " << ex.what()
			<< std::endl;
	}
}

bool ConfigFile::has (const Glib::ustring& prop)
{
	std::map<Glib::ustring,Glib::ustring>::iterator it = _map.find (prop);
	return it != _map.end();
}

int ConfigFile::get_i (const Glib::ustring& prop)
{
	char *tail;
	std::map<Glib::ustring,Glib::ustring>::iterator it = _map.find (prop);
	return (it != _map.end())? strtol ((_map[prop]).c_str(), &tail, 0) : -1;
}

void ConfigFile::set (const Glib::ustring& prop, int i)
{
	char str[16];
	sprintf (str, "%d", i);
	_map[prop] = Glib::ustring (str);
}


//-------------------------------------------------------------
void ConfigFile::on_start_document()
{
#ifdef DEBUG
	std::cout << "on_start_document()" << std::endl;
#endif
}

void ConfigFile::on_end_document()
{
#ifdef DEBUG
	 std::cout << "on_end_document()" << std::endl;
#endif
}

void ConfigFile::on_start_element(const Glib::ustring& name,
                                   const AttributeList& attributes)
{
	if (name == "property")
	{
		for (xmlpp::SaxParser::AttributeList::const_iterator iter = attributes.begin(); iter != attributes.end(); ++iter)
		{
			if (iter->name=="name")
				_name = iter->value;
			if (iter->name=="value")
				_value = iter->value;
		}
	}
}

void ConfigFile::on_end_element(const Glib::ustring& name)
{
std::cerr<<"cfg: "<<_name<<" = "<<_value<<std::endl;
	_map[_name] = _value;
}

void ConfigFile::on_characters(const Glib::ustring& text)
{
}

void ConfigFile::on_comment(const Glib::ustring& text)
{
}

void ConfigFile::on_warning(const Glib::ustring& text)
{
  std::cout << "ConfigFile::on_warning(): " << text << std::endl;
}

void ConfigFile::on_error(const Glib::ustring& text)
{
  std::cout << "ConfigFile::on_error(): " << text << std::endl;
}

void ConfigFile::on_fatal_error(const Glib::ustring& text)
{
  std::cout << "ConfigFile::on_fatal_error(): " << text << std::endl;
}



// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4 :

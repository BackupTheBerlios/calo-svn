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

ConfigFile::ConfigFile (const Glib::ustring& fname)
{
	_fname = fname;
}

ConfigFile::~ConfigFile()
{
}

//--------------------------------------------------------
void ConfigFile::init()
{
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
	return false;
}

int ConfigFile::get_i (const Glib::ustring& prop)
{
}

void ConfigFile::set (const Glib::ustring& prop, int i)
{
	char str[16];
	sprintf (str, "%d", i);
	_map[prop] = Glib::ustring (str);
}


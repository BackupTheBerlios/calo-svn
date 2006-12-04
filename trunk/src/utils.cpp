/* \file
 * Implementations of small utility functions.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <time.h>
#include <glibmm/convert.h>


void sleepms (unsigned long ms)
{
	struct timespec ti;
	ti.tv_sec = ms / 1000L;
	ti.tv_nsec = (ms - 1000L*int(ms/1000L)) * 1000000L;
	nanosleep (&ti, &ti);
}

std::string conv (const std::string& str)
{
	const std::string& to = "";
	const std::string& from = "UTF-8";
	const Glib::ustring& fallback = "?";
	
	return Glib::convert_with_fallback (str, to, from, fallback);
}

void remove_markup (const Glib::ustring& in, Glib::ustring& out)
{
	out.clear();
	bool inside_markup = false;
	//Glib::ustring::const_iterator it;
	//it = in.begin();
	const char *it = in.c_str();
	do 
	{
		if (!inside_markup)
		{
			if (*it == '<')
			{
				inside_markup = true;
				++it;
			}
			else if (*it == '&')
			{
				++it;
				if (*it == 'l')
				{
					++it;
					if (*it == 't')
					{
						++it;
						if (*it == ';')
						{
							inside_markup = true;
							++it;
						}
						else 
							out += "&lt";
					}
					else 
						out += "&l";
				}
				else
					out += '&';
			}
			else
			{
				out += *it;
				++it;
			}
		}
		else
		{
			if (*it == '>')
			{
				inside_markup = false;
				++it;
			}
			else if (*it=='&' && *(++it)=='g' && *(++it)=='t' && *(++it)==';')
			{
				inside_markup = false;
				++it;
			}
			else ++it;
		}
	}
	while (*it != '\0');
}




// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4 :

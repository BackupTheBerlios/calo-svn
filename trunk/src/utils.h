#ifndef __UTILS_H
#define __UTILS_H

/* \file
 * Declarations for small utility functions.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <string>
#include <glibmm/ustring.h>

extern void sleepms (unsigned long ms);
extern std::string conv (const std::string& str);
extern void remove_markup (const Glib::ustring&, Glib::ustring&);

#endif


// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4 :

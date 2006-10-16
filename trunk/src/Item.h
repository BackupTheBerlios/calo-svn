#ifndef __ITEM_H
#define __ITEM_H

/* \file
 * Declaration of RSS Item class.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <glibmm/ustring.h>

/// Item

class Item
{
public:
	Glib::ustring _title, _description, _link, _pubdate;
};

#endif

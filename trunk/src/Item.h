#ifndef __ITEM_H
#define __ITEM_H

/* \file
 * Declaration of RSS Item class.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <glibmm/ustring.h>

class ItemDisplayUnit;


/// Item

class Item
{
public:
	Item() : _ensured(false) { _simple_du = _normal_du = _full_du = NULL; }
	~Item();
	
	void make_display_unit();
	ItemDisplayUnit* get_display_unit();
	void reset_display_unit();

	Glib::ustring _title, _description, _link, _pubdate, _guid, _article;

private:
	void ensure_integrity();

	ItemDisplayUnit *_simple_du, *_normal_du, *_full_du;
	bool _ensured;
};

#endif


// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4 :

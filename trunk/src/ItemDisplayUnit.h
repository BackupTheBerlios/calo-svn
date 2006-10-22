#ifndef __ITEM_DISPLAYUNIT_H
#define __ITEM_DISPLAYUNIT_H

/* \file
 * The rectangle containing output for an item, dependent on output method.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <glibmm/ustring.h>

class Item;
class ItemDisplayUnit;

typedef enum { SIMPLE_ITEM_DU } item_display_unit_t;

/// ItemDisplayUnit

class ItemDisplayUnit
{
public:
	virtual ~ItemDisplayUnit();

	double get_pos_y() const 	{ return _pos_y; }
	double get_height() const 	{ return _height; }
	void set_item_changed() 	{ _item_changed = true; }
	bool has_layout() const		{ return _has_layout; }

	virtual void layout() = 0;
	virtual void render() = 0;

protected:
	Item 	*_item;
	double 	_pos_y, _height;
	bool 	_item_changed, _has_layout;
};

#endif

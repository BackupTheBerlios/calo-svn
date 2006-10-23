#ifndef __ITEM_DISPLAYUNIT_H
#define __ITEM_DISPLAYUNIT_H

/* \file
 * The rectangle containing output for an item, dependent on output method.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <glibmm/ustring.h>
#include <cairomm/context.h>

class Item;
class ItemDisplayUnit;

typedef enum { SIMPLE_ITEM_DU } item_display_unit_t;

/// ItemDisplayUnit

class ItemDisplayUnit
{
public:
	virtual ~ItemDisplayUnit();

	double get_pos_y() const 	{ return _pos_y; }
	void set_pos_y (double y)	{ _pos_y = y; }
	double get_height() const 	{ return _height; }

	virtual void layout() = 0;
	virtual void render (const Cairo::RefPtr<Cairo::Context>&, double, double) = 0;

protected:
	Item 	*_item;
	double 	_pos_y, _height;
};

#endif

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

typedef enum { NONE=0, SIMPLE, NORMAL, FULL } item_display_unit_t;

/// ItemDisplayUnit

class ItemDisplayUnit
{
public:
        ItemDisplayUnit() : _item (NULL), _height(0), _has_layout(false) {}
	virtual ~ItemDisplayUnit();

	double get_pos_y() const 	{ return _pos_y; }
	void set_pos_y (double y)	{ _pos_y = y; }
	double get_height() const 	{ return _height; }
	double get_width() const        { return _width; }
	void set_layout_flag (bool f)	{ _has_layout = f; }

	virtual void layout(const Cairo::RefPtr<Cairo::Context>&) = 0;
	virtual void render (const Cairo::RefPtr<Cairo::Context>&, double, double) = 0;

protected:
	Item 	*_item;
	double 	_pos_y, _height, _width;
	bool _has_layout;
};

#endif

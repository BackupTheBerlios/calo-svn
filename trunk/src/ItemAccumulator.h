#ifndef __ITEM_ACCUMULATOR_H
#define __ITEM_ACCUMULATOR_H

/* \file
 * Abstract interface for classes having an add_item() member func.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

class Item;

class ItemAccumulator
{
public:
	virtual ~ItemAccumulator() {}
	virtual void add_item (Item*) = 0;
};



#endif

#ifndef __SIMPLE_ITEM_DU_H
#define __SIMPLE_ITEM_DU_H

/* \file
 * Declaration of the item display unit containing the item header only.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include "ItemDisplayUnit.h"

class Item;


/// SimpleItemDU

class SimpleItemDU : public ItemDisplayUnit
{
public:
	SimpleItemDU (Item*);
	virtual ~SimpleItemDU();
	virtual void layout();
	virtual void render();
};

#endif

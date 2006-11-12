#ifndef __SIMPLE_ITEM_DU_H
#define __SIMPLE_ITEM_DU_H

/* \file
 * Declaration of the item display unit displaying the item header only.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <pangomm/layout.h>
#include "ItemDisplayUnit.h"

class Item;


/// SimpleItemDU

class SimpleItemDU : public ItemDisplayUnit
{
public:
	SimpleItemDU (Item*);
	virtual ~SimpleItemDU();
	virtual void layout (const Cairo::RefPtr<Cairo::Context>&);
	virtual void render (const Cairo::RefPtr<Cairo::Context>& cctx, double x, double y);

private:
	Glib::RefPtr<Pango::Layout>	_play;
	Item 				*_item;
};

#endif

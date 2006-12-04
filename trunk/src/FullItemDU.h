#ifndef __FULL_ITEM_DU_H
#define __FULL_ITEM_DU_H

/* \file
 * Declaration of the item display unit displaying the full item.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <pangomm/layout.h>
#include "ItemDisplayUnit.h"

class Item;
class ViewWindow;


/// FullItemDU

class FullItemDU : public ItemDisplayUnit
{
public:
	FullItemDU (Item*);
	virtual ~FullItemDU();
	virtual void layout (const Cairo::RefPtr<Cairo::Context>&);
	virtual void render (const Cairo::RefPtr<Cairo::Context>& cctx, double x, double y);

private:
	Glib::RefPtr<Pango::Layout>	_play;
	Item 				*_item;
	ViewWindow			*_vw;
};

#endif


// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4 :

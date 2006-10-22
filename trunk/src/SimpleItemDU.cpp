/* \file
 * Implementation of displaying an item on-screen by showing its header only.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <pango/pangocairo.h>
#include <pango/pango-types.h>
#include <pango/pango-layout.h>

#include "AppContext.h"
#include "AppWindow.h"
#include "ViewContainer.h"
#include "ViewWindow.h"
#include "SimpleItemDU.h"
#include "Item.h"


SimpleItemDU::SimpleItemDU (Item *theItem)
{
	_item = theItem;
}

SimpleItemDU::~SimpleItemDU()
{
}

//---------------------------------------------------------------------
void
SimpleItemDU::layout()
{
	PangoLayout* play = pango_cairo_create_layout (AppContext::get().get_appwindow()->get_viewcontainer()->get_viewwindow()->get_cairo_context()->cobj());

}

void SimpleItemDU::render()
{
}



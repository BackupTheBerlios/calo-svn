/* \file
 * Displaying an item on-screen, abstract base class, necessary code
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <glib/gerror.h>
#include "ItemDisplayUnit.h"
#include "SimpleItemDU.h"
#include "exceptions.h"


ItemDisplayUnit* ItemDisplayUnit::create (item_display_unit_t type)
{
	ItemDisplayUnit* du;

	switch (type)
	{
	case SIMPLE_ITEM_DU: du = new SimpleItemDU;
	
	default: throw MiscException ("ItemDisplayUnit ctor: can't happen");
	}

	du->_item = NULL;
	du->_has_layout = false;
	return du;
}

ItemDisplayUnit::~ItemDisplayUnit()
{
}



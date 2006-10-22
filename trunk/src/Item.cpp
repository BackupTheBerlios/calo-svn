/* \file
 * Implementing Item.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <iostream>
#include "AppContext.h"
#include "Item.h"
#include "ItemDisplayUnit.h"
#include "SimpleItemDU.h"
#include "exceptions.h"


Item::~Item() 
{ 
	if (_simple_du != NULL) 
	{ 
		delete _simple_du; 
		_simple_du = NULL; 
	} 
}

//------------------------------------------------------------
ItemDisplayUnit* 
Item::get_display_unit()
{
	switch (AppContext::get().get_display_type())
	{
	case SIMPLE_ITEM_DU: return _simple_du;
	default: throw MiscException ("Item::get_display-unit(): can't happen");
	}

	return NULL;
}

void 
Item::make_display_unit()
{
	switch (AppContext::get().get_display_type())
	{
	case SIMPLE_ITEM_DU: 
		if (_simple_du == NULL) 
			_simple_du = new SimpleItemDU (this);
		_simple_du->layout();
		break;

	default: throw MiscException ("Item::get_display-unit(): can't happen");
	}

}


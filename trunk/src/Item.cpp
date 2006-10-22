/* \file
 * Implementing Item.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <iostream>
#include "Item.h"
#include "ItemDisplayUnit.h"

Item::~Item() 
{ 
	if (_dunit != NULL) 
	{ 
		delete _dunit; 
		_dunit = NULL; 
	} 
}



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
#include "NormalItemDU.h"
#include "FullItemDU.h"
#include "exceptions.h"
#include "utils.h"


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
/// TODO: replace with curr_du and inheritance
	switch (AppContext::get().get_display_type())
	{
	case SIMPLE: return _simple_du;
	case NORMAL: return _normal_du;
	case FULL:   return _full_du;
	default: throw MiscException ("Item::get_display-unit(): can't happen");
	}

	return NULL;
}

void 
Item::reset_display_unit() 
{ 
	ItemDisplayUnit *du = get_display_unit();
	if (du)
		du->set_layout_flag (false); 
}

void
Item::ensure_integrity()
{
	if (_ensured) 
		return;
	static const Glib::ustring _empty("Empty title");
	if (_link.empty() && !_guid.empty())
		_link = _guid;
	if (_title.empty())
		_title = _empty;
	const Glib::ustring s = _description;
	remove_markup (s, _description);
	_ensured = true;
}

void 
Item::make_display_unit()
{
	switch (AppContext::get().get_display_type())
	{
	case SIMPLE: 
		if (_simple_du == NULL) 
		{
			ensure_integrity();
			_simple_du = new SimpleItemDU (this);
		}
		break;
	case NORMAL: 
		if (_normal_du == NULL) 
		{
			ensure_integrity();
			_normal_du = new NormalItemDU (this);
		}
		break;
	case FULL: 
		if (_full_du == NULL) 
		{
			ensure_integrity();
			_full_du = new FullItemDU (this);
		}
		break;

	default: throw MiscException ("Item::get_display-unit(): can't happen");
	}

}



// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4 :

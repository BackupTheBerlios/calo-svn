/* \file
 * Implementing Item.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <iostream>
#include <glibmm/date.h>
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
	if (_normal_du != NULL) 
	{ 
		delete _normal_du; 
		_normal_du = NULL; 
	} 
	if (_full_du != NULL) 
	{ 
		delete _full_du; 
		_full_du = NULL; 
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

    bool rd_is_valid = false;
    if (!_rcvdate.empty())
    {
        Glib::Date my;
        int y, m, d;
        sscanf (_rcvdate.c_str(), "%d-%d-%d", &y, &m, &d);
        my.set_day (d);
        my.set_year (y);
        my.set_month (Glib::Date::JANUARY);
        my.add_months (m-1);
        rd_is_valid = my.valid();
    }
    if (!rd_is_valid)
    {
        g_warning ("Invalid date: %s ... fixing", _rcvdate.c_str());
        Glib::Date now;
        now.set_time_current();
        _rcvdate = now.format_string ("%Y-%m-%d");
    }
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

/// TODO accelerate by giving exp Date as parameter
bool 
Item::older_than (unsigned int ndays)
{
    Glib::Date my, now;
    now.set_time_current();
    now.subtract_days (ndays);
    
    /// TODO: duplicated from ensure_integrity() above
    int y, m, d;
    sscanf (_rcvdate.c_str(), "%d-%d-%d", &y, &m, &d);
    my.set_day (d);
    my.set_year (y);
    my.set_month (Glib::Date::JANUARY);
    my.add_months (m-1);
    
    if (!my.valid())
    {
        g_warning (_("In Item::older_than(): Invalid date: %s"), _rcvdate.c_str());
        return true;
    }
    return my < now;
}


// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4 :

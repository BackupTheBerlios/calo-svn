/* \file
 * Implementation of the FeedListColumnRecord singleton.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include "FeedListColumnRecord.h"

static FeedListColumnRecord* _theColumnRcord = NULL;

FeedListColumnRecord& FeedListColumnRecord::get()
{
	if (_theColumnRcord != NULL)
		return *_theColumnRcord;
	_theColumnRcord = new FeedListColumnRecord;
	return *_theColumnRcord;
}

FeedListColumnRecord::FeedListColumnRecord() 
{ 
	add (_col_string); 
	add (_col_url);
	add (_col_feed);
}




// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4 :

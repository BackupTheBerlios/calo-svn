/* \file
 * Implementing the Expire Protocol.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include "ExpireProtocol.h"
#include "AppContext.h"
#include "AppWindow.h"
#include "FeedListColumnRecord.h"
#include "Feed.h"


namespace ExpireProtocol {

/// Helper for expiring items which is called per feed
/// TODO: do not call expire_items() twice per feed, see also AppContext::save()
static bool
expire_in_feed (const Gtk::TreeModel::Path& path, const Gtk::TreeModel::iterator& it)
{
	const FeedListColumnRecord& cr = FeedListColumnRecord::get();
	Feed* feed = ((*it)[cr._col_feed]);
    const unsigned int days = AppContext::get().get_expire();
	feed->expire_items (days);
	return false;
}

void
do_expire()
{
    AppWindow *aw = AppContext::get().get_appwindow();
	Glib::RefPtr<Gtk::TreeModel> tmodel = aw->get_feedlist()->get_tmodel();
	tmodel->foreach (sigc::ptr_fun (&expire_in_feed));
    AppContext::get().draw_view();
}


} // namespace

// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4 :

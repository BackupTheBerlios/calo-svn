/* \file
 * Implementing the Expire Dialog.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <list>
#include <glibmm/i18n.h>
#include <gtkmm/stock.h>
#include <gtkmm/box.h>
#include "ExpireDialog.h"
#include "AppContext.h"
#include "gtkutils.h"

typedef std::list<const Gtk::Widget*> cwidget_list_t;

ExpireDialog::ExpireDialog()
    : Gtk::Dialog(_("Expire Items"), true, true), 
    _spinner (1.0, 0),
    _adj (0, 0, 30, 1, 1, 1),
    _label1 (_("Minimum age of\nitems to be expired:")),
    _label2 (_("days"))
{
}

void
ExpireDialog::init()
{
    _spinner.set_numeric (true);
    _spinner.set_digits (1);
    _spinner.set_range (0, 30);
    _spinner.set_snap_to_ticks();
    _spinner.set_adjustment (_adj);
    _spinner.set_value (AppContext::get().get_expire());

    Gtk::VBox *vbox = get_vbox();
    vbox->pack_start (_label1, Gtk::PACK_SHRINK, 10);
    _box.add (*new Gtk::HBox);
    _box.pack_start (_spinner, Gtk::PACK_SHRINK, 10);
    _box.pack_start (_label2, Gtk::PACK_SHRINK, 0);
    _box.add (*new Gtk::HBox);
    vbox->pack_start (_box, Gtk::PACK_EXPAND_PADDING, 10);

    add_button (_("Do expire"), 1);
    add_button (Gtk::Stock::CANCEL, 0);

    /// Move focus to Do button
    cwidget_list_t list = get_action_area()->get_children();
    for (cwidget_list_t::iterator it = list.begin(); it != list.end(); ++it)
        if (get_response_for_widget (**it) == 1)
        { const_cast<Widget&>(**it).grab_focus(); break; }
    
    _changed_connection = _adj.signal_value_changed().connect (
            sigc::mem_fun (*this, &ExpireDialog::on_value_changed));
    
    show_all();
}

ExpireDialog::~ExpireDialog()
{
}

void
ExpireDialog::on_value_changed()
{
    AppContext::get().set_expire (static_cast<unsigned int> (_adj.get_value()));
}

// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4 :

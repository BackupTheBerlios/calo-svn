/* \file
 * Implementing the Expire Dialog.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <glibmm/i18n.h>
#include <gtkmm/stock.h>
#include "ExpireDialog.h"


ExpireDialog::ExpireDialog()
    : Gtk::Dialog(_("Expire Items"), true), 
    _spinner (1.0, 1),
    _label1 (_("Minimum age of\nitems to be expired:")),
    _label2 (_("days"))
{
    add (_label1);
    _box.add (_spinner);
    _box.add (_label2);
    add (_box);
    add_button (Gtk::Stock::YES, 1);
    add_button (Gtk::Stock::NO,  0);
    
    run();
}

ExpireDialog::~ExpireDialog()
{
}

// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4 :

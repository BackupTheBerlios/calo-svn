#ifndef __EXPIRE_DIALOG_H
#define __EXPIRE_DIALOG_H

/* \file
 * Declaration of Expire Dialog.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <gtkmm/dialog.h>
#include <gtkmm/spinbutton.h>
#include <gtkmm/adjustment.h>
#include <gtkmm/label.h>


/// ExpireDialog

class ExpireDialog : public Gtk::Dialog
{
public:
    ExpireDialog();
    virtual ~ExpireDialog();
    void init();

private:
    Gtk::SpinButton _spinner;
    Gtk::HBox       _box;
    Gtk::Adjustment _adj;
    Gtk::Label      _label1, _label2;

    void on_value_changed();
    sigc::connection _changed_connection;
};

#endif


// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4 :

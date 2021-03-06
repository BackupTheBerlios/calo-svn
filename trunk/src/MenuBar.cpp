/* \file
 * Code that creates the menu bar.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <iostream>
#include <glibmm/i18n.h>
#include <gtkmm/main.h>
#include <gtkmm/actiongroup.h>
#include <gtkmm/toggleaction.h>
#include <gtkmm/stock.h>
#include <gtkmm/window.h>
#include "MenuBar.h"
#include "AppContext.h"
#include "ExpireDialog.h"
#include "ExpireProtocol.h"


static MenuBar* _theContext = NULL;

Gtk::Widget* MenuBar::create (Gtk::Window* theWindow)
{
	if (_theContext == NULL)
		_theContext = new MenuBar (theWindow);
	Gtk::Widget* w = _theContext->_uim->get_widget ("/MenuBar");
	return w;
}

MenuBar::MenuBar (Gtk::Window* theWindow)
{
	_ag = Gtk::ActionGroup::create();

	//File menu:
	_ag->add( Gtk::Action::create("FileNew", 
			Gtk::Stock::NEW, _("_New Feed List"), _("Clear the feed list")),
		sigc::mem_fun(*this, &MenuBar::on_menu_file_new) );
	_ag->add( Gtk::Action::create("FileSaveAsDefault", 
			Gtk::Stock::SAVE, _("_Save List as Default"), _("Save feed list as default feed file")),
		sigc::mem_fun(*this, &MenuBar::on_menu_file_save_as_default) );
	_ag->add( Gtk::Action::create("FileQuit", 
			Gtk::Stock::QUIT, _("_Quit"), _("Leave the program")),
		sigc::mem_fun(*this, &MenuBar::on_menu_file_quit) );

	_ag->add( Gtk::Action::create("FileMenu", _("File")) );

	//Edit menu:
	_ag->add( Gtk::Action::create("EditMenu", _("Edit")) );
	_ag->add( Gtk::Action::create("EditCopy", Gtk::Stock::COPY),
    		sigc::mem_fun(*this, &MenuBar::on_menu_others) );
	_ag->add( Gtk::Action::create("EditPaste", Gtk::Stock::PASTE),
		sigc::mem_fun(*this, &MenuBar::on_menu_others) );
	_ag->add( Gtk::Action::create("EditFeed", _("Edit selected feed")), Gtk::AccelKey("<control>E"),
		sigc::mem_fun(*this, &MenuBar::on_menu_others) );
	_ag->add( Gtk::Action::create("Expire", _("Expire items")), Gtk::AccelKey("<control>X"),
		sigc::mem_fun(*this, &MenuBar::on_menu_expire) );

	//Options menu:
	_ag->add( Gtk::Action::create("OptionsMenu", _("Options")) );
	_ag->add( Gtk::ToggleAction::create("OptFeedtips", _("Show feed tooltips"), _("If on, hovering over a feed in the feed list\nwill show all its properties."), AppContext::get().get_has_feed_tips()),
    		sigc::mem_fun(*this, &MenuBar::on_menu_feed_tooltips) );

	//Help menu:
	_ag->add( Gtk::Action::create("HelpMenu", _("Help")) );
	_ag->add( Gtk::Action::create("HelpAbout", Gtk::Stock::HELP),
		sigc::mem_fun(*this, &MenuBar::on_menu_others) );

	_uim = Gtk::UIManager::create();
	_uim->insert_action_group (_ag);
	theWindow->add_accel_group (_uim->get_accel_group());

	//Layout the actions in the menubar
  
	Glib::ustring ui_info = 
        "<ui>"
        "  <menubar name='MenuBar'>"
        "    <menu action='FileMenu'>"
        "      <menuitem action='FileNew'/>"
        "      <menuitem action='FileSaveAsDefault'/>"
        "      <separator/>"
        "      <menuitem action='FileQuit'/>"
        "    </menu>"
        "    <menu action='EditMenu'>"
        "      <menuitem action='EditCopy'/>"
        "      <menuitem action='EditPaste'/>"
        "      <menuitem action='EditFeed'/>"
        "      <menuitem action='Expire'/>"
        "    </menu>"
        "    <menu action='OptionsMenu'>"
        "      <menuitem action='OptFeedtips'/>"
        "    </menu>"
        "    <menu action='HelpMenu'>"
        "      <menuitem action='HelpAbout'/>"
        "    </menu>"
        "  </menubar>"
        "</ui>";

#ifdef GLIBMM_EXCEPTIONS_ENABLED
	try
	{      
		_uim->add_ui_from_string(ui_info);
	}
	catch(const Glib::Error& ex)
	{
		std::cerr << "building menus failed: " <<  ex.what();
	}
#else
	std::auto_ptr<Glib::Error> ex;
	_uim->add_ui_from_string(ui_info, ex);
	if(ex.get())
	{ 
		std::cerr << "building menus failed: " <<  ex->what();
	}
#endif //GLIBMM_EXCEPTIONS_ENABLED
} 

void MenuBar::on_menu_file_new()
{
}

void MenuBar::on_menu_file_save_as_default()
{
}

void MenuBar::on_menu_file_quit()
{
	AppContext::get().save_geometry();
	Gtk::Main::quit();
}

void MenuBar::on_menu_edit_copy()
{
}

void MenuBar::on_menu_edit_paste()
{
}

void MenuBar::on_menu_edit_feed()
{
}

void MenuBar::on_menu_expire()
{
    ExpireDialog dialog;
    dialog.init();
    int r = dialog.run();
    if (r > 0)
        ExpireProtocol::do_expire();
}

void MenuBar::on_menu_feed_tooltips()
{
	AppContext& ac = AppContext::get();
	ac.set_has_feed_tips (!ac.get_has_feed_tips());
}

void MenuBar::on_menu_help_about()
{
}

void MenuBar::on_menu_others()
{
}


// vim: filetype=cpp:expandtab:shiftwidth=4:tabstop=4:softtabstop=4 :

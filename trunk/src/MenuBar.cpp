/* \file
 * Code that creates the menu bar.
 *
 * Copyright (c) 2006 R Stephan <ralf@ark.in-berlin.de>
 * Released under GNU GPL2, read the file 'COPYING' for more information.
 */

#include <iostream>
#include <gtkmm/main.h>
#include <gtkmm/actiongroup.h>
#include <gtkmm/stock.h>
#include <gtkmm/window.h>
#include "MenuBar.h"
#include "AppContext.h"


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
			Gtk::Stock::NEW, "_New Feed List", "Clear the feed list"),
		sigc::mem_fun(*this, &MenuBar::on_menu_file_new) );
	_ag->add( Gtk::Action::create("FileSaveAsDefault", 
			Gtk::Stock::SAVE, "_Save List as Default", "Save feed list as default feed file"),
		sigc::mem_fun(*this, &MenuBar::on_menu_file_save_as_default) );
	_ag->add( Gtk::Action::create("FileQuit", 
			Gtk::Stock::QUIT, "_Quit", "Leave the program"),
		sigc::mem_fun(*this, &MenuBar::on_menu_file_quit) );

	_ag->add( Gtk::Action::create("FileMenu", "File") );

	//Edit menu:
	_ag->add( Gtk::Action::create("EditMenu", "Edit") );
	_ag->add( Gtk::Action::create("EditCopy", Gtk::Stock::COPY),
    		sigc::mem_fun(*this, &MenuBar::on_menu_others) );
	_ag->add( Gtk::Action::create("EditPaste", Gtk::Stock::PASTE),
		sigc::mem_fun(*this, &MenuBar::on_menu_others) );
	_ag->add( Gtk::Action::create("EditFeed", "Edit the selected feed entry"), Gtk::AccelKey("<control>E"),
		sigc::mem_fun(*this, &MenuBar::on_menu_others) );

	//Help menu:
	_ag->add( Gtk::Action::create("HelpMenu", "Help") );
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
	AppContext::get().save();
	AppContext::destroy();
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

void MenuBar::on_menu_help_about()
{
}

void MenuBar::on_menu_others()
{
}